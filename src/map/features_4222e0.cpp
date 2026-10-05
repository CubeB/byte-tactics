// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Loads every features/*.tdf through FUN_004bcb50 into a local
// std::vector<Elem_004222e0>, then parses each file name into a new
// Class_004c2ea0 and appends it to the global vector DAT_00511fb4.
//
// The function must be __stdcall (or __fastcall): with the default __cdecl
// convention MSVC 5 schedules the loop entry as `cmp esi,eax; mov edi,esi`,
// while the original emits `mov edi,esi; cmp esi,eax`. A non-cdecl convention
// on this no-argument function changes only the instruction order.
// push_back inlines to a call of the out-of-line
// std::vector<Class_004c2ea0*>::insert (0x425480); #335 replaced the
// placeholder Class_00425480::FUN_00425480 that stood for it.
#include <vector>

class Class_004c2ea0 {
public:
    void* field_0;                     // +0x0
    void* field_4;                     // +0x4
    void* field_8;                     // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* name);
};

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct Elem_004222e0 {
    Class_004c9390 name;               // +0x0

    ~Elem_004222e0() { name.ReleaseRef(); }
};

extern std::vector<Class_004c2ea0*>* DAT_00511fb4;

void __stdcall FUN_004bcb50(char* dir, char* pattern, void* list, int flags, char recurse);

// FUNCTION: 0x4222e0
void __stdcall FUN_004222e0()
{
    DAT_00511fb4 = new std::vector<Class_004c2ea0*>;
    std::vector<Elem_004222e0> list;
    FUN_004bcb50("features", "*.tdf", &list, -1, 1);
    for (std::vector<Elem_004222e0>::iterator it = list.begin(); it < list.end(); it++) {
        Class_004c2ea0* obj = new Class_004c2ea0;
        if (((Class_004c2f60*)obj)->FUN_004c2f60(it->name.data)) {
            DAT_00511fb4->push_back(obj);
        } else {
            delete obj;
        }
    }
}
