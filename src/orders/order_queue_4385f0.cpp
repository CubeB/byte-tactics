// Decompiled by Opus. Names are provisional.
class CobScript {
public:
    int FindScript(char* name);
    int StartScriptWithArgsByIndex(int index, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct Object_004385f0 {
    char unknown_0[0x9a];
    CobScript* names;                  // +0x9a
};

struct Target_004385f0 {
    char unknown_0[0x42];
    unsigned int flags;                // +0x42
};
#pragma pack(pop)

int __stdcall SendScriptCallNoArgs(Object_004385f0* obj, short index);

// FUNCTION: 0x4385f0
void __stdcall StopBuildingScript(Object_004385f0* obj, Target_004385f0* target)
{
    if (target->flags & 0x400000) {
        int index = obj->names->FindScript("StopBuilding");
        ((CobScript*)obj->names)->StartScriptWithArgsByIndex(index, 0, 0, 0, 0, 0, 0, 0);
        SendScriptCallNoArgs(obj, index);
        target->flags &= ~0x400000;
    }
}
