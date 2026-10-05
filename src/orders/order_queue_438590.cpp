// Decompiled by Opus. Names are provisional.
class CobScript {
public:
    int FindScript(char* name);
    int StartScriptWithArgsByIndex(int index, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
};

#pragma pack(push, 1)
struct Object_00438590 {
    char unknown_0[0x9a];
    CobScript* names;                  // +0x9a
};

struct Target_00438590 {
    char unknown_0[0x42];
    unsigned int flags;                // +0x42
};
#pragma pack(pop)

void __stdcall SendScriptCall(Object_00438590* obj, int index, int param_3, int param_4, int param_5, int param_6, int param_7);

// FUNCTION: 0x438590
void __stdcall StartBuildingScript(Object_00438590* obj, Target_00438590* target, unsigned short param_3)
{
    int index = obj->names->FindScript("StartBuilding");
    ((CobScript*)obj->names)->StartScriptWithArgsByIndex(index, 0, 0, 1, param_3, 0, 0, 0);
    SendScriptCall(obj, index, 1, param_3, 0, 0, 0);
    target->flags |= 0x400000;
}
