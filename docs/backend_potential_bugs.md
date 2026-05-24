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
