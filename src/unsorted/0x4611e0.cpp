// Decompiled by space-bunny-free. Names are provisional.
// The constructor of the class whose only virtual is its own destructor (the
// vtable is 0x4fd514, the scalar deleting destructor 0x461340, the destructor
// 0x461420). It builds, in declaration order: an int at +0x04, a table of
// eleven 0x1044-byte channels from +0x08, a {pointer, used, capacity} triple at
// +0xb2f4, and a Class_00462d30 member at +0xb300 that is given the new object.
//
// The eleven channels come from a hand-written loop that writes the first
// field through the array index and the rest through a walking pointer: that
// is what leaves the loop with the two induction variables the original has
// (the array start in ecx, and in eax the group member the inlined Init runs
// from). MSVC 5 anchors that second variable on the third store of the first
// inlined member function, so the shape of the members decides every
// displacement in the run: the three-dword Channel_00460f40 at +0x38 gives
// loop one its +0x40, the four-dword Mid_00462d30 at +0x18 gives the ten small
// entries their +0x20. The two setters (Class_00462860::FUN_00462860 with
// 4000 ms and Class_004628a0::FUN_004628a0 with 200 ms) are inlined and
// constant-folded into the two trailing stores, 0x78 and 6.
//
// The small entries are built by the array member's own constructor loop, with
// each tail's buffer allocated by operator new and null-checked: 0x4b4f10 is
// operator new, and the three dwords the constructor writes at the start of the
// new block are the new object's own fields.

#include <stdlib.h>

struct Channel_00460f40 {              // the channel's last member (ctor 0x460f40)
    int field_0;                       // +0x38
    int field_4;                       // +0x3c
    int field_8;                       // +0x40

    void Init() { field_0 = 0; field_4 = 0; field_8 = -1; }
};

struct Channel_00460f60 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30
    int field_34;                      // +0x34
    Channel_00460f40 field_38;         // +0x38
    char unknown_44[0x1044 - 0x44];

    void Init()
    {
        field_4 = 0;
        field_8 = 0;
        field_c = 0;
        field_10 = -2;
        field_14 = -1;
        field_18 = 0;
        field_1c = -1;
        field_20 = 0;
        field_24 = 0;
        field_28 = 0;
        field_2c = 0;
        field_30 = 0;
        field_34 = 0;
        field_38.Init();
    }
};

struct Table_004611e0 {
    Channel_00460f60 channels[11];
    char* buffer;                      // +0xb2f4
    int used;                          // +0xb2f8
    int capacity;                      // +0xb2fc

    Table_004611e0()
    {
        Channel_00460f60* p = channels;
        for (int i = 0; i < 11; i++, p++) {
            channels[i].field_0 = -1;
            p->Init();
            p->field_18 = 0x78;         // 4000 ms in 30 Hz ticks
            p->field_4 = 6;             // 200 ms in 30 Hz ticks
        }
        buffer = 0;
        used = 0;
        capacity = 0;
    }
};

struct Buffer_00462d30 {              // what the new below allocates
    int count;                         // +0x00
    int used;                          // +0x04
    int current;                       // +0x08
    char data[0x180c - 0x0c];

    Buffer_00462d30() { count = 0; used = 0; current = -1; }
};

struct Mid_00462d30 {
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24

    void Init() { field_18 = 0; field_1c = 0; field_20 = 0; field_24 = 0; }
};

struct Tail_00462d30 {
    Mid_00462d30 mid;                  // +0x18
    Buffer_00462d30* buffer;           // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30

    Tail_00462d30()
    {
        buffer = 0;
        mid.Init();
        field_2c = -1;
        field_30 = -1;
        buffer = new Buffer_00462d30;
    }
};

struct F0_00462d30 {
    int field_0;                       // +0x00

    F0_00462d30() { field_0 = -1; }
};

struct Entry_00462d30 : public F0_00462d30 {
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    Tail_00462d30 tail;                // +0x18

    Entry_00462d30()
        : field_4(-1), field_8(-1), field_c(0), field_10(-1), field_14(0)
    {
    }
};

class Class_00462d30 {
public:
    virtual ~Class_00462d30();
    int field_4;                       // +0x04
    void* owner;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    Entry_00462d30 entries[10];         // +0x20
    int field_228;                     // +0x228
    int field_22c;                     // +0x22c
    int field_230;                     // +0x230
    int field_234;                     // +0x234
    int field_238;                     // +0x238

    Class_00462d30(void* o)
        : field_4(0), owner(o), field_c(-1), field_10(-1), field_14(0), field_18(0), field_1c(0),
          field_228(0), field_22c(0), field_230(0), field_234(-1), field_238(-1)
    {
    }
};

class Class_00460f60 {
public:
    virtual ~Class_00460f60();
    int field_4;                       // +0x04
    Table_004611e0 table;              // +0x08
    Class_00462d30 base;               // +0xb300

    Class_00460f60();
};

// FUNCTION: 0x4611e0
Class_00460f60::Class_00460f60() : field_4(200), table(), base(this)
{
}
