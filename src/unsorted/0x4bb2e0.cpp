// Decompiled by space-bunny-free. Names are provisional.
// Opens a file for reading (the mode comes from the caller: "rb" from 0x4bb5b0,
// "a+b" from 0x4bb2c0). If the file cannot be opened it falls back to the first
// already loaded item whose name tree has an entry without bit 0 of its flags
// set, reads that entry's texture block into a fresh "Block Sizes" buffer and
// un-obfuscates it in place.
// The block size is the 16.16 field rounded up to whole pixels, times 4. Written
// as w / 65536 + (w % 65536 != 0) (through the inlined helper) so that MSVC
// expands it to exactly the original's cdq / and edx,0xffff / add / sar plus the
// neg / sbb / neg remainder test.
// The four locals of the loading block are declared at the top of the function,
// in the order size, off, k, p. That order is not cosmetic: MSVC 5 puts the
// later-declared of two operands of a commutative add first, and the first
// operand becomes the base register of a two-register address, so the order
// fixes all three of the original's operand choices (off + size, k + off and
// p[k]) at once. Declaring them in the blocks where they are used, or in any
// other order, puts one of the three the wrong way round.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004bb2e0 {
    char* name;                          // +0x0
    void* child;                         // +0x4
    unsigned char flags;                 // +0x8
};
#pragma pack(pop)

struct List_004bb2e0 {
    int count;                           // +0x0
    Entry_004bb2e0* entries;             // +0x4
};

struct Node_004bb2e0 {
    char unknown_0[0xc];
    unsigned char obfuscate;             // +0xc
    List_004bb2e0* list;                 // +0x10
};

struct Tex_004bb2e0 {
    int offset;                          // +0x0, offset of the block in the item
    int width;                           // +0x4, 16.16 pixels
    unsigned char is_texture;            // +0x8
};

struct Item_004bb2e0 {
    FILE* fp;                            // +0x0
    int pos;                             // +0x4, read position
    Node_004bb2e0* node;                 // +0x8
    int count;                           // +0xc
    char unknown_10[4];
    char name[0x100];                    // +0x14
};

struct State_004bb2e0 {
    char unknown_0[0x618];
    Item_004bb2e0** items;               // +0x618
    int itemCount;                       // +0x61c
};

// The same class as 0x4bb6a0, whose file has these field names.
struct File_004bb2e0 {
    FILE* fp;                            // +0x0
    Item_004bb2e0* shared;               // +0x4
    Tex_004bb2e0* info;                  // +0x8
    int pos;                             // +0xc
    void* buffer;                        // +0x10, the block just read
    int* buffer2;                        // +0x14
    char name[0x100];                    // +0x18
};

State_004bb2e0* FUN_004b6220(void);
void* FUN_004d83b0(char* name, unsigned int size);
void FUN_004d85a0(void* p);
Entry_004bb2e0* __stdcall FUN_004bb4e0(List_004bb2e0* list, char* name);

// Whole pixels of a 16.16 size, rounded up.
static inline int nblocks(int w)
{
    return w / 65536 + (w % 65536 != 0);
}

// FUNCTION: 0x4bb2e0
File_004bb2e0* __stdcall FUN_004bb2e0(char* filename, const char* mode)
{
    int size;
    int off;
    int k;
    unsigned char* p;
    State_004bb2e0* state = FUN_004b6220();
    File_004bb2e0* h = (File_004bb2e0*)FUN_004d83b0("File Handle", 0x118);
    memset(h, 0, 0x118);
    strncpy(h->name, filename, 0x100);
    h->name[0xff] = 0;
    h->fp = fopen(filename, mode);
    if (h->fp) {
        h->shared = 0;
        return h;
    }
    for (int i = 0; i < state->itemCount; i++) {
        Entry_004bb2e0* e = FUN_004bb4e0(state->items[i]->node->list, filename);
        if (e == 0 || (e->flags & 1))
            continue;
        if (state->items[i]->fp == 0) {
            state->items[i]->fp = fopen(state->items[i]->name, "rb");
            if (state->items[i]->fp == 0) {
                FUN_004d85a0(h);
                return 0;
            }
            state->items[i]->pos = 0;
        }
        state->items[i]->count++;
        h->info = (Tex_004bb2e0*)e->child;
        h->shared = state->items[i];
        h->pos = 0;
        if (h->info->is_texture) {
            size = nblocks(h->info->width) * 4;
            h->buffer = FUN_004d83b0("Block Sizes", size);
            off = h->info->offset;
            fseek(h->shared->fp, off, 0);
            fread(h->buffer, 1, size, h->shared->fp);
            h->shared->pos = off + size;
            p = (unsigned char*)h->buffer;
            unsigned char key = h->shared->node->obfuscate;
            if (key) {
                for (k = 0; k < size; k++)
                    p[k] = (unsigned char)(p[k] ^ 0xff ^ (unsigned char)(k + off) ^ key);
            }
        }
        return h;
    }
    FUN_004d85a0(h);
    return 0;
}
