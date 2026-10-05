// Decompiled by space-bunny-free. Names are provisional.
// The object at +0 is a std::vector<std::vector<Elem_00434360> > (the same
// class as 0x433380), where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>; the fill value is a std::vector<Elem_00434360>.
// Its out-of-line members are declared only, so the compiler emits the same
// calls the original does instead of inlining them.

namespace std {
    template<class T> class allocator {
    public:
        typedef unsigned int size_type;
        typedef T* pointer;
        typedef T& reference;
        typedef T value_type;
    };

    template<class T, class A = allocator<T> > class vector {
    public:
        typedef A::size_type size_type;
        typedef T* iterator;
        A alloc;
        iterator _First;
        iterator _Last;
        iterator _End;

        explicit vector(const A& al = A())
            : alloc(al), _First(0), _Last(0), _End(0) {}
        iterator begin() { return _First; }
        iterator end() { return _Last; }
        size_type size() const { return _First == 0 ? 0 : (_Last - _First); }
        iterator erase(iterator first, iterator last);
        void _Destroy(iterator first, iterator last);
        void insert(iterator pos, size_type n, const T& x);
        ~vector()
        {
            _Destroy(_First, _Last);
            operator delete(_First);
        }
    };
}

void __cdecl operator delete(void* p);

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> W1_00433130;
typedef std::vector<W1_00433130> W2_00433130;

class Class_004c2ea0 {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* path);
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

class Class_00433380 {
public:
    void FUN_00433380(Class_004c2ea0* tdf, short index);
};

class Class_00433130 {
public:
    W2_00433130 tables;                // +0x0

    void FUN_00433130();
};

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);

// FUNCTION: 0x433130
void Class_00433130::FUN_00433130()
{
    Class_004c2ea0 tdf;
    char path[256];
    BuildDataPath(path, "gamedata", "los", "TDF");
    if (((Class_004c2f60*)&tdf)->FUN_004c2f60(path) != 0) {
        if (((Class_004c3410*)&tdf)->FUN_004c3410("TABLEINFO") != 0) {
            short numtables = (short)((Class_004c46c0*)tdf.field_4)->FUN_004c46c0("numtables", 0);
            {
                W1_00433130 temp;
                unsigned n = (unsigned)numtables;
                if (tables.size() < n)
                    tables.insert(tables.end(), n - tables.size(), temp);
                else if (n < tables.size())
                    tables.erase(tables.begin() + n, tables.end());
            }
            for (short i = 0; i < numtables; i++)
                ((Class_00433380*)this)->FUN_00433380(&tdf, i);
        }
        ((Class_004c3240*)&tdf)->FUN_004c3240();
    }
}
