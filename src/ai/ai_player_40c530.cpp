// Decompiled by Haiku, Sonnet, GPT-6 Astra, Opus, deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, mimo-v2.6-pro, space-bunny-free, Claude Opus 5.5 and GPT-6. Names are provisional.
// The out-of-line std::vector member functions at the end of the player AI
// unit (0x40c530 to 0x40d5b0), one per vector of the player AI object built by
// 0x409160: the unit lists at +0x05, +0x15 and +0x25 (Unit*), the map cells at
// +0x4d, the 3-byte entries at +0x65, the shorts at +0x7d, the bytes at +0x8d
// and +0x9d, the 1-byte entries at +0xad and the 4-byte entries at +0xcd.
// Each is emitted by taking its address (or by the use the comment names).
#include <windows.h>
#include <shlobj.h>
#include <imagehlp.h>
#include <d3d.h>
#include <memory.h>
#include <math.h>
// ta_types.h is included for its symbol ids: the vector instantiations below
// follow the ids the unit's headers gave them. Its views of two element types
// have no copy constructor a vector can use, so those two are renamed here and
// defined below.
#define Elem_0040cc40 Elem_0040cc40_view
#define Elem_0040d4f0 Elem_0040d4f0_view
#include "ta_types.h"
#undef Elem_0040cc40
#undef Elem_0040d4f0

// A map cell and its sort key, the element of the vector at +0x4d.
struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

// The element of the vector at +0xad, a 1-byte type other than unsigned char
// (a guess). It has no default constructor: the symbol ids of the vector
// instantiations below follow the declaration count.
struct Elem_0040d4f0 {
    char value;                        // +0x0
    Elem_0040d4f0(const Elem_0040d4f0& o) : value(o.value) {}
};

#include <new>
// The placement copies of Elem_0040cc40 and Elem_0040d4f0 that insert and
// resize call instead of expanding: std::_Construct, which pathfind.cpp holds
// at 0x40d5e0 and 0x40d600 under these names.
inline void __stdcall CopyOneByte(Elem_0040d4f0* dest, const Elem_0040d4f0* src) { new (dest) Elem_0040d4f0(*src); }
inline void __stdcall CopyDwordPair(Elem_0040cc40* dest, const Elem_0040cc40* src) { new (dest) Elem_0040cc40(*src); }
namespace std {
inline void _Construct(Elem_0040d4f0* dest, const Elem_0040d4f0& src) { CopyOneByte(dest, &src); }
inline void _Construct(Elem_0040cc40* dest, const Elem_0040cc40& src) { CopyDwordPair(dest, &src); }
}

typedef std::vector<Unit*> Vec_0040c530;
typedef std::vector<Elem_0040cc40> Vec_0040c580;
typedef std::vector<Elem_0040cfb0> Vec_0040c5d0;
typedef std::vector<Elem_0040d4f0> Vec_0040c600;
typedef std::vector<Elem_0040d550> Vec_0040c7f0;
typedef std::vector<short> Vec_0040d000;
typedef std::vector<unsigned char> Vec_0040d290;

// std::vector<Unit*>::~vector(): _First is freed and the three pointers
// zeroed. 0x40b390 calls it on the unit lists at +0x05, +0x15 and +0x25, and
// 0x4103e0, 0x410850 and 0x4152f0 on a local that 0x40c510 (the vector's
// constructor) builds. Taking the address of the outer vector's operator=
// makes the compiler emit the inner destructor out of line.
typedef std::vector<Vec_0040c530> Outer_0040c530;
typedef Outer_0040c530& (Outer_0040c530::*AssignFn_0040c530)(const Outer_0040c530&);
// FUNCTION: 0x40c530 ??1?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAE@XZ
AssignFn_0040c530 g_assign_0040c530 = &Outer_0040c530::operator=;

// std::vector<Unit*>::size(). 0x4152f0 calls it on the local that 0x40c510
// builds and 0x40c530 frees; 0x40ad80, 0x480250, 0x48ca20 and 0x48ddc0 call it
// from inlined insert code for 4-byte elements.
typedef Vec_0040c530::size_type (Vec_0040c530::*SizeFn_0040c560)() const;
// FUNCTION: 0x40c560 ?size@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QBEIXZ
SizeFn_0040c560 g_size_0040c560 = &Vec_0040c530::size;

