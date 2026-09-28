// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Walks the static {name, value1, value2} table passed in (the caller at
// 0x4195a7 pushes 0x501d38) and, for every record, inserts it into the global
// vector DAT_0051fc99 (initialiser 0x4b75a0, atexit destructor 0x4b75d0,
// clear 0x4b7ad0), keeping it sorted case-insensitively by name: a record
// whose name matches case-sensitively updates that element, anything else is
// inserted at its lower-bound position. The same body for a single record is
// the standalone function 0x4b7620, which /Ob2 inlined here.
//
// PARTIAL (51.3% with a static declaration, 43.2% as written): the loop bound
// `last` and the search key `key` have their ebx/ebp roles swapped versus the
// original (`last` should be ebx and `key` ebp, with the record pointer `rec`
// spilled to its parameter slot), and the original materialises the second
// comparison as `xor ecx,ecx; test eax,eax; sete cl; neg cl; sbb ecx,ecx;
// inc ecx`, while this source folds it. Declaration order, const, loop shape
// (for/while, local pointer), the record-as-array vs pointer, the Elem temp
// constructor, and re-deriving the body as an inlined FUN_004b7620 all failed
// to change the register choice.
//
// The vector is declared extern on purpose: the original's symbol is the
// file-local DAT_0051fc99$S4554 and a `static` declaration here compiles to
// _DAT_0051fc99$S4579, which data/symbols.csv cannot match. An extern
// declaration resolves to the placeholder name DAT_0051fc99.
#include <string.h>
#include <vector>

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

// Release of the reference-counted string handle (0x4c9390).
class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

// Copy constructor of the handle (0x4c91a0).
class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

// Constructor of the handle from a C string (0x4c91b0).
class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { FUN_004c9390(); }
};

struct Elem_004b75d0 {
    Class_004c91a0 name;               // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    Elem_004b75d0(const Class_004c91a0& n) : name(n), value1(0), value2(0) {}
    ~Elem_004b75d0() { name.FUN_004c9390(); }
};

extern std::vector<Elem_004b75d0> DAT_0051fc99;

struct Rec_004b7760 {
    char* name;                        // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8
};

// FUNCTION: 0x4b7760
void __stdcall FUN_004b7760(Rec_004b7760* rec)
{
    for (; rec->name; rec++) {
        int v1 = rec->value1;
        int v2 = rec->value2;
        Class_004c91b0 name(rec->name);
        char* key = name.data;
        Elem_004b75d0* first = DAT_0051fc99.begin();
        Elem_004b75d0* last = DAT_0051fc99.end();
        while (first != last) {
            Elem_004b75d0* mid = first + (last - first) / 2;
            bool less = _strcmpi(mid->name.data, key) < 0;
            if (less)
                first = mid + 1;
            else
                last = mid;
        }
        int eq = strcmp(first->name.data, key) == 0;
        if (first == DAT_0051fc99.end() || eq == 0)
            first = DAT_0051fc99.insert(first, Elem_004b75d0(name));
        first->value1 = v1;
        first->value2 = v2;
    }
}
