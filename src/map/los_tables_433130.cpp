// Decompiled by space-bunny-free. Names are provisional.
// The object at +0 is a std::vector<std::vector<Elem_00434360> > (the same
// class as 0x433380), where Elem_00434360 is a struct holding one
// std::vector<Elem_00434020>; the fill value is a std::vector<Elem_00434360>.

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
        // Declared only, never defined: keeps the calls out of line.
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

#include "../util/tdf.h"
#include "los_tables.h"

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);

// FUNCTION: 0x433130
void LosTables::LoadLosTables()
{
    TdfFile tdf;
    char path[256];
    BuildDataPath(path, "gamedata", "los", "TDF");
    if ((&tdf)->LoadFile(path) != 0) {
        if ((&tdf)->SelectRecord("TABLEINFO") != 0) {
            short numtables = (short)tdf.current->GetFieldInt("numtables", 0);
            {
                // The vector of tables held at +0, reached through the class's
                // member-free declaration.
                W2_00433130& tables = *(W2_00433130*)this;
                W1_00433130 temp;
                unsigned n = (unsigned)numtables;
                if (tables.size() < n)
                    tables.insert(tables.end(), n - tables.size(), temp);
                else if (n < tables.size())
                    tables.erase(tables.begin() + n, tables.end());
            }
            for (short i = 0; i < numtables; i++)
                LoadLosTable(&tdf, i);
        }
        (&tdf)->Unload();
    }
}
