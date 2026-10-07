// Decompiled by deepseek-v4.1-flash, space-bunny-free, GPT-6.1-sol and Haiku. Names are provisional.

#include <stdlib.h>

#pragma pack(push, 1)
struct View {
    int x;                             // +0x0
    int y;                             // +0x4
    unsigned int field_8;              // +0x8
    int field_c;                       // +0xc
    int msg;                           // +0x10
    int field_14;                      // +0x14
};

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Struct_00499200_531 {
    int unknown_0;
    int value;                         // +0x4
};

struct BitFlags16_00499200 {
    unsigned short b0:1;
    unsigned short b1:1;
    unsigned short b2:1;
    unsigned short b3:1;
    unsigned short b4:1;
    unsigned short b5:1;
    unsigned short b6:1;
    unsigned short b7:1;
    unsigned short b8:1;
    unsigned short b9:1;
    unsigned short b10:1;
    unsigned short b11:1;
    unsigned short b12:1;
    unsigned short b13:1;
    unsigned short b14:1;
    unsigned short b15:1;
};

union Flags16_00499200 {
    unsigned short value;
    BitFlags16_00499200 bits;
};

class Mission {
public:
    int FUN_00435100();
    int FUN_00435c50();
    char* FUN_004352b0();
    void LoadCampaign(void* p);
    int FUN_00435c00(int a);
    void LoadMissionByName(void* p);
};

class Sound {
public:
    void SetTrackCategory(int a);
};

struct Game {
    char unknown_0[0x10];
    Sound* field_10;                   // +0x10
    char unknown_14[0x519 - 0x14];
    char field_519[0x18];              // +0x519
    Struct_00499200_531* field_531;    // +0x531
    char unknown_535[0x29a0 - 0x535];
    char* field_29a0;                  // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    unsigned short field_2a3c;         // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    Flags16_00499200 field_2a44;       // +0x2a44
    char unknown_2a46[0x2c76 - 0x2a46];
    View view;                         // +0x2c76
    char unknown_2c8e[0x2c92 - 0x2c8e];
    int field_2c92;                    // +0x2c92
    int field_2c96;                    // +0x2c96
    int field_2c9a;                    // +0x2c9a
    int field_2c9e;                    // +0x2c9e
    int field_2ca2;                    // +0x2ca2
    int field_2ca6;                    // +0x2ca6
    union {
        Vec3 pos;                      // +0x2caa
        struct {
            char unknown_2caa[2];
            short field_2cac;          // +0x2cac
            char unknown_2cae[2];
            short field_2cb0;          // +0x2cb0
            char unknown_2cb2[2];
            short field_2cb4;          // +0x2cb4
        };
    };
    int field_2cb6;                    // +0x2cb6
    unsigned short field_2cba;         // +0x2cba
    char unknown_2cbc[0x2cbe - 0x2cbc];
    signed char selected;              // +0x2cbe
    char unknown_2cbf[0x2cc3 - 0x2cbf];
    unsigned char orderMode;           // +0x2cc3
    char unknown_2cc4[0x2cc6 - 0x2cc4];
    unsigned char flags_2cc6;          // +0x2cc6
    char unknown_2cc7[0x2cdf - 0x2cc7];
    int field_2cdf;                    // +0x2cdf
    char unknown_2ce3[0x14357 - 0x2ce3];
    char* units;                       // +0x14357
    char unknown_1435b[0x1487f - 0x1435b];
    void* table[21];                   // +0x1487f
    char unknown_148d3[0x37e9c - 0x148d3];
    unsigned short field_37e9c;        // +0x37e9c
    char unknown_37e9e[0x37efa - 0x37e9e];
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x391e9 - 0x37efe];
    Mission* net;                      // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                   // +0x391f1
    void (*field_391f5)(void);         // +0x391f5
    char unknown_391f9[0x3923b - 0x391f9];
    Flags16_00499200 field_3923b;      // +0x3923b
    char unknown_3923d[0x39249 - 0x3923d];
    int field_39249;                   // +0x39249
};
#pragma pack(pop)

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() : index(0) {}
};

extern Game* g_game;

void BeginMouseScroll();
void FUN_004197d0();
void FUN_0041c180();
void UpdateMouseScroll();
void FUN_0041d0f0();
void __stdcall FUN_0041d9f0(int a);
void BlankScreen();
int FUN_004572a0();
void FUN_00463c80();
void FUN_0047a760();
void FUN_0048bd00();
int __stdcall SelectUnitsInBox(void* p);
unsigned short __stdcall FUN_0048cd80();
int __stdcall FUN_0048d220(char mode);
void FUN_00491b60();
void __stdcall FUN_00491d70(int a);
void FUN_00496790();
void __stdcall FUN_00498da0(View* p);
void __stdcall IssueMobileBuildOrders(View* arg);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall FUN_0048c7f0(View* arg);
void __stdcall IssueOrderToSelection(void* a, unsigned char b, Class_00438760 kind, Vec3* d, int e, int f);
int __stdcall FindGadgetIndexBySubstring(int value, char* name);
void __stdcall FUN_004a6a40(void* obj, int index);
void __stdcall CloseTopScreen(void* a);
void __stdcall FUN_004ab400(void* a, void* b);
void __stdcall SetCloseHandler(void* a, int b);
int GetTicks();
void ClearKeyQueue();
int __stdcall IsKeyDown(int a);
void EndGameFrame();
void RunEndGameState(void);
void FUN_00496bb0();
void __cdecl LeaveNetGameCallback();

