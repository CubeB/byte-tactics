// Decompiled by space-bunny-free. Names are provisional.
// Looks a name up in the file-local global vector of 0x4889d0.cpp (its
// compiler-generated initialiser) and 0x488a00.cpp (the atexit destructor it
// registers), and, if the name is not there, allocates a zeroed 0x40-byte
// object and inserts a (name, object) pair at the position the sorted search
// ended on. The search loop is the same shape as 0x438760.cpp.
#include <string.h>

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

// Copy constructor of the reference-counted string handle (0x4c91a0).
class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

// Constructor of the same handle from a C string (0x4c91b0).
class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { FUN_004c9390(); }
};

struct Elem_00488a00 {
    Class_004c91a0 name;               // +0x0
    void* value;                       // +0x4

    Elem_00488a00(const Class_004c91a0& n, void* v) : name(n) { value = v; }
    ~Elem_00488a00() { name.FUN_004c9390(); }
};

class Class_00488c50 {
public:
    int unknown_0[0x10];               // the object is 0x40 bytes

    Class_00488c50() { memset(this, 0, sizeof(Class_00488c50)); }
};

// The global vector; its out-of-line insert is 0x488fb0.
class Class_00488fb0 {
public:
    char allocator;                    // +0x0
    Elem_00488a00* _First;             // +0x4
    Elem_00488a00* _Last;              // +0x8
    Elem_00488a00* _End;               // +0xc

    Elem_00488a00* begin() { return _First; }
    Elem_00488a00* end() { return _Last; }
    int size() const { return _Last - _First; }
    void FUN_00488fb0(Elem_00488a00* pos, int n, const Elem_00488a00& x);
};

extern Class_00488fb0 DAT_0051e6b0;

// FUNCTION: 0x488c50
void* __stdcall FUN_00488c50(char* name)
{
    Elem_00488a00* first = DAT_0051e6b0.begin();
    int n = DAT_0051e6b0.size();
    for (; 0 < n; ) {
        int n2 = n / 2;
        Elem_00488a00* m = first + n2;
        if (_strcmpi(m->name.data, name) < 0)
            first = ++m, n -= n2 + 1;
        else
            n = n2;
    }
    if (first != DAT_0051e6b0.end() && _strcmpi(first->name.data, name) == 0)
        return first->value;

    Class_00488c50* p = new Class_00488c50;
    DAT_0051e6b0.FUN_00488fb0(first, 1, Elem_00488a00(Class_004c91b0(name), p));
    return p;
}
