// Decompiled by Opus, GPT-6.1-sol, space-bunny-free, deepseek-v4.1-flash, Haiku,
// deepseek-v4.1, Claude Opus 5.5, Sonnet and GPT-6. Names are provisional.
// The debug library's image and symbol handling, and the performance status
// dialog: the pooled tree-node allocators, the loaded-image reader (the FPO
// records and the imagehlp symbol handler), the stack walkers and the
// call-stack formatter, the system information dump, and the performance
// window's settings and dialog procedure. The files of the module's fourth
// part, gathered in address order.
// Included only for its symbol count: the functions below match at this count.
#include <math.h>
#include <windows.h>
#include <yvals.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

// A 0x18-byte pooled node, allocated 0x155 at a time (one 0x2000 block).
struct Node_004ddc00 {
    Node_004ddc00* next;               // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x14 - 0x8];
    int field_14;                      // +0x14
};

extern void* DAT_00528a10;             // free list
extern void (*DAT_005289bc)();         // out-of-memory handler

static inline Node_004ddc00* AllocNode_004ddc00()
{
    if (DAT_00528a10 == 0) {
        Node_004ddc00* block;
        do {
            block = (Node_004ddc00*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        Node_004ddc00* head = (Node_004ddc00*)DAT_00528a10;
        for (int i = 0; i < 0x155; i++) {
            block->next = head;
            head = block;
            block++;
        }
        DAT_00528a10 = head;
    }
    Node_004ddc00* node = (Node_004ddc00*)DAT_00528a10;
    DAT_00528a10 = node->next;
    return node;
}

// A method that ignores `this`: its caller (0x4dbec0, an inlined tree insert
// after its std::_Lockit) sets ecx to the tree. Shaped like
// std::_Tree<...>::_Buynode(parent, colour) with a pooled allocator.
class Class_004ddc00 {
public:
    Node_004ddc00* FUN_004ddc00(int param_1, int param_2);
};

// FUNCTION: 0x4ddc00
Node_004ddc00* Class_004ddc00::FUN_004ddc00(int param_1, int param_2)
{
    Node_004ddc00* node = AllocNode_004ddc00();
    node->field_4 = param_1;
    node->field_14 = param_2;
    return node;
}

// std::_Tree<unsigned int, ...>::_Lbound(const key&) from MSVC 5's <xtree>,
// written out by hand: DAT_00528a50 is the tree's _Nil node, keys are
// unsigned ints compared with less<>.

struct Node_004ddc90 {
    Node_004ddc90* left;               // +0x0
    Node_004ddc90* parent;             // +0x4
    Node_004ddc90* right;              // +0x8
    unsigned int key;                  // +0xc
};

extern void* DAT_00528a50;             // the tree's _Nil node

struct Less_004ddc90 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_004ddc90 {
public:
    Less_004ddc90 compare;             // +0x0
    Node_004ddc90* head;               // +0x4

    Node_004ddc90* FUN_004ddc90(const unsigned int& key);
};

// FUNCTION: 0x4ddc90
Node_004ddc90* Class_004ddc90::FUN_004ddc90(const unsigned int& key)
{
    std::_Lockit lock;
    Node_004ddc90* x = head->parent;
    Node_004ddc90* y = head;
    while (x != DAT_00528a50) {
        if (compare(x->key, key))
            x = x->right;
        else
            y = x, x = x->left;
    }
    return y;
}

// A 0x40-byte pooled node, allocated 0x80 at a time.
struct Node_004ddce0 {
    Node_004ddce0* next;               // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x3c - 0x8];
    int field_3c;                      // +0x3c
};

extern void* DAT_005289e0;             // free list

static inline Node_004ddce0* AllocNode_004ddce0()
{
    if (DAT_005289e0 == 0) {
        Node_004ddce0* block;
        do {
            block = (Node_004ddce0*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        Node_004ddce0* head = (Node_004ddce0*)DAT_005289e0;
        for (int i = 0; i < 0x80; i++) {
            block->next = head;
            head = block;
            block++;
        }
        DAT_005289e0 = head;
    }
    Node_004ddce0* node = (Node_004ddce0*)DAT_005289e0;
    DAT_005289e0 = node->next;
    return node;
}

// A method that ignores `this`: its caller (0x4dc680, an inlined tree insert
// after its std::_Lockit) sets ecx to the tree. Shaped like
// std::_Tree<...>::_Buynode(parent, colour) with a pooled allocator.
class Class_004ddce0 {
public:
    Node_004ddce0* FUN_004ddce0(int param_1, int param_2);
};

// FUNCTION: 0x4ddce0
Node_004ddce0* Class_004ddce0::FUN_004ddce0(int param_1, int param_2)
{
    Node_004ddce0* node = AllocNode_004ddce0();
    node->field_4 = param_1;
    node->field_3c = param_2;
    return node;
}

// Pool allocator for the DAT_00528a10 free list: refills it 0x2000 bytes at a
// time (GlobalAlloc, retrying through the out-of-memory handler) by carving
// n-byte pieces, then pops one piece. Identical to 0x4dddf0 apart from the
// free list.

// A method that ignores `this`: its callers (0x4db610, 0x4dce60) set ecx
// to the tree whose nodes it allocates (the allocator sits at +0),
// pushing the node size (0x18).
class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

// FUNCTION: 0x4ddd70
void* Class_004ddd70::FUN_004ddd70(unsigned int n)
{
    if (DAT_00528a10 == 0) {
        unsigned int rem = 0x2000;
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (; rem >= n; rem -= n) {
            *(void**)block = DAT_00528a10;
            DAT_00528a10 = block;
            block += n;
        }
    }
    void* p = DAT_00528a10;
    DAT_00528a10 = *(void**)DAT_00528a10;
    return p;
}

// Pool allocator for the DAT_005289e0 free list: refills it 0x2000 bytes at a
// time (GlobalAlloc, retrying through the out-of-memory handler) by carving
// n-byte pieces, then pops one piece. Same shape as 0x4e2b60; 0x4ddce0 has
// an inlined copy for 0x40-byte nodes.

// A method that ignores `this`: its callers (0x4da8d0, 0x4dd430) set ecx
// to the tree whose nodes it allocates (the allocator sits at +0),
// pushing the node size (0x40).
class Class_004dddf0 {
public:
    void* FUN_004dddf0(unsigned int n);
};

// FUNCTION: 0x4dddf0
void* Class_004dddf0::FUN_004dddf0(unsigned int n)
{
    if (DAT_005289e0 == 0) {
        unsigned int rem = 0x2000;
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (; rem >= n; rem -= n) {
            *(void**)block = DAT_005289e0;
            DAT_005289e0 = block;
            block += n;
        }
    }
    void* p = DAT_005289e0;
    DAT_005289e0 = *(void**)DAT_005289e0;
    return p;
}

// Shaped like std::_Tree<...>::const_iterator::_Inc() from MSVC 5's <xtree>
// (step an iterator to the in-order successor) under a lock object;
// DAT_00528a50 is the tree's _Nil node. Compare 0x4dd710 and 0x4ddc90.

struct Node_004dde70 {
    Node_004dde70* left;               // +0x0
    Node_004dde70* parent;             // +0x4
    Node_004dde70* right;              // +0x8
};

static inline Node_004dde70* Min_004dde70(Node_004dde70* p)
{
    std::_Lockit lock;
    while (p->left != DAT_00528a50)
        p = p->left;
    return p;
}

class Class_004dde70 {
public:
    Node_004dde70* ptr;                // +0x0
    void FUN_004dde70();
};

// FUNCTION: 0x4dde70
void Class_004dde70::FUN_004dde70()
{
    std::_Lockit lock;
    if (ptr->right != DAT_00528a50)
        ptr = Min_004dde70(ptr->right);
    else {
        Node_004dde70* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}

class Class_004e1590 {
public:
    void OpenMappedFile(const char* fileName);
};

class MappedFile {
public:
    HANDLE hFile;     // +0x0
    HANDLE hMapping;  // +0x4
    void* view;       // +0x8
    int size;         // +0xc
    int state;        // +0x10

    MappedFile(const char* fileName);
    void CloseMappedFile();
};

// One FPO_DATA record (see 0x4de020).
struct Fpo_004de020 {
    unsigned int offStart;             // +0x0
    unsigned int procSize;             // +0x4
    unsigned int locals;               // +0x8
    unsigned short params;             // +0xc
    unsigned short flags;              // +0xe
};

class LoadedImage : public MappedFile {
public:
    HMODULE module;                        // +0x14
    unsigned int imageBase;                // +0x18
    IMAGE_DOS_HEADER* dosHeader;           // +0x1c
    IMAGE_NT_HEADERS* ntHeaders;           // +0x20
    IMAGE_DEBUG_DIRECTORY* debugDirs;      // +0x24
    int numDebugDirs;                      // +0x28

    LoadedImage(HMODULE m);
    ~LoadedImage();
    Fpo_004de020* GetFpoRecords();
};

// The loaded-image reader of the debug library; 0x4de0a0's function-local
// static builds the one at 0x528a78 lazily.
extern LoadedImage DAT_00528a78;

struct DebugDir_004ddfa0 {
    unsigned int characteristics;      // +0x00
    unsigned int timeDateStamp;        // +0x04
    unsigned short majorVersion;       // +0x08
    unsigned short minorVersion;       // +0x0a
    unsigned int type;                 // +0x0c
    unsigned int sizeOfData;           // +0x10
    unsigned int addressOfRawData;     // +0x14
    unsigned int pointerToRawData;     // +0x18
};

class Class_004ddfe0 {
public:
    char unknown_0[0x8];
    char* base;                        // +0x08, the mapped file
    char unknown_c[0x24 - 0xc];
    DebugDir_004ddfa0* debugDirs;      // +0x24
    int numDebugDirs;                  // +0x28

    unsigned int GetFpoRecordCount();
};

class Class_004de020 {
public:
    char unknown_0[0x18];
    unsigned int imageBase;            // +0x18
    Fpo_004de020* FindFpoRecord(unsigned int address);
};

// FUNCTION: 0x4ddf00
LoadedImage::LoadedImage(HMODULE m) : MappedFile(0)
{
    char path[1000];
    module = m;
    imageBase = (unsigned int)m;
    dosHeader = (IMAGE_DOS_HEADER*)m;
    if (GetModuleFileNameA(m, path, sizeof(path)))
        path[sizeof(path) - 1] = 0;
    else
        path[0] = 0;
    ((Class_004e1590*)this)->OpenMappedFile(path);
    ntHeaders = (IMAGE_NT_HEADERS*)((char*)dosHeader + dosHeader->e_lfanew);
    numDebugDirs = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size / sizeof(IMAGE_DEBUG_DIRECTORY);
    // Count stored before debugDirs is cleared: ntHeaders is reloaded for the sum.
    debugDirs = 0;
    if (numDebugDirs)
    {
        // base local and the second test pick the registers of the sum.
        char* base = (char*)imageBase;
        if (numDebugDirs)
            debugDirs = (IMAGE_DEBUG_DIRECTORY*)((char*)base + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
    }
}

// Returns the image's FPO records (debug directory type 3,
// IMAGE_DEBUG_TYPE_FPO), or 0 if it has none.

// The original calls this out of line from 0x4de020.
#pragma auto_inline(off)
// FUNCTION: 0x4ddfa0
Fpo_004de020* LoadedImage::GetFpoRecords()
{
    if (debugDirs == 0)
        return 0;
    for (int i = 0; i < numDebugDirs; i++) {
        if (debugDirs[i].Type == IMAGE_DEBUG_TYPE_FPO)
            return (Fpo_004de020*)((char*)view + debugDirs[i].PointerToRawData);
    }
    return 0;
}
#pragma auto_inline(on)

// The original calls this out of line from 0x4de020.
#pragma auto_inline(off)
// FUNCTION: 0x4ddfe0
unsigned int Class_004ddfe0::GetFpoRecordCount()
{
    if (debugDirs == 0)
        return 0;
    for (int i = 0; i < numDebugDirs; i++) {
        if (debugDirs[i].type == 3)
            return debugDirs[i].sizeOfData / sizeof(Fpo_004de020);
    }
    return 0;
}
#pragma auto_inline(on)

// FUNCTION: 0x4de020
Fpo_004de020* Class_004de020::FindFpoRecord(unsigned int address)
{
    Fpo_004de020* first = ((LoadedImage*)this)->GetFpoRecords();
    if (first == 0)
        return 0;
    int n = ((Class_004ddfe0*)this)->GetFpoRecordCount();
    if (n == 0)
        return 0;
    Fpo_004de020* last = first + n;
    Fpo_004de020* mid = first + n / 2;
    unsigned int rva = address - imageBase;
    while (first + 1 != last) {
        if (rva < mid->offStart)
            last = mid;
        else
            first = mid;
        mid = first + (last - first) / 2;
    }
    if (rva >= first->offStart && rva < first->offStart + first->procSize)
        return first;
    return 0;
}

// FUNCTION: 0x4de0a0
void __stdcall FUN_004de0a0(int unused, unsigned int address)
{
    static LoadedImage table(GetModuleHandleA(0));
    ((Class_004de020*)&table)->FindFpoRecord(address);
}

// FUNCTION: 0x4de0f0
void FUN_004de0f0()
{
    DAT_00528a78.CloseMappedFile();
}

extern void GetDebugLibInstance();

// FUNCTION: 0x4de100
void __stdcall FUN_004de100(int arg1, int arg2)
{
    GetDebugLibInstance();
}

// The imagehlp entry points, resolved by LoadImageHelp (0x4de180): the
// loader, the stack walker and the call-stack formatter each use a different
// signature of the same pointer.
typedef DWORD (__stdcall *SymSetOptions_004de180)(DWORD);
typedef BOOL (__stdcall *SymInitialize_004de180)(HANDLE, char*, DWORD);
typedef void (__stdcall *SymProc_004de180)(void);
typedef BOOL (__stdcall *StackWalk_004de700)(DWORD, HANDLE, HANDLE, void*, void*, void*, void*, void*, void*);

extern char DAT_00528ad8;
extern char DAT_00528adc;
extern HMODULE DAT_00528ae0;
extern SymSetOptions_004de180 DAT_00528ad0;
extern SymInitialize_004de180 DAT_00528ab8;
extern BOOL (__stdcall* DAT_00528abc)(HANDLE);
extern StackWalk_004de700 DAT_00528ac0;
extern void* DAT_00528ac4;
extern void* DAT_00528ac8;
extern SymProc_004de180 DAT_00528acc;
extern SymProc_004de180 DAT_00528ab4;
extern SymProc_004de180 DAT_00528ad4;

// The original calls this out of line from 0x4de180.
#pragma auto_inline(off)
// FUNCTION: 0x4de110
void UnloadImageHelp()
{
    DAT_00528ad8 = 0;
    if (DAT_00528ae0) {
        DAT_00528abc(GetCurrentProcess());
        FreeLibrary(DAT_00528ae0);
    }
    DAT_00528ae0 = 0;
    DAT_00528ad0 = 0;
    DAT_00528ab8 = 0;
    DAT_00528abc = 0;
    DAT_00528ac0 = 0;
    DAT_00528ac4 = 0;
    DAT_00528ac8 = 0;
    DAT_00528acc = 0;
    DAT_00528ab4 = 0;
    DAT_00528ad4 = 0;
}
#pragma auto_inline(on)

// A command-line switch: `on` is set from the default, then forced on or off
// by the switch strings. The empty inline destructor is what makes MSVC
// register the (empty) atexit thunk for the static local.
class Class_004d9fe0 {
public:
    char on;                           // +0x0
    Class_004d9fe0(char* name, int a, char def, char* onSwitch,
                   char* offSwitch, char* onSwitch2, char* offSwitch2);
    ~Class_004d9fe0() {}
};

// FUNCTION: 0x4de180
char __cdecl LoadImageHelp(char param)
{
    static Class_004d9fe0 imagehlp("imagehlp", 0, 1, "-enableimagehlp",
                                   "-disableimagehlp", 0, 0);
    char path[0x100];
    char symPath[0x3e8];
    // Declared bare and assigned 0 as a statement just before GetModuleFileNameA.
    char* searchPath;
    char* windir;
    char* slash;
    typedef int (__stdcall *GetProcAddress_004de180)(HMODULE, char*);
    GetProcAddress_004de180 getProcAddress = (GetProcAddress_004de180)GetProcAddress;
    HANDLE (__stdcall *getCurrentProcess)(void) = GetCurrentProcess;

    if (!param && !imagehlp.on)
        return 0;
    if (DAT_00528ad8)
        return DAT_00528adc;
    DAT_00528ad8 = 1;
    if (!(DAT_00528ae0 = LoadLibraryA("IMAGEHLP.DLL")))
        return 0;
    if (!(DAT_00528ad0 = (SymSetOptions_004de180)getProcAddress(DAT_00528ae0, "SymSetOptions")))
        return 0;
    if (!(DAT_00528ab8 = (SymInitialize_004de180)getProcAddress(DAT_00528ae0, "SymInitialize")))
        return 0;
    if (!(DAT_00528abc = (BOOL (__stdcall*)(HANDLE))getProcAddress(DAT_00528ae0, "SymCleanup")))
        return 0;
    if (!(DAT_00528ac0 = (StackWalk_004de700)getProcAddress(DAT_00528ae0, "StackWalk")))
        return 0;
    if (!(DAT_00528ac4 = (void*)getProcAddress(DAT_00528ae0, "SymFunctionTableAccess")))
        return 0;
    if (!(DAT_00528ac8 = (void*)getProcAddress(DAT_00528ae0, "SymGetModuleBase")))
        return 0;
    DAT_00528acc = (SymProc_004de180)getProcAddress(DAT_00528ae0, "SymGetSymFromAddr");
    DAT_00528ab4 = (SymProc_004de180)getProcAddress(DAT_00528ae0, "SymGetLineFromAddr");
    DAT_00528ad4 = (SymProc_004de180)getProcAddress(DAT_00528ae0, "UnDecorateSymbolName");
    DWORD symOpts = 4;
    if (DAT_00528ab4)
        symOpts = 0x14;
    DAT_00528ad0(symOpts);
    searchPath = 0;
    if (GetModuleFileNameA((HMODULE)searchPath, path, sizeof(path))) {
        windir = getenv("windir");
        if (windir) {
            if (strlen(path) + strlen(windir) < 0x3e8) {
                strcpy(symPath, windir);
                slash = strrchr(path, '\\');
                if (slash) {
                    *slash = 0;
                    strcat(symPath, ";");
                    strcat(symPath, path);
                }
                searchPath = symPath;
            }
        }
    }
    // The second SymInitialize passes this result as its last argument, not a literal.
    BOOL inited = DAT_00528ab8(getCurrentProcess(), searchPath, 1);
    if (!inited) {
        DAT_00528ac4 = (void*)FUN_004de0a0;
        DAT_00528ac8 = (void*)FUN_004de100;
        DAT_00528acc = 0;
        DAT_00528ab4 = 0;
        if (!DAT_00528ab8(getCurrentProcess(), searchPath, inited)) {
            GetLastError();
            UnloadImageHelp();
            DAT_00528ad8 = 1;
            return 0;
        }
    }
    DAT_00528adc = 1;
    return 1;
}

// FUNCTION: 0x4de4c0
void FUN_004de4c0(void)
{
}

// The original calls this out of line from 0x4dea00.
#pragma auto_inline(off)
// FUNCTION: 0x4de4d0
char FUN_004de4d0()
{
    static Class_004d9fe0 lines("imagehlplines", 1, 1, "-enableimagehlplines",
                                "-disableimagehlplines", 0, 0);
    if (lines.on && LoadImageHelp(1) && (DAT_00528ab4 || DAT_00528acc))
        return 1;
    return 0;
}
#pragma auto_inline(on)

// FUNCTION: 0x4de540
void FUN_004de540(void)
{
}

// The stack line the imagehlp lookup fills in.
struct Line_004de550 {
    DWORD SizeOfStruct;
    DWORD Key;
    DWORD LineNumber;
    DWORD FileName;
    DWORD Address;
};

typedef BOOL (__stdcall *SymGetLineFromAddr_004de550)(HANDLE, DWORD, DWORD*, Line_004de550*);

extern char DAT_00528aac;
extern HANDLE DAT_00528aa4;

// The original calls this out of line from 0x4dea00.
#pragma auto_inline(off)
// FUNCTION: 0x4de550
char __cdecl GetLineFromAddress(DWORD addr, Line_004de550* out, DWORD* err)
{
    if (DAT_00528ab4) {
        if (!(DAT_00528aac & 1)) {
            DAT_00528aac |= 1;
            DAT_00528aa4 = GetCurrentProcess();
        }
        // Aggregate-initialised inside this block, not hoisted: keeps the zero stores here.
        Line_004de550 line = { 0x14 };
        // No initialiser, zeroed after the Address store: fixes where the store lands.
        DWORD disp;
        line.Address = addr;
        disp = 0;
        if (((SymGetLineFromAddr_004de550)DAT_00528ab4)(DAT_00528aa4, addr, &disp, &line)) {
            *out = line;
            return 1;
        }
        *err = GetLastError();
        return 0;
    }
    *err = 0;
    return 0;
}
#pragma auto_inline(on)

// A chain of stack frames (each frame holds the next frame pointer and a
// return address), walked without imagehlp when a stack symbol lookup is
// unavailable.
char __cdecl IsOutsideStack(void* p, int size);
void __cdecl WalkStack(int param1, int param2, int param3, int param4,
                       int* param5, int param6, int* param7);

// FUNCTION: 0x4de600
void __cdecl WalkFrameChain(int* frame, int* stack, int eip, int skip, int* out, int max, int* count,
                          int* copy, int copyMax, int* copied)
{
    *count = 0;
    if (skip == 0) {
        out[*count] = eip;
        (*count)++;
    } else {
        skip--;
    }
    if (LoadImageHelp(0)) {
        WalkStack((int)frame, (int)stack, eip, skip + 1, out, max, count);
    } else {
        for (int* fp = frame; *count < max; fp = (int*)*fp) {
            if (IsOutsideStack(fp, 8))
                break;
            if (fp < stack)
                break;
            if (*fp <= (int)fp)
                break;
            if (fp[1] == 0)
                break;
            if (skip == 0) {
                out[*count] = fp[1];
                (*count)++;
            } else {
                skip--;
            }
        }
    }
    int i = 0;
    if (stack != 0) {
        for (i = 0; i < copyMax; i++) {
            if (IsOutsideStack(&stack[i], 4))
                break;
            copy[i] = stack[i];
        }
    }
    *copied = i;
    for (; i < copyMax; i++)
        copy[i] = 0;
}

// The STACKFRAME-ish record StackWalk fills in.
struct Frame_004de700 {
    int addrFrame;                             // +0x00
    int unknown_04;
    int flags0;                                // +0x08
    char unknown_0c[0x0c];
    int addrStack;                             // +0x18
    char unknown_1c[4];
    int flags1;                                // +0x20
    int handler;                               // +0x24
    char unknown_28[4];
    int flags2;                                // +0x2c
    char unknown_30[0x40];
};

extern char DAT_00528aa8;                     // "process handle read" flag
extern HANDLE DAT_00528ab0;                   // cached process handle

// FUNCTION: 0x4de700
void __cdecl WalkStack(int param1, int param2, int param3, int param4, int* param5, int param6, int* param7)
{
    Frame_004de700 frame;
    memset(&frame, 0, sizeof(frame));
    frame.addrFrame = param3;
    frame.flags0 = 3;
    frame.flags2 = 3;
    frame.flags1 = 3;
    frame.handler = param2;
    frame.addrStack = param1;
    if (!(DAT_00528aa8 & 1)) {
        DAT_00528aa8 |= 1;
        DAT_00528ab0 = GetCurrentProcess();
    }
    HANDLE thread = GetCurrentThread();
    while (*param7 < param6) {
        if (!DAT_00528ac0(0x14c, DAT_00528ab0, thread, &frame, 0, 0, DAT_00528ac4, DAT_00528ac8, 0)) {
            if (*param7 == 0) {
                *param7 = 1;
                param5[0] = param3;
            }
            return;
        }
        if (frame.addrStack == 0)
            return;
        if (frame.addrFrame == 0)
            return;
        if (param4 == 0) {
            param5[*param7] = frame.addrFrame;
            ++(*param7);
        } else {
            --param4;
        }
    }
}

// The original calls this out of line from 0x4de8a0.
#pragma auto_inline(off)
// FUNCTION: 0x4de810
bool __cdecl FileExists(char* dir, char* name)
{
    struct _stat st;
    char path[1000];
    strcpy(path, dir);
    strcat(path, name);
    return _stat(path, &st) == 0 ? true : false;
}
#pragma auto_inline(on)

// Builds the full path of a file that lives in the Visual C++ source tree.
// If the name has no directory part, the Visual Studio install directory is
// found once from the MSDevDir environment variable, cut down to its drive
// (everything from the first backslash on, and any ';' comment, is dropped),
// and the two usual source roots are tried in turn, MFC first, then the CRT.
// DAT_00528ae4 is the "MSDevDir already read" flag, DAT_00528ae8 the CRT root,
// DAT_00528ed0 the MFC root, DAT_005119b8 the empty default prefix.
extern char DAT_005119b8[];
extern char DAT_00528ae4;
extern char DAT_00528ae8[0x1e8];
extern char DAT_00528ed0[0x1e8];

// The original calls this out of line from 0x4dea00.
#pragma auto_inline(off)
// FUNCTION: 0x4de8a0
void __cdecl GetSourceFilePath(char* out, char* name)
{
    char* dir = DAT_005119b8;
    if (strchr(name, '\\') == 0) {
        if (!DAT_00528ae4) {
            DAT_00528ae4 = 1;
            char* msdev = getenv("MSDevDir");
            if (msdev) {
                strcpy(DAT_00528ae8, msdev);
                char* p = strchr(DAT_00528ae8, ';');
                if (p)
                    *p = 0;
                p = strrchr(DAT_00528ae8, '\\');
                if (p)
                    *p = 0;
                strcpy(DAT_00528ed0, DAT_00528ae8);
                strcat(DAT_00528ae8, "\\vc\\crt\\src\\");
                strcat(DAT_00528ed0, "\\vc\\mfc\\src\\");
            }
        }
        if (FileExists(DAT_00528ed0, name)) {
            dir = DAT_00528ed0;
        } else if (FileExists(DAT_00528ae8, name)) {
            dir = DAT_00528ae8;
        }
    }
    sprintf(out, "%s%s", dir, name);
}
#pragma auto_inline(on)

// The imagehlp symbol record FormatCallStack fills in.
struct Sym_004dea00 {
    unsigned long SizeOfStruct;
    unsigned long Address;
    unsigned long Size;
    unsigned long Flags;
    unsigned long MaxNameLength;
    char Name[0x204];
};

typedef BOOL (__stdcall *SymFn_004dea00)(HANDLE, unsigned long, int*, void*);
typedef DWORD (__stdcall *UnDecFn_004dea00)(char*, char*, DWORD, DWORD);

// FUNCTION: 0x4dea00
void __cdecl FormatCallStack(char* dest, int space, int per, int n, unsigned long* addrs)
{
    int i = 0;
    space--;
    char lines = FUN_004de4d0();
    int width;
    int disp;
    DWORD err;
    Line_004de550 line;
    Sym_004dea00 sym;
    char path[1000];
    char undec[2000];

    strcpy(dest, lines ? "Call stack:\n" : "Call stack: ");
    width = 15;
    space -= strlen(dest);
    dest += strlen(dest);
    if (lines)
        width = 800;

    char found = 0;
    for (; i < n; i++) {
        if (space <= width)
            break;
        if (lines) {
            if (GetLineFromAddress(addrs[i], &line, &err)) {
                GetSourceFilePath(path, (char*)line.FileName);
                sprintf(dest, "%s(%d) : %08lX", path, line.LineNumber, addrs[i]);
                found = 1;
            } else {
                sprintf(dest, "%08lX", addrs[i]);
            }
            sym.SizeOfStruct = 0x218;
            sym.MaxNameLength = 0x200;
            disp = 0;
            if (((SymFn_004dea00)DAT_00528acc)(GetCurrentProcess(), addrs[i], &disp, &sym)) {
                char* str;
                if (DAT_00528ad4 && ((UnDecFn_004dea00)DAT_00528ad4)(sym.Name, undec, 0x7d0, 0) > 0) {
                    str = undec;
                } else {
                    if (strcmp(sym.Name, "??2@YAPAXI@Z") == 0 ||
                        strcmp(sym.Name, "??2@YAPAXIPBDH@Z") == 0)
                        strcat(sym.Name, " - operator new");
                    str = sym.Name;
                }
                char* end = dest + strlen(dest);
                sprintf(end, " - %s + %d", str, disp);
                found = 1;
            }
            // One strcat per branch, no shared `sep` variable: moves the temp register rotation.
            if (found)
                strcat(dest, "\n");
            else if (i == n - 1 || i % per == per - 1)
                strcat(dest, "\n");
            else
                strcat(dest, " ");
        } else {
            sprintf(dest, "%08lX", addrs[i]);
            if (i == n - 1 || i % per == per - 1)
                strcat(dest, "\n");
            else
                strcat(dest, " ");
        }
        space -= strlen(dest);
        dest += strlen(dest);
    }
}

extern char* DAT_0050d4d0;
extern "C" int __cdecl GetStackLow(void);
extern "C" void* __cdecl GetStackHigh(void);

// FUNCTION: 0x4ded60
void __cdecl FormatSystemInfo(char* dest, int destLen)
{
    WORD fatDate;
    time_t now;
    WORD fatTime;
    DWORD userNameSize;
    FILETIME ft;
    MEMORYSTATUS memStatus;
    SYSTEM_INFO sysInfo;
    struct tm gmTimeCopy;
    struct tm localTimeCopy;
    char userName[300];
    char buf[2000];
    char exeName[1000];
    char* p;

    buf[0] = DAT_005119b8[0];
    memset(buf + 1, 0, 1999);

    now = time(NULL);
    localTimeCopy = *localtime(&now);
    p = buf + strlen(buf);
    sprintf(p, "Time: %s", asctime(&localTimeCopy));

    userNameSize = 300;
    if (GetUserNameA(userName, &userNameSize) == 0) {
        strcpy(userName, "unknown user");
    }

    char* machine = getenv("computername");
    if (machine == NULL) {
        machine = "unknown machine";
    }

    if (GetModuleFileNameA(NULL, exeName, 1000) == 0) {
        strcpy(exeName, "Unknown");
    }

    sprintf(buf + strlen(buf), "%s, run by %s on %s\n", exeName, userName, machine);

    HANDLE hFile = CreateFileA(exeName, GENERIC_READ, FILE_SHARE_READ, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != 0) {
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (GetFileTime(hFile, NULL, NULL, &ft)) {
            if (FileTimeToLocalFileTime(&ft, &ft)) {
                if (FileTimeToDosDateTime(&ft, &fatDate, &fatTime)) {
                    p = buf + strlen(buf);
                    sprintf(p,
                        "Executable is %d bytes long and dated %d/%d/%d %02d:%02d:%02d\n",
                        fileSize, (fatDate >> 5) & 0xf, fatDate & 0x1f, (fatDate >> 9) + 1980,
                        fatTime >> 11, (fatTime >> 5) & 0x3f, (fatTime & 0x1f) * 2);
                }
            }
        }
        CloseHandle(hFile);
    }

    HANDLE hMod = GetModuleHandleA(NULL);
    // NT header named and used twice: forces it into a register.
    IMAGE_NT_HEADERS* pNT = (IMAGE_NT_HEADERS*)((char*)hMod + ((IMAGE_DOS_HEADER*)hMod)->e_lfanew);
    gmTimeCopy = *gmtime((time_t*)&pNT->FileHeader.TimeDateStamp);
    p = buf + strlen(buf);
    sprintf(p, "UTC link time: %08lx - %s", pNT->FileHeader.TimeDateStamp, asctime(&gmTimeCopy));

    p = buf + strlen(buf);
    sprintf(p, "Library version %d. Library date %s\n",
            996, DAT_0050d4d0 + strlen("Library date stamp: "));

    GetSystemInfo(&sysInfo);
    if (sysInfo.dwNumberOfProcessors > 1) {
        // Then arm keeps the p temporary, else arm calls sprintf directly: not interchangeable.
        p = buf + strlen(buf);
    sprintf(p, "%d processors\n", sysInfo.dwNumberOfProcessors);
    } else {
        sprintf(buf + strlen(buf), "1 processor\n");
    }

    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatus(&memStatus);
    p = buf + strlen(buf);
    sprintf(p, "%d MBytes physical memory\n",
            (memStatus.dwTotalPhys + 900000) >> 20);

    p = buf + strlen(buf);
    sprintf(p, "Stack goes from %08lX to %08lX\n",
            GetStackLow(), GetStackHigh());

    strncpy(dest, buf, destLen);
    dest[destLen - 1] = '\0';
}

// FUNCTION: 0x4df160
void FUN_004df160()
{
    static Class_004d9fe0 fussy("fpufussy", 1, 0, "-fpufussy", "-fpunofussy", 0, 0);
    if (fussy.on)
        _controlfp(0, _EM_ZERODIVIDE | _EM_INVALID);
    else
        _controlfp(_EM_ZERODIVIDE | _EM_INVALID, _EM_ZERODIVIDE | _EM_INVALID);
}

// FUNCTION: 0x4df1d0
void FUN_004df1d0(void)
{
}

// The performance status window and the name table it shows. The singleton at
// 0x5292d0 is the Class_004df1e0 0x4df1e0 builds and 0x4dfd10 hands out; its
// name map at +0x21c is the same tree the global name table uses, so the two
// share the node and iterator types below. 0x4dfd10 and 0x4dfd50 stay in
// their own files: the singleton's atexit term function is that file's first
// static, and 0x4dfd50 is the name (_$E2) the placement build knows.

extern int DAT_00529dcc;
extern void* DAT_00529df8;

extern double GetTimeSeconds();
void InitPerformanceEvents();

// The map value: the name key and its 500-byte text.
struct Value_004df590 {
    const char* name;                  // +0x00
    char text[500];                    // +0x04
};

struct Node_004df590 {
    Node_004df590* left;               // +0x0
    Node_004df590* parent;             // +0x4
    Node_004df590* right;              // +0x8
    Value_004df590 value;              // +0xc
};

// The map's node, as the erase machinery sees it: the 0x1f8-byte value and
// the red/black colour at +0x204.
struct Node_004dfea0 {
    Node_004dfea0* _Left;              // +0x0
    Node_004dfea0* _Parent;            // +0x4
    Node_004dfea0* _Right;             // +0x8
    char _Value[0x1f8];                // +0xc
    int _Color;                        // +0x204  (0 = _Red, 1 = _Black)
};

extern Node_004dfea0* DAT_005292c4;    // tree _Nil
extern void* DAT_00529e58;             // node free list
extern unsigned int DAT_00529500;      // tree _Nilrefs

// The tree's in-order successor, as 0x4df590 walks the name map.
struct Iterator_004df590 {
    Node_004df590* ptr;
    Iterator_004df590(Node_004df590* p) : ptr(p) {}
    bool operator==(const Iterator_004df590& other) const { return ptr == other.ptr; }
    bool operator!=(const Iterator_004df590& other) const { return !(*this == other); }
};

// The tree iterator; passed by value and returned by value, so the caller
// supplies a hidden return buffer.
struct Node_004e0450;
class Class_004e0450 {
public:
    Node_004e0450* ptr;                // +0x0

    void FUN_004e0450();               // _Inc
    Class_004e0450& operator++() { FUN_004e0450(); return *this; }
    Class_004e0450 operator++(int)
    {
        Class_004e0450 t = *this;
        ++*this;
        return t;
    }
    Node_004dfea0* _Mynode() const { return (Node_004dfea0*)ptr; }
    bool operator==(const Class_004e0450& x) const { return ptr == x.ptr; }
    bool operator!=(const Class_004e0450& x) const { return !(*this == x); }
};

// std::_Tree<...>::_Erase(_Nodeptr): frees a whole subtree.
struct Node_004e03f0;
class Class_004e03f0 {
public:
    void FUN_004e03f0(Node_004e03f0* x);
};

// std::_Tree<...>::erase(iterator): erases one node, returns the next.
class Class_004dfea0 {
public:
    char _Alnod[4];                    // +0x0
    Node_004dfea0* _Head;              // +0x4
    char _Multi;                       // +0x8
    char pad_9[3];
    unsigned int _Size;                // +0xc

    static int& _Color(Node_004dfea0* _P) { return _P->_Color; }
    static Node_004dfea0*& _Left(Node_004dfea0* _P) { return _P->_Left; }
    static Node_004dfea0*& _Parent(Node_004dfea0* _P) { return _P->_Parent; }
    static Node_004dfea0*& _Right(Node_004dfea0* _P) { return _P->_Right; }
    Node_004dfea0*& _Root() const { return _Head->_Parent; }
    Node_004dfea0*& _Lmost() const { return _Head->_Left; }
    Node_004dfea0*& _Rmost() const { return _Head->_Right; }

    static Node_004dfea0* _Min(Node_004dfea0* _P)
    {
        std::_Lockit _Lk;
        while (_Left(_P) != DAT_005292c4)
            _P = _Left(_P);
        return (_P);
    }
    static Node_004dfea0* _Max(Node_004dfea0* _P)
    {
        std::_Lockit _Lk;
        while (_Right(_P) != DAT_005292c4)
            _P = _Right(_P);
        return (_P);
    }
    void _Lrotate(Node_004dfea0* _X)
    {
        std::_Lockit _Lk;
        Node_004dfea0* _Y = _Right(_X);
        _Right(_X) = _Left(_Y);
        if (_Left(_Y) != DAT_005292c4)
            _Parent(_Left(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Left(_Parent(_X)))
            _Left(_Parent(_X)) = _Y;
        else
            _Right(_Parent(_X)) = _Y;
        _Left(_Y) = _X;
        _Parent(_X) = _Y;
    }
    void _Rrotate(Node_004dfea0* _X)
    {
        std::_Lockit _Lk;
        Node_004dfea0* _Y = _Left(_X);
        _Left(_X) = _Right(_Y);
        if (_Right(_Y) != DAT_005292c4)
            _Parent(_Right(_Y)) = _X;
        _Parent(_Y) = _Parent(_X);
        if (_X == _Root())
            _Root() = _Y;
        else if (_X == _Right(_Parent(_X)))
            _Right(_Parent(_X)) = _Y;
        else
            _Left(_Parent(_X)) = _Y;
        _Right(_Y) = _X;
        _Parent(_X) = _Y;
    }
    static void _Freenode(Node_004dfea0* _P)
    {
        if (_P != 0) {
            *(void**)_P = DAT_00529e58;
            DAT_00529e58 = _P;
        }
    }

    Class_004e0450 FUN_004dfea0(Class_004e0450 _P);
};

// The name map: comparator and allocator bytes, _Head, _Multi, _Size and the
// "changed" flag at +0x10. It is the map member of the global NameTable and
// of the performance singleton; its destructor is the pooled tree teardown
// the atexit term function 0x4dfd50 runs.
struct Map_004df590 {
    char compare;                      // +0x0
    char allocator;                    // +0x1
    Node_004df590* head;               // +0x4
    char multi;                        // +0x8
    unsigned int size;                 // +0xc
    char changed;                      // +0x10

    Node_004df590*& _Root() const { return head->parent; }
    Node_004df590*& _Lmost() const { return head->left; }
    Node_004df590*& _Rmost() const { return head->right; }
    unsigned int Size() const { return size; }
    Class_004e0450 begin() const { Class_004e0450 i; i.ptr = (Node_004e0450*)head->left; return i; }
    Class_004e0450 end() const { Class_004e0450 i; i.ptr = (Node_004e0450*)head; return i; }

    // Shaped exactly like the MSVC 5 STL, including the dead `_F != begin()` test.
    Class_004e0450 erase(Class_004e0450 _F, Class_004e0450 _L)
    {
        if (Size() == 0 || _F != begin() || _L != end()) {
            while (_F != _L)
                ((Class_004dfea0*)this)->FUN_004dfea0(_F++);
            return _F;
        } else {
            std::_Lockit Lk;
            ((Class_004e03f0*)this)->FUN_004e03f0((Node_004e03f0*)_Root());
            _Root() = (Node_004df590*)DAT_005292c4;
            size = 0;
            _Lmost() = head;
            _Rmost() = head;
            return begin();
        }
    }

    ~Map_004df590()
    {
        erase(begin(), end());
        // Loaded nodes kept in locals (h, n) for the free-list push: re-reading shifts registers.
        Node_004df590* h = head;
        if (h != 0) {
            *(void**)h = DAT_00529e58;
            DAT_00529e58 = h;
        }
        head = 0, size = 0;
        {
            std::_Lockit Lk;
            if (--DAT_00529500 == 0) {
                Node_004df590* n = (Node_004df590*)DAT_005292c4;
                if (n != 0) {
                    *(void**)n = DAT_00529e58;
                    DAT_00529e58 = n;
                }
                DAT_005292c4 = 0;
            }
        }
    }
};

// The global name table (0x4e17c0 builds it, 0x4e1a90 hands it out).
class NameTable {
public:
    Map_004df590 names;                // +0x0

    NameTable();
};

NameTable* GetNameTable();

class CriticalSection {
public:
    CRITICAL_SECTION cs;
};

CriticalSection* FUN_004e1ac0();

class Class_004e18c0 {
public:
    void FUN_004e18c0();
};

class Class_004e1990 {
public:
    void FUN_004e1990(void* key);
};

struct Entry_004df590 {
    int field_0;                       // +0x0
    LPARAM text;                       // +0x4
    int flags_8;                       // +0x8
    char* name;                        // +0xc
};

extern Entry_004df590 DAT_00529e00[];
extern char* DAT_0050d660;
extern unsigned char DAT_00529dd8;
extern unsigned char DAT_00529dd4;
extern unsigned char DAT_00529ddc;
extern unsigned char DAT_00529e64;
extern unsigned char DAT_00529dc8;

void __cdecl SyncPerformanceSettings(int flag);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);
void __cdecl OpenUrl(HWND hwnd, const char* url, const char* ext);
void __cdecl FUN_004e33d0(HWND hwnd, char* name, double a, double b);

class Class_004df280 {
public:
    HWND hwnd;                         // +0x00
    char unknown_4[0x1c];
    unsigned char flag_20;             // +0x20

    void SetPerformanceWindowVisible(char show);
};

class Class_004df380 {
public:
    HWND hwnd;                          // +0x00
    char unknown_4[0x20];
    const char* name;                   // +0x24

    // The key test, with the pointer compare the original does first: the
    // key is loaded before this->name, which is the order the code needs.
    bool Same(const char* key)
    {
        return key == name || strcmp(key, name) == 0;
    }
    void FUN_004df380();
};

class Class_004df4e0 {
public:
    HWND hwnd;                          // +0x00
    void FUN_004df4e0();
};

class Id_004df1e0 {
public:
    const char* id;                    // +0x0
    char text[0x1f4];                  // +0x4
    Id_004df1e0(const char* s)
    {
        id = s;
        if (id == 0)
            id = DAT_005119b8;
        text[0] = 0;
    }
};

class Class_004df1e0 {
public:
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    int unknown_8;                     // +0x8
    int unknown_c;                     // +0xc
    double time;                       // +0x10
    int count;                         // +0x18
    void* table;                       // +0x1c
    char flag_20;                      // +0x20
    Id_004df1e0 ident;                 // +0x24
    NameTable map;                     // +0x21c

    Class_004df1e0();
    ~Class_004df1e0() {}
};

class PerformanceDialog {
public:
    HWND hwnd;                         // +0x00
    int left;                          // +0x04
    int top;                           // +0x08
    char unknown_0c[0xc];
    int count;                         // +0x18
    Entry_004df590* entries;           // +0x1c
    char flag_20;                      // +0x20
    char unknown_21[3];
    Value_004df590 selected;           // +0x24
    Map_004df590 set;                  // +0x21c

    BOOL HandlePerformanceMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void CreatePerformanceDialog(void);
};

HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK PerformanceDlgProc(HWND, UINT, WPARAM, LPARAM);

static inline Node_004df590* Min_004df590(Node_004df590* p)
{
    std::_Lockit lock;
    while (p->left != (Node_004df590*)DAT_005292c4)
        p = p->left;
    return p;
}

static inline bool NamesEqual_004df590(const char* a, const char* b) { return a == b || strcmp(a,b) == 0; }

// The original calls this out of line from 0x4dfd10.
#pragma auto_inline(off)
// FUNCTION: 0x4df1e0
Class_004df1e0::Class_004df1e0()
    : ident("This is a unique identifier, isn't it - tell me the truth!")
{
    unknown_0 = 0;
    unknown_4 = -1;
    unknown_8 = -1;
    flag_20 = 0;
    InitPerformanceEvents();
    count = DAT_00529dcc;
    table = DAT_00529df8;
    time = GetTimeSeconds();
    ((PerformanceDialog*)this)->CreatePerformanceDialog();
}
#pragma auto_inline(on)

// The original calls this out of line from 0x4df1e0.
#pragma auto_inline(off)
// FUNCTION: 0x4df250
void PerformanceDialog::CreatePerformanceDialog(void)
{
    if (CreateDialogFromTemplate(0x67, GetDesktopWindow(), (DLGPROC)PerformanceDlgProc, (LPARAM)this) == 0) {
        MessageBoxA(0, "Performance dialog failed to open", "Cavedog", 0);
    }
}
#pragma auto_inline(on)

// The original calls this out of line from 0x4df590 and 0x4dfd00.
#pragma auto_inline(off)
// FUNCTION: 0x4df280
void Class_004df280::SetPerformanceWindowVisible(char show)
{
    if (show) {
        if (hwnd) {
            EnableWindow(hwnd, 1);
            SetFocus(hwnd);
            SetForegroundWindow(hwnd);
            FUN_004e33d0(hwnd, DAT_0050d660, 1.0, 1.0);
            SetTimer(hwnd, 1, 200, 0);
        } else {
            flag_20 = 1;
        }
    } else if (IsWindowVisible(hwnd)) {
        KillTimer(hwnd, 1);
        SaveWindowPosition(hwnd, DAT_0050d660);
        ShowWindow(hwnd, 0);
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x4df330
BOOL __stdcall PerformanceDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_INITDIALOG) {
        SetWindowLongA(hwnd, GWL_USERDATA, lParam);
        ((PerformanceDialog*)lParam)->hwnd = hwnd;
    }
    PerformanceDialog* obj = (PerformanceDialog*)GetWindowLongA(hwnd, GWL_USERDATA);
    if (obj)
        return obj->HandlePerformanceMessage(msg, wParam, lParam);
    return 0;
}

// The name table's tree nodes as 0x4df380 walks them: the same bytes as
// Node_004df590 (key at +0xc, 500-byte text at +0x10, colour at +0x204).
struct Value_004df380 {
    char text[500];                     // +0x00
};

struct Node_004df380 {
    Node_004df380* left;               // +0x0
    Node_004df380* parent;             // +0x4
    Node_004df380* right;              // +0x8
    const char* first;                 // +0xc
    Value_004df380 second;             // +0x10
    int color;                         // +0x204
};

Node_004df380* __cdecl FUN_004e04e0(Node_004df380* p);

class Iter_004df380 {
public:
    Node_004df380* ptr;

    Iter_004df380() {}
    Iter_004df380(Node_004df380* p) : ptr(p) {}
    Iter_004df380& operator++() { _Inc(); return *this; }
    bool operator==(const Iter_004df380& x) const { return ptr == x.ptr; }
    bool operator!=(const Iter_004df380& x) const { return !(*this == x); }
    // The tree's iterator increment, out of <xtree>. The while loop is
    // unrolled twice and the trailing "if" is the tree's own test, which
    // keeps the parent from being stored when the node is a left child.
    void _Inc()
    {
        std::_Lockit lk;
        if (ptr->right != (Node_004df380*)DAT_005292c4)
            ptr = FUN_004e04e0(ptr->right);
        else {
            Node_004df380* p;
            while (ptr == (p = ptr->parent)->right)
                ptr = p;
            if (ptr->right != p)
                ptr = p;
        }
    }
};

class Map_004df380 {
public:
    char compare;                      // +0x0
    char allocator;                    // +0x1
    Node_004df380* head;               // +0x4
    char multi;                        // +0x8
    int size;                          // +0xc
    char changed;                      // +0x10

    Iter_004df380 begin() { return Iter_004df380(head->left); }
    Iter_004df380 end() { return Iter_004df380(head); }
};

// FUNCTION: 0x4df380
void Class_004df380::FUN_004df380()
{
    CriticalSection* lock = FUN_004e1ac0();
    EnterCriticalSection(&lock->cs);
    Map_004df380* map = (Map_004df380*)&GetNameTable()->names;
    for (Iter_004df380 it = map->begin(); it != map->end(); ++it) {
        if (Same(it.ptr->first)) {
            char buf[500];
            if (GetDlgItemTextA(hwnd, 0x3f0, buf, 500) == 0
                || strcmp(buf, it.ptr->second.text) != 0)
                SetDlgItemTextA(hwnd, 0x3f0, it.ptr->second.text);
            break;
        }
    }
    LeaveCriticalSection(&lock->cs);
}

unsigned char __cdecl HasPerfCounters(void);

// FUNCTION: 0x4df4e0
void Class_004df4e0::FUN_004df4e0()
{
    unsigned char b = HasPerfCounters() && DAT_00529dd8;
    EnableWindow(GetDlgItem(hwnd, 0x3f5), b);
    EnableWindow(GetDlgItem(hwnd, 0x3ed), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f3), b);
    EnableWindow(GetDlgItem(hwnd, 0x3ee), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f1), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f6), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f7), b);
    EnableWindow(GetDlgItem(hwnd, 0x3f8), b);
}

// FUNCTION: 0x4df590
BOOL PerformanceDialog::HandlePerformanceMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Case order WM_COMMAND, WM_TIMER, WM_INITDIALOG: decides register ids and frame slots.
    switch (msg) {
    case 0x111: {
        int id = LOWORD(wParam);
        switch (id) {
        case IDOK:
        case IDCANCEL:
            ((Class_004df280*)this)->SetPerformanceWindowVisible(0);
            return 0;

        case 0x3ed:
        case 0x3ee: {
                // HIWORD(wParam) for the notification tests: keeps the shr.
                if (HIWORD(wParam) != CBN_SELCHANGE)
                    return 0;
                int b = 0;
                if (LOWORD(wParam) == 0x3ee) b = 1;
                int sel = (int)SendDlgItemMessageA(hwnd, id, 0x147, 0, 0);
                int mask = 1 << b;
                int i = 0;
                for (int j = 0; j < count; j++) {
                    if (entries[j].flags_8 & mask) {
                        if (i == sel) {
                            DAT_00529e00[b] = entries[j];
                            if (DAT_00529dc8 && entries[j].name != 0) {
                                int nmask = ~mask;
                                int n = 0;
                                for (int k = 0; k < count; k++) {
                                    if (entries[k].flags_8 & nmask) {
                                        if (entries[k].field_0 == (int)entries[j].name) {
                                            DAT_00529e00[1 - b] = entries[k];
                                            SendDlgItemMessageA(hwnd,
                                                (LOWORD(wParam) == 0x3ed) ? 0x3ee : 0x3ed,
                                                0x14e, n, 0);
                                        }
                                        n++;
                                    }
                                }
                            }
                            sel = -1;
                        }
                        i++;
                    }
                }
                SyncPerformanceSettings(0);
                return 0;
            }

            case 0x3fa:
                OpenUrl(hwnd,
                    "http://10.0.150.18/programming/library/extras/performancestatusdialog.html",
                    ".htm");
                return 0;

            case 0x3ef:
                DAT_00529dd8 = (DAT_00529dd8 == 0);
                SyncPerformanceSettings(0);
                ((Class_004df4e0*)this)->FUN_004df4e0();
                return 0;

            case 0x3f1:
                DAT_00529dd4 = (DAT_00529dd4 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f6:
                DAT_00529ddc = (DAT_00529ddc == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f7:
                DAT_00529e64 = (DAT_00529e64 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f8:
                DAT_00529dc8 = (DAT_00529dc8 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f4: {
                if (HIWORD(wParam) != LBN_SELCHANGE)
                    return 0;
                int sel = (int)SendDlgItemMessageA(hwnd, id, 0x188, 0, 0);
                int j = 0;
                Node_004df590* node = set.head->left;
                while (Iterator_004df590(node) != Iterator_004df590(set.head)) {
                    if (j == sel) {
                        selected = *(Value_004df590*)((char*)node + 0xc);
                    }
                    j++;
                    {
                        // Min_004df590 inlined with node->right as argument: loads it before the lock.
                        std::_Lockit lock;
                        if (node->right != (Node_004df590*)DAT_005292c4) {
                            node = Min_004df590(node->right);
                        } else {
                            Node_004df590* p;
                            while (node == (p = node->parent)->right)
                                node = p;
                            if (node->right != p)
                                node = p;
                        }
                    }
                }
                ((Class_004df380*)this)->FUN_004df380();
                return 0;
            }
        }
        return 0;
    }

    case 0x113: {
        if (!IsWindowVisible(hwnd))
            return 0;
        RECT rect;
        GetWindowRect(hwnd, &rect);
        if (rect.left != left || rect.top != top) {
            left = rect.left;
            top = rect.top;
            SaveWindowPosition(hwnd, DAT_0050d660);
        }
        CriticalSection* cs = FUN_004e1ac0();
        EnterCriticalSection(&cs->cs);
        NameTable* info = GetNameTable();
        if (info->names.changed) {
            int sel = -1;
            int n = 0;
            Node_004df590* node = info->names.head->left;
            SendDlgItemMessageA(hwnd, 0x3f4, 0x184, 0, 0);
            ((Class_004e18c0*)&set)->FUN_004e18c0();
            // Guarded do-while: a while or for loop moves the loop registers.
            if (Iterator_004df590(node) != Iterator_004df590(info->names.head)) {
                do {
                    ((Class_004e1990*)&set)->FUN_004e1990(&node->value);
                    SendDlgItemMessageA(hwnd, 0x3f4, 0x180, 0, (LPARAM)node->value.name);
                    if (NamesEqual_004df590(node->value.name, selected.name))
                        sel = n;
                    n++;
                    ((Class_004e0450*)&node)->FUN_004e0450();
                } while (Iterator_004df590(node) != Iterator_004df590(info->names.head));
            }
            if (sel >= 0)
                SendDlgItemMessageA(hwnd, 0x3f4, 0x186, sel, 0);
            info->names.changed = 0;
        }
        ((Class_004df380*)this)->FUN_004df380();
        LeaveCriticalSection(&cs->cs);
        return 0;
    }

    case 0x110: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        left = rect.left;
        top = rect.top;
        for (int i = 0; i < 2; i++) {
            int mask = 1 << i;
            int id = 0x3ed;
            // Branch, not arithmetic: stops strength reduction of DAT_00529e00[i].
            if (i == 1)
                id = 0x3ee;
            int sel = 0;
            int n = 0;
            for (int j = 0; j < count; j++) {
                Entry_004df590* e = &entries[j];
                if (entries[j].flags_8 & mask) {
                    if (e->field_0 == DAT_00529e00[i].field_0)
                        sel = n;
                    n++;
                    SendDlgItemMessageA(hwnd, id, 0x143, 0, e->text);
                }
            }
            SendDlgItemMessageA(hwnd, id, 0x14e, sel, 0);
        }
        RegisterHotKey(hwnd, 10, 1, 0x24);
        CheckDlgButton(hwnd, 0x3ef, DAT_00529dd8);
        CheckDlgButton(hwnd, 0x3f1, DAT_00529dd4);
        CheckDlgButton(hwnd, 0x3f6, DAT_00529ddc);
        CheckDlgButton(hwnd, 0x3f7, DAT_00529e64);
        CheckDlgButton(hwnd, 0x3f8, DAT_00529dc8);
        ((Class_004df4e0*)this)->FUN_004df4e0();
        if (flag_20)
            ((Class_004df280*)this)->SetPerformanceWindowVisible(1);
        return 1;
    }

    case 0x312:
        if (wParam == 10) {
            ((Class_004df280*)this)->SetPerformanceWindowVisible(IsWindowVisible(hwnd) == 0);
        }
        return 0;

    }
    return 0;
}

Class_004df1e0* GetPerformanceWindow(void);

// The original calls this out of line from 0x4dfe80.
#pragma auto_inline(off)
// FUNCTION: 0x4dfd00
void ShowPerformanceStatus()
{
    ((Class_004df280*)GetPerformanceWindow())->SetPerformanceWindowVisible(1);
}
#pragma auto_inline(on)

extern const char* __cdecl FindCommandLineSwitch(const char*);

// FUNCTION: 0x4dfe80
void StartPerformanceStatus() {
    GetPerformanceWindow();
    const char* result = FindCommandLineSwitch("-performancestatus");
    if (result != 0) {
        ShowPerformanceStatus();
    }
}

// FUNCTION: 0x4dfea0
Class_004e0450 Class_004dfea0::FUN_004dfea0(Class_004e0450 _P)
{
    Node_004dfea0* _X;
    Node_004dfea0* _Y = (_P++)._Mynode();
    Node_004dfea0* _Z = _Y;
    std::_Lockit _Lk;
    if (_Left(_Y) == DAT_005292c4)
        _X = _Right(_Y);
    else if (_Right(_Y) == DAT_005292c4)
        _X = _Left(_Y);
    else
        _Y = _Min(_Right(_Y)), _X = _Right(_Y);
    if (_Y != _Z) {
        _Parent(_Left(_Z)) = _Y;
        _Left(_Y) = _Left(_Z);
        if (_Y == _Right(_Z))
            _Parent(_X) = _Y;
        else {
            _Parent(_X) = _Parent(_Y);
            _Left(_Parent(_Y)) = _X;
            _Right(_Y) = _Right(_Z);
            _Parent(_Right(_Z)) = _Y;
        }
        if (_Root() == _Z)
            _Root() = _Y;
        else if (_Left(_Parent(_Z)) == _Z)
            _Left(_Parent(_Z)) = _Y;
        else
            _Right(_Parent(_Z)) = _Y;
        _Parent(_Y) = _Parent(_Z);
        std::swap(_Color(_Y), _Color(_Z));
        _Y = _Z;
    } else {
        _Parent(_X) = _Parent(_Y);
        if (_Root() == _Z)
            _Root() = _X;
        else if (_Left(_Parent(_Z)) == _Z)
            _Left(_Parent(_Z)) = _X;
        else
            _Right(_Parent(_Z)) = _X;
        if (_Lmost() != _Z)
            ;
        else if (_Right(_Z) == DAT_005292c4)
            _Lmost() = _Parent(_Z);
        else
            _Lmost() = _Min(_X);
        if (_Rmost() != _Z)
            ;
        else if (_Left(_Z) == DAT_005292c4)
            _Rmost() = _Parent(_Z);
        else
            _Rmost() = _Max(_X);
    }
    if (_Color(_Y) == 1) {
        while (_X != _Root() && _Color(_X) == 1)
            if (_X == _Left(_Parent(_X))) {
                Node_004dfea0* _W = _Right(_Parent(_X));
                if (_Color(_W) == 0) {
                    _Color(_W) = 1;
                    _Color(_Parent(_X)) = 0;
                    _Lrotate(_Parent(_X));
                    _W = _Right(_Parent(_X));
                }
                if (_Color(_Left(_W)) == 1
                    && _Color(_Right(_W)) == 1) {
                    _Color(_W) = 0;
                    _X = _Parent(_X);
                } else {
                    if (_Color(_Right(_W)) == 1) {
                        _Color(_Left(_W)) = 1;
                        _Color(_W) = 0;
                        _Rrotate(_W);
                        _W = _Right(_Parent(_X));
                    }
                    _Color(_W) = _Color(_Parent(_X));
                    _Color(_Parent(_X)) = 1;
                    _Color(_Right(_W)) = 1;
                    _Lrotate(_Parent(_X));
                    break;
                }
            } else {
                Node_004dfea0* _W = _Left(_Parent(_X));
                if (_Color(_W) == 0) {
                    _Color(_W) = 1;
                    _Color(_Parent(_X)) = 0;
                    _Rrotate(_Parent(_X));
                    _W = _Left(_Parent(_X));
                }
                if (_Color(_Right(_W)) == 1
                    && _Color(_Left(_W)) == 1) {
                    _Color(_W) = 0;
                    _X = _Parent(_X);
                } else {
                    if (_Color(_Left(_W)) == 1) {
                        _Color(_Right(_W)) = 1;
                        _Color(_W) = 0;
                        _Lrotate(_W);
                        _W = _Left(_Parent(_X));
                    }
                    _Color(_W) = _Color(_Parent(_X));
                    _Color(_Parent(_X)) = 1;
                    _Color(_Left(_W)) = 1;
                    _Rrotate(_Parent(_X));
                    break;
                }
            }
        _Color(_X) = 1;
    }
    _Freenode(_Y);
    --_Size;
    return (_P);
}

// Shaped like std::_Tree<...>::_Erase(_Nodeptr) from MSVC 5's <xtree>
// (recursively frees a subtree) under a lock object; DAT_005292c4 is the
// tree's _Nil node. Nodes are returned to a free list (DAT_00529e58) linked
// through their first dword instead of being deleted.
struct Node_004e03f0 {
    Node_004e03f0* left;               // +0x0
    Node_004e03f0* parent;             // +0x4
    Node_004e03f0* right;              // +0x8
};

static inline void FreeNode(Node_004e03f0* p)
{
    if (p != 0) {
        p->left = (Node_004e03f0*)DAT_00529e58;
        DAT_00529e58 = p;
    }
}

// FUNCTION: 0x4e03f0
void Class_004e03f0::FUN_004e03f0(Node_004e03f0* x)
{
    std::_Lockit lock;
    for (Node_004e03f0* y = x; y != (Node_004e03f0*)DAT_005292c4; x = y) {
        FUN_004e03f0(y->right);
        y = y->left;
        FreeNode(x);
    }
}

// Shaped like std::_Tree<...>::iterator::_Inc() from MSVC 5's <xtree>: moves
// the iterator (a node pointer at +0) to the next node in order, under a lock.
// DAT_005292c4 is the tree's _Nil node; the inlined _Min (0x4e04e0) takes its
// own lock.
struct Node_004e0450 {
    Node_004e0450* left;               // +0x0
    Node_004e0450* parent;             // +0x4
    Node_004e0450* right;              // +0x8
};

static inline Node_004e0450* Min_004e0450(Node_004e0450* p)
{
    std::_Lockit lock;
    while (p->left != (Node_004e0450*)DAT_005292c4)
        p = p->left;
    return p;
}

// The original calls this out of line from 0x4df590.
#pragma auto_inline(off)
// FUNCTION: 0x4e0450
void Class_004e0450::FUN_004e0450()
{
    std::_Lockit lock;
    if (ptr->right != (Node_004e0450*)DAT_005292c4)
        ptr = Min_004e0450(ptr->right);
    else {
        Node_004e0450* p;
        while (ptr == (p = ptr->parent)->right)
            ptr = p;
        if (ptr->right != p)
            ptr = p;
    }
}
#pragma auto_inline(on)
