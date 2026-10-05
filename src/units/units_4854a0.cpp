// Decompiled by DeepSeek V4.1 Flash, GPT-6, GPT-6.1-sol, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// MATCH. This translation unit stands in for <algorithm> with __stdcall
// instantiations, because the original was built with /Gz and its sort helpers
// are callee-clean (0x488810, 0x488920, 0x488960). The three helper templates
// carry their real global names (FUN_00488810/FUN_00488920/FUN_00488960) so
// the recursive and out-of-line calls at 0x485691/0x48574b/0x48576d/0x485778
// reference the names data/symbols.csv already has, while the body and
// one-level inline stay exactly as the std:: originals. The std headers are
// included for their __cdecl helper templates (copy_backward) and type traits.
// The tail free-list loop is written with the item fields themselves (no
// `slot` local): reading `item->0x67` back for the start and `item->0x6b` for
// the end leaves the compiler's edx/esi roles exactly as the original (slot in
// esi, zero-extended unitsPerPlayer in edx). A named `slot` local makes it put
// slot in edx, unitsPerPlayer in esi and hoist `xor esi, esi`, 94.3%.

#include <windows.h>
#include <iterator>
#include <xutility>

#define _ALGORITHM_

template<class _RI, class _Ty, class _Pr> void __stdcall FUN_00488810(_RI _F, _RI _L, _Pr _P, _Ty *);
template<class _RI, class _Ty, class _Pr> _RI __stdcall FUN_00488960(_RI _F, _RI _L, _Ty _Piv, _Pr _P);
template<class _RI, class _Ty, class _Pr> void __stdcall FUN_00488920(_RI _L, _Ty _V, _Pr _P);

