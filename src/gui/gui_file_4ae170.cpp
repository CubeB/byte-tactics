// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

class Class_004c46c0 {
public:
    int GetFieldInt(const char* name, int def);
};

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, size_t size, char* def);
};

struct Source_004ae170 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae170 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    short range;                       // +0x136
    char unknown_138[4];               // +0x138
    int thick;                         // +0x13c
    short knobpos;                     // +0x140
    short knobsize;                    // +0x142
    int field_144;                     // +0x144
};
#pragma pack(pop)

extern char DAT_005119b8[];

char* __stdcall Translate(char* text);

// FUNCTION: 0x4ae170
void __stdcall ReadSliderFields(Obj_004ae170* obj, Source_004ae170* src)
{
    obj->range = ((Class_004c46c0*)src->tdf)->GetFieldInt("range", 0);
    obj->thick = (short)((Class_004c46c0*)src->tdf)->GetFieldInt("thick", 0);
    obj->knobpos = ((Class_004c46c0*)src->tdf)->GetFieldInt("knobpos", 0);
    obj->knobsize = ((Class_004c46c0*)src->tdf)->GetFieldInt("knobsize", 0);
    obj->field_144 = 0;
    src->tdf->GetFieldString(obj->text, "text", 0x80, DAT_005119b8);
    strcpy(obj->text, Translate(obj->text));
}
