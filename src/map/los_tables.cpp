// Decompiled by Opus, Haiku, Sonnet, deepseek-v4.1-flash, space-bunny-free, DeepSeek V4.1 Flash, Claude Opus 5.5, deepseek-v4.1, mimo-v2.6-pro and Space Bunny Free. Names are provisional.
// The line-of-sight tables loaded from gamedata\los.tdf (numtables /
// TABLEINFO / numlines / TABLE%d): the LosTables object at 0x51e6a0, its
// table and line vectors, and the STL instantiations that build them, from
// vector<Elem_00434020> (a 4-byte line point) up through the vector of
// vectors of Elem_00434360 (a struct holding one such vector): the size,
// insert, erase, _Destroy, _Ucopy, allocator, copy-constructor and
// assignment members the table code uses, each emitted out of line by taking
// an address.
// Six of the module's functions stay in files of their own: 0x433130
// (los_tables_433130.cpp) needs a hand-written std::vector so that insert and
// erase stay out of line, which the real <vector> here would redefine; 0x433270
// (los_tables_433270.cpp) needs its Wrap_00433270 element view so that the
// innermost _Destroy/deallocate calls stay out of line; 0x433a80, 0x434020 and
// 0x4344e0 (los_tables_433a80.cpp) need std::_Destroy and std::_Construct
// overloads that call the out-of-line DestroyPoint and StoreDwordIfDst; and
// 0x434360 (los_tables_434360.cpp) needs the same _Destroy name for the element
// type to call DestroyLine. Those overloads disagree with the plain template
// this file's functions need. 0x432d40 and 0x433500 have joined this file, at
// the end and out of address order: their own bodies' symbol ids would
// otherwise move other functions' register windows (docs/c2-regalloc.md), and
// GetLosTable's body before SortUnitTypes puts SortUnitTypes on its window.
#include <windows.h>
#include <algorithm>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "los_tables.h"

// The reference-counted string handle: a pointer to the characters with the
// reference count in the int just before them. The views in this file are the
// copy constructor 0x4c91a0, the assignment 0x4c93b0, ReleaseRef 0x4c9390 and
// the scalar deleting destructors 0x432c00 and 0x432c20.
class StringRef {
public:
    char* ptr;                         // +0x0

    StringRef();
    StringRef(const StringRef& other);
    StringRef(const char* text);
    StringRef(const char* text, int len);
    StringRef& operator=(const StringRef& other);
    void ReleaseRef();
    StringRef* Assign(StringRef* param_1);
    StringRef* Append(const StringRef& other);
    StringRef* MakeLower();
    StringRef* MakeUpper();
    StringRef* AssignText(const char* text);
    char* GetUnique();
    int IsEmpty() const;
    StringRef SubString(int start, int end) const;
    void* FUN_00432c00(unsigned char param_1);
    void* FUN_00432c20(unsigned char param_1);
};

struct Elem_00432be0 {
    char* data;                        // +0x0

    ~Elem_00432be0() { ((StringRef*)this)->ReleaseRef(); }
};

struct Entry_00432cf0 {
    StringRef name;         // +0x0
    int value;                 // +0x4

    void* AssignPair(Entry_00432cf0* param);
};

// An 8-byte element: the string handle and the int after it, the same pair as
// Entry_00432cf0.
struct Elem_432cb0 {
    StringRef handle;               // +0x0
    int field_4;                       // +0x4
};

#pragma pack(push, 1)
#include "../units/unit_def.h"
#pragma pack(pop)

typedef int (__stdcall* Compare)(const UnitDef&, const UnitDef&);

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Column_00433270;

// Keeps its own view of the parser classes: with the shared tdf.h LoadLosTable
// (0x433380), GetLosLine (0x4335e0) and SortUnitTypes (0x432d40) drop.
class TdfRecord {
public:
    int GetFieldInt(const char* name, int def);
    int GetFieldString(char* dst, char* key, int size, char* def);
};

class TdfFile {
public:
    void* root;                        // +0x0
    TdfRecord* current;                // +0x4

    int SelectRecord(char* name);
    void ResetCurrentRecord();
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    int LoadFile(char* path);
    void StripComments(char* text);
    int SelectRecordAt(int index);
    void LoadBuffer(char* buffer, int size, int flags, char* name);
};

typedef std::vector<Elem_00434020> Vec_004336f0;

class LosLine : public Vec_004336f0 {
public:
    int GetLosLineStepCount();
    void GetLosLineStep(short index, unsigned short* out1, unsigned short* out2);
    void LoadLosLine(TdfFile* obj, short line, short mode);
};

