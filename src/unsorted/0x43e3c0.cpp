// Decompiled by Opus. Names are provisional.
// Looks up the "SweetSpot" entry in the object's name table at +0x9a and
// passes the value found on to FUN_0043e0b0.

class Class_004b0bc0 {
public:
    int FUN_004b0bc0(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
};

#pragma pack(push, 2)
struct Obj_0043e3c0 {
    char unknown_0[0x9a];
    Class_004b0bc0* table;             // +0x9a
};
#pragma pack(pop)

void __stdcall FUN_0043e0b0(Obj_0043e3c0* obj, int param_2, int value);

// FUNCTION: 0x43e3c0
void __stdcall FUN_0043e3c0(Obj_0043e3c0* obj, int param_2)
{
    int value = 0;
    obj->table->FUN_004b0bc0("SweetSpot", &value, 0, 0, 0);
    FUN_0043e0b0(obj, param_2, value);
}
