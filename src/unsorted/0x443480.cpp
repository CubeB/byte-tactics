// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <math.h>

struct Gadget_00443480 {
    char unknown_0[0x18];
    int* field_18;                     // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

#pragma pack(push, 1)
struct Game_00443480 {
    char unknown_0[0x2bbf];
    unsigned char field_2bbf;          // +0x2bbf
    unsigned char field_2bc0;          // +0x2bc0
    char unknown_2bc1[0x2bee - 0x2bc1];
    unsigned char field_2bee;          // +0x2bee
};
#pragma pack(pop)

extern Game_00443480* g_game;

int __stdcall FUN_0049fd60(Gadget_00443480* obj, char* str);
int __stdcall FUN_0049fdf0(int a, char* name, int flag);
char __stdcall FUN_004a04f0(Gadget_00443480* obj, char* name);
int __stdcall FUN_004a0f30(Gadget_00443480* obj, int handle);
void __stdcall FUN_00491c80(int value);
void __stdcall FUN_0046bf00(int value);
void __stdcall FUN_004ab0a0(Gadget_00443480* obj);

// FUNCTION: 0x443480
void __stdcall FUN_00443480(Gadget_00443480* obj)
{
    int i;
    int iVar1;
    int value;
    char buf[16];
    int handle;
    int acc;

    iVar1 = obj->field_18[1];
    if (obj->field_60 == -1)
        return;
    if (FUN_0049fd60(obj, "OK")) {
        acc = 0;
        for (i = 0; i < 16u; i++) {
            wsprintfA(buf, "CHK%d", i);
            handle = FUN_0049fdf0(iVar1, buf, 1);
            if (!FUN_004a04f0(obj, buf))
                break;
            value = FUN_004a0f30(obj, handle);
            acc = (int)(pow(2.0, i) * value + acc);
        }

        FUN_00491c80(0x14);
        FUN_0046bf00(acc);
        if ((g_game->field_2bee & 0x10) || g_game->field_2bbf == 0x14) {
            g_game->field_2bc0 = 0x15;
        } else {
            g_game->field_2bc0 = 0x11;
        }
    } else {
        FUN_004ab0a0(obj);
    }
}
