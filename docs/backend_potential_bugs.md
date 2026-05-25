# Backend Potential Bugs

Use this file during the backend characterization pass to record suspicious
behavior without changing the original source code.

For each finding, include:

- Test case or snapshot path.
- Variant and build configuration.
- Observed exact behavior.
- Why the behavior looks suspicious.
- Related source location, if known.

Do not remove or rewrite an entry as part of characterization. Bug fixes should
happen in a later, separate pass after the current behavior is fully captured by
tests.

## `src/r4/renketu4.c`: motion accumulator aliases the angle byte

- Test case or snapshot path: `tests/backend/unit/r4/test_renketu4.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `renketu4_move` adds the direction word at legacy
  word slot 26 into legacy word slot 33 before calling `sinset`. Under the
  tested 32-bit layout, word slot 33 overlaps `actfree[20]` and `actfree[21]`,
  so the add mutates `actfree[21]`, which is then used as the sine angle.
- Why the behavior looks suspicious: `actfree[21]` is assigned from
  `renketu4_tbl` as a per-piece angle, but the motion accumulator changes that
  same byte every frame. This is intentionally not fixed during characterization.
- Related source location: `src/r4/renketu4.c`, lines 102-105.

## `src/r4/escal4.c`: `escal4_ridechk` is missing a return value

- Test case or snapshot path: `tests/backend/unit/r4/test_escal4.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: compiling `backend_r4_escal4` emits MSVC warning
  C4716 because `escal4_ridechk` is declared to return `Sint16`, but it only
  calls `hitchk` and does not explicitly return its result. Under the tested
  build, the caller in `escal4_wait` behaves as if the `hitchk` return value is
  propagated.
- Why the behavior looks suspicious: this relies on compiler/code generation
  behavior for a non-void function that falls off the end. This is intentionally
  not fixed during characterization.
- Related source location: `src/r4/escal4.c`, lines 25-26.

## `src/actset.c`: `tm_setchk` does not return for unexpected time state

- Test case or snapshot path: `tests/backend/unit/test_actset.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `tm_setchk` returns for `time_flag & 127` values 0,
  1, and 2. For any other value, control falls through to `while (1) ;` and the
  function never returns.
- Why the behavior looks suspicious: the backend appears to use only three time
  states here, but the function accepts a byte-sized global state and has no
  recovery path for out-of-range values. This is intentionally not fixed during
  characterization.
- Related source location: `src/actset.c`, lines 174-184.

## `src/loader2.c`: `ReadScrolMap` clears the same map page repeatedly

- Test case or snapshot path: `tests/backend/unit/test_loader2.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `ReadScrolMap` runs its clear loop 128 times, but
  every `sMemSet` call receives `pmapwk` rather than the advancing local pointer
  `p`. The first 512 bytes of scroll-map storage are cleared repeatedly; later
  pages are not cleared by that loop.
- Why the behavior looks suspicious: the loop increments `p += 256`, which
  suggests the intended behavior may have been to clear each 512-byte page.
  This is intentionally not fixed during characterization.
- Related source location: `src/loader2.c`, lines 64-67.

## `src/suicide.c`: `enemy_suicide` checks an unsigned flag for negativity

- Test case or snapshot path: `tests/backend/unit/test_suicide.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `enemy_suicide` calls `frameout`, restores the
  actor's X/Y position, changes `actno` to 24, and does not call `soundset(158)`
  even when `actflg` is set to 255.
- Why the behavior looks suspicious: `actflg` is declared as `Uint8`, so
  `pActwk->actflg < 0` cannot be true under the tested 32-bit MSVC build. The
  source appears to be checking for the high bit as though the field were signed.
  This is intentionally not fixed during characterization.
- Related source location: `src/suicide.c`, lines 21-23.

## `src/edit.c`: actor spawn path is hardcoded unreachable

- Test case or snapshot path: `tests/backend/unit/test_edit.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `edit` always sets its local `CKey` value to `0`
  before checking `if (CKey & 128)`, so the guarded `actwkchk`/actor-spawn
  block is not reached. The trailing `CKeyTogle = CKey & 1` assignment stores
  `0`.
- Why the behavior looks suspicious: the block appears to preserve a key-gated
  edit-mode spawn operation, but the current source discards any external key
  state before the gate. This is intentionally not fixed during
  characterization.
- Related source location: `src/edit.c`, lines 82-99.

## `src/item.c`: `error_item` contains a no-effect comparison

- Test case or snapshot path: `tests/backend/unit/test_item.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: compiling the `backend_item` test target emits MSVC
  warning C4552 for `error_item`: the comparison expression is evaluated and
  discarded, then `itemwk->patbase` is set back to `itempat`.
- Why the behavior looks suspicious: the expression
  `(Uint32)((Sint32)itemwk->patbase ^ (Sint32)corepat) < 1;` appears to test
  whether the old pattern table was `corepat`, but the result is unused. This is
  intentionally not fixed during characterization.
- Related source location: `src/item.c`, lines 500-504.

## `src/enemy.c`: `ka_move` can fail to return when outside its patrol band

- Test case or snapshot path: `tests/backend/unit/test_enemy.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: after `ka_init`, placing the actor outside the
  128-pixel patrol band through visible position fields and calling `ka_move`
  makes the `do while (1)` loop toggle the internal speed/flip state repeatedly
  without reaching the `patchg` call or returning.
