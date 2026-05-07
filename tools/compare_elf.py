import sys
import struct
from pathlib import Path


SHT_PROGBITS = 1
SHF_ALLOC = 0x2
DIFF_LIMIT = 40


def u16(data: bytes, offset: int) -> int:
    return struct.unpack_from("<H", data, offset)[0]


def u32(data: bytes, offset: int) -> int:
    return struct.unpack_from("<I", data, offset)[0]


def parse_elf(data: bytes) -> dict:
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        raise ValueError("expected 32-bit little-endian ELF")

    phoff = u32(data, 0x1C)
    shoff = u32(data, 0x20)
    phentsize = u16(data, 0x2A)
    phnum = u16(data, 0x2C)
    shentsize = u16(data, 0x2E)
    shnum = u16(data, 0x30)
    shstrndx = u16(data, 0x32)

    phdrs = []
    for index in range(phnum):
        offset = phoff + index * phentsize
        p_type, p_offset, p_vaddr, p_paddr, p_filesz, p_memsz, p_flags, p_align = struct.unpack_from(
            "<IIIIIIII", data, offset
        )
        phdrs.append(
            {
                "index": index,
                "type": p_type,
                "offset": p_offset,
                "vaddr": p_vaddr,
                "filesz": p_filesz,
                "memsz": p_memsz,
                "flags": p_flags,
                "align": p_align,
            }
        )

    shdrs = []
    for index in range(shnum):
        offset = shoff + index * shentsize
        name, sh_type, flags, addr, sh_offset, size, link, info, align, entsize = struct.unpack_from(
            "<IIIIIIIIII", data, offset
        )
        shdrs.append(
            {
                "index": index,
                "name_offset": name,
                "type": sh_type,
                "flags": flags,
                "addr": addr,
                "offset": sh_offset,
                "size": size,
                "link": link,
                "info": info,
                "align": align,
                "entsize": entsize,
            }
        )

    if shstrndx < len(shdrs):
        names = data[shdrs[shstrndx]["offset"] : shdrs[shstrndx]["offset"] + shdrs[shstrndx]["size"]]
        for section in shdrs:
            start = section["name_offset"]
            end = names.find(b"\0", start)
            section["name"] = names[start:end].decode("latin1") if end >= 0 else ""
    else:
        for section in shdrs:
            section["name"] = ""

    return {"phdrs": phdrs, "shdrs": shdrs}


def section_by_name(elf: dict, name: str) -> dict | None:
    for section in elf["shdrs"]:
        if section["name"] == name:
            return section
    return None


def code_data_sections(elf: dict) -> list[dict]:
    return [
        section
        for section in elf["shdrs"]
        if section["type"] == SHT_PROGBITS
        and section["flags"] & SHF_ALLOC
        and section["size"] > 0
    ]


def print_elf_summary(label: str, data: bytes) -> None:
    elf = parse_elf(data)
    print(f"  {label}: phnum={len(elf['phdrs'])} shnum={len(elf['shdrs'])}")
    for phdr in elf["phdrs"]:
        print(
            "    "
            f"PH[{phdr['index']}] off=0x{phdr['offset']:x} vaddr=0x{phdr['vaddr']:x} "
            f"filesz=0x{phdr['filesz']:x} memsz=0x{phdr['memsz']:x} flags=0x{phdr['flags']:x}"
        )
    for name in ("main", "heap", ".mwcats", ".debug", ".line", ".comment", ".reginfo"):
        section = section_by_name(elf, name)
        if section is not None:
            print(
                "    "
                f"{name}: addr=0x{section['addr']:x} off=0x{section['offset']:x} "
                f"size=0x{section['size']:x}"
            )


def print_section_byte_diffs(ref: bytes, got: bytes, ref_section: dict, got_section: dict) -> int:
    ref_data = ref[ref_section["offset"] : ref_section["offset"] + ref_section["size"]]
    got_data = got[got_section["offset"] : got_section["offset"] + got_section["size"]]
    limit = min(len(ref_data), len(got_data))
    shown = 0

    for offset in range(limit):
        if ref_data[offset] != got_data[offset]:
            vaddr = ref_section["addr"] + offset
            print(
                f"    +0x{offset:06x} vaddr=0x{vaddr:08x}: "
                f"ref=0x{ref_data[offset]:02x} got=0x{got_data[offset]:02x}"
            )
            shown += 1
            if shown == DIFF_LIMIT:
                break

    if len(ref_data) != len(got_data):
        print(f"    EOF at +0x{limit:x}: reference size=0x{len(ref_data):x} candidate size=0x{len(got_data):x}")
        shown += 1

    return shown


def compare_code_data_sections(ref: bytes, got: bytes) -> int:
    ref_elf = parse_elf(ref)
    got_elf = parse_elf(got)
    ref_sections = {section["name"]: section for section in code_data_sections(ref_elf)}
    got_sections = {section["name"]: section for section in code_data_sections(got_elf)}

    def section_addr(name: str) -> int:
        section = ref_sections.get(name) or got_sections[name]
        return section["addr"]

    names = sorted(set(ref_sections) | set(got_sections), key=section_addr)
    mismatch_count = 0

    print("  code/data section byte comparison:")
    for name in names:
        ref_section = ref_sections.get(name)
        got_section = got_sections.get(name)
        if ref_section is None:
            print(f"    {name}: missing from reference")
            mismatch_count += 1
            continue
        if got_section is None:
            print(f"    {name}: missing from candidate")
            mismatch_count += 1
            continue

        ref_data = ref[ref_section["offset"] : ref_section["offset"] + ref_section["size"]]
        got_data = got[got_section["offset"] : got_section["offset"] + got_section["size"]]
        if ref_data == got_data:
            print(f"    {name}: MATCH size=0x{ref_section['size']:x}")
            continue

        print(
            f"    {name}: DIFFER "
            f"ref(addr=0x{ref_section['addr']:08x}, size=0x{ref_section['size']:x}) "
            f"got(addr=0x{got_section['addr']:08x}, size=0x{got_section['size']:x})"
        )
        mismatch_count += 1
        shown = print_section_byte_diffs(ref, got, ref_section, got_section)
        if shown == DIFF_LIMIT:
            print(f"    ... stopped after {DIFF_LIMIT} byte differences in {name}")

    if mismatch_count == 0:
        print("    all code/data sections match")

    return mismatch_count


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: compare_elf.py reference candidate", file=sys.stderr)
        return 2

    reference = Path(sys.argv[1])
    candidate = Path(sys.argv[2])
    ref = reference.read_bytes()
    got = candidate.read_bytes()

    if ref == got:
        print(f"MATCH: {candidate} is byte-identical to {reference}")
        return 0

    print(f"INFO: {candidate} is not byte-identical to {reference}")
    print(f"  reference size: {len(ref)}")
    print(f"  candidate size: {len(got)}")
    print_elf_summary("reference", ref)
    print_elf_summary("candidate", got)
    mismatches = compare_code_data_sections(ref, got)
    if mismatches == 0:
        print("MATCH: code/data sections match")
        return 0

    print("DIFFER: code/data sections differ")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
