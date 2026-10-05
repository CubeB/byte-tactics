// Decompiled by Opus. Names are provisional.

struct Entry_004d0a10 {
    short a;                 // +0x0
    short b;                 // +0x2
    short c;                 // +0x4
};

struct Struct_00526ff0 {
    Entry_004d0a10 entries[0x1000];
    short field_6000;        // +0x6000
    short field_6002;        // +0x6002
    short field_6004;        // +0x6004
};

extern Struct_00526ff0* DAT_00526ff0;

// FUNCTION: 0x4d0a10
void __stdcall FUN_004d0a10(int index)
{
    DAT_00526ff0->field_6000 = 0;
    DAT_00526ff0->field_6004 = (short)index;
    DAT_00526ff0->field_6002 = 0;
    DAT_00526ff0->entries[index].a = 0x1000;
    DAT_00526ff0->entries[index].c = 0;
    DAT_00526ff0->entries[index].b = 0;
}
