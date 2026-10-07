// Decompiled by Opus, deepseek-v4.1-flash, Claude Opus 5.5, Haiku, Space Bunny Free. Names are provisional.
// The std::vector instantiations of the los_tables module from
// vector<Elem_00434020> (a 4-byte line point) up through the vector of
// vectors of Elem_00434360 (a struct holding one such vector): the size,
// insert, erase, _Destroy, _Ucopy, allocator, copy-constructor and
// assignment members the table code uses, each emitted out of line by taking
// an address.
// Four of the part's functions stay out of this file: 0x433a80, 0x434020
// and 0x4344e0 (los_tables_433a80.cpp) need std::_Destroy and std::_Construct
// overloads that call the out-of-line FUN_00434430 and FUN_004345c0, and
// 0x434360 (los_tables_434360.cpp) needs the same _Destroy name for the
// element type to call FUN_00434440. Those overloads disagree with the plain
// template this file's functions need.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

// std::vector<Elem_00434020>::~vector(): frees _First and zeroes the three
// pointers. The original calls it from the element destroy loop of
// vector<Elem_00434360>::operator= (0x434770, at 0x4348f5).
typedef std::vector<Elem_00434360> Outer_00433a30;
typedef Outer_00433a30& (Outer_00433a30::*AssignFn_00433a30)(const Outer_00433a30&);

// Taking this operator='s address makes the destructor get emitted out of line.
AssignFn_00433a30 g_assign_00433a30 = &Outer_00433a30::operator=;

// FUNCTION: 0x433a30 ??1?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAE@XZ

// std::vector<Elem_00434020>::size().
// The same vector as 0x433d50 (erase): 0x4336f0 calls 0x433d50, 0x433a60
// (size) and 0x433d90 (_Destroy) with ecx set to it, and the destroy loop
// 0x433270 runs each inner vector's inlined destructor through 0x433d90
// and 0x433da0 (allocator::deallocate).
typedef std::vector<Elem_00434020> Vec_00433a60;
// Taking the member's address makes the compiler emit it.
typedef Vec_00433a60::size_type (Vec_00433a60::*SizeFn_00433a60)() const;

// FUNCTION: 0x433a60 ?size@?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QBEIXZ
SizeFn_00433a60 g_size_00433a60 = &Vec_00433a60::size;

// std::vector<Elem_00434360>::size() (16-byte elements, each a struct holding
// a std::vector<Elem_00434020>). Its caller 0x433380 resizes the same vector
// with 0x433db0 (its insert), which copies elements with the
// vector<Elem_00434020> copy constructor (0x434470) and operator= (0x4345e0).
typedef std::vector<Elem_00434360> Vec_00433b00;
// Taking the member's address makes the compiler emit it.
typedef Vec_00433b00::size_type (Vec_00433b00::*SizeFn_00433b00)() const;

// FUNCTION: 0x433b00 ?size@?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QBEIXZ
SizeFn_00433b00 g_size_00433b00 = &Vec_00433b00::size;

// std::vector<Elem_00434020>::insert(iterator, size_type, const T&) for the
// 4-byte element (two unsigned shorts) used by the vector at 0x433d50 (erase),
// 0x433d90 (_Destroy) and 0x433da0 (deallocate). Its only caller, 0x4336f0, is
// an inlined resize: it calls this with end(), n - size() and a temporary
// element.
typedef std::vector<Elem_00434020> Vec_00433b20;
// Taking the member's address makes the compiler emit it.
typedef void (Vec_00433b20::*InsertFn_00433b20)(
    Vec_00433b20::iterator, Vec_00433b20::size_type, const Elem_00434020&);

// FUNCTION: 0x433b20 ?insert@?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAEXPAUElem_00434020@@IABU3@@Z
InsertFn_00433b20 g_insert_00433b20 = &Vec_00433b20::insert;

// std::vector<Elem_00434020>::erase(iterator first, iterator last) for a
// 4-byte element (its caller at 0x4336f0 shrinks the vector with it, as an
// inlined resize). The element layout (two unsigned shorts) follows 0x4339e0,
// which reads entries of a vector at +0x4.
typedef std::vector<Elem_00434020> Vec_00433d50;
// Taking the member's address makes the compiler emit it.
typedef Vec_00433d50::iterator (Vec_00433d50::*EraseFn_00433d50)(
    Vec_00433d50::iterator, Vec_00433d50::iterator);

// FUNCTION: 0x433d50 ?erase@?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAEPAUElem_00434020@@PAU3@0@Z
EraseFn_00433d50 g_erase_00433d50 = &Vec_00433d50::erase;

// std::vector<Elem_00434020>::_Destroy(first, last): empty, since the element
// type is trivial.
// The same vector as 0x433d50 (erase): 0x4336f0 calls 0x433d50, 0x433a60
// (size) and 0x433d90 (_Destroy) with ecx set to it, and the destroy loop
// 0x433270 runs each inner vector's inlined destructor through 0x433d90
// and 0x433da0 (allocator::deallocate).
typedef std::vector<Elem_00434020> Vec_00433d90;
typedef void (Vec_00433d90::*DestroyFn_00433d90)(Vec_00433d90::iterator, Vec_00433d90::iterator);

