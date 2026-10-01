// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash retry (10 min, 5 check runs): function-scope `_Ps = _P` plus
// `_Xs = _X` aliases, a hand-written prefix copy loop (`for (; _F != _P; ++_F, ++_Q)
// allocator.construct(_Q, *_F);`), a `size_type _Mf = _M;` fill count, and a split
// `iterator _S; ... _S = allocate(...)` declaration ALL compile to the byte-identical
// 795-byte object (each compared instruction for instruction against the baseline
// diff: zero differences), so this TU's codegen for every spelling of the
// reallocating branch is canonical and the esi/edx pick for _P is not reachable
// from the source, as the notes below already concluded.
// deepseek-v4.1-flash (#3287): the real-<vector> recipe of the matched sibling
// 0x43c3a0 (explicit instantiation, 0x44-byte trivial element) scores 82.9%,
// 796 bytes, against this clone's 83.0%, 795; swapping _Destroy and deallocate
// regresses to 76.5%. The retained wall is unchanged: _P in esi instead of edx.
// deepseek-v4.1-flash retry (10 min): no improvement on the retained 83.0%
// (795/794). Tried <windows.h> added (82.9%, 796 bytes), a late `_P3 = _P`
// local for the third copy (83.0%, _P stays in esi), and reusing a cached
// `_S2 = _Q + _M` destination (75.3%). The ceiling remains the allocator
// putting the iterator _P in esi instead of edx in the reallocating branch.
// deepseek-v4.1 retry (#2496): 83.0% confirmed, 10 check runs (declfirst _S/_N, split _Q decl/assign, cached _Q+_M, non-const _Ucopy params, const_iterator bound alias, _QE precompute) all stay at 795 bytes with _P in esi instead of edx; no source-level lever found for the allocator pick.
// Refinement issue #2306: best remains 83.0% (795/794 bytes). The reallocating branch allocates _P in esi instead of edx, shifting spills and copy-loop registers; all other branches match.
// GPT-6 retry: 83.0%, 795 of 794 bytes; pointer and buffer constness and allocator pointer typedef variants did not change the saved register family.
// Sonnet 5.5 retry (#1081): /Gz and /Gr change nothing (it is a method), and about
// 700 more variants (deallocate/_Destroy order and spelling, helper parameter
// orders and loop shapes, manual third-copy loops, size and tail spellings) all
// stay at 83.0%. The same single cause as 0x476490: _P stays in edx and _S in esi.
// std::vector<T>::insert(iterator, size_type, const T&) from MSVC 5's <vector>,
// with _Ucopy, _Ufill, fill and copy_backward inlined. The element is 0x44
// (68) bytes, so every copy is a rep movsd of 0x11 dwords and every stride is
// 0x44; the three pointers sit at this+4, this+8 and this+0xc in the original
// (a class that derives from the vector), which is why the code reads
// [this+4] for _First here.
//
// The class below is a hand-written clone of the <vector> class template rather
// than an include of <vector>, for one measurable reason: <vector> pulls in
// <stdexcept> and its std::string instantiations, and with it this build makes
// the third copy's source pointer with
//     mov eax, edx / sub eax, ebx / add eax, esi / sub eax, ecx     (796 bytes)
// while the hand-written clone makes the same value with one lea:
//     lea eax, [edx + esi] / sub eax, ebx / sub eax, ecx            (795 bytes)
// Nothing else about the class matters: the element type (int[0x11], char[0x44],
// short[0x22], float, double, pointer + ints, a nested struct, a user copy
// constructor), the parameter and member names, the loop shapes of _Ucopy and
// _Ufill, the order of the increments, the header set (including <windows.h>,
// which makes no difference here) and dummy code all give the same result.
//
// Still differs: 83.0%, ours 795 bytes against the original's 794. Every
// difference is inside the reallocating branch and comes from one register
// choice: the original keeps the iterator _P in edx from the first instruction
// after the operator new call to the end of the suffix copy, so nothing has to
// reload it and the fill loop can use ebp as its counter:
//     mov edx, [esp + 0x24]    ; _P                      original
//     mov esi, [esp + 0x24]    ; _P                      this build
//     ...
//     mov ebp, edi             ; fill counter in ebp     original
//     mov ebp, [esp + 0x28]    ; &_X cached in ebp       this build
//     mov edx, edi             ; fill counter in edx     this build
//     ...
//     mov esi, [esp + 0x28]    ; &_X reloaded each turn  original
//     mov esi, ebp             ; &_X from ebp            this build
// Because _P is in esi here, the prefix copy's loop bound has to spill and
// reload it (`mov esi, [esp + 0x20]` inside the loop at 0x475fc4), and the
// third copy then computes its source from the reloaded copy
// (`lea eax, [edx + esi]`) where the original reuses the register it already
// has (`sub edx, ebx` / `add edx, eax` on _P itself). The original also keeps
// the new buffer in two registers (esi and ebx, copied with `mov ebx, esi`)
// and spills _S to [esp+0x14] where this build keeps one register (ebx) and
// spills it to [esp+0x18]; _N takes the other slot in each. Everything after
// the reallocating branch, including both in-place branches, the delete call,
// the three pointer stores, the epilogue and every jump target, is
// byte-identical.
//
// The register choice is not reachable from the source: about 450 variants
// (source spellings, statement orders, the operator new/delete spelling, the
// copy/fill helpers as members or free functions, loop shapes, the size()
// and the free-space test, the header set, the element type) all put _P in
// esi, ecx or ebp. Only a differently shaped branch reaches edx, and then the
// rest of the branch no longer matches.
//
// One more spelling ruled out (space-bunny-free, 66.7%, 849 bytes, so clearly
// worse than the _Q form above): dropping the _Q local for the real <vector>
// wording
//     _Ucopy(_First, _P, _S);
//     _Ufill(_S + size_type(_P - _First), _M, _X);
//     _Ucopy(_P, _Last, _S + _M + size_type(_P - _First));
// gives back a third copy loop bound it recomputes and loses the register
// rotation, so the _Q local really is what the original used.
//
// The only two structural differences left, both inside the first copy loop,
// are that ours rematerialises _P from the argument slot on every turn
// (`mov esi, [esp + 0x20]` inside the loop, the single extra byte) and swaps
// the two spill slots: the original puts _N in [esp+0x18] and _S in [esp+0x14]
// after the argument push is popped, ours puts _N in [esp+0x14] and _S in
// [esp+0x1c]. Since ours re-reads _P instead of keeping it, the allocator
// ranked _P above _S; the original ranks it below every callee-saved register.
// Refinement: a register alias, reference alias, reversed realloc branch, and
// cached free-space local did not improve the retained 83.0% result.
#include <climits>
#include <memory>
#include <xutility>

