// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// A method of PlayerAI, whose other methods are in ai_player.cpp.
// Symbol ids must stay small: the cut-down <vector> below and this header set
// decide the store SIB and the order of the weapon divisions.
#include <stdio.h>
#include <minmax.h>
typedef int ptrdiff_t;

// MSVC 5's <vector> (with what it uses from <new>, XMEMORY and XUTILITY),
// cut down to the members this function uses; the bodies are the header's.

inline void *__cdecl operator new(size_t, void *_P)
	{return (_P); }

namespace std {
		// TEMPLATE FUNCTION copy
template<class _II, class _OI> inline
	_OI copy(_II _F, _II _L, _OI _X)
	{for (; _F != _L; ++_X, ++_F)
		*_X = *_F;
	return (_X); }
		// TEMPLATE FUNCTION copy_backward
template<class _BI1, class _BI2> inline
	_BI2 copy_backward(_BI1 _F, _BI1 _L, _BI2 _X)
	{while (_F != _L)
		*--_X = *--_L;
	return (_X); }
		// TEMPLATE FUNCTION fill
template<class _FI, class _Ty> inline
	void fill(_FI _F, _FI _L, const _Ty& _X)
	{for (; _F != _L; ++_F)
		*_F = _X; }
		// TEMPLATE FUNCTION _Allocate
template<class _Ty> inline
	_Ty *_Allocate(ptrdiff_t _N, _Ty *)
	{if (_N < 0)
		_N = 0;
	return ((_Ty *)operator new(
		(size_t)_N * sizeof (_Ty))); }
		// TEMPLATE FUNCTION _Construct
template<class _T1, class _T2> inline
	void _Construct(_T1 *_P, const _T2& _V)
	{new ((void *)_P) _T1(_V); }
		// TEMPLATE FUNCTION _Destroy
template<class _Ty> inline
	void _Destroy(_Ty *_P)
	{(_P)->~_Ty(); }
inline void _Destroy(char *_P)
	{}
inline void _Destroy(wchar_t *_P)
	{}
		// TEMPLATE CLASS allocator
template<class _Ty>
	class allocator {
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Ty *pointer;
	typedef const _Ty *const_pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;
	typedef _Ty value_type;
	pointer allocate(size_type _N, const void *)
		{return (_Allocate((difference_type)_N, (pointer)0)); }
	void deallocate(void *_P, size_type)
		{operator delete(_P); }
	void construct(pointer _P, const _Ty& _V)
		{_Construct(_P, _V); }
	void destroy(pointer _P)
		{_Destroy(_P); }
	};
		// TEMPLATE CLASS vector
template<class _Ty, class _A = allocator<_Ty> >
	class vector {
public:
	typedef _A::size_type size_type;
	typedef _A::pointer _Tptr;
	typedef _A::const_pointer _Ctptr;
	typedef _A::reference reference;
	typedef _Tptr iterator;
	typedef _Ctptr const_iterator;
	iterator begin()
		{return (_First); }
	iterator end()
		{return (_Last); }
	void resize(size_type _N, const _Ty& _X = _Ty())
		{if (size() < _N)
			insert(end(), _N - size(), _X);
		else if (_N < size())
			erase(begin() + _N, end()); }
	size_type size() const
		{return (_First == 0 ? 0 : _Last - _First); }
	reference operator[](size_type _P)
		{return (*(begin() + _P)); }
	void insert(iterator _P, size_type _M, const _Ty& _X)
		{if (_End - _Last < _M)
			{size_type _N = size() + (_M < size() ? size() : _M);
			iterator _S = allocator.allocate(_N, (void *)0);
			iterator _Q = _Ucopy(_First, _P, _S);
			_Ufill(_Q, _M, _X);
			_Ucopy(_P, _Last, _Q + _M);
			_Destroy(_First, _Last);
			allocator.deallocate(_First, _End - _First);
			_End = _S + _N;
			_Last = _S + size() + _M;
			_First = _S; }
		else if (_Last - _P < _M)
			{_Ucopy(_P, _Last, _P + _M);
			_Ufill(_Last, _M - (_Last - _P), _X);
			fill(_P, _Last, _X);
			_Last += _M; }
		else if (0 < _M)
			{_Ucopy(_Last - _M, _Last, _Last);
			copy_backward(_P, _Last - _M, _Last);
			fill(_P, _P + _M, _X);
			_Last += _M; }}
	iterator erase(iterator _F, iterator _L)
		{iterator _S = copy(_L, end(), _F);
		_Destroy(_S, end());
		_Last = _S;
		return (_F); }
protected:
	void _Destroy(iterator _F, iterator _L)
		{for (; _F != _L; ++_F)
			allocator.destroy(_F); }
	iterator _Ucopy(const_iterator _F, const_iterator _L,
		iterator _P)
		{for (; _F != _L; ++_P, ++_F)
			allocator.construct(_P, *_F);
		return (_P); }
	void _Ufill(iterator _F, size_type _N, const _Ty &_X)
		{for (; 0 < _N; --_N, ++_F)
			allocator.construct(_F, _X); }
	_A allocator;
	iterator _First, _Last, _End;
	};
}
struct Unit {
    int unknown_0;
};

