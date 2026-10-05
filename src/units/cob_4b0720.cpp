// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

// CobScript (see src/units/cob_4b0610.cpp) and its derived class
// UnitScript (src/units/units_485e30.cpp). SetCob attaches the
// object's state data: it stores the data block, looks up its runtime state,
// reallocates the "Object States" and "Static Varibles" tables and clears the
// state table. Called from 0x485d40 with unit->type->field_18e.

struct Data_004b0720 {
    char unknown_0[0x8];
    int countStates;      // +0x8, entries of 0x4c bytes
    char unknown_c[0x4];
    int countVars;        // +0x10
};

class CobScript {
public:
    char unknown_0[0x8];
    int field_8;              // +0x8
    void* field_c;            // +0xc
    void* ptr10;              // +0x10
    void* ptr14;              // +0x14

    void SetCob(Data_004b0720* data);
};

int __stdcall GetCobChecksum(void* param);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);

// FUNCTION: 0x4b0720
void CobScript::SetCob(Data_004b0720* data)
{
    field_8 = (int)data;
    if (data != 0) {
        field_c = (void*)GetCobChecksum(data);
        ptr14 = FUN_004d84a0(ptr14, "Object States", data->countStates * 0x4c);
        ptr10 = FUN_004d84a0(ptr10, "Static Varibles", data->countVars * 4);
        memset(ptr14, 0, data->countStates * 0x4c);
    }
}
