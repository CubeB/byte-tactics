// Decompiled by Sonnet 5.5, finished by space-bunny-free, deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash (600s run): confirmed 97.9% and did not improve on it.
// Re-tried and ruled out: `if (key)`/`if (key != 0)`/`(int)key` (base moves to edx,
// key to cl, 91.7), named `hi`/`lo` locals (92 to 95.3), `+`/`^` for `|`, both
// operand orders, separate output byte locals, ternaries and `key >> 6 | key << 2`
// (using the byte for the shifts). If the condition is on the byte the allocator
// puts the shift temp and key in ecx and base in edx; if it is on `w` base stays
// ecx but the test folds into the `and eax,0xff` flags.
// Opens a HAPI archive: reads the 20 byte header and checks the "HAPI" magic
// and version bytes, then checks that the file ends with the Cavedog
// copyright line (with the year patched to "0000", as the writer 0x4bd160
// does). It loads the whole header block, decrypts everything past the header
// with the key byte from the header (which is itself stored rotated and
// complemented), turns the directory's offsets into pointers and, for every
// entry flagged 1, runs FUN_004be010 on its name. With mode 0 the file is
// closed again and only the in-memory copy stays.
//
// NOT MATCHED: 97.9%, 661 of 661 bytes. Three differences remain, all in the
// key derivation (0x4bdf29 to 0x4bdf4c), each a consequence of the same thing:
//   - the original tests the byte copy (`test dl, dl`), here the test is the
//     flags of `and eax, 0xff` (the condition must be written on the int
//     copy `w` to get base in ecx and the key in dl; `if (key)` puts the base
//     in edx and the key in cl and costs 6 points);
//   - the original computes `mov edx, eax; shr edx, 6; shl eax, 2; or edx,
//     eax` (shift right first, into a copy, then shift left in place), here
//     `lea edx, [eax*4]; shr eax, 6; or edx, eax`;
//   - the next reload of h->header goes into ecx in the original, eax here.
// What worked (91.0% to 97.9%): the byte local `key` plus an int copy `w`
// (that pair produces the original's byte home in the dead `name` slot and
// the `and eax, 0xff` reload), the rotate done on `w` alone
// (`w = (w >> 6) | (w << 2); key = ~w;`, which keeps the shifts 32-bit and
// narrows only the `not`), the condition on `w`, and the decrypt loop's
// locals declared in a nested block in the order k, i, n, p (that order gives
// `[esi + eax]` instead of `[eax + esi]`). Tried without effect: helper
// functions and member functions for the rotate, `?:` forms, `+` or `^` for
// `|`, `w * 4` for `w << 2`, every declaration order of base, key and w,
// int and unsigned w, char types for the result.
// The earlier note that the original stores a stale byte is wrong: the
// original does rotate and complement the value it stores.
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
        unsigned int w = key;
        if (w) { w = (w >> 6) | (w << 2); key = ~w; }
        base->key = key;
        hdr.key = h->header->key;
        {
        unsigned char k;
        int i;
        int n;
        unsigned char* p;
        p = (unsigned char*)h->header;
        p += 0x14;
        k = hdr.key;
        n = hdr.size - 0x14;
        if (k != 0) {
            for (i = 0; i < n; i++)
                p[i] = (unsigned char)((i + 0x14) ^ k ^ ~p[i]);
        }
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
