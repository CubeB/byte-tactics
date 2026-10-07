// Decompiled by Opus, Space Bunny Free. Names are provisional.
// The three instantiations of the los_tables module whose destroys call the
// out-of-line helpers FUN_00434430 (0x434430) and, for the copy constructor,
// FUN_004345c0 (0x4345c0): the destructor of vector<Elem_00434360>
// (0x433a80), the erase of the same vector (0x434020) and the copy
// constructor of the same vector (0x4344e0). They need std::_Destroy and
// std::_Construct overloads that disagree with the plain template the rest
// of the module's los_tables_433a30.cpp needs, so they share this file.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

void __stdcall FUN_00434430(int);
void __stdcall FUN_004345c0(int* p, const int* q);

namespace std {
inline void _Destroy(Elem_00434020* p)
{
    FUN_00434430((int)p);
}

inline void _Construct(Elem_00434020* p, const Elem_00434020& v)
{
    FUN_004345c0((int*)p, (const int*)&v);
}
}

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434020> Inner_00433a80;

namespace std {
// Calls the held vector's destructor directly: one inline level shallower than
// the implicit destructor.
inline void _Destroy(Elem_00434360* p)
{
    p->v.~Inner_00433a80();
}
}

// std::vector<Elem_00434360>::~vector() (the same class as the erase at
// 0x434020), where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>: each element's held vector has its elements go
// through the empty FUN_00434430 (std::_Destroy overload above), then its
// _First is freed and its three pointers zeroed; finally the outer _First is
// freed and zeroed. Its callers (0x433270, 0x4340f0) are the destroy loops of
// a vector of these vectors.
typedef std::vector<Elem_00434360> Middle_00433a80;
typedef std::vector<Middle_00433a80> Outer_00433a80;
typedef Outer_00433a80& (Outer_00433a80::*AssignFn_00433a80)(const Outer_00433a80&);

// Taking the next level's operator= address makes this destructor get emitted.
AssignFn_00433a80 g_assign_00433a80 = &Outer_00433a80::operator=;

// FUNCTION: 0x433a80 ??1?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAE@XZ

// std::vector<Elem_00434360>::erase(iterator first, iterator last) from MSVC
// 5's <vector>, where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>: copy the tail down with the held vector's
// operator= (0x4345e0), then destroy the leftover elements. Destroying an
// inner element calls the empty out-of-line FUN_00434430.
typedef std::vector<Elem_00434360> Outer_00434020;
typedef Outer_00434020::iterator (Outer_00434020::*EraseFn_00434020)(
    Outer_00434020::iterator, Outer_00434020::iterator);

// Taking the member's address emits the template instantiation out of line.
// FUNCTION: 0x434020 ?erase@?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAEPAUElem_00434360@@PAU3@0@Z
EraseFn_00434020 g_erase_00434020 = &Outer_00434020::erase;

// std::vector<Elem_00434360>::vector(const vector&), the copy constructor
// from MSVC 5's <vector>, where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>: copy the empty allocator byte, allocate size()
// elements and copy them with the placement-new construct. The held
// vector<Elem_00434020> copy constructor copies every element through the
// out-of-line allocator<Elem_00434020>::construct (0x4345c0).
// The only caller is the outer vector<vector<Elem_00434360>>::insert
// (0x4340f0), one level above 0x434470's caller.
typedef std::vector<Elem_00434020> Inner_004344e0;
typedef std::vector<Elem_00434360> Middle_004344e0;
typedef std::vector<Middle_004344e0> Outer_004344e0;
typedef void (Outer_004344e0::*InsertFn_004344e0)(
    Outer_004344e0::iterator, Outer_004344e0::size_type, const Middle_004344e0&);

// Taking insert's address emits this copy constructor out of line.
// FUNCTION: 0x4344e0 ??0?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAE@ABV01@@Z
InsertFn_004344e0 g_insert_004344e0 = &Outer_004344e0::insert;
