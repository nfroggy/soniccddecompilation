# Backend Coverage Exceptions

The backend characterization suite should keep this file as small as possible.
Entries here are production lines that the coverage tool reports as uncovered
after branch-focused tests have exercised the surrounding behavior, but which do
not appear reachable without changing the original source or falsifying source
mapping.

## `src/fcol.c`

- `scramapad`, lines 585-588: after `iYwork` and `iXwork` are derived from
  `iYposi`/`iXposi`, both values are masked before these clamps run:
  `iYwork &= 7` and `iXwork &= 63`. Under 32-bit MSVC Win32, the tested signed
  negative and positive inputs still produce nonnegative values by the time the
  `if (iYwork < 0)` and `if (iXwork < 0)` checks execute. The surrounding
  `stageno.w != 0` branch and map lookup behavior are covered by
  `tests/backend/unit/test_fcol.c`; only the assignments inside those two
  unreachable defensive checks remain uncovered.

## `src/suicide.c`

- `enemy_suicide`, line 23: `soundset(158)` is guarded by
  `if (pActwk->actflg < 0)`, but `actflg` is declared as `Uint8`. Under 32-bit
  MSVC Win32, setting `actflg` to 255 still does not make the comparison true.
  The surrounding delete/restore behavior is covered by
  `tests/backend/unit/test_suicide.c`, and the suspicious unsigned comparison is
  recorded in `docs/backend_potential_bugs.md`.

## `src/ring.c`

- `flyringinit`, line 292: `d4.w = 648` is guarded by
  `if ((d4.w -= 128) < 0)` inside the positive-angle update path. The function
  clamps `plring` to 32, and `d4.w` is negated after each spawned ring, so the
  positive-angle path runs at most 16 times in one call. Under 32-bit MSVC Win32,
  that reaches the subtract branch but not enough times to make the subtraction
  negative. The surrounding long-sequence behavior is covered by
  `tests/backend/unit/test_ring.c`.

## `src/r3/coli3.c`

- `eggman_chk`, lines 330 and 335-341: the function returns immediately unless
  `bossstart == 1`, then switches on `bossstart`. Under 32-bit MSVC Win32, that
  makes the `case 4`, `case 5`, and `default` switch arms unreachable through
  `eggman_chk`. The boss handlers themselves are covered by direct calls in
  `tests/backend/unit/r3/test_coli3.c`, and the suspicious guard/switch shape is
  recorded in `docs/backend_potential_bugs.md`.

## `src/r6/coli6.c`

- `eggman_chk`, lines 335 and 340-346: this matches the `src/r3/coli3.c`
  guard/switch shape. The function returns immediately unless `bossstart == 1`,
  so `case 4`, `case 5`, and `default` are not reachable through `eggman_chk`
  under 32-bit MSVC Win32. The boss handlers themselves are covered by direct
  calls in `tests/backend/unit/r6/test_coli6.c`, and the suspicious
  guard/switch shape is recorded in `docs/backend_potential_bugs.md`.

## `src/r6/dev61c.c`

- `cg_change`, line 40: after `--cgchgtim[2]`, the body is entered only when
  `(char)cgchgtim[2] < 0`, which means the byte is in the signed-negative
  range under 32-bit MSVC. XORing that value with `1` cannot make it `0`, so
  the `cgchgtim[2] = 90` assignment does not appear reachable. The false
  branch, normal thunder path, null-table path, and `= 30` assignment are
  covered by `tests/backend/unit/r6/test_dev61c.c`.

## `src/r6/dev62c.c`

- `cg_change`, line 40: this matches the `src/r6/dev61c.c` thunder/null toggle
  shape. After `--cgchgtim[2]`, the body is entered only when
  `(char)cgchgtim[2] < 0`, and XORing that byte with `1` cannot make it `0`.
  The false branch, normal thunder path, null-table path, and `= 30`
  assignment are covered by `tests/backend/unit/r6/test_dev62c.c`.

## `src/goal.c`

- `gene_move0`, lines 69, 72, and 80-83: the horizontal bounce path repeats the
  same horizontal bounds calculation that `gene_coli` already requires to pass
  before `gene_move0` can reach the bounce logic. Inputs that would take either
  `goto label1` in `gene_move0` return `0` from `gene_coli` first, so the label
  body does not appear reachable without changing the original source or
  falsifying the collision helper. The vertical bounce and no-collision paths
  are covered by `tests/backend/unit/test_goal.c`.

## `src/item.c`

