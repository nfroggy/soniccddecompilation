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
