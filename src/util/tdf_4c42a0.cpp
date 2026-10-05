// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Destructor of a tree node (its scalar deleting destructor is 0x4c32f0):
// frees the name at +0, deletes the child nodes held in the vector at +4, and
// lets the entries vector at +0x15 (pairs of reference-counted string handles)
// destroy its elements itself.
#include <vector>

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

class Handle_004c42a0 {
public:
    char* data;

    ~Handle_004c42a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

struct Elem_004c42a0 {
    Handle_004c42a0 key;                    // +0x0
    Handle_004c42a0 value;                  // +0x4
};

void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
class Class_004c42a0 {
public:
    int* name;                              // +0x0
    std::vector<Class_004c42a0*> children;  // +0x4
    char unknown_14;                        // +0x14
    std::vector<Elem_004c42a0> entries;     // +0x15

    ~Class_004c42a0();
};
#pragma pack(pop)

// FUNCTION: 0x4c42a0
Class_004c42a0::~Class_004c42a0()
{
    if (name)
        FUN_004d85a0(name);
    for (std::vector<Class_004c42a0*>::iterator p = children.begin(); p < children.end(); p++)
        delete *p;
}
