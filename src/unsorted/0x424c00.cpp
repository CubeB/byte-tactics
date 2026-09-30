// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, edited by deepseek-v4.1. Names are provisional.
// Partial: 69.1%, 1514 bytes versus 1496. Names allocation keeps its
// result in EDI and rematerializes zero inside the fill loop; the original
// uses EAX as the fill cursor and reloads EDI afterward. Feature-list
// destruction and subsequent record-loop registers also differ. Twelve
// allocator ABI variants and twelve native vector fill variants did not
// improve it. The specialized remap members retain the original inline split.
// Rechecked by deepseek-v4.1-flash: call census is EXACT (22 calls, all 26
// references agree) and the frame sub esp,0xc0 already matches. Residual is
// the allocator's choice of the shared zero: the original resets ebp=0 right
// after the Feature Type Names block (0x424e0f) and reuses ebp as the zero and
// then as `file` for the rest of the function; ours materialises edi=0 inside
// the inlined FreeFeatureList (0x424ee2 region) and reloads `file` into ebx
// twice, leaving a redundant mov edi,eax and a per-iteration xor ebp,ebp in
// the names vector fill. Hoisting the remap index to function scope, spelling
// out the delete null-check, and a post-FreeFeatureList local `file` pointer
// were all flat at 69.1%.
// deepseek-v4.1 re-run: confirmed the root is EBP's meaning at the join after
// the names record loop. The original restores ebp=0 on the then path at
// 0x424e13, so ebp is the shared constant zero for the FreeFeatureList inlines
// and for the tail; there it is pushed as the 0 default of FUN_004b4800 and
// then reused to hold `file` (0x424f45), which is why the tail keeps count in
// ebx with no reloads. Ours lets ebp die as count, so the FreeFeatureList
// picks edi as its zero and `file` lands in ebx, is copied to esi inside each
// record loop and reloaded twice (1514 vs 1496 bytes). A tail `const int zero`
// fed to all three FUN_004b4800 defaults, promoting n to function scope, a
// names vector with an explicit default argument, one counter per tail loop
// and literal byte offsets were all scored through check.py at 69.1%.

#include <string.h>
#include <utility>

// <xutility> as the original file compiled it (with /Gz): the same
// templates, __stdcall. Defining _XUTILITY_ keeps out the header's __cdecl
// ones, which <vector> and <xstring> would include.
#define _XUTILITY_
namespace std {
template <class _II, class _OI>
inline _OI __stdcall copy(_II _F, _II _L, _OI _X)
{
    for (; _F != _L; ++_X, ++_F)
        *_X = *_F;
    return (_X);
}
template <class _BI1, class _BI2>
inline _BI2 __stdcall copy_backward(_BI1 _F, _BI1 _L, _BI2 _X)
{
    while (_F != _L)
        *--_X = *--_L;
    return (_X);
}
template <class _II1, class _II2>
inline bool __stdcall equal(_II1 _F, _II1 _L, _II2 _X)
{
    return (mismatch(_F, _L, _X).first == _L);
}
template <class _II1, class _II2, class _Pr>
inline bool __stdcall equal(_II1 _F, _II1 _L, _II2 _X, _Pr _P)
{
    return (mismatch(_F, _L, _X, _P).first == _L);
}
template <class _FI, class _Ty>
inline void __stdcall fill(_FI _F, _FI _L, const _Ty& _X)
{
    for (; _F != _L; ++_F)
        *_F = _X;
}
template <class _OI, class _Sz, class _Ty>
inline void __stdcall fill_n(_OI _F, _Sz _N, const _Ty& _X)
{
    for (; 0 < _N; --_N, ++_F)
        *_F = _X;
}
template <class _II1, class _II2>
inline bool __stdcall lexicographical_compare(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2)
{
    for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2)
        if (*_F1 < *_F2)
            return (true);
        else if (*_F2 < *_F1)
            return (false);
    return (_F1 == _L1 && _F2 != _L2);
}
template <class _II1, class _II2, class _Pr>
inline bool __stdcall lexicographical_compare(_II1 _F1, _II1 _L1, _II2 _F2, _II2 _L2, _Pr _P)
{
    for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2)
        if (_P(*_F1, *_F2))
            return (true);
        else if (_P(*_F2, *_F1))
            return (false);
    return (_F1 == _L1 && _F2 != _L2);
}
#define _MAX _cpp_max
#define _MIN _cpp_min
template <class _Ty>
inline const _Ty& __stdcall _cpp_max(const _Ty& _X, const _Ty& _Y)
{
    return (_X < _Y ? _Y : _X);
}
template <class _Ty, class _Pr>
inline const _Ty& __stdcall _cpp_max(const _Ty& _X, const _Ty& _Y, _Pr _P)
{
    return (_P(_X, _Y) ? _Y : _X);
}
template <class _Ty>
inline const _Ty& __stdcall _cpp_min(const _Ty& _X, const _Ty& _Y)
{
    return (_Y < _X ? _Y : _X);
}
template <class _Ty, class _Pr>
inline const _Ty& __stdcall _cpp_min(const _Ty& _X, const _Ty& _Y, _Pr _P)
{
    return (_P(_Y, _X) ? _Y : _X);
}
template <class _II1, class _II2>
inline pair<_II1, _II2> __stdcall mismatch(_II1 _F, _II1 _L, _II2 _X)
{
    for (; _F != _L && *_F == *_X; ++_F, ++_X)
        ;
    return (pair<_II1, _II2>(_F, _X));
}
template <class _II1, class _II2, class _Pr>
inline pair<_II1, _II2> __stdcall mismatch(_II1 _F, _II1 _L, _II2 _X, _Pr _P)
{
    for (; _F != _L && _P(*_F, *_X); ++_F, ++_X)
        ;
    return (pair<_II1, _II2>(_F, _X));
}
template <class _Ty>
inline void __stdcall swap(_Ty& _X, _Ty& _Y)
{
    _Ty _Tmp = _X;
    _X = _Y, _Y = _Tmp;
}
}

