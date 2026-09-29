// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 53.4% (best effort within the 10 minute timebox): full structural
// translation of the battleroom setup handler. See the note at the bottom of
// the file for what still differs.
//
// Local frame map (esp after prologue = F-0x38):
//   [esp+0x10] = F-0x28  metal value   (init 1000, overwritten by DAT_00512d74)
//   [esp+0x14] = F-0x24  energy value  (init 1000, overwritten by DAT_00512d70)
//   [esp+0x18] = F-0x20  holder returned by FUN_004aa8f0
//   [esp+0x1c] = F-0x1c  player base, later reused for maxunits-20
//   [esp+0x20] = F-0x18  spilled copy of (info->field_97 & 1)
//   [esp+0x24] = F-0x14  itoa text buffer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* g_game;                 // 0x511de8

extern int DAT_00512994;
extern int DAT_0050550c;
extern int DAT_00512764;
extern int DAT_00512d6c;
extern int DAT_00512d68;
extern int DAT_00512d70;
extern int DAT_00512d74;
extern int DAT_00512d78;
extern int DAT_00512d7c;
extern int DAT_00512d80;
extern int DAT_00512d84;
extern int DAT_00512d88;
extern int DAT_00512d8c;
extern char DAT_00512ce8;
extern const char* DAT_00505518[];
extern const char* DAT_005054b0[];

extern "C" {
int __stdcall FUN_0041d6a0(int);
void __stdcall FUN_004288d0(const char*, int, int, int);
void __stdcall FUN_00428b60(void);
void FUN_00447b10(void);
void __stdcall FUN_00445b70(void*, int);
void __stdcall FUN_00445c70(void*, int);
void __stdcall FUN_00445d60(void);
void __stdcall FUN_004455b0(void);
void __stdcall FUN_00445ed0(void);
void __stdcall FUN_00446a50(void);
void __stdcall FUN_00448c70(void);
void __stdcall FUN_00450f90(void);
void __stdcall FUN_00451180(void);
int __stdcall FUN_00456760(void);
int __stdcall FUN_00457a50(void);
int __stdcall FUN_0045b660(void);
void __stdcall FUN_0045b9b0(void*, int);
int __stdcall FUN_0045ba20(void*);
void __stdcall FUN_0046c8e0(int);
void __stdcall FUN_0049fa90(void*);
void __stdcall FUN_0049fb10(void*, int);
int __stdcall FUN_0049fdf0(void*, const char*, int);
void* __stdcall FUN_0049ff90(void*, const char*);
int __stdcall FUN_004a0180(void*, const char*);
void* __stdcall FUN_004a0200(void*, const char*);
int __stdcall FUN_004a0280(void*, const char*);
void __stdcall FUN_004a0570(void*, const char*, int);
void __stdcall FUN_004a0bf0(void*, const char*, const char*, int);
void __stdcall FUN_004a1250(void*, const char*, int);
void __stdcall FUN_004a1450(void*, const char*, int);
void __stdcall FUN_004a32a0(void*, const char*, void*, int, int);
void __stdcall FUN_004a7190(void*, int);
void __stdcall FUN_004a81e0(void*, int);
void* __stdcall FUN_004aa8f0(void*, const char*, int);
int __stdcall FUN_004ab060(void*, const char*);
void __stdcall FUN_004b6290(const char*);
int __stdcall FUN_004b8d40(void*, const char*);
void* __stdcall FUN_004d83b0(const char*, int);
}

class Class_00435920 { public: int FUN_00435920(); };
class Class_00435a20 { public: void FUN_00435a20(void*); };
class Class_00435c30 { public: char* FUN_00435c30(); };
class Class_00435c40 { public: bool FUN_00435c40(); };
class Class_00435d30 { public: void FUN_00435d30(int); };
class Class_004373a0 { public: int FUN_004373a0(); };
class Class_0046e000 { public: int FUN_0046e000(); };

#pragma pack(push, 1)

// Layout object returned by FUN_004aa8f0: the "entries" list starts at +4,
// +8 is a callback and +0xc is the game pointer.
struct Holder_00449bb0 {
    int unknown_0;                  // +0x00
    void* entries;                  // +0x04
    void* callback;                 // +0x08
    void* game;                     // +0x0c
};

