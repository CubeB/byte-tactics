// Decompiled by deepseek-v4.1-flash. Names are provisional.
// An inlined std::vector insert. The vector holds 4-byte points (two shorts,
// the element type of 0x44ee60/0x44ee90/0x44eec0/0x44eef0), and the method
// resets the list to a single point taken from this->+8, that is an inlined
// vector::assign(1, pos) or clear() plus push_back(pos):
//
//   list->clear();          // erase(begin(), end())
//   list->push_back(pos);   // inserts *(Point*)(this + 8) at the end
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
