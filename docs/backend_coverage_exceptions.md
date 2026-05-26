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

## `src/edit.c`

- `edit`, lines 84-95: the actor-spawn block is guarded by `if (CKey & 128)`
  immediately after the local `CKey` has been assigned `0`. Under 32-bit MSVC
  Win32, no caller-provided input can make that branch true. The disabled,
  movement, map lookup, entry cycling, exit, display-field update, and
  `CKeyTogle` behavior are covered by `tests/backend/unit/test_edit.c`, and the
  suspicious hardcoded key state is recorded in
  `docs/backend_potential_bugs.md`.

## `src/emie1.c`

- `emie1_matu`, line 147: this is the brace-only `else` line for the
  negative-speed branch. The branch body immediately below it is covered by
  `tests/backend/unit/test_emie1.c`, including negative chase, clamp, and
  boundary behavior, but MSVC coverage still reports the `else` source line as
  uncovered.
- `dakicheck`, line 405: the assignment/comparison in the `else if` is
  reported as partial even after tests cover the cddat-facing path, the
  nonnegative fallthrough, and the negative return path. The tested outcomes
  document the existing distance guards without direct raw actor-storage reads.

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

## `src/r6/shoot6.c`

- `shootermove`, line 593: this repeats `if (actwk[0].r_no0 == 4)` after an
  earlier identical check in the same function has already changed `r_no0` from
  `4` to `2`. No intervening statement writes `r_no0`, so the second body does
  not appear reachable under 32-bit MSVC Win32. The wrapper dispatch cases,
  early returns, first `r_no0 == 4` adjustment, route movement, button gates,
  and speed calculations are covered by `tests/backend/unit/r6/test_shoot6.c`;
  the duplicated check is recorded in `docs/backend_potential_bugs.md`.

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

## `src/player6.c`

- `lmovecol`, line 988: this matches the `src/player.c` `lmovecol`
  sign-extension shape. `add_speed` is assigned from `(char)dircolm(...)`, and
  the function returns immediately when that value is nonnegative. Any value
  that reaches the shift is therefore sign-extended negative under 32-bit MSVC
  Win32, so the plain left-shift branch does not appear reachable. The visible
  return, X/Y adjustment, and side-stop paths are covered by
  `tests/backend/unit/test_player6.c`.
- `jumpmove`, lines 1290 and 1294: these match the `src/player.c` drag clamps.
  After `cal_speed = cal_speed - cal_speed / 32`, integer truncation keeps
  positive speeds positive and negative speeds negative under 32-bit MSVC
  Win32. The air-control, camera-return, drag, and small-speed return paths are
  covered by `tests/backend/unit/test_player6.c`.
- `patchgmain2`, line 1871: MSVC reports the `switch` line as partial even
  though `tests/backend/unit/test_player6.c` covers all three case labels
  reachable from this switch (`253`, `254`, and `255`) and the direct
  `pat_no < 253` path.

## `src/r8/trap_r82.c`

- `itaset_chk`, line 228: the switch line remains partial under MSVC coverage
  even though `tests/backend/unit/r8/test_trap_r82.c` covers all four case
  labels (`300`, `600`, `840`, and `1080`) and the default return path.
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

## `src/r8/boss_8.c`

- `egg8_spin_r`, line 679: this increment branch is guarded by
  `if (pActwk->actfree[3] & 1)` immediately after `actfree[3]` is assigned the
  constant value `133`. Under 32-bit MSVC Win32 that value is always odd, so the
  decrement branch is covered but the increment branch does not appear
  reachable. The surrounding spin stop-point cases are covered by
  `tests/backend/unit/r8/test_boss_8.c`, and the suspicious assignment/check
  shape is recorded in `docs/backend_potential_bugs.md`.

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

## `src/r4/coli4.c`

- `pcolspecial`, line 335: `cColiNo` is computed as
  `pColliAct->colino & 63`, so the subsequent `if (cColiNo > 63)` body is not
  reachable under 32-bit MSVC Win32. The `bossstart == 1` below-60 and 60-63
  paths are covered by `tests/backend/unit/r4/test_coli4.c`, and the redundant
  range check is recorded in `docs/backend_potential_bugs.md`.

## `src/r4/boss_4.c`

