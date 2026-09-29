// Decompiled by Sonnet 5.5. Names are provisional.
// One pass of the main loop: rolls the per-frame timing buckets over, runs
// the frame pacing (0x495230) and, when it says frames are due, the
// simulation step and the three groups of profiled subsystem calls, charging
// the elapsed time to a bucket after each. Then the input/UI part, the
// mouse-capture and hotkey flag handling, the per-frame render step and the
// periodic "FRAM" marker.
//
// NOT MATCHED: 67.2%, 692 of 711 bytes. The structure and the call sequence
// follow the original (the four timer charges, the frames/paused/else
// branches, the flag word tests). What still differs: the clear of bit 2 of
// the word at +0x38d75 (and of bit 4 of +0x37ebe) is `mov cx,[m]; and ecx,
// 0xfffb; mov [m],cx` in the original but folds to `and word ptr [m], 0xfffb`
// here, whatever the field type, a local copy of the flag struct or an int
// temporary. The later blocks are shifted by that and by the register chosen
// for g_game around the timer charge in the bit 2 block.
#include <stddef.h>

class Class_004618a0 {
public:
    void FUN_004618a0(int param_1);
};

#pragma pack(push, 1)
struct Flags_38d75 {
    unsigned short pad : 2;
    unsigned short bit2 : 1;
    unsigned short rest : 13;
};

struct Timers_00496790 {
    int last;                          // +0x0
    int total;                         // +0x4
    int prev[9];                       // +0x8
    int cur[9];                        // +0x2c
};

struct Game_00496790 {
    char unknown_0[0x2a44 - 0x0];
    unsigned short bit0_2a44 : 1;
    unsigned short rest_2a44 : 15;   // +0x2a44
    char unknown_2a46[0x2bee - 0x2a46];
    unsigned short pad_2bee : 5;
    unsigned short flags_2bee : 3;
    unsigned short rest_2bee : 8;   // +0x2bee
    char unknown_2bf0[0x37e9c - 0x2bf0];
    unsigned short field_37e9c;   // +0x37e9c
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
    char unknown_37ec0[0x38a37 - 0x37ec0];
    unsigned int lastTick;   // +0x38a37
    int frames;   // +0x38a3b
    char unknown_38a3f[0x38a47 - 0x38a3f];
    unsigned int leadTick;   // +0x38a47
    char unknown_38a4b[0x38a51 - 0x38a4b];
    unsigned short paused : 1;
    unsigned short rest_38a51 : 15;   // +0x38a51
    char unknown_38a53[0x38b53 - 0x38a53];
    char text_38b53[0x100];   // +0x38b53
    int field_38c53;   // +0x38c53
    int field_38c57;   // +0x38c57
    unsigned int field_38c5b;   // +0x38c5b
    char unknown_38c5f[0x38d75 - 0x38c5f];
Flags_38d75 flags_38d75;   // +0x38d75
    char unknown_38d77[0x38d85 - 0x38d77];
    Timers_00496790 timers;   // +0x38d85
};
#pragma pack(pop)

extern Game_00496790* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;
extern int DAT_0051f300;

int FUN_004b6560();
void FUN_00495230();
void __stdcall FUN_00495490(int param_1);
void FUN_00428c00();
void FUN_00428c10();
void FUN_00428c20();
void FUN_00428c30();
void FUN_00428c40();
void FUN_00428c50();
int FUN_004568c0();
void FUN_00453d40();
unsigned int FUN_004b6340();
int FUN_0044fdb0();
void __stdcall FUN_00453320(int a, int b);
void FUN_00495e90();
void FUN_0041ce90();
void FUN_0048bae0();
void FUN_0041b2e0();
void __stdcall FUN_00468cf0(int a, int b);
void __stdcall FUN_004cb170(char* buf, char* name);

static inline void Charge(int bucket)
{
    Timers_00496790* t = &g_game->timers;
    int now = FUN_004b6560();
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
    t->last = FUN_004b6560();

    if (g_game->bit0_2a44) {
        FUN_00495230();
        if (g_game->frames != 0) {
            FUN_00495490(1);
            CHARGE(8);
            FUN_00428c00();
            FUN_00428c10();
            FUN_00428c20();
            if (g_game->flags_38d75.bit2) {
                if (FUN_004568c0())
                {
                    Flags_38d75 f = g_game->flags_38d75;
                    f.bit2 = 0;
                    g_game->flags_38d75 = f;
                }
                CHARGE(0);
            }
            FUN_00428c30();
            FUN_00428c40();
            FUN_00428c50();
        } else if (g_game->paused) {
            if (DAT_00506dbc)
                DAT_00513000.FUN_004618a0(0);
            FUN_00453d40();
            if ((int)FUN_004b6340() > DAT_0051f300) {
                DAT_0051f300 = FUN_004b6340() + 60;
                FUN_00453320(FUN_0044fdb0(), 0);
            }
            CHARGE(0);
        }
    } else if (!g_game->bit0_37ebe) {
        if (!g_game->paused) {
            FUN_00495230();
            if (g_game->frames != 0) {
                FUN_00495490(0);
                CHARGE(8);
            }
        }
    }
    if (!g_game->bit0_37ebe) {
        FUN_00495e90();
        FUN_0041ce90();
    }
    FUN_0048bae0();
    unsigned short flags = g_game->word_37ebe;
    if ((flags & 0x800) || (flags & 0x65) || g_game->flags_2bee) {
        if ((flags >> 4) & 1)
            g_game->field_37e9c = 0;
    } else {
        if ((flags >> 4) & 1) {
            g_game->bit4_37ebe = 0;
            FUN_0041b2e0();
        }
    }
    FUN_00468cf0(1, 1);
    CHARGE(8);
    FUN_00428c40();
    if (g_game->field_38c53 > 0 && g_game->field_38c5b <= g_game->leadTick) {
        FUN_004cb170(g_game->text_38b53, "FRAM");
        g_game->field_38c5b += 30 / g_game->field_38c57;
        g_game->lastTick = FUN_004b6340();
    }
    FUN_00428c50();
}
