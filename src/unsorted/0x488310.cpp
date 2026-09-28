// Decompiled by space-bunny-free, finished by mimo-v2.6-flash. Names are provisional.
// Builds the unit list of a recorded game (g_game->net->list) in two passes:
// one entry per list slot, then the per-unit "extra" strings through
// FUN_00487bf0, which walks the same list again and looks units up in the
// vector passed as its third argument.
// The 0x80-byte frame is [the constructor's dead _Al slot][its _V temporary]
// [the player byte][the vector][the sprintf buffer], so the vector has to be
// 16 bytes with the empty allocator at +0: that is what FUN_00487bf0 and
// 0x487af0 read as Table_00488310's +0 and +4.
// Two apparently cosmetic things decide the byte match, so do not tidy them
// away: the mask on e->flags is 0xffffff80, not 0x80, because MSVC 5 narrows a
// 0xFFFFFFxx mask to `and al` and so keeps the xor eax,eax / mov al
// zero-extension (0x80 gives a 5-byte dword and), and <math.h> is included
// because dropping it moves the f108 multiply onto the other register.
#include <stdio.h>
#include <math.h>
#include <memory>

struct Unit;

// std::vector<Unit*> is declared here rather than included because MSVC 5
// inlines _Ufill's body into its (count, value) constructor, while the
// original calls the out-of-line copy (0x406c40) that the game's own vector
// object holds. Everything else is the real container: allocator::allocate
// gives the `if (_N < 0) _N = 0` clamp and operator new (not new[]), and
// ~vector's deallocate is the operator delete at the end.
namespace std {
template <class T, class A = allocator<T> > class vector {
public:
    typedef T value_type;
    typedef A::size_type size_type;

    vector(size_type _N, const T& _V = T(), const A& _Al = A())
        : allocator(_Al)
    {
        _Myfirst = allocator.allocate(_N, (void *)0);
        _Ufill(_Myfirst, _N, _V);
        _Mylast = _Myfirst + _N;
        _Myend = _Mylast;
    }
    ~vector()
        { allocator.deallocate(_Myfirst, _Myend - _Myfirst); }
    T& operator[](size_type _P)
        { return (*(_Myfirst + _P)); }

protected:
    A allocator;                       // +0x0
    T* _Myfirst;                       // +0x4
    T* _Mylast;                        // +0x8
    T* _Myend;                         // +0xc
    void _Ufill(T* _F, size_type _N, const T& _X);
};
}

#pragma pack(push, 1)
struct Pos_00488310 {                // 12 bytes, by value
    int x;
    int y;
    int z;
};

struct Item_00488310 {                // 0x249 bytes
    char unknown_0[0x21e];
    unsigned short id;                // +0x21e
    char unknown_220[0x249 - 0x220];
};

struct Player_00488310 {              // 0x14b bytes
    int active;                       // +0x0
    char unknown_4[0x73 - 4];
    unsigned char type;               // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char f146;               // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Entry_00488310 {               // 0x24 bytes
    char* name;                       // +0x0
    char* unknown_4;                  // +0x4
    char* extra;                      // +0x8
    Pos_00488310 pos;                 // +0xc
    short f18;                        // +0x18
    short f1a;                        // +0x1a
    char unknown_1c[0x22 - 0x1c];
    unsigned char player;             // +0x22
    unsigned char flags;              // +0x23
};

class Class_00435100 {
public:
    char unknown_0[0xdac];
    Entry_00488310* list;             // +0xdac
    int count;                        // +0xdb0
};

class Class_004904b0 {
public:
    char unknown_0[0x88];
    int field_88;

    void FUN_004904b0();
};

struct Def_00488310 {
    char unknown_0[0x1fa];
    int f1fa;                         // +0x1fa
};

struct Unit {
    char unknown_0[0x66];
    short f66;                        // +0x66
    char unknown_68[0x92 - 0x68];
    Def_00488310* def;                // +0x92
    char unknown_96[0x108 - 0x96];
    short f108;                       // +0x108
    char unknown_10a[0x110 - 0x10a];
    int flags;                        // +0x110
};

struct Game_00488310 {
    char unknown_0[0x1b63];
    Player_00488310 players[10];      // +0x1b63
    char unknown_2851[0x391e9 - 0x2851];
    Class_00435100* net;              // +0x391e9
    Class_004904b0* mission;          // +0x391ed
};

// FUN_00487bf0's third argument is the container itself: +0 is the empty
// allocator and +4 is _First.
struct Table_00488310 {
    char allocator_byte;
    char pad[3];
    Unit** ids;                       // +0x4
};
#pragma pack(pop)

extern Game_00488310* g_game;

Item_00488310* __stdcall FUN_00488a50(const char* name);
void __stdcall FUN_0047ddc0(Item_00488310* type, Pos_00488310* pos);
Unit* __stdcall FUN_00485f50(unsigned char player, unsigned short id,
                                      Pos_00488310 pos, int a, int b, int c);
void __stdcall FUN_00487bf0(Unit* unit, char* text, Table_00488310* table);
void __stdcall FUN_004b6290(char* message);

// FUNCTION: 0x488310
void __cdecl FUN_00488310()
{
    Player_00488310* p;
    char buf[100];
    int n = g_game->net->count;        // only the constructor takes it; the
    std::vector<Unit*> units(n);       // loops re-read net->count themselves
    for (int i = 0; i < g_game->net->count; i++) {
        Entry_00488310* e = &g_game->net->list[i];
        Item_00488310* item = FUN_00488a50(e->name);
        if (item == 0) {
            units[i] = 0;
            continue;
        }
        unsigned char pl = (unsigned char)(e->player - 1);
        if (pl >= 10
            || (p = &g_game->players[pl], g_game->players[pl].active == 0)
            || (p->type != 1 && p->type != 2 && p->type != 3)
            || p->f146 == 10) {
            sprintf(buf, "Player number %d invalid for unit %s", e->player, e->name);
            FUN_004b6290(buf);
        }
        FUN_0047ddc0(item, &e->pos);
        Unit* u = FUN_00485f50((unsigned char)(e->player - 1), item->id, e->pos, 1, 1, 0);
        if (u) {
            u->flags = (u->flags & ~0x8000) | ((e->flags & 0xffffff80) << 8);
            u->f108 = (unsigned short)((unsigned)(u->def->f1fa * e->f1a) / 100);
            u->f66 = e->f18;
            units[i] = u;
        }
    }
    for (int j = 0; j < g_game->net->count; j++) {
        Entry_00488310* e = &g_game->net->list[j];
        if (e->extra && units[j])
            FUN_00487bf0(units[j], e->extra, (Table_00488310*)&units);
    }
    if (g_game->net->count <= 0)
        g_game->mission->FUN_004904b0();
}
