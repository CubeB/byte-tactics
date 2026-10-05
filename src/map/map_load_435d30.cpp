// Decompiled by Opus. Names are provisional.
// Makes sure the map list at +0xd24 is loaded (dropping it first when the
// multiplayer flag changes back to 0), then passes it to LoadMissionByName.

void __cdecl FUN_004d85a0(int* param_1);
int __stdcall LoadMapList(char** out, int param_2, int param_3);

class Class_00435a20 {
public:
    int LoadMissionByName(char* name);
};

class Class_00435d30 {
public:
    char unknown_0[0xd24];
    char* list;                        // +0xd24
    int count;                         // +0xd28
    int multi;                         // +0xd2c

    void FUN_00435d30(int param_1);
};

// FUNCTION: 0x435d30
void Class_00435d30::FUN_00435d30(int param_1)
{
    if (multi != 0 && param_1 == 0 && list != 0) {
        FUN_004d85a0((int*)list);
        list = 0;
    }
    if (list == 0) {
        count = LoadMapList(&list, param_1, param_1);
    }
    ((Class_00435a20*)this)->LoadMissionByName(list);
    multi = param_1;
}
