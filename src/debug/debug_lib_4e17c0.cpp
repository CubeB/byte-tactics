// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Constructor of the global name-table singleton (allocated by GetNameTable).
// It is an MSVC 5 std::map whose value is a 500-byte buffer, so the tree node
// is 0x208 bytes with the colour at +0x204 (see 0x4df380.cpp, 0x4e1990.cpp).
// The tree's static _Nil / _Nilrefs are DAT_005292c4 / DAT_00529500 and the
// pooled node free list is DAT_00529e58, exactly as in <xtree>'s _Init with a
// pooled allocator. The map member sits at +0, the "changed" flag at +0x10.
//
// The map's allocator pools 0x208-byte nodes. Its _Charalloc is the routine
// matched as 0x4e2b60; /Ob2 inlines it into the second _Buynode call and
// leaves the first as an out-of-line call, which is what the original does.
// _Charalloc is renamed to FUN_004e2b60 so the call uses the same name as the
// already-matched 0x4e2b60 file (the real symbol is the STL allocator's
// _Charalloc, which no natural instantiation can spell that way).
#include <windows.h>
#include <string.h>

#define _Charalloc FUN_004e2b60
#include <map>
#undef _Charalloc

extern void* DAT_00529e58;             // node free list
extern void* DAT_005292c4;             // tree _Nil
extern unsigned int DAT_00529500;      // tree _Nilrefs
extern void (*DAT_005289bc)();         // out-of-memory handler

struct Less_004e17c0 {
    bool operator()(const char* a, const char* b) const
    {
        return a != b && strcmp(a, b) < 0;
    }
};

struct Value_004e17c0 {
    char text[500];
};

typedef std::pair<const char*, Value_004e17c0> ValueType_004e17c0;

// The pooled allocator; its _Charalloc is the out-of-line 0x4e2b60.
class Class_004e2b60 {
public:
    typedef ValueType_004e17c0 value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type n, const void* = 0)
    {
        return (pointer)FUN_004e2b60(n * sizeof(value_type));
    }
    void deallocate(void* p, size_type)
    {
        if (p != 0) {
            *(void**)p = DAT_00529e58;
            DAT_00529e58 = p;
        }
    }
    size_type max_size() const { return (size_type)(-1) / sizeof(value_type); }

    char* FUN_004e2b60(size_type n)
    {
        if (DAT_00529e58 == 0) {
            char* block;
            do {
                block = (char*)GlobalAlloc(0, 0x2000);
                if (block == 0 && DAT_005289bc != 0)
                    DAT_005289bc();
            } while (block == 0 && DAT_005289bc != 0);
            if (block == 0)
                return 0;
            for (unsigned int rem = 0x2000; rem >= n; rem -= n) {
                *(void**)block = DAT_00529e58;
                DAT_00529e58 = block;
                block += n;
            }
        }
        void* p = DAT_00529e58;
        DAT_00529e58 = *(void**)p;
        return (char*)p;
    }
};

typedef std::map<const char*, Value_004e17c0, Less_004e17c0, Class_004e2b60>
    Map_004e17c0;

class NameTable {
public:
    Map_004e17c0 names;                // +0x0
    bool changed;                      // +0x10

    NameTable();
};

// FUNCTION: 0x4e17c0
NameTable::NameTable() : changed(0)
{
}
