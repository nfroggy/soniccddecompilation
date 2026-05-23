# Backend Unit Test Plan

This plan covers the backend DLL sources: every source file that is compiled into
the CMake shared-library variants, excluding `src/frontend/` and `src/winexe/`.
The target outcome is a test suite that characterizes the current behavior with
exact integer, fixed-point, palette, grid, and sprite-call assertions, and then
enforces 100% line/function/branch coverage for those backend sources.

The original backend source files must remain untouched. Do not edit production
files to expose internals, add test-only branches, rename symbols, change
linkage, add headers, or make behavior easier to observe. All test access must
come from separate test code, the existing public/exported entry points, existing
global symbols, callbacks, generated build metadata, debugger/coverage metadata,
white-box inclusion of unchanged `.c` files, or linker/compiler options that do
not alter the source files.

This pass is characterization-only. Treat the original code as read-only and do
not fix suspected bugs, even when a calculation looks wrong or an object is a
subpixel out of place. Document those findings in
`docs/backend_potential_bugs.md` with the exact test case, observed behavior, and
why it looks suspicious. The tests should still preserve the observed behavior
until a later, separate bug-fix pass deliberately changes it.

The required compatibility target is 32-bit MSVC. The tests must pass when the
backend code is compiled with MSVC for Win32/x86, and the test CMake path should
not configure or build these backend tests as 64-bit/x64 at this point.
The long-term purpose of this characterization pass is to make a later 64-bit
port safer by revealing behavior that accidentally depends on 32-bit execution.
For that reason, tests should focus on observable game behavior rather than ABI
facts that are expected to change during the port.

## Scope

Use `CMakeLists.txt` as the source of truth for "used in backend DLLs". The
backend variants are the entries in `SONICCD_ALL_VARIANTS`:

- Level DLLs: `R11A` through `R83D`.
- Special/gameplay DLLs: `SPECIAL`, `WARP`.
- Title/menu DLLs: `BESTTIME`, `OPENING`, `PLANET`, `SAVEDATA`, `SOUNDTST`,
  `STAGETST`, `TA`, `THANKS`, `VISUALMD`.

Do not include:

- `src/frontend/**`
- `src/winexe/**`
- Backend-looking files that are not referenced by any active CMake variant,
  unless they are deliberately added to a backend target first.

The first deliverable should be an automatically generated inventory:

- Variant -> source files.
- Source file -> variants that compile it.
- Source file -> compile definitions used by each variant.
- Whether the source file contains executable code or only static/global data.
- Current coverage percentage for each file.

This inventory should be committed as generated output or produced by a script in
`tests/backend/tools/`, so a CMake source-list change cannot silently escape the
test plan.

Data-only source files, such as files that only define lookup tables, maps,
object-placement tables, sprite pattern tables, or other static/global data, do
not need dedicated unit tests and should not be tested only by hashing or
spot-checking the table contents. Keep them in the inventory as backend inputs,
but exempt them from the unit-test and coverage gates. Their effect should be
covered indirectly by tests for the executable code that consumes them.

## Test Harness Architecture

Add an opt-in test build:

```cmake
option(SONICCD_BUILD_TESTS "Build backend unit tests" OFF)
```

When enabled, CMake should:

- Enable CTest.
- Build backend tests with the same include paths and compile definitions as the
  production variant under test.
- Require a 32-bit MSVC configuration for the primary test build. On Visual
  Studio generators, configure with the Win32 platform, not x64. Add an early
  CMake check that fails the backend test configuration if `CMAKE_SIZEOF_VOID_P`
  is 8.
- Build tests with coverage instrumentation.
- Compile/link production source files as-is. Test-specific wrapper files,
  generated manifests, object libraries, and linker options are allowed, but the
  files under `src/` are read-only inputs.
- Exclude `src/frontend/**`, `src/winexe/**`, `tests/**`, generated files, and
  compiler/system headers from coverage reports.

Use a small in-repo C or C++ test runner instead of starting with a large testing
dependency. The needs here are simple but exact: fixtures, assertions, snapshot
comparison, and CTest integration. A lightweight local runner also keeps the
tests easy to compile once per variant.

Recommended layout:

```text
tests/
  backend/
    CMakeLists.txt
    support/
      test_runner.h
      backend_fixture.c
      backend_fixture.h
      callback_log.c
      callback_log.h
      snapshot.c
      snapshot.h
    unit/
      test_action.c
      test_actset.c
      test_player.c
      ...
    variants/
      test_r11a_frames.c
      test_special_frames.c
      test_title_besttime.c
      ...
    golden/
      action/
      frames/
      title/
    tools/
      inventory_backend_sources.ps1
      coverage_report.ps1
      bless_golden.ps1
```

Prefer one test executable per variant or per variant family. The backend code
has many global symbols and variant-specific compile definitions, so separate
executables are cleaner than trying to link several variants into one process.

## Fixture Requirements

The central fixture must emulate the frontend/DLL boundary exactly enough for
unit tests to be deterministic.

For gameplay DLLs, `DLL_meminit(char ***pBufTbl, void **pFuncTbl)` wires imported
callbacks and shared memory. The fixture should provide:

- Real allocated buffers for `pmapwk`, palettes, `lphscrollbuff`, fade flags,
  `game_info`, window handle storage, and any variant-specific buffers.
- Stub implementations for every imported callback in `src/impfuncs.h`.
- Exact call logs for rendering/audio/file callbacks such as `SetGrid`,
  `EAsprset`, `ClrSpriteDebug`, `WaveRequest`, `CDPlay`, `CDPause`,
  `ChangeTileBmp`, and `WaveAllStop`.
- Deterministic implementations for memory and file functions:
  `sMemSet`, `sMemCpy`, `sMemCmp`, `sRandom`, `sOpenFile`, `sReadFile`,
  `sGetFileSize`, `sCloseFile`, `ReadScore`, `WriteScore`, and related helpers.

For title/menu DLLs, provide an `engine_dll` fixture that records all EA/HMX
operations and score-file calls. Tests should assert handle creation order,
palette writes, sprite positions, grid data, and return modes exactly.

Each test should start from a known state. The safest first version is:

- One process per CTest case for high-level frame tests.
- A fixture reset for low-level unit tests that zeroes global state, reinitializes
  buffers, clears callback logs, and then calls the appropriate `DLL_meminit` or
  title `DLLInit`.

Do not add ABI/layout tests such as `sizeof`, `offsetof`, or raw memory-layout
assertions. Those checks would become noise during the 64-bit port. If a layout
dependency is suspected, record it in `docs/backend_potential_bugs.md` or a
future porting note, then characterize the resulting visible behavior instead.

## Exactness Strategy

The tests should treat the current code as the oracle, but only after the oracle
has been captured in reviewed golden files.

Use three complementary assertion styles:

- Field assertions for small pure functions and boundaries.
- Callback trace assertions for rendering, sound, grid, file, and HMX behavior.
- Golden snapshots for dense state after one or more frames.

Snapshots should include exact fixed-point values, not rounded positions. For
example, actor position checks should compare `xposi.l` and `yposi.l`, plus the
visible word fields where relevant. This catches subpixel drift.

Golden frame snapshots should be deterministic records of:

- Selected globals from `equ.c` and variant-specific global files.
- Actor identity and externally visible actor effects, without directly
  snapshotting raw `sprite_status` or `sprite_status_sp` storage.
- Scroll positions, limits, water state, timers, rings, score, flags, and palette
  data.
- `SetGrid` calls, `EAsprset` calls, HMX calls, sound requests, CD requests, and
  file reads/writes.
- Return values from `game`, `FadeProc`, `DLLMain`, and similar entry points.

Avoid raw whole-process memory dumps. Prefer typed snapshots with explicit field
names for stable gameplay state. Do not directly read or snapshot structures
whose storage is accessed through raw pointer casts and expected to change during
the 64-bit port, including `sprite_status` and `sprite_status_sp`; observe their
effects through entry points, callbacks, rendered sprite calls, collision results,
timers, score/ring changes, and other stable behavior.

Golden update flow:

1. Run the old binary/tests and write candidate snapshots.
2. Review the snapshot diff as source code behavior.
3. Bless the snapshot only when the behavior change is intentional.
4. Never auto-update goldens during normal test runs.

## Coverage Strategy