- Why the behavior looks suspicious: the patrol-turn block reverses direction
  but does not move the actor back inside the band before looping, so visible
  out-of-band positions can oscillate forever. This is intentionally not fixed
  during characterization.
- Related source location: `src/enemy.c`, lines 300-316.

## `src/r3/coli3.c`: `eggman_chk` switch cases are guarded out

- Test case or snapshot path: `tests/backend/unit/r3/test_coli3.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `eggman_chk` returns `-1` for every `bossstart`
  value except `1`. When `bossstart` is `1`, the following `switch (bossstart)`
  can only enter `case 1`; the `case 4`, `case 5`, and `default` bodies are not
  reachable through this function.
- Why the behavior looks suspicious: the switch appears to describe multiple
  boss collision modes, but the preceding guard prevents all modes except `1`.
  The `boss_4` and `boss_5` functions are characterized directly, and this is
  intentionally not fixed during characterization.
- Related source location: `src/r3/coli3.c`, lines 325-345.

## `src/r6/coli6.c`: collision helpers have missing returns

- Test case or snapshot path: `tests/backend/unit/r6/test_coli6.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: compiling `backend_r6_coli6` emits C4716 for
  `pcolplay2` and C4715 for `playdamageset`. The characterized call paths still
  produce the surrounding side effects, including setting the enemy damage flag,
  spawning loose rings when available, and entering the death path when no
  protection applies.
- Why the behavior looks suspicious: both functions are declared `Sint16`, but
  `pcolplay2` falls through after calling `pcolplay`, and `playdamageset` falls
  through after calling `playdieset` on the no-ring/no-debug path. This is
  intentionally not fixed during characterization.
- Related source location: `src/r6/coli6.c`, lines 215-217 and 258-290.

## `src/r6/coli6.c`: `eggman_chk` switch cases are guarded out

- Test case or snapshot path: `tests/backend/unit/r6/test_coli6.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `eggman_chk` returns `-1` for every `bossstart`
  value except `1`. When `bossstart` is `1`, the following `switch (bossstart)`
  can only enter `case 1`; the `case 4`, `case 5`, and `default` bodies are not
  reachable through this function.
- Why the behavior looks suspicious: the switch appears to describe multiple
  boss collision modes, but the preceding guard prevents all modes except `1`.
  The `boss_4` and `boss_5` functions are characterized directly, and this is
  intentionally not fixed during characterization.
- Related source location: `src/r6/coli6.c`, lines 330-349.

## `src/r6/dev61c.c`: thunder/null toggle has an unreachable duration

- Test case or snapshot path: `tests/backend/unit/r6/test_dev61c.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: the timer underflow path can set `cgchgtim[2]` to
  `30`, but the adjacent assignment to `90` is not reachable through byte timer
  values under the tested 32-bit MSVC build.
- Why the behavior looks suspicious: the code appears to intend a two-state
  toggle between `30` and `90`, but the signed-negative guard is only entered
  for byte values that remain nonzero after `cgchgtim[2] ^= 1`.
- Related source location: `src/r6/dev61c.c`, lines 35-42.

## `src/r6/dev62c.c`: thunder/null toggle has an unreachable duration

- Test case or snapshot path: `tests/backend/unit/r6/test_dev62c.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: the timer underflow path can set `cgchgtim[2]` to
  `30`, but the adjacent assignment to `90` is not reachable through byte timer
  values under the tested 32-bit MSVC build.
- Why the behavior looks suspicious: this matches the `dev61c` toggle shape;
  the code appears to intend a two-state toggle between `30` and `90`, but the
  signed-negative guard is only entered for byte values that remain nonzero
  after `cgchgtim[2] ^= 1`.
- Related source location: `src/r6/dev62c.c`, lines 35-42.

## `src/r3/edtbl32a.c`: `edit_tbl.num` is smaller than initialized entries

- Test case or snapshot path: `tests/backend/unit/r3/test_edtbl32a.c`
- Variant and build configuration: `R32A` table shape, MSVC 19.51, Win32 Debug
- Observed exact behavior: `edit_tbl.num` is 24, while the table initializer
  contains 36 concrete `edit_data_entry` values when compiled with
  `EDIT_DATA_ENTRY_COUNT=36`.
- Why the behavior looks suspicious: the final 12 initialized entries may be
  unreachable to code that uses `edit_tbl.num` as the authoritative table
  length. This is intentionally not fixed during characterization.
- Related source location: `src/r3/edtbl32a.c`, lines 24-62.

## `src/r8/scarab.c`: `c_move` contains a no-effect comparison

- Test case or snapshot path: `tests/backend/unit/r8/test_scarab.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: compiling the `backend_r8_scarab` test target emits
  MSVC warning C4553 for `flagwk.w == 0;`. The comparison result is discarded,
  and `c_move` continues without assigning to `flagwk.w`.
