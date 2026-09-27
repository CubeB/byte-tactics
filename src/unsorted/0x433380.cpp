// Decompiled by space-bunny-free. Names are provisional.
// Best attempt, 42% of the bytes match (not MATCH).
//
// The function: for table number `table` (0-based) it builds the name
// "TABLE%d" (table + 1), resets the TDF reader, looks the name up, and if it
// is there resizes tables[table] to numlines * 4 lines, then calls
// FUN_004336f0 on each of the four quarter blocks (line i, numlines + i,
// 2 * numlines + i, 3 * numlines + i) with the block number as the third
// argument.
//
// The type is three levels of std::vector: `tables` is a
// vector<vector<vector<Elem_00434020> > >. tables[table] is resized (its size,
// insert and erase are the out-of-line 0x433b00, 0x433db0, 0x434020 of a
// vector<vector<Elem_00434020> >), the fill value is a default-constructed
// std::vector<Elem_00434020> (constructor 0x433a10, destructor 0x433a30) and
// the four calls go to the extra method of the inner vector (0x4336f0). The
// hand-rolled std::vector interface above, with size, insert, erase, the
// constructor and the destructor only declared, is what makes the compiler
// emit those same out-of-line calls, as in the sibling 0x433130.cpp.
//
// What still differs (the remaining 58%):
//   * register choice in the prologue. The original keeps `this` in esi and
//     the table parameter as a 16-bit copy in di (`mov di, word ptr [esp+54]`,
//     `mov esi, ecx`, then `movsx eax, di` at each use). This version
//     sign-extends the parameter into esi up front and keeps `this` in edi,
//     and every later difference follows from that.
//   * the frame is 0x20 bytes here against 0x3c in the original, whose
//     locals are 2n, -n and n spilled in the loop plus the 16-byte fill
//     value and the 16-byte name buffer. The original reloads numlines from
//     the dead `file` argument slot, which frees ebp for the loop's
//     induction variable 2n + i; here numlines stays in a register, so the
//     compiler strength-reduces the loop indices instead.
#include <stdio.h>

// A hand-rolled std::vector interface, as in 0x433130.cpp: size, insert and
// erase are declared only, so the compiler emits the same out-of-line calls
// the original does (0x433b00, 0x433db0, 0x434020) instead of inlining them,
// while begin(), end() and operator[] stay inline.
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

        vector(const A& al = A());
        iterator begin() { return _First; }
        iterator end() { return _Last; }
        T& operator[](size_type i) { return _First[i]; }
        size_type size() const;
        void insert(iterator pos, size_type n, const T& x);
        void erase(iterator first, iterator last);
        ~vector();
    };
}

struct Elem_00434020 {
    int value;                         // +0x0
};

class Class_004c3410;
class Class_004c46c0;

// The 16-byte line: a std::vector<Elem_00434020> whose method the original
// calls on every entry (0x4336f0). It is used through a view, so that the
// vector<Elem_00434020> instantiations keep the names data/symbols.csv
// records for 0x433a10, 0x433a30, 0x433b00 and 0x434020.
typedef std::vector<Elem_00434020> Line_00433380;

class LineView_00433380 : public Line_00433380 {
public:
    void FUN_004336f0(Class_004c3410* file, short line, int col);
};

typedef std::vector<Line_00433380> Group_00433380;
typedef std::vector<Group_00433380> Table_00433380;

class Class_004c3410 {
public:
    void* root;                        // +0x0
    Class_004c46c0* current;           // +0x4

    int FUN_004c3410(char* name);
};

class Class_004c3e10 {
public:
    void* root;                        // +0x0
    void* current;                     // +0x4

    void FUN_004c3e10();
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_00433380 {
public:
    Table_00433380 tables;             // +0x0

    void FUN_00433380(Class_004c3410* file, short table);
};

// FUNCTION: 0x433380
void Class_00433380::FUN_00433380(Class_004c3410* file, short table)
{
    char name[16];
    sprintf(name, "TABLE%d", table + 1);
    ((Class_004c3e10*)file)->FUN_004c3e10();
    if (file->FUN_004c3410(name) == 0)
        return;
    short numlines = (short)file->current->FUN_004c46c0("numlines", 0);
    Group_00433380& group = tables[table];
    {
        Line_00433380 blank;
        short n = numlines * 4;
        if (group.size() < n)
            group.insert(group.end(), n - group.size(), blank);
        else if (n < group.size())
            group.erase(group.begin() + n, group.end());
    }
    for (short i = 0; i < numlines; i++) {
        short k = (short)(numlines * 2 + i);
        ((LineView_00433380&)group[i]).FUN_004336f0(file, i, 0);
        ((LineView_00433380&)group[(short)(k - numlines)]).FUN_004336f0(file, i, 1);
        ((LineView_00433380&)group[k]).FUN_004336f0(file, i, 2);
        ((LineView_00433380&)group[(short)(k + numlines)]).FUN_004336f0(file, i, 3);
    }
}
