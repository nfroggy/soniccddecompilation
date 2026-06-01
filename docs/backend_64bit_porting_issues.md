# Backend 64-Bit Porting Issues

This note summarizes the broad categories of backend code that are likely to break,
miscompile, or corrupt memory when the backend DLLs are built as 64-bit code. It is
not an exhaustive bug list; it is a map of the recurring patterns to audit first.

The short version is that much of the backend is still shaped like a 32-bit
decompilation. Some code merely uses fixed-width game values, which is fine. The
problematic code is where pointer-sized C objects are mixed with 32-bit layout
assumptions.

## Actor Work Struct Layout

The biggest issue is `sprite_status`. On 32-bit MSVC it is a 68-byte object, and
much of the code treats it as a byte/word/long register block. See
`docs/sprite_status_layout.md` for the verified 32-bit layout.

On 64-bit, `sprite_pattern **patbase` becomes 8 bytes and requires 8-byte
alignment. That changes the offsets of the fields that follow it and grows the
struct. Code like this then stops referring to the intended bytes:

- `src/branko1.c`: `((Sint16 *)pActwk)[29]`
- `src/enemy.c`: `((Sint32 *)pActwk)[12]`
- `src/r4/tobiras4.c`: `((Sint16 *)pActwk)[23]`

For example, on the 32-bit layout `((Sint32 *)pActwk)[12]` means byte offset
`48`, which is inside `actfree`. On a natural 64-bit layout, byte offset `48`
lands around `direc`/`userflag`, not the same scratch field.

Packing the struct is not enough to fix this, because the pointer field is still
8 bytes. The actor layout needs either pointer-free 32-bit storage for the
decompiled work area, or the cast-slot accesses need to be replaced with named
fields that do not depend on the old byte offsets.

## Pointers Stored In Actor Scratch Slots

Several actors store real C pointers inside `sprite_status` by casting the whole
actor to a pointer array. This only works when a pointer is 4 bytes.

Examples:

- `src/enemy.c`: `((Uint8 ***)pActwk)[13] = pchg_e;`
- `src/enemy.c`: `patchg(pActwk, ((Uint8 ***)pActwk)[13]);`
- `src/r4/tobiras4.c`: `((Uint8 **)pActwk)[13] = p;`
- `src/r4/wall42.c`: `((Uint8 **)pActwk)[12] = a1;`
- `src/r5/shoot5.c`: `((Sint16 **)pActwk)[16] = a2;`
- `src/special/ens.c`: `((Sint16 **)a2)[20] = a1tbl;`

On 32-bit, `((Uint8 ***)pActwk)[13]` writes at byte offset `52`, inside
`actfree`. On 64-bit, slot `13` is byte offset `104`, which is past the end of a
normal 64-bit `sprite_status`. That is immediate out-of-bounds memory
corruption.

This category should be fixed by replacing pointer-in-scratch-slot storage with
explicit fields, stable indices, or side tables keyed by actor index.

## Shared DLL ABI Uses Untyped Positional Tables

The round, special, and title DLLs receive frontend services through positional
tables rather than typed structs:

- `src/game.c`: `void DLL_meminit(char ***pBufTbl, void **pFuncTbl)`
- `src/r4/game4.c`: same `DLL_meminit` pattern
- `src/title/*/*entry.c`: `DLLInit(engine_dll *lpDllIn, char ***pBufTbl, void **pFuncTbl)`
- `src/frontend/globals.c`: defines the frontend-side `memoryTbl` and `functionTbl`

This can work in a 64-bit build only if every DLL and the frontend are rebuilt
with exactly matching pointer sizes, slot order, and function prototypes. It is
very easy to be off by one slot or to assign a function through the wrong
prototype.

Specific examples of prototype drift:

- `src/game.c` declares `sPrintf` as `void (*)(char *, const char *, ...)`.
- `src/title/soundtst/sndentry.c` declares `sPrintf` as `void (*)(char *, char *)`.
- Several title DLLs declare `CDPause` as `void (*)(void)`, while the frontend
  implementation is `void CDPause(Sint16 mode)` in `src/frontend/sound.c`.

On x86, calling convention accidents are often masked by the stack-based ABI. On
x64, the Windows calling convention uses registers and has stricter varargs
rules, so mismatched prototypes are much more likely to pass bad arguments or
break when optimized.

## 32-Bit Handles And Pointer-Sized Values

Some title/HMX interfaces store handles or pointer-like values in `Uint32`.
That matches the original 32-bit executable but truncates real pointers or
handles in a 64-bit process.

Examples:

- `src/title/common/engine_dll.h`: `Uint32 hWnd;` and `Uint32 hSurf;`
- `src/title/ta/ta.c`: `Uint32 ghWnd = 0;` and `Uint32 hSurf = 0;`
- `src/title/opening/opnentry.c`: `Uint32 hSurf = 0;`
- `src/title/common/hmx_types.h`: `Uint32 **lphBmp`, `Uint32 *lphTile`,
  `Uint32 *lphGrid`, `Uint32 *hBmp`, and `Uint32 *lphSpr`

The winexe reference shows the same original assumption, for example
`src/winexe/dll.c` casts `hWnd` and `hSurf` to `UINT` before passing them to
title DLLs. A 64-bit frontend should use `uintptr_t`, `intptr_t`, or real opaque
pointer/handle typedefs for these values instead of `Uint32`.

## Pointer-Size-Dependent Shared Structs

Some shared structs are passed across DLL boundaries and contain pointers. They
are safe only if both sides agree on the exact 64-bit layout.

Examples:

- `src/types.h`: `game_info` contains `void *pSprBmp`, `Uint16 *ptv_adr`, and
  `Uint16 *sm_adr0`.
- `src/title/common/hmx_types.h`: `draw_context` contains many HMX object
  pointers and arrays of pointers.
- `src/title/common/engine_dll.h`: `engine_dll` contains many callback pointers
  as `void *`.

If a 32-bit DLL talks to a 64-bit frontend, this cannot work at all. If all
modules are 64-bit, it can work, but the ABI should still be made explicit so
that a future field reorder, packing change, or compiler difference does not
silently desynchronize the frontend and backend.

## Type Punning, Endianness, And Strict Aliasing

The backend frequently reinterprets data through incompatible pointer types or
unions to access low/high bytes and words.

Examples:

- `src/loader2.c`: `((short_union *)&StageNo)->b.h`
- `src/playsub.c`: `((ushort_union *)&ppw_offset)->b.l`
- `src/dircol.c`: manually splits `Sint32` values into 16-bit pieces
- Many files use `int_union`, `uint_union`, `short_union`, and `ushort_union`
  as register views.

This is not inherently a 64-bit pointer truncation problem, but it becomes more
fragile when moving compilers or enabling stronger optimization. The code assumes
little-endian layout and MSVC-style tolerance for aliasing. A portable 64-bit
backend should replace these views with helper functions/macros for high/low
byte and word access.

## Whole-Object Copies And Clears

The actor system often copies or clears entire `sprite_status` objects:

- `src/action.c`: `frameout` clears `sizeof(*pActwk)`.
- `src/r4/tobiras4.c`: `sMemCpy(pNewActwk, pActwk, sizeof(*pActwk));`
- `src/actset.c`: actor allocation scans the global `actwk` array as
  `sprite_status` objects.

These are fine only if the real runtime actor object is allowed to grow. In this
codebase, however, many actor fields are addressed by 32-bit byte offsets. Once
`sizeof(sprite_status)` and internal offsets change, these copies preserve the
wrong layout and can copy pointers into places where old code expects 16-bit or
32-bit scratch values.

## Winexe Reference Code Has Additional 32-Bit API Assumptions

This is mostly outside the backend DLLs, but it is useful context because the
backend was decompiled against this ABI. The winexe reference code contains many
Win32-era casts that are not valid on 64-bit Windows:

- `src/winexe/cdaudio.c`: casts parameter pointers to `DWORD` for `mciSendCommand`.
- `src/winexe/wave.c`: stores a pointer in `waveHdr.dwUser` via `(DWORD)pWaveInfo`.
- `src/winexe/winmain.c`: compares module handles with `(ULONG)ghGameStageDll < 32`.
- `src/winexe/dll.c`: casts `HWND`/surface handles to `UINT` for `engine_dll`.

These examples explain why the backend headers still expose 32-bit handle fields.
They should not be copied into a 64-bit frontend.

## Suggested Audit Order

1. Stabilize `sprite_status` so its decompiled byte/word/long slots mean the
   same thing on every target.
2. Remove all pointer-array casts on actor objects, or replace them with actor
   side tables.
3. Change pointer/handle storage in title/HMX interfaces from `Uint32` to
   `uintptr_t` or typed opaque pointers.
4. Replace the untyped positional function tables with typed structs, or at
   least centralize the slot definitions and static-check every prototype.
5. Add compile-time checks for all ABI structs shared across DLL boundaries.
6. Only after the layout is stable, enable 64-bit warnings and fix the remaining
   pointer truncation, aliasing, and prototype warnings.