- Why the behavior looks suspicious: the statement appears intended to clear
  `flagwk.w`, but it has no effect. This is intentionally not fixed during
  characterization.
- Related source location: `src/r8/scarab.c`, line 215.

## `src/r8/scr81*.c`, `src/r8/scr82*.c`, and `src/r8/scr83*.c`: `scrollwrtb` can read past its write table at the top edge

- Test case or snapshot path: `tests/backend/unit/r8/test_scr82a.c` through
  `tests/backend/unit/r8/test_scr82d.c` and
  `tests/backend/unit/r8/test_scr83c.c` and
  `tests/backend/unit/r8/test_scr83d.c`; same guarded fixture shape is used by
  `tests/backend/unit/r8/test_scr81a.c` through
  `tests/backend/unit/r8/test_scr81d.c`.
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: while adding coverage for the screen-update flags,
  setting the upward update bit with `scrb_v_posit.w.h == 0` made `scrollwrtb`
  compute `(scrb_v_posit.w.h - 16) / 16`, mask it to `127`, and index beyond
  the local `z81awrttbl` array. The same top-edge calculation is present in
  `src/r8/scr81a.c` through `src/r8/scr81d.c`, `src/r8/scr82a.c` through
  `src/r8/scr82d.c`, `src/r8/scr83c.c`, and `src/r8/scr83d.c`. The
  characterization tests seed
  `scrb_v_posit.w.h = 16` before exercising that normal update path.
- Why the behavior looks suspicious: the table has 33 entries in these files,
  49 entries in the `scr81*` files, but the masked top-edge index can address
  `z81awrttbl[128]`. This is intentionally not fixed during characterization.
- Related source location: `src/r8/scr81a.c` through `src/r8/scr81d.c`, lines
  690-707 or equivalent; `src/r8/scr82a.c` through `src/r8/scr82d.c`, lines
  689-706 or equivalent; `src/r8/scr83c.c`, lines 683-700;
  `src/r8/scr83d.c`, lines 607-624.

## `src/r8/scr83c.c` and `src/r8/scr83d.c`: `block_wrt` draws but does not store the new block number

- Test case or snapshot path: `tests/backend/unit/r8/test_scr83c.c` and
  `tests/backend/unit/r8/test_scr83d.c`.
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug.
- Observed exact behavior: `block_wrt(12, 64, 0)` resolves a visible map cell
  and emits the expected four `SetGrid` calls, but the map word remains
  unchanged. Neighboring scroll files assign `*pBlockIndex = BlockNo` before
  drawing the block.
- Why the behavior looks suspicious: the function name and sibling
  implementations suggest the map should be updated as well as VRAM, but this
  characterization pass intentionally preserves the existing behavior.
- Related source location: `src/r8/scr83c.c`, lines 955-960, and
  `src/r8/scr83d.c`, lines 888-893.

## `src/r6/batta.c`: `emylr_jump` does not return a value

- Test case or snapshot path: `tests/backend/unit/r6/test_batta.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: compiling the `backend_r6_batta` test target emits
  MSVC warning C4716 because `emylr_jump` is declared to return `Sint16`, but
  control reaches the end of the function after calling either `emycol_r` or
  `emycol_l`.
- Why the behavior looks suspicious: `batta_down` and `batta_up` use the return
  value from `emylr_jump` to choose wall, floor, and ceiling behavior, so the
  source appears to rely on an implicit register result from the final collision
  helper call. This is intentionally not fixed during characterization.
- Related source location: `src/r6/batta.c`, lines 59-63.

## `src/r6/togebl6a.c` and `src/r6/togebl6b.c`: one spawned option is overwritten in the parent link table

- Test case or snapshot path: `tests/backend/unit/r6/test_togebl6a.c` and
  `tests/backend/unit/r6/test_togebl6b.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `togeball` allocates option actors, but the third
  and fourth allocations both write their actor index into the parent's same
  hidden child slot. On the next parent move, only the fourth actor in that slot
  receives a new hidden position; the third allocated option still runs its own
  option logic but copies the untouched zero hidden position into its visible
  `xposi`/`yposi`.
- Why the behavior looks suspicious: the allocation sequence appears intended to
  preserve all spawned option actors, but one child is no longer referenced by
  the parent movement update. This is intentionally not fixed during
  characterization.
- Related source location: `src/r6/togebl6b.c`, duplicated assignment to the
  same parent child slot in the option allocation sequence.

## `src/r7/col7d.c`: `clchgctr` secondary palette reads past local tables

- Test case or snapshot path: `tests/backend/unit/r7/test_col7d.c`
- Variant and build configuration: `R7xD`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `clchgctr` calls `clchg_sub0` with cycle tables whose
  count values can make `lpcolorwk3[idx] = pPlttbl[cnt + timwk]` read beyond the
  local `PALETTEENTRY` array passed for that cycle. The primary `lpcolorwk`
  palette writes still occur deterministically and are characterized by the
  test.
- Why the behavior looks suspicious: the count fields appear to describe the
  number of cycle states, but several local palette arrays contain fewer than
  `cnt + timwk + 1` entries on the first update. This is intentionally not fixed
  during characterization.
