// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Order-button handler: when the game is not in order mode, selects the STOP
// order (inlined body of 0x495860); otherwise dispatches on the order flags
// (+0x2cc6): bit 1 cancels the current order, bit 0 switches to the 0x13
// (STOP) selection, bit 2 hands a zero order kind to IssueOrderToSelection.

struct Obj_004ab400;
struct Src_004ab400;

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() : index(0) {}
};

#pragma pack(push, 1)
struct Struct_00499100 {
    int unknown_0;
    int value;                         // +0x4
};

struct Vec3_00499100 {
    int x;
    int y;
    int z;
};

struct Game {
    char unknown_0[0x519];
    char field_519[0x18];              // +0x519
    Struct_00499100* unknown_531;      // +0x531
    char unknown_535[0x2caa - 0x535];
    Vec3_00499100 pos;                 // +0x2caa
    char unknown_2cb6[0x2cbe - 0x2cb6];
    signed char selected;              // +0x2cbe
    char unknown_2cbf[0x2cc3 - 0x2cbf];
    unsigned char field_2cc3;          // +0x2cc3
    char unknown_2cc4[2];
    unsigned char field_2cc6;          // +0x2cc6
    char unknown_2cc7[0x1487f - 0x2cc7];
    Src_004ab400* table[1];            // +0x1487f
    char unknown_14883[0x37efa - 0x14883];
    int field_37efa;                   // +0x37efa
};

struct Arg_00499100 {
    char unknown_0[8];
    unsigned int field_8;              // +0x8
};
#pragma pack(pop)

extern Game* g_game;

void BeginMouseScroll();
void FUN_0048bd00(void);
int __stdcall FUN_00491d70(int force);
int __stdcall FindGadgetIndexBySubstring(int value, char* name);
void __stdcall FUN_004a6a40(void* obj, int index);
void __stdcall FUN_004ab400(Obj_004ab400* p, Src_004ab400* src);
void __stdcall IssueOrderToSelection(void* a, int b, Class_00438760 kind, Vec3_00499100* d, int e, int f);

// FUNCTION: 0x499100
void __stdcall FUN_00499100(Arg_00499100* param_1)
{
    if (g_game->field_2cc3 != 1) {
        g_game->field_2cc3 = 1;
        g_game->field_2cc6 &= 0xdf;
        int index = FindGadgetIndexBySubstring(g_game->unknown_531->value, "STOP");
        if (index != -1) {
            FUN_004a6a40(g_game->field_519, index);
        }
        return;
    }
    if (g_game->field_37efa == 0) {
        if (g_game->field_2cc6 & 2) {
            if (param_1->field_8 & 8) {
                BeginMouseScroll();
                return;
            }
            FUN_0048bd00();
            FUN_00491d70(1);
            return;
        }
        if (g_game->field_2cc6 & 1) {
            g_game->field_2cc6 = g_game->field_2cc6 | 0x10;
            if (g_game->selected != 0x13) {
                g_game->selected = 0x13;
                FUN_004ab400((Obj_004ab400*)g_game->field_519, g_game->table[0x13]);
                return;
            }
        }
    } else if (g_game->field_2cc6 & 4) {
        Class_00438760 kind;
        IssueOrderToSelection(param_1, 1, kind, &g_game->pos, 0, 0);
    }
}
