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
