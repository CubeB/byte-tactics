// Decompiled by Opus. Names are provisional.

class Class_00415c10 {
public:
    void FUN_00415c10(int value, int bits);
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
    virtual void Write(Class_00415c10* stream);     // +0x28
};

struct Target_004908c0 {
    char unknown_0[0x2e];
    unsigned char mode : 2;            // +0x2e
};

struct Holder_004908c0 {
    Target_004908c0* ptr;
};

class Class_004908c0 {
public:
    char unknown_0[4];
    Link_004908c0* link;               // +0x04
    Holder_004908c0* holder;           // +0x08
    char unknown_c[0x27 - 0xc];
    unsigned char state : 3;           // +0x27: bit 0 dirty, bits 1-2 mode

    void FUN_004908c0(Class_00415c10* stream);
};

// Separate dirty:1 and mode:2 fields give two masks (0xfe, 0xf9); the original
// clears all three bits with one 0xf8 mask.
// FUNCTION: 0x4908c0
void Class_004908c0::FUN_004908c0(Class_00415c10* stream)
{
    if (link == 0) {
        stream->FUN_00415c10(0, 2);
    } else if (link->GetType() == 2) {
        stream->FUN_00415c10(1, 2);
        link->Write(stream);
    } else if (link->GetType() == 3) {
        stream->FUN_00415c10(2, 2);
        link->Write(stream);
    }
    stream->FUN_00415c10(holder->ptr->mode, 2);
    state = holder->ptr->mode << 1;     // clears the dirty bit too
}
