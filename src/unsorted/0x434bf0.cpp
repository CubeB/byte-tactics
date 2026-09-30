// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// Partial (92.9%, size 886 = the original's). Every instruction matches except
// the frame offsets of five locals; the code around them is identical, only the
// `esp+` displacements differ. Frame map (offsets from esp while the four
// callee-saved registers are pushed, so the return address is at 0x10 and the
// three arguments at 0x340/0x344/0x348):
//
//   slot   original        ours
//   0x10   count           allocator temp
//   0x14   allocator temp  count
//   0x18   bFlag           bFlag
//   0x1c   loop index i    (files, the std::vector)
//   0x20   files (16 B)    loop index i
//   0x30   parser          parser          (identical from here up)
//
// The two pairs are each swapped, so the fix is one ordering decision in MSVC's
// local-slot numbering, not a spelling. What is NOT a difference: the odd
// `mov al, byte ptr [esp+0x13]` / `mov byte ptr [esp+0x2c], al` pair at 0x434cca.
// That is the std::vector's own constructor: in this game's headers
// `vector(const _A& _Al = _A()) : allocator(_Al), _First(0), _Last(0), _End(0)`
// puts the empty `std::allocator` first, so the object is 16 bytes (allocator
// byte at +0, _First/_Last/_End at +4/+8/+0xc) and copying the default
// argument's temporary allocator is a one-byte copy out of a 4-byte temp slot.
// It is reproduced automatically by #include <vector>; do not add a local for it.
// Tried and all still 92.9%: declaring the loop index above the vector and
// using `for (i = 0; ...)`, declaring the count before the vector, giving the
// vector an explicit `std::allocator<Class_004c91a0>()` argument, and gpt-6's
// 768 header sets. MSVC5 does not number these slots in declaration order or in
// first-reference order, so the swap has to come from a change in the IL shape.
// GPT-6.1-sol refinement: a named std::allocator passed to the vector constructor
// left the checker at 92.9%, so the original default-construction form is retained.
// GPT-6.1-sol refinement: declaring count and i before files, initializing count
// to zero, and deriving count from end()-begin() did not improve the frame-slot
// permutation; end()-begin() instead reduced the score to 70.7%.
// deepseek-v4.1 follow-up: the five swapped offsets are not reachable from the
// declarations or from statement order. Declaring `int i;`/`int count;` before
// the vector, renaming them, swapping the DAT_005122e0 store with the count
// line, a while-loop form, and `for (int i = 0, count = files.size(); ...)`
// either keep the same permutation or break the bytes (while-loop 90.1%,
// statement swap 91.8%, for-init 86.8%). The homes come out in a fixed IL order
// (count, allocator temp, bFlag, files, i) that source reordering does not
// touch, so the swap needs a change in the IL shape that was not found here.

#include <string.h>
#include <vector>

class Class_004c2ea0;

class Class_004c9390 {
public:
    char* data;
    void FUN_004c9390();
};

class Class_004c91a0 {
public:
    char* ptr;
    ~Class_004c91a0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

class Class_004c2ea0 {
public:
    int field_0;
    void* current;
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_00435c00 {
public:
    int FUN_00436860(int type, Class_004c2ea0* parser, char* schema);
};

class Class_004618a0 {
public:
    void FUN_004618a0(int param);
};

extern char* DAT_005122d4;
extern int DAT_005122d8;
extern int DAT_005122dc;
extern int DAT_005122e0;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;
extern char* g_game;

void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __stdcall FUN_00491c80(int n);
void __stdcall FUN_004bca30(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
char* __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
char* __stdcall FUN_004bb0f0(char* name);
char* __stdcall FUN_004c5740(char* text);
void FUN_00453d40();

// FUNCTION: 0x434bf0
int __stdcall FUN_00434bf0(void** param_1, int param_2, int param_3)
{
    if (DAT_005122d4 != 0) {
        if (param_1 != 0) {
            if (param_3 != 0 && DAT_005122d8 == 0) {
                *param_1 = DAT_005122d4;
                DAT_005122d4 = 0;
            } else {
                char* p = (char*)FUN_004d83b0("MULTI MAPS", DAT_005122dc);
                *param_1 = p;
                memcpy(p, DAT_005122d4, DAT_005122dc);
            }
        }
        return DAT_005122e0;
    }

    FUN_00491c80(0x14);
    int bFlag;
    if (param_2 == 0) {
        bFlag = 1;
        if (*(int*)(*(int*)(g_game + 0x391e9)) != 3)
            bFlag = 0;
    } else {
        bFlag = 0;
    }
    int offset = 0;
    DAT_005122dc = 1;
    DAT_005122d4 = (char*)FUN_004d83b0("MULTI MAPS", 1);
    *(char*)DAT_005122d4 = 0;

    std::vector<Class_004c91a0> files;
    FUN_004bca30("Maps\\*.ota", 0, &files);
    DAT_005122e0 = 0;

    int count = files.size();
    for (int i = 0; i < count; i++) {
        char path[256];
        FUN_004290f0(path, "Maps", files[i].ptr, "OTA");
        Class_004c2ea0 parser;
        if (((Class_004c2f60*)&parser)->FUN_004c2f60(path) != 0
            && ((Class_00435c00*)(*(int*)(g_game + 0x391e9)))
                   ->FUN_00436860(3, &parser, 0) != 0) {
            char name[256];
            char lower[256];
            strcpy(name, files[i].ptr);
            FUN_004bb0f0(name);
            strcpy(lower, name);
            _strlwr(lower);
            char* src = FUN_004c5740(lower);
            if (_strcmpi(src, lower) == 0)
                src = name;
            int len = strlen(src) + 1;
            DAT_005122d4 = (char*)FUN_004d84a0(DAT_005122d4, "MULTI MAPS",
                                               len + DAT_005122dc);
            strcpy(DAT_005122d4 + offset, src);
            DAT_005122d4[offset + len] = 0;
            offset += len;
            DAT_005122dc += len;
            DAT_005122e0++;
            if (param_2 != 0)
                break;
        }
        if (bFlag != 0) {
            FUN_00453d40();
            if (DAT_00506dbc != 0)
                DAT_00513000.FUN_004618a0(0);
        }
    }
    FUN_00491c80(0x13);
    if (DAT_005122d8 == 0)
        DAT_005122d8 = (param_2 == 0);
    int result = FUN_00434bf0(param_1, param_2, param_3);
    return result;
}
