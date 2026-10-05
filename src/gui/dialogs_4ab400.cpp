// Decompiled by Opus. Names are provisional.
// Compare 0x4ab4e0: stores the source, points the reference at +0x30 at its
// first entry, sets the active flag and passes the entry's value on to
// FUN_004c2b20.

struct Ref_004ab400;
struct Src_004ab400;

extern void __stdcall InitGafSequence(Ref_004ab400* ref, Src_004ab400* src, int index);
extern int __stdcall GetGafSequenceFrame(Ref_004ab400* ref);
extern void __stdcall FUN_004c2b20(int handle);

struct Ref_004ab400 {
    unsigned short index;              // +0x0
    unsigned short value;              // +0x2
    unsigned char kind;                // +0x4
    char unknown_5[3];
    Src_004ab400* src;                 // +0x8
};

struct Flags_004ab400 {
    unsigned int active : 1;
    unsigned int rest : 31;
};

struct Obj_004ab400 {
    char unknown_0[0x2c];
    Src_004ab400* src;                 // +0x2c
    Ref_004ab400 ref;                  // +0x30
    char unknown_3c[0x5c - 0x3c];
    Flags_004ab400 flags_5c;           // +0x5c
};

// FUNCTION: 0x4ab400
void __stdcall FUN_004ab400(Obj_004ab400* p, Src_004ab400* src)
{
    p->src = src;
    InitGafSequence(&p->ref, src, 0);
    p->flags_5c.active = 1;
    FUN_004c2b20(GetGafSequenceFrame(&p->ref));
}