// FUNCTION: 0x498f70
void __stdcall HandleLeftClick(View* param_1)
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
            index = FindGadgetIndexBySubstring(g_game->field_531->value, "STOP");
            if (index != -1) {
                FUN_004a6a40(g_game->field_519, index);
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
        IssueOrderToSelection(param_1, g_game->orderMode, kind, &g_game->pos, 0, 0);
    }
    if (param_1->field_8 & 4) {
        g_game->flags_2cc6 |= 0x20;
        return;
    }
    g_game->orderMode = 1;
    g_game->flags_2cc6 &= 0xdf;
    index = FindGadgetIndexBySubstring(g_game->field_531->value, "STOP");
    if (index != -1) {
        FUN_004a6a40(g_game->field_519, index);
    }
}


// Order-button handler: when the game is not in order mode, selects the STOP
// order (inlined body of 0x495860); otherwise dispatches on the order flags
// (+0x2cc6): bit 1 cancels the current order, bit 0 switches to the 0x13
// (STOP) selection, bit 2 hands a zero order kind to IssueOrderToSelection.
// FUNCTION: 0x499100
void __stdcall HandleRightClick(View* param_1)
{
    if (g_game->orderMode != 1) {
        g_game->orderMode = 1;
        g_game->flags_2cc6 &= 0xdf;
        int index = FindGadgetIndexBySubstring(g_game->field_531->value, "STOP");
        if (index != -1) {
            FUN_004a6a40(g_game->field_519, index);
        }
        return;
    }
    if (g_game->field_37efa == 0) {
        if (g_game->flags_2cc6 & 2) {
            if (param_1->field_8 & 8) {
                BeginMouseScroll();
                return;
            }
            FUN_0048bd00();
            FUN_00491d70(1);
            return;
        }
        if (g_game->flags_2cc6 & 1) {
            g_game->flags_2cc6 = g_game->flags_2cc6 | 0x10;
            if (g_game->selected != 0x13) {
                g_game->selected = 0x13;
                FUN_004ab400((void*)g_game->field_519, g_game->table[0x13]);
                return;
            }
        }
    } else if (g_game->flags_2cc6 & 4) {
        Class_00438760 kind;
        IssueOrderToSelection(param_1, 1, kind, &g_game->pos, 0, 0);
    }
}


static inline void SetCursor(int n)
{
    if (g_game->selected != n) {
        g_game->selected = n;
        FUN_004ab400(g_game->field_519, g_game->table[n]);
    }
}

