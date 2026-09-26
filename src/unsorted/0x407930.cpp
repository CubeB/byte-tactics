// Decompiled by Sonnet. Names are provisional.

extern void* DAT_004fc988;

class Class_00407930
{
public:
    void* vtable;      // +0x0
    int field_4;       // +0x4
    int field_8;       // +0x8
    int field_c;       // +0xc
    int field_10;      // +0x10
    int field_14;      // +0x14
    int field_18;      // +0x18
    int field_1c;      // +0x1c
    int field_20;      // +0x20
    int field_24;      // +0x24

    Class_00407930(int param_1, int param_2, int param_3, int param_4);
};

// FUNCTION: 0x407930
Class_00407930::Class_00407930(int param_1, int param_2, int param_3, int param_4)
{
    field_4 = param_1;
    field_8 = param_2;
    field_c = 0;
    field_10 = *(unsigned char*)(param_1 + 4);
    field_1c = param_4;
    field_20 = param_3;
    vtable = &DAT_004fc988;
    field_24 = 0;
    field_18 = 6;
    field_14 = 3;
}
