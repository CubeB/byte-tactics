// Decompiled by space-bunny-free. Names are provisional.
// The constructor of the class whose only virtual is its own destructor
// (0x461340 is its scalar deleting destructor, 0x461420 its destructor). It
// builds, in declaration order: an int, a table of eleven big entries, a
// {pointer, used, capacity} buffer triple, and a Class_00462d30 member that is
// given the new object as its argument.
//
// NOT MATCHED YET (68.4%). All 98 instructions are present, in the same order,
// with the same registers, and the two loops write the same bytes; the whole
// remaining difference is which member each loop's induction variable is
// anchored to. The original emits `lea eax, [ecx + 0x40]` in the first loop
// (stores then run from -0x3c up to 0) and `lea esi, [eax + 0x20]` in the second
// (stores from -0x1c up to +0x10). This file emits [ecx + 0x20] and [eax + 0xc],
// which shifts every displacement in the run by a constant while addressing
// exactly the same addresses. Tried: flattening the nested sub-structs into one
// run of plain ints moves both bases to 0xc (worse, and it breaks the second
// loop too); swapping the order of the two redundant trailing stores, and
// anchoring them through `(&p->mid)->field_18` and `(&p->field_4)[0]`, change
// nothing. The base does follow the aggregate nesting, but no consistent rule
// falls out: with the nesting above it is the third int of the *first* nested
// sub-struct in both loops (mid's third int = 0x20, head's third int = 0x0c),
// while the original wants 0x40 and 0x20, which is the third int of the *last*
// sub-struct in the second loop but of no sub-struct at all in the first (0x40
// is the bare trailing int). Worth trying next: a nested sub-struct at +0x38
// holding 0x38/0x3c/0x40, so that the "last sub-struct" reading also gives 0x40
// in the first loop, and re-nesting Class_00462d30 so Head and Tail swap roles.

struct Buffer_00462d30 {
    int count;                         // +0x00
    int used;                          // +0x04
    int current;                       // +0x08
    char data[0x180c - 0x0c];

    Buffer_00462d30() { count = 0; used = 0; current = -1; }
};

struct F0_00462d30 {
    int field_0;                       // +0x00

    F0_00462d30() { field_0 = -1; }
};

struct Head_00462d30 {
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14

    Head_00462d30() { field_4 = -1; field_8 = -1; field_c = 0; field_10 = -1; field_14 = 0; }
};

struct Tail_00462d30 {
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24
    Buffer_00462d30* buffer;           // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30

    Tail_00462d30()
    {
        buffer = 0;
        field_18 = 0; field_1c = 0; field_20 = 0; field_24 = 0; field_2c = -1; field_30 = -1;
    }
};

struct Entry_00462d30 : public F0_00462d30 {
    Head_00462d30 head;                // +0x04
    Tail_00462d30 tail;                // +0x18

    Entry_00462d30() { tail.buffer = new Buffer_00462d30; }
};

struct Tail5_00462d30 {
    int field_228;
    int field_22c;
    int field_230;
    int field_234;
    int field_238;

    Tail5_00462d30() { field_228 = 0; field_22c = 0; field_230 = 0; field_234 = -1; field_238 = -1; }
};

class Class_00462d30 {
public:
    virtual ~Class_00462d30();
    int field_4;                       // +0x04
    void* field_8;                     // +0x08
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

    Class_00462d30(void* owner)
        : field_4(0), field_8(owner), field_c(-1), field_10(-1), field_14(0), field_18(0), field_1c(0)
    {
        field_228 = 0;
        field_22c = 0;
        field_230 = 0;
        field_234 = -1;
        field_238 = -1;
    }
};

struct Mid_004611e0 {
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30
    int field_34;                      // +0x34
    int field_38;                      // +0x38
    int field_3c;                      // +0x3c

    void Init()
    {
        field_18 = 0; field_1c = 0; field_20 = 0; field_24 = 0; field_28 = 0;
        field_2c = 0; field_30 = 0; field_34 = 0; field_38 = 0; field_3c = 0;
    }
};

struct Entry_004611e0 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    Mid_004611e0 mid;                  // +0x18
    int field_40;                      // +0x40
    char unknown_44[0x1044 - 0x44];

    void Init()
    {
        field_4 = 0;
        field_8 = 0;
        field_c = 0;
        field_10 = -2;
        field_14 = -1;
        mid.Init();
        field_40 = -1;
    }
};

struct Table_004611e0 {
    Entry_004611e0 entries[11];
    char* buffer;                      // +0xb2f4
    int used;                          // +0xb2f8
    int capacity;                      // +0xb2fc

    Table_004611e0() { Init(); }

    void Init()
    {
        Entry_004611e0* p = entries;
        for (int i = 0; i < 11; i++, p++) {
            entries[i].field_0 = -1;
            p->Init();
            p->mid.field_18 = 0x78;
            p->field_4 = 6;
        }
        buffer = 0;
        used = 0;
        capacity = 0;
    }
};

class Class_004611e0 {
public:
    virtual ~Class_004611e0();
    int field_4;                       // +0x04
    Table_004611e0 table;              // +0x08
    Class_00462d30 base;               // +0xb300

    Class_004611e0();
};

// FUNCTION: 0x4611e0
Class_004611e0::Class_004611e0() : field_4(200), table(), base(this)
{
}