- `egg4air_01`, line 263: `E4A_Y` is a `Sint16`, so the
  `E4A_Y == 65344` fallback body does not appear reachable under 32-bit MSVC
  Win32. The compiled default route tables also only select `E4A_VEC == 2`
  with `E4A_Y == 1088`. Reachable route vectors, the dormant `E4A_VEC == 3`
  branch via a temporary test-table mutation, table-boundary cases, hit
  cooldowns, bomb spawning, air-head setup, gate cleanup, and boss escape/drop
  paths are covered by `tests/backend/unit/r4/test_boss_4.c`.

## `src/r4/boss_4_2.c`

- `ychg_ret`, line 895: the condition is written as
  `if (!(pEggwk->actfree[2]) & 8)`. Under 32-bit MSVC Win32, the logical-not
  result is `0` or `1`, so bitwise-and with `8` is always zero and the body does
  not appear reachable. The matching positive/negative rotation cases,
  `actfree[2] & 8` priority toggle, radius end normalization, bubble deletion,
  projectile cleanup, boss screen/demo/movement/damage/escape states, and mecha
  follower path are covered by `tests/backend/unit/r4/test_boss_4_2.c`; the
  suspicious condition is recorded in `docs/backend_potential_bugs.md`.

## `src/r4/game4.c`

- `sdfdout`, lines 605-614: the body is guarded by
  `if (!(pauseflag.b.h | 128))`. Under 32-bit MSVC Win32, bitwise-or with `128`
  is always nonzero, so logical-not always makes the condition false. The caller
  paths, pause toggles, `sdfdin`, DA selection, game init, water movement, and
  water collision behavior are covered by `tests/backend/unit/r4/test_game4.c`;
  the suspicious condition is recorded in `docs/backend_potential_bugs.md`.

## `src/r4/scr41a.c`

- `scrollwrtb`, line 849: `WrtTblCnt` is assigned from
  `(Uint16)(scrb_v_posit.w.h / 16)`, so the subsequent `if (WrtTblCnt < 0)`
  guard does not appear reachable under 32-bit MSVC Win32. The high clamp and
  active scroll-write paths are covered by `tests/backend/unit/r4/test_scr41a.c`.
- `mapadrset99`, line 1002: `i` is calculated after unsigned offset clamping,
  so the subsequent `if (i < 0)` guard does not appear reachable under 32-bit
  MSVC Win32. Empty-screen, populated-screen, and high-offset clamp paths are
  covered by `tests/backend/unit/r4/test_scr41a.c`.

## `src/r4/scr41b.c`

- `mapadrset99`, line 921: `i` is calculated after unsigned offset clamping,
  so the subsequent `if (i < 0)` guard does not appear reachable under 32-bit
  MSVC Win32. Empty-screen, populated-screen, `mapadrset1`, `mapadrset2`, and
  high-offset clamp paths are covered by `tests/backend/unit/r4/test_scr41b.c`.

## `src/r4/scr41c.c`

- `mapadrset99`, line 925: `i` is calculated after unsigned offset clamping,
  so the subsequent `if (i < 0)` guard does not appear reachable under 32-bit
  MSVC Win32. Empty-screen, populated-screen, `mapadrset1`, `mapadrset2`, and
  high-offset clamp paths are covered by `tests/backend/unit/r4/test_scr41c.c`.

## `src/r4/scr41d.c`

- `mapadrset99`, line 921: `i` is calculated after unsigned offset clamping,
  so the subsequent `if (i < 0)` guard does not appear reachable under 32-bit
  MSVC Win32. Empty-screen, populated-screen, `mapadrset1`, `mapadrset2`, and
  high-offset clamp paths are covered by `tests/backend/unit/r4/test_scr41d.c`.

## `src/r4/scr42a.c`

- `mapadrset99`, line 916: `i` is calculated after unsigned offset clamping,
  so the subsequent `if (i < 0)` guard does not appear reachable under 32-bit
  MSVC Win32. Empty-screen, populated-screen, `mapadrset1`, `mapadrset2`, and
  wrapped/high-offset clamp paths are covered by
  `tests/backend/unit/r4/test_scr42a.c`.

## `src/r4/scr42c.c`

- `mapadrset99`, line 917: `i` is calculated after unsigned offset clamping,
  so the subsequent `if (i < 0)` guard does not appear reachable under 32-bit
  MSVC Win32. Empty-screen, populated-screen, `mapadrset1`, `mapadrset2`, and
  wrapped/high-offset clamp paths are covered by
  `tests/backend/unit/r4/test_scr42c.c`.

