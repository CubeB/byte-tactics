// Decompiled by space-bunny-free and Claude Opus 5.5, verified by GPT-6.1-sol. Names are provisional.
// Finished by DeepSeek V4.1 Flash (MATCH, 398 of 398 bytes).
// Slot 2 (FUN_00472e30) of SmokeParticles (vtable 0x4fd618, see 0x474cd0.cpp),
// the fog-culled twin of Class_004750b0::FUN_00472e30 (0x475700). Every
// 32-byte record of the vector at +0xc is drawn through an inlined record
// method that computes the screen position first and only draws when the
// record's world cell is visible to the local player: the player's explored
// byte map (+0x7c data, +0x80 width, +0x84 height) when bit 1 of the flag byte
// at +0x14281 is set, else that player's bit in the global short map at
// +0x14273 (the matched 0x408090, inlined here). The same explored-map inline
// appears in 0x407e90 (0x407f74), 0x465ac0 (0x465b6a) and 0x473a00.
//
// What fixed it (DeepSeek V4.1 Flash). The last two spots, the folded
// `add ebx, [ebp+0x7c]` in the explored arm and the folded
// `imul eax, [ebp+0x80]` in the mask arm, are decided by the allocator, and
// the compiler state can be shifted with declarations (that is why the two
// arms never folded or materialised together in any earlier attempt). Two
// source changes move the mask arm and then the explored arm to the original:
//   1. `ByteMap::Index(x, y) { return size.width * y + x; }`, used by `Get`
//      and by the mask arm through a ByteMap pointer local declared after the
//      Contains test. The extra virtual register makes MSVC materialise the
//      width (`mov ebp, [ebp+0x80]; imul ebp, eax`) instead of folding it, and
//      the same register pressure then makes the explored arm load the data
//      pointer into a register (`add ebx, ecx; mov ecx, [ebp+0x7c]`).
//   2. `Get` spelled `data[Index(x, y)]` rather than `data[size.width * y + x]`.
// The earlier measurements that led here: the twin 0x475700 loop shape, the
// sx-before-sy order and the ByteMap::Get method all stand; `tools/headers.py`
// and every earlier declaration/local sweep did not reach past 89.9.
#include <stddef.h>
#include <vector>

void* __stdcall GetGafFrame(void* a, int b);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);

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

    int Index(int x, int y) { return size.width * y + x; }
    unsigned char Get(int x, int y) { return data[Index(x, y)]; }
};

#pragma pack(push, 1)
struct Player_00475470 {
    char unknown_0[0x7c];
    ByteMap_00475470 explored;         // +0x7c
    char unknown_88[0x14b - 0x88];
};

struct Game {
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

extern Game* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class ParticleSystem {
public:
    int field_4;                                        // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
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
    ByteMap_00475470* b = &map->explored;
    return (g_game->visibilityMask[b->Index(tx, ty)] &
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
            DrawFrameBlended(dest, GetGafFrame(data, field_14), sx, sy);
    }
};

struct Vec3_00474d50;

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class SmokeParticles : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Record_00475470> records;               // +0xc (_First +0x10)
    char unknown_1c[0x38 - 0x1c];

    SmokeParticles();
    virtual void Update();                              // slot 1, 0x475340
    virtual void FUN_00472e30(int);                     // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void Emit();                                // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// FUNCTION: 0x475470
void SmokeParticles::FUN_00472e30(int dest)
{
    for (std::vector<Record_00475470>::iterator it = records.begin(); it != records.end();
         ++it) {
        it->DrawIfVisible((void*)dest, g_game->scrollX, g_game->scrollY);
    }
}