// _Destroy is protected: a derived class takes its address to get it emitted.
struct Access_00433d90 : Vec_00433d90 {
    static DestroyFn_00433d90 fn;
};

// FUNCTION: 0x433d90 ?_Destroy@?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@IAEXPAUElem_00434020@@0@Z
DestroyFn_00433d90 Access_00433d90::fn = &Access_00433d90::_Destroy;

// std::allocator<Elem_00434020>::deallocate(p, n): operator delete(p). Called
// with ecx set to a vector's allocator, with _First and _End - _First as
// arguments.
// The same vector as 0x433d50 (erase): 0x4336f0 calls 0x433d50, 0x433a60
// (size) and 0x433d90 (_Destroy) with ecx set to it, and the destroy loop
// 0x433270 runs each inner vector's inlined destructor through 0x433d90
// and 0x433da0 (allocator::deallocate).
typedef std::allocator<Elem_00434020> Alloc_00433da0;
// Taking the member's address makes the compiler emit it.
typedef void (Alloc_00433da0::*DeallocateFn_00433da0)(void*, Alloc_00433da0::size_type);

// FUNCTION: 0x433da0 ?deallocate@?$allocator@UElem_00434020@@@std@@QAEXPAXI@Z
DeallocateFn_00433da0 g_deallocate_00433da0 = &Alloc_00433da0::deallocate;

// std::vector<Elem_00434360>::insert(iterator, size_type, const T&), where
// Elem_00434360 is a struct holding one std::vector<Elem_00434020>. Its
// callers, 0x433380 and 0x4335f0, resize a vector<Elem_00434360> with it.
typedef std::vector<Elem_00434360> Vec_00433db0;
// Taking the member's address makes the compiler emit it.
typedef void (Vec_00433db0::*InsertFn_00433db0)(
    Vec_00433db0::iterator, Vec_00433db0::size_type, const Elem_00434360&);

// FUNCTION: 0x433db0 ?insert@?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAEXPAUElem_00434360@@IABU3@@Z
InsertFn_00433db0 g_insert_00433db0 = &Vec_00433db0::insert;

// std::vector<Elem_00434360>::_Destroy(first, last) from MSVC 5's <vector>:
// runs each element's destructor, which is ~vector<Elem_00434020>
// (free _First and zero the three pointers). The caller (0x433130) calls it
// with ecx set to a local vector.
typedef std::vector<Elem_00434360> Vec_004340b0;
typedef void (Vec_004340b0::*DestroyFn_004340b0)(Vec_004340b0::iterator, Vec_004340b0::iterator);

// _Destroy is protected: a derived class takes its address to emit it out of line.
struct Access_004340b0 : Vec_004340b0 {
    static DestroyFn_004340b0 fn;
};

// FUNCTION: 0x4340b0 ?_Destroy@?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@IAEXPAUElem_00434360@@0@Z
DestroyFn_004340b0 Access_004340b0::fn = &Access_004340b0::_Destroy;

// std::vector<std::vector<Elem_00434360> >::insert(iterator, size_type,
// const T&) from MSVC 5's <vector>, where Elem_00434360 is a struct holding
// one std::vector<Elem_00434020>: the insert of n copies behind the global at
// 0x51e6a0. The inner vector's copy constructor (0x4344e0), operator=
// (0x434770) and destructor (0x433a80) stay out of line.
typedef std::vector<Elem_00434360> Inner_004340f0;
typedef std::vector<Inner_004340f0> Outer_004340f0;
typedef void (Outer_004340f0::*InsertFn_004340f0)(
    Outer_004340f0::iterator, Outer_004340f0::size_type, const Inner_004340f0&);

// Taking the member's address emits the template instantiation out of line.
// FUNCTION: 0x4340f0 ?insert@?$vector@V?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@V?$allocator@V?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@@2@@std@@QAEXPAV?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@2@IABV32@@Z
InsertFn_004340f0 g_insert_004340f0 = &Outer_004340f0::insert;

// std::allocator<Elem_00434020>::destroy(pointer), out of line: empty, since
// the element type is trivial. Its only caller (0x433540) sets ecx to the
// allocator first, so this is the allocator member, like 0x434400 one level
// up. Taking the member's address makes the compiler emit it.
typedef std::allocator<Elem_00434020> Alloc_004343f0;
typedef void (Alloc_004343f0::*DestroyFn_004343f0)(Elem_00434020*);

// FUNCTION: 0x4343f0 ?destroy@?$allocator@UElem_00434020@@@std@@QAEXPAUElem_00434020@@@Z
DestroyFn_004343f0 g_destroy_004343f0 = &Alloc_004343f0::destroy;

// std::allocator<Elem_00434360>::destroy(pointer), out of line: the element's
// implicit destructor is ~vector<Elem_00434020>, which frees _First and
// zeroes the three pointers. Its caller (0x4330b0, the destroy loop of a
// vector of vectors of these elements) sets ecx to the allocator before each
// call, so this is the allocator member, not std::_Destroy. The element type is
// the one 0x434770 (operator= of the middle vector) copies with 0x4345e0,
// vector<Elem_00434020>::operator=.
typedef std::allocator<Elem_00434360> Alloc_00434400;
typedef void (Alloc_00434400::*DestroyFn_00434400)(Elem_00434360*);

