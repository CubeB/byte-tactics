// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1-flash, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Appends `count` 25-byte records to the global registration table (a
// std::vector at 0x512340) and sorts the whole table by name. The record has a
// per-kind notify callback at +4 (0x43bad0 calls it) and its name at +0x15.
#include <vector>
#include <algorithm>
#include <iterator>
#include <string.h>

class Class_0043a1f0;

#pragma pack(push, 1)
struct Elem_0043c390 {
    char unknown_0[4];
    int (__stdcall* notify)(void* unit, Class_0043a1f0* obj, int code); // +4
    char unknown_8[0x15 - 0x8];
    char* name; // +0x15
};
#pragma pack(pop)

typedef std::vector<Elem_0043c390> Vec_0043c390;
typedef int(__stdcall* Pred_0043c390)(const Elem_0043c390&, const Elem_0043c390&);

// File-static: internal linkage keeps the vector bounds in registers across the sort.
static Vec_0043c390 DAT_00512340;

int __stdcall CompareOrderTypeNames(const Elem_0043c390& a, const Elem_0043c390& b);

// std::_Unguarded_insert
inline void __stdcall FUN_0043c940(Elem_0043c390* _L, Elem_0043c390 _V, Pred_0043c390 _P)
{
    for (Elem_0043c390* _M = _L; _P(_V, *--_M); _L = _M)
        *_L = *_M;
    *_L = _V;
}

// std::_Insertion_sort_1
inline void __stdcall FUN_0043c990(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P,
                                   Elem_0043c390*)
{
    if (_F != _L)
        for (Elem_0043c390* _M = _F; ++_M != _L; ) {
            Elem_0043c390 _V = *_M;
            if (!_P(_V, *_F))
                FUN_0043c940(_M, _V, _P);
            else {
                std::copy_backward(_F, _M, _M + 1);
                *_F = _V;
            }
        }
}

// std::_Insertion_sort
inline void _Insertion_sort_0043bc90(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P)
{
    FUN_0043c990(_F, _L, _P, std::_Val_type(_F));
}

// std::_Median
inline Elem_0043c390 __stdcall FUN_0043ca70(Elem_0043c390 _X, Elem_0043c390 _Y, Elem_0043c390 _Z,
                                            Pred_0043c390 _P)
{
    if (_P(_X, _Y))
        return (_P(_Y, _Z) ? _Y : _P(_X, _Z) ? _Z : _X);
    else
        return (_P(_X, _Z) ? _X : _P(_Y, _Z) ? _Z : _Y);
}

// std::_Unguarded_partition
inline Elem_0043c390* __stdcall FUN_0043cb20(Elem_0043c390* _F, Elem_0043c390* _L,
                                             Elem_0043c390 _Piv, Pred_0043c390 _P)
{
    for (; ; ++_F) {
        for (; _P(*_F, _Piv); ++_F)
            ;
        for (; _P(_Piv, *--_L); )
            ;
        if (_L <= _F)
            return (_F);
        std::iter_swap(_F, _L);
    }
}

// std::_Sort
inline void __stdcall FUN_0043c720(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P,
                                   Elem_0043c390*)
{
    for (; std::_SORT_MAX < _L - _F; ) {
        Elem_0043c390* _M = FUN_0043cb20(_F, _L, FUN_0043ca70(Elem_0043c390(*_F),
            Elem_0043c390(*(_F + (_L - _F) / 2)), Elem_0043c390(*(_L - 1)), _P), _P);
        if (_L - _M <= _M - _F)
            FUN_0043c720(_M, _L, _P, std::_Val_type(_F)), _L = _M;
        else
            FUN_0043c720(_F, _M, _P, std::_Val_type(_F)), _F = _M;
    }
}

// std::_Sort_0
inline void _Sort_0_0043bc90(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P,
                             Elem_0043c390*)
{
    if (_L - _F <= std::_SORT_MAX)
        _Insertion_sort_0043bc90(_F, _L, _P);
    else {
        FUN_0043c720(_F, _L, _P, (Elem_0043c390*)0);
        _Insertion_sort_0043bc90(_F, _F + std::_SORT_MAX, _P);
        for (_F += std::_SORT_MAX; _F != _L; ++_F)
            FUN_0043c940(_F, Elem_0043c390(*_F), _P);
    }
}