// One table: a vector of lines (see 0x4335f0 below).
class LosTable : public std::vector<Elem_00434360> {
public:
    // Inline copy of GetLosLine.
    Elem_00434360* GetLine(short i) { return &(*this)[i]; }
    // Inline copy of ResizeLines.
    void SetNumLines(short n) { resize(n * 4); }

    int GetLosLineCount();
    void ResizeLines(short n);
    LosLine* GetLosLine(short i);
    void FreeLines();
};


typedef std::vector<Elem_00434020> Inner_00433500;

struct Elem_004336c0 {
    int value;                         // +0x0
};

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
void CopyDwordIfNonNull(int*, int*);
struct ApproachRadius { int GetType(); };
struct RingApproach { int GetType(); };
struct PointMarker { int GetType(); };
int FUN_00490200();
int GetBuildRating(int, unsigned short);

extern void __cdecl operator delete(void*);

// std::vector<Elem_00432be0>::~vector() (its size() is 0x432be0 below). Each
// element is a reference-counted string handle released by ReleaseRef. The
// original calls it on a local vector in 0x42a8d0.
typedef std::vector<Elem_00432be0> Inner_00432ba0;
typedef std::vector<Inner_00432ba0> Outer_00432ba0;
typedef Outer_00432ba0& (Outer_00432ba0::*AssignFn_00432ba0)(const Outer_00432ba0&);

// Taking this operator='s address makes the destructor get emitted out of line.
AssignFn_00432ba0 g_assign_00432ba0 = &Outer_00432ba0::operator=;

// FUNCTION: 0x432ba0 ??1?$vector@UElem_00432be0@@V?$allocator@UElem_00432be0@@@std@@@std@@QAE@XZ

// std::vector<T>::size() from MSVC 5's <vector>, out of line, for a vector of
// 4-byte string handles (its destructor is 0x432ba0 above). Taking the
// member's address makes the compiler emit the template instantiation out of
// line.
typedef std::vector<Elem_00432be0> Vec_00432be0;
typedef Vec_00432be0::size_type (Vec_00432be0::*SizeFn_00432be0)() const;

// FUNCTION: 0x432be0 ?size@?$vector@UElem_00432be0@@V?$allocator@UElem_00432be0@@@std@@@std@@QBEIXZ
SizeFn_00432be0 g_size_00432be0 = &Vec_00432be0::size;

// FUNCTION: 0x432c00
void* StringRef::FUN_00432c00(unsigned char param_1)
{
    ReleaseRef();
    if ((param_1 & 1) != 0) {
        operator delete(this);
    }
    return this;
}

// Same shape as the matched sibling 0x432c00 above: a scalar deleting
// destructor that calls the reference-count release at 0x4c9390 (already
// named StringRef::ReleaseRef in data/symbols.csv and called as a plain
// method by every other caller) then conditionally frees this.
// FUNCTION: 0x432c20
void* StringRef::FUN_00432c20(unsigned char param_1)
{
    ReleaseRef();
    if ((param_1 & 1) != 0) {
        operator delete(this);
    }
    return this;
}

// std::vector<Entry_00432cf0>::_Ucopy(first, last, dest): copy-constructs
// {string handle, int} pairs into raw storage (the placement-new construct of
// 0x432cf0 below). Called with ecx set to the vector.
typedef std::vector<Entry_00432cf0> Vec_00432c40;
typedef Vec_00432c40::iterator (Vec_00432c40::*UcopyFn_00432c40)(
    Vec_00432c40::const_iterator, Vec_00432c40::const_iterator, Vec_00432c40::iterator);

// _Ucopy is protected: a derived class takes its address to get it emitted.
struct Access_00432c40 : Vec_00432c40 {
    static UcopyFn_00432c40 fn;
};

// FUNCTION: 0x432c40 ?_Ucopy@?$vector@UEntry_00432cf0@@V?$allocator@UEntry_00432cf0@@@std@@@std@@IAEPAUEntry_00432cf0@@PBU3@0PAU3@@Z
UcopyFn_00432c40 Access_00432c40::fn = &Access_00432c40::_Ucopy;

// std::fill over an array of 8-byte string handles (compare the
// copy_backward at 0x432cb0 below): assigns the handle through its assignment
// operator 0x4c93b0 and copies its field_4. The function was compiled with
// __stdcall as the default, hence `ret 0xc`.
// FUNCTION: 0x432c80
void __stdcall AssignRange(Elem_432cb0* first, Elem_432cb0* last, Elem_432cb0* value)
{
    for (; first != last; ++first) {
        first->handle.Assign(&value->handle);
        first->field_4 = value->field_4;
    }
}

