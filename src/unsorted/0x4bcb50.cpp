// Decompiled by space-bunny-free. Names are provisional.
// Recursive directory walk: opens path with the search 0x4bc4b0 and, for every
// entry whose name is neither "." nor "..", either descends into the
// subdirectory (the sub-search is not itself recursive, because this function
// recurses) or, when the name matches the wildcard pattern, appends
// "path\\name" to the vector passed in. The search handle is closed at the end.
// The frame is exactly a path buffer, a _finddata_t and the string object, so
// the two sprintfs have to share one buffer.
//
// Still differs, 90.2 percent: the path parameter and the search handle get
// ebx and ebp the other way round. The original loads the path into ebx in the
// prologue and keeps the handle in ebp; this file loads the path into ebp and
// keeps the handle in ebx. Twelve instructions differ, all of them the same
// two registers, and the byte count is already identical (442 = 442), so the
// frame, every esp displacement, both sprintfs, the strcmp expansions, the
// recursion and the `cmp reg, eax` pair in the tail all match.
//
// Two source details are load-bearing and worth keeping. The two tests in the
// tail only survive if they name a second variable: written on the loop's own
// handle, MSVC knows from the `h != -1` above that the `-1` test is redundant
// and drops it. And the loop handle stays live across both sprintfs, so a
// `Find*` local in the loop and an `int` in the tail are not enough: the tail's
// variable has to be the pointer cast of the int (the other way round also
// gives both tests).
//
// Tried with no effect on the register choice: the handle as int, unsigned,
// long or pointer; the path as const or non-const, and as a local copy; the
// last parameter as char; declaring the handle before or between the buffer
// and the finddata; a hand-written finddata instead of io.h's; a name pointer
// local for fd.name; an inline helper for the construct/insert/destroy; the
// tail inside or outside the `h != -1` block; nested ifs instead of `&&`, the
// two strcmp operands swapped, `if/else` instead of `else if`, a continue
// guard, a `for(;;)` with a break, and an early return. Renaming the variables
// does not move it either.
#include <io.h>
#include <stdio.h>
#include <string.h>

// The search 0x4bc4b0 allocates.
#pragma pack(push, 1)
struct Find_004bcb50 {
    char dir[0x100];         // +0x000
    char pattern[0x100];     // +0x100
    int state;               // +0x200
    char recursive;          // +0x204
    long handle;             // +0x205
    int index;               // +0x209
};
#pragma pack(pop)

class Class_004c9390 {
public:
    char* data;
    void FUN_004c9390();
};

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { FUN_004c9390(); }
};

class Class_004be6c0 {
public:
    char unknown_0[4];
    int* _First;
    int* _Last;
    int* _End;
    void FUN_004be6c0(int* pos, int n, const Class_004c91a0* value);
};

int __stdcall FUN_004bc4b0(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall FUN_004bc640(Find_004bcb50* f, struct _finddata_t* fd);
int __stdcall FUN_004bc370(const char* str, const char* pat);
void FUN_004d85a0(void* p);
void __stdcall FUN_004bcb50(char* path, const char* pat, Class_004be6c0* tree, int state, int recursive);

// FUNCTION: 0x4bcb50
void __stdcall FUN_004bcb50(char* path, const char* pat, Class_004be6c0* tree, int state, int recursive)
{
    char buf[0x100];
    struct _finddata_t fd;

    sprintf(buf, "%s\\*", path);
    int h = FUN_004bc4b0(buf, &fd, state, recursive);
    if (h != -1) {
        do {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0) {
                if ((fd.attrib & 0x10) != 0) {
                    sprintf(buf, "%s\\%s", path, fd.name);
                    FUN_004bcb50(buf, pat, tree, ((Find_004bcb50*)h)->state, 0);
                } else if (FUN_004bc370(fd.name, pat)) {
                    sprintf(buf, "%s\\%s", path, fd.name);
                    Class_004c91b0 key(buf);
                    tree->FUN_004be6c0(tree->_Last, 1, &key);
                }
            }
        } while (FUN_004bc640((Find_004bcb50*)h, &fd) != -1);
        Find_004bcb50* f = (Find_004bcb50*)h;
        if (f != (Find_004bcb50*)-1 && f != 0) {
            if (f->state < 0)
                _findclose(f->handle);
            FUN_004d85a0(f);
        }
    }
}
