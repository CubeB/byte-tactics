// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of the TDF entry (a key
// and a value, both reference-counted string handles; the vector<entry>
// _Ucopy is 0x4c5bc0). The entry's destructor is implicit, so MSVC only
// emits this ??_G, and calls it with flag 0 where the inline depth runs out:
// in the destroy loop of the entry vector inside Class_004c2ea0's destructor
// (0x4c2eb0), which `delete`s the root section.
//
// That destructor is defined again below, unannotated, to emit this COMDAT;
// with this layout it compiles byte-identical to 0x4c2eb0. The entry vector
// sits two (implicit-destructor) wrapper levels deep in the section: with one
// level, MSVC calls ~Elem_004c5bc0 out of line instead of this ??_G. The
// real wrapper classes are unknown (0x4c48c0 binary-searches the entries).
// 0x4c9390 is really the handle's destructor, but it is established as
// Class_004c9390::ReleaseRef, so the handle's inline destructor calls it.
#include <vector>

class Class_004c9390 {
public:
    char* data;
    void ReleaseRef();
};

class Class_004c91a0 {
public:
    char* p;
    Class_004c91a0();
    Class_004c91a0(const Class_004c91a0& other);
    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

struct Elem_004c5bc0 {
    Class_004c91a0 a;                  // +0x0 key
    Class_004c91a0 b;                  // +0x4 value
};

void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Inner_004c51b0 {
    std::vector<Elem_004c5bc0> v;
};

struct Entries_004c51b0 {
    Inner_004c51b0 v;
};

class Class_004c42a0 {
public:
    int* name;                                 // +0x0
    std::vector<Class_004c42a0*> children;   // +0x4
    char unknown_14;                           // +0x14
    Entries_004c51b0 entries;                  // +0x15

    ~Class_004c42a0()
    {
        if (name)
            FUN_004d85a0(name);
        for (Class_004c42a0** p = children.begin(); p < children.end(); p++)
            delete *p;
    }
};
#pragma pack(pop)

class Class_004c2ea0 {
public:
    Class_004c42a0* root;
    int field_4;
    int field_8;

    ~Class_004c2ea0();
};

// FUNCTION: 0x4c2eb0
// FUNCTION: 0x4c51b0 ??_GElem_004c5bc0@@QAEPAXI@Z
Class_004c2ea0::~Class_004c2ea0()
{
    delete root;
    root = 0;
    field_4 = 0;
    field_8 = 0;
}