- Related source location: `src/r7/col7d.c`, lines 160-221.

## `src/r8/dev83d.c`: pan animation gate checks the PIS counter

- Test case or snapshot path: `tests/backend/unit/r8/test_dev83d.c`
- Variant and build configuration: `R83D` table shape, MSVC 19.51, Win32 Debug
- Observed exact behavior: when `shut_flag` is nonzero, `cg_change` still runs
  the pan tile animation if `cgchgcnt[0]` is nonzero. If the PIS timer does not
  advance and leaves `cgchgcnt[0]` at zero, the pan animation is skipped even
  when the pan counter itself is nonzero.
- Why the behavior looks suspicious: the neighboring `src/r8/dev83c.c` gate
  checks the current pan counter pointer, while `dev83d.c` checks the first
  animation counter directly. This is intentionally not fixed during
  characterization.
- Related source location: `src/r8/dev83d.c`, pan gate in `cg_change`.

## `src/r8/trap_r82.c`: `togeitax` repeats the same parent guard

- Test case or snapshot path: `tests/backend/unit/r8/test_trap_r82.c`
- Variant and build configuration: `R82`, MSVC 19.51, Win32 Debug
- Observed exact behavior: `togeitax` frames out when the parent actor stored in
  the hidden ride-number word is not `actno == 51`, and follows the parent when
  it is `actno == 51`. The second identical parent guard is not reachable:
  when the first guard is true it returns immediately, and when the first guard
  is false the second guard is false too.
- Why the behavior looks suspicious: the duplicate check looks accidental or
  like a missing alternate condition, but it is intentionally not fixed during
  characterization.
- Related source location: `src/r8/trap_r82.c`, lines 270-276.

## `src/r8/tobira.c`: `ms_hitchk` does not return a value

- Test case or snapshot path: `tests/backend/unit/r8/test_tobira.c`
- Variant and build configuration: default/R81 sprite-base path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: compiling the `backend_r8_tobira` test target emits
  MSVC warning C4716 because `ms_hitchk` is declared to return `Sint16`, but it
  only calls `hitchk` for the two door halves and then reaches the end of the
  function.
- Why the behavior looks suspicious: callers ignore the return value today, so
  the visible door behavior is deterministic, but the signature implies a value
  should be returned. This is intentionally not fixed during characterization.
- Related source location: `src/r8/tobira.c`, lines 197-202.

## `src/r8/shut.c`: `ridechk_k` does not return a value

- Test case or snapshot path: `tests/backend/unit/r8/test_shut.c`
- Variant and build configuration: default/R81 sprite-base path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: compiling the `backend_r8_shut` test target emits
  MSVC warning C4716 because `ridechk_k` is declared to return `Sint16`, but it
  only calls `ridechk(loopwk, &actwk[0])` and reaches the end of the function.
- Why the behavior looks suspicious: callers currently use the value in branch
  conditions, so the missing return may depend on compiler/codegen behavior.
  This is intentionally not fixed during characterization.
- Related source location: `src/r8/shut.c`, line 265.

## `src/r6/kdai6.c`: `kdai6_ridechk` does not return a value

- Test case or snapshot path: `tests/backend/unit/r6/test_kdai6.c`
- Variant and build configuration: default/R6 include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: compiling the `backend_r6_kdai6` test target emits
  MSVC warning C4716 because `kdai6_ridechk` is declared to return `Sint16`, but
  it only calls `ridechk(pActwk, &actwk[0])` and reaches the end of the
  function. Under this build, `kdai6_move` still observes the stubbed `ridechk`
  result when branching on `iD0 = kdai6_ridechk(pActwk)`.
- Why the behavior looks suspicious: the caller depends on a return value from a
  function that has no explicit return, so this may depend on compiler/codegen
  behavior. It is intentionally not fixed during characterization.
- Related source location: `src/r6/kdai6.c`, lines 19 and 42-44.

## `src/r6/et6.c`: collision count remains observable after death trigger

- Test case or snapshot path: `tests/backend/unit/r6/test_et6.c`
- Variant and build configuration: default/R6 include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: when `m_wait` is entered with `colicnt == 5`, the
  actor transitions to `r_no0 == 4`, clears `colino`, sets `patno == 7`, sets
  `generate_flag`, and awards 150 points, but the test still observes
  `colicnt == 5` after the public `et` call.
- Why the behavior looks suspicious: the source statement appears intended to
  clear both collision fields during the death trigger. The characterization
  test records the observed state and does not change the source.
- Related source location: `src/r6/et6.c`, lines 109-117.

## `src/r8/shut.c`: `shut_init` forces its own userflag branch

- Test case or snapshot path: `tests/backend/unit/r8/test_shut.c`
- Variant and build configuration: default/R81 sprite-base path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: `shut_init` writes `shutwk->userflag.b.h = 1` and
  then immediately branches on that same value, so the actor's own low-userflag
  branch is not reachable through the existing code path. The spawned partner
  can still take its separate high-userflag branch if its storage already has
  that value.
- Why the behavior looks suspicious: the local branch shape implies the door may
  once have supported both directions, but the current code forces one side.
  This is intentionally not fixed during characterization.
