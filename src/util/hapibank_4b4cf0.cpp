// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Chunk_004b4cf0 {             // 0x14 bytes
    char unknown_0[8];
    int capacity;                   // +0x08
    int size;                       // +0x0c
    char* data;                     // +0x10
};

struct Slot_004b4cf0 {              // 0x18 bytes
    char unknown_0[0xc];
    int current;                    // +0x0c
    char unknown_10[4];
    Chunk_004b4cf0* chunks;         // +0x14
};

struct Table_004b4cf0 {
    char unknown_0[4];
    Slot_004b4cf0* slots;           // +0x04
    int index;                      // +0x08
};

int* __cdecl FUN_004d8580(int* param_1, int param_2);

class Class_004b4cf0 {
public:
    Table_004b4cf0* table;          // +0x00
    int FUN_004b4cf0(void* src, int len);
};

// FUNCTION: 0x4b4cf0
int Class_004b4cf0::FUN_004b4cf0(void* src, int len)
{
    Chunk_004b4cf0* c = &table->slots[table->index].chunks[table->slots[table->index].current];
    int need = len + c->size;
    if (need > c->capacity) {
        c->data = (char*)FUN_004d8580((int*)c->data, need);
        c->capacity = need;
    }
    memcpy(c->data + c->size, src, len);
    c->size += len;
    return len;
}
