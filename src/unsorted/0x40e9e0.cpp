// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// AISearch constructor (201-byte object): seeds the per-player search cost
// table, sizes the map cell array from the game's map dimensions, and builds
// the "touched" bitmap.
//
// Two details the compiler forces and that look odd in C++:
//  - field_1c is cleared with memset, not `field_1c = 0`. With a plain
//    assignment MSVC folds the later `operator delete(field_1c)` to a push of
//    the zero register (1 byte instead of mov+push), so the original source
//    must have gone through an opaque memory clear.
//  - field_2c is filled with 0xff for n - 1 bytes only, and its last dword is
//    then forced to zero and partially re-set by the loop below.
#include <string.h>

void* operator new(unsigned int size);
void operator delete(void* p);

void* FUN_004d83b0(char* name, unsigned int size);

extern char* g_game;
extern int DAT_005119e8[10];

#pragma pack(push, 1)
class Class_0040e9e0 {
public:
    int field_00;
    int field_04;
    int field_08;
    int field_0c;
    int field_10;
    int field_14;
    int field_18;
    int* field_1c;
    int field_20;
    int field_24;
    int field_28;
    int* field_2c;
    char unknown_30[0x48 - 0x30];
    int field_48;
    int field_4c;
    int field_50;
    int field_54;
    int field_58;
    char unknown_5c[0x78 - 0x5c];
    unsigned char field_78;
    int field_79[10];
    int field_a1[10];

    Class_0040e9e0();
    void FUN_0040d900();
};
#pragma pack(pop)

// FUNCTION: 0x40e9e0
Class_0040e9e0::Class_0040e9e0()
{
    field_00 = 0;
    field_04 = 0;
    field_10 = 0;
    field_14 = 0;
    field_08 = -1;
    field_0c = 0;
    field_18 = 0;
    field_20 = 0;
    field_24 = 0;
    field_28 = 0;
    memset(&field_1c, 0, sizeof(field_1c));

    int w, h;
    h = *(int*)(g_game + 0x14237);
    w = *(int*)(g_game + 0x14233);
    field_20 = w;
    field_24 = h;
    operator delete(field_1c);
    field_28 = (h * w + 7) & ~7;
    field_1c = field_28 ? new int[field_28] : 0;

    unsigned int m = (field_28 + 0xff) >> 8;
    unsigned int n = m * 4;
    field_2c = (int*)FUN_004d83b0("AISearch touched mapentries", n);
    memset(field_2c, 0xff, n - 1);
    *(int*)((char*)field_2c + n - 4) = 0;

    for (unsigned int i = field_28 - 0x100; i < (unsigned int)field_28; i++)
        field_2c[i >> 8] |= 1 << ((i >> 3) & 0x1f);

    FUN_0040d900();

    field_58 = 0;
    field_48 = 0x535;
    field_4c = 0;
    field_78 = 0;
    field_50 = 0;
    field_54 = 0x18000;

    for (unsigned int k = 0; k < 10; k++) {
        DAT_005119e8[k] = field_54;
        field_a1[k] = 0;
        field_79[k] = *(int*)(g_game + 0x1a7f + 0x14b * (k + 1));
    }
}