- Related source location: `src/r8/shut.c`, lines 65-71.

## `src/r8/dango8.c`: roll rebound speed preserves unsigned low word

- Test case or snapshot path: `tests/backend/unit/r8/test_dango8.c`
- Variant and build configuration: default/R8 sprite-base path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: in the rolling collision path, with `cos = 128`,
  the player X speed is set to `-896`, but the actor's stored rebound speed
  becomes `-16547840` after masking `lD1` to 16 bits, negating it, and shifting
  left by 8.
- Why the behavior looks suspicious: the sibling `dango7` implementation narrows
  through a signed 16-bit temporary before the shift, which produces a much
  smaller positive rebound speed for the same callback values. This may be a
  transcription difference, but it is intentionally not fixed during
  characterization.
- Related source location: `src/r8/dango8.c`, lines 203-210.

## `src/r4/scrchk4.c`: downward limit movement stores the step value

- Test case or snapshot path: `tests/backend/unit/r4/test_scrchk4.c`
- Variant and build configuration: default/R41A-style include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: when `scrchk` moves `scralim_down` toward a larger
  `scralim_n_down`, it assigns `scralim_down` to `4`, or `16` when the player is
  grounded near the camera threshold, instead of adding that step to the current
  limit.
- Why the behavior looks suspicious: sibling scroll-check implementations add
  the step to the current limit, so this may be a transcription error. It is
  intentionally not fixed during characterization.
- Related source location: `src/r4/scrchk4.c`, downward-limit branch in
  `scrchk`.

## `src/r8/shoot.c`: `mspd_set` indexes a four-entry table with a nibble

- Test case or snapshot path: `tests/backend/unit/r8/test_shoot.c`
- Variant and build configuration: default/R8 include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: `mspd_set` declares `Sint16 mspdtbl[4]`, then reads
  `mspdtbl[shootwk->userflag.b.h & 15]`. During characterization, using a
  non-negative high-userflag value outside `0..3` produced unstable movement
  setup, so the tests avoid those inputs and keep the source behavior
  documented rather than corrected.
- Why the behavior looks suspicious: the mask allows indexes `4..15` even
  though the table has only four entries. This is intentionally not fixed during
  characterization.
- Related source location: `src/r8/shoot.c`, lines 329-338.

## `src/r1/shoot1.c`: `mspd_set` indexes a four-entry table with a nibble

- Test case or snapshot path: `tests/backend/unit/r1/test_shoot1.c`
- Variant and build configuration: default/R1 include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: `mspd_set` declares `Sint16 mspdtbl[4]`, then reads
  `mspdtbl[shootwk->userflag.b.h & 15]`. The tests cover the in-bounds values
  and the negative high-userflag early return, but avoid non-negative
  high-userflag values outside `0..3` because they would depend on an
  out-of-bounds stack read.
- Why the behavior looks suspicious: the mask allows indexes `4..15` even
  though the table has only four entries. This is intentionally not fixed during
  characterization.
- Related source location: `src/r1/shoot1.c`, lines 344-353.

## `src/r8/scr81*.c`, `src/r8/scr82*.c`, and `src/r8/scr83*.c`: high `scrollwrtb` clamp can walk past the write table

- Test case or snapshot path: attempted during scroll coverage expansion; not
  retained as a unit test because it crashes several variants.
- Variant and build configuration: default/R8 scroll include paths, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: forcing `scrb_v_posit.w.h` high enough for
  `WrtTblCnt` to clamp to `113` in `scrollwrtb` can still crash while the
  following loop reads sixteen entries starting at that clamped index.
- Why the behavior looks suspicious: the clamp protects the starting index, but
  not the whole range consumed by the loop. This is intentionally not fixed
  during characterization.
- Related source location: `src/r8/scr81a.c`, lines 724-733, and sibling
  `scr81*`/`scr82*` scroll files.

## `src/r1/scr11d.c`: top-edge `scrollwrtb` row can walk past the write table

- Test case or snapshot path: `tests/backend/unit/r1/test_scr11d.c`
- Variant and build configuration: `R11A`, MSVC 19.51, Win32 Debug
- Observed exact behavior: normal scroll-write paths are characterized with
  `scrb_v_posit.w.h` inside the visible table range. Driving the bit-1
  top-edge path at `scrb_v_posit.w.h == 0` computes a negative row, masks it
  with `127`, and reads past the 33-entry `z11dwrttbl`.
- Why the behavior looks suspicious: the later clamp only applies to the
  multi-row flag pass, not to this first row lookup, so the top-edge write path
  can walk outside the static table. This is intentionally not fixed during
  characterization.
- Related source location: `src/r1/scr11d.c`, lines 603-605.

## `src/r4/walls.c`: child pattern setup reads before the selected row

- Test case or snapshot path: `tests/backend/unit/r4/test_walls.c`
- Variant and build configuration: default/R4 include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: `main_init` assigns `pPatno` to the start of
  `patno_tbl[pActwk->userflag.b.h + 1]`, then reads `*--pPatno` for each child.
  For `userflag.b.h == 0`, the eight child pattern numbers therefore come from
  row 0 in reverse order rather than row 1.