// FUNCTION: 0x432cb0
Elem_432cb0* __stdcall AssignRangeBack(Elem_432cb0* param_1, Elem_432cb0* param_2, Elem_432cb0* param_3)
{
    while (param_1 != param_2) {
        --param_2;
        --param_3;
        param_3->handle.Assign(&param_2->handle);
        param_3->field_4 = param_2->field_4;
    }
    return param_3;
}

// std::allocator<Entry_00432cf0>::construct: placement-new copy of a string
// handle plus an int. Taking the member's address makes the compiler emit the
// template instantiation out of line.
typedef std::allocator<Entry_00432cf0> Alloc_00432cf0;
typedef void (Alloc_00432cf0::*ConstructFn_00432cf0)(Entry_00432cf0*, const Entry_00432cf0&);

// FUNCTION: 0x432cf0 ?construct@?$allocator@UEntry_00432cf0@@@std@@QAEXPAUEntry_00432cf0@@ABU3@@Z
ConstructFn_00432cf0 g_construct_00432cf0 = &Alloc_00432cf0::construct;

// A reference-counted string handle, with the count in the dword before the
// characters, and an int after it.
// FUNCTION: 0x432d20
void* Entry_00432cf0::AssignPair(Entry_00432cf0* param)
{
    name.Assign(&param->name);
    value = param->value;
    return this;
}

// The original calls this from 0x432d20 above rather than inlining it.
#pragma auto_inline(off)
// FUNCTION: 0x4c93b0
StringRef* StringRef::Assign(StringRef* param_1)
{
    *(int*)(param_1->ptr - 4) += 1;
    *(int*)(ptr - 4) -= 1;
    if (*(int*)(ptr - 4) == 0) {
        free(ptr - 4);
    }
    ptr = param_1->ptr;
    return this;
}
#pragma auto_inline(on)

// Element type: 0x249 bytes with a name string at +0x20 (see 0x42db60.cpp).
static UnitDef* __inline Copy_backward(UnitDef* F, UnitDef* L, UnitDef* X)
{
    while (F != L)
        *--X = *--L;
    return X;
}

static void __inline Unguarded_insert(UnitDef* L, UnitDef V, Compare P)
{
    for (UnitDef* M = L; P(V, *--M); L = M)
        *L = *M;
    *L = V;
}

// FUNCTION: 0x432fb0
void __stdcall InsertionSortUnitTypes(UnitDef* first, UnitDef* last, Compare comp, void* tag)
{
    if (first != last)
        for (UnitDef* M = first; ++M != last; ) {
            UnitDef V = *M;
            if (!comp(V, *first))
                Unguarded_insert(M, V, comp);
            else {
                Copy_backward(first, M, M + 1);
                *first = V;
            }
        }
}

// The out-of-line destructor body of the global at 0x51e6a0 (see
// 0x4814c0.cpp and 0x4814f0.cpp): a std::vector of std::vector<Elem_00434360>,
// where Elem_00434360 is a struct holding one std::vector<Elem_00434020>. The
// Elem_00434360 elements are destroyed through allocator::destroy (0x434400).
typedef std::vector<Column_00433270> Outer_004330b0;

// FUNCTION: 0x4330b0
void LosTables::FreeTables()
{
    ((Outer_004330b0*)this)->~vector();
}

// The object at +0 is a std::vector<Column_00433270>, where the column is a
// std::vector<Elem_00434360> and Elem_00434360 is a struct holding one
// std::vector<Elem_00434020> (see docs/consolidation.md, STL instantiations):
// its insert is 0x4340f0, the column's operator= 0x434770 and destructor
// 0x433a80, and the held vector's _Destroy 0x433d90 and deallocate 0x433da0.
//
// Loads table number `table` (0-based) of gamedata\los.tdf: builds the
// section name "TABLE%d" (table + 1), rewinds the TDF reader, finds the
// section and, if it is there, resizes the table to numlines * 4 lines and
// has each line read itself (LoadLosLine) from the four quarter blocks
// (line i, numlines + i, 2 * numlines + i, 3 * numlines + i).
// Header set matters: fewer headers change the table index's register choice.
// Inline copy of the table accessor: table number n, counted from 1. Its
// inline body is load-bearing, SortUnitTypes matches only with it present
// (docs/c2-regalloc.md).
static inline LosTable* GetTable(LosTables* self, short n)
{
    n--;
    return (LosTable*)&(*(Outer_004330b0*)self)[n];
}

