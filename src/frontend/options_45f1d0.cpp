// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// The key lever was the top-of-function register allocation. The original
// loads g_game into eax and copies it to edi (edi = g_game + index) after the
// *33/*165 players scale, reading playerType and rules afterwards. Two source
// details reproduce it: `opts` is computed before `rule`, and the player index
// is masked in its own statement:
//     unsigned int index = FUN_00456850();
//     index &= 0xff;
// Writing `FUN_00456850() & 0xff` in one statement, adding a `Game* game`
// local, or swapping opts/rule each flip the global allocation (g_game to
// ecx/edx) and cost 20+ points. The split mask keeps the index in ecx so the
// players scale lands in eax, which is exactly the original's preference.
// Note: Rule_0045f1d0 (24 bytes, startMetal at +0xc, startEnergy at +0x10)
// and RuleSet_0045f1d0 (startType at +0x118) are the SAME memory: the exe
// walks g_game->rules with a 24-byte stride for metal and energy but reads
// startType from element 0, i.e. it ignores playerType for that field.
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Entry_0045f1d0 {                  // 0x15b bytes
    char unknown_0[0x1b];
    int flags;                           // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    union {
        short count;                     // +0xb6 (entry 0 holds the entry count)
        char text[0x15b - 0xb6];
    } u;
};

struct Layer_0045f1d0 {
    int unknown_0;
    Entry_0045f1d0* entries;             // +0x4
    void (__stdcall* handler)(void*);    // +0x8
};

struct Opts_0045f1d0 {                   // 0x14b bytes
    char unknown_0[0x9b];
    union {
        unsigned short value;            // +0x9b
        struct {
            unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1,
                          b7 : 1, b8 : 1, b9 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1,
                          b14 : 1, b15 : 1;
        } b;
    } u;
    char unknown_9d[0xa1 - 0x9d];
    unsigned short startEnergy;          // +0xa1
    unsigned short startMetal;           // +0xa3
    char unknown_a5[0x14b - 0xa5];
};

struct PlayerEntry_0045f1d0 {            // 0x14b bytes
    Opts_0045f1d0* info;                 // +0x0
    char unknown_4[0x14b - 4];
};

struct Rule_0045f1d0 {                   // 0x18 bytes, the record g_game->rules is
    char unknown_0[0xc];                 //   walked with (stride 24)
    int startMetal;                      // +0xc
    int startEnergy;                     // +0x10
    char unknown_18[0x18 - 0x14];
};

struct RuleSet_0045f1d0 {                // +0x118 startType, read from element 0
    char unknown_0[0xc];
    int startMetal;                      // +0xc
    int startEnergy;                     // +0x10
    char unknown_14[0x118 - 0x14];
    int startType;                       // +0x118
};

struct Class_00435100 { int FUN_00435100(); };
struct Class_00435c30 { char* FUN_00435c30(); };

