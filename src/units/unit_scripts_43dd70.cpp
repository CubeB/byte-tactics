// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <stdio.h>

struct UnitInfo_43dd70 {
    char unknown_0[0xa8];
    unsigned short id;               // +0xa8
};

class Class_004b4ba0 {
public:
    int OpenNamedBox(char* name);
};

class Class_004b4c10 {
public:
    void SeekBox(int pos);
};

class Class_004b4cf0 {
public:
    int WriteBox(void* src, int len);
};

struct Vec3i_43dd70 {
    int x, y, z;
};

#pragma pack(push, 2)
// 0x23-byte record written under the unit's "u%04xmob" entry; the load
// counterpart is 0x43de30.
struct MobHdr_43dd70 {
    Vec3i_43dd70 a;                  // +0x00
    Vec3i_43dd70 b;                  // +0x0c
    int g;                           // +0x18
    unsigned short h;                // +0x1c
    int i;                           // +0x1e
    unsigned char f1 : 2;            // +0x22
    unsigned char f2 : 1;            // +0x22
};

class UnitMotion {
public:
    char unknown_0[8];               // +0x00
    Vec3i_43dd70 a;                  // +0x08
    Vec3i_43dd70 b;                  // +0x14
    int g;                           // +0x20
    unsigned short h;                // +0x24
    int i;                           // +0x26
    int unknown_2a;                  // +0x2a
    unsigned char f1 : 2;            // +0x2e
    unsigned char f2 : 1;            // +0x2e
    void SaveMotion(UnitInfo_43dd70* info, Class_004b4ba0* file);
};
#pragma pack(pop)

// FUNCTION: 0x43dd70
void UnitMotion::SaveMotion(UnitInfo_43dd70* info, Class_004b4ba0* file)
{
    char name[32];
    MobHdr_43dd70 hdr;
    hdr.a = a;
    hdr.b = b;
    hdr.g = g;
    hdr.h = h;
    hdr.i = i;
    hdr.f1 = f1;
    hdr.f2 = f2;
    sprintf(name, "u%04xmob", info->id);
    file->OpenNamedBox(name);
    ((Class_004b4c10*)file)->SeekBox(0);
    ((Class_004b4cf0*)file)->WriteBox(&hdr, 0x23);
}