// std::sort with a predicate
inline void sort_0043bc90(Elem_0043c390* _F, Elem_0043c390* _L, Pred_0043c390 _P)
{
    _Sort_0_0043bc90(_F, _L, _P, std::_Val_type(_F));
}

// 0x43bad0, the function before this one in the exe (matched in its own file).
#pragma pack(push, 1)

class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char kind;             // +4, index into DAT_00512340
    unsigned char count;            // +5
    unsigned int flags_6;           // +6, bit 0 set while the node is waiting
    unsigned int wakeFrame;         // +0xa, frame the node becomes due at
    void* unit;                     // +0xe, passed to the callback
    char unknown_12[0x42 - 0x12];
    unsigned int flags;             // +0x42, bit 0x40000 picks the second list
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;           // +0x4a

    ~Class_0043a1f0();
};

struct Parent_0043bad0 {
    char unknown_0[0x5c];
    Class_0043a1f0* first;          // +0x5c
    Class_0043a1f0* firstTop;       // +0x60
};

struct Game {
    char unknown_0[0x38a47];
    unsigned int frame;             // +0x38a47
};

#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int n);

static void RemoveAndDelete(Parent_0043bad0* p, Class_0043a1f0* child)
{
    Class_0043a1f0* first = p->first;
    Class_0043a1f0** link = (child->flags & 0x40000) ? &p->firstTop : &p->first;
    Class_0043a1f0* node = *link;
    while (node != 0) {
        if (node == child) {
            *link = child->next;
            if (child != first)
                child->flags |= 0x10000;
            delete child;
            break;
        }
        link = &node->next;
        node = *link;
    }
}

void __stdcall FUN_0043bad0(Parent_0043bad0* p)
{
    Class_0043a1f0* child = p->firstTop;
    while (child != 0) {
        if (child->flags_6 == 0 || g_game->frame >= child->wakeFrame) {
            child->flags_6 = 0;
            switch (DAT_00512340[child->kind].notify(child->unit, child, 0)) {
            case 3: {
                unsigned int when = RandomInt(0xf) + 0x1e;
                child->flags_6 |= 1;
                child->wakeFrame = g_game->frame + when;
                break;
            }
            case 1:
                child->count++;
                break;
            case 0:
                child->count = 0;
                break;
            case 2:
            case 4:
                break;
            case 5:
            case 8:
            case 9:
                RemoveAndDelete(p, child);
                break;
            case 6:
            case 7:
                RemoveAndDelete(p, child);
                return;
            default:
                RemoveAndDelete(p, child);
                break;
            }
            child = p->firstTop;
        } else {
            child = child->next;
            continue;
        }
    }
}

// FUNCTION: 0x43bc90
void __stdcall RegisterOrderTypes(Elem_0043c390* from, int count)
{
    // Own statement: `size() + count` in one expression swaps the lea operands.
    int sz = DAT_00512340.size();
    DAT_00512340.reserve(sz + count);
    std::copy(from, from + count, std::back_inserter(DAT_00512340));
    sort_0043bc90(DAT_00512340.begin(), DAT_00512340.end(), CompareOrderTypeNames);
}

// The rest of the translation unit (each matched in its own file).
int __stdcall CompareOrderTypeNames(const Elem_0043c390& a, const Elem_0043c390& b)
{
    return _strcmpi(a.name, b.name) < 0 ? 1 : 0;
}

extern Elem_0043c390 DAT_004fd288[];
void RegisterGroundOrders();
void FUN_00415b20();
void RegisterAICommands();
void RegisterUnitOrders();

void RegisterAllOrderTypes()
{
    RegisterOrderTypes(DAT_004fd288, 1);
    RegisterGroundOrders();
    FUN_00415b20();
    RegisterAICommands();
    RegisterUnitOrders();
}

void FUN_0043c350()
{
    DAT_00512340.clear();
}