#include <vector>

class Class_004b4560 {
public:
    void FUN_004b4560(char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4bf0 {
public:
    int FUN_004b4bf0();
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* dst, int len);
};

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

namespace std {
template <>
unsigned short* __stdcall copy(unsigned short* first, unsigned short* last, unsigned short* dest);  // 0x4256a0, out of line

template <>
class vector<unsigned short, allocator<unsigned short> > {
public:
    typedef allocator<unsigned short> _A;
    typedef unsigned int size_type;
    typedef unsigned short* iterator;

    _A allocator;
    iterator _First;
    iterator _Last;
    iterator _End;

    explicit vector(const _A& al = _A())
        : allocator(al), _First(0), _Last(0), _End(0) {}
    ~vector()
    {
        operator delete(_First);
    }
    size_type _Size() const
    {
        return _First == 0 ? 0 : _Last - _First;
    }
    size_type size() const;  // 0x4251f0, out of line
    void insert(iterator p, size_type m, const unsigned short& x);  // 0x425210, out of line
    iterator erase(iterator f, iterator l);  // 0x425430, out of line
    void resize1(size_type n, const unsigned short& x)
    {
        if (_Size() < n)
            insert(_Last, n - _Size(), x);
        else if (n < size())
            erase(_First + n, _Last);
    }
    void resize2(size_type n, const unsigned short& x)
    {
        if (_Size() < n)
            insert(_Last, n - _Size(), x);
        else if (n < _Size()) {
            iterator s = copy(_Last, _Last, _First + n);
            _Destroy(s, _Last);
            _Last = s;
        }
    }

protected:
    void _Destroy(iterator f, iterator l);  // 0x425470, out of line
};

template <>
class vector<Class_004c2ea0*, allocator<Class_004c2ea0*> > {
public:
    typedef allocator<Class_004c2ea0*> _A;
    typedef Class_004c2ea0** iterator;

    _A allocator;
    iterator _First;
    iterator _Last;
    iterator _End;

