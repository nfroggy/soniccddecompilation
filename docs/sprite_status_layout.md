# `sprite_status` 32-Bit Layout

This documents the byte layout of `sprite_status` from `src/types.h` when compiled with a 32-bit compiler using the default MSVC packing rules.

Verified with 32-bit MSVC:

```text
sizeof(void*)         = 4
alignof(sprite_status)= 4
sizeof(sprite_status) = 68
```

Offsets and sizes are in bytes. Hex offsets are included because much of the decompiled code uses byte/word/long slot casts.

## Top-Level Fields

| Field | Offset | Hex | Size |
|---|---:|---:|---:|
| `actno` | 0 | `0x00` | 1 |
| `actflg` | 1 | `0x01` | 1 |
| `sproffset` | 2 | `0x02` | 2 |
| `patbase` | 4 | `0x04` | 4 |
| `xposi` | 8 | `0x08` | 4 |
| `yposi` | 12 | `0x0C` | 4 |
| `xspeed` | 16 | `0x10` | 2 |
| `yspeed` | 18 | `0x12` | 2 |
| `mspeed` | 20 | `0x14` | 2 |
| `sprhsize` | 22 | `0x16` | 1 |
| `sprvsize` | 23 | `0x17` | 1 |
| `sprhs` | 24 | `0x18` | 1 |
| `sprpri` | 25 | `0x19` | 1 |
| `patno` | 26 | `0x1A` | 1 |
| padding | 27 | `0x1B` | 1 |
| `mstno` | 28 | `0x1C` | 2 |
| `patcnt` | 30 | `0x1E` | 1 |
| `pattim` | 31 | `0x1F` | 1 |
| `pattimm` | 32 | `0x20` | 1 |
| `colino` | 33 | `0x21` | 1 |
| `colicnt` | 34 | `0x22` | 1 |
| `cddat` | 35 | `0x23` | 1 |
| `cdsts` | 36 | `0x24` | 1 |
| `r_no0` | 37 | `0x25` | 1 |
| `r_no1` | 38 | `0x26` | 1 |
| padding | 39 | `0x27` | 1 |
| `direc` | 40 | `0x28` | 2 |
| `userflag` | 42 | `0x2A` | 2 |
| `dummy` | 44 | `0x2C` | 2 |
| `actfree` | 46 | `0x2E` | 22 |

## Union Members

The union views overlap their parent field. For example, `xposi.l`, `xposi.w`, and `xposi.b` all start at offset `0x08`.

| Element | Offset | Hex | Size |
|---|---:|---:|---:|
| `xposi.l` | 8 | `0x08` | 4 |
| `xposi.w` | 8 | `0x08` | 4 |
| `xposi.w.l` | 8 | `0x08` | 2 |
| `xposi.w.h` | 10 | `0x0A` | 2 |
| `xposi.b` | 8 | `0x08` | 4 |
| `xposi.b.b4` | 8 | `0x08` | 1 |
| `xposi.b.b3` | 9 | `0x09` | 1 |
| `xposi.b.b2` | 10 | `0x0A` | 1 |
| `xposi.b.b1` | 11 | `0x0B` | 1 |
| `yposi.l` | 12 | `0x0C` | 4 |
| `yposi.w` | 12 | `0x0C` | 4 |
| `yposi.w.l` | 12 | `0x0C` | 2 |
| `yposi.w.h` | 14 | `0x0E` | 2 |
| `yposi.b` | 12 | `0x0C` | 4 |
| `yposi.b.b4` | 12 | `0x0C` | 1 |
| `yposi.b.b3` | 13 | `0x0D` | 1 |
| `yposi.b.b2` | 14 | `0x0E` | 1 |
| `yposi.b.b1` | 15 | `0x0F` | 1 |
| `xspeed.w` | 16 | `0x10` | 2 |
| `xspeed.b` | 16 | `0x10` | 2 |
| `xspeed.b.l` | 16 | `0x10` | 1 |
| `xspeed.b.h` | 17 | `0x11` | 1 |
| `yspeed.w` | 18 | `0x12` | 2 |
| `yspeed.b` | 18 | `0x12` | 2 |
| `yspeed.b.l` | 18 | `0x12` | 1 |
| `yspeed.b.h` | 19 | `0x13` | 1 |
| `mspeed.w` | 20 | `0x14` | 2 |
| `mspeed.b` | 20 | `0x14` | 2 |
| `mspeed.b.l` | 20 | `0x14` | 1 |
| `mspeed.b.h` | 21 | `0x15` | 1 |
| `mstno.w` | 28 | `0x1C` | 2 |
| `mstno.b` | 28 | `0x1C` | 2 |
| `mstno.b.l` | 28 | `0x1C` | 1 |
| `mstno.b.h` | 29 | `0x1D` | 1 |
| `direc.w` | 40 | `0x28` | 2 |
| `direc.b` | 40 | `0x28` | 2 |
| `direc.b.l` | 40 | `0x28` | 1 |
| `direc.b.h` | 41 | `0x29` | 1 |
| `userflag.w` | 42 | `0x2A` | 2 |
| `userflag.b` | 42 | `0x2A` | 2 |
| `userflag.b.l` | 42 | `0x2A` | 1 |
| `userflag.b.h` | 43 | `0x2B` | 1 |

## Array Elements

| Element | Offset | Hex | Size |
|---|---:|---:|---:|
| `dummy[0]` | 44 | `0x2C` | 1 |
| `dummy[1]` | 45 | `0x2D` | 1 |
| `actfree[0]` | 46 | `0x2E` | 1 |
| `actfree[1]` | 47 | `0x2F` | 1 |
| `actfree[2]` | 48 | `0x30` | 1 |
| `actfree[3]` | 49 | `0x31` | 1 |
| `actfree[4]` | 50 | `0x32` | 1 |
| `actfree[5]` | 51 | `0x33` | 1 |
| `actfree[6]` | 52 | `0x34` | 1 |
| `actfree[7]` | 53 | `0x35` | 1 |
| `actfree[8]` | 54 | `0x36` | 1 |
| `actfree[9]` | 55 | `0x37` | 1 |
| `actfree[10]` | 56 | `0x38` | 1 |
| `actfree[11]` | 57 | `0x39` | 1 |
| `actfree[12]` | 58 | `0x3A` | 1 |
| `actfree[13]` | 59 | `0x3B` | 1 |
| `actfree[14]` | 60 | `0x3C` | 1 |
| `actfree[15]` | 61 | `0x3D` | 1 |
| `actfree[16]` | 62 | `0x3E` | 1 |
| `actfree[17]` | 63 | `0x3F` | 1 |
| `actfree[18]` | 64 | `0x40` | 1 |
| `actfree[19]` | 65 | `0x41` | 1 |
| `actfree[20]` | 66 | `0x42` | 1 |
| `actfree[21]` | 67 | `0x43` | 1 |

## Common Cast Slot Offsets

These are not separate fields, but they are useful when reading decompiled expressions such as `((Sint32*)pActwk)[12]`.

| Cast slot | Byte offset | Hex |
|---|---:|---:|
| `((Uint8*)pActwk)[n]` | `n` | varies |
| `((Sint16*)pActwk)[n]` | `n * 2` | varies |
| `((Sint32*)pActwk)[n]` | `n * 4` | varies |
| `((void**)pActwk)[n]` on 32-bit | `n * 4` | varies |

Because pointer slots are 4 bytes only in the 32-bit layout, expressions such as `((Uint8***)pActwk)[13]` refer to byte offset `52` (`0x34`) on 32-bit builds.
