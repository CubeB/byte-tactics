// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Vec3_00498f70 {
    int x;
    int y;
    int z;
};

struct Struct_00498f70 {
    int unknown_0;
    int value;                       // +0x4
};

struct Arg_00498f70 {
    char unknown_0[8];
    unsigned int field_8;            // +0x8
};

struct Game {
    char unknown_0[0x519];
    char unknown_519[0x18];          // +0x519
    Struct_00498f70* ptr_531;        // +0x531
    char unknown_535[0x2caa - 0x535];
    Vec3_00498f70 pos;               // +0x2caa
    char unknown_2cb6[0x2cbe - 0x2cb6];
    signed char selected;            // +0x2cbe
    char unknown_2cbf[0x2cc3 - 0x2cbf];
    unsigned char orderMode;         // +0x2cc3
    char unknown_2cc4[0x2cc6 - 0x2cc4];
    unsigned char flags_2cc6;        // +0x2cc6
    char unknown_2cc7[0x37efa - 0x2cc7];
    int field_37efa;                 // +0x37efa
};
#pragma pack(pop)

class Class_00438760 {
public:
    unsigned char index;
};

extern Game* g_game;

void __stdcall IssueMobileBuildOrders(Arg_00498f70* arg);
void __stdcall PlaySoundByName(char* name, int param_2);
void FUN_0048bd00(void);
void __stdcall FUN_0048c7f0(Arg_00498f70* arg);
void __stdcall IssueOrderToSelection(void* a, unsigned char b, Class_00438760 kind,
                            int d, int e, int f);
void __stdcall FUN_00491d70(int value);
int __stdcall FindGadgetIndexBySubstring(int value, char* name);
void __stdcall FUN_004a6a40(void* obj, int index);

// FUNCTION: 0x498f70
void __stdcall FUN_00498f70(Arg_00498f70* param_1)
{
    int index;

    if (g_game->orderMode == 0xe) {
        if (g_game->flags_2cc6 & 0x40) {
            IssueMobileBuildOrders(param_1);
            PlaySoundByName("oktobuild", 0);
            if (param_1->field_8 & 4) {
                g_game->flags_2cc6 |= 0x20;
                return;
            }
            g_game->orderMode = 1;
            g_game->flags_2cc6 &= 0xdf;
            index = FindGadgetIndexBySubstring(g_game->ptr_531->value, "STOP");
            if (index != -1) {
                FUN_004a6a40(g_game->unknown_519, index);
            }
        } else {
            PlaySoundByName("notoktobuild", 0);
        }
        return;
    }
    if (g_game->selected == 0xf) {
        FUN_0048c7f0(param_1);
        return;
    }
    if (g_game->selected >= 0x11) {
        if (g_game->field_37efa == 1 && g_game->orderMode == 1) {
            FUN_0048bd00();
            FUN_00491d70(1);
        }
        return;
    }
    {
        Class_00438760 kind;
        kind.index = 0;
        IssueOrderToSelection(param_1, g_game->orderMode, kind, (int)&g_game->pos, 0, 0);
    }
    if (param_1->field_8 & 4) {
        g_game->flags_2cc6 |= 0x20;
        return;
    }
    g_game->orderMode = 1;
    g_game->flags_2cc6 &= 0xdf;
    index = FindGadgetIndexBySubstring(g_game->ptr_531->value, "STOP");
    if (index != -1) {
        FUN_004a6a40(g_game->unknown_519, index);
    }
}
