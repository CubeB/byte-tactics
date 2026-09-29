// Decompiled by longcat-2.5-preview-free, finished by space-bunny-free and deepseek-v4.1-flash. Names are provisional.
// Class_0046eba0 is a std::vector<Packet_0046cef0>. The toolchain's own
// VECTOR lays vector<_Ty,_A> out as `_A allocator; iterator _First, _Last,
// _End;`, which is exactly the 4-byte allocator at +0 and the three iterators
// at +0x4, +0x8, +0xc seen here. This is its out-of-line
// insert(iterator, size_type, const _Ty&); 0x46e640 is the vector<int>
// instantiation of the same member. The only caller is at 0x46cfd3 and pushes
// (in reverse) `edx`, `1`, `[esi+0x24]`, with ecx = esi+0x1c, i.e.
// insert(end(), 1, x), which fixes the argument order: arg1 = the iterator,
// arg2 = the count, arg3 = the value.
//
// The element type is 0xe bytes. That size is why every pointer difference is
// a REAL signed division (imul 0x92492493; add; sar edx,3; shr eax,0x1f; add)
// and why the byte-scaled multiplies come out as shl 3; sub; shl 1 (a *7 with
// the 2 folded into an addressing-mode scale, e.g. `lea edx,[esi+ecx*2]` at
// 0x46ed45 for _End = _S + _N). A char* model with hand written *14 gets the
// divides right but cannot make an induction variable step by 0xe.
//
// The template's shape is taken from VECTOR and XUTILITY verbatim:
//   * size() is null guarded, and the three size() uses inside
//     `size() + (_M < size() ? size() : _M)` are three separate inlined copies,
//     each with its own `test _First,_First` guard (0x46ebd7, 0x46ebfc,
//     0x46ec21), plus a fourth after the deallocate (0x46ed43).
//   * _Ucopy and _Ufill go through allocator.construct, whose null check is the
//     `test dest,dest` guard at the top of each of those loops. fill and
//     copy_backward are the free templates, which assign directly, so only the
//     _Ucopy/_Ufill loops carry the guard.
//   * copy_backward is XUTILITY's `while (_F != _L) *--_X = *--_L;`, so BOTH
//     decrements are at the TOP of the loop and the rotated test sits between
//     them and the copy: 0x46eee1 sub eax,0xe; 0x46eee4 sub ecx,0xe;
//     0x46eee7 mov edx,eax; 0x46eee9 mov ebx,ecx; 0x46eeeb cmp eax,edi;
//     copy; 0x46ef05 jne. A for(;;_L -= 14) with the decrement in the third
//     clause does NOT produce this and costs about half a point.
//   * `*14` is never written by hand here; the byte step is expressed as
//     _F += 14, and the source pointer of _Ucopy consequently steps by one
//     byte (inc eax) where the original steps by 0xe. See "still differs" 1.
//
// NOT MATCHING yet (check.py: 68.2%, 924 of 936 bytes, started at 67.6%/925).
// What still differs, in the order I would attack it next:
//   1. The _Ucopy source induction variable. Every _Ucopy loop here advances
//      the SOURCE by one byte (`inc eax`) where the original advances it by
//      0xe (`add eax,0xe`), while the destination advances by 0xe in both.
//      Five sites: 0x46ec8c, 0x46ed13, 0x46eea4(+0xec), 0x46eee1(loop head of
//      the third _Ucopy in branch 2) and branch 3's first _Ucopy. Simply
//      writing _F += 14 fixes the instruction but FLIPS the whole register
//      allocation: this goes to 53.2% at 939 bytes with `this` in ebx, the
//      same collapse as before. A 14-byte Packet* field model gives the step
//      for free (sizeof is 14, so ++ is add reg,0xe and the difference
//      divides) but costs a THIRD local dword, sub esp,0xc against the
//      original's sub esp,8: _Q, the new_finish, gets its own stack slot
//      because Packet* costs a register the char* model did not. Forcing _Q
//      away by calling _Ucopy twice duplicates the loop and gives 1002 bytes.
//      So the 14-byte model is right in principle and one local too fat.
//   2. The new length is spilled as the ELEMENT COUNT, before the clamp. The
//      original does `lea eax,[edx+esi]; test eax,eax; mov [esp+0x20],eax;
//      jge; xor eax,eax; mov edx,eax; shl edx,3; sub edx,eax; shl edx,1;
//      push edx; call new`, so _N lands in the now-dead _M argument home at
//      its DEFINITION, before `if (_N < 0) _N = 0`, and is reloaded after the
//      deallocate for _End = _S + _N*14 (0x46ed2e), re-multiplying as
//      shl ecx,3; sub ecx,eax with the 2 in the lea scale. This file instead
//      keeps the value in a register and spills the PRODUCT _N*14 after the
//      push, so _End reuses it in a plain lea. The multiply is not CSE'd in
//      the original, so the two are not the same expression to MSVC; a
//      by-const-reference helper that takes the length's address (which
//      forces a stack home) changes nothing measurable, 68.2%/924.
//      This single difference is the keystone: it moves operator new's result
//      from edx to ecx, which swaps the _Ucopy source and destination
//      registers through the whole realloc path, makes the original reload
//      _M_last + _M*14 with a read-modify-write (0x46ee6f) where this file
//      gets `add dword ptr [esi+8], ebp`, and adds three redundant
//      `mov edi,[esp+0x20]` reloads.
//   3. Register selection in the else-if (realloc-in-place) path. The original
//      holds _M*14 in esi and spills it to [esp+0x1c] at once
//      (0x46eda9-0x46edb4), leaving ebp free as the copy scratch and the
//      _Ufill counter in ecx; this file puts _M*14 in ebp, reloads it inside
//      the copy loop (0x46ede1 region) and has to spill the _Ufill counter to
//      [esp+0x24] and write it back every iteration.
//   4. The original stores _M_first to the dead third argument home right
//      before the deallocate (0x46ed25 `mov [esp+0x28],eax`, a store this
//      file does not emit at all). MSVC keeps it, so it is not a bug fix to
//      make; it is another symptom of 2.
//   5. One extra /14 hoist order: at 0x46edf3 the original emits
//      sar edx,3; mov ecx,edx; shr ecx,0x1f; add edx,ecx AFTER `mov eax,esi`
//      while this file emits it before. Pure scheduling.
//
// A scratch variant (build/scratch/0x46eba0/v10.cpp) declares _Ucopy
// result-first, _Ucopy(result, first, last), which lands on EXACTLY 936 bytes
// (67.1%, 239 diff lines against this file's 230), so the declaration order of
// _Ucopy is a live axis, but it is not by itself the answer.
//
// deepseek-v4.1-flash pass (#1455): the whole first hunk up to 0x46ec58 (the
// operator new call) can be matched, store and product register included, by
// routing the allocation through a member function. A 4-byte nested
// `struct allocator_type { char _pad[4]; Packet* allocate(size_type, const
// void*); }` as the +0 member, with allocate inlining the `if (_N < 0) _N = 0;`
// clamp and `::operator new(_N * 14)`, makes MSVC emit
// `mov [esp+0x20],eax` (the _M argument home) and the product in edx, and the
// first _Ucopy then advances the SOURCE by 0xe. build/scratch/0x46eba0/v1.cpp
// (identical with a free static _Allocate helper) is 944 bytes / 67.1%:
// everything through 0x46ec72 matches, but the reload of _P after new lands in
// ebp where the original uses esi and _S lands in esi where the original uses
// edx, shifting every later register and adding 8 bytes. Calling the free
// _Allocate directly without the 4-byte allocator member (v2.cpp, 933 bytes /
// 67.0%) loses the match: the store goes to a fresh [esp+0x14] and the source
// step is back to `inc eax`. So the allocator member is what forces the arg2
// home; the remaining problem is downstream register allocation in the realloc
// path, not the early expression shape.
#include <memory>
#include <xutility>

