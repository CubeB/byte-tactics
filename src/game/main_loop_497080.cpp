// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Walks the ten player slots: for every player that is playing (the same
// check as IsPlaying in 0x40eb70), folds that slot's +0xc and +0x10 values
// into running maxima and passes them to an inlined copy of FUN_00496e90,
// which sets the player's +0x149 flag and stores max(value, 200) as floats
// at +0xdc (from the +0x10 maximum) and +0xe0 (from the +0xc maximum).
//
// The inlined copy needs the clamp spelled as a ternary,
// `width >= 200 ? width : 200` (the standalone 0x496e90 matches with it too);
// the AtLeast200 helper in 0x496e90.cpp gives the two maxima more weight than
// the player offset, so the offset is spilled instead of kept in edi. The
// header only sets compiler state: without it the player address is
// [edi+edx] instead of [edx+edi] (any single header from tools/headers.py
// works).

#include <windows.h>

#pragma pack(push, 1)
struct Player_00497080 {               // 0x14b bytes
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    char type;                         // +0x73
    char unknown_74[0xdc - 0x74];
    float width;                       // +0xdc
    float height;                      // +0xe0
    char unknown_e4[0x146 - 0xe4];
    char field_146;                    // +0x146
    char unknown_147[0x149 - 0x147];
    unsigned short flag_149 : 1;       // +0x149
    unsigned short rest_149 : 15;
};

struct Slot_00497080 {                 // 0x18 bytes
    char unknown_0[0xc];
    int field_c;                       // +0xc
    int field_10;                      // +0x10
    char unknown_14[4];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00497080 players[10];       // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Slot_00497080* slots;              // +0x29a0
};
#pragma pack(pop)

extern Game* g_game;

static int IsPlaying(unsigned char i)
{
    if (i < 10) {
        Player_00497080* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10)
            return 1;
    }
    return 0;
}

// Inlined copy of FUN_00496e90.
static inline void __stdcall SetSize_00496e90(Player_00497080* obj, int height, int width)
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
