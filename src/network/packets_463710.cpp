// Decompiled by Opus. Names are provisional.
// Destructor of the class built by 0x4636b0 (the same layout as the
// Tail_004635b0 member of Class_004635b0, whose fields 0x463680 frees in the
// same order): frees the buffer at +0x10, then the block at +0xc.

struct Buffer_004636b0;

class Class_004636b0 {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    void* field_c;                     // +0x0c
    Buffer_004636b0* buffer;           // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    ~Class_004636b0();
};

// FUNCTION: 0x463710
Class_004636b0::~Class_004636b0()
{
    operator delete(buffer);
    operator delete(field_c);
}
