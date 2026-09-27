// Decompiled by space-bunny-free. Names are provisional.
// Slot 2 (FUN_00472e30) of Class_00474cd0 (vtable 0x4fd618, see 0x474cd0.cpp),
// the fog-culled twin of Class_004750b0::FUN_00472e30 (0x475700). Every
// 32-byte record of the vector at +0xc gets its screen position computed, but
// it is only drawn when its world cell is visible to the local player: the
// per-player byte map (+0x7c, +0x80, +0x84) when bit 1 of the flag byte at
// +0x14281 is set, else that player's bit in the global short map at +0x14273
// (the same test as the matched 0x408090, inlined here).
//
// Still differs (42.4 percent, 387 of 398 bytes). Every expression tree
// matches: the three `short` field reads, the two screen positions, the
// `&&` chain in the byte-map branch (which is what gives the shared false
// label and `mov eax, 1` / `xor eax, eax`), the nested tests in the short-map
// branch (which is what gives `neg eax; sbb eax, eax; neg eax`), the argument
// order of the two calls, and the whole tail. What is left is one register
// allocation, and it cascades:
//
// The original runs TWO memory induction variables. Slot 1 (frame +4) is the
// unbiased `it`, and slot 0 (frame +0) is a *biased* copy at `it + 0xe`, which
// is the base of every record field access: the loop head reloads it into eax
// and reads `[eax-8]`, `[eax-4]`, `[eax]`, `[eax+6]`, and the draw block
// reloads it into eax again for `field_14`. `it` itself is only used for
// `it->data` and the loop test. The two are incremented side by side in the
// tail (`mov edx,[slot0]; mov eax,[slot1]; add ..,0x20` twice).
//
// Here the anchored base is a real variable, so MSVC keeps it in edi across
// the loop head (`lea edi, [eax+0xe]` before the pushes, then `mov dx,[edi]`,
// `mov ax,[edi-4]`, `mov bp,[edi-8]`) and only reloads it after the calls.
// That costs edi, so the allocation shifts by one: x lands in bp instead of
// bx, the player pointer is computed into ebx and spilled to frame slot 4
// instead of living in ebp, and both branches then pick different scratch
// registers (`mov ebp,[ebx+0x80]`, `add eax,[ebx+0x7c]` where the original
// has `mov ebx,[ebp+0x80]` twice, `imul ebx,eax`, `add ebx,ecx` and
// `cmp byte [ebx+ecx],0`). The frame slots follow: the original spends slot 0
// on the biased base, slot 1 on `it`, slot 4 on the col temp of branch 2 and
// slot 5 on `this`; this file has `it` in slot 0, the base in slot 1, the
// player pointer in slot 4, and the dead store of `_First` before the guard in
// slot 0 instead of slot 1.
//
// The base is anchored where the original has it (+0xe) without being written
// out as a cast, which is the one thing the source shape does reproduce: with
// no pointer local MSVC keeps the iterator in ebp and reads `[ebp+6]` etc.
// (32.6 percent), and with the loop-carried pointer declared before the loop it
// is anchored but the whole loop shape moves (32.8 percent). Tied at 42.4:
// `short* s = (short*)it + 7;` with `s[-4]`, `s[-2]`, `s[0]`, a
// `Pos_00474cd0*` local bound by reference, and the pointer to the nested
// position struct this file uses. What is needed next is a source that spends
// one more callee-saved register inside the body, or none on the base, so
// that the base falls back to its frame slot.
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
    char unknown_88[0x14b - 0x88];
};

struct Game_00474cd0 {
    char unknown_0[0x1b63];
    Player_00474cd0 players[10];       // +0x1b63
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

extern Game_00474cd0* g_game;

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

// The position sub-struct of the 32-byte record (whose draw method is
// 0x475040). Taking its address is what makes MSVC anchor the loop's field
// reads on its last member, at +0xe from the record.
struct Pos_00474cd0 {
    short x;                           // +0
    char unknown_2[2];
    short h;                           // +4
    char unknown_6[2];
    short y;                           // +8
    char unknown_a[4];
};

struct Record_00474cd0 {
    void* data;                        // +0x00
    char unknown_4[0x6 - 0x4];
    Pos_00474cd0 pos;                  // +0x06, 0xe bytes
    int field_14;                      // +0x14
    char unknown_18[0x20 - 0x18];
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
    for (std::vector<Record_00474cd0>::iterator it = records.begin(); it != records.end(); ++it) {
        Pos_00474cd0* pos = &it->pos;
        short x = pos->x;
        short height = pos->h;
        short y = pos->y;
        short sy = y - g_game->scrollY - (height >> 1) + 0x20;
        short sx = x - g_game->scrollX + 0x80;
        Player_00474cd0* p = &g_game->players[g_game->playerIndex];
        int visible;
        if ((g_game->flags & 2) == 2) {
            int row = (y - (height >> 1)) >> 5;
            int col = x >> 5;
            visible = (unsigned int)col < p->mapWidth && (unsigned int)row < p->mapHeight &&
                    p->fogMap[p->mapWidth * row + col] != 0;
        } else {
            int row = (y - (height >> 1)) >> 5;
            int col = x >> 5;
            visible = (unsigned int)col < p->mapWidth
                    ? ((unsigned int)row < p->mapHeight
                       ? (g_game->visibilityMask[p->mapWidth * row + col] &
                          (1 << g_game->playerIndex)) != 0
                       : 0)
                    : 0;
        }
        if (visible) {
            FUN_004b8500((void*)dest, FUN_004b7f30(it->data, it->field_14), sx, sy);
        }
    }
}
