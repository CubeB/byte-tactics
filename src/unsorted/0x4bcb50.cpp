// Decompiled by space-bunny-free, finished by muse-spark-1.3-free. Names are provisional.
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
// The most useful lead left, from a matched neighbour. 0x4bca30 is the same
// walk without the recursion and it MATCHES, and it hands its callee-saved
// registers the other way round: the handle is in edi, the tree in ebp and the
// int parameter in ebx, while here the handle takes the first register it can
// get. The original of 0x4bcb50 wants the same order as 0x4bca30 (path ebx,
// handle ebp, tree edi, the string constant esi), so whatever moves the handle
// behind the parameters in 0x4bca30 is what this function needs. Probing that
// file: dropping one of the tree's three references into the loop condition
// moves the tree from ebp to edi and the handle from edi up to ebp, so the
// priority there does move with the number of references, and swapping two
// parameters of that function swaps their registers while leaving the handle in
// edi. Neither the reference count nor the parameter position reproduces this
// function's order, so the ranking is not a plain sort of either.
//
// Tried with no effect on the register choice: the handle as int, unsigned,
// long or pointer; the path as const or non-const, and as a local copy; the
// last parameter as char; declaring the handle before or between the buffer
// and the finddata; a hand-written finddata instead of io.h's; a name pointer
// local for fd.name; an inline helper for the construct/insert/destroy; the
// tail inside or outside the `h != -1` block; nested ifs instead of `&&`, the
// two strcmp operands swapped, `if/else` instead of `else if`, a continue
// guard, a `for(;;)` with a break, and an early return. Renaming the variables
// does not move it either. (The second pass below adds about forty more.)
//
// The header lever is dead here too, so nobody sweeps it again. tools/headers.py
// 0x4bcb50 --cpp was run over all 768 sets (every combination of windows.h,
// stdio.h, stdlib.h, string.h, math.h, memory.h and ddraw.h, each also with
// one of <string>, <vector>, <map>, <list>, <iostream> in front): the best is
// 90.2 percent, that is, every single set produces this same output with the
// path in ebp. On the free harness the same negative for hand-picked sets: the
// three sets 0x4399f0 records (stdlib+math+memory, windows+memory, windows),
// each on top of io.h, plus io.h+stdio+string, plus each of stdlib, memory,
// math, windows, direct, fcntl, time, limits, setjmp, string, stdio, process,
// errno, assert and sys/types alone on top of io.h. The unused-declaration
// calibration from that file is flat as well: with a header-free prelude
// (hand-declared sprintf, strcmp, _findclose and the io.h-shaped finddata,
// which reproduces this output exactly) and N unused `extern int` declarations
// in front of the body, every N from 0 to 316 in steps of 4, and every N from
// 0 to 63 in steps of 1, is identical. There is no window, so unlike 0x4399f0
// no header set can be chosen to land in one. This is the same clean negative
// as 0x438ea0: the register allocator here does not read the symbol table size
// at all. Marking this one as one of the "callee-saved register rotation wall"
// functions in docs/field-notes.md looks right: the allocation below is
// completely insensitive to the source.
//
// A second session (this one) spent about forty more shapes on the allocation
// and every one of them gives the same 90.2 percent with the handle in ebx and
// the path in ebp, so all of these are ruled out, not merely untried:
//   - the parameter types: pat, tree, path and all three as int, each with the
//     casts the other two uses need (no change in the bytes at all);
//   - the handle as long, unsigned, void*, as a pointer for the whole body, as
//     an int with the tail's tests on h (that one drops the cmp reg,eax pair
//     and lands on 88.0), as a pointer copy made before the loop, with a fresh
//     int or pointer copy in the tail, and with the tail's field reads casting
//     h instead of using f;
//   - the declarations of the callees: FUN_004bc640 taking an int handle and
//     being passed h, FUN_004d85a0 taking int* as 0x4bca30's file has it, and
//     both at once;
//   - throwaway extra references, the trick 0x424890 and 0x4ae410 use to test a
//     priority tie: `path = path`, `h = h`, and `path ? path : path` inside all
//     three sprintfs, which adds three more references to path and still leaves
//     the handle in ebx. So the priority here does not even read the number of
//     references of the path;
//   - extra loop levels and goto loops (0x40e160's lever): the do/while as a
//     label plus goto, the whole body inside a for(;;) with a break, inside a
//     while(again) with a flag, and the latch as an if with a break;
//   - the path as a local copy taken as the very first statement of the
//     function, before the first sprintf, with and without the parameter still
//     used there, and as a const char* copy;
//   - the handle declared before the buffer and the finddata (int h; then the
//     two big locals; then the assignment), and both it and a path copy
//     declared at the top;
//   - the key object in its own nested block, the attrib test as `fd.attrib &
//     0x10`, `(fd.attrib & 0x10) == 0x10`, the two strcmp operands swapped, the
//     tail's two tests swapped and nested, and the two tail variable spellings
//     (pointer copy of the int, or the int named twice);
//   - the order of the four class declarations in the file and extern on each
//     forward declaration.
// Two shapes that stay at 90.2 percent and are worth keeping as the closest
// alternatives: the single pointer variable (loop, tail and state all through
// `Find_004bcb50* f`, 88.0 because one instruction goes) and the pointer copy
// taken before the loop (90.2, identical bytes).
//
// A third session confirmed the wall from the other side and ruled out the
// remaining levers, every one scoring the identical 90.2 percent (or 88.0
// where noted) on the free scratch harness with the same ebx/ebp-only diff:
// an inlined strcmp helper for the dot checks, hoisting tree->_Last, the
// child state, fd.attrib or fd.name into locals, the loop as while(1) with a
// break, nested tail ifs, copying all recursion arguments into locals,
// `register` on the path, pat as non-const char*, compiling the real adjacent
// function 0x4bca30 first in the same file, and both tail tests on the int
// handle with one cast inside (88.0, 438 bytes, the cmp pair folds). The
// register choice is insensitive to the source, so this file keeps the 90.2
// version above unchanged.
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
