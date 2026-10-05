// Decompiled by Opus. Names are provisional.
// Claims the first free one of 8 channel slots for entry `id` of the table
// at +8 and returns its index; -1 when `id` is out of range or no slot is
// free (see 0x4b0830.cpp and 0x4b0a10.cpp).

struct Table_004b08c0 {
    char unknown_0[4];
    int count;                         // +0x4
    char unknown_8[0x18 - 0x8];
    int* entries;                      // +0x18
};

struct Channel_004b08c0 {              // 0xa4 bytes
    int used;                          // +0x0
    int entry;                         // +0x4
    int unknown_8;                     // +0x8
    char unknown_c[0x1c - 0xc];
    int unknown_1c;                    // +0x1c
    int value;                         // +0x20
    char unknown_24[0xa4 - 0x24];
};

class CobScript {
public:
    char unknown_0[8];
    Table_004b08c0* table;             // +0x8
    char unknown_c[0x1c - 0xc];
    Channel_004b08c0 channels[8];      // +0x1c
    int activeCount;                   // +0x53c

    int StartThread(int id);
};

// FUNCTION: 0x4b08c0
int CobScript::StartThread(int id)
{
    if (id < 0 || id >= table->count)
        return -1;
    for (int i = 0; i < 8; i++) {
        if (channels[i].used == 0) {
            channels[i].used = 0x1000000;
            channels[i].entry = table->entries[id];
            channels[i].unknown_8 = -1;
            channels[i].value = 0;
            channels[i].unknown_1c = 1;
            activeCount++;
            return i;
        }
    }
    return -1;
}