- `t_item_move0`, line 169, and `ride_on_chk_i`, line 232: these are `} else {`
  source lines in stage-6 priority gates. Under 32-bit MSVC Win32 coverage, the
  statements inside both the true and false priority branches are covered by
  `tests/backend/unit/test_item.c`, but the coverage XML still reports the
  `else` line itself as uncovered. This appears to be source mapping on a
  non-actionable brace/keyword line rather than an untested behavior path.

## `src/player.c`

- `lmovecol`, line 847: `add_speed` is assigned from `(char)dircolm(...)` and
  the function returns immediately when that value is nonnegative. Any value
  that reaches the shift is therefore sign-extended negative under 32-bit MSVC
  Win32, so `add_speed & 32768` is always true and the plain left-shift branch
  does not appear reachable. The covered cases exercise the visible X/Y speed
  adjustment paths and the side-stop flags.
- `jumpmove`, lines 1151 and 1155: these are zero-crossing clamps after
  `cal_speed = cal_speed - cal_speed / 32`. For positive speeds, integer
  truncation keeps the result positive; for negative speeds, it keeps the result
  negative. The surrounding air-control, time-attack gate, camera return, and
  drag behavior are covered by `tests/backend/unit/test_player.c`.

## `src/r8/trap_r82.c`

- `togeitax`, lines 274-276: this is a duplicate guard immediately after an
  identical `if (actwk[ride_number].actno != 51)` that already calls
  `frameout` and returns. If the first guard is true, execution never reaches
  the second one; if the first guard is false, the second one is false too.
  The live-parent follow path and the missing-parent frameout path are covered
  by `tests/backend/unit/r8/test_trap_r82.c`, and the duplicated unreachable
  check is recorded in `docs/backend_potential_bugs.md`.

## `src/r8/okusieso.c`

- `ball_move`, lines 222-223: after the gravity update, the code compares
  signed 16-bit `yspeed.w` to the positive literal `57344`. The matching bit
  pattern is `-8192` in `Sint16`, and under 32-bit MSVC Win32 the comparison
  remains false even when the ball reaches that exact stored speed. The
  surrounding launched-ball, landing, zero-speed animation, paused, and parent
  gate paths are covered by `tests/backend/unit/r8/test_okusieso.c`.

## `src/r8/shoot.c`

- `mspd_set`, line 338: the assignment is guarded by
  `if (cal_mspeed > actwk[0].mspeed.w)` immediately after
  `actwk[0].mspeed.w = 4096`. The in-bounds speed table values are
  `4096`, `3072`, `3072`, and `2048`, so the branch is false for every
  in-bounds entry under 32-bit MSVC Win32. Reaching a larger value would require
  depending on the documented nibble-index out-of-bounds read in
  `docs/backend_potential_bugs.md`.

## `src/r1/shoot1.c`

- `mspd_set`, line 353: this matches the `src/r8/shoot.c` behavior above. The
  function assigns `actwk[0].mspeed.w = 4096` immediately before checking
  `if (cal_mspeed > actwk[0].mspeed.w)`, and the in-bounds table values are
  `4096`, `3072`, `3072`, and `2048`. Reaching the assignment would require
  relying on the same nibble-index out-of-bounds table read documented in
  `docs/backend_potential_bugs.md`.

## `src/r1/coli1.c`

- `pcolspecial`, line 337: `cColiNo` is assigned from
  `pColliAct->colino & 63`, so the preceding `if (cColiNo > 63)` body does not
  appear reachable under 32-bit MSVC Win32. The neighboring special-collision
  cases, the low default return, and the boss-check path are covered by
  `tests/backend/unit/r1/test_coli1.c`.

## `src/r8/scr81a.c`, `src/r8/scr81b.c`, `src/r8/scr81c.c`, `src/r8/scr81d.c`, `src/r8/scr82a.c`, `src/r8/scr82b.c`, `src/r8/scr82c.c`, `src/r8/scr82d.c`, `src/r8/scr83c.c`, and `src/r8/scr83d.c`

- `scrollwrtb`, the `WrtTblCnt < 0` and `WrtTblCnt > 113` clamp bodies:
  `WrtTblCnt` is `Uint16`, so the negative clamp is not reachable under
  32-bit MSVC Win32. The high clamp is tied to the documented top-edge and
  high-edge write-table out-of-bounds behavior in
  `docs/backend_potential_bugs.md`; characterization tests cover the normal
  write-table rows and avoid forcing the unsafe table walk.
