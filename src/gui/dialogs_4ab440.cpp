// Decompiled by Opus. Names are provisional.
// Picks the source at +0x28 (alt) or +0x2c; with a source, points the
// reference at +0x30 at it (compare 0x4ab400) and sets the active flag,
// otherwise passes the plain value at +0x24 or +0x20 on to FUN_004c2b20
// (compare 0x4ab4e0) and clears the flag. Nothing happens when the source
// or value is already current.

struct Ref_004ab400;
struct Src_004ab400;

extern void __stdcall InitGafSequence(Ref_004ab400* ref, Src_004ab400* src, int index);
extern int __stdcall GetGafSequenceFrame(Ref_004ab400* ref);
extern void __stdcall FUN_004c2b20(int handle);
int FUN_004c2ba0();

struct Ref_004ab400 {
    unsigned short index;              // +0x0
    unsigned short value;              // +0x2
    unsigned char kind;                // +0x4
    char unknown_5[3];
    Src_004ab400* src;                 // +0x8
};

struct Flags_004ab440 {
    unsigned int active : 1;
    unsigned int rest : 31;
};

struct Obj_004ab440 {
    char unknown_0[0x20];
    int value_20;                      // +0x20
    int value_24;                      // +0x24
    Src_004ab400* src_28;              // +0x28
    Src_004ab400* src_2c;              // +0x2c
    Ref_004ab400 ref;                  // +0x30
    char unknown_3c[0x5c - 0x3c];
    Flags_004ab440 flags_5c;           // +0x5c
};

static inline void SetSource(Obj_004ab440* p, Src_004ab400* src)
{
    if (p->ref.src == src)
        return;
    InitGafSequence(&p->ref, src, 0);
    FUN_004c2b20(GetGafSequenceFrame(&p->ref));
    p->flags_5c.active = 1;
}

static inline void SetValue(Obj_004ab440* p, int value)
{
    if (FUN_004c2ba0() == value)
        return;
    FUN_004c2b20(value);
    p->ref.src = 0;
    p->flags_5c.active = 0;
}

// FUNCTION: 0x4ab440
void __stdcall FUN_004ab440(Obj_004ab440* p, int alt)
{
    if (alt) {
        if (p->src_28)
            SetSource(p, p->src_28);
        else
            SetValue(p, p->value_24);
    } else {
        if (p->src_2c)
            SetSource(p, p->src_2c);
        else
            SetValue(p, p->value_20);
    }
}
