// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Destructor-shaped method: frees the TDF section tree hanging off root and
// zeroes the 12-byte object. Same body as Class_004c2ea0's destructor
// (0x4c2eb0, in 0x4c51b0.cpp), but the entry vector here is a direct
// std::vector<TdfField> member, so its destroy loop calls ~Elem
// out of line (0x4c5190) instead of the scalar deleting destructor 0x4c51b0.
//
// The one byte-level difference from `delete root;` on its own is the register
// allocator: without the explicit test it keeps root in edi and the loops in
// esi, the original has them the other way round. Spelling the null test out
// (the redundant `if (root)` folds into delete's own check) makes MSVC give
// root esi, exactly as the original does.
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

struct TdfField {
    Class_004c91a0 a;                  // +0x0 key
    Class_004c91a0 b;                  // +0x4 value

    ~TdfField();
};

void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
class TdfRecord {
public:
    int* name;                                 // +0x0
    std::vector<TdfRecord*> children;        // +0x4 (_First at +0x8)
    char unknown_14;                           // +0x14
    std::vector<TdfField> entries;             // +0x15 (_First at +0x19)

    ~TdfRecord()
    {
        if (name)
            FUN_004d85a0(name);
        for (TdfRecord** p = children.begin(); p < children.end(); p++)
            delete *p;
    }
};
#pragma pack(pop)

class Class_004c3240 {
public:
    TdfRecord* root;                 // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    void Unload();
};

// FUNCTION: 0x4c3240
void Class_004c3240::Unload()
{
    if (root)
        delete root;
    root = 0;
    field_4 = 0;
    field_8 = 0;
}
