// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads a .TDF document from an in-memory buffer: throws away the tree parsed
// from the previous load and parses the buffer again. The buffer is copied
// into a fresh "TDF file" block that is NUL terminated, comments are blanked
// out in that copy (TdfFile::StripComments), and the text is parsed
// into a new root section named "root" (FUN_004c3e40). The class holding the
// tree is the same one 0x4c2f60 loads files into.
#include <vector>
#include <string.h>

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
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
void* __cdecl FUN_004d83b0(char* name, int size);

#pragma pack(push, 1)
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

    TdfRecord* FUN_004c3e40(char* name, char* text, int flag, char* path);

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
    TdfRecord* root;                     // +0x0
    int field_4;                           // +0x4
    int field_8;                           // +0x8
    void LoadBuffer(char* data, int size, int flag, char* path);
    void StripComments(char* p);
};

// FUNCTION: 0x4c3120
void TdfFile::LoadBuffer(char* data, int size, int flag, char* path)
{
    delete root;
    root = 0;
    field_4 = 0;
    field_8 = flag;
    char* text = (char*)FUN_004d83b0("TDF file", size + 1);
    memcpy(text, data, size);
    text[size] = 0;
    ((TdfFile*)this)->StripComments(text);
    TdfRecord* node = (TdfRecord*)operator new(0x29);
    root = node ? node->FUN_004c3e40("root", text, 0, path) : 0;
    FUN_004d85a0((int*)text);
}
