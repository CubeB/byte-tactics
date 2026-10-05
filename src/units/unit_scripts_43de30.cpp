// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Load counterpart of 0x43dd70: reads this unit type's movement state back
// from the unit's "u%04xmob" entry.

#include <stdio.h>

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Unit {
    char unknown_0[0xa8];
    unsigned short id;               // +0xa8
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* buf, int size);
};

#pragma pack(push, 1)
struct Record_0043de30 {
    Vec3 velocity;                   // +0x00
    Vec3 v2;                         // +0x0c
    int d6;                          // +0x18
    short w7;                        // +0x1c
    int d8;                          // +0x1e
    unsigned char state : 2;         // +0x22 bits 0-1
    unsigned char flag : 1;          // +0x22 bit 2
};

class Class_0043d210 {
public:
    char unknown_0[8];
    Vec3 velocity;                   // +0x08
    Vec3 v2;                         // +0x14
    int field_20;                    // +0x20
    short field_24;                  // +0x24
    int field_26;                    // +0x26
    char unknown_2a[4];
    unsigned char state : 2;         // +0x2e bits 0-1
    unsigned char flag : 1;          // +0x2e bit 2

    void FUN_0043de30(Unit* unit, Class_004b4ba0* file);
};
#pragma pack(pop)

// FUNCTION: 0x43de30
void Class_0043d210::FUN_0043de30(Unit* unit, Class_004b4ba0* file)
{
    char name[32];
    Record_0043de30 rec;
    sprintf(name, "u%04xmob", unit->id);
    file->FUN_004b4ba0(name);
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    ((Class_004b4c80*)file)->FUN_004b4c80(&rec, 0x23);
    velocity = rec.velocity;
    v2 = rec.v2;
    field_20 = rec.d6;
    field_24 = rec.w7;
    field_26 = rec.d8;
    state = rec.state;
    flag = rec.flag;
}