// Local player info, stride 0x14b (see 0x444930 for the same array).
struct PlayerInfo_00449bb0 {
    char unknown_0[0x8b];
    short field_8b;                 // +0x8b
    short field_8d;                 // +0x8d
    char unknown_8f[0x97 - 0x8f];
    unsigned char field_97;         // +0x97
    char unknown_98[0x99 - 0x98];
    unsigned short field_99;        // +0x99
    unsigned short field_9b;        // +0x9b (bits 8-10 are the second byte)
    unsigned short field_9d;        // +0x9d
    char unknown_9f[0xa1 - 0x9f];
    short field_a1;                 // +0xa1
    char unknown_a3[0xa5 - 0xa3];
    short field_a5;                 // +0xa5
    char unknown_a7[0xa9 - 0xa7];
    int field_a9;                   // +0xa9
};

// Entry in the GUI list, stride 0x15b. +0xb6 doubles as a text buffer for the
// MEMx control.
struct GuiEntry_00449bb0 {
    char unknown_0[0x1b];
    int field_1b;                   // +0x1b
    char unknown_1f[0x23 - 0x1f];
    int field_23;                   // +0x23
    char unknown_27[0xb6 - 0x27];
    char text[0x138 - 0xb6];        // +0xb6
    unsigned short field_138;       // +0x138
};

// Gadget created by FUN_004a0200.
struct Gadget_00449bb0 {
    char unknown_0[0x13c];
    int field_13c;                  // +0x13c
    unsigned short field_140;       // +0x140
    char unknown_142[0x144 - 0x142];
    void* field_144;                // +0x144
    char unknown_148[0x14a - 0x148];
    void* field_14a;                // +0x14a
};

struct Battlestart_00449bb0 {
    char unknown_0[0xbe];
    int field_be;                   // +0xbe
    char unknown_c2[0xc6 - 0xc2];
    unsigned short field_c6;        // +0xc6
    int field_c8;                   // +0xc8
};

#pragma pack(pop)