struct Element_00475ef0 {
    int data[0x11];
};

namespace std {

// MSVC 5's <vector> class template, with only the members insert needs. The
// name and the two template parameters have to match the real ones for the
// member to mangle as ?insert@?$vector@UElement_00475ef0@@...
template<class _Ty, class _A = allocator<_Ty> >
class vector {
public:
    typedef vector<_Ty, _A> _Myt;
    typedef _A allocator_type;
    typedef _A::size_type size_type;
    typedef _A::difference_type difference_type;
    typedef _A::pointer iterator;
    typedef _A::const_pointer const_iterator;
    typedef _A::reference reference;
    typedef _A::const_reference const_reference;
    typedef _Ty value_type;
    vector() : allocator(), _First(0), _Last(0), _End(0) {}
    size_type size() const
        {return (_First == 0 ? 0 : _Last - _First); }
    iterator begin() { return (_First); }
    iterator end() { return (_Last); }
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
protected:
    void _Destroy(iterator _F, iterator _L)
        {for (; _F != _L; ++_F)
            allocator.destroy(_F); }
    iterator _Ucopy(const_iterator _F, const_iterator _L,
        iterator _P)
        {for (; _F != _L; ++_P, ++_F)
            allocator.construct(_P, *_F);
        return (_P); }
    void _Ufill(iterator _F, size_type _N, const _Ty& _X)
        {for (; 0 < _N; --_N, ++_F)
            allocator.construct(_F, _X); }
    _A allocator;
    iterator _First, _Last, _End;
    };

} // namespace std

typedef std::vector<Element_00475ef0> Vec_00475ef0;
typedef void (Vec_00475ef0::*InsertFn_00475ef0)(
    Vec_00475ef0::iterator, Vec_00475ef0::size_type, Element_00475ef0 const&);

// FUNCTION: 0x475ef0 ?insert@?$vector@UElement_00475ef0@@V?$allocator@UElement_00475ef0@@@std@@@std@@QAEXPAUElement_00475ef0@@IABU3@@Z
InsertFn_00475ef0 g_insert_00475ef0 = &Vec_00475ef0::insert;
