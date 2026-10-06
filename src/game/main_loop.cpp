// Decompiled by Sonnet 5.5, Space Bunny Free, deepseek-v4.1-flash, Opus, DeepSeek V4.1 Flash and Claude Opus 5.5. Names are provisional.
// One pass of the main loop: rolls the per-frame timing buckets over, runs
// the frame pacing (0x495230) and, when it says frames are due, the
// simulation step and the three groups of profiled subsystem calls, charging
// the elapsed time to a bucket after each. Then the input/UI part, the
// mouse-capture and hotkey flag handling, the per-frame render step and the
// periodic "FRAM" marker.
//
// MATCH (711 of 711). Three changes to the previous 67.2% version, in the
// order they were found. The previous file's own diagnosis ("the clear of bit
// 2 folds to a memory `and`, whatever the field type, a local copy of the
// flag struct or an int temporary") was correct about the symptom and wrong
// about the cause: the spelling of the clear was never the problem, and no
// non-volatile spelling of it can produce the original's code.
//
// 1. The bit-4 test at the end has to be a bitfield READ of memory, not a
//    shift of a copy. The previous file tested `flags` held in a local:
//        unsigned short flags = g_game->word_37ebe;
//        if ((flags >> 4) & 1) ...
//    MSVC 5 folds that to `test al, 0x10` (2 bytes), where the original has
//    `mov dl,al; shr dl,4; test bl,dl` in the else branch (which must keep
//    `al` for the `and eax, 0xffef` store) and `shr al,4; test bl,al` in the
//    other (where `al` is dead). Writing the test as `if (g_game->bit4_37ebe)`
//    reads the 1-bit field, which MSVC 5 does NOT fold, and the compiler then
//    CSEs the second load against the `mov ax, word ptr [ecx+0x37ebe]` already
//    done for the 0x800 and 0x65 tests. This one change made 280 bytes of the
//    function identical, including the whole flag block and the last timer
//    charge, and it is why the check.py percentage barely moved: with 12
//    bytes still missing, every jump target in the function was wrong, and
//    difflib reshuffles the text diff. Read the lengths, not the percentage.
//
// 2. The word at g_game+0x38d75 is `volatile`. The original clears bit 2 of it
//    through a register (`mov cx,[m]; and ecx,0xfffb; mov [m],cx`) and MSVC 5
//    folds every plain spelling of that store into `and word ptr [m], 0xfffb`,
//    including a whole-struct copy, a local word, a pointer to the struct and
//    `m.word = m.word & 0xfffb`. The evidence in the exe is the AGENTS.md
//    criterion for this field, and it is strong: 0x498556 does
//    `mov cx,[m]; and ecx,0xfffb; mov [m],cx` and then 0x49856f, with nothing
//    in between but a reload of g_game, does `mov dx,[m]; or edx,8; mov [m],dx`
//    on the same location. A non-volatile field would be CSE'd and folded to
//    `and [m],0xfffb; or [m],8`. The same two shapes are at 0x497c57, 0x498323
//    and here. The union is what makes this work: the bitfield overlay is not
//    volatile (so the test at 0x496846 keeps its `mov al,byte; shr al,2;
//    test bl,al`, an 8-bit container would have folded to `test byte [m],4`),
//    and the word overlay is (so the clear stays a read-modify-write).
//
// 3. Nothing else. The register choice around the timer charges in the bit-2
//    block (`mov edx,[esi]; mov ecx,eax; sub ecx,edx` rather than the
//    swapped pair) and the one-byte `mov edx, g_game` before the paused
//    branch's charge were both fixed by 1 and 2, not by touching the charge
//    helper.
#include <stddef.h>
#include <stdio.h>
#include <string.h>

class PacketManager {
public:
    void SendAllQueued(int param_1);
};

#pragma pack(push, 1)
// The network flags at +0x38d75. The bitfield overlay is not volatile (so the
// bit 2 test keeps its load, shift and test) and the word overlay is (so the
// clear of bit 2 stays a read-modify-write through a register). See the note
// at the top of this file.
struct Flags_38d75 {
    union {
        volatile unsigned short word;   // cleared with &0xfffb
        struct {
            unsigned short pad : 2;
            unsigned short bit2 : 1;
            unsigned short rest : 13;
        };
    };
};

