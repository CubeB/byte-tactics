// Decompiled by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// Load game screen click handler (sibling of 0x492df0, save game screen).
//
// MATCHED (1964 of 1964 bytes).
//
// The last fault was a one-byte register-allocation difference that showed up
// as jump targets one byte off: the store `g_game->p38d6b = 0;` after freeing
// the save object used ecx for the g_game load where the original used the
// 5-byte `mov eax, [g_game]`. The original source called the already-matched
// helper 0x432590 (FUN_00432590, "if (obj) { obj->FUN_004b3630(); operator
// delete(obj); }") and /Ob2 inlined it; reproducing that call through an
// inline helper, with the field test outside it, gives the original's
// registers at both delete sites. A bare local pointer could not.
//
// The game struct needs #pragma pack(1); missing the 4-byte pad between the
// pointer at +0x391e9 and +0x391f1 shifts every later g_game field by 4.
// The flags at +0x2a44 are a bitfield union: reading one gives the original's
// `mov cl,[..]; shr cl,2; test cl,1` (a plain `(v >> 2) & 1` folds to
// `test byte ptr [..],4`), and setting one still gives `or byte ptr`.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_00492360 {                  // 0x15b bytes
    char unknown_0[0xb6];
    char text[4];                        // +0xb6
    short field_ba;                      // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Layer_00492360 {
    int unknown_0;
    Entry_00492360* entries;             // +0x04
};

struct Gadget_00492360 {
    char unknown_0[0x18];
    Layer_00492360* layer;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                        // +0x60
};

struct Obj_00492360 {
    char unknown_0[0x108];
    int field_108;                       // +0x108
    int field_10c;                       // +0x10c
    int field_110;                       // +0x110
    int field_114;                       // +0x114
    int field_118;                       // +0x118
    char text_11c[1];                    // +0x11c
};

struct Game {
    char unknown_0[0x519];
    char message[0x1b8a - 0x519];        // +0x519
    void* p1b8a;                         // +0x1b8a
    char unknown_1b8e[0x1cd5 - 0x1b8e];
    void* p1cd5;                         // +0x1cd5
    char unknown_1cd9[0x29a0 - 0x1cd9];
    Obj_00492360* p29a0;                 // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    short field_2a3c;                    // +0x2a3c
    char unknown_2a3e[0x2a44 - 0x2a3e];
    union Flags_2a44 {
        unsigned short value;
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short rest : 12;
        };
    } flags_2a44;                        // +0x2a44
    char unknown_2a46[0x2cbe - 0x2a46];
    unsigned char field_2cbe;            // +0x2cbe
    char unknown_2cbf[0x148cf - 0x2cbf];
    void* p148cf;                        // +0x148cf
    char unknown_148d3[0x37eee - 0x148d3];
    int field_37eee;                     // +0x37eee
    int field_37ef2;                     // +0x37ef2
    char unknown_37ef6[0x38c6b - 0x37ef6];
    char saveName[0x38d6b - 0x38c6b];    // +0x38c6b
    void* p38d6b;                        // +0x38d6b
    char unknown_38d6f[0x391cf - 0x38d6f];
    char buf391cf[0x391e9 - 0x391cf];    // +0x391cf
    void* p391e9;                        // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                     // +0x391f1
    void (*field_391f5)();               // +0x391f5
    char unknown_391f9[0x3923b - 0x391f9];
    union Flags_3923b {
        unsigned short value;
        struct {
            unsigned short b0 : 1;
            unsigned short b1 : 1;
            unsigned short b2 : 1;
            unsigned short b3 : 1;
            unsigned short rest : 12;
        };
    } flags_3923b;                       // +0x3923b
};
#pragma pack(pop)

struct Class_00435100 { int FUN_00435100(); };
struct Class_00435110 { void FUN_00435110(char* name); };
struct Class_00435a20 { void* FUN_00435a20(char* name); };
struct Class_004b3630 { void FUN_004b3630(); };
struct Class_004b4560 { void FUN_004b4560(char* name); };
struct Class_004b4800 { int FUN_004b4800(char* name, int def); };
struct Class_004b48a0 { char* FUN_004b48a0(char* name, int def); };
struct Class_004b48f0 { int FUN_004b48f0(char* name); };

extern Game* g_game;
extern char* DAT_005091c8;
extern char* DAT_0051f2e0;
extern char* DAT_0051f2e4;
extern char* DAT_0051f2e8;
extern char* DAT_0051f2ec;

