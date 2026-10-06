// Decompiled by Haiku and DeepSeek V4.1 Flash. Names are provisional.

#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

class LosTable : public std::vector<Elem_00434360> {
public:
    int GetLosLineCount();
    void FUN_004335f0(short n);
};

// FUNCTION: 0x4335c0
int LosTable::GetLosLineCount()
{
    return size();
}

//
// Inlined std::vector<Elem_00434360>::resize(_N, _X) for the table code built
// by 0x433130/0x433380: the object at +0 is the vector (its _First at +4), the
// element Elem_00434360 is a struct holding one std::vector<Elem_00434020>
// (see docs/consolidation.md), and _N is the argument times 4 (the number of
// lines). The value _X is a default-constructed Elem_00434360 local; its held
// vector's inline constructor copies the empty (one byte) allocator, which is
// where the `mov al, byte [n]` / `mov byte [x], al` pair comes from, and the
// compiler reuses the low byte of the argument for that otherwise undefined
// byte. insert, the element's operator= and the element destructor stay out
// of line (0x433db0, 0x4345e0, 0x433a30), so the real <vector> header
// reproduces the original exactly. Hand-writing erase() only got close: MSVC
// merged the two argument reads into one word load plus `movsx eax, ax`,
// while the original keeps a separate byte load and word load.
//
// The sibling 0x433270 is the same wrapper for the next element level up
// (`vector<vector<Elem_00434360>>`), where `resize(n, x)` has no `* 4`.
// FUNCTION: 0x4335f0
void LosTable::FUN_004335f0(short n)
{
    Elem_00434360 x;
    resize(n * 4, x);
}
