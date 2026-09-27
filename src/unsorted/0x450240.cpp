// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Finds the highest field_4 among the active players of type 1 or 3, then
// looks that player up by field_4 and sets bit 0 of its info flags. The
// player lookup is inlined and its index search appears twice.
//
// PARTIAL (19.9%): the instruction sequence and struct offsets are right, but
// MSVC picks different registers here. The original keeps g_game in edi, max
// in ebp, the max-loop countdown in esi and the constant 10 in eax/al (the
// lookup's "no player" sentinel), so the max loop walks with ecx and the
// inlined search reuses esi as scratch. Our compile keeps g_game in esi, max
// in edi, the countdown in edx and the loop base in eax. The inlined search
// also loses the "cmp bl,al; je" entry guard the original has before each of
// its two loops (0x44fed0, whose helper is otherwise identical, keeps it).
//
// Notes from a second attempt (Claude Opus 5.5, #195):
// - Wrong source shape, not compiler state: with 0 to 400 unused extern
//   declarations in front, this file scores 19.9% at every N, and every
//   header set from tools/headers.py leaves it unchanged.
// - The lookup is the neighbour FUN_0044fed0 inlined: defining the real
//   FUN_0044fe40 (index search, 0x44fe40's PlayerId getter keyed on +0x73)
//   and FUN_0044fed0 (returns 0 when the index is 10, else &players[index])
//   above this function, and writing
//       Player* q = FUN_0044fed0(max); if (q) q->info->flags |= 1;
//   reproduces the whole instruction stream, entry guards included, with
//   only registers differing (it scores 10.1% to 13.8%, lower than this file,
//   because more of its registers differ). The max loop's pointer walk and
//   "mov bl, 3" also come out right.
// - What never appears is the constant 10 held in eax for the whole function
//   (mov eax, 0xa; mov esi, eax for the countdown; cmp bl, al in the
//   searches). Types of max, the id field, the getter return and both
//   parameters, index vs pointer loops, IsType(p, 3) || IsType(p, 1)
//   helpers, an inline max helper, declaration order and else-if bodies
//   all leave 10 as an immediate. Even extra uses of 10 inside the max loop
//   do not enregister it, so the missing piece is probably something else
//   in the original source that ties up the scratch registers differently.

#pragma pack(push, 1)
struct Info_00450240 {
    char unknown_0[0x97];
    unsigned char flags;                // +0x97
};

struct Player_00450240 {
    int active;                         // +0x00
    unsigned int field_4;               // +0x04
    char unknown_8[0x27 - 0x8];
    Info_00450240* info;                // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                 // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00450240 {
    char unknown_0[0x1b63];
    Player_00450240 players[10];        // +0x1b63
};
#pragma pack(pop)

extern Game_00450240* g_game;

static inline unsigned char FindPlayerIndex(int id)
{
    if (id != -1) {
        for (unsigned char i = 0; i < 10; i++) {
            int v = -1;
            if (g_game->players[i].type)
                v = g_game->players[i].field_4;
            if (v == id)
                return i;
        }
    }
    return 10;
}

// FUNCTION: 0x450240
void FUN_00450240()
{
    unsigned int max = 0;
    Player_00450240* p = g_game->players;
    int n = 10;
    do {
        if ((p->active != 0 && p->type == 3)
            || (p->active != 0 && p->type == 1)) {
            if (p->field_4 > max)
                max = p->field_4;
        }
        p++;
    } while (--n);
    if (FindPlayerIndex(max) != 10) {
        Player_00450240* q = &g_game->players[FindPlayerIndex(max)];
        if (q != 0)
            q->info->flags |= 1;
    }
}
