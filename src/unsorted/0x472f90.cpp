// Decompiled by Opus. Names are provisional.
// Slot 2 (FUN_00472e30) of Class_00471560 (vtable 0x4fd5b8, see 0x472200.cpp
// and 0x471cc0.cpp for the family): calls FUN_00473a00 on every 48-byte
// element of the vector at +0xc (the same object as 0x472fd0), passing the
// map scroll position as shorts.
#include <stddef.h>
#include <vector>

#pragma pack(push, 1)
struct Game_00472f90 {
    char unknown_0[0x1431f];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
};
#pragma pack(pop)

extern Game_00472f90* g_game;

class Class_00473a00 {
public:
    char unknown_0[0x30];
    void FUN_00473a00(int param_1, short x, short y);
};

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

// Vtable 0x4fd5b8, ??_G 0x471560; 0x4c bytes.
class Class_00471560 : public Class_00471cc0 {
public:
    int field_8;                                        // +0x8
    std::vector<Class_00473a00> items;                  // +0xc (_First +0x10)
    char unknown_1c[0x4c - 0x1c];

    virtual void FUN_00472d50();                        // slot 1, 0x472eb0
    virtual void FUN_00472e30(int);                     // slot 2, 0x472f90
    virtual int FUN_00472e70();                         // slot 3, 0x472fd0
    virtual void FUN_00473d50();                        // slot 4, 0x473d50
    virtual int FUN_00472f60();                         // slot 5, 0x472f60
    virtual void FUN_00473b50(void*, void*, int);       // slot 6, 0x473b50
};

// FUNCTION: 0x472f90
void Class_00471560::FUN_00472e30(int param_1)
{
    for (std::vector<Class_00473a00>::iterator it = items.begin(); it != items.end(); ++it)
        it->FUN_00473a00(param_1, g_game->scroll_x, g_game->scroll_y);
}