struct Timers_00496790 {
    int last;                          // +0x0
    int total;                         // +0x4
    int prev[9];                       // +0x8
    int cur[9];                        // +0x2c
};

struct Sub_00496b10 {
    char unknown_0[0xa6];
};

struct Sub_00496db0 {
    char unknown_0[0x96];
    char flag_96;                      // +0x96
};

struct Obj_004ab400;
struct Src_004ab400;

struct PlayerInfo_00496ce0 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97, bit 0 tested
};

struct Player_00496ee0 {
    char unknown_0[0x95];
    unsigned char nameIndex;           // +0x95
    unsigned char index2;              // +0x96
};

struct Flags_00496ce0 {
    unsigned short low : 4;            // +0x2b4c, bits 0-3
    unsigned short flag : 1;           // bit 4
    unsigned short rest : 11;
};

struct Slot_00497080 {                 // 0x18 bytes
    char unknown_0[0xc];
    int field_c;                       // +0xc
    int field_10;                      // +0x10
    char unknown_14[4];
};

struct TeamDef_00496ee0 {            // 0x18 bytes, array at g_game+0x29a0
    char unknown_0[0x4];
    unsigned char nameIndex;           // +0x4
    char unknown_5[0xc - 0x5];
    int size1;                         // +0xc
    int size2;                         // +0x10
    unsigned char index2;              // +0x14
    char unknown_15[0x18 - 0x15];
};

struct PlayerName_00496ee0 {            // 0x232 bytes, array at g_game+0x37f5f
    char name[0x232];
};

struct Struct_00496e90 {
    char unknown_0[0xdc];
    float width;                       // +0xdc
    float height;                      // +0xe0
    char unknown_e4[0x149 - 0xe4];
    unsigned short flag_149 : 1;       // +0x149
};

struct Settings_00496e10 {
    int value;                         // +0x0
    int flag_4;                        // +0x4
    int flag_8;                        // +0x8
    int flag_c;                        // +0xc
};

struct Vec3_00437320 {
    int x;
    int y;
    int z;
};

class Mission {
public:
    int FUN_00435100();
    int GetStartPosition(Vec3_00437320* out, int id);
};

union Fixed_00496ee0 {
    int i;                              // 16.16
    struct {
        short frac;
        short whole;
    } h;
};

struct FixedPos_00496ee0 {
    Fixed_00496ee0 x;
    Fixed_00496ee0 y;
    Fixed_00496ee0 z;
};

// The ten team records at g_game+0x1b63 are 0x14b bytes. Three functions view
// them under different names; the fields that disagree over the same bytes sit
// in anonymous unions. +0xdc and +0xe0 are the two clamped sizes as floats,
// +0x149 a 1-bit unsigned short bitfield (setting it gives the straight-to-
// memory `or byte ptr [ecx+0x149], 1`).
struct Player_00496ce0 {               // 0x14b bytes, array at g_game+0x1b63
    int active;                        // +0x0
    int dpid;                          // +0x4
    char unknown_8[0x27 - 0x8];
    union {
        PlayerInfo_00496ce0* info;     // +0x27
        Player_00496ee0* player;
    };
    char unknown_2b[0x73 - 0x2b];
    char type;                         // +0x73
    char unknown_74[0xdc - 0x74];
    union {
        float size1;                   // +0xdc
        float width;
    };
    union {
        float size2;                   // +0xe0
        float height;
    };
    char unknown_e4[0x146 - 0xe4];
    char field_146;                    // +0x146
    char unknown_147[0x148 - 0x147];
    union {
        struct {
            unsigned short bits0 : 8;  // +0x148
            unsigned short started : 1;
        };
        struct {
            char unknown_148[0x149 - 0x148];
            unsigned short flag_149 : 1;   // +0x149
            unsigned short rest_149 : 15;
        };
    };
};

