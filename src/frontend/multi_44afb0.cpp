// Decompiled by Opus. Names are provisional.

struct Obj_44afb0 {
    char unknown_0[0x60];
    int field_60;                    // +0x60
};

struct Game {
    char unknown_0[0x2bee];
    unsigned short bits0 : 4;
    unsigned short flag4 : 1;        // +0x2bee, bit 4
    unsigned short bits5 : 11;
};

extern Game* g_game;

int __stdcall FUN_0049fd60(Obj_44afb0* obj, char* str);
void __stdcall FUN_0047f1a0(char* str, int flag);
void __stdcall FUN_004ab0a0(void* param_1);
void __stdcall FUN_00425860(int a, int line, char* file);
void __stdcall FUN_00490b30(int a);
void LeaveNetGame();

// FUNCTION: 0x44afb0
void __stdcall FUN_0044afb0(Obj_44afb0* obj)
{
    if (obj->field_60 == -1) {
        if (g_game->flag4)
            LeaveNetGame();
    } else if (FUN_0049fd60(obj, "OK")) {
        FUN_0047f1a0("BigButton", 0);
        FUN_00425860(2, 0x1412, "c:\\cavedog\\wargame\\multi.cpp");
        FUN_00490b30(1);
    } else {
        FUN_004ab0a0(obj);
    }
}