// FUNCTION: 0x433380
void LosTables::LoadLosTable(TdfFile* file, short table)
{
    char name[32];
    sprintf(name, "TABLE%d", table + 1);
    file->ResetCurrentRecord();
    if (file->SelectRecord(name)) {
        // The table vector at +0: table number n, counted from 1, so the
        // caller's table + 1 lands on element table.
        LosTable* t = GetTable(this, table + 1);
        short numlines = (short)file->current->GetFieldInt("numlines", 0);
        t->SetNumLines(numlines);
        // Short locals before the loop: match the original induction variables.
        short n2 = numlines * 2;
        short n3 = numlines * 3;
        for (short i = 0; i < numlines; i++) {
            ((LosLine*)t->GetLine(i))->LoadLosLine(file, i, 0);
            ((LosLine*)t->GetLine(numlines + i))->LoadLosLine(file, i, 1);
            ((LosLine*)t->GetLine(n2 + i))->LoadLosLine(file, i, 2);
            ((LosLine*)t->GetLine(n3 + i))->LoadLosLine(file, i, 3);
        }
    }
}

// FUNCTION: 0x433520
int LosTables::GetLosTableCount()
{
    return ((Outer_004330b0*)this)->size();
}

// std::vector<std::vector<Elem_00434020> >::~vector(): each inner vector's
// elements are destroyed through allocator::destroy (empty for a trivial
// element), then its _First is freed and its three pointers zeroed; finally
// the outer _First is freed and zeroed.
typedef std::vector<Elem_00434020> Inner_00434020;
typedef std::vector<Inner_00434020> Outer_00434020;
typedef void (std::allocator<Elem_00434020>::*DestroyFn_00434020)(Elem_00434020*);

// Taking this address makes allocator::destroy get emitted out of line.
DestroyFn_00434020 g_destroy_00434020 = &std::allocator<Elem_00434020>::destroy;

// FUNCTION: 0x433540
void LosTable::FreeLines()
{
    // Explicit destructor call from a method, as in 0x4330b0 above.
    ((Outer_00434020*)this)->~vector();
}

// FUNCTION: 0x4335c0
int LosTable::GetLosLineCount()
{
    return size();
}

// Resizes the table vector at +0 (its _First at +4) to the argument times 4
// (the number of lines), filling with a default Elem_00434360, a struct
// holding one std::vector<Elem_00434020> (see docs/consolidation.md). Used by
// the table code built by 0x433130/0x433380.
//
// The sibling 0x433270 is the same wrapper for the next element level up
// (`vector<vector<Elem_00434360>>`), where `resize(n, x)` has no `* 4`.
// FUNCTION: 0x4335f0
void LosTable::ResizeLines(short n)
{
    Elem_00434360 x;
    resize(n * 4, x);
}

// FUNCTION: 0x4335e0
LosLine* LosTable::GetLosLine(short param_1)
{
    return (LosLine*)&(*this)[param_1];
}

// std::vector<T>::~vector() from MSVC 5's <vector>, out of line, for a
// trivial element type: byte-identical to 0x433a30 (vector<Elem_00434020>),
// but a separate instantiation with no callers, so the element type is not
// known. Emitted the way 0x433a30 emits its copy.
typedef std::vector<Elem_004336c0> Inner_004336c0;
typedef std::vector<Inner_004336c0> Outer_004336c0;
typedef Outer_004336c0& (Outer_004336c0::*AssignFn_004336c0)(const Outer_004336c0&);

AssignFn_004336c0 g_assign_004336c0 = &Outer_004336c0::operator=;

// FUNCTION: 0x4336c0 ??1?$vector@UElem_004336c0@@V?$allocator@UElem_004336c0@@@std@@@std@@QAE@XZ

extern char DAT_005119b8[];

