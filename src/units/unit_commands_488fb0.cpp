// Decompiled by Sonnet 5.5. Names are provisional.
// std::vector<UnitCategory>::insert(iterator, size_type, const T&), out of
// line, for the global vector of 0x488a00.cpp: 8-byte elements holding a
// reference-counted string handle and an int. The copy constructor is
// 0x489260 and the assignment 0x489240; the destructor releases the handle
// through 0x4c9390. Taking insert's address makes the compiler emit the
// template instantiation, as in 0x434470.cpp.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

class Class_00489240 {
public:
    char unknown_0[8];
    Class_00489240* FUN_00489240(Class_00489240* other);
};

class UnitCategory {
public:
    Class_004c9390 name;               // +0x0
    int value;                         // +0x4

    UnitCategory(const UnitCategory& other);
    UnitCategory& operator=(const UnitCategory& other)
    {
        ((Class_00489240*)this)->FUN_00489240((Class_00489240*)&other);
        return *this;
    }
    ~UnitCategory() { name.ReleaseRef(); }
};

typedef std::vector<UnitCategory> Vec_00488fb0;
typedef void (Vec_00488fb0::*InsertFn_00488fb0)(
    Vec_00488fb0::iterator, Vec_00488fb0::size_type, const UnitCategory&);

// FUNCTION: 0x488fb0 ?insert@?$vector@VUnitCategory@@V?$allocator@VUnitCategory@@@std@@@std@@QAEXPAVUnitCategory@@IABV3@@Z
InsertFn_00488fb0 g_insert_00488fb0 = &Vec_00488fb0::insert;
