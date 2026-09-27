// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

int __stdcall FUN_004b6c30(int range);
void __stdcall FUN_00421620(void* spawn);

struct Spawn_00420e50 {
    void* unit;            // +0x00
    int index;             // +0x04
    int x08;               // +0x08
    int x0c;               // +0x0c
    int x10;               // +0x10
    int x14;               // +0x14
    int x18;               // +0x18
    int x1c;               // +0x1c
    int x20;               // +0x20
    int x24;               // +0x24
    unsigned int b0 : 1;   // +0x28
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 1;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 26;
    int x2c;               // +0x2c
};

#pragma pack(push, 1)
struct Unit_00420e50 {
    char unknown_0[0x9e];
    int* records;          // +0x9e, points at { int count; ... }
};
#pragma pack(pop)

// FUNCTION: 0x420e50
void __stdcall FUN_00420e50(Unit_00420e50* unit)
{
    Spawn_00420e50 s;
    s.b4 = 0;
    s.b5 = 0;
    s.unit = unit;
    s.x20 = 900;
    s.x24 = 1;
    for (int i = 0; i < *unit->records; i++) {
        s.b1 = FUN_004b6c30(100) & 1;
        s.b2 = 1;
        s.b3 = 1;
        s.x08 = FUN_004b6c30(3000);
        s.x0c = FUN_004b6c30(3000);
        s.x10 = FUN_004b6c30(3000);
        s.x14 = (20 - FUN_004b6c30(40)) << 14;
        s.x18 = FUN_004b6c30(10) << 16;
        s.x1c = (20 - FUN_004b6c30(40)) << 14;
        s.index = i;
        FUN_00421620(&s);
    }
}