struct Elem_0040cfb0 {
    char a;
    char b;
    char c;
};

struct Elem_0040d4f0 {
    char value;
};

struct Elem_0040d550 {
    int unknown_0;
};

struct Point16 {
    short x;
    short y;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

#pragma pack(push, 1)
struct Weapon_00409730 {
    char unknown_0[0xd4];
    unsigned short field_d4;           // +0xd4
    char unknown_d6[0xdc - 0xd6];
    int field_dc;                      // +0xdc
    char unknown_e0[0x10a - 0xe0];
    char field_10a;                    // +0x10a
};

struct Flags241_00409730 {
    unsigned int bits_0 : 6;
    unsigned int flag_6 : 1;           // bit 6
    unsigned int bits_7 : 4;
    unsigned int flag_11 : 1;          // bit 11
    unsigned int bits_12 : 12;
    unsigned int flag_24 : 1;          // bit 24
    unsigned int bits_25 : 7;
};

struct Def_00409730 {
    int HasField1ce() { return field_1ce != 0.0f; }
    char unknown_0[0x186];
    float field_186;                   // +0x186
    float field_18a;                   // +0x18a
    char unknown_18e[0x1c0 - 0x18e];
    short field_1c0;                   // +0x1c0
    float field_1c2;                   // +0x1c2
    char unknown_1c6[0x1ce - 0x1c6];
    float field_1ce;                   // +0x1ce
    float field_1d2;                   // +0x1d2
    char unknown_1d6[0x1ee - 0x1d6];
    Weapon_00409730* weapons[3];       // +0x1ee
    char unknown_1fa[0x204 - 0x1fa];
    short field_204;                   // +0x204
    short field_206;                   // +0x206
    char unknown_208[0x22d - 0x208];
    char field_22d;                    // +0x22d
    char unknown_22e[0x241 - 0x22e];
    Flags241_00409730 flags_241;       // +0x241
    unsigned int bits_245_0 : 4;
    unsigned int flag_245_4 : 1;       // bit 4
    unsigned int bits_245_5 : 3;
    unsigned int flag_245_8 : 1;       // bit 8
    unsigned int bits_245_9 : 23;
};

struct Game {
    char unknown_0[0x1425f];
    int field_1425f;                   // +0x1425f
    char unknown_14263[0x1434f - 0x14263];
    unsigned short field_1434f;        // +0x1434f
    char unknown_14351[0x1438f - 0x14351];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_00409730* defs;                // +0x1439b
    char unknown_1439f[0x37ec8 - 0x1439f];
    int field_37ec8;                   // +0x37ec8
};

struct Player_00409730 {
    char unknown_0[0x144];
    unsigned short field_144;          // +0x144
};

struct UnitList_00409730 {
    std::vector<Unit*> units;
};

class PlayerAI {
public:
    Player_00409730* player;           // +0x00
    unsigned char index;               // +0x04
    UnitList_00409730 list_5;          // +0x05
    UnitList_00409730 list_15;         // +0x15
    UnitList_00409730 list_25;         // +0x25
    int pos_35[3];                     // +0x35
    int pos_41[3];                     // +0x41
    std::vector<Elem_0040cc40> vec_4d; // +0x4d
    short centerX;                     // +0x5d
    short centerY;                     // +0x5f
    int field_61;                      // +0x61
    std::vector<Elem_0040cfb0> vec_65; // +0x65
    int field_75;                      // +0x75
    int field_79;                      // +0x79
    std::vector<short> vec_7d;  // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<unsigned char> vec_9d; // +0x9d

