// Decompiled by Opus. Names are provisional.

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 2)
struct Object_00499c10 {
    char unknown_0[0x66];
    short heading;                     // +0x66
    char unknown_68[0x9a - 0x68];
    CobScript* anims;                  // +0x9a
};
#pragma pack(pop)

struct Source_00499c10 {
    char unknown_0[0x16];
    short heading;                     // +0x16
};

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// FUNCTION: 0x499c10
void __stdcall FUN_00499c10(Object_00499c10* obj, Source_00499c10* src)
{
    short angle = src->heading - obj->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    obj->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
}
