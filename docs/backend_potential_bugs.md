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