// std::vector<Elem_0040cc40>::~vector() (the same code as 0x40c530). 0x40b390
// calls it on the vector at +0x4d.
typedef std::vector<Vec_0040c580> Outer_0040c580;
typedef Outer_0040c580& (Outer_0040c580::*AssignFn_0040c580)(const Outer_0040c580&);
// FUNCTION: 0x40c580 ??1?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@QAE@XZ
AssignFn_0040c580 g_assign_0040c580 = &Outer_0040c580::operator=;

// std::vector<Elem_0040cc40>::size(). 0x40a7b0 calls it on the vector at
// +0x4d; 0x40a260 and 0x40ca50 call it from the same inlined insert code.
typedef Vec_0040c580::size_type (Vec_0040c580::*SizeFn_0040c5b0)() const;
// FUNCTION: 0x40c5b0 ?size@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@QBEIXZ
SizeFn_0040c5b0 g_size_0040c5b0 = &Vec_0040c580::size;

// std::vector<Elem_0040cfb0>::~vector() (the same code as 0x40c530). 0x40b390
// calls it on the vector of 3-byte elements at +0x65.
typedef std::vector<Vec_0040c5d0> Outer_0040c5d0;
typedef Outer_0040c5d0& (Outer_0040c5d0::*AssignFn_0040c5d0)(const Outer_0040c5d0&);
// FUNCTION: 0x40c5d0 ??1?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAE@XZ
AssignFn_0040c5d0 g_assign_0040c5d0 = &Outer_0040c5d0::operator=;

// std::vector<Elem_0040d4f0>::resize(), on the vector at +0xad. It calls
// _Ucopy (0x40d4f0), _Ufill (0x40d520) and _Destroy (0x40d4b0).
typedef void (Vec_0040c600::*ResizeFn_0040c600)(unsigned int, const Elem_0040d4f0&);
// FUNCTION: 0x40c600 ?resize@?$vector@UElem_0040d4f0@@V?$allocator@UElem_0040d4f0@@@std@@@std@@QAEXIABUElem_0040d4f0@@@Z
ResizeFn_0040c600 g_resize_0040c600 = &Vec_0040c600::resize;

// std::vector<Elem_0040d550>::resize(), on the 4-byte entries at +0xcd. It
// calls _Ucopy (0x40d550), _Ufill (0x40d580) and _Destroy (0x40d4e0).
typedef void (Vec_0040c7f0::*ResizeFn_0040c7f0)(unsigned int, const Elem_0040d550&);
// FUNCTION: 0x40c7f0 ?resize@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@QAEXIABUElem_0040d550@@@Z
ResizeFn_0040c7f0 g_resize_0040c7f0 = &Vec_0040c7f0::resize;

// std::vector<Unit*>::erase(first, last). 0x40aa40 calls it (through clear())
// on the unit lists at +0x05, +0x15 and +0x25, and 0x48d220 together with
// _Destroy (0x406c00).
typedef Vec_0040c530::iterator (Vec_0040c530::*EraseFn_0040c9f0)(
    Vec_0040c530::iterator, Vec_0040c530::iterator);
// FUNCTION: 0x40c9f0 ?erase@?$vector@PAUUnit@@V?$allocator@PAUUnit@@@std@@@std@@QAEPAPAUUnit@@PAPAU3@0@Z
EraseFn_0040c9f0 g_erase_0040c9f0 = &Vec_0040c530::erase;

// std::vector<Elem_0040cc40>::capacity(), called from the inlined reserve in
// 0x40a260.
typedef Vec_0040c580::size_type (Vec_0040c580::*CapacityFn_0040ca30)() const;
// FUNCTION: 0x40ca30 ?capacity@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@QBEIXZ
CapacityFn_0040ca30 g_capacity_0040ca30 = &Vec_0040c580::capacity;