Coverage must be measured on the backend source inventory, not on test files.
Use line, function, and branch coverage. The goal is 100% for all three.

Recommended build modes:

- `msvc-win32`: the required correctness target for this pass. Tests must build
  and pass here.

Do not add an x64 backend test configuration during this pass. If a developer has
an existing 64-bit CMake build directory for other work, the backend test option
should refuse to enable there rather than silently testing the wrong ABI. Any
non-MSVC coverage build used during this pass should also target a 32-bit ABI, or
be treated as an advisory report rather than the required correctness gate.

The coverage report should fail if:

- Any executable backend source in the inventory has no test coverage.
- Any included executable backend source is below 100% line/function/branch
  coverage.
- A backend source is compiled into a production variant but missing from the
  coverage inventory.

Data-only backend sources should be reported separately from executable sources.
They are not required to reach coverage thresholds because they do not contain
behavior to execute.

Do not exclude production lines just to make the percentage green. If a line is
truly unreachable, add a test or document the reason in a dedicated
`coverage_exceptions.md` file. Keep that file small and treat every entry as
technical debt.

## Phased Implementation

### Phase 1: Inventory and Baseline

- Add scripts to parse `CMakeLists.txt` and list backend sources by variant.
- Add `SONICCD_BUILD_TESTS` and a placeholder CTest target.
- Add coverage flags and a report command.
- Produce the first coverage report with a smoke test that loads/initializes one
  gameplay variant and one title variant.

Exit criteria:

- `ctest` runs.
- Coverage report includes only backend sources.
- Inventory clearly identifies all active backend files.

### Phase 2: Core Fixture

- Implement gameplay `DLL_meminit` fixture data and callback tables.
- Implement title/menu `engine_dll` fixture data and HMX callback logs.
- Implement global reset helpers.
- Add guardrails that keep tests focused on behavior rather than ABI/layout
  facts that are expected to change during the 64-bit port.
- Add callback-log assertions and snapshot serialization.

Exit criteria:

- Tests can call `DLL_meminit`, `game_init`, `game`, `DLLInit`, `DLLMain`, and
  `DLLEnd` without the real frontend.
- Stubs record every externally visible side effect.
- Fixture reset is deterministic across repeated runs.

### Phase 3: Low-Level Characterization

Start with files whose behavior can be asserted directly:

- `action.c`: `speedset`, `speedset2`, `actionsub`, `spatset`,
  `scronchk`, `scronchk2`, `frameout`.
- `actset.c`: actor allocation, frame-out checks, edit/object set behavior.
- `score.c`, `save.c`, `io.c`, `bmp.c`, `loader2.c`: memory, file, score, and
  data-loading behavior.
- `zone.c`, `scrchk.c`, `ridechk.c`, `fcol.c`, `dircol.c`: boundary and collision
  helpers.

For each function:

- Test every branch boundary, especially signed/unsigned comparisons.
- Assert exact fixed-point math.
- Assert exact callback calls and argument order.
- Add coverage only after assertions prove the behavior, not by calling code
  without checking outputs.

Exit criteria:

- Shared root files have meaningful coverage.
- The fixture can express exact sprite/grid/palette outcomes.

### Phase 4: Actor and Movement Files

Work through actor files by variant family:

- Common actor/player files first: `player.c`, `enemy.c`, `item.c`, `ring.c`,
  `spring.c`, `goal.c`, `taki.c`, `suicide.c`, `playsub.c`, `playsp*.c`,
  `plchg*.c`.
- Zone-specific files next: `src/r1/**`, `src/r3/**`, `src/r4/**`, `src/r5/**`,
  `src/r6/**`, `src/r7/**`, `src/r8/**`.

Skip files that only contain data definitions and no executable code. Do not add
unit tests whose only purpose is to fingerprint static tables. If a data-only
file is important to gameplay, cover it through the actor, collision, scroll,
frame, or variant test that consumes it.

Use table-driven tests for actor state machines:

- Initial actor setup through helper functions or public entry points.
- Initial globals and map/collision data.
- Input switch state.
- Number of frames to run.
- Expected final observable state, globals, and callback trace. Avoid direct
  assertions over raw actor storage such as `sprite_status` and
  `sprite_status_sp`.