- Why the behavior looks suspicious: the code appears to intend reverse
  iteration through the selected row, but starts at the row's first element
  instead of one-past-the-end. This is intentionally not fixed during
  characterization.
- Related source location: `src/r4/walls.c`, child pattern assignment in
  `main_init`.

## `src/r4/tagameb4.c`: `a_stop` checks whether an unsigned flag is negative

- Test case or snapshot path: `tests/backend/unit/r4/test_tagameb4.c`
- Variant and build configuration: default/R4 include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: `a_stop` advances the timer and launches child spike
  actors, but the `soundset(179)` body is not reached because `actflg` is a
  `Uint8` and `pActwk->actflg < 0` is always false.
- Why the behavior looks suspicious: the condition looks like it may have been
  intended to test bit 7 as a signed flag. This is intentionally not fixed
  during characterization.
- Related source location: `src/r4/tagameb4.c`, line 220.

## `src/r4/coli4.c`: masked collision id is checked for values above 63

- Test case or snapshot path: `tests/backend/unit/r4/test_coli4.c`
- Variant and build configuration: default/R4 include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: `pcolspecial` masks `colino` with `63`, then checks
  `if (cColiNo > 63)` in the `bossstart == 1` path. The greater-than check is
  not reachable after the mask.
- Why the behavior looks suspicious: the condition is redundant and may be a
  leftover range guard from before masking. This is intentionally not fixed
  during characterization.
- Related source location: `src/r4/coli4.c`, line 334.

## `src/r4/boss_4_2.c`: logical-not is combined with a bit mask

- Test case or snapshot path: `tests/backend/unit/r4/test_boss_4_2.c`
- Variant and build configuration: default/R4 boss 2 include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: `ychg_ret` uses
  `if (!(pEggwk->actfree[2]) & 8)` before optionally negating the bubble's
  angular speed. The condition never reaches its body because logical-not
  produces only `0` or `1`, and neither value has bit `8` set.
- Why the behavior looks suspicious: the expression looks like it may have
  meant `if (!(pEggwk->actfree[2] & 8))`, matching nearby direction checks.
  This is intentionally not fixed during characterization.
- Related source location: `src/r4/boss_4_2.c`, line 894.

## `src/r4/playsub4.c`: standalone pointer arithmetic has no effect

- Test case or snapshot path: `tests/backend/unit/r4/test_playsub4.c`
- Variant and build configuration: default/R4 playsub include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: compiling the characterization test emits MSVC
  warning C4552 for the statement `pActwk + 1;` in `plawamaster_jump2`. The
  statement does not change `pActwk` or any actor state.
- Why the behavior looks suspicious: the expression looks like it may have been
  intended to advance the pointer or was left behind during translation. This
  is intentionally not fixed during characterization.
- Related source location: `src/r4/playsub4.c`, line 709.

## `src/r5/boss_5.c`: standalone pointer arithmetic has no effect

- Test case or snapshot path: `tests/backend/unit/r5/test_boss_5.c`
- Variant and build configuration: default/R5 boss include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: compiling the characterization test emits MSVC
  warning C4552 for the statements `pActwk + 1;` in `egg5belt_3` and
  `frameout_sp4`. The expression results are discarded and do not modify the
  actor pointer or actor state.
- Why the behavior looks suspicious: both expressions look like leftover
  pointer advances from translation or editing. This is intentionally not fixed
  during characterization.
- Related source locations: `src/r5/boss_5.c`, lines 1164 and 1364.

## `src/r4/game4.c`: standalone comparison has no effect

- Test case or snapshot path: `tests/backend/unit/r4/test_game4.c`
- Variant and build configuration: default/R4 game include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: compiling the characterization test emits MSVC
  warning C4552 for the statement `((Uint32)demo_cnt ^ 2048) < 1;` in
  `game`. The comparison result is discarded and does not affect control flow.
- Why the behavior looks suspicious: the expression looks like it may have
  been intended to guard demo timing logic, but as written it is a no-op. This
  is intentionally not fixed during characterization.
- Related source location: `src/r4/game4.c`, line 170.

## `src/r4/game4.c`: `sdfdout` pause fade-out body is unreachable

- Test case or snapshot path: `tests/backend/unit/r4/test_game4.c`
- Variant and build configuration: default/R4 game include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: `sdfdout` always calls `soundset(171)`, but the
  body guarded by `if (!(pauseflag.b.h | 128))` is not reached because
  bitwise-or with `128` always produces a nonzero value.
- Why the behavior looks suspicious: the condition looks like it may have meant
  to check whether bit `128` was absent, for example `!(pauseflag.b.h & 128)`.
  This is intentionally not fixed during characterization.
- Related source location: `src/r4/game4.c`, line 603.

## `src/r5/game5.c`: `sdfdout` pause fade-out body is unreachable

- Test case or snapshot path: `tests/backend/unit/r5/test_game5.c`
- Variant and build configuration: R5 game include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: `sdfdout` always calls `soundset(171)`, but the
  body guarded by `if (!(pauseflag.b.h | 128))` is not reached because
  bitwise-or with `128` always produces a nonzero value.