// One view of the game state. The ten team records and the sub-object at
// +0x1cd5 overlap
// the same bytes, as do the name table at +0x37f5f and the timing block the
// main loop reads there, so each pair sits in an anonymous union. Two views
// name the pointer at +0x29a0 differently (team definitions, start-position
// slots), and two name the Mission pointer at +0x391e9 (obj, net).
struct Game {
    char unknown_0[0x519];
    Sub_00496b10 sub;                          // +0x519
    char unknown_5bf[0x1b63 - 0x5bf];
    union {
        Player_00496ce0 players[10];           // +0x1b63
        struct {
            char unknown_1b63[0x1cd5 - 0x1b63];
            Sub_00496db0* sub_1cd5;            // +0x1cd5
            char unknown_1cd9[0x2851 - 0x1cd9];
        };
    };
    char unknown_2851[0x29a0 - 0x2851];
    union {
        TeamDef_00496ee0* teams;               // +0x29a0
        Slot_00497080* slots;
    };
    char unknown_29a4[0x2a3c - 0x29a4];
    unsigned short state_2a3c;                 // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;                 // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    unsigned short bit0 : 1;                   // +0x2a44
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short bit3 : 1;
    unsigned short rest_2a44 : 12;
    char unknown_2a46[0x2b4c - 0x2a46];
    Flags_00496ce0 flags_2b4c;                 // +0x2b4c
    char unknown_2b4e[0x2bbe - 0x2b4e];
    unsigned char state_2bbe;                  // +0x2bbe
    unsigned char state_2bbf;                  // +0x2bbf
    char unknown_2bc0[0x2bee - 0x2bc0];
    unsigned short pad_2bee : 5;               // +0x2bee
    unsigned short flags_2bee : 3;
    unsigned short rest_2bee : 8;
    char unknown_2bf0[0x2cbe - 0x2bf0];
    signed char selected;                      // +0x2cbe
    char unknown_2cbf[0x14281 - 0x2cbf];
    unsigned short bit0_14281 : 1;             // +0x14281, bit 0
    unsigned short bit1_14281 : 1;             // bit 1
    unsigned short bit2_14281 : 1;             // bit 2
    unsigned short rest_14281 : 13;
    char unknown_14283[0x1487f - 0x14283];
    Src_004ab400* table[1];                    // +0x1487f
    char unknown_14883[0x37e37 - 0x14883];
    int viewWidth;                             // +0x37e37
    int viewHeight;                            // +0x37e3b
    char unknown_37e3f[0x37e9c - 0x37e3f];
    unsigned short field_37e9c;                // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    union {
        unsigned short word_37ebe;
        struct {
            unsigned short bit0_37ebe : 1;
            unsigned short bit1_37ebe : 1;
            unsigned short bit2_37ebe : 1;
            unsigned short bit3_37ebe : 1;
            unsigned short bit4_37ebe : 1;
            unsigned short bit5_37ebe : 1;
            unsigned short bit6_37ebe : 1;
            unsigned short bit7_37ebe : 1;
            unsigned short bits8_37ebe : 3;
            unsigned short bit11_37ebe : 1;
            unsigned short rest_37ebe : 4;
        };
    };   // +0x37ebe
    char unknown_37ec0[0x37ef6 - 0x37ec0];
    int value_37ef6;                           // +0x37ef6
    char unknown_37efa[0x37f5f - 0x37efa];
    union {
        PlayerName_00496ee0 names[8];          // +0x37f5f
        struct {
            char unknown_37f5f[0x38a37 - 0x37f5f];
            unsigned int lastTick;             // +0x38a37
            int frames;                        // +0x38a3b
            char unknown_38a3f[0x38a47 - 0x38a3f];
            unsigned int leadTick;             // +0x38a47
            char unknown_38a4b[0x38a51 - 0x38a4b];
            unsigned short paused : 1;
            unsigned short rest_38a51 : 15;    // +0x38a51
            char unknown_38a53[0x38b53 - 0x38a53];
            char text_38b53[0x100];            // +0x38b53
            int field_38c53;                   // +0x38c53
            int field_38c57;                   // +0x38c57
            unsigned int field_38c5b;          // +0x38c5b
            char unknown_38c5f[0x38d75 - 0x38c5f];
            Flags_38d75 flags_38d75;           // +0x38d75
            char unknown_38d77[0x38d85 - 0x38d77];
            Timers_00496790 timers;            // +0x38d85
        };
    };
    char unknown_390ef[0x391e9 - 0x390ef];
    union {
        Mission* obj_391e9;                    // +0x391e9
        Mission* net;
    };
    char unknown_391ed[0x391f1 - 0x391ed];
    int mode;                                  // +0x391f1
    void (*handler)();                         // +0x391f5
    char unknown_391f9[0x3923b - 0x391f9];
    unsigned short bits_3923b : 2;             // +0x3923b
    unsigned short flag2_3923b : 1;
    unsigned short bit3_3923b : 1;
    unsigned short flag4_3923b : 1;
    unsigned short rest_3923b : 11;
};
#pragma pack(pop)

