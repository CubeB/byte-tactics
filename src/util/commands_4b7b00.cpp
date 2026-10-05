// Decompiled by Sonnet 5.5. Names are provisional.
// std::vector<Class_004b7e30>::insert(iterator, size_type, const T&), out of
// line: 12-byte elements holding a reference-counted string handle and two
// ints. The copy constructor is 0x4b7e30 and the assignment 0x4b7e00; the
// destructor releases the handle through 0x4c9390. Taking insert's address
// makes the compiler emit the template instantiation, as in 0x488fb0.cpp.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

class Class_004b7e00 {
public:
    char unknown_0[12];
    void* FUN_004b7e00(int* param_1);
};

class Class_004b7e30 {
public:
    Class_004c9390 name;               // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004b7e30(const Class_004b7e30& other);
    Class_004b7e30& operator=(const Class_004b7e30& other)
    {
        ((Class_004b7e00*)this)->FUN_004b7e00((int*)&other);
        return *this;
    }
    ~Class_004b7e30() { name.ReleaseRef(); }
};

typedef std::vector<Class_004b7e30> Vec_004b7b00;
typedef void (Vec_004b7b00::*InsertFn_004b7b00)(
    Vec_004b7b00::iterator, Vec_004b7b00::size_type, const Class_004b7e30&);

// FUNCTION: 0x4b7b00 ?insert@?$vector@VClass_004b7e30@@V?$allocator@VClass_004b7e30@@@std@@@std@@QAEXPAVClass_004b7e30@@IABV3@@Z
InsertFn_004b7b00 g_insert_004b7b00 = &Vec_004b7b00::insert;
