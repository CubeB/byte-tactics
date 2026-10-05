// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
// Loads a .TDF file: opens it, takes the file length, reads the whole file
// into a fresh block, copies it into a "TDF file" block that is NUL
// terminated, blanks out comments in that copy (Class_004c33a0::FUN_004c33a0,
// reached through a base class of the object that holds the tree) and parses
// it into a new root section named "root" (FUN_004c3e40). The tree parsed by
// the previous load is deleted first. Returns 1 on success, 0 when the file
// cannot be opened or the read fails. The tail of the function is the same
// code as Class_004c2f60::FUN_004c3120 (0x4c3120), written out again here.
//
// Two things decide the register allocation and the stack layout, and both
// are needed for a byte match:
//   * the section's entry vector is a std::vector<Elem_004c2f60> whose
//     elements have no destructor, with a std::_Destroy overload that calls
//     the out-of-line release function FUN_004c5170 on each one. That is what
//     makes the destroy loop of the inlined ~Class_004c42a0 call
//     FUN_004c5170 (0x4c5170) with the element pointer as a stack argument,
//     as it does here, instead of Elem_004c5bc0::~Elem_004c5bc0 (0x4c5190,
//     0x4c3240) or its scalar deleting destructor ??_G (0x4c51b0,
//     0x4c3120);
//   * `got` and `flag` below. The call results have to land in locals, not in
//     registers, for `result` to be spilled to its frame slot the way the
//     original has it (mov [esp+0xc], 0 after the size call, and
//     mov eax, [esp+0xc] in the epilogue), and that fixes the order of the
//     three stack slots (this, buf, result). Without them MSVC keeps result in
//     esi, keeps path in its argument slot, and permutes the slots.
#include <vector>
#include <string.h>

class Class_004c9390 {
public:
    char* data;
    void FUN_004c9390();
};

class Class_004c91a0 {
public:
    char* p;
};

// One key/value entry of a section: two reference-counted string handles.
struct Elem_004c2f60 {
    Class_004c91a0 a;                  // +0x0 key
    Class_004c91a0 b;                  // +0x4 value
};

void __stdcall FUN_004c5170(char* p);

namespace std {
// The entries are released by a plain function rather than by an element
// destructor, so the vector's destroy loop calls this once per element.
inline void _Destroy(Elem_004c2f60* p)
{
    FUN_004c5170((char*)p);
}
}

void __cdecl FUN_004d85a0(int* param_1);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
void* __cdecl FUN_004d83b0(char* name, int size);

#pragma pack(push, 1)
class Class_004c42a0 {
public:
    int* name;                                 // +0x0
    std::vector<Class_004c42a0*> children;   // +0x4 (_First at +0x8)
    char unknown_14;                           // +0x14
    std::vector<Elem_004c2f60> entries;        // +0x15 (_First at +0x19)

    Class_004c42a0* FUN_004c3e40(char* name, char* text, int flag, char* path);

    ~Class_004c42a0()
    {
        if (name)
            FUN_004d85a0(name);
        for (Class_004c42a0** p = children.begin(); p < children.end(); p++)
            delete *p;
    }
};
#pragma pack(pop)

class Class_004c33a0 {
public:
    void FUN_004c33a0(char* p);
};

class Class_004c2f60 {
public:
    Class_004c42a0* root;                // +0x0
    int field_4;                         // +0x4
    int field_8;                         // +0x8

    int FUN_004c2f60(char* path);
};

char* __stdcall FUN_004bb5b0(char* path);
int __stdcall FUN_004bb5d0(char* file);
int __stdcall FUN_004bb650(char* file);
int __stdcall FUN_004bb710(char* file, int pos);
int __stdcall FUN_004bb7c0(char* file, void* buf, int size);
int __stdcall FUN_004bbd00(char* file);

// FUNCTION: 0x4c2f60
int Class_004c2f60::FUN_004c2f60(char* path)
{
    char* file = FUN_004bb5b0(path);
    if (!file)
        return 0;
    int size = FUN_004bbd00(file);
    int result = 0;
    if (size > 0) {
        char* buf = (char*)FUN_004d83b0(path, size);
        FUN_004bb710(file, 0);
        int got = FUN_004bb7c0(file, buf, size);
        if (got >= 0) {
            int flag = FUN_004bb650(file);
            result = flag;
            if (root)
                delete root;
            root = 0;
            field_4 = 0;
            field_8 = result;
            char* text = (char*)FUN_004d83b0("TDF file", size + 1);
            memcpy(text, buf, size);
            text[size] = 0;
            ((Class_004c33a0*)this)->FUN_004c33a0(text);
            Class_004c42a0* node = (Class_004c42a0*)operator new(0x29);
            root = node ? node->FUN_004c3e40("root", text, 0, path) : 0;
            FUN_004d85a0((int*)text);
            result = 1;
        }
        FUN_004d85a0((int*)buf);
    }
    FUN_004bb5d0(file);
    return result;
}
