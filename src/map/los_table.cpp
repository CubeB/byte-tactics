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

// Resizes the table vector at +0 (its _First at +4) to the argument times 4
// (the number of lines), filling with a default Elem_00434360, a struct
// holding one std::vector<Elem_00434020> (see docs/consolidation.md). Used by
// the table code built by 0x433130/0x433380.
//
// The sibling 0x433270 is the same wrapper for the next element level up
// (`vector<vector<Elem_00434360>>`), where `resize(n, x)` has no `* 4`.
// FUNCTION: 0x4335f0
void LosTable::FUN_004335f0(short n)
{
    Elem_00434360 x;
    resize(n * 4, x);
}