// FUNCTION: 0x449bb0
void FUN_00449bb0(void)
{
    int metalVal = 1000;
    int energyVal = 1000;
    void* holder = 0;
    int local_1c = 0;
    int flag97 = 0;
    char text[16];

    DAT_00512994 = 0;
    DAT_0050550c = -1;
    *(unsigned char*)(g_game + 0x2bee) |= 1;
    memset(g_game + 0x2c28, 0, 0x2c);

    int index = *(unsigned char*)(g_game + 0x2a42);
    local_1c = (int)(g_game + index * 0x14b);
    PlayerInfo_00449bb0* info =
        *(PlayerInfo_00449bb0**)(local_1c + 0x1b8a);

    flag97 = info->field_97 & 1;
    *(unsigned short*)(g_game + 0x37ee8) = 0;
    if (flag97 != 0) {
        *(unsigned short*)(g_game + 0x37eea) = *(unsigned short*)(g_game + 0x37eec);
    }

    info->field_8b = *(unsigned short*)(g_game + 0x37f1b);
    info->field_8d = *(unsigned short*)(g_game + 0x37f1f);

    bool c = FUN_0041d6a0(1) != 0;
    info->field_9d = (info->field_9d & 0xfffb) | ((c ? 1 : 0) << 2);

    holder = FUN_004aa8f0(g_game + 0x519, "LOUNGE2.GUI", 0);
    ((Holder_00449bb0*)holder)->callback = (void*)&FUN_00447b10;
    ((Holder_00449bb0*)holder)->game = g_game;

    FUN_004288d0("battleroom", 0, 1, 0);

    DAT_00512764 = (int)*(short*)((char*)((Holder_00449bb0*)holder)->entries + 0xb6);

    GuiEntry_00449bb0* list =
        (GuiEntry_00449bb0*)*(int*)(*(int*)(g_game + 0x531) + 4);

    int i = FUN_0049fdf0(list, "MESSAGE", 3);
    if (i != -1) {
        list[i].field_138 = 0x7f;
    }

    if (FUN_00457a50() == 0) {
        int j = FUN_0049fdf0(list, "MAP", 1);
        if (j != -1) {
            list[j].field_1b = 2;
            FUN_004a0bf0(g_game + 0x519, "MAP", "View Map", 0);
        }
    }

    if (FUN_0045b660() != 0) {
        if (DAT_00512d6c != 0) {
            info->field_a5 = (short)DAT_00512d6c;
            *(unsigned short*)(g_game + 0x37eea) = (unsigned short)DAT_00512d6c;
        }

        unsigned short old = *(unsigned short*)(g_game + 0x2c74);
        *(unsigned short*)(g_game + 0x2c74) =
            (old & 0xfffe) | (DAT_00512d68 != 0 ? 1 : 0);

        int v = DAT_00512d78 - 1;
        if (DAT_00512d78 != 0 && v >= 0 && v <= 2) {
            *(int*)(g_game + 0x39229) = v;
            *(int*)(g_game + 0x37ef6) = v;
            PlayerInfo_00449bb0* pi =
                *(PlayerInfo_00449bb0**)(g_game + index * 0x14b + 0x1b8a);
            pi->field_9b = (pi->field_9b & 0xe7ff) | ((v & 3) << 0xb);
        }

        if (DAT_00512d70 != 0) {
            energyVal = DAT_00512d70;
        }
        if (DAT_00512d74 != 0) {
            metalVal = DAT_00512d74;
        }

        if (DAT_00512d7c != 0) {
            if (DAT_00512d7c == 1) {
                info->field_9b |= 0x600;
            } else if (DAT_00512d7c == 2) {
                info->field_9b = (info->field_9b & 0xfbff) | 0x200;
            } else if (DAT_00512d7c == 3) {
                info->field_9b &= 0xfdff;
            }
        }
        if (DAT_00512d80 != 0) {
            info->field_9b = (info->field_9b & 0xdfff) | ((DAT_00512d80 == 2) << 0xd);
        }
        if (DAT_00512d84 != 0) {
            info->field_9b = (info->field_9b & 0xbfff) | ((DAT_00512d84 == 1) << 0xe);
        }
        if (DAT_00512d88 != 0) {
            info->field_9b = (info->field_9b & 0xfeff) | ((DAT_00512d88 == 1) << 8);
        }
        if (DAT_00512d8c != 0) {
            info->field_9b = (info->field_9b & 0xff7f) | ((DAT_00512d8c == 2) << 7);
        }
    } else if (flag97 != 0) {
        info->field_9b =
            (info->field_9b & 0xfeff) |
            ((*(unsigned char*)(g_game + 0x3922d) & 1) << 8);
        info->field_9b =
            (info->field_9b & 0xfdff) |
            ((*(unsigned char*)(g_game + 0x39231) & 1) << 9);
        info->field_9b =
            (info->field_9b & 0xfbff) |
            ((*(unsigned char*)(g_game + 0x39235) & 1) << 0xa);
        info->field_9b =
            (info->field_9b & 0xbfff) |
            ((*(unsigned char*)(*(int*)(g_game + 0x29a0) + 0x118) & 1) << 0xe);
        info->field_9b =
            (info->field_9b & 0xe7ff) |
            ((*(unsigned char*)(g_game + 0x39229) & 3) << 0xb);
    }

    if (flag97 != 0) {
        if ((*(unsigned char*)(g_game + 0x2c74) & 1) == 0) {
            goto L_a042;
        }
    }

    {
        const char** p = DAT_00505518;
        while (*p != 0) {
            FUN_004a1250(g_game + 0x519, *p, 1);
            p++;
        }
    }

L_a042:
    FUN_00445ed0();

    *(void**)(g_game + 0x2a9b) = FUN_004d83b0("LOUNGE CHATTER", 0xa00);
    **(unsigned char**)(g_game + 0x2a9b) = 0;

    GuiEntry_00449bb0* mem =
        (GuiEntry_00449bb0*)FUN_004a0180(list, "MEMx");

    int r = ((Class_00435920*)*(int*)(g_game + 0x391e9))->FUN_00435920();
    {
        PlayerInfo_00449bb0* pi =
            *(PlayerInfo_00449bb0**)(local_1c + 0x1b8a);
        unsigned short cnt = pi->field_99;
        int a = ((int)cnt >= r) ? 1 : 0;
        a = (a - 1) & 0xc;
        mem->field_23 = a;
    }
    sprintf(mem->text, "%d",
            *(unsigned short*)(*(int*)(g_game + index * 0x14b + 0x1b8a) + 0x99));

    flag97 = (*(unsigned char*)(*(int*)(g_game + index * 0x14b + 0x1b8a) + 0x97)) & 1;
    FUN_0046c8e0((short)flag97);

    int startArg = ((Class_0046e000*)*(int*)(g_game + 0x2a30))->FUN_0046e000();
    FUN_004a0570(g_game + 0x519, "START", startArg);

    int enable;
    if (flag97 != 0 && FUN_00456760() != 0 &&
        ((Class_0046e000*)*(int*)(g_game + 0x2a30))->FUN_0046e000() != 0) {
        enable = 0;
    } else {
        enable = 1;
    }
    FUN_004a1250(g_game + 0x519, "START", enable);
    FUN_004a1250(g_game + 0x519, "RESTRICTIONS", 0);
    FUN_004a32a0(g_game + 0x519, "OUTPUT", *(void**)(g_game + 0x2a9b), 0, 0);

    {
        GuiEntry_00449bb0* e = (GuiEntry_00449bb0*)FUN_0049ff90(list, "OUTPUT");
        e->field_1b |= 0x100;
    }
    FUN_004a0bf0(g_game + 0x519, "METALTEXT", "0", 0);
    FUN_004a0bf0(g_game + 0x519, "ENERGYTEXT", "0", 0);

    {
        GuiEntry_00449bb0* gadgets =
            (GuiEntry_00449bb0*)*(int*)(*(int*)(g_game + 0x531) + 4);
        int mi = FUN_0049fdf0(gadgets, "METAL", 0xe);
        if (mi != -1) {
            Gadget_00449bb0* g = (Gadget_00449bb0*)FUN_004a0200(gadgets, "METAL");
            g->field_13c = 0x2711;
            g->field_144 = (void*)&FUN_00445c70;
            g->field_140 = (unsigned short)metalVal;
            FUN_0045b9b0(g, (short)metalVal);
            g->field_14a = g_game;
        }
        FUN_00445c70(g_game + 0x519, mi);
        FUN_0049fa90(g_game + 0x519);
    }

    {
        GuiEntry_00449bb0* gadgets =
            (GuiEntry_00449bb0*)*(int*)(*(int*)(g_game + 0x531) + 4);
        int mu = (int)(*(unsigned short*)(g_game + 0x37eea)) - 0x14;
        local_1c = mu;
        int ui = FUN_0049fdf0(gadgets, "MAXUNITS", 0xe);
        if (ui != -1) {
            Gadget_00449bb0* g = (Gadget_00449bb0*)FUN_004a0200(gadgets, "MAXUNITS");
            g->field_13c = local_1c;
            g->field_144 = (void*)&FUN_00445b70;
            g->field_140 = (unsigned short)local_1c;
            FUN_0045b9b0(g, (short)local_1c);
            g->field_14a = g_game;
        }
        FUN_00445b70(g_game + 0x519, ui);
        FUN_0049fa90(g_game + 0x519);
    }

    if (flag97 == 0 || (*(unsigned char*)(g_game + 0x2c74) & 1) != 0) {
        FUN_004a1450(g_game + 0x519, "MAXUNITS", 1);
        FUN_004a1450(g_game + 0x519, "ENERGY", 1);
        FUN_004a1450(g_game + 0x519, "METAL", 1);
    }

    {
        int list2 = *(int*)(*(int*)(g_game + 0x531) + 4);
        int ei = FUN_0049fdf0((void*)list2, "ENERGY", 0xe);
        if (ei != -1) {
            Gadget_00449bb0* g =
                (Gadget_00449bb0*)FUN_004a0200((void*)list2, "ENERGY");
            g->field_140 = (unsigned short)energyVal;
            g->field_13c = 0x2711;
            g->field_144 = (void*)&FUN_00445d60;
            FUN_0045b9b0(g, (short)energyVal);
            g->field_14a = g_game;
        }

        Gadget_00449bb0* ge =
            (Gadget_00449bb0*)FUN_004a0200((void*)list2, "ENERGY");
        if (ge != 0) {
            int amount = (FUN_0045ba20(ge) / 100) * 100;
            _itoa(amount, text, 10);
            FUN_004a0bf0(g_game + 0x519, "ENERGYTEXT", text, 0);
            PlayerInfo_00449bb0* pi = info;
            pi->field_a1 = (short)(amount / 100);
            if ((pi->field_97 & 1) != 0) {
                FUN_00450f90();
                FUN_00451180();
            }
        }
        FUN_0049fa90(g_game + 0x519);
    }

    ((Class_00435d30*)*(int*)(g_game + 0x391e9))->FUN_00435d30(1);

    if (flag97 != 0 && FUN_0045b660() != 0 && DAT_00512ce8 != 0) {
        ((Class_00435a20*)*(int*)(g_game + 0x391e9))->FUN_00435a20(&DAT_00512ce8);
    }

    if (!((Class_00435c40*)*(int*)(g_game + 0x391e9))->FUN_00435c40()) {
        FUN_004b6290("Could not find the multiplayer map!!");
    }

    strcpy((char*)info,
           ((Class_00435c30*)*(int*)(g_game + 0x391e9))->FUN_00435c30());
    info->field_a9 = ((Class_004373a0*)*(int*)(g_game + 0x391e9))->FUN_004373a0();

    FUN_00450f90();

    FUN_004a7190(g_game + 0x519, FUN_0049fdf0(list, "MESSAGE", 0xe));

    {
        const char** p = DAT_005054b0;
        while (*p != 0) {
            int k = FUN_0049fdf0(((Holder_00449bb0*)holder)->entries, *p, 0xe);
            if (k != -1) {
                ((char*)((Holder_00449bb0*)holder)->entries)[k * 0x15b + 0x29] = 0;
            }
            p++;
        }
    }

    FUN_004455b0();
    FUN_00446a50();
    FUN_00448c70();
    FUN_00428b60();

    if (FUN_004ab060(g_game + 0x519, "LOUNGE2.GUI") != 0) {
        Battlestart_00449bb0* b = (Battlestart_00449bb0*)FUN_004a0280(
            ((Holder_00449bb0*)holder)->entries, "battlestart");
        b->field_be = FUN_004b8d40(
            *(void**)((char*)((Holder_00449bb0*)holder)->entries + 0xc0),
            "battlestart");
        b->field_c6 = 0;
        b->field_c8 |= 1;
        FUN_004a0570(g_game + 0x519, "battlestart", 1);
    }

    FUN_0049fb10(g_game + 0x519, 1);
    FUN_004a81e0(g_game + 0x519, 0x40);
}

