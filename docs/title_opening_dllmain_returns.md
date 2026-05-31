# `title/opening` `DLLMain` Return Values

This documents the values returned by `src/title/opening/opnentry.c`'s
`DLLMain()` and how `src/winexe/dll.c` interprets them.

## Where Returns Come From

`DLLMain()` normally returns `0`. The only nonzero returns come from
`OEEnd()` when `nSequenceNum == 999`:

```c
case 999:
    ret = OEEnd();
    break;
```

The title code reaches sequence `999` by starting a fade-out and setting
`nFadoSeqNum = 999`. `OEEnd()` only returns a nonzero value when
`nTimerCunt == 0`, so each transition code is emitted for one tick. It also
calls `CDPause(2)` on that tick.

The normal menu maps `kind - 25` into `nMenuKind`. There are also three hidden
input sequences and one idle timeout path that set `nMenuKind` directly.

## Opening Return Codes

| `DLLMain()` return | Opening source | Meaning in opening | `winexe` behavior |
| --- | --- | --- | --- |
| `0` | Default return | Keep running the current opening/title sequence. Also returned after the first `OEEnd()` tick. | `changeMenuState()` does nothing and returns `0`. |
| `1` | `nMenuKind == 0` | New Game menu item. | Unloads opening and returns `1`; `winmain.c` handles this as `New Game`. |
| `2` | `nMenuKind == 1` | Continue menu item. | Unloads opening and returns `2`; `winmain.c` loads the saved game stage. |
| `3` | `nMenuKind == 2` | Time Attack menu item. | Unloads opening and returns `3`; `winmain.c` calls `loadTimeAttack(TRUE)`. |
| `4` | `nMenuKind == 3` | Save Data menu item. | Unloads opening and loads `savedata.dll`; `changeMenuState()` returns `0`. |
| `5` | `nMenuKind == 4` | D.A. Garden / Little Planet menu item. | Unloads opening and returns `4`; `winmain.c` calls `loadPlanet()`. |
| `6` | `nMenuKind == 5` | Visual Mode menu item. | Unloads opening and loads `visualmd.dll`; `changeMenuState()` returns `0`. |
| `7` | `nMenuKind == 7` | Idle timeout demo path. Set after 900 title-menu ticks without input. | Unloads opening and returns `5`; `winmain.c` enters the demo/movie cycle. |
| `13` | `nMenuKind == 8` | Sound Test hidden-code path. | Unloads opening and loads `soundtst.dll`; `changeMenuState()` returns `0`. |
| `14` | `nMenuKind == 9` | Stage Select hidden-code path. | Unloads opening and loads `stagetst.dll`; `changeMenuState()` returns `0`. |
| `20` | `nMenuKind == 10` | Best Time hidden-code path. | Unloads opening and loads `besttime.dll`; `changeMenuState()` returns `0`. |
| `21` | `nMenuKind == 6` | Exit menu item (`EXIT.BMP`). | `winexe` has no `case 21` in `changeMenuState()`, so this return is ignored by the observed host code. |

## Menu Enable Notes

`GetEnabeMenu()` controls which normal menu items can be reached:

| `nMenuKind` | Normal item | Enable condition |
| --- | --- | --- |
| `0` | New Game | Enabled unless all six save slots are occupied by saved games. |
| `1` | Continue | Enabled when `crntScorData.saved == 1`. |
| `2` | Time Attack | Enabled when `crntScorData.roundNo != 0`. |
| `3` | Save Data | Enabled when any score data exists. |
| `4` | D.A. Garden / Little Planet | Enabled when `roundNo >= 7 && total < 134734`. |
| `5` | Visual Mode | Enabled when `roundNo >= 7 && total < 92767`. |
| `6` | Exit | Always enabled by the opening menu. |

## Hidden Paths

The title menu also watches for three hidden input sequences while the initial
`START GAME` prompt is visible:

| `nMenuKind` | Return | Destination |
| --- | --- | --- |
| `8` | `13` | Sound Test |
| `9` | `14` | Stage Select |
| `10` | `20` | Best Time |

These are separate from the normal left/right menu item flow.

## Codes Handled By `winexe` But Not Returned Here

`changeMenuState()` handles several other values (`8`, `9`, `10`, `11`, `12`,
`15`, `16`, `17`, `18`, `19`) for other menu/movie DLLs or other host paths.
`title/opening`'s `DLLMain()` does not return those values in the current code.
