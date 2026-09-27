// Decompiled by Opus. Names are provisional.

void FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Entry_0047ffa0 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int* data;                         // +0x0c
    char field_10;                     // +0x10
};

class Class_0047ffa0 {
public:
    Entry_0047ffa0 entries[9];         // +0x00
    int count;                         // +0x99
    void FUN_0047ffa0(int index);
};
#pragma pack(pop)

// FUNCTION: 0x47ffa0
void Class_0047ffa0::FUN_0047ffa0(int index)
{
    Entry_0047ffa0* e = &entries[index];
    if (e->data != 0) {
        FUN_004d85a0(e->data);
        e->data = 0;
    }
    for (int i = index; i < count; i++) {
        entries[i] = entries[i + 1];
    }
    count--;
}
