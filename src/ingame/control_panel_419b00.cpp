// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

// The copy constructor is only declared: it is never called (the temporary is
// elided), but declaring it makes MSVC build the by-value argument in place in
// the callee's argument slot, which is what the original does.
class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760(const Class_00438760& other);
};

#pragma pack(push, 1)
struct Unit_00419b00 {
    int field_0;                       // +0
    char unknown_4[0xff - 4];
    unsigned char field_ff;            // +0xff
};

struct Game_00419b00 {
    char unknown_0[0x2a43];
    unsigned char field_2a43;          // +0x2a43
};
#pragma pack(pop)

extern Game_00419b00* g_game;

void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_0043b0b0(Class_00438760 kind, Unit_00419b00* unit, int id, int count);
short __stdcall FUN_00488b10(char* name);

// FUNCTION: 0x419b00
void __stdcall FUN_00419b00(char* name, Unit_00419b00* unit, int count)
{
    if (unit->field_ff == g_game->field_2a43) {
        if (count > 0)
            FUN_0047f1a0("addbuild", 0);
        else
            FUN_0047f1a0("subbuild", 0);
    }
    if (strstr(name, "MAKENUKE") != 0 || strstr(name, "MAKEANTI") != 0) {
        FUN_0043b0b0(Class_00438760("BUILDWEAPON"), unit, 0, count);
        return;
    }
    unsigned short id = FUN_00488b10(name);
    if (id == 0)
        return;
    int mobile = unit->field_0;
    const char* kind = mobile ? "MOBILEBUILD" : "BUILDINGBUILD";
    FUN_0043b0b0(Class_00438760(kind), unit, id, count);
}
