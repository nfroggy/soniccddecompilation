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

## `src/enemy.c`

- `ka_move`, lines 312-315: this patrol-turn block is reached when the mosquito
  actor is outside its 128-pixel home band. With the actor initialized through
  `ka_init` and then placed outside that band by visible position fields, the
  `do while (1)` loop toggles the internal speed/flip state and does not return
  to a covered movement path. The normal patrol, player-triggered drop, turn
  animation, drop, stop, and wrapper dispatch paths are covered by
  `tests/backend/unit/test_enemy.c`; the non-returning outside-band behavior is
  recorded in `docs/backend_potential_bugs.md`.
