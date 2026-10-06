// Decompiled by Haiku, Opus and deepseek-v4.1-flash. Names are provisional.

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

void __cdecl FUN_004d85a0(int* param_1);

struct TdfField {
    Class_004c91a0 a;                  // +0x0 key
    Class_004c91a0 b;                  // +0x4 value

    ~TdfField();
};

#pragma pack(push, 1)
class TdfRecord {
public:
    int* name;                                 // +0x0
    std::vector<TdfRecord*> children;          // +0x4
    char unknown_14;                           // +0x14
    std::vector<TdfField> entries;             // +0x15

    // TdfRecord::FindSubRecord (0x4c4470), inlined.
    TdfRecord* FindChild(char* key)
    {
        for (TdfRecord** p = children.begin(); p < children.end(); p++) {
            if (_strcmpi((char*)(*p)->name, key) == 0)
                return *p;
        }
        return 0;
    }

    TdfRecord* GetChild(int index)
    {
        return index >= children.size() ? 0 : children.at(index);
    }

    ~TdfRecord()
    {
        if (name)
            FUN_004d85a0(name);
        for (TdfRecord** p = children.begin(); p < children.end(); p++)
            delete *p;
    }
};
#pragma pack(pop)

// Handles the text at p (a // comment, a /* */ comment, or one ordinary
// character) and returns where to continue.
static inline char* SkipComment(char* p)
{
    if (p[0] == '/' && p[1] == '/') {
        while (1) {
            if (*p == '\n')
                break;
            *p = ' ';
            p++;
            if (*p == 0)
                break;
        }
    } else if (p[0] == '/' && p[1] == '*') {
        p[1] = ' ';
        p[0] = ' ';
        p += 2;
        while (*p) {
            if (p[-1] == '*' && p[0] == '/') {
                p[0] = ' ';
                p[-1] = ' ';
                p++;
                break;
            }
            p[-1] = ' ';
            p++;
        }
    } else {
        p++;
    }
    return p;
}

class TdfFile {
public:
    TdfRecord* root;                   // +0x0
    TdfRecord* current;                // +0x4
    int field_8;                       // +0x8

    // The functions that delete the root section each destroy its entries a
    // different way (inline, through FUN_004c5170, ~TdfField or
    // ??_GTdfField, as the inline budget runs out), and no one view of the
    // section gives all of them: those stay in files of their own with the
    // view each needs. They are the destructor (0x4c2eb0, with ??_GTdfField
    // in tdf_4c51b0.cpp), LoadFile (tdf_4c2f60.cpp), LoadBuffer
    // (tdf_4c3120.cpp) and ??_ETdfFile (data_files_42a870.cpp).
    TdfFile();
    ~TdfFile();
    int LoadFile(char* path);
    void StripComments(char* p);
    void LoadBuffer(char* data, int size, int flag, char* path);
    void Unload();
    int SelectRecord(char* name);
    int SelectRecordAt(int index);
    void ResetCurrentRecord();
    int GetCurrentRecord();
    void SetCurrentRecord(int val);
};

// Default constructor of a 12-byte class; its destructor is 0x4c2eb0.
// FUNCTION: 0x4c2ea0
TdfFile::TdfFile()
{
    root = 0;
    current = 0;
    field_8 = 0;
}

// Destructor-shaped method: frees the TDF section tree hanging off root and
// zeroes the 12-byte object. Same body as TdfFile's destructor
// (0x4c2eb0, in tdf_4c51b0.cpp), but the entry vector here is a direct
// std::vector<TdfField> member, so its destroy loop calls ~Elem
// out of line (0x4c5190) instead of the scalar deleting destructor 0x4c51b0.
//
// The one byte-level difference from `delete root;` on its own is the register
// allocator: without the explicit test it keeps root in edi and the loops in
// esi, the original has them the other way round. Spelling the null test out
// (the redundant `if (root)` folds into delete's own check) makes MSVC give
// root esi, exactly as the original does.
// FUNCTION: 0x4c3240
void TdfFile::Unload()
{
    if (root)
        delete root;
    root = 0;
    current = 0;
    field_8 = 0;
}

// Blanks out C and C++ style comments in a text buffer, in place. A method
// that ignores `this`: its callers (0x4c2f60, 0x4c3120) pass their own
// `this` through in ecx.
// FUNCTION: 0x4c33a0
void TdfFile::StripComments(char* p)
{
    while (*p)
        p = SkipComment(p);
}

// Moves the cursor to the child of the current node (or of the root when
// there is none) whose name matches case-insensitively; returns whether
// one was found.
// FUNCTION: 0x4c3410
int TdfFile::SelectRecord(char* name)
{
    TdfRecord* node = current;
    if (node == 0)
        node = root;
    current = node->FindChild(name);
    return current != 0;
}

// Moves the cursor to the child of the current TDF node (or of the root when
// there is none) at the given index. The node's child vector is a
// std::vector<TdfRecord*> at +4 (_First at +8, _Last at +0xc), and the
// out-of-range case throws out_of_range through the inlined vector::at()
// (the literal "invalid vector<T> subscript"). The inline GetChild wrapper is
// what keeps logic_error's constructor out of line: without one extra inline
// level MSVC 5 inlines it into this function too, and the call to the
// out-of-line logic_error constructor (0x4c35c0) is lost.
// FUNCTION: 0x4c3490
int TdfFile::SelectRecordAt(int index)
{
    TdfRecord* n = current ? current : root;
    TdfRecord* node = n->GetChild(index);
    current = node;
    return node != 0;
}

// FUNCTION: 0x4c3e10
void TdfFile::ResetCurrentRecord()
{
    current = 0;
}

// FUNCTION: 0x4c3e20
int TdfFile::GetCurrentRecord()
{
    return (int)current;
}

// FUNCTION: 0x4c3e30
void TdfFile::SetCurrentRecord(int val)
{
    current = (TdfRecord*)val;
}