// std::vector<Elem_0040cc40>::insert(iterator, const T&), called from 0x40a7b0
// and with the insert code inlined: it calls _Ufill (0x40d5b0), _Ucopy
// (0x40cc40) and _Destroy (0x40cc30) with ecx set to the vector.
typedef Vec_0040c580::iterator (Vec_0040c580::*InsertFn_0040ca50)(Vec_0040c580::iterator, const Elem_0040cc40&);
// FUNCTION: 0x40ca50 ?insert@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@QAEPAUElem_0040cc40@@PAU3@ABU3@@Z
InsertFn_0040ca50 g_insert_0040ca50 = &Vec_0040c580::insert;

// The members of std::vector<Elem_0040cc40> that are protected: _Destroy
// (empty, since the element's destructor is trivial), _Ucopy (copies [first,
// last) into raw storage at dest and returns the end of the copies) and
// _Ufill (copy-constructs n copies of value into raw storage at first).
typedef void (Vec_0040c580::*DestroyFn_0040cc30)(Vec_0040c580::iterator, Vec_0040c580::iterator);
typedef Vec_0040c580::iterator (Vec_0040c580::*UcopyFn_0040cc40)(
    Vec_0040c580::const_iterator, Vec_0040c580::const_iterator, Vec_0040c580::iterator);
typedef void (Vec_0040c580::*UfillFn_0040d5b0)(
    Vec_0040c580::iterator, Vec_0040c580::size_type, const Elem_0040cc40&);

struct Access_0040c580 : Vec_0040c580 {
    static DestroyFn_0040cc30 destroy;
    static UcopyFn_0040cc40 ucopy;
    static UfillFn_0040d5b0 ufill;
};

// FUNCTION: 0x40cc30 ?_Destroy@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@IAEXPAUElem_0040cc40@@0@Z
DestroyFn_0040cc30 Access_0040c580::destroy = &Access_0040c580::_Destroy;

// FUNCTION: 0x40cc40 ?_Ucopy@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@IAEPAUElem_0040cc40@@PBU3@0PAU3@@Z
UcopyFn_0040cc40 Access_0040c580::ucopy = &Access_0040c580::_Ucopy;

// std::vector<Elem_0040cfb0>::size() for the 3-byte element type (the `/ 3`
// is the pointer difference). 0x409160 calls it on its vector at +0x65, from
// the inlined resize() around the insert (0x40cca0) and erase (0x40cfb0).
typedef Vec_0040c5d0::size_type (Vec_0040c5d0::*SizeFn_0040cc80)() const;
// FUNCTION: 0x40cc80 ?size@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QBEIXZ
SizeFn_0040cc80 g_size_0040cc80 = &Vec_0040c5d0::size;

// std::vector<Elem_0040cfb0>::insert(iterator, size_type, const T&) on a
// 3-byte element; 0x409160 and 0x409730 call it.
typedef void (Vec_0040c5d0::*InsertFn_0040cca0)(Vec_0040c5d0::iterator, unsigned int, const Elem_0040cfb0&);

// A growth step on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate reserve (or the copy
// constructor).
void __stdcall Grow_0040cca0(Vec_0040c5d0* v, int extra)
{
    v->reserve(extra + v->size());
}

// FUNCTION: 0x40cca0 ?insert@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEXPAUElem_0040cfb0@@IABU3@@Z
InsertFn_0040cca0 g_insert_0040cca0 = &Vec_0040c5d0::insert;

// std::vector<Elem_0040cfb0>::erase(first, last) for a 3-byte element type.
typedef Vec_0040c5d0::iterator (Vec_0040c5d0::*EraseFn_0040cfb0)(
    Vec_0040c5d0::iterator, Vec_0040c5d0::iterator);
// FUNCTION: 0x40cfb0 ?erase@?$vector@UElem_0040cfb0@@V?$allocator@UElem_0040cfb0@@@std@@@std@@QAEPAUElem_0040cfb0@@PAU3@0@Z
EraseFn_0040cfb0 g_erase_0040cfb0 = &Vec_0040c5d0::erase;

// std::vector<short>::size(), on the vector at +0x7d, from the inlined
// resize() around the insert (0x40d020) and erase (0x40d240). The element is
// signed: every read of it sign-extends (0x4099fc in 0x409730, 0x40bd41 in
// 0x40bb00, 0x40c21b in 0x40c200), and 0x40aa40 fills it as a vector<short>.
typedef Vec_0040d000::size_type (Vec_0040d000::*SizeFn_0040d000)() const;
// FUNCTION: 0x40d000 ?size@?$vector@FV?$allocator@F@std@@@std@@QBEIXZ
SizeFn_0040d000 g_size_0040d000 = &Vec_0040d000::size;

