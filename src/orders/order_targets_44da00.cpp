// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds the outline of a rectangle (x1..x2, y1..y2) into a
// std::vector<Point_0044eec0>: first the two vertical edges (for each x, the
// bottom and top points), then the two horizontal edges (for each interior y,
// the left and right points). Every point is appended with push_back.
// Stays in its own file: the std::copy specialization below changes the
// vector<Point_0044eec0> instantiations 0x44d0e0 and 0x44d560 share (their
// inlined clear turns into a call to 0x44eef0), so it cannot join them in
// order_targets.cpp.
#include <string.h>
#include <utility>

// <xutility> as this file compiled it (with /Gz): the same templates, but
// __stdcall. Defining _XUTILITY_ keeps out the header's __cdecl ones, which
// <vector> and <xstring> would otherwise pull in.
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

struct Point_0044eec0 {
    short x;
    short y;
};

// The out-of-line copy the inlined vector::clear tail calls (0x44eef0).
Point_0044eec0* __stdcall CopyDwordRangeUnchecked(Point_0044eec0* first, Point_0044eec0* last, Point_0044eec0* dest);

namespace std {
template <>
Point_0044eec0* __stdcall copy(Point_0044eec0* first, Point_0044eec0* last, Point_0044eec0* dest)
{
    return CopyDwordRangeUnchecked(first, last, dest);
}
}

typedef std::vector<Point_0044eec0> Vec_0044da00;

class PointMarker {
public:
    char unknown_0[8];
    int x1;                 // +0x8
    int x2;                 // +0xc
    int y1;                 // +0x10
    int y2;                 // +0x14

    void AppendGoalCell(Vec_0044da00* list);
};

static Point_0044eec0 MakePoint_0044da00(int x, int y)
{
    Point_0044eec0 p;
    p.x = (short)x;
    p.y = (short)y;
    return p;
}

// FUNCTION: 0x44da00
void PointMarker::AppendGoalCell(Vec_0044da00* list)
{
    list->clear();
    for (int i = x1; i <= x2; i++) {
        list->push_back(MakePoint_0044da00(i, y1));
        list->push_back(MakePoint_0044da00(i, y2));
    }
    for (int j = y1 + 1; j <= y2 - 1; j++) {
        list->push_back(MakePoint_0044da00(x1, j));
        list->push_back(MakePoint_0044da00(x2, j));
    }
}
