// Decompiled by space-bunny-free. Names are provisional.
// Partial: 78.4% (396 bytes against the original 386), not MATCH.
// Structure recovered: it clamps the unit index at +0xa8 into the local
// player's unit range (two unsigned shorts at player +0x6f and +0x71) and
// then walks the whole cyclic range, skipping the starting index, in the
// direction given by the second argument, returning the first unit whose
// bit 4 of the flags at +0x110 is set.
// What still differs, at all five "index 0 means no unit" tests (the four
// GetUnit call sites and the unit argument test at the top), the original
// jumps over the address computation, so the "xor eax, eax" is the
// fall-through:
//   test dx,dx / jne <compute> / xor eax,eax / jmp <use> / <compute> / <use>
// this version branches to the null block instead:
//   test dx,dx / je <null> / <compute> / jmp <use> / <null>: xor eax,eax
// Inverting the source polarity (if (i == 0) u = 0; else u = units + i, or
// (i == 0) ? 0 : units + i) puts the blocks the right way round, but then the
// two downward loops emit "add edx, 0xffff" where the original has "dec edx",
// so neither polarity matches. The index variables must be unsigned short
// (the original compares them with 16 bit cmp/test) while the unit pointer
// needs the 32 bit variable, which is what produces "and ecx, 0xffff".

#pragma pack(push, 1)
struct Unit_0048c190 {
    char unknown_0[0xa8];
    unsigned short index;               // +0xa8
    char unknown_aa[0x110 - 0xaa];
    union {
        unsigned int flags;             // +0x110
        struct {
            unsigned int unknown_0 : 4;
            unsigned int selected : 1;
            unsigned int unknown_5 : 27;
        };
    };
    char unknown_114[0x118 - 0x114];
};

struct Player_0048c190 {                // 0x14b bytes
    char unknown_0[0x6f];
    unsigned short firstIndex;           // +0x6f
    unsigned short lastIndex;            // +0x71
    char unknown_73[0x14b - 0x73];
};

struct Game_0048c190 {
    char unknown_0[0x1b63];
    Player_0048c190 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;               // +0x2a42
    char unknown_2a43[0x14357 - 0x2a43];
    Unit_0048c190* units;               // +0x14357
};
#pragma pack(pop)

extern Game_0048c190* g_game;

static inline Unit_0048c190* GetUnit_0048c190(unsigned short i)
{
    return i ? g_game->units + i : 0;
}

// FUNCTION: 0x48c190
Unit_0048c190* __stdcall FUN_0048c190(Unit_0048c190* unit, int dir)
{
    Player_0048c190* p = &g_game->players[g_game->player];
    unsigned short x = unit ? unit->index : 0;
    if (x < p->firstIndex || x > p->lastIndex) {
        x = p->firstIndex;
    }
    unsigned short j;
    if (dir) {
        j = x;
        while (j != p->firstIndex) {
            j--;
            Unit_0048c190* v = GetUnit_0048c190(j);
            if (v->selected) {
                return v;
            }
        }
        j = p->lastIndex + 1;
        while (j != x) {
            j--;
            Unit_0048c190* v = GetUnit_0048c190(j);
            if (v->selected) {
                return v;
            }
        }
    } else {
        j = x;
        while (j != p->lastIndex) {
            Unit_0048c190* v = GetUnit_0048c190(j + 1);
            if (v->selected) {
                return v;
            }
            j++;
        }
        j = p->firstIndex - 1;
        while (j != x) {
            Unit_0048c190* v = GetUnit_0048c190(j + 1);
            if (v->selected) {
                return v;
            }
            j++;
        }
    }
    return 0;
}
