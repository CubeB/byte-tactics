// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Clears a std::vector<Point_0044eec0> and then appends a copy of the point
// at +0x8, with its y moved by (field_c + field_10) / 32.
#include <vector>

struct Point_0044eec0 {
    short x;
    short y;
};

typedef std::vector<Point_0044eec0> Vec_0044d560;

class Class_0044d560 {
public:
    char unknown_0[8];
    Point_0044eec0 pos;                // +0x8
    int field_c;                       // +0xc
    int field_10;                      // +0x10

    void FUN_0044d560(Vec_0044d560* list);
};

// FUNCTION: 0x44d560
void Class_0044d560::FUN_0044d560(Vec_0044d560* list)
{
    list->clear();
    Point_0044eec0 p = pos;
    // Sum written field_10 + field_c: the operand order follows the load order.
    p.y = p.y + (field_10 + field_c) / 32;
    list->push_back(p);
}
