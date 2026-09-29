// Decompiled by longcat-2.5-preview-free, finished by space-bunny-free and deepseek-v4.1-flash. Names are provisional.
// Class_0046eba0 is a std::vector<Packet_0046cef0> whose three iterators are
// byte pointers over 0xe-byte elements (allocator byte, _First +0x4, _Last +0x8,
// _End +0xc). This is its insert(iterator, size_type, const T&), out of line,
// and it is the same code as 0x46e640, which is the vector<int> instantiation of
// the same member. The template's shape is taken from 0x46e640: size() is the
// null-guarded element count, the copy/fill helpers go through
// allocator.construct (whose null check is the `test dest,dest` guard inside
// each of those loops), and fill and copy_backward assign directly, which is
// why only the _Ucopy and _Ufill loops carry the guard.
//
// NOT MATCHING yet (check.py: 925 of 936 bytes, 67.6%). What still differs:
//   * In every _Ucopy loop this file's source induction variable advances by
//     ONE byte (`inc eax`) where the original advances by 0xe (`add eax,0xe`),
//     while the destination advances by 0xe in both. copy_backward had the
//     same one-byte source step and now uses `_L -= 14` (0x46eee1).
//   * Consequently the whole realloc path is one live-range step off: the
//     original keeps _N in the (dead) _M argument home slot [esp+0x20] across
//     the operator new call, holds _First in edi and uses edx for the _N*14
//     scaling; this file keeps _N*14 in edi.
// deepseek-v4.1-flash: the prologue and the first 64 instructions match the
// original instruction for instruction (this in ebp, _M in ebx); the divergence
// is the grow path above. The one-byte step is the whole problem: every spelling
// that makes the source advance by 0xe (>= 14 in the for-header: 53.2%; the same
// increments as body statements: 59.4%; Packet* locals in _Ucopy: 53.7%; the
// real <vector> header or the hand-rolled template on Packet* iterators:
// 62.7%/63.3%) FLIPS the allocator and puts `this` in ebx, moving the whole
// function. Only the char* byte-pointer model keeps `this` in ebp. A flat
// N-declarations sweep (0 to 400 unused externs, step 8) on the real-vector
// model scores 62.7% at every N, so this is not compiler state.
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

    char unknown_0[4];
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
        // The original decrements the SOURCE by 0xe too (0x46eee1 sub eax,0xe,
        // 0x46eee4 sub ecx,0xe), not by one byte.
        for (--_L; _F != _L; _L -= 14) {
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
