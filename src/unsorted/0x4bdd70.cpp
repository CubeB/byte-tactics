// Decompiled by Sonnet 5.5, finished by space-bunny-free, deepseek-v4.1-flash. Names are provisional.
// space-bunny-free (second pass): still 97.9%, no scratch variant beat it (5 free
// --sym scorings). Two new facts for the key block:
//   - A pure `unsigned char key` (no int copy) drops the byte's slot store AND
//     the `mov eax,[esp+0x70]; and eax,0xff` reload: MSVC keeps the byte in a
//     register only (`mov al,[ecx+0xc]; test al,al; mov dl,al; shl dl,2; shr
//     al,6; or dl,al; not dl; mov [ecx+0xc],al`, 91.7%). So the original really
//     does have a byte local and a dword local sharing slot 0x70, and the
//     `and eax,0xff` is the dword one.
//   - With the condition on the byte (`if (key)`) the `and eax,0xff` stays in
//     eax but everything else moves one register down: base=edx, key=cl, shift
//     temp=ecx (the byte condition needs cl, not dl). Splitting the rotate into
//     two temps (`unsigned int hi = w >> 6; w = w << 2; key = (unsigned
//     char)~(hi | w);`) does give the wanted `shl eax, 2` and a byte `not` (95.3%)
//     but keeps the cl/edx roles. Reversing the shift order with the byte
//     condition is 91.7% and 2 bytes long. The only form with the original's
//     roles (base=ecx, key=dl, w=eax) is the one with the condition on `w`,
//     which folds the test into the `and`. Both the wanted `test dl,dl` and the
//     wanted roles cannot be had at once with any spelling tried.
// deepseek-v4.1-flash (600s run): confirmed 97.9% and did not improve on it.
//
// deepseek-v4.1-flash (second run): the three remaining diffs are ONE allocator
// decision. Testing the int `w` (this file) keeps the original register roles
// (base=ecx, key=dl, m=eax) but folds the test into the `and eax,0xff` flags.
// Testing the byte `key` (in ANY spelling: `if (key)`, `if (key != 0)`,
// `if ((unsigned char)key)`, `if (key==0) { } else`, do/while(0), ternary,
// switch, a static inline helper that inlines, a separate copy `k2 = key`) emits
// the wanted `test al,al` but rotates every role: base=edx, key=al, m=ecx, and
// costs exactly one byte because `and ecx,0xff` is `81 E1` (6 bytes) where
// `and eax,0xff` is `25` (5 bytes). The rotation is self-reinforcing: with key
// in al, m cannot take eax, so it takes ecx, so base falls to edx. ~30 scratch
// forms tried this run, all 90.4 to 96.2%, none above 97.9%: every declaration
// order of base/key/m, `m = key` vs `m = key & 0xff` vs `(unsigned char)key`,
// separate shift temp `t`, reversed shift order `(w << 2) | (w >> 6)` (same
// 97.9 when the condition stays on w), `unsigned long m`, `int m`, `long`,
// `k = 0` else forms, ternary, and inline-helper forms. The load/store of the
// byte plus `mov eax,[esp+0x70]; and eax,0xff` (the dword reload of the byte
// home, in the dead `name` argument slot) is identical in all of them.
// deepseek-v4.1-flash (first run): confirmed 97.9% and did not improve on it.
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
