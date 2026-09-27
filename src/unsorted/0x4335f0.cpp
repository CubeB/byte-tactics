// Decompiled by Space Bunny Free. Names are provisional.
//
// Class_004335f0 has a vector-of-vectors at +0 (the empty allocator base at
// +0, _First/_Last/_End at +4/+8/+0xc) and this member resizes it. A 16-byte
// value is built on the stack from the sole short argument: byte 0 is the low
// byte of the argument and the three pointers are zeroed. N is arg * 4. If
// size() < N it calls insert out of line (0x433db0); else if N < size() it
// inlines the tail of MSVC 5's vector::erase(iterator, iterator):
//
//     iterator _S = copy(_L, end(), _F);
//     _Destroy(_S, end());
//     _Last = _S;
//
// The copy is std::copy with both ends spelled end(), so the loop is dead code
// (the original keeps it, emitting `cmp edi, edi` / `je`); that is why _S
// equals _F and _Last ends up as _First + N * 16. The dead loop calls the
// element operator= (0x4345e0) out of line and _Destroy calls the element
// destructor (0x433a30) out of line, both as separate template instantiations.
//
// Writing the resize as the real `vector<vector<Elem>>::resize` (taking its
// address) makes MSVC inline insert and the element destructor, giving a
// ~550-byte function; hand-writing erase() and _Destroy() as members is what
// reproduces the out-of-line calls. Keeping erase() a *member* of the outer
// container is what fixes the register allocation: with the same code inlined
// into the caller, MSVC puts end() in ebx and the destination in edi, while
// the original (and the member version) has end() in edi and the destination
// in ebx. Shaping the container like <vector> and calling erase() is what
// reproduces that, and it takes the function from 59.5% to 77.2%.
//
// What still differs:
//   - MSVC loads the argument once as a word (`mov ax, word ptr [esp+0x14]`,
//     then `movsx eax, ax`); the original loads the low byte first
//     (`mov al, byte ptr [esp+0x14]`), stores it, and only then loads the word
//     (`movsx eax, word ptr [esp+0x1c]`). Both loads read the same two bytes of
//     the argument: [esp+0x14] before the pushes and [esp+0x1c] after them are
//     the same slot at [entry esp+4]. The byte has to reach the stack slot in al
//     while the word is rematerialised into eax after the two pushes, so the
//     original ended up with two independent loads where ours merges into one
//     and a 3-byte register move, which is also why we are 1 byte shorter.
//     Nothing tried splits them: a char local, a short local, an int local for
//     the size, `*(const char*)&n`, `*(const unsigned char*)&n`,
//     `(unsigned char)n` with `(short)n` for the size, setting the tag through
//     an inline setter, reading the byte through an inline method, reading the
//     size through an inline method (the 0x404270 recipe with the two roles
//     swapped), building the value by body assignment with a separate tag
//     store, and reordering the two statements. No header set changes it
//     either (tools/headers.py: 128 sets, all 59.5% on the old structure).
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00434020> Inner_004335f0;

class Value_004335f0 {
public:
    char tag;                          // +0x0
    char pad[3];
    void* first;                       // +0x4
    void* last;                        // +0x8
    void* end;                         // +0xc

    Value_004335f0(char c)
    {
        tag = c;
        first = 0;
        last = 0;
        end = 0;
    }

    ~Value_004335f0() { operator delete(first); }
};

class Class_00433db0 {
public:
    void FUN_00433db0(Inner_004335f0* _P, unsigned _M,
                      const Inner_004335f0& _X);
};

class Class_00433a30 {
public:
    void FUN_00433a30();
};

template<class It>
static It Copy_004335f0(It _F, It _L, It _X)
{
    for (; _F != _L; ++_F, ++_X)
        *_X = *_F;
    return _X;
}

// Shaped like MSVC 5's vector: begin/end, size() with the empty check, and a
// protected _Destroy().
class Outer_004335f0 {
public:
    char unknown_0[4];                 // +0x0
    Inner_004335f0* _First;            // +0x4
    Inner_004335f0* _Last;             // +0x8
    Inner_004335f0* _End;              // +0xc

    unsigned size() const
    {
        return _First == 0 ? 0 : (unsigned)(_Last - _First);
    }
    Inner_004335f0* begin() { return _First; }
    Inner_004335f0* end() { return _Last; }

    Inner_004335f0* erase(Inner_004335f0* _F, Inner_004335f0* _L)
    {
        Inner_004335f0* _S = Copy_004335f0(_L, end(), _F);
        _Destroy(_S, end());
        _Last = _S;
        return _F;
    }

protected:
    void _Destroy(Inner_004335f0* _F, Inner_004335f0* _L)
    {
        for (; _F != _L; ++_F)
            ((Class_00433a30*)_F)->FUN_00433a30();
    }
};

class Class_004335f0 {
public:
    Outer_004335f0 items;              // +0x0
    void FUN_004335f0(short n);
};

// FUNCTION: 0x4335f0
void Class_004335f0::FUN_004335f0(short n)
{
    Value_004335f0 v((char)n);
    unsigned _N = (unsigned)n * 4;
    if (items.size() < _N)
        ((Class_00433db0*)&items)->FUN_00433db0(items.end(), _N - items.size(),
                                                *(const Inner_004335f0*)&v);
    else if (_N < items.size())
        items.erase(items.begin() + _N, items.end());
}
