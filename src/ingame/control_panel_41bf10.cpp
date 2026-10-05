// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Stats_0041bf10 {
    char unknown_0[0x22e];
    unsigned char field_22e;           // +0x22e
};

struct Unit {
    char unknown_0[0x92];
    Stats_0041bf10* stats;             // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                    // +0xa6
    char unknown_a8[0x110 - 0xa8];
    unsigned int field_110;            // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short unitIndex;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char field_37ebe;         // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall PlaySoundByName(char* name, int param_2);

// The 3-bit page field (bits 23-25) is set from stats->field_22e, or stepped
// down with wraparound when it is already past the first page.
static inline unsigned int SetPage(Unit* u, unsigned int f)
{
    return (((u->stats->field_22e + 0x1ff) << 23) ^ f) & 0x3800000 ^ f;
}

static inline unsigned int StepPage(unsigned int f)
{
    return (((f & 0xff800000) - 1) ^ f) & 0x3800000 ^ f;
}

// FUNCTION: 0x41bf10
void __stdcall StepBuildMenuPageBack(int param_1)
{
    unsigned short index = g_game->unitIndex;
    Unit* u;
    if (index == 0)
        goto nextbuild;
    u = &g_game->units[index];
    if (u->field_a6 == 0)
        u = 0;
    if (u == 0)
        goto nextbuild;

    // Each branch declares its own f: a single shared local made MSVC put the
    // field in eax and the unit in ecx, swapping every later operand.
    if (param_1) {
        unsigned int f = u->field_110;
        if ((f & 0x400000) == 0) {
            f |= 0x400000;
            u->field_110 = f;
            u->field_110 = SetPage(u, f);
        } else if ((f & 0x3800000) == 0x800000) {
            u->field_110 = f & 0xffbfffff;
        } else {
            u->field_110 = StepPage(f);
        }
    } else {
        unsigned int f = u->field_110;
        if ((f & 0x3800000) < 0x1000000)
            u->field_110 = SetPage(u, f);
        else
            u->field_110 = StepPage(f);
        u->field_110 |= 0x400000;
    }
    g_game->field_37ebe |= 0x10;

nextbuild:
    PlaySoundByName("nextbuildmenu", 0);
}
