// Decompiled by Opus, class family declarations copied from 0x471cc0.cpp.
// Names are provisional.
// Slot 2 of Class_004750b0 (vtable 0x4fd638; the family is listed in
// 0x471cc0.cpp): draws every 32-byte record of the vector at +0xc relative to
// the game's scroll position, with the record's draw method (0x475040)
// inlined. <windows.h> is needed (found with tools/headers.py): without it
// MSVC subtracts the scroll y before the half height.
#include <windows.h>
#include <stddef.h>
#include <vector>

void* __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);

#pragma pack(push, 1)
struct Game_00475700 {
    char unknown_0[0x1431f];
    short field_1431f;                 // +0x1431f
    char unknown_14321[2];
    short field_14323;                 // +0x14323
};
#pragma pack(pop)

extern Game_00475700* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
};

// The 32-byte record; its draw method is 0x475040.
struct Record_004750b0 {
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

    void Draw(void* dest, short px, short py)
    {
        short sy = y - (height >> 1) - py + 0x20;
        short sx = x - px + 0x80;
        FUN_004b8500(dest, FUN_004b7f30(data, field_14), sx, sy);
    }
};

struct Vec3_00475150;

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class Class_004750b0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_004750b0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x34 - 0x1c];

    Class_004750b0();
    virtual void FUN_00472d50();                        // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void FUN_004751c0();                        // slot 4, 0x4751c0
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* pos, int a, int b, int c); // slot 6, 0x475150
};

// FUNCTION: 0x475700
void Class_004750b0::FUN_00472e30(int dest)
{
    for (std::vector<Record_004750b0>::iterator it = records.begin(); it != records.end(); ++it) {
        it->Draw((void*)dest, g_game->field_1431f, g_game->field_14323);
    }
}
