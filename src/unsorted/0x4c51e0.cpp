// Decompiled by Sonnet 5.5. Names are provisional.
// std::vector<Class_004c54a0>::insert(iterator, size_type, const T&), out of
// line: 8-byte elements holding two reference-counted string handles. The
// copy constructor is 0x4c54a0, the assignment 0x4c5470 and the destructor
// 0x4c5190. Taking insert's address makes the compiler emit the template
// instantiation, as in 0x488fb0.cpp.
#include <vector>

struct Elem_004c5bc0 {
    char* a;
    char* b;

    ~Elem_004c5bc0();
};

class Class_004c5470 {
public:
    char unknown_0[8];
    void* FUN_004c5470(int* param_1);
};

class Class_004c54a0 {
public:
    Elem_004c5bc0 pair;                // +0x0

    Class_004c54a0(const Class_004c54a0& other);
    Class_004c54a0& operator=(const Class_004c54a0& other)
    {
        ((Class_004c5470*)this)->FUN_004c5470((int*)&other);
        return *this;
    }
};

typedef std::vector<Class_004c54a0> Vec_004c51e0;
typedef void (Vec_004c51e0::*InsertFn_004c51e0)(
    Vec_004c51e0::iterator, Vec_004c51e0::size_type, const Class_004c54a0&);

// FUNCTION: 0x4c51e0 ?insert@?$vector@VClass_004c54a0@@V?$allocator@VClass_004c54a0@@@std@@@std@@QAEXPAVClass_004c54a0@@IABV3@@Z
InsertFn_004c51e0 g_insert_004c51e0 = &Vec_004c51e0::insert;
