// Decompiled by Opus. Names are provisional.

class BitWriter {
public:
    void WriteBits(int value, int bits);
};

class Link_004908c0 {
public:
    virtual void vf0();
    virtual void vf1();
    virtual int GetType();                          // +0x08
    virtual void vf3();
    virtual void vf4();
    virtual void vf5();
    virtual void vf6();
    virtual void vf7();
    virtual void vf8();
    virtual void vf9();
    virtual void Write(BitWriter* stream);          // +0x28
};

struct Target_004908c0 {
    char unknown_0[0x2e];
    unsigned char mode : 2;            // +0x2e
};

struct Holder_004908c0 {
    Target_004908c0* ptr;
};

// Class_004907e0's override of slot 8 (vtable 0x4fd9b0, see 0x44ef60.cpp for
// the family): writes the object at +0x4 (a 2-bit kind, then its own data)
// and the owner's mode to the stream, then takes that mode as its own and
// clears the dirty bit. The matching reader looks like 0x490a10
// (Class_00490880's slot 9).
class Class_004907e0 {
public:
    Link_004908c0* link;               // +0x04
    Holder_004908c0* holder;           // +0x08
    char unknown_c[0x27 - 0xc];
    unsigned char state : 3;           // +0x27: bit 0 dirty, bits 1-2 mode

    virtual void FUN_0044efc0(BitWriter* stream);       // slot 8
};

// Separate dirty:1 and mode:2 fields give two masks (0xfe, 0xf9); the original
// clears all three bits with one 0xf8 mask.
// FUNCTION: 0x4908c0
void Class_004907e0::FUN_0044efc0(BitWriter* stream)
{
    if (link == 0) {
        stream->WriteBits(0, 2);
    } else if (link->GetType() == 2) {
        stream->WriteBits(1, 2);
        link->Write(stream);
    } else if (link->GetType() == 3) {
        stream->WriteBits(2, 2);
        link->Write(stream);
    }
    stream->WriteBits(holder->ptr->mode, 2);
    state = holder->ptr->mode << 1;     // clears the dirty bit too
}
