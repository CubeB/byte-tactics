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
// NOT MATCHED: 85.8%, 647 of 661 bytes. Two regions differ.
//
// (1) The failure block. In the original the block (fclose, free, xor eax,eax,
// epilogue, ret 8) sits inline between the copyright strcmp and the success
// continuation, the strcmp's `je` jumps over it, and all five header checks
// `jne` forward into it. Nothing is materialised. Here the checks are an inline
// helper returning 1, which is the only spelling found that keeps that block
// inline, but it costs a `jmp` plus a `mov eax,1` (7 bytes). What did NOT work:
// writing the cleanup out six times (one copy per check, hoping MSVC 5 would
// tail-merge them) gave 757 bytes, so it merges nothing; collapsing the five
// header checks into one `||` chain, whether with its own body (625 bytes) or
// with a `goto` to a label inside the copyright check's `if` (also 625 bytes),
// makes MSVC invert the `if (f == 0) return 0;` into `je <epilogue>`, so the
// early return merges with the tail and the whole first block diffs. Getting
// all of the original at once needs a shape that keeps the fopen failure inline
// and the cleanup shared, and I did not find it.
//
// (2) The key derivation. The original stores the key byte to [esp+0x70] (a
// reused incoming-argument slot, since `name` is dead), reloads it as a dword
// and masks with `and eax,0xff` before rotating, then reloads `h->header->key`
// after storing it and writes the result into the STACK header's key byte at
// [esp+0x24]. So the source must assign the decoded key into the stack copy of
// the 20 byte header as well, and must keep an int-width copy of the key alive.
// Adding `hdr.key = base->key;` plus `unsigned char k = base->key;` (two
// structurally different read trees, to break the store-to-load forwarding) and
// a separate `p += 0x14` gets the total SIZE right, 663 against 661 bytes, but
// it adds one live graph node and demotes both `f` and `h` one step: `f` moves
// ebx->ebp and `h` ebp->ebx, so the whole prologue and every field store diff
// (71.9%). With `hdr.key = base->key;` removed again it is back to 647 bytes at
// 85.0%. The two effects are one allocation problem, not two.
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
    return strcmp(copyright, DAT_004fdbf0);
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
        unsigned char k = h->header->key;
        unsigned char* p = (unsigned char*)h->header + 0x14;
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