namespace std {
const int _CHUNK_SIZE = 7;
const int _SORT_MAX = 16;

template<class _Ty, class _Pr> inline _Ty __stdcall _Median(_Ty _X, _Ty _Y, _Ty _Z, _Pr _P)
    {if (_P(_X, _Y))
        return (_P(_Y, _Z) ? _Y : _P(_X, _Z) ? _Z : _X);
    else
        return (_P(_X, _Z) ? _X : _P(_Y, _Z) ? _Z : _Y); }

template<class _FI1, class _FI2> inline void __stdcall iter_swap(_FI1 _X, _FI2 _Y)
    {_Iter_swap(_X, _Y, _Val_type(_X)); }
template<class _FI1, class _FI2, class _Ty> inline void __stdcall _Iter_swap(_FI1 _X, _FI2 _Y, _Ty *)
    {_Ty _Tmp = *_X;
    *_X = *_Y, *_Y = _Tmp; }

template<class _RI, class _Pr> inline void __stdcall sort(_RI _F, _RI _L, _Pr _P)
    {_Sort_0(_F, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline void __stdcall _Sort_0(_RI _F, _RI _L, _Pr _P, _Ty *)
    {if (_L - _F <= _SORT_MAX)
        _Insertion_sort(_F, _L, _P);
    else
        {FUN_00488810(_F, _L, _P, (_Ty *)0);
        _Insertion_sort(_F, _F + _SORT_MAX, _P);
        for (_F += _SORT_MAX; _F != _L; ++_F)
            FUN_00488920(_F, _Ty(*_F), _P); }}
template<class _RI, class _Pr> inline void __stdcall _Insertion_sort(_RI _F, _RI _L, _Pr _P)
    {_Insertion_sort_1(_F, _L, _P, _Val_type(_F)); }
template<class _RI, class _Ty, class _Pr> inline void __stdcall _Insertion_sort_1(_RI _F, _RI _L, _Pr _P, _Ty *)
    {if (_F != _L)
        for (_RI _M = _F; ++_M != _L; )
            {_Ty _V = *_M;
            if (!_P(_V, *_F))
                FUN_00488920(_M, _V, _P);
            else
                {copy_backward(_F, _M, _M + 1);
                *_F = _V; }}}
}

template<class _RI, class _Ty, class _Pr> void __stdcall FUN_00488920(_RI _L, _Ty _V, _Pr _P)
    {for (_RI _M = _L; _P(_V, *--_M); _L = _M)
        *_L = *_M;
    *_L = _V; }

template<class _RI, class _Ty, class _Pr> _RI __stdcall FUN_00488960(_RI _F, _RI _L, _Ty _Piv, _Pr _P)
    {for (; ; ++_F)
        {for (; _P(*_F, _Piv); ++_F)
            ;
        for (; _P(_Piv, *--_L); )
            ;
        if (_L <= _F)
            return (_F);
        std::iter_swap(_F, _L); }}

template<class _RI, class _Ty, class _Pr> void __stdcall FUN_00488810(_RI _F, _RI _L, _Pr _P, _Ty *)
    {for (; std::_SORT_MAX < _L - _F; )
        {_RI _M = FUN_00488960(_F, _L, std::_Median(_Ty(*_F),
            _Ty(*(_F + (_L - _F) / 2)), _Ty(*(_L - 1)), _P), _P);
        if (_L - _M <= _M - _F)
            FUN_00488810(_M, _L, _P, std::_Val_type(_F)), _L = _M;
        else
            FUN_00488810(_F, _M, _P, std::_Val_type(_F)), _F = _M; }}

#include <algorithm>

class Class_00435100 {
public:
    int FUN_00435100();
};

struct Player_004854a0 {
    char unknown_0[4];
    unsigned int key;                   // +0x4
    char unknown_8[0x146 - 8];
    unsigned char field_146;            // +0x146
    char unknown_147[0x14b - 0x147];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1b63];
    unsigned char players[10 * 0x14b];  // +0x1b63, stride 0x14b
    char unknown_2851[0x1434f - 0x2851];
    unsigned short field_1434f;         // +0x1434f
    unsigned short poolCount;           // +0x14351
    char unknown_14353[0x14357 - 0x14353];
    unsigned char* pool;                // +0x14357
    unsigned char* field_1435b;         // +0x1435b
    void* hotUnits;                     // +0x1435f
    void* hotRadar;                     // +0x14363
    char unknown_14367[0x1436f - 0x14367];
    unsigned short field_1436f;         // +0x1436f
    char unknown_14371[0x14373 - 0x14371];
    unsigned int field_14373;           // +0x14373
    char unknown_14377[0x1439b - 0x14377];
    unsigned int field_1439b;           // +0x1439b
    char unknown_1439f[0x37ee6 - 0x1439f];
    unsigned short unitsPerPlayer;      // +0x37ee6
    char unknown_37ee8[0x391e9 - 0x37ee8];
    Class_00435100* mode;               // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

int __stdcall ComparePlayers(Player_004854a0* a, Player_004854a0* b)
{
    if (g_game->mode->FUN_00435100() == 3)
        return a->key < b->key;
    return a < b;
}

// FUNCTION: 0x4854a0
void __stdcall AllocateUnitMemory(void)
{
    g_game->field_1436f = 0;
    g_game->field_14373 &= 0xfffffffd;
    g_game->field_1434f = g_game->unitsPerPlayer;
    g_game->poolCount = (unsigned short)(g_game->unitsPerPlayer * 10 + 1);

    unsigned char* pool = g_game->pool = (unsigned char*)FUN_004d83b0("UNIT MEMORY", g_game->poolCount * 0x118);
    memset(pool, 0, g_game->poolCount * 0x118);

    unsigned int ten = g_game->unitsPerPlayer * 10;
    g_game->hotUnits = FUN_004d83b0("HOT UNITS", ten * 2);
    g_game->hotRadar = FUN_004d83b0("HOT RADAR UNITS", ten * 10);
    g_game->field_1435b = g_game->pool + g_game->poolCount * 0x118 - 0x118;

    unsigned short n;
    for (n = 0; n < g_game->poolCount; n++) {
        *(unsigned short*)(pool + n * 0x118 + 0xa8) = n;
        *(unsigned int*)(pool + n * 0x118 + 0x92) = g_game->field_1439b;
    }

    Player_004854a0* v[10];
    int k;
    for (k = 0; k < 10; k++)
        v[k] = (Player_004854a0*)(g_game->players + k * 0x14b);

    std::sort(v, v + 10, ComparePlayers);

    pool[0xff] = 0xff;
    *(unsigned int*)(pool + 0x96) = 0;
    int i;
    for (i = 0; i < 10; i++) {
        Player_004854a0* item = v[i];
        int c = g_game->unitsPerPlayer * i + 1;
        *(unsigned char**)((char*)item + 0x67) = pool + c * 0x118;
        *(unsigned char**)((char*)item + 0x6b) =
            *(unsigned char**)((char*)item + 0x67) + g_game->unitsPerPlayer * 0x118 - 0x118;
        *(unsigned short*)((char*)item + 0x6f) =
            *(unsigned short*)(*(unsigned char**)((char*)item + 0x67) + 0xa8);
        *(unsigned short*)((char*)item + 0x71) =
            *(unsigned short*)(*(unsigned char**)((char*)item + 0x6b) + 0xa8);
        for (unsigned char* q = *(unsigned char**)((char*)item + 0x67);
             q <= *(unsigned char**)((char*)item + 0x6b); q += 0x118) {
            *(void**)(q + 0x96) = item;
            q[0xff] = *(unsigned char*)((char*)item + 0x146);
            *(unsigned int*)(q + 0xac) = 0xffffffffu;
        }
    }
}
