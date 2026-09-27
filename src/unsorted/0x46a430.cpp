// Decompiled by Space Bunny Free. Names are provisional.
// Hit point bar drawn under a unit: a 34x4 box in the frame colour, then a
// bar 32 pixels wide at full health, green above two thirds of maxHealth,
// yellow above one third and red below.

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x1fa];
    unsigned int maxHealth;          // +0x1fa
};

struct Unit {
    char unknown_0[0x92];
    UnitDef* def;                    // +0x92
    char unknown_96[0x108 - 0x96];
    short health;                    // +0x108
};

struct Game {
    char unknown_0[0xdcb];
    unsigned char colors[16];        // +0xdcb
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004bf6f0(void* surface, Rect_004b0510* rect, int color);

// FUNCTION: 0x46a430
void __stdcall FUN_0046a430(void* surface, Unit* unit, int x, int y)
{
    if (unit->health <= 0)
        return;
    unsigned char* colors = g_game->colors;
    Rect_004b0510 r;
    r.x1 = x - 17;
    r.y1 = y - 2;
    r.x2 = x + 17;
    r.y2 = y + 2;
    FUN_004bf6f0(surface, &r, colors[0]);
    // The width has to be a separate local: written inline, MSVC tail merges
    // the three colour calls below into one.
    int width;
    r.x1++;
    r.y1++;
    r.y2--;
    width = unit->health * 32 / unit->def->maxHealth;
    r.x2 = r.x1 + width;
    if (unit->health > (int)(unit->def->maxHealth / 3) * 2)
        FUN_004bf6f0(surface, &r, colors[10]);
    else if (unit->health > (int)(unit->def->maxHealth / 3))
        FUN_004bf6f0(surface, &r, colors[14]);
    else
        FUN_004bf6f0(surface, &r, colors[12]);
}