extern Game* g_game;
extern int g_usePacketManager;
extern PacketManager g_packetManager;
extern int DAT_0051f300;
extern int DAT_0051f304;

int GetMilliseconds();
void UpdateFramePacing();
void __stdcall RunGameSteps(int param_1);
void FUN_00428c00();
void FUN_00428c10();
void FUN_00428c20();
void FUN_00428c30();
void FUN_00428c40();
void FUN_00428c50();
int FUN_004568c0();
void HandleNetPackets();
unsigned int GetTicks();
int GetLocalDpid();
void __stdcall FUN_00453320(int a, int b);
void HandleGameKey();
void UpdateEdgeScroll();
void FUN_0048bae0();
void RefreshSelectionOrders();
void __stdcall FUN_00468cf0(int a, int b);
void __stdcall SaveScreenshot(char* buf, char* name);
void __stdcall FUN_004ab400(Obj_004ab400* p, Src_004ab400* src);
void FUN_004c2870();
void __stdcall FUN_00434ab0(int param);
void ClearKeyQueue();
void __cdecl LeaveNetGameCallback(int param);
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param);
void FUN_00496bb0();
void EnterMainMenuState();
void __stdcall FUN_0049fa50(Sub_00496b10* p);
void RefreshUnitInfo();
void RunFrontendStateMachine();
void FUN_004c2470();
void FUN_00496ce0();
void FUN_00496db0();
void LoadingScreenFrame();
void __stdcall FUN_004ab170(Sub_00496b10* sub, unsigned int* param_2, int* param_3);
int __stdcall BroadcastPacket(int player, void* data, int size);
void SendNetHeartbeat();
void __stdcall FUN_00464290(unsigned char player, unsigned char kind);
unsigned short __stdcall FindUnitTypeId(const char* name);
void __stdcall FatalError(char* message);
void __stdcall SetCameraPosition(int x, int y, int instant);
void __stdcall CreateUnit(int team, unsigned short id, FixedPos_00496ee0 pos, int a, int b,
    int c);

static inline void Charge(int bucket)
{
    Timers_00496790* t = &g_game->timers;
    int now = GetMilliseconds();
    t->cur[bucket] += now - t->last;
    t->last = now;
}
#define CHARGE(bucket) Charge(bucket)

