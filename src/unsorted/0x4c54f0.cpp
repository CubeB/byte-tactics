// Decompiled by Space Bunny Free. Names are provisional.
// Loads a TDF section into the global map at 0x51fdb8: the section name is
// compared with the one already loaded, the map is thrown away and rebuilt,
// then every section of the file contributes one entry keyed by its own
// name. The map's owner (0x4c5840) is a packed 17-byte class whose vector
// member starts one byte in, so the insert helper (0x4c59d0) is handed
// this+1; the key the owner is born with is an uninitialised local byte
// (the exe reads [esp+0x17], never written here), kept as `flag`.
//
// Still differs (57%):
//  - the loop counter lives in ebx here, in the frame slot at [esp+0x18] in
//    the original, so every [esp+N] here is 4 low and the frame is 0x220
//    instead of 0x224. The original keeps a constant zero in a register
//    across the loop, which is what squeezes the counter out; writing the
//    strcmp test so that its zero comes from that register (cmp eax,ebx
//    rather than test eax,eax) is the missing piece, and MSVC 5 folds every
//    form of it I tried.
//  - the original materialises the "keys differ" test as a bool in cl with
//    the int form beside it (xor ecx,ecx; cmp; sete cl; neg cl; sbb ecx,ecx;
//    inc ecx; test cl,cl), here it is a plain test eax,eax.
//  - the destructor frees the vector through this+1 (lea edi,[eax+1], then
//    [edi+4]/[edi+8]/[edi+0xc]); written as member accesses of `v` the
//    compiler folds the +1 away and uses this+5/+9/+0xd.
//  - the original re-reads the `section` argument from the stack for the
//    strcpy instead of holding it in ebx, and saves ebx after the strcmpi
//    test, again because ebx is wanted for that zero constant.
#include <string.h>

extern char DAT_0051fdc0[256];
extern char* DAT_005119b8;

void __cdecl FUN_004d83a0(int);
void __cdecl operator delete(void* p);

// A reference counted string handle. 0x4c91b0 builds one from a C string,
// 0x4c9180 makes an empty one, 0x4c93f0 assigns a C string and 0x4c9390 is
// the release.
class Class_004c9390 {
public:
    char* ptr;

    void FUN_004c9390();
};

class Class_004c93f0 {
public:
    char* ptr;

    Class_004c93f0* FUN_004c93f0(const char* text);
};

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 {
public:
    char* ptr;

    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

class Class_004c9180 {
public:
    char* ptr;

    Class_004c9180();
    ~Class_004c9180() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

// One entry of the global map: a key and a value, both string handles.
struct Elem_004c5bc0 {
    Class_004c91a0 key;                  // +0x0
    Class_004c91a0 value;                // +0x4

    ~Elem_004c5bc0();
};

class Class_004c54d0 : public Elem_004c5bc0 {
public:
    Class_004c54d0(const Class_004c91a0& a, const Class_004c91a0& b);
};

// The map itself, keyed by the string the handle points at.
#pragma pack(push, 1)
class Class_004c5c60 {
public:
    char unknown_0[5];
    Elem_004c5bc0* first;               // +0x5
    Elem_004c5bc0* last;                // +0x9

    Elem_004c5bc0* FUN_004c5c60(const char* key);
};

// The same vector seen from the insert helper, which is handed the object
// plus one, so its pointers sit four bytes lower.
class Vec_004c54f0 {
public:
    char count;                          // +0x0
    char pad[3];
    Elem_004c5bc0* first;                // +0x4
    Elem_004c5bc0* last;                 // +0x8
    Elem_004c5bc0* end;                  // +0xc

    Elem_004c5bc0* FUN_004c59d0(Elem_004c5bc0* pos, Elem_004c5bc0* val);
};

class Class_004c5840 {
public:
    char unknown_0;                      // +0x0
    Vec_004c54f0 v;                       // +0x1 (_First at +0x5)

    Class_004c5840(char count);
    ~Class_004c5840();
};
#pragma pack(pop)

extern Class_004c5840* DAT_0051fdb8;

Class_004c5840::Class_004c5840(char count)
{
    v.first = 0;
    v.count = count;
    v.last = 0;
    v.end = 0;
}

Class_004c5840::~Class_004c5840()
{
    Elem_004c5bc0* p = v.first;
    Elem_004c5bc0* e = v.last;
    while (p != e) {
        p->~Elem_004c5bc0();
        p++;
    }
    ::operator delete(v.first);
    v.first = 0;
    v.last = 0;
    v.end = 0;
    ::operator delete(this);
}

// A TDF section: its name and the entries under it.
class Class_004c4420 {
public:
    const char* name;                    // +0x0

    void FUN_004c4420(char* dest, size_t count);
};

class Class_004c48c0 {
public:
    char unknown_0[0x19];

    int FUN_004c48c0(char* dst, const char* key, size_t size, const char* def);
};

// The open TDF file.
class Class_004c2ea0 {
public:
    int root;                            // +0x0
    Class_004c4420* current;             // +0x4
    int file;                            // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    char unknown_0[4];
    int field_4;

    int FUN_004c2f60(char* filename);
};

class Class_004c3490 {
public:
    char unknown_0[4];
    int field_4;

    Class_004c4420* FUN_004c3490(int index);
};

class Class_004c3e10 {
public:
    char unknown_0[4];
    int field_4;

    void FUN_004c3e10();
};

class Class_004c3240 {
public:
    char unknown_0[4];
    int field_4;
    int field_8;

    void FUN_004c3240();
};

// FUNCTION: 0x4c54f0
void __stdcall FUN_004c54f0(char* filename, char* section)
{
    char flag;
    int i;

    if (_strcmpi(section, DAT_0051fdc0) == 0)
        return;
    if (DAT_0051fdb8)
        DAT_0051fdb8->~Class_004c5840();
    DAT_0051fdb8 = new Class_004c5840(flag);
    FUN_004d83a0((int)DAT_0051fdb8);
    strcpy(DAT_0051fdc0, section);
    {
        Class_004c2ea0 f;
        char value[256];
        char name[256];
        if (((Class_004c2f60*)&f)->FUN_004c2f60(filename)) {
            for (i = 0; ((Class_004c3490*)&f)->FUN_004c3490(i); i++) {
                f.current->FUN_004c4420(name, 0xff);
                ((Class_004c48c0*)f.current)->FUN_004c48c0(value, DAT_0051fdc0, 0xff, DAT_005119b8);
                if (strlen(value) != 0) {
                    Class_004c91b0 key(name);
                    Class_004c5840* s = DAT_0051fdb8;
                    Elem_004c5bc0* e = ((Class_004c5c60*)s)->FUN_004c5c60(key.ptr);
                    if (e == ((Class_004c5c60*)s)->last || !(strcmp(e->key.ptr, key.ptr) == 0)) {
                        Class_004c9180 empty;
                        Class_004c54d0 tmp((Class_004c91a0&)key, (Class_004c91a0&)empty);
                        e = ((Vec_004c54f0*)((char*)s + 1))->FUN_004c59d0(e, (Elem_004c5bc0*)&tmp);
                    } else {
                        ((Class_004c93f0*)&e->value)->FUN_004c93f0(value);
                    }
                }
                ((Class_004c3e10*)&f)->FUN_004c3e10();
            }
            ((Class_004c3240*)&f)->FUN_004c3240();
        }
    }
}
