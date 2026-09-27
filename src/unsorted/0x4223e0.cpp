// Decompiled by Opus. Names are provisional.
// Codex / GPT-6 retest in #13:
// a vector-shaped container and a release helper taking the first
// and last pointers by reference still produced 109 bytes, not 127.
// Deletes every entry of the global vector at DAT_00511fb4 (see 0x422460),
// then the vector itself.
//
// Not matched: the original keeps &_First and &_Last of the inlined ~vector in
// esi/ebx across operator delete and stores immediate zeros; this version
// keeps a zero register instead. The same lea pattern appears where this code
// is inlined at 0x424f10 (with an out-of-line _Destroy, 0x4251e0), so the
// source construct is probably shared; wrappers, derived/member classes,
// explicit destructor calls, element types and header sets did not change it.
#include <vector>

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

extern std::vector<Class_004c2ea0*>* DAT_00511fb4;

// FUNCTION: 0x4223e0
void FUN_004223e0()
{
    for (Class_004c2ea0** p = DAT_00511fb4->begin(); p < DAT_00511fb4->end(); p++)
        delete *p;
    delete DAT_00511fb4;
    DAT_00511fb4 = 0;
}
