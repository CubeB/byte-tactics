// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_004c42a0, the
// out-of-line destructor of which is 0x4c42a0. It inlines that destructor:
// frees the name at +0, deletes the child nodes held in the vector at +4 (each
// delete goes back through this same function, the inline budget being spent),
// destroys the reference-counted string handles in the entries vector at
// +0x15, frees both vectors' storage, then frees the object itself when the
// flag's low bit is set. The holder's destructor below exists only to make the
// compiler emit this COMDAT.
#include <vector>

class Class_004c9390 {
public:
    char* data;

    void FUN_004c9390();
};

class Handle_004c42a0 {
public:
    char* data;

    ~Handle_004c42a0() { ((Class_004c9390*)this)->FUN_004c9390(); }
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

class Holder_004c32f0 {
public:
    Class_004c42a0* root;
    int field_4;
    int field_8;

    ~Holder_004c32f0();
};

// FUNCTION: 0x4c32f0 ??_GClass_004c42a0@@QAEPAXI@Z
Class_004c42a0::~Class_004c42a0()
{
    if (name)
        FUN_004d85a0(name);
    for (std::vector<Class_004c42a0*>::iterator p = children.begin(); p < children.end(); p++)
        delete *p;
}

Holder_004c32f0::~Holder_004c32f0()
{
    delete root;
    root = 0;
    field_4 = 0;
    field_8 = 0;
}
