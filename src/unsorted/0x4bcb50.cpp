// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by Sonnet 5.5. Names are provisional.
// Recursive directory walk: opens path with the search 0x4bc4b0 and, for every
// entry whose name is neither "." nor "..", either descends into the
// subdirectory or, when the name matches the wildcard pattern, appends
// "path\\name" to the vector passed in. The search handle is closed at the end
// (the inlined body of 0x4bc8d0).
//
// What fixed the last 9.8 percent (path and handle in ebx and ebp swapped):
// the advance call `FUN_004bc640(h, &fd)` goes through a tiny inline wrapper,
// `static inline int Next(int h, fd) { int r = FUN_004bc640(h, fd); return r; }`.
// The wrapper leaves the bytes alone but gives the handle one more use in the
// loop, which is what tipped the register priority (the original evidently had
// such an inlined helper there). A wrapper that returns the call directly does
// not help; one with an `h == -1` early return also works. Every other lever
// (types, loop shapes, tail shapes, headers, dummy uses) was flat.
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

// vector<Class_004c91a0> written by hand so insert (0x4be6c0) keeps its real name.
namespace std {
template<class T> class allocator;
template<class T, class A = allocator<T> > class vector {
public:
    char allocator_;
    T* _First;
    T* _Last;
    T* _End;
    void insert(T* pos, unsigned int n, const T& x);
};
}
typedef std::vector<Class_004c91a0> Class_004be6c0;

int __stdcall FUN_004bc4b0(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall FUN_004bc640(Find_004bcb50* f, struct _finddata_t* fd);
int __stdcall FUN_004bc370(const char* str, const char* pat);
void FUN_004d85a0(void* p);
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
        } while (Next(h, &fd) != -1);
        Find_004bcb50* f = (Find_004bcb50*)h;
        if (f != (Find_004bcb50*)-1 && f != 0) {
            if (f->state < 0)
                _findclose(f->handle);
            FUN_004d85a0(f);
        }
    }
}
