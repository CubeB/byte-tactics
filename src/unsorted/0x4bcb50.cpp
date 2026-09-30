// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash. Names are provisional.
// Recursive directory walk: opens path with the search 0x4bc4b0 and, for every
// entry whose name is neither "." nor "..", either descends into the
// subdirectory or, when the name matches the wildcard pattern, appends
// "path\\name" to the vector passed in. The search handle is closed at the end
// (the inlined body of 0x4bc8d0).
//
// SOLVED: the register rotation. Four earlier sessions left this at 90.2
// percent with the path parameter in ebp and the search handle in ebx, the
// reverse of the original (path ebx, handle ebp, tree edi, the string constant
// esi), and concluded the allocation was insensitive to the source after
// trying about forty shapes. It is not. The lever is a SINGLE dead-looking
// local assigned in the loop condition:
//
//     int r;
//     do { ... } while ((r = FUN_004bc640((Find_004bcb50*)h, &fd)) != -1);
//
// `r` is never read (the classic pointer tail below tests the separate `f`),
// but the assignment still reaches the allocator as a live graph node, and one
// extra live node demotes the handle from ebx to ebp, which in turn frees ebx
// for the path parameter. That is confirmed technique 2 in the brief, and it
// is worth 9.8 points: 90.2 to 100. Four spellings of the same statement all
// give an exact match (int r, long r, and a Find_004bcb50* r), so the fix is
// the extra node, not its type. Variants that only add a use of path
// (`path = path`, `path ? path : path`) do nothing, so the demotion is of the
// handle, not a promotion of the path.
//
// Two source details in the tail are load-bearing and are kept. The
// `f != (Find_004bcb50*)-1` test only survives if it names a second variable:
// written on the loop's own handle, MSVC knows from the `h != -1` above that
// the -1 test is redundant and drops it (438 bytes, 96.5 percent). And the
// tail's variable has to be the pointer cast of the int, because MSVC folds
// the comparison against the -1 the loop already left in eax, giving the
// original's `cmp ebp, eax`. Skipping `r` entirely and writing only the tail
// gives 90.2 with the wrong rotation.
//
// The insert call is reachable only as the protected std::vector member, so
// the container is hand-rolled and `tree` is a std::vector<Class_004c91a0>
// (same shape as the matching sibling 0x4bca30), which keeps 0x4be6c0 an
// out-of-line call under the name data/symbols.csv gives it.
//
// Ruled out across the earlier sessions, so nobody repeats them: the header
// set (tools/headers.py --cpp over all 768 sets, plus the N-unused-declarations
// calibration, is flat at 90.2); the handle and path types (int, long,
// unsigned, void*, Find_004bcb50*); the parameter positions; a local copy of
// the path, name or buffer; nested ifs, for/while/goto loop shapes; swapping
// the loop branches or the strcmp operands; hoisting tree->_Last, fd.attrib or
// the state; and `register`/casts. All of those stay at 90.2 (or 88.0 for the
// pure pointer model) except the header-negative cases noted above.
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

// The caller's std::vector<Class_004c91a0>, written by hand so that insert
// (0x4be6c0, which has its own file) stays an out-of-line call under its real
// name.
namespace std {
template<class T> class allocator;
template<class T, class A = allocator<T> > class vector {
public:
    char allocator_;                   // +0x0
    T* _First;                         // +0x4
    T* _Last;                          // +0x8
    T* _End;                           // +0xc
    void insert(T* pos, unsigned int n, const T& x);
};
}
typedef std::vector<Class_004c91a0> Class_004be6c0;

int __stdcall FUN_004bc4b0(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall FUN_004bc640(Find_004bcb50* f, struct _finddata_t* fd);
int __stdcall FUN_004bc370(const char* str, const char* pat);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FUN_004bcb50(char* path, const char* pat, Class_004be6c0* tree, int state, int recursive);


static inline int Next(int h, struct _finddata_t* fd) { int r = FUN_004bc640((Find_004bcb50*)h, fd); return r; }
// FUNCTION: 0x4bcb50
void __stdcall FUN_004bcb50(char* path, const char* pat, Class_004be6c0* tree, int state, int recursive)
{
    char buf[0x100];
    struct _finddata_t fd;

    sprintf(buf, "%s\\*", path);
    int h = FUN_004bc4b0(buf, &fd, state, recursive);
    if (h != -1) {
        int r;
        do {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0) {
                if ((fd.attrib & 0x10) != 0) {
                    sprintf(buf, "%s\\%s", path, fd.name);
                    FUN_004bcb50(buf, pat, tree, ((Find_004bcb50*)h)->state, 0);
                } else if (FUN_004bc370(fd.name, pat)) {
                    sprintf(buf, "%s\\%s", path, fd.name);
                    Class_004c91b0 key(buf);
                    tree->insert(tree->_Last, 1, key);
                }
            }
        } while ((r = FUN_004bc640((Find_004bcb50*)h, &fd)) != -1);
        Find_004bcb50* f = (Find_004bcb50*)h;
        if (f != (Find_004bcb50*)-1 && f != 0) {
            if (f->state < 0)
                _findclose(f->handle);
            FUN_004d85a0(f);
        }
    }
}
