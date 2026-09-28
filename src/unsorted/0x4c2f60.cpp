// Decompiled by Space Bunny Free. Names are provisional.
// NOT MATCHING (65.8%): every difference left is one register choice. The
// original keeps the path parameter in a callee-saved register (edi, from the
// prologue) and therefore has no free register for the return-value local,
// which lives in the frame slot at -0xc ("mov [esp+0x18], 0" after the size
// call and "mov eax, [esp+0x18]" in the epilogue). This version instead keeps
// the return value in esi and reloads the path from its argument slot, which
// also moves the frame slots (this/buf/kids order) and turns the fresh
// `mov eax, [esi+8]` plus dead slot store before the children free into a
// reload of the kids local. The constant 0 follows: the original materialises
// it in edi just before the six "= 0" stores ("xor edi, edi"), this version
// materialises it early as the initial value of the return local.
//
// Loads a .TDF file, throws away the tree parsed from the previous load and
// parses the file again: open, take the file length, read the whole file into
// a fresh block, copy it into a "TDF file" block that is NUL terminated,
// blanks out comments in that copy (Class_004c33a0::FUN_004c33a0), and parses
// it into a new root section named "root" (FUN_004c3e40). Returns 1 on
// success, 0 when the file cannot be opened or the read fails. The class
// holding the tree is named after the method 0x4c33a0 that is called on it.
#include <string.h>

struct Elem_004c2f60 {
    int key;                             // +0x0
    int value;                           // +0x4
};

// A parsed .TDF section: its name, a flag, a vector of child sections and a
// vector of key/value entries. The vector is not aligned inside the section.
#pragma pack(push, 1)
class Class_004c32f0 {
public:
    int* name;                           // +0x0
    int field_4;                         // +0x4
    Class_004c32f0** children;           // +0x8
    Class_004c32f0** childrenEnd;        // +0xc
    Class_004c32f0** childrenCap;        // +0x10
    char unknown_14[5];                  // +0x14
    Elem_004c2f60* entries;              // +0x19
    Elem_004c2f60* entriesEnd;           // +0x1d
    Elem_004c2f60* entriesCap;           // +0x21
    void FUN_004c32f0(int flag);
    Class_004c32f0* FUN_004c3e40(char* name, char* text, int flag, char* path);
};
#pragma pack(pop)

void __stdcall FUN_004c5170(Elem_004c2f60* e);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
void __cdecl FUN_004d85a0(void* p);
void* __cdecl FUN_004d83b0(char* name, int size);
char* __stdcall FUN_004bb5b0(char* path);
int __stdcall FUN_004bb5d0(char* file);
int __stdcall FUN_004bb650(char* file);
int __stdcall FUN_004bb710(char* file, int pos);
int __stdcall FUN_004bb7c0(char* file, void* buf, int size);
int __stdcall FUN_004bbd00(char* file);

class Class_004c33a0 {
public:
    Class_004c32f0* root;                // +0x0
    int field_4;                         // +0x4
    int field_8;                         // +0x8
    void FUN_004c33a0(char* p);
    int FUN_004c2f60(char* path);
};

// FUNCTION: 0x4c2f60
int Class_004c33a0::FUN_004c2f60(char* path)
{
    char* file = (char*)FUN_004bb5b0(path);
    if (!file)
        return 0;
    int size = FUN_004bbd00(file);
    int result = 0;
    if (size > 0) {
        char* buf = (char*)FUN_004d83b0(path, size);
        FUN_004bb710(file, 0);
        if (FUN_004bb7c0(file, buf, size) >= 0) {
            result = FUN_004bb650(file);
            Class_004c32f0* s = root;
            if (s) {
                if (s->name)
                    FUN_004d85a0(s->name);
                Class_004c32f0** kids = s->children;
                for (Class_004c32f0** p = kids; p < s->childrenEnd; p++) {
                    if (*p)
                        (*p)->FUN_004c32f0(1);
                }
                Elem_004c2f60* pe = s->entriesEnd;
                for (Elem_004c2f60* e = s->entries; e != pe; e++)
                    FUN_004c5170(e);
                operator delete(s->entries);
                s->entries = 0;
                s->entriesEnd = 0;
                s->entriesCap = 0;
                operator delete(kids);
                s->children = 0;
                s->childrenEnd = 0;
                s->childrenCap = 0;
                operator delete(s);
            }
            root = 0;
            field_4 = 0;
            field_8 = result;
            char* text = (char*)FUN_004d83b0("TDF file", size + 1);
            memcpy(text, buf, size);
            text[size] = 0;
            FUN_004c33a0(text);
            Class_004c32f0* node = (Class_004c32f0*)operator new(0x29);
            root = node ? node->FUN_004c3e40("root", text, 0, path) : 0;
            FUN_004d85a0(text);
            result = 1;
        }
        FUN_004d85a0(buf);
    }
    FUN_004bb5d0(file);
    return result;
}
