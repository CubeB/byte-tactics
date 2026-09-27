// Decompiled by Opus. Names are provisional.
// Reads up to len bytes from the current chunk of the current slot into dst
// and advances the read position; the read counterpart of 0x4b4cf0.
#include <string.h>

struct Chunk_004b4c80 {             // 0x14 bytes
    char unknown_0[8];
    int size;                       // +0x08
    int pos;                        // +0x0c
    char* data;                     // +0x10
};

struct Slot_004b4c80 {              // 0x18 bytes
    char unknown_0[0xc];
    int current;                    // +0x0c
    char unknown_10[4];
    Chunk_004b4c80* chunks;         // +0x14
};

struct Table_004b4c80 {
    char unknown_0[4];
    Slot_004b4c80* slots;           // +0x04
    int index;                      // +0x08
};

class Class_004b4c80 {
public:
    Table_004b4c80* table;          // +0x00
    int FUN_004b4c80(void* dst, int len);
};

// FUNCTION: 0x4b4c80
int Class_004b4c80::FUN_004b4c80(void* dst, int len)
{
    Chunk_004b4c80* c = &table->slots[table->index].chunks[table->slots[table->index].current];
    int avail = c->size - c->pos;
    if (avail <= 0) {
        return 0;
    }
    if (len > avail) {
        len = avail;
    }
    memcpy(dst, c->data + c->pos, len);
    c->pos += len;
    return len;
}