// Remaining differences as left by deepseek-v4.1-flash (check.py: 53.4%):
// - Semantics are complete, but MSVC's frame-slot allocation differs:
//   original frame map is metalVal@0, energyVal@4, holder@8, local_1c@0xc,
//   flag97@0x10, text@0x14; ours came out flag97@0, metalVal@4, energyVal@8,
//   holder@0xc, local_1c@0x10, list@0x14, text@0x18. Renaming/reordering the
//   declarations did not move flag97 off offset 0. The `list` local also gets
//   a stack slot where the original keeps the GUI entry table in edi.
// - `*(unsigned char*)(g_game+0x2bee) |= 1;` compiles to mov/or/mov through
//   cl here, original has a single `or byte ptr [eax+0x2bee], 1`.
// - the 0x2c74 bit-0 update in the FUN_0045b660()!=0 path: original emits
//   xor/and/xor (bit assignment through a bitfield), ours writes mask/or;
//   declare the field as a 1-bit bitfield in the game struct.
// - the LUP/PLAYERx loops and the FUN_004ab060 block pick different scratch
//   registers (eax/edx swaps) because of the frame-offset differences above.
// - FUN_0045ba20's division by 100 and the following /100 both reduce to
//   multiply by 0x51eb851f + sar 5 + sign fixup; kept as plain /100.
// - entry/gadget structs are declared from observed offsets only; the real
//   types are shared with 0x444930's Entry/Holder.
