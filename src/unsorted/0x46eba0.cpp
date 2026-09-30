// Decompiled by longcat-2.5-preview-free, finished by space-bunny-free and
// deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free.
// Names are provisional. PARTIAL 81.2%, 924 of 936 bytes. This is MSVC 5's
// std::vector<Packet_0046cef0>::insert(iterator, size_type, const T&), the same
// template as the near-matched 0x476210 (99.6% there, 32-byte element), taken
// out of line by taking the member's address in that file's translation unit.
// The empty allocator member is one byte, so the three pointers are at +4, +8
// and +0xc, which is the layout the original reads. Keep the allocator at
// offset zero, use its allocate/deallocate, and preserve the capacity while the
// allocator clamps its own signed allocation count. The destination-first
// _Ucopy(_P, _F, _L) improves the register family.
//
// What is still different, and the two experiments that bracket it:
// 1. The reallocating branch's tail copy is the source-first
// _Ucopy(_P, _Last, _Q + _M) of the real <vector>, not the destination-first
// form above. That spelling builds the 12 bytes this build is missing, the
// `(_P + dest) - _Q - (_M * 14)` tree at 0x46eceb (sub, add, sub, and the
// mov eax, esi after it), which the 0x476210 notes already record for the
// 32-byte case. But spelling it that way here scores worse, not better: with
// 0x476210's std::vector clone class and its whole insert body verbatim the
// build is 939 bytes at 63.3%, and with the destination-first head and middle
// copies plus a source-first tail it is 937 bytes at 73.0%, against 81.2% for
// this file. Both are the right SIZE (937 or 939 against 936) and the wrong
// registers, so the tail copy spelling is not what decides the score here.
// 2. The register family hangs off one decision: the original puts `this` in
// ebp, spills it to the frame slot [esp+0x10] at 0x46ebbe and then reuses ebp
// as the 14-byte copy's data temp, which frees ebx for _M. This build puts
// `this` in ebx, so ebp holds _M for the whole function, the head copy's data
// temp lands in edi instead of ebp, the head copy's advancing destination lands
// in esi instead of edx and the tail copy's loop bound lands in edx instead of
// edi. Every other difference in the first hunk follows from that swap.
// 3. The two live values are also swapped between homes: the original spills
// the new capacity _N into the dead arg2 slot at 0x46ec41 and keeps the new
// buffer in the frame slot [esp+0x18] (0x46ec5c), while this build does the
// reverse, which is the only difference in that hunk apart from the registers.
// 4. The original also spills the deallocate argument into the dead arg3 slot
// at 0x46ed25 (`push eax; mov [esp+0x28], eax; call`), six bytes this build
// does not have, and that store is never read back.
#include <memory>
#include <xutility>

#pragma pack(push, 1)
struct Packet_0046cef0 { // 0xe bytes
    unsigned char type;  // +0x0
    unsigned char arg;   // +0x1
    unsigned int id;     // +0x2
    int field_6;         // +0x6
    int field_a;         // +0xa
};
#pragma pack(pop)

class Class_0046eba0 {
  public:
    typedef std::allocator<Packet_0046cef0> allocator_type;
    typedef unsigned int size_type;

    allocator_type alloc;
    Packet_0046cef0* first; // +0x4
    Packet_0046cef0* last;  // +0x8
    Packet_0046cef0* end;   // +0xc

    void FUN_0046eba0(Packet_0046cef0* _P, unsigned int _M, const Packet_0046cef0& _X);

    size_type size() { return (first == 0 ? 0 : last - first); }
    Packet_0046cef0* _Ucopy(Packet_0046cef0* _P, Packet_0046cef0* _F, Packet_0046cef0* _L) {
        for (; _F != _L; ++_F, ++_P)
            alloc.construct((Packet_0046cef0*)_P, *(Packet_0046cef0*)_F);
        return _P;
    }
    void _Ufill(Packet_0046cef0* _F, size_type _N, const Packet_0046cef0& _X) {
        for (; 0 < _N; --_N, ++_F)
            alloc.construct((Packet_0046cef0*)_F, _X);
    }
    static void fill(Packet_0046cef0* _F, Packet_0046cef0* _L, const Packet_0046cef0& _X) {
        for (; _F != _L; ++_F)
            *(Packet_0046cef0*)_F = _X;
    }
    static void copy_backward(Packet_0046cef0* _F, Packet_0046cef0* _L, Packet_0046cef0* _P) {
        // XUTILITY's copy_backward is `while (_F != _L) *--_X = *--_L;`: both
        // decrements at the top of the loop, the rotated test between them
        // and the copy.
        while (_F != _L) {
            --_L;
            --_P;
            *(Packet_0046cef0*)_P = *(Packet_0046cef0*)_L;
        }
    }
};

// FUNCTION: 0x46eba0
void Class_0046eba0::FUN_0046eba0(Packet_0046cef0* _P, unsigned int _M, const Packet_0046cef0& _X) {
    if (end - last < _M) {
        size_type _N = size() + (_M < size() ? size() : _M);
        Packet_0046cef0* _S = alloc.allocate(_N, (void*)0);
        Packet_0046cef0* _Q = _Ucopy(_S, first, _P);
        _Ufill(_Q, _M, _X);
        _Ucopy(_Q + _M, _P, last);
        ::operator delete(first);
        end = _S + _N;
        last = _S + (size() + _M);
        first = _S;
    } else if (last - _P < _M) {
        _Ucopy(_P + _M, _P, last);
        _Ufill(last, _M - (last - _P), _X);
        fill(_P, last, _X);
        last += _M;
    } else if (0 < _M) {
        _Ucopy(last, last - _M, last);
        copy_backward(_P, last - _M, last);
        fill(_P, _P + _M, _X);
        last += _M;
    }
}