- Why the behavior looks suspicious: the condition looks like it may have meant
  to check whether bit `128` was absent, for example `!(pauseflag.b.h & 128)`.
  This is intentionally not fixed during characterization.
- Related source location: `src/r5/game5.c`, line 598.

## `src/r5/scr51a.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr51a.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr51a.c`, lines 850-856 and 887-899.

## `src/r5/scr51b.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr51b.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 1B include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr51b.c`, lines 824-827 and 856-864.

## `src/r5/scr51c.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr51c.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 1C include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr51c.c`, lines 736-739 and 768-776.

## `src/r5/scr51d.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr51d.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 1D include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr51d.c`, lines 797-800 and 829-837.

## `src/r5/scr52a.c`: `scrollwrtb` can read past its write tables

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr52a.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 2A include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first lookup use index `128` into either
  `z81awrttbl` or `z81awrttbl2`, depending on `enkeino`. Forcing the later
  high-row clamp to `113` also starts a sixteen-entry loop beyond the safe range
  of both write tables.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside the write table selected
  by `enkeino`. This is intentionally not fixed during characterization.
- Related source location: `src/r5/scr52a.c`, lines 843-849 and 880-891.

## `src/r5/scr52b.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr52b.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 2B include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr52b.c`, lines 823-825 and 855-863.

## `src/r5/scr52c.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr52c.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 2C include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr52c.c`, lines 736-738 and 768-776.

## `src/r5/scr52d.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr52d.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 2D include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr52d.c`, lines 797-799 and 829-837.

## `src/r5/scr53c.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr53c.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 3C include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr53c.c`, lines 669-671 and 701-709.

## `src/r5/scr53d.c`: `scrollwrtb` can read past its write table

- Test case or snapshot path: discovered while expanding
  `tests/backend/unit/r5/test_scr53d.c`; the unsafe cases are not retained as
  unit tests.
- Variant and build configuration: R5 scroll 3D include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: setting the upward update bit with
  `scrb_v_posit.w.h == 0` makes the first `z81awrttbl` lookup use index `128`
  on a 65-byte table. Forcing the later high-row clamp to `113` also starts a
  sixteen-entry loop beyond the safe range of the same table.
- Why the behavior looks suspicious: the clamp protects only some row values
  and does not keep all subsequent table reads inside `z81awrttbl`. This is
  intentionally not fixed during characterization.
- Related source location: `src/r5/scr53d.c`, lines 712-714 and 744-752.

## `src/r4/scr41a.c`: negative guards follow unsigned-derived values

- Test case or snapshot path: `tests/backend/unit/r4/test_scr41a.c`
- Variant and build configuration: default/R4 scroll include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: `scrollwrtb` assigns `WrtTblCnt` from a `Uint16`
  expression before checking `if (WrtTblCnt < 0)`, and `mapadrset99` checks
  `if (i < 0)` after clamping unsigned offsets. The guarded bodies do not
  appear reachable under the test build.
- Why the behavior looks suspicious: both checks look like defensive clamps
  left over from a signed-offset implementation. This is intentionally not
  fixed during characterization.
- Related source locations: `src/r4/scr41a.c`, lines 849 and 1002.

## `src/game.c`: standalone comparison has no effect

- Test case or snapshot path: `tests/backend/unit/test_game.c`
- Variant and build configuration: default/common game include path, MSVC
  19.51, Win32 Debug
- Observed exact behavior: compiling the characterization test emits MSVC
  warning C4552 for the statement `(Uint32)(demo_cnt ^ 2048) < 1;` in `game`.
  The comparison result is discarded and does not affect control flow.
- Why the behavior looks suspicious: the expression looks like it may have
  been intended to guard demo timing logic, but as written it is a no-op. This
  is intentionally not fixed during characterization.
- Related source location: `src/game.c`, line 181.

## `src/title/opening/opngrid.c`: standalone comparison has no effect

- Test case or snapshot path:
  `tests/backend/unit/title/opening/test_opngrid.c`
- Variant and build configuration: title opening grid include path, MSVC
  19.51, Win32 Debug
- Observed exact behavior: compiling the characterization test emits MSVC
  warning C4552 for the statement `(Uint32)(indx ^ 2) < 1;` in
  `OEGridDelete`. The comparison result is discarded and does not affect grid
  cleanup.
- Why the behavior looks suspicious: the expression looks like it may have
  been intended to distinguish index `2`, but as written it is a no-op. This
  is intentionally not fixed during characterization.
- Related source location: `src/title/opening/opngrid.c`, line 129.

## `src/game.c`: `sdfdout` pause fade-out body is unreachable

- Test case or snapshot path: `tests/backend/unit/test_game.c`
- Variant and build configuration: default/common game include path, MSVC
  19.51, Win32 Debug
- Observed exact behavior: `sdfdout` always calls `soundset(171)`, but the
  body guarded by `if (!(pauseflag.b.h | 128))` is not reached because
  bitwise-or with `128` always produces a nonzero value.
- Why the behavior looks suspicious: the condition looks like it may have meant
  to check whether bit `128` was absent, for example `!(pauseflag.b.h & 128)`.
  This is intentionally not fixed during characterization.
