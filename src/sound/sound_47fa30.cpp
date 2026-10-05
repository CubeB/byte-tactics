// Decompiled by Opus. Names are provisional.
void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Entry_0047fa30 {                // 0x11 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int* data;                         // +0xc
    char field_10;                     // +0x10
};

class Class_0047fa30 {
public:
    Entry_0047fa30 entries[9];         // +0x0
    int count;                         // +0x99
    int field_9d;                      // +0x9d

    void Remove(int i)
    {
        if (entries[i].data) {
            FUN_004d85a0(entries[i].data);
            entries[i].data = 0;
        }
        for (int j = i; j < count; j++)
            entries[j] = entries[j + 1];
        count--;
    }

    void FUN_0047fa30(void);
};
#pragma pack(pop)

// FUNCTION: 0x47fa30
void Class_0047fa30::FUN_0047fa30(void)
{
    while (count > 0)
        Remove(count - 1);
    field_9d = 0;
}