// FUNCTION: 0x4336f0
void LosLine::LoadLosLine(TdfFile* obj, short line, short mode)
{
    char name[32];
    char buf[0x200];

    sprintf(name, "line%d", line + 1);
    if (obj->current->GetFieldString(buf, name, 0x200, DAT_005119b8) != 0) {
        char* tok = strtok(buf, ", ");
        if (tok == 0)
            return;
        short n = atoi(tok);
        Elem_00434020 x;
        resize(n, x);
        // short counter: gives the original's countdown loop.
        short i;
        switch (mode) {
        case 0:
            // strtok called inside atoi: the element address is loaded after the call.
            for (i = 0; i < n; i++) {
                (*this)[i].a = atoi(tok = strtok(0, ", "));
                (*this)[i].b = -atoi(tok = strtok(0, ", "));
            }
            break;
        case 1:
            for (i = 0; i < n; i++) {
                (*this)[i].b = atoi(tok = strtok(0, ", "));
                (*this)[i].a = atoi(tok = strtok(0, ", "));
            }
            break;
        case 2:
            for (i = 0; i < n; i++) {
                (*this)[i].a = -atoi(tok = strtok(0, ", "));
                (*this)[i].b = atoi(tok = strtok(0, ", "));
            }
            break;
        case 3:
            for (i = 0; i < n; i++) {
                (*this)[i].b = -atoi(tok = strtok(0, ", "));
                (*this)[i].a = -atoi(tok = strtok(0, ", "));
            }
            break;
        }
    } else {
        Elem_00434020 x;
        resize(0, x);
    }
}

// FUNCTION: 0x4339c0
int LosLine::GetLosLineStepCount()
{
    return size();
}

// FUNCTION: 0x4339e0
void LosLine::GetLosLineStep(short index, unsigned short* out1, unsigned short* out2)
{
    int idx = index;
    unsigned char* base = (unsigned char*)*(void**)((char*)this + 4);
    unsigned char* ptr = base + idx * 4;
    unsigned short val1 = *(unsigned short*)ptr;
    *out1 = val1;
    unsigned short val2 = *(unsigned short*)(ptr + 2);
    *out2 = val2;
}

// std::vector<Elem_00434020>::vector(const allocator&): copies the empty
// allocator byte and zeroes _First, _Last and _End. It is also the default
// constructor (the allocator is a default argument). Its caller 0x433380
// builds the fill value of a vector<Elem_00434360> resize with it (the held
// vector's destructor is 0x433a30).
bool operator==(const Elem_00434020&, const Elem_00434020&);
bool operator<(const Elem_00434020&, const Elem_00434020&);

// A constructor's address can't be taken: explicit instantiation emits it, and
// needs the comparison operators above.
template class std::vector<Elem_00434020>;

// FUNCTION: 0x433a10 ??0?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAE@ABV?$allocator@UElem_00434020@@@1@@Z

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
void __stdcall DestroyPoint(int)
{
}

// std::_Destroy(Elem_00434360*) from MSVC 5's <xmemory>, where Elem_00434360
// is a struct holding one std::vector<Elem_00434020>: runs the element's
// destructor, which is the held vector's. Its caller is the destroy loop of
// 0x434360.
// Explicit __stdcall: the original takes no ecx and ends in `ret 4`.
// FUNCTION: 0x434440
void __stdcall DestroyLine(Elem_00434360* p)
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
void __stdcall StoreDwordIfDst(int* param_1, int* param_2)
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

// The two functions below are kept after 0x51e6a0's methods and out of address
// order: defined at their addresses their bodies' symbol ids would move other
// functions' register windows (docs/c2-regalloc.md), and SortUnitTypes needs
// GetLosTable's body, declared here just before it, to land on its window.
// Returns the address of element n - 1 of the table vector held at +0 (the
// global at 0x51e6a0). The index is narrowed to a short before indexing.
// FUNCTION: 0x433500
Inner_00433500* LosTables::GetLosTable(int n)
{
    return (Inner_00433500*)&(*(Outer_004330b0*)this)[(short)(n - 1)];
}

// Unused here: the symbol ids these declarations take keep SortUnitTypes on
// its register window (docs/c2-regalloc.md).
void StepAllGafSequences(void);

// 585-byte GUI list entry (same class as 0x432fb0 below). The 4th parameter is
// the template's unused _Ty* tag, passed as 0 and never read.
// FUNCTION: 0x432d40
void __stdcall SortUnitTypes(UnitDef* first, UnitDef* last,
                            Compare comp, int unused)
{
    for (; std::_SORT_MAX < last - first; ) {
        UnitDef* _M = std::_Unguarded_partition(first, last,
            std::_Median(UnitDef(*first),
                UnitDef(*(first + (last - first) / 2)),
                UnitDef(*(last - 1)), comp), comp);
        if (last - _M <= _M - first)
            SortUnitTypes(_M, last, comp, 0), last = _M;
        else
            SortUnitTypes(first, _M, comp, 0), first = _M;
    }
}