struct Game {
    char unknown_0[0x519];
    Layer_0045f1d0 menu;                 // +0x519
    char unknown_525[0x1b8a - 0x525];
    PlayerEntry_0045f1d0 players[10];    // +0x1b8a
    char unknown_2878[0x29a0 - 0x2878];
    RuleSet_0045f1d0* rules;              // +0x29a0
    char unknown_29a4[0x2a42 - 0x29a4];
    unsigned char playerType;            // +0x2a42
    char unknown_2a43[0x14281 - 0x2a43];
    union {
        unsigned short losFlags;         // +0x14281
        struct {
            unsigned short l0 : 1, l1 : 1, l2 : 1, l3 : 1, l4 : 1, l5 : 1, l6 : 1,
                          l7 : 1, l8 : 1, l9 : 1, l10 : 1, l11 : 1, l12 : 1, l13 : 1,
                          l14 : 1, l15 : 1;
        } lb;
    } los;
    char unknown_14283[0x37ee6 - 0x14283];
    unsigned short maxUnits;             // +0x37ee6
    char unknown_37ee8[0x37eee - 0x37ee8];
    int difficulty;                      // +0x37eee
    char unknown_37ef2[0x37ef6 - 0x37ef2];
    int commanderDeath;                  // +0x37ef6
    char unknown_37efa[0x391e9 - 0x37efa];
    Class_00435100* net;                   // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

Layer_0045f1d0* __stdcall FUN_004aa8f0(Layer_0045f1d0* menu, const char* name, int flags);
void __stdcall FUN_004288d0(const char* name, int, int, int);
int FUN_00456850();
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004ab1b0(Layer_0045f1d0* layer, char* type, char* text, int x, int y,
                            int width, int attr);
void __stdcall FUN_0045f190(void*);
void __stdcall FUN_0049fb10(Layer_0045f1d0* menu, int flag);
void __stdcall FUN_004a81e0(Layer_0045f1d0* menu, int flag);
char* __cdecl _itoa(int value, char* buf, int radix);

// FUNCTION: 0x45f1d0
void FUN_0045f1d0()
{


    Layer_0045f1d0* layer = FUN_004aa8f0(&g_game->menu, "GAMEOPTIONS.GUI", 0x1881);
    Entry_0045f1d0* entries = layer->entries;
    layer->handler = FUN_0045f190;
    FUN_004288d0("GameSettings", 0, 0, 0);
    int count = layer->entries->u.count;
    unsigned int index = FUN_00456850();
    index &= 0xff;
    char num[0x40];
    Opts_0045f1d0* opts = g_game->players[index].info;
    Rule_0045f1d0* rule = &((Rule_0045f1d0*)g_game->rules)[g_game->playerType];
    char* deathStrs[3] = { "Game Continues", "Game Ends", "Deathmatch" };
    char* locStrs[2] = { "Random", "Fixed" };
    char* mapStrs[2] = { "Mapped", "Unmapped" };
    char* losStrs[3] = { "True", "Circular", "Permanent" };
    char* cheatStrs[2] = { "Disallowed", "Allowed" };
    char* watchStrs[2] = { "Disallowed", "Allowed" };
    char* diffStrs[3] = { "Easy", "Medium", "Hard" };
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Commander Death:"), 0x12, 0x5a, 0x6e, 2);
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740(deathStrs[g_game->commanderDeath]), 0x8c,
                 0x5a, 0x78, 2);
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Starting Locations:"), 0x12, 0x6c, 0x6e, 2);
    if (g_game->net->FUN_00435100() == 2) {
        FUN_004ab1b0(layer, "TEXT", FUN_004c5740(locStrs[g_game->rules->startType]), 0x8c,
                     0x6c, 0x78, 2);
    } else {
        FUN_004ab1b0(layer, "TEXT", FUN_004c5740(locStrs[opts->u.b.b14]), 0x8c, 0x6c,
                     0x78, 2);
    }
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Mapping Mode:"), 0x12, 0x7e, 0x6e, 2);
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740(mapStrs[g_game->los.lb.l0]), 0x8c, 0x7e, 0x78, 2);
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Line of Sight:"), 0x12, 0x90, 0x6e, 2);
    unsigned short losFlags = g_game->los.losFlags;
    int losIdx = !(losFlags & 2) ? 2 : (int)(((unsigned char)~losFlags >> 2) & 1);
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740(losStrs[losIdx]), 0x8c, 0x90, 0x78, 2);
    int y;
    if (g_game->net->FUN_00435100() == 3) {
        FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Cheat Codes:"), 0x12, 0xa2, 0x6e, 2);
        FUN_004ab1b0(layer, "TEXT", FUN_004c5740(cheatStrs[opts->u.b.b13]), 0x8c,
                     0xa2, 0x78, 2);
        FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Watching:"), 0x12, 0xb4, 0x6e, 2);
        FUN_004ab1b0(layer, "TEXT", FUN_004c5740(watchStrs[opts->u.b.b7]), 0x8c, 0xb4,
                     0x78, 2);
        y = 0xc6;
    } else {
        FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Difficulty:"), 0x12, 0xa2, 0x6e, 2);
        FUN_004ab1b0(layer, "TEXT", FUN_004c5740(diffStrs[g_game->difficulty]), 0x8c, 0xa2,
                     0x78, 2);
        y = 0xb4;
    }
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Map:"), 0x12, y, 0x6e, 2);
    FUN_004ab1b0(layer, "TEXT",
                 FUN_004c5740(((Class_00435c30*)g_game->net)->FUN_00435c30()), 0x8c, y,
                 0x78, 2);
    y += 0x12;
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Starting Metal:"), 0x12, y, 0x6e, 2);
    if (g_game->net->FUN_00435100() == 3) {
        FUN_004ab1b0(layer, "TEXT", _itoa(opts->startMetal * 100, num, 10), 0x8c, y, 0x78,
                     2);
    } else {
        FUN_004ab1b0(layer, "TEXT", _itoa(rule->startMetal, num, 10), 0x8c, y, 0x78, 2);
    }
    y += 0x12;
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Starting Energy:"), 0x12, y, 0x6e, 2);
    if (g_game->net->FUN_00435100() == 3) {
        FUN_004ab1b0(layer, "TEXT", _itoa(opts->startEnergy * 100, num, 10), 0x8c, y,
                     0x78, 2);
    } else {
        FUN_004ab1b0(layer, "TEXT", _itoa(rule->startEnergy, num, 10), 0x8c, y, 0x78, 2);
    }
    y += 0x12;
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740("Max Units:"), 0x12, y, 0x6e, 2);
    FUN_004ab1b0(layer, "TEXT", _itoa(g_game->maxUnits, num, 10), 0x8c, y, 0x78, 2);
    int i;
    for (i = count + 1; i <= layer->entries->u.count; i++)
        entries[i].flags = 1;
    FUN_0049fb10(&g_game->menu, 1);
    FUN_004a81e0(&g_game->menu, 0x40);
}
