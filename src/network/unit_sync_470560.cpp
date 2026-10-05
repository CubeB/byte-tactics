// Decompiled by space-bunny-free. Names are provisional.
// Class_00470560::operator=: copies three ints, then assigns two vectors of a
// 14-byte element type (Elem_0046faf0, packed to 2 bytes, so the three ints and
// the short give 14 bytes and the copy loops move three dwords and a word).
// The first vector's operator= is inlined by /Ob2, the second stays a call to
// its out-of-line copy (0x4707a0), and the same inline budget is why three of
// the seven size() calls are calls to 0x470770 while the other four are
// expanded inline. The element type's _Ucopy (0x46faf0) and _Destroy
// (0x46e870) are called, and the 14-byte std::copy of the "enough room, but
// longer" branch is 0x470a40.
// The file of the original was compiled with /Gz, so <xutility>'s templates
// are __stdcall there and the std::copy call cleans up its own arguments.
// Defining _XUTILITY_ keeps out the header's __cdecl ones, which <vector>
// would otherwise include; the block below stands in for <xutility> (the same
// stand-in 0x424c00.cpp uses, with the C4666 warnings it gives).
//
// Every byte matches, but check.py still reports two references as wrong, both
// of them names in data/ that this file cannot use and should not work around:
//
//  0. The three calls to 0x470770 come out as `Elem_0046faf0::?$vector::size`,
//     which is what they are: taking the address of the real
//     `std::vector<Elem_0046faf0>::size` compiles to 0x470770 byte for byte
//     (checked with `?size@?$vector@UElem_0046faf0@@...@QBEIXZ`). data/symbols.csv
//     gives 0x470770 the placeholder name `Class_00470770::FUN_00470770` from
//     0x470770.cpp, a hand-written stand-in for the same body. Renaming that
//     row the way 0x40c5b0.cpp names vector<Elem_0040cc40>::size would make
//     these three references right.
//  1. The std::copy at 0x470a40 is the second `std::copy` instantiation the
//     checker sees (0x4256a0 is the first, for unsigned short). Overloads and
//     duplicate instantiations share one name in base_name, which is the case
//     docs/consolidation.md predicts needs a row in data/aliases.csv:
//     `std::copy,0x470a40,...`.
#include <utility>

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

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

struct Class_00470560 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    std::vector<Elem_0046faf0> list_c; // +0x0c (16 bytes, _First at +0x10)
    std::vector<Elem_0046faf0> list_d; // +0x1c (operator= is 0x4707a0)
    Class_00470560& operator=(const Class_00470560& rhs);
};

// FUNCTION: 0x470560 ??4Class_00470560@@QAEAAU0@ABU0@@Z
Class_00470560& Class_00470560::operator=(const Class_00470560& rhs)
{
    field_0 = rhs.field_0;
    field_4 = rhs.field_4;
    field_8 = rhs.field_8;
    list_c = rhs.list_c;
    list_d = rhs.list_d;
    return *this;
}
