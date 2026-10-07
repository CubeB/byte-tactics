// Decompiled by Sonnet 5.5, finished by space-bunny-free, deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <stdio.h>
#include <string.h>


#pragma pack(push, 1)
struct ArchiveEntry {                  // 9 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    unsigned char flags;               // +0x8
};

struct ArchiveDirectory {
    int count;                         // +0x0
    ArchiveEntry* entries;             // +0x4
};

struct Header_004bdd70 {
    char magic[4];                     // +0x0
    unsigned char version[4];          // +0x4
    unsigned int size;                 // +0x8
    unsigned char key;                 // +0xc
    char unknown_d[3];
    ArchiveDirectory* table;           // +0x10
};

struct OPENHAPIFILE {                  // 0x118 bytes
    FILE* fp;                          // +0x0
    int field_4;                       // +0x4
    Header_004bdd70* header;           // +0x8
    int field_c;                       // +0xc
    int mode;                          // +0x10
    char path[0x104];                  // +0x14
};
#pragma pack(pop)

extern char g_hapiCopyright[];         // "Copyright 0000 Cavedog Entertainment"

void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void __stdcall HAPI_RelocateDirectory(int name, int base);

static inline int Bad_004bdd70(FILE* f, Header_004bdd70* hdr, char* copyright)
{
    if (strncmp(hdr->magic, "HAPI", 4) != 0 || hdr->version[0] != 0 || hdr->version[1] != 0 ||
        hdr->version[2] != 1 || hdr->version[3] != 0)
        return 1;
    int len = strlen(g_hapiCopyright);
    fseek(f, -len, 2);
    fread(copyright, 1, len, f);
    copyright[len] = 0;
    strncpy(copyright + (strstr(g_hapiCopyright, "0000") - g_hapiCopyright), "0000", 4);
    if (strcmp(copyright, g_hapiCopyright) == 0)
        return 0;
    return 1;
}

// FUNCTION: 0x4bdd70
OPENHAPIFILE* __stdcall HAPI_OpenArchive(const char* name, int mode)
{
    FILE* f = fopen(name, "rb");
    if (f == 0)
        return 0;
    OPENHAPIFILE* h = (OPENHAPIFILE*)FUN_004d83b0("OPENHAPIFILE structure", 0x118);
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
        // Key derivation: a byte key for the condition, an unsigned w for the split rotate,
        // and a second byte local r receiving the result.
        unsigned char key = base->key;
        unsigned int w = key;
        unsigned char r = key;
        if (key) { unsigned int hi = w >> 6; w = w << 2; hi = hi | w; r = (unsigned char)~hi; }
        base->key = r;
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
        base->table = (ArchiveDirectory*)((char*)base->table + (int)base);
        Header_004bdd70* b = h->header;
        ArchiveDirectory* t = b->table;
        t->entries = (ArchiveEntry*)((char*)t->entries + (int)b);
        for (int i = t->count - 1; i >= 0; i--) {
            ArchiveEntry* e = &t->entries[i];
            e->field_0 += (int)b;
            e->field_4 += (int)b;
            if (e->flags & 1)
                HAPI_RelocateDirectory(e->field_4, (int)b);
        }
    }
    if (mode == 0) {
        fclose(f);
        h->fp = 0;
    }
    return h;
}