void __cdecl operator delete(void* p);
void __stdcall FUN_0041d4c0();
char __stdcall FUN_0041d6a0(int flag);
void __stdcall FUN_0041da30();
void __stdcall FUN_004257a0();
void __stdcall FUN_00425860(int code, int line, char* file);
void* __stdcall FUN_00432520(char* path);
void __stdcall FUN_00432590(void* handle);
void* __stdcall FUN_004325b0(char* path);
void __stdcall FUN_00434ab0(int value);
void __stdcall FUN_0047f1a0(char* name, int param);
void __stdcall FUN_00491b60();
void __stdcall FUN_00491d70(int flag);
void FUN_00496bb0();
void __stdcall FUN_0049fa70(void* menu);
int __stdcall FUN_0049fd60(Gadget_00492360* gadget, char* name);
Entry_00492360* __stdcall FUN_0049ff90(Entry_00492360* entries, char* name);
void __stdcall FUN_004ab0a0(Gadget_00492360* menu);
void __stdcall FUN_004ab400(void* menu, void* data);
void __stdcall FUN_004abd90(void* menu, char* message, int a, int b, int c);
void __stdcall FUN_004b4fd0(void (__cdecl *callback)(int), int param);
char* __stdcall FUN_004b6af0(char* text, int n);
void __cdecl FUN_004578f0(int param);
char* __stdcall FUN_004c5740(char* text);
void __cdecl FUN_004d85a0(void* p);

__inline void DeleteSave_00492360(Class_004b3630* obj)
{
    if (obj) {
        obj->FUN_004b3630();
        operator delete(obj);
    }
}