// Taking the member's address emits the template instantiation out of line.
// FUNCTION: 0x434400 ?destroy@?$allocator@UElem_00434360@@@std@@QAEXPAUElem_00434360@@@Z
DestroyFn_00434400 g_destroy_00434400 = &Alloc_00434400::destroy;

// FUNCTION: 0x434430
void __stdcall FUN_00434430(int)
{
}

// std::_Destroy(Elem_00434360*) from MSVC 5's <xmemory>, where Elem_00434360
// is a struct holding one std::vector<Elem_00434020>: runs the element's
// destructor, which is the held vector's. Its caller is the destroy loop of
// 0x434360.
// Explicit __stdcall: the original takes no ecx and ends in `ret 4`.
// FUNCTION: 0x434440
void __stdcall FUN_00434440(Elem_00434360* p)
{
    std::_Destroy(p);
}

// Elem_00434360::Elem_00434360(const Elem_00434360&), the implicit copy
// constructor of the struct holding one std::vector<Elem_00434020>: it
// inlines that vector's copy constructor from MSVC 5's <vector> (allocate
// size() elements and copy them with the placement-new construct). Both
// callers, vector<Elem_00434360>::insert (0x433db0) and operator=
// (0x434770), reference it under this name when they are compiled with the
// struct as the element.
// Taking insert's address makes the compiler emit this constructor.
typedef std::vector<Elem_00434360> Vec_00434470;
typedef void (Vec_00434470::*InsertFn_00434470)(
    Vec_00434470::iterator, Vec_00434470::size_type, const Elem_00434360&);

typedef std::vector<Elem_00434360> Vec_004349f0;
typedef Vec_004349f0& (Vec_004349f0::*AssignFn_004349f0)(const Vec_004349f0&);

// FUNCTION: 0x434470 ??0Elem_00434360@@QAE@ABU0@@Z
InsertFn_00434470 g_insert_00434470 = &Vec_00434470::insert;

// The compiler-generated scalar deleting destructor of Elem_00434360, the
// element of the vector whose operator= is 0x434770: a struct holding a
// std::vector<Elem_00434020> with an implicit destructor, so this is
// ~vector (free _First, then zero the three pointers).
// Taking operator='s address emits this; the element must be a struct, not a
// bare std::vector.
// FUNCTION: 0x4349f0 ??_GElem_00434360@@QAEPAXI@Z
AssignFn_004349f0 g_assign_004349f0 = &Vec_004349f0::operator=;

// FUNCTION: 0x4345c0
void __stdcall FUN_004345c0(int* param_1, int* param_2)
{
    if (param_1 != 0) {
        *param_1 = *param_2;
    }
}

// std::vector<Elem_00434020>::operator=(const vector&) from MSVC 5's
// <vector>: the three-way size/capacity test with copy, _Ucopy and the
// allocate branch. Taking the member's address makes the compiler emit the
// template instantiation out of line, the way the original file did.
typedef std::vector<Elem_00434020> Inner_004345e0;
typedef Inner_004345e0& (Inner_004345e0::*AssignFn_004345e0)(const Inner_004345e0&);

// FUNCTION: 0x4345e0 ??4?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAEAAV01@ABV01@@Z
AssignFn_004345e0 g_assign_004345e0 = &Inner_004345e0::operator=;

// std::vector<Elem_00434360>::operator=(const vector&) from MSVC 5's
// <vector>, where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>: the three-way size/capacity test with copy,
// _Ucopy and the allocate branch. Its callers are the outer
// vector<vector<Elem_00434360>>'s insert (0x4340f0) and erase (0x434360).
typedef std::vector<Elem_00434360> Vec_00434770;
typedef Vec_00434770& (Vec_00434770::*AssignFn_00434770)(const Vec_00434770&);

// Taking the member's address emits the template instantiation out of line.
// FUNCTION: 0x434770 ??4?$vector@UElem_00434360@@V?$allocator@UElem_00434360@@@std@@@std@@QAEAAV01@ABV01@@Z
AssignFn_00434770 g_assign_00434770 = &Vec_00434770::operator=;

// std::vector<Elem_00434020>::_Ucopy(first, last, dest) from MSVC 5's
// <vector> for a 4-byte element type.
typedef std::vector<Elem_00434020> Vec_004349c0;
typedef Vec_004349c0::iterator (Vec_004349c0::*UcopyFn_004349c0)(
    Vec_004349c0::const_iterator, Vec_004349c0::const_iterator, Vec_004349c0::iterator);

// _Ucopy is protected: a derived class takes its address to emit it out of line.
struct Access_004349c0 : Vec_004349c0 {
    static UcopyFn_004349c0 fn;
};

// FUNCTION: 0x4349c0 ?_Ucopy@?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@IAEPAUElem_00434020@@PBU3@0PAU3@@Z
UcopyFn_004349c0 Access_004349c0::fn = &Access_004349c0::_Ucopy;
