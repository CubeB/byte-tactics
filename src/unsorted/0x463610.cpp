// Decompiled by Opus. Names are provisional.
// PlayerFrameInfo::Initialize (from its debug string), a method of the class
// built by the constructor 0x4635b0: resets the fields that constructor sets
// and allocates the buffer if there is none yet. The tail's reset is an
// inline method of the member (same layout as Class_004636b0); written out
// flat, MSVC hoists the buffer load above the head stores.

struct Buffer_00463610 {
    int count;                         // +0x0
    int used;                          // +0x4
    int current;                       // +0x8
    char data[0x180c - 0xc];

    Buffer_00463610() { count = 0; used = 0; current = -1; }
};

struct Head_00463610 {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14

    void Init(long id) { field_0 = id; field_4 = -1; field_8 = -1; field_c = 0; }
};

struct Tail_00463610 {
    int field_0;                       // +0x18
    int field_4;                       // +0x1c
    int field_8;                       // +0x20
    int field_c;                       // +0x24
    Buffer_00463610* buffer;           // +0x28
    int field_14;                      // +0x2c
    int field_18;                      // +0x30

    void Init()
    {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        field_c = 0;
        field_14 = -1;
        field_18 = -1;
        if (buffer == 0)
            buffer = new Buffer_00463610;
    }
};

class Class_004635b0 {
public:
    Head_00463610 head;                // +0x00
    Tail_00463610 tail;                // +0x18

    void FUN_00463610(long id);
};

void FUN_00461170(const char* fmt, ...);

// FUNCTION: 0x463610
void Class_004635b0::FUN_00463610(long id)
{
    FUN_00461170("PlayerFrameInfo::Initialize: %ld", id);
    head.Init(id);
    tail.Init();
}
