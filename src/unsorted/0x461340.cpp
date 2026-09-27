// Decompiled by space-bunny-free. Names are provisional.
// The scalar deleting destructor of Class_004611e0 (its vtable is 0x4fd514,
// one slot; the out-of-line destructor is 0x461420 and the constructor
// 0x4611e0). It is a compiler-generated function, so it is emitted by the
// compiler for the static object below; its body is the whole destructor:
// the Class_00462d30 member (vtable 0x4fd518) and its ten entries first, then
// the eleven big entries, then the deleting-destructor flag test.
//
// The ten small entries are walked with the start pointer one element past the
// end of the array, which is what MSVC 5 emits for an array of objects with
// destructors; the first subtraction lines it up again, so the pointers freed
// are the ones the array really holds.
//
// Suspected bug in the original: the constructor 0x4611e0 initialises its
// eleven big entries at +0x08 (ecx = ebp+8, eleven times, step 0x1044, so
// +0x08 to +0xb2d4), while this destructor frees eleven entries at +0x10
// (+0x10 to +0xb2fc). The last entry's tail is never initialised and is freed
// anyway. The Class_00462d30 member part of both agrees (+0xb300, and the
// constructor writes 0xb304 to 0xb31c, which this destructor reads).

struct Buffers_00462d30 {
    char* a;                           // +0x0
    int field_4;
    int field_8;
    int field_c;
    char* b;                           // +0x10
    char* c;                           // +0x14
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
    char unknown_2c[0x34 - 0x2c];
};

class Class_00462d30 {
public:
    virtual ~Class_00462d30()
    {
        if (field_1c)
            operator delete(field_1c);
        else
            operator delete(field_18);
    }
    int field_4;                       // +0x04
    int field_8;
    int field_c;
    int field_10;
    int field_14;
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
    Entry_00462d30 entries[10];        // +0x20
    char unknown_228[0x24];
};

struct Entry_004611e0 {
    void** items;                      // +0x00
    unsigned count;                    // +0x04
    char unknown_8[0x18];
    void* field_20;                    // +0x20
    char unknown_24[0x1044 - 0x24];
    ~Entry_004611e0()
    {
        if (items) {
            for (unsigned i = 0; i < count; i++)
                delete items[i];
            delete items;
        }
        delete field_20;
    }
};

class Class_004611e0 {
public:
    virtual ~Class_004611e0() { }
    int field_4;                       // +0x04
    int field_8;
    int field_c;
    Entry_004611e0 entries[11];        // +0x10
    int field_b2fc;                    // +0xb2fc
    Class_00462d30 member;             // +0xb300
};

static Class_004611e0 s_obj;

// FUNCTION: 0x461340 ??_GClass_004611e0@@UAEPAXI@Z
