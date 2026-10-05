// Decompiled by Opus. Names are provisional.
#include <string.h>

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, size_t size, char* def);
    int GetFieldInt(const char* name, int def);
};

struct Source_004adf10 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004adf10 {
    char unknown_0[0xb6];
    char text[0x82];                   // +0xb6
    short maxchars;                    // +0x138
};
#pragma pack(pop)

extern char DAT_005119b8[];

char* __stdcall Translate(char* text);

// FUNCTION: 0x4adf10
void __stdcall ReadTextInputFields(Obj_004adf10* obj, Source_004adf10* src)
{
    obj->maxchars = ((TdfRecord*)src->tdf)->GetFieldInt("maxchars", 0);
    if (obj->maxchars > 0x80)
        obj->maxchars = 0x80;
    src->tdf->GetFieldString(obj->text, "text", 0x80, DAT_005119b8);
    strcpy(obj->text, Translate(obj->text));
}