// Main-loop frame handler. Copies the 24-byte view/input block off g_game,
// feeds it to the camera update, then runs the order/selection state machine
// off the flags byte at +0x2cc6 and the mouse message stored in the block.
// Advances the frame queues and, on the network/skirmish paths, flips the
// end-of-frame hooks.
// FUNCTION: 0x499200
void BattleFrame(void)
{
    View view = g_game->view;
    FUN_00498da0(&view);

    unsigned char flags = g_game->flags_2cc6;
    if ((flags & 2) != 0 && g_game->orderMode == 0xe) {
        FUN_004197d0();
    } else if ((flags & 2) == 0 && (flags & 1) == 0) {
        SetCursor(0x13);
    } else {
        g_game->field_2cba = FUN_0048cd80();
        SetCursor(FUN_0048d220(g_game->orderMode));
    }

    FUN_0041c180();
    if ((g_game->flags_2cc6 & 0x20) != 0) {
        if (IsKeyDown(0xf9) == 0) {
            g_game->orderMode = 1;
            g_game->flags_2cc6 &= 0xdf;
            int index = FindGadgetIndexBySubstring(g_game->field_531->value, "STOP");
            if (index != -1) {
                FUN_004a6a40(g_game->field_519, index);
            }
        }
    }

    flags = g_game->flags_2cc6;
    if ((flags & 0x10) != 0) {
        if (g_game->field_37efa == 0) {
            if (view.msg == 0x205) {
                g_game->flags_2cc6 = flags & 0xef;
            } else {
                FUN_0041d0f0();
            }
        } else {
            if (view.msg == 0x202) {
                g_game->flags_2cc6 = flags & 0xef;
            } else {
                FUN_0041d0f0();
            }
        }
    } else if (g_game->field_2cdf != 0) {
        UpdateMouseScroll();
    } else if (view.msg == 0x204) {
        HandleRightClick(&view);
    } else if (g_game->orderMode != 1) {
        if (view.msg == 0x201) {
            HandleLeftClick(&view);
        }
    } else if ((flags & 8) != 0) {
        if (view.msg == 0x202) {
            g_game->flags_2cc6 = flags & 0xf7;
            int dx = g_game->field_2c92 - g_game->field_2c9e;
            int dz = g_game->field_2c9a - g_game->field_2ca6;
            dx = abs(dx);
            dz = abs(dz);
            int now = GetTicks();
            if (g_game->field_2cb6 + 0x19 > now && dx < 0x20 && dz < 0x20) {
                HandleLeftClick(&view);
            } else if (SelectUnitsInBox(&view) == 0) {
                FUN_0048bd00();
                FUN_00491d70(1);
            }
        } else {
            g_game->field_2c9e = g_game->field_2cac;
            g_game->field_2ca2 = g_game->field_2cb0;
            g_game->field_2ca6 = g_game->field_2cb4;
        }
    } else if (view.msg == 0x201) {
        if ((flags & 2) != 0) {
            g_game->flags_2cc6 = flags | 8;
            g_game->field_2cb6 = GetTicks();
            g_game->field_2c92 = g_game->field_2cac;
            g_game->field_2c96 = g_game->field_2cb0;
            g_game->field_2c9a = g_game->field_2cb4;
            g_game->field_2c9e = g_game->field_2cac;
            g_game->field_2ca2 = g_game->field_2cb0;
            g_game->field_2ca6 = g_game->field_2cb4;
            SetCursor(0x13);
        } else if (g_game->field_37efa == 1) {
            if ((flags & 1) != 0) {
                g_game->flags_2cc6 = flags | 0x10;
                SetCursor(0x13);
            }
        } else if ((flags & 1) != 0) {
            HandleLeftClick(&view);
        }
    }

    FUN_00496790();
    {
        unsigned short unit = g_game->field_37e9c;
        if (unit != 0 && *(short*)(g_game->units + unit * 0x118 + 0xa6) == 0) {
            FUN_00491d70(0);
        }
    }

    if (g_game->field_3923b.bits.b2 || g_game->field_3923b.bits.b4) {
        if (g_game->net->FUN_00435100() != 3 ||
            (((Mission*)g_game->net)->FUN_00435100() == 3 &&
             FUN_004572a0() != 0)) {
            SetCursor(0x13);
            FUN_00491d70(1);
            CloseTopScreen(g_game->field_519);
            if (g_game->net->FUN_00435100() == 3) {
                FUN_00463c80();
                FUN_00496790();
            }
            FUN_00491b60();
            ClearKeyQueue();
            g_game->field_10->SetTrackCategory(4);
            g_game->field_391f1 = 7;
            g_game->field_391f5 = EndGameFrame;
            SetCloseHandler(LeaveNetGameCallback, 0);
            FUN_0041d9f0(0);
        }
    }

    if (g_game->field_39249 != 0) {
        // Each arm keeps its own copy of the hook stores, and `|= 4` in both: the compiler merges them.
        if (g_game->net->FUN_00435100() == 1) {
            FUN_00491b60();
            FUN_00491d70(1);
            CloseTopScreen(g_game->field_519);
            BlankScreen();
            int a = ((Mission*)g_game->net)->FUN_00435c50();
            char* b = ((Mission*)g_game->net)->FUN_004352b0();
            ((Mission*)g_game->net)->LoadCampaign(b);
            if (((Mission*)g_game->net)->FUN_00435c00(a) != 0) {
                g_game->field_2a44.bits.b3 = 1;
                g_game->field_2a44.value |= 4;
            }
            g_game->field_391f1 = 2;
            g_game->field_391f5 = FUN_00496bb0;
            SetCloseHandler(LeaveNetGameCallback, 0);
        } else {
            unsigned int saved = g_game->field_2a3c;
            FUN_00491b60();
            FUN_00491d70(1);
            CloseTopScreen(g_game->field_519);
            BlankScreen();
            SetCursor(0x14);
            g_game->field_2a3c = saved;
            ((Mission*)g_game->net)->LoadMissionByName(g_game->field_29a0 + 0x11c);
            FUN_0047a760();
            g_game->field_2a44.value |= 4;
            g_game->field_391f1 = 2;
            g_game->field_391f5 = FUN_00496bb0;
            SetCloseHandler(LeaveNetGameCallback, 0);
        }
        g_game->field_10->SetTrackCategory(4);
    }
}

// FUNCTION: 0x499880
void EndGameFrame(void)
{
    RunEndGameState();
}
