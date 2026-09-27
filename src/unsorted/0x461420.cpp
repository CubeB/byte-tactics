// Decompiled by Space Bunny Free. Names are provisional.
// Out-of-line destructor of the class whose vtable is 0x4fd514 (its scalar
// deleting destructor is 0x461340, which has this body inlined). It first
// destroys the Class_00462d30 member at +0xb300, then the eleven big entries
// at +0x10 in reverse order. The class declarations for Class_00462d30 are
// copied from 0x462cc0.cpp so its destructor is inlined here.
// Note: the constructor at 0x4611e0 initialises its eleven 0x1044 entries at
// +0x8 (so they end at +0xb54), eight bytes away from the range this
// destructor frees (+0x10 to +0xb2fc). The two do not agree; this file follows
// the destructor.

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

struct Entry_00461420 {
    void** items;                      // +0x0
    unsigned count;                    // +0x4
    char unknown_8[0x18];
    void* field_20;                    // +0x20
    char unknown_24[0x1020];
    ~Entry_00461420()
    {
        if (items) {
            for (unsigned i = 0; i < count; i++)
                delete items[i];
            delete items;
        }
        delete field_20;
    }
};

class Class_00460f60 {
public:
    Class_00460f60();
    virtual ~Class_00460f60();
    int field_4;
    char unknown_8[8];
    Entry_00461420 entries[11];        // +0x10
    int field_b2fc;
    Class_00462d30 member;              // +0xb300
};

static Class_00460f60 s_obj;

// FUNCTION: 0x461420
Class_00460f60::~Class_00460f60()
{
}
