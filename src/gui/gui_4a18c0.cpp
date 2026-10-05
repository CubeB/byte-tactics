// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_004a18c0 {
    unsigned char type;                // +0x0
    char unknown_1[0x27 - 0x1];
    char field_27;                     // +0x27
    char unknown_28[0xb6 - 0x28];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// Counts the type-8 entries up to the one numbered by entries[index].field_27.
// Both paths return 0 in the original, although the caller tests the result.
// FUNCTION: 0x4a18c0
int __stdcall FUN_004a18c0(Entry_004a18c0* entries, int index)
{
    int n = 0;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 8) {
            if (n == entries[index].field_27) {
                return 0;
            }
            n++;
        }
    }
    return 0;
}
