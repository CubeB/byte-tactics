// Decompiled by deepseek-v4.1-flash. Names are provisional.
// An inlined std::vector insert. The vector holds 4-byte points (two shorts,
// the element type of 0x44ee60/0x44ee90/0x44eec0/0x44eef0), and the method
// resets the list to a single point taken from this->+8, that is an inlined
// vector::assign(1, pos) or clear() plus push_back(pos):
//
//   list->clear();          // erase(begin(), end())
//   list->push_back(pos);   // inserts *(Point*)(this + 8) at the end
//
// <vector>'s erase and insert are inlined, so their helpers are called out of
// line: _Ucopy (0x44ee90), _Ufill (0x44eec0), _Destroy (0x44ee60) and, once,
// size (0x44ee70); operator new/delete are 0x4b4f10/0x4b4f20. Every byte
// matches; see the note below about the one reference check.py still refuses.
//
// PARTIAL: check.py reports 100% (every byte matches), but refuses the one
// call to size() at 0x44d1d7 because of a name in data/symbols.csv. That call
// is the size() of the same std::vector<Point_0044eec0> whose _Ucopy
// (0x44ee90), _Ufill (0x44eec0) and _Destroy (0x44ee60) symbols.csv records
// as UPoint_0044eec0::?$vector::_Ucopy / ::_Ufill / ::_Destroy, so the
// consistent name for 0x44ee70 would be UPoint_0044eec0::?$vector::size
// (exactly how 0x40c5b0.cpp and 0x40d4c0.cpp name the size() of other
// vectors). Instead 0x44ee70.cpp gave its class the placeholder name of its
// own address, and symbols.csv holds "Class_0044ee70::FUN_0044ee70" for
// 0x44ee70. check.py compares mangled names, so the correctly named size()
// reference is refused.
//
// Naming the call Class_0044ee70::FUN_0044ee70 instead was tried and dropped:
// declaring the insert's size() as an out-of-line member of a class named
// Class_0044ee70 (with the real std::vector<Point_0044eec0> as a base, for the
// _Ucopy/_Ufill/_Destroy names) does resolve the reference, but writing the
// insert by hand instead of letting <vector> inline it changes the optimiser's
// inlining decisions and register allocation, dropping the result to 49%. The
// fix belongs in 0x44ee70.cpp: declaring the vector there would give 0x44ee70
// the name the other three helpers have, and this file would match as it
// stands. This is the same situation as 0x471820.cpp and packet_sequencer.cpp.
#include <vector>

struct Point_0044eec0 {
    short x;
    short y;
};

typedef std::vector<Point_0044eec0> Vec_0044d0e0;

class Class_0044d0e0 {
public:
    char unknown_0[8];
    Point_0044eec0 pos;                // +0x8

    void FUN_0044d0e0(Vec_0044d0e0* list);
};

// FUNCTION: 0x44d0e0
void Class_0044d0e0::FUN_0044d0e0(Vec_0044d0e0* list)
{
    list->clear();
    list->push_back(pos);
}
