// Decompiled by longcat-2.5-preview-free, finished by space-bunny-free and deepseek-v4.1-flash,
// finished by GPT-6. Names are provisional. PARTIAL 81.2%, 924 of 936 bytes. Replace the old
// byte-pointer copy loop, which advanced its source by one byte, with typed 14-byte packet
// pointers. Put the empty allocator at offset zero so the three pointers
// remain at +4/+8/+0xc. Use its allocate method, preserving the capacity
// while the allocator clamps its own signed allocation count.
// A destination-first _Ucopy(_P, _F, _L) improves the register family.
// The original capacity/new-buffer stack homes and some loop registers
// still differ. Helper permutations, const/reference forms, capacity-slot
// variants and 768 header sets did not improve this version further.
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