// FUNCTION: 0x492360
void __stdcall FUN_00492360(Gadget_00492360* gadget)
{
    Entry_00492360* entries = gadget->layer->entries;
    char buf[0x104];

    if (gadget->field_60 == -1)
        return;

    if (FUN_0049fd60(gadget, "CANCEL")) {
        if (g_game->flags_2a44.b2)
            FUN_0049fa70(g_game->message);
        if (DAT_0051f2e0)
            FUN_004d85a0(DAT_0051f2e0);
        if (DAT_0051f2e4)
            FUN_004d85a0(DAT_0051f2e4);
        if (DAT_0051f2e8)
            FUN_004d85a0(DAT_0051f2e8);
        DAT_0051f2e8 = 0;
        DAT_0051f2e4 = 0;
        DAT_0051f2e0 = 0;
        if (DAT_0051f2ec)
            FUN_004d85a0(DAT_0051f2ec);
        DAT_0051f2ec = 0;
        FUN_0047f1a0("Previous", 0);
        return;
    }
    if (!FUN_0049fd60(gadget, "LOAD") && !FUN_0049fd60(gadget, "GAMES")) {
        if (gadget->field_60 != -1)
            FUN_004ab0a0(gadget);
        return;
    }
    Entry_00492360* e = FUN_0049ff90(entries, "GAMES");
    sprintf(buf, "%s\\%s", DAT_005091c8, FUN_004b6af0(DAT_0051f2e0, e->field_ba));
    void* save = FUN_00432520(buf);
    if (save != 0) {
        int type = ((Class_004b4800*)save)->FUN_004b4800("Gametype", 0);
        FUN_00432590(save);
        switch (type) {
        case 1:
            if (!FUN_0041d6a0(0)) {
                FUN_004abd90(g_game->message,
                    FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"),
                    200, 1, 1);
                FUN_004ab0a0((Gadget_00492360*)g_game->message);
                return;
            }
            break;
        case 2:
            if (!FUN_0041d6a0(1)) {
                FUN_004abd90(g_game->message,
                    FUN_004c5740("Please insert the Multiplayer CD (Disc 1) and try again"),
                    200, 1, 1);
                FUN_004ab0a0((Gadget_00492360*)g_game->message);
                return;
            }
            break;
        default:
            goto invalid;
        }
        FUN_0041d4c0();
        FUN_004257a0();
        if (g_game->field_2cbe != 20) {
            g_game->field_2cbe = 20;
            FUN_004ab400(g_game->message, g_game->p148cf);
        }
        if (g_game->flags_2a44.b2)
            FUN_0049fa70(g_game->message);
        FUN_0047f1a0("SMLBUTTON", 0);
        e = FUN_0049ff90(entries, "GAMES");
        sprintf(g_game->saveName, "%s\\%s", DAT_005091c8,
                FUN_004b6af0(DAT_0051f2e0, e->field_ba));
        if (g_game->flags_2a44.b2)
            FUN_00491b60();
        g_game->flags_3923b.b3 = 1;

        g_game->p38d6b = FUN_004325b0(g_game->saveName);
        if (g_game->p38d6b == 0)
            goto invalid;
        ((Class_004b4560*)g_game->p38d6b)->FUN_004b4560("summary");
        FUN_00434ab0(((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("Gametype", 0));
        char* campaign = ((Class_004b48a0*)g_game->p38d6b)->FUN_004b48a0("Campaign", 0);
        if (campaign != 0)
            ((Class_00435110*)g_game->p391e9)->FUN_00435110(campaign);
        g_game->field_37ef2 = ((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("Side", 0);
        g_game->field_37eee = ((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("Difficulty", 0);
        if (((Class_00435100*)g_game->p391e9)->FUN_00435100() == 1) {
            if (g_game->field_37ef2 == 0) {
                *(unsigned char*)((char*)g_game->p1b8a + 0x95) = 0;
                *(unsigned char*)((char*)g_game->p1cd5 + 0x95) = 1;
            } else {
                *(unsigned char*)((char*)g_game->p1b8a + 0x95) = 1;
                *(unsigned char*)((char*)g_game->p1cd5 + 0x95) = 0;
            }
        }
        char* mission = ((Class_004b48a0*)g_game->p38d6b)->FUN_004b48a0("Mission", 0);
        if (mission == 0)
            goto invalid;
        if (strlen(mission) == 0)
            goto invalid;
        if (((Class_00435a20*)g_game->p391e9)->FUN_00435a20(mission) == 0)
            goto invalid;
        strcpy((char*)g_game->p29a0 + 0x11c, mission);
        char* thumbs = ((Class_004b48a0*)g_game->p38d6b)->FUN_004b48a0("Thumbs", 0);
        strncpy(g_game->buf391cf, thumbs, 0x19);
        if (strlen(g_game->buf391cf) != 0x19)
            FUN_0041da30();
        if (((Class_00435100*)g_game->p391e9)->FUN_00435100() == 2) {
            ((Class_004b4560*)g_game->p38d6b)->FUN_004b4560("summary");
            g_game->field_2a3c =
                (short)((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("Players", 0);
            g_game->p29a0->field_108 =
                ((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("CommanderDeath", 1);
            g_game->p29a0->field_118 =
                ((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("Location", 1);
            g_game->p29a0->field_10c =
                ((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("Mapping", 1);
            g_game->p29a0->field_110 =
                ((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("LineOfSight", 1);
            g_game->p29a0->field_114 =
                ((Class_004b4800*)g_game->p38d6b)->FUN_004b4800("LineOfSightType", 1);
        }
        g_game->flags_2a44.b2 = 1;
        g_game->field_391f1 = 2;
        g_game->field_391f5 = FUN_00496bb0;
        FUN_004b4fd0(FUN_004578f0, 0);
        if (DAT_0051f2e0)
            FUN_004d85a0(DAT_0051f2e0);
        if (DAT_0051f2e4)
            FUN_004d85a0(DAT_0051f2e4);
        if (DAT_0051f2e8)
            FUN_004d85a0(DAT_0051f2e8);
        DAT_0051f2e8 = 0;
        DAT_0051f2e4 = 0;
        DAT_0051f2e0 = 0;
        if (DAT_0051f2ec)
            FUN_004d85a0(DAT_0051f2ec);
        DAT_0051f2ec = 0;
        FUN_00491d70(1);
        if (((Class_00435100*)g_game->p391e9)->FUN_00435100() == 1 &&
            ((Class_004b48f0*)g_game->p38d6b)->FUN_004b48f0("BetweenMissions")) {
            g_game->flags_2a44.b3 = 1;
            if (g_game->p38d6b) {
                DeleteSave_00492360((Class_004b3630*)g_game->p38d6b);
                g_game->p38d6b = 0;
            }
            g_game->flags_2a44.b2 = 0;
            g_game->field_391f1 = 2;
            g_game->field_391f5 = FUN_00496bb0;
            FUN_004b4fd0(FUN_004578f0, 0);
            FUN_00425860(0xe, 0x48c, "c:\\cavedog\\wargame\\wargame.cpp");
        }
        FUN_004257a0();
        return;
    }
invalid:
    if (g_game->p38d6b) {
        DeleteSave_00492360((Class_004b3630*)g_game->p38d6b);
    }
    g_game->p38d6b = 0;
    FUN_004abd90(gadget, FUN_004c5740("Invalid savegame file"), 0x140, 1, 1);
    FUN_004ab0a0(gadget);
}