// FUNCTION: 0x496790
void FUN_00496790()
{
    Timers_00496790* t = &g_game->timers;
    t->total = 0;
    for (int i = 0; i < 9; i++) {
        t->total += t->cur[i];
        t->prev[i] = t->cur[i];
        t->cur[i] = 0;
    }
    if (t->total <= 0)
        t->total = 1;
    t->last = GetMilliseconds();

    if (g_game->bit0) {
        UpdateFramePacing();
        if (g_game->frames != 0) {
            RunGameSteps(1);
            CHARGE(8);
            FUN_00428c00();
            FUN_00428c10();
            FUN_00428c20();
            if (g_game->flags_38d75.bit2) {
                if (FUN_004568c0())
                    g_game->flags_38d75.word &= 0xfffb;
                CHARGE(0);
            }
            FUN_00428c30();
            FUN_00428c40();
            FUN_00428c50();
        } else if (g_game->paused) {
            if (g_usePacketManager)
                g_packetManager.SendAllQueued(0);
            HandleNetPackets();
            if ((int)GetTicks() > DAT_0051f300) {
                DAT_0051f300 = GetTicks() + 60;
                FUN_00453320(GetLocalDpid(), 0);
            }
            CHARGE(0);
        }
    } else if (!g_game->bit0_37ebe) {
        if (!g_game->paused) {
            UpdateFramePacing();
            if (g_game->frames != 0) {
                RunGameSteps(0);
                CHARGE(8);
            }
        }
    }
    if (!g_game->bit0_37ebe) {
        HandleGameKey();
        UpdateEdgeScroll();
    }
    FUN_0048bae0();
    unsigned short flags = g_game->word_37ebe;
    if ((flags & 0x800) || (flags & 0x65) || g_game->flags_2bee) {
        if (g_game->bit4_37ebe)
            g_game->field_37e9c = 0;
    } else {
        if (g_game->bit4_37ebe) {
            g_game->bit4_37ebe = 0;
            RefreshSelectionOrders();
        }
    }
    FUN_00468cf0(1, 1);
    CHARGE(8);
    FUN_00428c40();
    if (g_game->field_38c53 > 0 && g_game->field_38c5b <= g_game->leadTick) {
        SaveScreenshot(g_game->text_38b53, "FRAM");
        g_game->field_38c5b += 30 / g_game->field_38c57;
        g_game->lastTick = GetTicks();
    }
    FUN_00428c50();
}

// Sets the game selection to 0x13 (inlined body of FUN_00491c80), resets the
// input/UI state and installs the game's own handler.
// FUNCTION: 0x496a60
void FUN_00496a60()
{
    if (g_game->selected != 0x13) {
        g_game->selected = 0x13;
        FUN_004ab400((Obj_004ab400*)&g_game->sub, g_game->table[0x13]);
    }
    FUN_004c2870();
    g_game->bit2 = 0;
    g_game->bit3 = 0;
    g_game->bit0 = 0;
    FUN_00434ab0(0);
    g_game->flag4_3923b = 0;
    g_game->flag2_3923b = 0;
    ClearKeyQueue();
    g_game->mode = 2;
    g_game->handler = FUN_00496bb0;
    SetCloseHandler(LeaveNetGameCallback, 0);
}

// FUNCTION: 0x496b10
void FUN_00496b10()
{
    EnterMainMenuState();
    g_game->bit2 = 0;
    g_game->bit3 = 0;
    g_game->bit0 = 0;
    FUN_00434ab0(0);
    g_game->flag4_3923b = 0;
    g_game->flag2_3923b = 0;
    ClearKeyQueue();
    FUN_0049fa50(&g_game->sub);
    g_game->mode = 2;
    g_game->handler = FUN_00496bb0;
    SetCloseHandler(LeaveNetGameCallback, 0);
}

// Picks the next game-setup state: 4 or 5 from what FUN_00435100 returns when
// bit 2 of +0x2a44 is set, otherwise 3 for state 0x11 with that bit, or for
// state 0x10 with bit 4 of +0x2b4c and a sub-state of 0x12 or 0x13.
//
// The last two branches are written as nested ifs, each with its own copy of
// the state change; MSVC merges the copies (the 0x11 case ends in
// "je <tail>; jmp <body>"). A standalone "if (bitfield)" on an unsigned short
// bitfield compiles to "mov cl, [m]; shr cl, N; test cl, 1", while the same
// test inside an && chain folds to "test byte ptr [m], mask".
// FUNCTION: 0x496bb0
void FUN_00496bb0()
{
    RefreshUnitInfo();
    RunFrontendStateMachine();
    if (g_game->bit2 && g_game->obj_391e9->FUN_00435100() == 1) {
        FUN_004c2470();
        g_game->mode = 4;
        g_game->handler = FUN_00496db0;
        SetCloseHandler(LeaveNetGameCallback, 0);
    } else if (g_game->bit2 && g_game->obj_391e9->FUN_00435100() == 2) {
        FUN_004c2470();
        g_game->mode = 5;
        g_game->handler = LoadingScreenFrame;
        SetCloseHandler(LeaveNetGameCallback, 0);
    } else if (g_game->state_2bbe == 0x11) {
        if (g_game->bit2) {
            FUN_004c2470();
            g_game->mode = 3;
            g_game->handler = FUN_00496ce0;
            SetCloseHandler(LeaveNetGameCallback, 0);
        }
    } else if (g_game->state_2bbe == 0x10) {
        if (g_game->flags_2b4c.flag) {
            if (g_game->state_2bbf == 0x12 || g_game->state_2bbf == 0x13) {
                FUN_004c2470();
                g_game->mode = 3;
                g_game->handler = FUN_00496ce0;
                SetCloseHandler(LeaveNetGameCallback, 0);
            }
        }
    }
    FUN_004c2470();
    FUN_004ab170(&g_game->sub, 0, 0);
    FUN_004c2870();
}