#pragma pack(push, 1)
struct Packet_0046cef0 {         // 0xe bytes
    unsigned char type;          // +0x0
    unsigned char arg;           // +0x1
    unsigned int id;             // +0x2
    int field_6;                 // +0x6
    int field_a;                 // +0xa
};
#pragma pack(pop)

class Class_0046eba0 {
public:
    typedef std::allocator<Packet_0046cef0> allocator_type;
    typedef unsigned int size_type;

    char unknown_0[4];           // the allocator, +0x0
    char* first;                 // +0x4
    char* last;                  // +0x8
    char* end;                   // +0xc
    allocator_type alloc;

    void FUN_0046eba0(char* _P, unsigned int _M, const Packet_0046cef0* _X);

    size_type size() { return (first == 0 ? 0 : (last - first) / 14); }
    char* _Ucopy(char* _F, char* _L, char* _P)
    {
        for (; _F != _L; ++_F, _P += 14)
            alloc.construct((Packet_0046cef0*)_P, *(Packet_0046cef0*)_F);
        return _P;
    }
    void _Ufill(char* _F, size_type _N, const Packet_0046cef0* _X)
    {
        for (; 0 < _N; --_N, _F += 14)
            alloc.construct((Packet_0046cef0*)_F, *_X);
    }
    static void fill(char* _F, char* _L, const Packet_0046cef0* _X)
    {
        for (; _F != _L; _F += 14)
            *(Packet_0046cef0*)_F = *_X;
    }
    static void copy_backward(char* _F, char* _L, char* _P)
    {
        // XUTILITY's copy_backward is `while (_F != _L) *--_X = *--_L;`: both
        // decrements at the top of the loop, the rotated test between them
        // and the copy.
        while (_F != _L) {
            _L -= 14;
            _P -= 14;
            *(Packet_0046cef0*)_P = *(Packet_0046cef0*)_L;
        }
    }
};

// FUNCTION: 0x46eba0
void Class_0046eba0::FUN_0046eba0(char* _P, unsigned int _M, const Packet_0046cef0* _X)
{
    if ((end - last) / 14 < _M)
    {
        int _N = size() + (_M < size() ? size() : _M);
        if (_N < 0)
            _N = 0;
        char* _S = (char*)::operator new(_N * 14);
        char* _Q = _Ucopy(first, _P, _S);
        _Ufill(_Q, _M, _X);
        _Ucopy(_P, last, _Q + _M * 14);
        ::operator delete(first);
        end = _S + _N * 14;
        last = _S + (size() + _M) * 14;
        first = _S;
    }
    else if ((last - _P) / 14 < _M)
    {
        _Ucopy(_P, last, _P + _M * 14);
        _Ufill(last, _M - (last - _P) / 14, _X);
        fill(_P, last, _X);
        last += _M * 14;
    }
    else if (0 < _M)
    {
        _Ucopy(last - _M * 14, last, last);
        copy_backward(_P, last - _M * 14, last);
        fill(_P, _P + _M * 14, _X);
        last += _M * 14;
    }
}
