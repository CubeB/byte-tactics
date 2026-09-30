// Decompiled by space-bunny-free and Claude Opus 5.5, verified by GPT-6.1-sol. Names are provisional.
// Slot 2 (FUN_00472e30) of Class_00474cd0 (vtable 0x4fd618, see 0x474cd0.cpp),
// the fog-culled twin of Class_004750b0::FUN_00472e30 (0x475700). Every
// 32-byte record of the vector at +0xc is drawn through an inlined record
// method that computes the screen position first and only draws when the
// record's world cell is visible to the local player: the player's explored
// byte map (+0x7c data, +0x80 width, +0x84 height) when bit 1 of the flag byte
// at +0x14281 is set, else that player's bit in the global short map at
// +0x14273 (the matched 0x408090, inlined here). The same explored-map inline
// appears in 0x407e90 (0x407f74), 0x465ac0 (0x465b6a) and 0x473a00.
//
// Still differs (84.6 percent, 394 of 398 bytes). Writing the loop as a call
// of the inline record method DrawIfVisible, with IsVisible(player, &pos)
// split into the two inline tests, is what gives the +0xe-biased induction variable in its own
// stack slot and the original's frame (42.4 to 64.5 percent); computing sx
// before sy gives the original's load order (x, height, z) and keeps x in bx
// (78.2); reading the byte map through the ByteMap::Get method keeps
// `width * y + x` as an index and loads the width into its own register.
// Two spots are left:
//
// 1. The short-map branch multiplies into ty's register:
//      ours:  imul eax, [ebp+0x80]; add eax, ebx; mov ebx, [esi+0x14273];
//             xor ebp, ebp; mov bp, [ebx+eax*2]
//      orig:  mov ebp, [ebp+0x80]; imul ebp, eax; mov eax, [esi+0x14273];
//             add ebp, ebx; xor ebx, ebx; mov bx, [eax+ebp*2]; mov eax, ebx
//    in every compiler state tried. Tried without effect: either operand
//    order and casts in the product, an index local, MapSize::Index and
//    ByteMap::Index methods, a SeenCell(map, x, y) helper, Player member
//    functions, a mask row pointer. A MapSize* parameter (37.6) and an
//    `if (Contains) return ...;` form (71.5) are worse. The matched 0x408090
//    has the original's form on its own, but stops matching (72.5, the same
//    fold) with `#include <vector>` in front of it, so this is compiler
//    state there; here no state unfolds it.
// 2. Compiler state: unused declarations (extern ints, prototypes, structs,
//    typedefs, enums or inline functions) cycle through four outcomes with a
//    period of about 520: 84.6 (N = 0 to 7, the end of that window: the
//    byte-map lookup folds the data pointer, `add ebx, [ebp+0x7c]` for the
//    original's `add ebx, ecx; mov ecx, [ebp+0x7c]`), 78.2 (N = 8 to 200: the byte-map
//    lookup is exact, but the scratch registers of the call block rotate by
//    one and the loop tail differs, probably the one temporary that spot 1
//    is missing), 75.0 and 80.5. tools/headers.py finds nothing higher.
// DEEPSEEK-V4.1-FLASH screened three more variants (scratch only, no new
// file runs): <memory.h> swaps this 84.6 basin for the 78.2 one (the byte-map
// data pointer is materialised, but the call block and loop tail rotate), so
// the folded-data-pointer diff and the call block are the two sides of one
// switch; writing the two arms straight into DrawIfVisible (the 0x4745e0
// shape) collapses the frame to 375 bytes and 37.4; adding a `w`/`m` local
// pair to IsSeen is byte-identical to this version. 84.6 stands as the best.
// GPT-6.1-sol rechecked this version (84.6) and tested a local alias for the
// byte-map data pointer; the generated code and score were unchanged.
#include <stddef.h>
#include <vector>

void* __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);

struct Position_00475470 {             // 16.16 fixed point; only high words read
    short xFrac;
    short x;                           // +0x2
    short yFrac;
    short y;                           // +0x6
    short zFrac;
    short z;                           // +0xa
};

struct MapSize_00475470 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00475470 {
    unsigned char* data;               // +0x0
    MapSize_00475470 size;             // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

#pragma pack(push, 1)
struct Player_00475470 {
    char unknown_0[0x7c];
    ByteMap_00475470 explored;         // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct Game_00475470 {
    char unknown_0[0x1b63];
    Player_00475470 players[10];       // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;               // +0x14281
    char unknown_14282[0x1431f - 0x14282];
    short scrollX;                     // +0x1431f
    char unknown_14321[2];
    short scrollY;                     // +0x14323
};
#pragma pack(pop)

extern Game_00475470* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
};

static inline int IsExplored(Player_00475470* map, Position_00475470* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (map->explored.size.Contains(tx, ty) && map->explored.Get(tx, ty))
        return 1;
    return 0;
}

// The body of the matched 0x408090.
static inline int IsSeen(Player_00475470* map, Position_00475470* pos)
{
    int tx = pos->x >> 5;
    int ty = (pos->z - (pos->y >> 1)) >> 5;
    if (!map->explored.size.Contains(tx, ty)) {
        return 0;
    }
    return (g_game->visibilityMask[map->explored.size.width * ty + tx] &
            (1 << g_game->playerIndex)) != 0;
}

static inline int IsVisible(Player_00475470* map, Position_00475470* pos)
{
    if ((g_game->flags & 2) == 2)
        return IsExplored(map, pos);
    return IsSeen(map, pos);
}

// The 32-byte record; its unculled draw method is 0x475040.
struct Record_00475470 {
    void* data;                        // +0x00
    Position_00475470 pos;             // +0x04
    char unknown_10[0x14 - 0x10];
    int field_14;                      // +0x14
    char unknown_18[0x20 - 0x18];

    void DrawIfVisible(void* dest, short px, short py)
    {
        short sx = pos.x - px + 0x80;
        short sy = pos.z - (pos.y >> 1) - py + 0x20;
        if (IsVisible(&g_game->players[g_game->playerIndex], &pos))
            FUN_004b8500(dest, FUN_004b7f30(data, field_14), sx, sy);
    }
};

struct Vec3_00474d50;

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class Class_00474cd0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_00475470> records;               // +0xc (_First +0x10)
    char unknown_1c[0x38 - 0x1c];

    Class_00474cd0();
    virtual void FUN_00472d50();                        // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void FUN_00474df0();                        // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// FUNCTION: 0x475470
void Class_00474cd0::FUN_00472e30(int dest)
{
    for (std::vector<Record_00475470>::iterator it = records.begin(); it != records.end();
         ++it) {
        it->DrawIfVisible((void*)dest, g_game->scrollX, g_game->scrollY);
    }
}
