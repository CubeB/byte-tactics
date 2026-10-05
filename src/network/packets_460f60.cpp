// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// The compiler-generated static destructor (_$E2) of the global object
// DAT_00513000, whose dynamic initialiser is 0x460e20 and whose out-of-line
// destructor is 0x461420. The class is built so the compiler inlines the
// embedded Class_00462d30 destructor and the two array destructors in the same
// order as the original.

void __cdecl operator delete(void*);

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
    ~Buffers_00462d30();
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
    virtual ~Class_00462d30();
    int field_4;
    int field_8;
    int field_c;
    int field_10;
    int field_14;
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
    Entry_00462d30 entries[10];        // +0x20
};

// One of the eleven per-player objects embedded in the global.
struct Sub_00460f60 {
    void** items;                      // +0x0
    unsigned int count;                // +0x4
    char pad[0x18];
    void* q;                           // +0x20
    char pad2[0x1020];
    ~Sub_00460f60();
};

class Class_00460f60 {
public:
    char pad0[0xc];
    Sub_00460f60 subs[11];             // +0x10
    char pad1[4];
    Class_00462d30 member;             // +0xb300
    virtual ~Class_00460f60() {}
};

// FUNCTION: 0x460f60 _$E2
Class_00460f60 DAT_00513000;

Buffers_00462d30::~Buffers_00462d30()
{
    delete a;
    delete c;
    delete b;
}

Class_00462d30::~Class_00462d30()
{
    void* p = field_1c;
    if (!p)
        p = field_18;
    operator delete(p);
}

Sub_00460f60::~Sub_00460f60()
{
    if (items) {
        for (unsigned int i = 0; i < count; i++)
            operator delete(items[i]);
        operator delete(items);
    }
    operator delete(q);
}
