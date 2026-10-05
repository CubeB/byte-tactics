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

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int ReadBox(void* buf, int size);
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

class UnitMotion {
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

    void LoadMotion(Unit* unit, HapiBank* file);
};
#pragma pack(pop)

// FUNCTION: 0x43de30
void UnitMotion::LoadMotion(Unit* unit, HapiBank* file)
{
    char name[32];
    Record_0043de30 rec;
    sprintf(name, "u%04xmob", unit->id);
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->ReadBox(&rec, 0x23);
    velocity = rec.velocity;
    v2 = rec.v2;
    field_20 = rec.d6;
    field_24 = rec.w7;
    field_26 = rec.d8;
    state = rec.state;
    flag = rec.flag;
}