## `src/r4/scr43c.c`

- `mapadrset99`, line 912: `i` is calculated after unsigned offset clamping,
  so the subsequent `if (i < 0)` guard does not appear reachable under 32-bit
  MSVC Win32. Empty-screen, populated-screen, `mapadrset1`, `mapadrset2`,
  saved-demo start-position, and wrapped/high-offset clamp paths are covered by
  `tests/backend/unit/r4/test_scr43c.c`.

## `src/r4/scr43d.c`

- `mapadrset99`, line 916: `i` is calculated after unsigned offset clamping,
  so the subsequent `if (i < 0)` guard does not appear reachable under 32-bit
  MSVC Win32. Empty-screen, populated-screen, `mapadrset1`, `mapadrset2`, and
  wrapped/high-offset clamp paths are covered by
  `tests/backend/unit/r4/test_scr43d.c`.

## `src/r5/game5.c`

- `sdfdout`, lines 600-609: the body is guarded by
  `if (!(pauseflag.b.h | 128))`. Under 32-bit MSVC Win32, bitwise-or with `128`
  is always nonzero, so logical-not always makes the condition false. The
  caller paths, pause toggles, `sdfdin`, DA selection, game init, flower setup,
  and normal/exit game loops are covered by `tests/backend/unit/r5/test_game5.c`;
  the suspicious condition is recorded in `docs/backend_potential_bugs.md`.

## `src/r5/scr51a.c`

- `scrollwrtb`, lines 890 and 892: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  write-table rows and avoid relying on the unsafe out-of-bounds table walk.
- `mapadrset99`, line 1045: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr51b.c`

- `scrollwrtb`, lines 859 and 861: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  rows, including a test-owned nonzero row mutation, and avoid relying on the
  unsafe out-of-bounds table walk.
- `mapadrset99`, line 1010: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr51c.c`

- `scrollwrtb`, lines 771 and 773: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  rows, including a test-owned nonzero row mutation, and avoid relying on the
  unsafe out-of-bounds table walk.
- `mapadrset99`, line 922: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr51d.c`

- `scrollwrtb`, lines 832 and 834: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  rows, including a test-owned nonzero row mutation, and avoid relying on the
  unsafe out-of-bounds table walk.
- `mapadrset99`, line 983: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr52a.c`

- `scrollwrtb`, lines 883 and 885: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of both `z81awrttbl` and `z81awrttbl2`, so the retained tests cover
  normal write-table rows and avoid relying on the unsafe out-of-bounds table
  walk.
- `mapadrset99`, line 1038: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr52b.c`

- `scrollwrtb`, lines 858 and 860: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  rows, including a test-owned nonzero row mutation, and avoid relying on the
  unsafe out-of-bounds table walk.
- `mapadrset99`, line 1009: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr52c.c`

- `scrollwrtb`, lines 771 and 773: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  rows, including a test-owned nonzero row mutation, and avoid relying on the
  unsafe out-of-bounds table walk.
- `mapadrset99`, line 922: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr52d.c`

- `scrollwrtb`, lines 832 and 834: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  rows, including a test-owned nonzero row mutation, and avoid relying on the
  unsafe out-of-bounds table walk.
- `mapadrset99`, line 983: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr53c.c`

- `scrollwrtb`, lines 704 and 706: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  rows, including a test-owned nonzero row mutation, and avoid relying on the
  unsafe out-of-bounds table walk.
- `mapadrset99`, line 855: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r5/scr53d.c`

- `scrollwrtb`, lines 747 and 749: `WrtTblCnt` is assigned from a `Uint16`
  expression, so the negative clamp does not appear reachable under 32-bit MSVC
  Win32. The high clamp starts a sixteen-entry walk at row `113`, beyond the
  safe range of the 65-byte `z81awrttbl`, so the retained tests cover normal
  rows, including a test-owned nonzero row mutation, and avoid relying on the
  unsafe out-of-bounds table walk.
- `mapadrset99`, line 898: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr71a.c`

- `scrollwrtb`, line 843: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 996: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r1/scr11a.c`

- `scrollwrtb`, line 914: `WrtTblCnt` is assigned from `scrb_v_posit.w.h / 16`,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 1067: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r1/scr12a.c`

