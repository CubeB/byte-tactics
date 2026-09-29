// Decompiled by Sonnet 5.5, finished by space-bunny-free. Names are provisional.
// Opens a HAPI archive: reads the 20 byte header and checks the "HAPI" magic
// and version bytes, then checks that the file ends with the Cavedog
// copyright line (with the year patched to "0000", as the writer 0x4bd160
// does). It loads the whole header block, decrypts everything past the header
// with the key byte from the header (which is itself stored rotated and
// complemented), turns the directory's offsets into pointers and, for every
// entry flagged 1, runs FUN_004be010 on its name. With mode 0 the file is
// closed again and only the in-memory copy stays.
//
// NOT MATCHED: 91.0%, 650 of 661 bytes (was 85.8% / 647 when this pass
// started). Two of the three diff regions are now exact; only the key
// derivation is left.
//
// FIXED, region (1), the failure block, now byte exact. The trick is the
// last two statements of Bad_004bdd70: writing the strcmp test positively
// (`if (strcmp(...) == 0) return 0; return 1;` instead of `return
// strcmp(...)`) is what keeps MSVC 5 from materialising `mov eax, 1`. With
// the plain `return strcmp(copyright, DAT_004fdbf0)` the helper's other exit
// has to become the constant 1, and the caller then needs a `jmp` plus that
// `mov eax, 1` (7 bytes) between the strcmp's `sbb eax, -1` and the shared
// `test eax, eax`. Spelling the same test as a positive compare lets both
// failure edges reach the cleanup with the value already in eax, and the five
// header `jne`s then land on the same inline block the strcmp falls into, at
// the original's 0x4bded6. Worth remembering: for a helper that returns 0/1,
// writing `if (cond) return 0; return 1;` beats `return cond` whenever cond's
// failure path must share a block with the caller's.
//
// ALSO FIXED, incidentally, `unsigned char* p = (unsigned char*)h->header;
// p += 0x14;` as two statements rather than one expression. That is what
// produces the original's separate `mov esi, [ebp+8]` / `add esi, 0x14`
// pair, so the SIB base in the decrypt loop stays `esi` rather than folding
// the displacement into it.
//
// STILL DIFFERS, region (2), the key derivation, 11 bytes. The original is
//     mov dl, byte ptr [ecx + 0xc]     ; base->key into a BYTE register
//     mov byte ptr [esp + 0x70], dl    ; stored to the dead `name` arg slot
//     mov eax, dword ptr [esp + 0x70]  ; re-read as a DWORD
//     and eax, 0xff
//     test dl, dl                     ; the test uses the byte copy
//     mov edx, eax / shr edx, 6 / shl eax, 2 / or edx, eax   ; rotate the INT copy
//     not dl                          ; ... then complement the STALE byte
//     mov byte ptr [ecx + 0xc], dl
// so the rotate is dead code and what is stored is `~key`, not
// `~((key >> 6) | (key << 2))`. Getting this needs two variables with two
// different homes at once: a byte local that MSVC 5 puts in the reused
// incoming-argument slot, and a separate int-width copy of the same byte that
// is masked with `and eax, 0xff` and rotated in eax/edx while the byte copy
// in dl carries the `test` and the `not`. Neither variable alone produces
// both halves.
//
// Measured, with the score and the emitted size (original 661):
//   `unsigned char key` with `(unsigned char)~((key >> 6) | (key << 2))`
//     91.0% / 650. Everything stays in al/dl, no stack home. This is what is
//     in the file.
//   the same with `~(unsigned char)(...)`, `(unsigned char)~(int)` on the
//     shifts, `unsigned int` shifts, `key & 0xff` inside, or a `static inline`
//     rotate helper taking an int: all 91.0% / 650, byte-identical output. The
//     cast position does not move it, so the `not dl` on the stale byte is not
//     a narrowing-cast artefact of that kind.
//   function-scope `unsigned char bkey` (which DOES get the `[esp+0x70]`
//     home, as j2 shows) plus `int key = bkey`: 90.0% / 660, and j2's own diff
//     is one register class out: `mov al,[edx+0xc]` and the rotate in
//     al/ecx, where the original has `mov dl,[ecx+0xc]` and eax/edx, and the
//     original reloads `h->header` into ecx where j2 reloads it into eax.
//   homing the byte in a struct field (`hdr.unknown_d[0]`): 90.9% / 660, same
//     shape, wrong slot and an extra `jmp`.
//   `int key = base->key` with the rotate then a separate
//     `unsigned char bkey`: 91.1% / 656, closest on size, but it adds a `jmp`
//     and homes `bkey` at `[esp+0x74]`.
//   letting the rotate die (`int wide = key; if (wide) wide = ...; key =
//     (unsigned char)~key;`): 92.0% / 634, the highest score reached here but
//     27 bytes short, so it is a different function, not a better match.
//   an array element (`unsigned char keys[1]`) to force a home: 87.7%.
//   `unsigned char` vs `int` vs `unsigned int` for the key local, key declared
//     with a separate initialising statement, `key = key ? ... : key`, reading
//     `base->key` a second time instead of the local: all 91.0% / 650.
//
// So the two halves are individually reachable and never together: every
// spelling that produces the `[esp+0x70]` byte home with the
// `and eax, 0xff` reload also picks ecx for the base and al/ecx for the
// rotate, and every spelling that keeps dl for the byte drops the home
// entirely. That looks like one live-range decision, and I did not find the
// declaration order that splits it.
#include <windows.h>
#include <stdio.h>
#include <string.h>


