// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of the TDF entry (a key
// and a value, both reference-counted string handles; the vector<entry>
// _Ucopy is 0x4c5bc0). It is called with flag 0 in the destroy loop of the
// entry vector inside TdfFile's destructor (0x4c2eb0), which `delete`s the
// root section.
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
};

void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
// Two wrapper levels around the vector: with one, ~TdfField is called out of line.
struct Inner_004c51b0 {
    std::vector<TdfField> v;
};

struct Entries_004c51b0 {
    Inner_004c51b0 v;
};

class TdfRecord {
public:
    int* name;                                 // +0x0
    std::vector<TdfRecord*> children;        // +0x4
    char unknown_14;                           // +0x14
    Entries_004c51b0 entries;                  // +0x15

    ~TdfRecord()
    {
        if (name)
            FUN_004d85a0(name);
        for (TdfRecord** p = children.begin(); p < children.end(); p++)
            delete *p;
    }
};
#pragma pack(pop)

class TdfFile {
public:
    TdfRecord* root;
    int field_4;
    int field_8;

    ~TdfFile();
};

// Own file: needs its own view of TdfRecord (see the note in TdfFile, tdf_4c2ea0.cpp).
// FUNCTION: 0x4c2eb0
// FUNCTION: 0x4c51b0 ??_GTdfField@@QAEPAXI@Z
TdfFile::~TdfFile()
{
    delete root;
    root = 0;
    field_4 = 0;
    field_8 = 0;
}