    ~vector()
    {
        _Destroy(_First, _Last);
        operator delete(_First);
        _First = 0, _Last = 0, _End = 0;
    }

protected:
    void _Destroy(iterator f, iterator l);  // 0x4251e0, out of line
};
}

struct Vec3_00424c00 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Rot16_00424c00 {
    short x, y, z;
};

struct FeatureName_00424c00 {
    char name[0x80];
};

struct Feature_00424c00 {
    char name[0x100];
};

struct Spot_00424c00 {
    char unknown_0[4];
    unsigned short frame;              // +0x4
    char unknown_6[0x26 - 6];
    unsigned short damage;             // +0x26
    char unknown_28[0x2e - 0x28];
    unsigned char animBits;            // +0x2e
    char unknown_2f;
};

struct Cell_00424c00 {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    unsigned char flags;               // +0xc
};

struct Game_00424c00 {
    char unknown_0[0x1420b];
    Spot_00424c00* spots;              // +0x1420b
    char unknown_1420f[0x14253 - 0x1420f];
    int featureCount;                  // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature_00424c00* features;        // +0x1426f
};

struct Normal_00424c00 {
    unsigned short x;
    unsigned short y;
    unsigned short feature;
    unsigned short spot;
};

struct Anim_00424c00 {
    unsigned short x;
    unsigned short y;
    unsigned short feature;
    unsigned short damage;
    unsigned char frame;
    union {
        unsigned char bits;
        struct {
            unsigned char anim : 4;
            unsigned char animHi : 4;
        };
    };
};

struct Model_00424c00 {
    unsigned short x;
    unsigned short y;
    unsigned short feature;
    unsigned short damage;
    Vec3_00424c00 pos;
    Rot16_00424c00 rot;
};
#pragma pack(pop)

extern Game_00424c00* g_game;
extern std::vector<Class_004c2ea0*>* DAT_00511fb4;

void __stdcall FUN_004222e0();
unsigned short __stdcall FUN_004224b0(char* name);
void __stdcall FUN_00422ea0();
void __stdcall FUN_004233a0(int x, int y, int flag);
void __stdcall FUN_00423550(int x, int y, int flag);
Cell_00424c00* __stdcall FUN_00481550(int x, int y);
void* __stdcall FUN_00423c50(Cell_00424c00* cell, unsigned short feature, void* pos, void* rot, unsigned char owner);

// FUN_00422e40, inlined
static inline unsigned short FindName(char* name)
{
    for (int i = 0; i < g_game->featureCount; i++) {
        if (_strcmpi(name, g_game->features[i].name) == 0) {
            return (unsigned short)i;
        }
    }
    return 0xffff;
}

static inline unsigned short FeatureIndex(char* name)
{
    unsigned short i = FindName(name);
    if (i != 0xffff)
        return i;
    return FUN_004224b0(name);
}

// FUN_004223e0, inlined
static inline void FreeFeatureList()
{
    for (Class_004c2ea0** p = DAT_00511fb4->_First; p < DAT_00511fb4->_Last; p++)
        delete *p;
    delete DAT_00511fb4;
    DAT_00511fb4 = 0;
}

// FUNCTION: 0x424c00
void __stdcall FUN_00424c00(Class_004b4ba0* file)
{
    ((Class_004b4560*)file)->FUN_004b4560("Features");
    std::vector<unsigned short> remap;
    FUN_004222e0();
    if (file->FUN_004b4ba0("Feature Type Names")) {
        int count = ((Class_004b4bf0*)file)->FUN_004b4bf0() / sizeof(FeatureName_00424c00);
        remap.resize1(count, 0);
        std::vector<FeatureName_00424c00> names(count);
        ((Class_004b4c80*)file)->FUN_004b4c80(names.begin(), count * sizeof(FeatureName_00424c00));
        for (int i = 0; i < count; i++) {
            if (i < g_game->featureCount && _strcmpi(names[i].name, g_game->features[i].name) == 0) {
                remap._First[i] = i;
            } else {
                int j;
                for (j = 0; j < g_game->featureCount; j++) {
                    if (_strcmpi(names[i].name, g_game->features[j].name) == 0) {
                        remap._First[i] = j;
                        goto found;
                    }
                }
                remap._First[i] = FeatureIndex(names[i].name);
            found:;
            }
        }
    } else {
        remap.resize2(g_game->featureCount, 0);
        for (int i = 0; i < g_game->featureCount; i++)
            remap._First[i] = i;
    }
    FUN_00422ea0();
    FreeFeatureList();

    int n = ((Class_004b4800*)file)->FUN_004b4800("Number of Normal Features", 0);
    file->FUN_004b4ba0("Normal Features");
    for (int k = 0; k < n; k++) {
        Normal_00424c00 rec;
        ((Class_004b4c10*)file)->FUN_004b4c10(k * sizeof(Normal_00424c00));
        if (((Class_004b4c80*)file)->FUN_004b4c80(&rec, sizeof(Normal_00424c00)) >= sizeof(Normal_00424c00)) {
            Cell_00424c00* c = FUN_00481550(rec.x, rec.y);
            FUN_00423c50(c, remap._First[rec.feature], 0, 0, 10);
            c->spot = rec.spot;
        }
    }

    n = ((Class_004b4800*)file)->FUN_004b4800("Number of Animating Features", 0);
    file->FUN_004b4ba0("Animating Features");
    for (k = 0; k < n; k++) {
        Anim_00424c00 rec;
        ((Class_004b4c10*)file)->FUN_004b4c10(k * sizeof(Anim_00424c00));
        if (((Class_004b4c80*)file)->FUN_004b4c80(&rec, sizeof(Anim_00424c00)) >= sizeof(Anim_00424c00)) {
            Cell_00424c00* c = FUN_00481550(rec.x, rec.y);
            FUN_00423c50(c, remap._First[rec.feature], 0, 0, 10);
            switch (rec.anim) {
            case 0:
                FUN_004233a0(rec.x, rec.y, 0);
                break;
            case 1:
                FUN_00423550(rec.x, rec.y, 0);
                break;
            case 2:
                FUN_00423550(rec.x, rec.y, 1);
                break;
            }
            Spot_00424c00* s = &g_game->spots[c->spot];
            s->damage = rec.damage;
            s->frame = rec.frame;
            s->animBits = rec.bits & 0xf0;
        }
    }

    n = ((Class_004b4800*)file)->FUN_004b4800("Number of 3D Features", 0);
    file->FUN_004b4ba0("3D Features");
    for (k = 0; k < n; k++) {
        Model_00424c00 rec;
        ((Class_004b4c10*)file)->FUN_004b4c10(k * sizeof(Model_00424c00));
        if (((Class_004b4c80*)file)->FUN_004b4c80(&rec, sizeof(Model_00424c00)) >= sizeof(Model_00424c00)) {
            Cell_00424c00* c = FUN_00481550(rec.x, rec.y);
            FUN_00423c50(c, remap._First[rec.feature], &rec.pos, &rec.rot, 10);
            g_game->spots[c->spot].damage = rec.damage;
        }
    }
}
