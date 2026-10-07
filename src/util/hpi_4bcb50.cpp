// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, finished by deepseek-v4.1-flash. Names are provisional.
// Recursive directory walk: opens path with the search 0x4bc4b0 and, for every
// entry whose name is neither "." nor "..", either descends into the
// subdirectory or, when the name matches the wildcard pattern, appends
// "path\\name" to the vector passed in. The search handle is closed at the end
// (the inlined body of 0x4bc8d0).
#include <io.h>
#include <stdio.h>
#include <string.h>

// The search 0x4bc4b0 allocates.
#pragma pack(push, 1)
struct FindFiles {
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
    void ReleaseRef();
};

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ReleaseRef(); }
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

int __stdcall HAPI_FindFirst(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall HAPI_FindNext(FindFiles* f, struct _finddata_t* fd);
int __stdcall MatchWildcard(const char* str, const char* pat);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FindFilesRecursive(char* path, const char* pat, Class_004be6c0* tree, int state, int recursive);


static inline int Next(int h, struct _finddata_t* fd) { int r = HAPI_FindNext((FindFiles*)h, fd); return r; }
// FUNCTION: 0x4bcb50
void __stdcall FindFilesRecursive(char* path, const char* pat, Class_004be6c0* tree, int state, int recursive)
{
    char buf[0x100];
    struct _finddata_t fd;

    sprintf(buf, "%s\\*", path);
    int h = HAPI_FindFirst(buf, &fd, state, recursive);
    if (h != -1) {
        // Never read: the extra live value demotes the handle from ebx to ebp.
        int r;
        do {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0) {
                if ((fd.attrib & 0x10) != 0) {
                    sprintf(buf, "%s\\%s", path, fd.name);
                    FindFilesRecursive(buf, pat, tree, ((FindFiles*)h)->state, 0);
                } else if (MatchWildcard(fd.name, pat)) {
                    sprintf(buf, "%s\\%s", path, fd.name);
                    Class_004c91b0 key(buf);
                    tree->insert(tree->_Last, 1, key);
                }
            }
        } while ((r = HAPI_FindNext((FindFiles*)h, &fd)) != -1);
        // A second variable, cast from h: the -1 test is dropped if written on h.
        FindFiles* f = (FindFiles*)h;
        if (f != (FindFiles*)-1 && f != 0) {
            if (f->state < 0)
                _findclose(f->handle);
            FUN_004d85a0(f);
        }
    }
}
