// Decompiled by Opus. Names are provisional.
class Class_004b07c0 {
public:
    int FUN_004b07c0(char* name);
};

class Class_004b0b00 {
public:
    int FUN_004b0b00(int index, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct Object_004385f0 {
    char unknown_0[0x9a];
    Class_004b07c0* names;             // +0x9a
};

struct Target_004385f0 {
    char unknown_0[0x42];
    unsigned int flags;                // +0x42
};
#pragma pack(pop)

int __stdcall SendScriptCallNoArgs(Object_004385f0* obj, short index);

// FUNCTION: 0x4385f0
void __stdcall FUN_004385f0(Object_004385f0* obj, Target_004385f0* target)
{
    if (target->flags & 0x400000) {
        int index = obj->names->FUN_004b07c0("StopBuilding");
        ((Class_004b0b00*)obj->names)->FUN_004b0b00(index, 0, 0, 0, 0, 0, 0, 0);
        SendScriptCallNoArgs(obj, index);
        target->flags &= ~0x400000;
    }
}