- `scrollwrtb`, lines 782-783: `WrtTblCnt` is assigned from
  `scrb_v_posit.w.h / 16`, so the negative clamp does not appear reachable
  under 32-bit MSVC Win32.
- `scrollwrtb`, lines 784-785: forcing the high clamp requires a starting
  `WrtTblCnt` greater than the 33-entry `z12awrttbl` table can support during
  the following 16-entry walk. The retained tests cover zero flags, top/bottom
  row writes, nonzero write-table rows, and shifted row flags without driving
  the source into that out-of-bounds table walk.
- `mapadrset99`, lines 933-934: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the `scrbinit` sequential zero-initializers,
  map block modulo lines, and the four `SetGrid` callback lines even though the
  tests exercise the visible initialized values, return values, block pointer
  outputs, flip orders, and callback arguments.

## `src/r1/scr12b.c`

- `scrollc_h`, lines 319-321, and `scrollz_h`, lines 337-339: `lXsv` is set
  from the same post-addition expression as `lXwk`, so `lXwk - lXsv` is always
  zero under 32-bit MSVC Win32. The positive flag branch is covered; the
  negative branch does not appear reachable through these helpers.
- `scrollwrtb`, lines 699-700: `WrtTblCnt` is assigned from
  `scrb_v_posit.w.h / 16`, so the negative clamp does not appear reachable
  under 32-bit MSVC Win32.
- `scrollwrtb`, lines 701-702: forcing the high clamp requires a starting
  `WrtTblCnt` greater than the 33-entry `z12bwrttbl` table can support during
  the following 16-entry walk. The retained tests cover zero flags, top/bottom
  row writes, nonzero write-table rows, and shifted row flags without driving
  the source into that out-of-bounds table walk.
- `mapadrset99`, lines 843-844: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r1/scr12c.c`

- `scrollwrtb`, lines 692-693: `WrtTblCnt` is assigned from
  `scrb_v_posit.w.h / 16`, so the negative clamp does not appear reachable
  under 32-bit MSVC Win32.
- `scrollwrtb`, lines 694-695: forcing the high clamp requires a starting
  `WrtTblCnt` greater than the 33-entry `z12cwrttbl` table can support during
  the following 16-entry walk. The retained tests cover zero flags, top/bottom
  row writes, nonzero write-table rows, shifted row flags, and the high
  hscroll-fill branch without driving the source into that out-of-bounds table
  walk.
- `mapadrset99`, lines 837-838: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r1/scr12d.c`

- `scrollwrtb`, lines 658-659: `WrtTblCnt` is assigned from
  `scrb_v_posit.w.h / 16`, so the negative clamp does not appear reachable
  under 32-bit MSVC Win32.
- `scrollwrtb`, lines 660-661: forcing the high clamp requires a starting
  `WrtTblCnt` greater than the 33-entry `z12dwrttbl` table can support during
  the following 16-entry walk. The retained tests cover zero flags, top/bottom
  row writes, nonzero write-table rows, shifted row flags, and hscroll-fill
  behavior without driving the source into that out-of-bounds table walk.
- `mapadrset99`, lines 803-804: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r1/scr11b.c`

- `scrollwrtb`, line 661: `WrtTblCnt` is assigned from `scrb_v_posit.w.h / 16`,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32.
- `scrollwrtb`, line 663: forcing the high clamp requires a starting
  `WrtTblCnt` greater than the 33-entry `z11bwrttbl` table can support during
  the following 16-entry walk. The retained tests cover zero flags, top/bottom
  row writes, nonzero write-table rows, and shifted row flags without driving
  the source into that out-of-bounds table walk.
- `mapadrset99`, line 809: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r1/scr11c.c`

- `scrollwrtb`, line 673: `WrtTblCnt` is assigned from `scrb_v_posit.w.h / 16`,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32.
- `scrollwrtb`, line 675: forcing the high clamp requires a starting
  `WrtTblCnt` greater than the 33-entry `z11cwrttbl` table can support during
  the following 16-entry walk. The retained tests cover zero flags, both
  initial horizontal write variants, nonzero write-table rows, and shifted row
  flags without driving the source into that out-of-bounds table walk.
- `mapadrset99`, line 821: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr71b.c`

- `scrollwrtb`, line 842: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 995: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr71c.c`

