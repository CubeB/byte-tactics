// Decompiled by Opus. Names are provisional.
#include <string.h>

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

struct Source_004adf10 {
    char unknown_0[4];
    Class_004c48c0* tdf;               // +0x4
};

#pragma pack(push, 1)
struct Obj_004adf10 {
    char unknown_0[0xb6];
    char text[0x82];                   // +0xb6
    short maxchars;                    // +0x138
};
#pragma pack(pop)

extern char DAT_005119b8[];

char* __stdcall FUN_004c5740(char* text);

// FUNCTION: 0x4adf10
void __stdcall ReadTextInputFields(Obj_004adf10* obj, Source_004adf10* src)
{
    obj->maxchars = ((Class_004c46c0*)src->tdf)->FUN_004c46c0("maxchars", 0);
    if (obj->maxchars > 0x80)
        obj->maxchars = 0x80;
    src->tdf->FUN_004c48c0(obj->text, "text", 0x80, DAT_005119b8);
    strcpy(obj->text, FUN_004c5740(obj->text));
}