Exit criteria:

- Each actor function has branch-complete tests.
- Each state transition has at least one golden or field-level assertion.
- Position, speed, animation frame, collision, and deletion behavior are exact.

### Phase 5: Variant Frame Tests

For each level DLL variant, add a small set of deterministic frame tests:

- Boot: `DLL_meminit` -> `game_init` -> snapshot.
- Idle frame: no input for one frame -> snapshot.
- Scripted input: one or more frames with controlled `swdata`/`swdata1`/`swdata2`.
- Representative actor/object setup for that variant's unique source files.

These tests are not a replacement for unit tests, but they catch integration
drift: compile definitions, data tables, object tables, scroll setup, palette
setup, and per-variant source combinations.

Exit criteria:

- Every CMake backend variant has at least one CTest executable or case.
- Shared files are tested under every compile-definition family that changes
  their behavior.

### Phase 6: Special, Warp, and Title/Menu DLLs

Cover non-level DLLs with the same exactness rules:

- `SPECIAL`: special stage game loop, scale/scroll helpers, palette changes,
  UFO/ring/time behavior.
- `WARP`: warp animation state, palette fade, atom/sonic actor movement, sound.
- `TA`, `THANKS`, `PLANET`: game-style title modules with `DLL_meminit`.
- `BESTTIME`, `OPENING`, `SAVEDATA`, `SOUNDTST`, `STAGETST`, `VISUALMD`:
  `DLLInit`/`DLLMain`/`DLLEnd`, HMX sprite/grid/bitmap calls, score IO, selection
  state, palette and menu behavior.

Exit criteria:

- All non-level backend targets are in the coverage gate.
- HMX/EA side effects are snapshot-tested exactly.

### Phase 7: 100% Gate and Maintenance

- Add a `backend-coverage` target that builds tests, runs CTest, merges coverage,
  writes HTML/text reports, and fails below 100%.
- Add a `backend-characterization` target for the golden snapshot suite.
- Add CI or a local pre-merge script that runs the coverage gate.
- Require every backend source-list change to update the inventory and tests.
- Require every suspected bug found during characterization to be recorded in
  `docs/backend_potential_bugs.md`, not fixed in source.

Exit criteria:

- `cmake --build build --target backend-coverage` is the normal confidence
  command before backend changes.
- 100% coverage is enforced on all active backend DLL sources.
- Any subpixel movement, sprite placement, palette value, callback argument, or
  state-machine change fails a test unless the golden/assertion is deliberately
  updated.

## Test Design Rules

- Tests should assert outputs and side effects. Calling a function only to mark a
  line covered is not acceptable.
- Prefer exact integer assertions. Do not use epsilon comparisons for fixed-point
  game state.
- Keep duplicated expected data out of tests when the production data table can
  be referenced directly. Duplicate only the expected result, not the algorithm.
- When a bug is discovered, first write the characterization test for current
  behavior, then document it in `docs/backend_potential_bugs.md`. Do not fix it
  in this pass.
- Make random, file IO, score IO, sound, and rendering fully deterministic.
- Keep each test's initial state explicit enough that failures can be debugged
  from the test body and snapshot diff.

## Risks and Mitigations

- Global state makes tests order-dependent. Mitigate with fixture reset and
  process-per-case frame tests.
- Variant compile definitions can change behavior in shared files. Mitigate by
  compiling tests with the exact variant definitions and reporting coverage by
  variant family.
- Golden files can preserve wrong behavior. Mitigate by reviewing golden diffs
  and pairing broad snapshots with focused, named assertions for important
  behavior.
- Static functions may be hard to reach directly. Prefer testing through public
  entry points and existing externally visible state. When that leaves coverage
  gaps, use separate test translation units, white-box tests that include an
  unchanged `.c` file, callback observation, scripted frame states, generated
  symbol maps, or debugger/coverage-guided scenarios. Do not add
  `SONICCD_TESTING` paths, edit the included `.c` file, or modify source linkage
  just for tests.
- Undefined or implementation-defined C behavior can make snapshots
  compiler-sensitive. Mitigate by pinning the coverage compiler for the gate,
  characterizing visible behavior, and documenting any compiler-specific
  expectations.