// If the local player's info has bit 0 of +0x97 set, sends a one-byte
// message 8 to its id; otherwise, with bit 4 of +0x2b4c set, calls
// SendNetHeartbeat once GetTicks() passes DAT_0051f304 (then 0x3c later).
// Every path then switches to state 5 (LoadingScreenFrame).
//
// Each branch has its own copy of the state change; MSVC merges the first two
// and keeps the third, which reuses the g_game pointer still in edx. `msg`
// declared at function scope keeps its store before the pushes.
// FUNCTION: 0x496ce0
void FUN_00496ce0()
{
    char msg;
    Player_00496ce0* p = &g_game->players[g_game->localPlayer];
    if (p->info->flags & 1) {
        msg = 8;
        BroadcastPacket(p->dpid, &msg, 1);
        g_game->mode = 5;
        g_game->handler = LoadingScreenFrame;
        SetCloseHandler(LeaveNetGameCallback, 0);
    } else if (g_game->flags_2b4c.flag) {
        if (DAT_0051f304 < GetTicks()) {
            DAT_0051f304 = GetTicks() + 0x3c;
            SendNetHeartbeat();
        }
        g_game->mode = 5;
        g_game->handler = LoadingScreenFrame;
        SetCloseHandler(LeaveNetGameCallback, 0);
    } else {
        g_game->mode = 5;
        g_game->handler = LoadingScreenFrame;
        SetCloseHandler(LeaveNetGameCallback, 0);
    }
}

// FUNCTION: 0x496db0
void FUN_00496db0()
{
    g_game->state_2a3c = 2;
    FUN_00464290(0, 1);
    FUN_00464290(1, 2);
    g_game->sub_1cd5->flag_96 = 1;
    g_game->mode = 5;
    g_game->handler = LoadingScreenFrame;
    SetCloseHandler(LeaveNetGameCallback, 0);
}

// Copies a settings block into the game: the first value to +0x37ef6 and
// three flags into bits 2, 0 and 1 of the word at +0x14281.
// FUNCTION: 0x496e10
void __stdcall FUN_00496e10(Settings_00496e10* s)
{
    g_game->value_37ef6 = s->value;
    g_game->bit2_14281 = s->flag_c;
    g_game->bit0_14281 = s->flag_4;
    g_game->bit1_14281 = s->flag_8;
}

static inline int AtLeast200(int v)
{
    if (v < 200)
        v = 200;
    return v;
}

// FUNCTION: 0x496e90
void __stdcall FUN_00496e90(Struct_00496e90* obj, int height, int width)
{
    obj->flag_149 = 1;
    obj->width = (float)AtLeast200(width);
    obj->height = (float)AtLeast200(height);
}

