// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_004a1a50 {
    unsigned char type;                // +0x0
    unsigned char id;                  // +0x1
    char unknown_2[0xb6 - 0x2];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x4a1a50
int __stdcall FUN_004a1a50(Entry_004a1a50* entries, int index)
{
    unsigned char id = entries[index].id;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 3 && entries[i].id == id) {
            return i;
        }
    }
    return 0;
}
