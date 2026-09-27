// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Class_00462d30 (vtable
// 0x4fd518, one slot); its out-of-line destructor 0x462d30 is inlined here.
// The class declaration is copied from 0x462d30.cpp with the destructor body
// made inline. The static object below exists only to make the compiler emit
// the vtable (and with it this COMDAT); the real constructor is 0x462c00.

struct Obj_00462d30 {
    int a, b, c;
    Obj_00462d30() { a = 0; b = 0; c = -1; }
};

struct Buffers_00462d30 {
    char* a;                           // +0x0
    int field_4;
    int field_8;
    int field_c;
    char* b;                           // +0x10
    Obj_00462d30* c;                   // +0x14
    ~Buffers_00462d30()
    {
        delete a;
        delete c;
        delete b;
    }
};

struct Entry_00462d30 {
    int field_0;
    int field_4;
    int field_8;
    int field_c;
    int field_10;
    Buffers_00462d30 buffers;          // +0x14
    int field_2c;
    int field_30;
};

class Class_00462d30 {
public:
    Class_00462d30();
    virtual ~Class_00462d30()
    {
        if (field_1c)
            operator delete(field_1c);
        else
            operator delete(field_18);
    }
    int field_4;
    int field_8;
    int field_c;
    int field_10;
    int field_14;
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
    Entry_00462d30 entries[10];        // +0x20
};

// FUNCTION: 0x462cc0 ??_GClass_00462d30@@UAEPAXI@Z
static Class_00462d30 s_obj;