#pragma pack(push, 1)
struct Entry_004bdd70 {                // 9 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    unsigned char flags;               // +0x8
};

struct Table_004bdd70 {
    int count;                         // +0x0
    Entry_004bdd70* entries;           // +0x4
};

struct Header_004bdd70 {
    char magic[4];                     // +0x0
    unsigned char version[4];          // +0x4
    unsigned int size;                 // +0x8
    unsigned char key;                 // +0xc
    char unknown_d[3];
    Table_004bdd70* table;             // +0x10
};

struct File_004bdd70 {                 // 0x118 bytes
    FILE* fp;                          // +0x0
    int field_4;                       // +0x4
    Header_004bdd70* header;           // +0x8
    int field_c;                       // +0xc
    int mode;                          // +0x10
    char path[0x104];                  // +0x14
};
#pragma pack(pop)

extern char DAT_004fdbf0[];            // "Copyright 0000 Cavedog Entertainment"

void* FUN_004d83b0(const char* name, unsigned int size);
void FUN_004d85a0(void* p);
void __stdcall FUN_004be010(int name, int base);

static inline int Bad_004bdd70(FILE* f, Header_004bdd70* hdr, char* copyright)
{
    if (strncmp(hdr->magic, "HAPI", 4) != 0 || hdr->version[0] != 0 || hdr->version[1] != 0 ||
        hdr->version[2] != 1 || hdr->version[3] != 0)
        return 1;
    int len = strlen(DAT_004fdbf0);
    fseek(f, -len, 2);
    fread(copyright, 1, len, f);
    copyright[len] = 0;
    strncpy(copyright + (strstr(DAT_004fdbf0, "0000") - DAT_004fdbf0), "0000", 4);
    if (strcmp(copyright, DAT_004fdbf0) == 0)
        return 0;
    return 1;
}

// FUNCTION: 0x4bdd70
File_004bdd70* __stdcall FUN_004bdd70(const char* name, int mode)
{
    FILE* f = fopen(name, "rb");
    if (f == 0)
        return 0;
    File_004bdd70* h = (File_004bdd70*)FUN_004d83b0("OPENHAPIFILE structure", 0x118);
    char* filePart;
    h->fp = f;
    h->field_4 = -1;
    h->field_c = 0;
    h->mode = mode;
    GetFullPathNameA(name, 0x104, h->path, &filePart);
    Header_004bdd70 hdr;
    fread(&hdr, 0x14, 1, f);
    char copyright[0x40];
    if (Bad_004bdd70(f, &hdr, copyright)) {
        fclose(f);
        FUN_004d85a0(h);
        return 0;
    }
    h->header = (Header_004bdd70*)FUN_004d83b0("HAPIFILE header", hdr.size);
    rewind(f);
    fread(h->header, hdr.size, 1, f);
    {
        Header_004bdd70* base = h->header;
        unsigned char key = base->key;
        if (key != 0)
            key = ~(unsigned char)((key >> 6) | (key << 2));
        base->key = key;
        hdr.key = h->header->key;
        unsigned char* p = (unsigned char*)h->header;
        p += 0x14;
        unsigned char k = hdr.key;
        int n = hdr.size - 0x14;
        if (k != 0) {
            for (int i = 0; i < n; i++)
                p[i] = (unsigned char)((i + 0x14) ^ k ^ ~p[i]);
        }
        base = h->header;
        base->table = (Table_004bdd70*)((char*)base->table + (int)base);
        Header_004bdd70* b = h->header;
        Table_004bdd70* t = b->table;
        t->entries = (Entry_004bdd70*)((char*)t->entries + (int)b);
        for (int i = t->count - 1; i >= 0; i--) {
            Entry_004bdd70* e = &t->entries[i];
            e->field_0 += (int)b;
            e->field_4 += (int)b;
            if (e->flags & 1)
                FUN_004be010(e->field_4, (int)b);
        }
    }
    if (mode == 0) {
        fclose(f);
        h->fp = 0;
    }
    return h;
}