- `scrollwrtb`, line 814: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, seeded nonzero
  write-table rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 967: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr71d.c`

- `scrollwrtb`, line 821: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, seeded nonzero
  write-table rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 974: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr72a.c`

- `scrollwrtb`, line 843: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 996: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr72b.c`

- `scrollwrtb`, line 842: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 995: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr72c.c`

- `scrollwrtb`, line 814: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, seeded nonzero
  write-table rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 967: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr72d.c`

- `scrollwrtb`, line 821: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 974: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr73c.c`

- `scrollwrtb`, line 813: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, seeded nonzero
  write-table rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 966: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r7/scr73d.c`

- `scrollwrtb`, line 820: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 973: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.

## `src/r6/scr61a.c`

- `scrollwrtb`, line 712: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 862: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr61b.c`

- `scrollwrtb`, line 753: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 903: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr61c.c`

- `scrollwrtb`, line 710: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 860: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr61d.c`

- `scrollwrtb`, line 712: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 862: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr62a.c`

- `scrollwrtb`, line 696: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 846: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr62b.c`

- `scrollwrtb`, line 741: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 891: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr62c.c`

- `scrollwrtb`, line 696: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 846: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr62d.c`

- `scrollwrtb`, line 696: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 846: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr63c.c`

- `scrollwrtb`, line 712: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 862: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/r6/scr63d.c`

- `scrollwrtb`, line 710: `WrtTblCnt` is assigned from a `Uint16` expression,
  so the negative clamp does not appear reachable under 32-bit MSVC Win32. The
  retained tests cover zero flags, top/bottom row writes, nonzero write-table
  rows, shifted row flags, and the high clamp.
- `mapadrset99`, line 860: `xOffs` and `yOffs` are unsigned inputs and are
  clamped to nonnegative map ranges before `i` is calculated, so the
  negative-index correction does not appear reachable through the public map
  address helpers.
- MSVC reports partial coverage on the map block modulo lines and the four
  `SetGrid` callback lines even though the tests exercise the visible return
  values, block pointer outputs, flip orders, and callback arguments.

## `src/game.c`

- `sdfdout`, lines 613-622: the body is guarded by
  `if (!(pauseflag.b.h | 128))`. Under 32-bit MSVC Win32, bitwise-or with `128`
  is always nonzero, so logical-not always makes the condition false. The
  caller paths, pause toggles, `sdfdin`, DA selection, game init, flower setup,
  and normal/exit game loops are covered by `tests/backend/unit/test_game.c`;
  the suspicious condition is recorded in `docs/backend_potential_bugs.md`.

## `src/title/savedata/svdsprt.c`

- `CreateCharSprt`, lines 134 and 155, and `DeleteCharSprt`, line 181: the
  block guard is `nBlockNo >= 2 || nBlockNo < 9`, which is true for every
  integer value. The final `return 0` path cannot be reached through any input,
  and tests avoid out-of-range block numbers because the guarded body indexes
  `hSprFile[nBlockNo - 2]`.

## `src/title/opening/opndo.c`

- `GetNextMenu`, line 320: if all seven menu entries are disabled, the function
  reaches the end without returning a value. The test suite covers forward and
  backward searches with enabled entries and avoids the undefined no-entry
  result.
- `OEUpdatePlanet`, line 144: the `++num > 7` reset appears unreachable through
  the `ptPlnt` table because entry `7` is the `time == -1` sentinel handled by
  the following branch before `num` can be incremented past `7`.

## `src/title/common/hmx_oeeactl.c`

- `ld_load_sprite1`, lines 394-395 and 404: `wx` is assigned from
  `(s->wx + 7) / 8 * 8` immediately before the `if (wx & 4)` guard, so under
  32-bit MSVC Win32 the checked value is always a multiple of 8. The width
  adjustment and corresponding reversal remain unreachable through the loader
  entry point. `ld_load_sprite2` retains the analogous branch coverage through
  its `i == 3` special case, which uses the raw sprite width instead.

## `src/title/thanks/sprmove.c`

- `sonicinit`, lines 140-142: the preceding code forces `ld0.w.l = 1`, so the
  `else` branch cannot be reached under 32-bit MSVC Win32. The forced branch is
  also recorded in `docs/backend_potential_bugs.md`.