    void ComputeBaseWeights();
};
#pragma pack(pop)

extern Game* g_game;

float __stdcall GetEnergyUse(Def_00409730* def);

// 0x409520 and 0x4095d0 (matched in their own files): no callers in the
// exe, both inlined into the loop below.
#define MIN(a, b) (((a) > (b)) ? (b) : (a))

int __stdcall RateWeapons(Def_00409730* p)
{
    int result = 1;
    if (p->flag_245_4)
        result = 0xb;
    Weapon_00409730** pp = p->weapons;
    for (int i = 3; i != 0; i--) {
        Weapon_00409730* s = *pp;
        if (s->field_10a != 0)
            result = result + s->field_d4 / 40 + s->field_dc / 100 + 5;
        pp++;
    }
    if (MIN(result, 100) < -100)
        return -100;
    return MIN(result, 100);
}

int __stdcall RateUnitType(Def_00409730* p)
{
    int result = 1;
    if (p->field_1ce != 0.0f)
        result = 0xb;
    if (p->field_22d != 0)
        result += 10;
    if (GetEnergyUse(p) < 0.0f)
        result += 10;
    result = (int)((int)(result - p->field_18a * -0.01f) - p->field_186 * -0.002f);
    result += (signed char)RateWeapons(p);
    if (MIN(result, 100) < -100)
        return -100;
    return MIN(result, 100);
}

// FUNCTION: 0x409730
void PlayerAI::ComputeBaseWeights()
{
    vec_8d.resize(g_game->count, 0);
    {
        Elem_0040cfb0 e;
        e.a = 0;
        e.b = 0;
        e.c = 0;
        vec_65.resize(g_game->count, e);
    }
    for (int i = 1; i < g_game->count; i++) {
        Def_00409730* def = &g_game->defs[(unsigned short)i];
        vec_8d[i] = RateUnitType(def);

        int a = 1;
        Elem_0040cfb0* e = &vec_65[i];
        int n = (short)vec_7d[i];
        if (def->flag_245_4)
            a = 21;
        if (def->flags_241.flag_6 && n < 3)
            a += 30;
        if (GetEnergyUse(def) < 0.0f)
            a += 50;
        // Stays a method: the first resize's erase needs its inline budget.
        if (def->HasField1ce())
            a += 50;
        if (def->field_22d)
            a += 25;
        Flags241_00409730 flags = def->flags_241;
        if (flags.flag_11)
            a += 40;
        if (def->field_206)
            a += 15;
        if (def->field_204)
            a += 5;
        int x = (int)(a + min(max(def->field_1c2, 0.0f), 30.0f));
        if (n == 0)
            x *= 4;
        if (n == 1)
            x *= 2;
        if (def->field_1c0 >= 0)
            x *= 3;
        if (player->field_144 > (unsigned short)(g_game->field_1434f / 2)) {
            x += (char)vec_8d[i] / 2;
        }
        if (def->flag_245_8)
            x = 0;
        if (flags.flag_24)
            x = 0;
        if (def->field_1d2 != 0.0f && g_game->field_1425f < g_game->field_37ec8 / 2)
            x = 0;
        x = min(x, 100);
        e->a = x;
        e->c = (char)max(0.0f, min(100.0f, def->field_186 * -0.0025f - GetEnergyUse(def) * 5.0f));
        // The e->b bonus is a plain conditional: RateWeapons only inlines then.
        e->b = (char)max(0.0f, min(100.0f, (float)(def->field_18a * -0.02f) + (def->field_22d ? 25 : 0) + (def->field_1ce != 0.0f ? 100 : 0)));
    }
}