- Related source location: `src/game.c`, line 611.

## `src/r6/shoot6.c`: duplicate `r_no0 == 4` check appears unreachable

- Test case or snapshot path: `tests/backend/unit/r6/test_shoot6.c`
- Variant and build configuration: R6 shooter include path, MSVC 19.51, Win32
  Debug
- Observed exact behavior: `shootermove` checks `if (actwk[0].r_no0 == 4)` and
  changes it to `2`, then later checks the same value again without any
  intervening write to `r_no0`. The second guarded body does not appear
  reachable through normal state setup.
- Why the behavior looks suspicious: this looks like duplicated state cleanup,
  or a missed write between the two checks. This is intentionally not fixed
  during characterization.
- Related source location: `src/r6/shoot6.c`, lines 579 and 592-593.

## `src/title/savedata/svdsprt.c`: block-number guard is always true

- Test case or snapshot path:
  `tests/backend/unit/title/savedata/test_svdsprt.c`
- Variant and build configuration: savedata sprite include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: `CreateCharSprt` and `DeleteCharSprt` use
  `nBlockNo >= 2 || nBlockNo < 9` to select the file-sprite block path. That
  expression is true for every integer, so the final `return 0` in
  `CreateCharSprt` is unreachable and out-of-range `nBlockNo` values would index
  outside `hSprFile[nBlockNo - 2]`.
- Why the behavior looks suspicious: the condition looks like it may have meant
  to require `2 <= nBlockNo < 9`, probably using `&&` rather than `||`. This is
  intentionally not fixed during characterization.
- Related source locations: `src/title/savedata/svdsprt.c`, lines 134, 155, and
  181.

## `src/title/opening/opndo.c`: `GetNextMenu` can fall through without returning

- Test case or snapshot path: `tests/backend/unit/title/opening/test_opndo.c`
- Variant and build configuration: opening title include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: compiling the characterization test emits MSVC
  warning C4715 because `GetNextMenu` has no return after the forward/backward
  seven-entry search loops. The tests cover enabled-entry searches and do not
  call the all-disabled-table case because the returned value would be
  undefined.
- Why the behavior looks suspicious: callers appear to expect a valid menu
  index, but the helper has no fallback when every menu entry is disabled. This
  is intentionally not fixed during characterization.
- Related source location: `src/title/opening/opndo.c`, line 320.

## `src/title/opening/opndo.c`: planet table reset branch appears unreachable

- Test case or snapshot path: `tests/backend/unit/title/opening/test_opndo.c`
- Variant and build configuration: opening title include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: `OEUpdatePlanet` advances through the `ptPlnt`
  timing table and resets at the sentinel entry where `time == -1`. The
  preceding `if ((Uint32)++num > 7)` reset does not appear reachable through the
  table's normal timing values.
- Why the behavior looks suspicious: the code has two reset mechanisms for the
  same table, but the sentinel path seems to handle the terminal entry first.
  This is intentionally not fixed during characterization.
- Related source location: `src/title/opening/opndo.c`, line 144.

## `src/title/common/hmx_oeeactl.c`: `ld_load_sprite2` loads one more bitmap than its clamped size

- Test case or snapshot path: `tests/backend/unit/title/common/test_hmx_oeeactl.c`
- Variant and build configuration: common title helper include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: after clamping `read_count` to the caller-provided
  `size`, `ld_load_sprite2` increments `read_count` before the load loop. With
  `header.count == 32` and `size == 32`, it returns `33` and writes
  `bitmaps[32]`.
- Why the behavior looks suspicious: the caller-provided size appears to be a
  capacity, but this path writes one element past that capacity after clamping.
  This is intentionally not fixed during characterization.
- Related source location: `src/title/common/hmx_oeeactl.c`, lines 284-291.

## `src/player6.c`: standalone comparison has no effect

- Test case or snapshot path: `tests/backend/unit/test_player6.c`
- Variant and build configuration: common stage-6 player helper, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: compiling the characterization test emits MSVC
  warning C4553 for `generate_flag == 0;`. The expression is evaluated as a
  comparison and the result is discarded.
- Why the behavior looks suspicious: this may have been intended to assign
  `generate_flag = 0`. This is intentionally not fixed during characterization.
- Related source location: `src/player6.c`, line 272.

## `src/title/thanks/sprmove.c`: `sonicinit` forces the right-facing branch

- Test case or snapshot path:
  `tests/backend/unit/title/thanks/test_sprmove.c`
- Variant and build configuration: thanks title include path, MSVC 19.51,
  Win32 Debug
- Observed exact behavior: `sonicinit` reads a random value and masks it to one
  bit, then immediately assigns `ld0.w.l = 1` before the branch. The `else`
  branch that would place Sonic at `XPOSI == 496` and give him positive X speed
  is not reachable through that code path.
- Why the behavior looks suspicious: the random bit appears intended to choose
  Sonic's starting side, but the explicit assignment makes the branch
  deterministic. This is intentionally not fixed during characterization.
- Related source location: `src/title/thanks/sprmove.c`, lines 132-142.