- `s_metalchk`, line 252: MSVC coverage does not credit the bare `} else {`
  line even though the tests cover the positive-speed path body at lines
  253-254 and the fallthrough continuation at line 256.

## `src/warp/warp.c`

- `clchg`, line 307: MSVC reports the counter-reset assignment as partial even
  though `tests/backend/unit/warp/test_warp.c` drives `clchg_cnt[1]` through
  the `>= 14` wrap case and verifies that the counter returns to zero.

## `src/title/planet/pls.c`

- `play1`, lines 39 and 43: MSVC reports the key-up assignments as partial even
  though `tests/backend/unit/title/planet/test_pls.c` drives both button-down
  and button-up controller states through the `sPeriPadGet` callback and
  verifies the resulting `keyBuf` values.

## `src/title/planet/plm.c`

- Lines 82, 184, 224, 330, and 366: MSVC reports the modulo assignment lines as
  partial. `tests/backend/unit/title/planet/test_plm.c` drives the visible
  branch outcomes and verifies the resulting actor positions, speeds, flags,
  timers, and pattern selections under 32-bit MSVC Win32.

## `src/title/planet/chamov.c`

- Lines 293, 315, 339, 451, 453, and 646: MSVC reports the modulo or
  fixed-point assignment lines as partial. `tests/backend/unit/title/planet/test_chamov.c`
  drives the surrounding branches and verifies the resulting actor positions,
  speeds, flags, timers, pattern selections, endpoint targets, and state
  transitions under 32-bit MSVC Win32.

## `src/title/planet/lplmain.c`

- `game_init`, lines 251-254: the body after `if (init_dsp() == 0)` is not
  reachable because `init_dsp()` always returns `0` under 32-bit MSVC Win32.
  `tests/backend/unit/title/planet/test_lplmain.c` covers the normal startup
  cases and direct display initialization behavior.
- `cgdata_change`, line 590, and the partial switch at line 576: the default
  case would require an invalid `comdata_m5` value, but the function indexes
  `ScrAMapFileName[comdata_m5]` before the switch. Tests cover the valid map
  indices `0`, `1`, and `2`, plus the explicit `3` no-change path.

## `src/special/act_s.c`

- `n_patset`, lines 179-180: the switch expression is `sprdat.etc & 24`, so
  the only possible values are `0`, `8`, `16`, and `24`. The default case is
  not reachable through any `sprite_data` value.
- `zbuf_init`, line 52, and `zbuf_set`, line 67: MSVC reports the loop body and
  sentinel write as partial even though `tests/backend/unit/special/test_act_s.c`
  covers empty buckets, occupied buckets, clamped depth, full-bucket insertion,
  and the z-buffer pattern traversal paths under 32-bit MSVC Win32.

## `src/special/ens.c`

- `ptset_ufo`, line 646: `d0l` is clamped to `1280` immediately before it is
  shifted right by four bits, so the following `if (d0l > 80)` guard does not
  appear reachable under 32-bit MSVC Win32. The surrounding distance clamp,
  pattern update, unchanged-pattern return, and z-buffer/scaling side effects
  are covered by `tests/backend/unit/special/test_ens.c`.
- Lines 254, 302, 333, 428, 727, 756, 792, and 950: MSVC reports the actor
  dispatch `switch` lines as partial. The test covers every listed case for
  those dispatchers; the source does not provide default arms for out-of-range
  state values.

## `src/title/ta/ta.c`

- `game_init`, line 391: MSVC does not credit the bare `} else {` line even
  though `tests/backend/unit/title/ta/test_ta.c` covers the special-stage
  record insertion path at lines 392-394 and verifies the resulting inserted
  time/name state under 32-bit MSVC Win32.

## `src/title/ta/taeactrl.c`

- `CreatePic`, line 576: the failure return after `if (LoadPicBmp() != 0)` is
  not reachable because `LoadPicBmp()` always returns `0` under 32-bit MSVC
  Win32. `tests/backend/unit/title/ta/test_taeactrl.c` covers successful
  picture allocation/loading/copying plus the direct allocation-failure returns
  before that guard.

## `src/r3/scr31a.c`