// std::vector<short>::insert(iterator, size_type, const T&), from the
// inlined resize() of the vector at +0x7d in 0x409160.
typedef void (Vec_0040d000::*InsertFn_0040d020)(
    Vec_0040d000::iterator, Vec_0040d000::size_type, const short&);
// FUNCTION: 0x40d020 ?insert@?$vector@FV?$allocator@F@std@@@std@@QAEXPAFIABF@Z
InsertFn_0040d020 g_insert_0040d020 = &Vec_0040d000::insert;

// std::vector<short>::erase(first, last), called from the inlined resize()
// in 0x409160.
typedef Vec_0040d000::iterator (Vec_0040d000::*EraseFn_0040d240)(
    Vec_0040d000::iterator, Vec_0040d000::iterator);
// FUNCTION: 0x40d240 ?erase@?$vector@FV?$allocator@F@std@@@std@@QAEPAFPAF0@Z
EraseFn_0040d240 g_erase_0040d240 = &Vec_0040d000::erase;

// std::vector<short>::_Destroy(first, last): empty for a trivial element
// type. 0x40b390 runs it from the inlined destructor of the vector at +0x7d.
typedef void (Vec_0040d000::*DestroyFn_0040d280)(Vec_0040d000::iterator, Vec_0040d000::iterator);

struct Access_0040d000 : Vec_0040d000 {
    static DestroyFn_0040d280 destroy;
};

// FUNCTION: 0x40d280 ?_Destroy@?$vector@FV?$allocator@F@std@@@std@@IAEXPAF0@Z
DestroyFn_0040d280 Access_0040d000::destroy = &Access_0040d000::_Destroy;

// std::vector<unsigned char>::insert(iterator, size_type, const T&), with
// _Ucopy, _Ufill, fill and copy_backward all inlined. 0x409160 calls it from
// the inlined resize() of the vector at +0x9d (next to its erase, 0x40d470).
// The member pointer's type is spelled inline (no typedef): it shifts symbol ids.
// FUNCTION: 0x40d290 ?insert@?$vector@EV?$allocator@E@std@@@std@@QAEXPAEIABE@Z
void (std::vector<unsigned char>::*g_insert_0040d290)(unsigned char*, unsigned int,
                                                      const unsigned char&) = &std::vector<unsigned char>::insert;

// std::vector<unsigned char>::erase(first, last), called from the inlined
// resize() in 0x409160.
typedef Vec_0040d290::iterator (Vec_0040d290::*EraseFn_0040d470)(
    Vec_0040d290::iterator, Vec_0040d290::iterator);
// FUNCTION: 0x40d470 ?erase@?$vector@EV?$allocator@E@std@@@std@@QAEPAEPAE0@Z
EraseFn_0040d470 g_erase_0040d470 = &Vec_0040d290::erase;

// std::vector<unsigned char>::_Destroy(first, last): empty for a trivial
// element type. 0x40b390 runs it from the inlined destructors of the vectors
// at +0x8d and +0x9d with ecx set to each.
typedef void (Vec_0040d290::*DestroyFn_0040d4a0)(Vec_0040d290::iterator, Vec_0040d290::iterator);

struct Access_0040d290 : Vec_0040d290 {
    static DestroyFn_0040d4a0 destroy;
};

// FUNCTION: 0x40d4a0 ?_Destroy@?$vector@EV?$allocator@E@std@@@std@@IAEXPAE0@Z
DestroyFn_0040d4a0 Access_0040d290::destroy = &Access_0040d290::_Destroy;

// The protected members of std::vector<Elem_0040d4f0>: _Destroy (empty, since
// the element type is trivial), _Ucopy and _Ufill. The vector's resize
// (0x40c600) calls them with ecx set to the vector at +0xad.
typedef void (Vec_0040c600::*DestroyFn_0040d4b0)(Vec_0040c600::iterator, Vec_0040c600::iterator);
typedef Vec_0040c600::iterator (Vec_0040c600::*UcopyFn_0040d4f0)(
    Vec_0040c600::const_iterator, Vec_0040c600::const_iterator, Vec_0040c600::iterator);