// Sets a team up for one start position, for each of the (up to ten) teams the
// caller walks: the two bytes of the team's definition are copied into the
// player object, the two sizes in the definition are clamped to a minimum of
// 200 and stored as floats, the start position is looked up in the campaign's
// entry table (16.16 fixed point), and the team number, the player's own unit
// type and the position go to CreateUnit. When the team is the local one the
// view is centred on the position afterwards.
//
// Layout notes, for whoever reads the neighbours of this code:
// - the ten team records at g_game+0x1b63 are 0x14b bytes: +0x27 the player
//   (its +0x95 is the index into the name table, its +0x96 the second copied
//   byte), +0xdc and +0xe0 the two clamped sizes as floats, +0x149 a 1-bit
//   unsigned short bitfield (setting it gives the straight-to-memory
//   `or byte ptr [ecx+0x149], 1`; a plain unsigned char field goes through a
//   register, which moves the whole allocation).
// - the team definitions are 0x18-byte records behind the pointer at
//   g_game+0x29a0: +0x4 and +0x14 the two copied bytes, +0xc and +0x10 the two
//   sizes, +0x10 first stored to +0xdc.
// - the name table at g_game+0x37f5f is 0x232 bytes per entry and the player
//   index selects the entry, whose name is looked up by FindUnitTypeId. MSVC 5
//   gives `Entry names[8]` a size of 0x1190, not 8 * 0x232, so the field after
//   it starts at +0x390ef.
// - the position is a Vec3 of 16.16 values; the view is centred on the whole
//   part of x and z, read as the high half of each fixed-point int through a
//   `union Fixed`. x goes with the view width and z with the view height.
// FUNCTION: 0x496ee0
void __stdcall FUN_00496ee0(int team, int startpos)
{
    g_game->players[team].player->nameIndex = g_game->teams[team].nameIndex;
    g_game->players[team].player->index2 = g_game->teams[team].index2;
    Player_00496ce0* p = &g_game->players[team];
    int w = g_game->teams[team].size2;
    int v = g_game->teams[team].size1;
    p->started = 1;
    p->size1 = (float)(w >= 200 ? w : 200);
    p->size2 = (float)(v >= 200 ? v : 200);

    FixedPos_00496ee0 pos;
    if (g_game->net->GetStartPosition((Vec3_00437320*)&pos, startpos)) {
        unsigned short id = FindUnitTypeId(
            g_game->names[g_game->players[team].player->nameIndex].name);
        CreateUnit(team, id, pos, 1, 1, 0);
    } else {
        char buf[128];
        sprintf(buf,
            "Error: Could not find start position number %i on the map!",
            startpos);
        FatalError(buf);
    }

    if (team == g_game->localPlayer)
        SetCameraPosition(pos.x.h.whole - g_game->viewWidth / 2,
            pos.z.h.whole - g_game->viewHeight / 2, 0);
}

// Walks the ten player slots: for every player that is playing (the same
// check as IsPlaying in 0x40eb70), folds that slot's +0xc and +0x10 values
// into running maxima and passes them to an inlined copy of FUN_00496e90,
// which sets the player's +0x149 flag and stores max(value, 200) as floats
// at +0xdc (from the +0x10 maximum) and +0xe0 (from the +0xc maximum).
//
// The inlined copy needs the clamp spelled as a ternary,
// `width >= 200 ? width : 200` (the standalone 0x496e90 matches with it too);
// the AtLeast200 helper in FUN_00496e90 gives the two maxima more weight than
// the player offset, so the offset is spilled instead of kept in edi. The
// header only sets compiler state: without it the player address is
// [edi+edx] instead of [edx+edi] (any single header from tools/headers.py
// works).
static int IsPlaying(unsigned char i)
{
    if (i < 10) {
        Player_00496ce0* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10)
            return 1;
    }
    return 0;
}

// Inlined copy of FUN_00496e90.
static inline void __stdcall SetSize_00496e90(Player_00496ce0* obj, int height, int width)
{
    obj->flag_149 = 1;
    obj->width = (float)(width >= 200 ? width : 200);
    obj->height = (float)(height >= 200 ? height : 200);
}

// Possible original bug: the maxima are applied inside the same loop, so each
// player gets the maxima of the slots up to and including its own, not of all
// ten slots (only the last playing player sees the true maximum).
// FUNCTION: 0x497080
void FUN_00497080()
{
    int h = 0;
    int w = 0;
    for (int i = 0; i < 10; i++) {
        if (IsPlaying(i)) {
            Slot_00497080* s = &g_game->slots[i];
            if (s->field_c > h)
                h = s->field_c;
            if (s->field_10 > w)
                w = s->field_10;
            SetSize_00496e90(&g_game->players[i], h, w);
        }
    }
}
