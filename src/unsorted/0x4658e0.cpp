// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// GPT-6.1-sol refinement: five checks kept 96.5%. Three raw-word flag-load
// variants tied or scored lower; the best source was restored. No MATCH.
// Retry #1781: GPT-6.1-sol independently confirmed 96.5%; no MATCH. The word flag load/store and EDX versus DL test still differ.
// deepseek-v4.1 retry: the two remaining hunks are (1) original loads the flags word
// (`mov ax, word [ecx+0x14281]; and ax,2; cmp ax,2; mov word [esp+0x24],ax`) while
// every source shape tried here compiles to a sliced byte load plus 32-bit AND and a
// dword spill (`mov al, byte [ecx+0x14281]; and eax,2; cmp ax,2; mov dword [esp+0x24],eax`),
// and (2) the original tests the first result 32-bit (`test edx,edx`) where the `char vis`
// local forces `test dl,dl` (an `int vis` local fixes the test but moves the map into esi
// and drops the score to 75.5%). Shapes tried for the flags: `unsigned short flag = g_game->flags & 2;`,
// `short flag = ...`, `flag = g_game->flags; flag &= 2;` (both types, compound and plain), an
// inline helper whose mask is a parameter, `unsigned short flags : 16` and 1-bit bitfield Game
// structs, and `(unsigned short)(g_game->flags & 2)`; a standalone probe file (build/scratch/0x4658e0/probe*.cpp)
// shows MSVC5 always slices the load, so the original's 16-bit form is not reachable from
// these spellings. The direct `if (IsExplored(...)) return 1;` / `else if (IsSeen(...))` block
// that would give the edx test instead compiles to a different prologue and scores 51.9%.
// deepseek-v4.1 (retry 2): 24 more shapes, all 87.2% or lower except the retained 96.5%.
// New facts: (a) the original's wording of the FIRST test must be `flag == 2` (the masked
// value compared to 2), not `(flag & 2) == 2`: that alone fixes the instruction schedule
// (the third memset dword lands after push esi at [esp+0x14]) and gives the original's
// second test `cmp word ptr [esp+0x24],2` from memory, but it reverts the load to
// `mov al, byte [...]` and measures 87.2%. (b) The only spelling that produces the
// original's 16-bit load `mov ax, word [ecx+0x14281]` is keeping the RAW word in the
// local (`unsigned short flag = g_game->flags;` with `(flag & 2) == 2` at both tests):
// 87.8%, but the mask then compiles to `and eax,2` and the spill to `mov dword
// [esp+0x24],eax`, so that variant is 2 bytes short. (c) `int vis` / `short vis` /
// `unsigned short vis` / a ternary in the if-condition all change the frame (the first two
// move the map to esi and shift every argument slot by 4; a ternary to the if-condition
// gives `mov al` plus a 0x10 frame), so `char vis` is what pins the frame and the `dl`
// test is the price. (d) A probe of 8 scalar shapes (unsigned short/short, `&=`,
// casts, two-step init) shows MSVC5 always slices the load to a byte when the mask is in
// the initializer, so the original's `mov ax / and ax,2 / mov word` triple needs a
// construct this session could not find; treat the 16-bit AND plus 16-bit spill as the
// remaining unknown, not the test width.
// GPT-6.1-sol lead pass (#1510): comparing the masked flag directly (`flag == 2`) scored 87.2%; retained the 96.5% best.
// PARTIAL: 96.5% (best, verified with check.py). A small mask helper raised
// similarity from 87.8%, though the inlined helper now makes the compiler
// narrow the global load and apply the mask twice. Remaining differences:
// the original loads a word, masks AX, and spills AX, while this version adds
// a stack store, loads AL, masks EAX twice, and spills EAX; the original also
// tests EDX where this version tests DL. Tried three further variants without
// improvement and stopped within the assigned attempt budget.
// Is point (x, y) or point (x+dx, y+dy) visible to the local player?  The
// 12-byte Position local (6 shorts, x/y/z among them) is zeroed with an
// inlined memset and then filled from the arguments; the compiler promotes the
// fields to registers but keeps the three dead zero dword stores, which is the
// only source spelling that reproduces the original prologue.  When bit 1 of
// the game flags word at g_game+0x14281 is set the player's explored byte map
// at +0x7c (width +0x80, height +0x84) is used, otherwise the shared
// visibility bit mask at +0x14273 with this player's bit (g_game+0x2a43).
// The `Map_004658e0* m = map;` local and `char vis` (a 1-byte result) are what
// finally put g_game in ecx, the map in eax, pos.x in di, pos.y in bx and
// h = size>>1 in esi, matching the original's whole register assignment.
#include <memory.h>
#pragma pack(push, 1)
struct MapSize_004658e0 {
    unsigned int width;
    unsigned int height;
    int Contains(unsigned int tx, unsigned int ty) { return tx < width && ty < height; }
};
struct ByteMap_004658e0 {
    unsigned char* data;
    MapSize_004658e0 size;
    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};
struct Map_004658e0 {
    char unknown_0[0x7c];
    ByteMap_004658e0 explored;
};
struct Game_004658e0 {
    char unknown_0[0x2a43];
    unsigned char playerIndex;
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;
    char unknown_14277[0x14281 - 0x14277];
    unsigned short flags;
};
#pragma pack(pop)
extern Game_004658e0* g_game;
struct Pos_004658e0 {
    short xFrac;
    short x;
    short yFrac;
    short y;
    short zFrac;
    short z;
};
static inline unsigned short MaskFlags_004658e0(unsigned short flags) { return (unsigned short)(flags & 2); }
static inline int IsExplored(Map_004658e0* map, Pos_004658e0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty) && map->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}
static inline int IsSeen(Map_004658e0* map, Pos_004658e0* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (!map->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->visibilityMask[map->explored.size.width * ty + tx] &
            (1 << g_game->playerIndex)) != 0;
}
// FUNCTION: 0x4658e0
int __stdcall FUN_004658e0(Map_004658e0* map, int x, int y, int dx, int dy, short size)
{
    Map_004658e0* m = map;
    char vis;
    Pos_004658e0 pos;
    memset(&pos, 0, sizeof(pos));
    pos.x = (short)(x << 4);
    pos.y = size;
    pos.z = (short)(y << 4);
    unsigned short flag = MaskFlags_004658e0(g_game->flags);

    if ((flag & 2) == 2)
        vis = IsExplored(m, &pos);
    else
        vis = IsSeen(m, &pos);
    if (vis)
        return 1;
    pos.x = (short)(pos.x + (dx << 4));
    pos.z = (short)(pos.z + (dy << 4));
    if ((flag & 2) == 2)
        return IsExplored(m, &pos);
    return IsSeen(m, &pos);
}
