// Decompiled by space-bunny-free. Names are provisional.
// Slot 2 (FUN_00472e30) of Class_00474cd0 (vtable 0x4fd618, see 0x474cd0.cpp),
// the fog-culled twin of Class_004750b0::FUN_00472e30 (0x475700). Every
// 32-byte record of the vector at +0xc gets its screen position computed, but
// it is only drawn when its world cell is visible to the local player: the
// per-player byte map (+0x7c, +0x80, +0x84) when bit 1 of the flag byte at
// +0x14281 is set, else that player's bit in the global short map at +0x14273.
//
// Still differs (33%): register allocation. The original keeps the record
// pointer in a stack slot biased to +0xe (it reads x, height and y as
// [eax-8], [eax-4], [eax]) and keeps a second, unbiased copy as a second
// induction variable, so ebp is free for the player pointer and x stays in
// bx. This version keeps one unbiased induction variable in ebp, which pushes
// the player pointer and x into stack slots instead. Every expression tree
// (the `&&` chain in the byte-map branch, the nested `if`s in the short-map
// branch, the argument order of the two calls) does match; the shape of the
// loop is the missing piece, not the arithmetic.
#include <stddef.h>
#include <vector>

void* __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);

#pragma pack(push, 1)
struct Player_00474cd0 {
    char unknown_0[0x7c];
    unsigned char* fogMap;             // +0x7c
    unsigned int mapWidth;             // +0x80
    unsigned int mapHeight;            // +0x84
    char unknown_88[0x14a - 0x88];
};

struct Game_00474cd0 {
    char unknown_0[0x1b63];
    Player_00474cd0 players[10];       // +0x1b63
    char unknown_2847[0x2a43 - 0x2847];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* visibilityMask;    // +0x14273, one bit per player
    char unknown_14277[0x14281 - 0x14277];
    unsigned char flags;               // +0x14281, bit 1 (mask 2)
    char unknown_14282[0x1431f - 0x14282];
    short scrollX;                     // +0x1431f
    char unknown_14321[2];
    short scrollY;                     // +0x14323
};
#pragma pack(pop)

extern Game_00474cd0* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(void*) = 0;               // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
};

// The 32-byte record, with its inlined visibility test and draw method.
struct Record_00474cd0 {
    void* data;                        // +0x00
    char unknown_4[0x6 - 0x4];
    short x;                           // +0x06
    char unknown_8[0xa - 0x8];
    short height;                      // +0x0a
    char unknown_c[0xe - 0xc];
    short y;                           // +0x0e
    char unknown_10[0x14 - 0x10];
    int field_14;                      // +0x14
    char unknown_18[0x20 - 0x18];

    int Visible()
    {
        Player_00474cd0* p = &g_game->players[g_game->playerIndex];
        if ((g_game->flags & 2) == 2) {
            int row = (y - (height >> 1)) >> 5;
            int col = x >> 5;
            return (unsigned int)col < p->mapWidth && (unsigned int)row < p->mapHeight &&
                   p->fogMap[p->mapWidth * row + col] != 0;
        }
        int col = x >> 5;
        int row = (y - (height >> 1)) >> 5;
        if ((unsigned int)col < p->mapWidth) {
            if ((unsigned int)row < p->mapHeight) {
                return (g_game->visibilityMask[p->mapWidth * row + col] &
                        (1 << g_game->playerIndex)) != 0;
            }
        }
        return 0;
    }

    void Draw(void* dest)
    {
        short sy = y - g_game->scrollY;
        sy = sy - (height >> 1) + 0x20;
        short sx = x - g_game->scrollX + 0x80;
        if (Visible()) {
            FUN_004b8500(dest, FUN_004b7f30(data, field_14), sx, sy);
        }
    }
};

struct Vec3_00474d50;

// Vtable 0x4fd618, constructor 0x474cd0, ??_G 0x474d10; 0x38 bytes.
class Class_00474cd0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_00474cd0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x38 - 0x1c];

    Class_00474cd0();
    virtual void FUN_00472d50();                        // slot 1, 0x475340
    virtual void FUN_00472e30(void*);                   // slot 2, 0x475470
    virtual int FUN_00472e70();                         // slot 3, 0x474f80
    virtual void FUN_00474df0();                        // slot 4, 0x474df0
    virtual int FUN_00475440();                         // slot 5, 0x475440
    virtual void FUN_00474d50(Vec3_00474d50* pos, int limit, int a, int b, int c,
                              int alt);                 // slot 6, 0x474d50
};

// FUNCTION: 0x475470
void Class_00474cd0::FUN_00472e30(void* dest)
{
    for (std::vector<Record_00474cd0>::iterator it = records.begin(); it != records.end(); ++it) {
        it->Draw(dest);
    }
}