- `mapadrset99`, the `i < 0` clamp body: `xOffs` and `yOffs` are unsigned
  inputs and are clamped to nonnegative map ranges before `i` is calculated, so
  the negative-index correction does not appear reachable through the public map
  address helpers.
- For `src/r8/scr83d.c`, `scrollwrtb` lines 649-652 are also left uncovered:
  the variant's `z81awrttbl` has no in-range zero row selected by the normal
  `wD0 + 1` lookup. Forcing the only apparent zero-row route would require the
  same unsafe top-edge/out-of-bounds indexing described in
  `docs/backend_potential_bugs.md`.

## `src/r1/scr13c.c`

- `scrollwrtb`, the `WrtTblCnt < 0` and `WrtTblCnt > 113` clamp bodies:
  `WrtTblCnt` is `Uint16`, so the negative clamp is not reachable under
  32-bit MSVC Win32. The high clamp uses a maximum row index far beyond the
  33-entry `z12cwrttbl`, so forcing that path would characterize an unsafe
  out-of-bounds table walk rather than normal scroll-write behavior.
- `mapadrset99`, the `i < 0` clamp body: `xOffs` and `yOffs` are unsigned
  inputs and are clamped to nonnegative map ranges before `i` is calculated, so
  the negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r1/scr13d.c`

- `scrollwrtb`, the `WrtTblCnt < 0` and `WrtTblCnt > 113` clamp bodies:
  `WrtTblCnt` is `Uint16`, so the negative clamp is not reachable under
  32-bit MSVC Win32. The high clamp uses a maximum row index far beyond the
  33-entry `z12dwrttbl`, so forcing that path would characterize an unsafe
  out-of-bounds table walk rather than normal scroll-write behavior.
- `mapadrset99`, the `i < 0` clamp body: `xOffs` and `yOffs` are unsigned
  inputs and are clamped to nonnegative map ranges before `i` is calculated, so
  the negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r1/scr11d.c`

- `scrollwrtb`, lines 637 and 639: these are the same write-table clamp bodies
  covered by the `scr13c`/`scr13d` exceptions. The negative clamp is not
  reachable after assigning a `Uint16` row value to `WrtTblCnt`, and the high
  clamp is tied to unsafe write-table indexing beyond the 33-entry
  `z11dwrttbl`. Normal top, bottom, parallax, and empty flag paths are covered
  by `tests/backend/unit/r1/test_scr11d.c`.
- `mapadrset99`, line 785: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative ranges before `i` is calculated, so the negative-index
  correction does not appear reachable through the public map address helpers.

## `src/r8/shut.c`

- `shut_init`, line 71: the function writes `shutwk->userflag.b.h = 1`
  immediately before checking `if (shutwk->userflag.b.h)`, so the actor's own
  `else` branch does not appear reachable under 32-bit MSVC Win32. The partner
  actor's matching high-userflag path and the normal/failed allocation paths are
  covered by `tests/backend/unit/r8/test_shut.c`, and the suspicious forced
  branch is recorded in `docs/backend_potential_bugs.md`.

## `src/r8/dai8.c`

- `dai8`, lines 49-57: these child/parent position mismatch guards compare
  hidden actor words that are written only by `act_init_sub`. A test can cover
  the live-parent child path and the missing-parent frameout path by creating
  the child through `dai8`, but forcing only the hidden stored X or Y snapshot to
  differ would require direct raw `sprite_status` storage writes. The backend
  test plan forbids that because those storage slots are expected to change
  during the 64-bit port.

## `src/r4/friend4.c`

- `m0move`, lines 125-126: this frameout path depends on
  `((char *)tempact)[67]` for the parent actor. Under the 32-bit MSVC layout
  used by these tests, byte 67 is outside `actfree` and is not reachable through
  a stable `sprite_status` field. The invalid-parent path and the live-parent
  movement/toggle paths are covered by `tests/backend/unit/r4/test_friend4.c`;
  forcing only this status byte would require a direct raw actor storage write
  that the backend unit test plan forbids.

## `src/r4/tagameb4.c`

- `a_stop`, line 221: `actflg` is declared as `Uint8`, so the
  `if (pActwk->actflg < 0)` body is not reachable under 32-bit MSVC Win32.
  The timed stop, child launch setup, stop-to-dash, dash, and child movement
  paths are covered by `tests/backend/unit/r4/test_tagameb4.c`, and the
  suspicious signedness check is recorded in `docs/backend_potential_bugs.md`.