- `scroll`, line 323: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `87`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `scrollwrtb`, line 991: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 993: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z31awrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1141: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1104-1107, 1151, 1154, 1181, and 1184: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr31a.c`
  drives all block flip modes, visible and hidden block writes, normal and
  clamped map lookups, and `mapadrset2` block-pointer lookup behavior.

## `src/r3/scr31b.c`

- `scroll`, line 329: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `91`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `zonescrsetsub0`, line 424: this fall-through writer branch is not reached
  by the safe table-boundary cases covered by the shared R31 scroll harness.
- `scrollwrtb`, line 995: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 997: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z31bwrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1145: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1108-1111, 1155, 1158, 1185, and 1188: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr31b.c`
  runs the shared R31 scroll harness against `src/r3/scr31b.c`, driving all
  block flip modes, visible and hidden block writes, normal and clamped map
  lookups, and `mapadrset2` block-pointer lookup behavior.

## `src/r3/scr31c.c`

- `scroll`, line 320: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `87`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `scrollwrtb`, line 986: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 988: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z31cwrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1136: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1099-1102, 1146, 1149, 1176, and 1179: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr31c.c`
  runs the shared R31 scroll harness against `src/r3/scr31c.c`, driving all
  block flip modes, visible and hidden block writes, normal and clamped map
  lookups, and `mapadrset2` block-pointer lookup behavior.

## `src/r3/scr31d.c`

- `scroll`, line 304: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `29`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `scrollwrtb`, line 970: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 972: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z31dwrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1120: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1083-1086, 1130, 1133, 1160, and 1163: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr31d.c`
  runs the shared R31 scroll harness against `src/r3/scr31d.c`, driving all
  block flip modes, visible and hidden block writes, normal and clamped map
  lookups, and `mapadrset2` block-pointer lookup behavior.

## `src/r3/scr32a.c`

- `scroll`, line 320: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `87`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `scrollwrtb`, line 986: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 988: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z32awrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1136: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1099-1102, 1146, 1149, 1176, and 1179: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr32a.c`
  runs the shared scroll harness with the R32 map layout and stage limits.

## `src/r3/scr32b.c`

- `scroll`, line 329: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `91`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `zonescrsetsub0`, lines 414 and 424: these fall-through writer branches are
  not reached by the safe table-boundary cases covered by the shared scroll
  harness.
- `scrollwrtb`, line 995: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 997: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z32bwrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1145: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1108-1111, 1155, 1158, 1185, and 1188: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr32b.c`
  runs the shared scroll harness with the R32 map layout and stage limits.

## `src/r3/scr32c.c`

- `scroll`, line 320: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `87`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `scrollwrtb`, line 986: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 988: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z32cwrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1136: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1099-1102, 1146, 1149, 1176, and 1179: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr32c.c`
  runs the shared scroll harness with the R32 map layout and stage limits.

## `src/r3/scr32d.c`

- `scroll`, line 304: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `29`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `scrollwrtb`, line 970: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 972: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z32dwrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1120: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1083-1086, 1130, 1133, 1160, and 1163: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr32d.c`
  runs the shared scroll harness with the R32 map layout and stage limits.

## `src/r3/scr33c.c`

- `scroll`, line 320: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `87`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `scrollwrtb`, line 986: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 988: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z33cwrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1136: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1099-1102, 1146, 1149, 1176, and 1179: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr33c.c`
  runs the shared scroll harness with the R33 map layout and stage limits.

## `src/r3/scr33d.c`

- `scroll`, line 306: the `else` after the water setup range check does not
  appear reachable through `scroll()` because `scrollb_v()` is recomputed just
  before the check and keeps the derived `wD3` value at or below the `29`
  threshold under the characterized 32-bit MSVC Win32 paths.
- `scrollwrtb`, line 972: `WrtTblCnt` is computed from an unsigned scroll
  position divided by 16, so the `< 0` clamp is not reachable.
- `scrollwrtb`, line 974: the `> 113` clamp would leave `WrtTblCnt` pointing
  beyond the 49-byte `z33dwrttbl` before the following loop reads it, so the
  test avoids forcing that unsafe table-read path.
- `mapadrset99`, line 1122: `i` is computed from clamped unsigned tile offsets,
  so the `< 0` clamp is not reachable.
- Lines 1085-1088, 1132, 1135, 1162, and 1165: MSVC reports these block-write
  and modulo assignment lines as partial. `tests/backend/unit/r3/test_scr33d.c`
  runs the shared scroll harness with the R33 map layout and stage limits.
