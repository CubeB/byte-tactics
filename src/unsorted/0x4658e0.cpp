// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 87.8% (best).  Including <memory.h> instead of <string.h> fixed the
// two "folded data pointer" diffs in both IsExplored sites and the IsSeen
// register order all at once (82.2 -> 87.8); windows.h/stdio/stdlib are all
// worse, headers.py found <memory.h> best.  What still differs, 2 bytes:
//   1. The flags local: the original loads the full word, does `and ax,2` and
//      stores `mov word [esp+0x24],ax`; every spelling tried (`unsigned short
//      flag = g_game->flags;` then `flag & 2`, `short`/`unsigned short local =
//      flags & 2`, `&=`, casts) gives either `and eax,2` + a dword store, or
//      narrows the load to `mov al, byte [..]`.  The 16-bit AND plus 16-bit
//      store is worth exactly the missing 2 bytes.
//   2. `test edx,edx` (original) vs `test dl,dl` (ours): `char vis` truncates
//      before the test; `int vis` fixes the test but loses the whole register
//      assignment (76.1%), so the char spelling is kept.
// PARTIAL (82.2%, best of many scratch scorings; see notes below).
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
// What still differs from the original (about 17% of the instruction lines):
//  - The flags test: the original's local is a 16-bit value, so it does
//    `and ax,2` and spills `mov word [esp+0x24],ax`; ours does `and eax,2`
//    and `mov dword [esp+0x24],eax` even though the load is the original's
//    `mov ax,word [ecx+0x14281]` (reading the whole word is required; every
//    `g_game->flags & 2` form that masks immediately narrows the load to a
//    byte).  Every declared type tried (short, unsigned short, chained `&=`,
//    a second `unsigned short` intermediate) still widens the and/spill.
//  - The explored byte map: ours folds the data pointer into the index add
//    (`add ebp,[eax+0x7c]`); the original materialises it (`add ebp,edx;
//    mov edx,[eax+0x7c]`) in both arms.  `Get`/direct indexing/`At(index)` all
//    fold here; declaring the data pointer as a helper local is much worse.
//  - The mask arm materialises the short cell in dx (`xor edx,edx;
//    mov dx,word [ecx+ebp*2]`) where the original uses cx and then
//    `mov edx,ecx`, so the mask pointer lands in ecx there and edx here.
//  - The first visibility result is tested with `test dl,dl` (vis is char)
//    where the original tests `test edx,edx`; `bool`/`int vis` fixes that test
//    but loses the whole register assignment (69% and 64%).
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
    unsigned short flag = g_game->flags;

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

