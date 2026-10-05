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

class UnitCategory {
public:
    Class_004c91a0 name;               // +0x0
    void* value;                       // +0x4

    UnitCategory(const Class_004c91a0& n, void* v) : name(n) { value = v; }
    ~UnitCategory() { name.FUN_004c9390(); }
};

class UnitTypeSet {
public:
    int unknown_0[0x10];               // the object is 0x40 bytes

    UnitTypeSet() { memset(this, 0, sizeof(UnitTypeSet)); }
};

// The global std::vector<UnitCategory>, written by hand so that insert
// (0x488fb0, which has its own file) stays an out-of-line call under its real
// name, as the original has it.
namespace std {
template<class T> class allocator;
template<class T, class A = allocator<T> > class vector {
public:
    char allocator_;                   // +0x0
    T* _First;                         // +0x4
    T* _Last;                          // +0x8
    T* _End;                           // +0xc

    T* begin() { return _First; }
    T* end() { return _Last; }
    int size() const { return _Last - _First; }
    void insert(T* pos, unsigned int n, const T& x);
};
}

extern std::vector<UnitCategory> DAT_0051e6b0;

// FUNCTION: 0x488c50
void* __stdcall GetCategoryMask(char* name)
{
    UnitCategory* first = DAT_0051e6b0.begin();
    int n = DAT_0051e6b0.size();
    for (; 0 < n; ) {
        int n2 = n / 2;
        UnitCategory* m = first + n2;
        if (_strcmpi(m->name.data, name) < 0)
            first = ++m, n -= n2 + 1;
        else
            n = n2;
    }
    if (first != DAT_0051e6b0.end() && _strcmpi(first->name.data, name) == 0)
        return first->value;

    UnitTypeSet* p = new UnitTypeSet;
    DAT_0051e6b0.insert(first, 1, UnitCategory(Class_004c91b0(name), p));
    return p;
}
