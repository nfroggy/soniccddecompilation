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