typedef void (Vec_0040c600::*UfillFn_0040d520)(
    Vec_0040c600::iterator, Vec_0040c600::size_type, const Elem_0040d4f0&);

struct Access_0040c600 : Vec_0040c600 {
    static DestroyFn_0040d4b0 destroy;
    static UcopyFn_0040d4f0 ucopy;
    static UfillFn_0040d520 ufill;
};

// FUNCTION: 0x40d4b0 ?_Destroy@?$vector@UElem_0040d4f0@@V?$allocator@UElem_0040d4f0@@@std@@@std@@IAEXPAUElem_0040d4f0@@0@Z
DestroyFn_0040d4b0 Access_0040c600::destroy = &Access_0040c600::_Destroy;

// std::vector<Elem_0040d550>::size() (`>> 2`: a 4-byte element). Its caller
// 0x40c7f0 is the vector's resize(), and 0x40d4e0, 0x40d550 and 0x40d580 are
// more of its members.
typedef Vec_0040c7f0::size_type (Vec_0040c7f0::*SizeFn_0040d4c0)() const;
// FUNCTION: 0x40d4c0 ?size@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@QBEIXZ
SizeFn_0040d4c0 g_size_0040d4c0 = &Vec_0040c7f0::size;

// The protected members of std::vector<Elem_0040d550>: _Destroy (empty, since
// the element type is trivial), _Ucopy (for a 4-byte element type) and
// _Ufill. The vector's resize (0x40c7f0) calls them with ecx set to it.
typedef void (Vec_0040c7f0::*DestroyFn_0040d4e0)(Vec_0040c7f0::iterator, Vec_0040c7f0::iterator);
typedef Vec_0040c7f0::iterator (Vec_0040c7f0::*UcopyFn_0040d550)(
    Vec_0040c7f0::const_iterator, Vec_0040c7f0::const_iterator, Vec_0040c7f0::iterator);
typedef void (Vec_0040c7f0::*UfillFn_0040d580)(
    Vec_0040c7f0::iterator, Vec_0040c7f0::size_type, const Elem_0040d550&);

struct Access_0040c7f0 : Vec_0040c7f0 {
    static DestroyFn_0040d4e0 destroy;
    static UcopyFn_0040d550 ucopy;
    static UfillFn_0040d580 ufill;
};

// FUNCTION: 0x40d4e0 ?_Destroy@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@IAEXPAUElem_0040d550@@0@Z
DestroyFn_0040d4e0 Access_0040c7f0::destroy = &Access_0040c7f0::_Destroy;

// FUNCTION: 0x40d4f0 ?_Ucopy@?$vector@UElem_0040d4f0@@V?$allocator@UElem_0040d4f0@@@std@@@std@@IAEPAUElem_0040d4f0@@PBU3@0PAU3@@Z
UcopyFn_0040d4f0 Access_0040c600::ucopy = &Access_0040c600::_Ucopy;

// FUNCTION: 0x40d520 ?_Ufill@?$vector@UElem_0040d4f0@@V?$allocator@UElem_0040d4f0@@@std@@@std@@IAEXPAUElem_0040d4f0@@IABU3@@Z
UfillFn_0040d520 Access_0040c600::ufill = &Access_0040c600::_Ufill;

// FUNCTION: 0x40d550 ?_Ucopy@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@IAEPAUElem_0040d550@@PBU3@0PAU3@@Z
UcopyFn_0040d550 Access_0040c7f0::ucopy = &Access_0040c7f0::_Ucopy;

// FUNCTION: 0x40d580 ?_Ufill@?$vector@UElem_0040d550@@V?$allocator@UElem_0040d550@@@std@@@std@@IAEXPAUElem_0040d550@@IABU3@@Z
UfillFn_0040d580 Access_0040c7f0::ufill = &Access_0040c7f0::_Ufill;

// FUNCTION: 0x40d5b0 ?_Ufill@?$vector@UElem_0040cc40@@V?$allocator@UElem_0040cc40@@@std@@@std@@IAEXPAUElem_0040cc40@@IABU3@@Z
UfillFn_0040d5b0 Access_0040c580::ufill = &Access_0040c580::_Ufill;
