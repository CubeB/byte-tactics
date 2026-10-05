// Decompiled by Sonnet 5.5. Names are provisional.
// std::vector<Class_004c91a0>::insert(iterator, size_type, const T&), out of
// line, for a vector of reference-counted string handles (4 bytes each). The
// copy constructor is 0x4c91a0, the assignment 0x4c93b0 and the destructor
// releases the handle through 0x4c9390. Taking insert's address makes the
// compiler emit the template instantiation, as in 0x488fb0.cpp.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct Class_004c93b0 {
    char* ptr;
    Class_004c93b0* Assign(Class_004c93b0* param_1);
};

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
    Class_004c91a0& operator=(const Class_004c91a0& other)
    {
        ((Class_004c93b0*)this)->Assign((Class_004c93b0*)&other);
        return *this;
    }
    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

typedef std::vector<Class_004c91a0> Vec_004be6c0;
typedef void (Vec_004be6c0::*InsertFn_004be6c0)(
    Vec_004be6c0::iterator, Vec_004be6c0::size_type, const Class_004c91a0&);

// FUNCTION: 0x4be6c0 ?insert@?$vector@VClass_004c91a0@@V?$allocator@VClass_004c91a0@@@std@@@std@@QAEXPAVClass_004c91a0@@IABV3@@Z
InsertFn_004be6c0 g_insert_004be6c0 = &Vec_004be6c0::insert;
