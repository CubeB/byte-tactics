// Decompiled by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
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
//
// Notes from a third attempt (deepseek-v4.1-flash, #1332):
// - Rebuilt the inlined lookup exactly like the matched 0x44fed0 helper
//   (GetPlayerField_00450240 with the explicit "i != 10" guard, then
//   FindPlayerIndex, then an inlined GetPlayer). That version is 302 bytes
//   (original is 305) and the whole instruction stream, guards and all,
//   lines up; only the registers differ, and it scores 10.1% because every
//   register operand then mismatches. tools/headers.py tried all 128 sets,
//   all stay at 10.1%.
// - The linchpin is the constant 10: the original occupies eax with it for
//   the whole function, which forces the max-loop countdown into a
//   callee-saved register (esi), which in turn forces max into ebp (edi holds
//   g_game). When 10 stays an immediate, edx is free for the countdown and
//   eax for the loop base, which is exactly our wrong allocation.
//
// Notes from a fourth attempt (deepseek-v4.1-flash, #1683):
// - The real structural gap is the "cmp bl, al; je" entry guard at the top of
//   each of the two inlined searches. It is the "i != 10" short-circuit of the
//   getter, and it only survives when the getter is a separate static inline
//   function (GetPlayerField taking the byte index), exactly as in the matched
//   0x44fe40 / 0x44fed0 / 0x450380. Writing the getter body directly in
//   FindPlayerIndex lets MSVC prove i < 10 and delete the guard (that is this
//   file's shape: it scores 19.9 but can never match).
// - Adding the guard through the nested getter is byte-structurally right
//   (the 0x44fed0 loop shape appears, guards included) but scores 10.2% here
//   and 302 bytes vs 305. MSVC then also changes the allocation: the search
//   index moves from bl to dl, the search scratch pointer moves from ebp to
//   ebx, and ebp stops being pushed at all (frame becomes 4 pushes + push ecx
//   instead of 5, so the local sits at [esp+0xc] rather than [esp+0x10]).
// - With the guard present our allocation is g_game=esi, max=edi, n=edx,
//   p=eax, scratch=ebx; the original is g_game=edi, max=ebp, n=esi, p=ecx,
//   scratch=esi, const 10=eax. That is the 3-cycle esi -> edi -> ebp -> esi
//   plus p and n moving. Tried and measured at a flat 10.2% (score.sh over
//   build/scratch/0x450240): declaration orders (max,n,p permutations), the
//   0x457b90 for-loop-with-per-iteration-pointer form, an unsigned char loop
//   limit passed to the getter and the search, and the count as the search
//   bound. None move the constant 10 out of the immediate or move g_game to
//   edi. This is the allocator wall the guide already records for 0x450240.
// - Best kept here is the guard-less 19.9% body; the correct-structure body is
//   build/scratch/0x450240/v1.cpp (10.2%). Next attempt should try to move
//   g_game into edi while keeping the nested getter.
//
// Notes from a fifth attempt (deepseek-v4.1, #2057):
// - 22 more source shapes were compiled and eyeballed in the prologue search
//   (build/scratch/0x450240/variants*.py, batch1..3.txt): index-for max loop,
//   pointer-range loop, for/while countdowns, byte counters, a max scan as an
//   inline helper taking the count, an inline helper returning the count 10,
//   sizeof-based count, type constant in a byte variable, search helpers with
//   byte and int count parameters, hand-written searches, and the 0x44fed0 /
//   0x450380 tail shapes. None emits "mov eax, 0xa" plus "mov esi, eax"; all
//   keep mov edx, 0xa and immediate "cmp bl, 0xa". So the max loop and the
//   lookup text are not what moves 10 into eax; some value that produces 10
//   (not a folded literal) is the missing piece.
// - MSVC 5 does hoist byte constants out of loops when they are compared
//   against a memory byte: the sibling 0x4464d0 loop
//   (g_game->players[i].active != 0; g_game->players[i].state == 3) emits
//   "mov bl, 0x3" outside the loop and "cmp BYTE PTR [eax+esi*1+0x1bd6], bl"
//   inside it, and our 19.9% body reproduces that hoist for the type 3 test.
//   The original 0x450240 hoists the sentinel 10 the same way, which needs a
//   use of 10 as a value, not as a compare immediate.
// - The 19.9% body is byte-structurally closest (5 pushes, frame 0x14, the
//   byte index at [esp+0x10]); all the 0x44fed0-shaped tails drop the ebp push
//   (frame 0x10) and shift that slot, which is why they sit at 10.1%.

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
