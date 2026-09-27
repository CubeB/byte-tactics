// Decompiled by Opus. Names are provisional.
// Compare 0x4ab4c0 and 0x4ab400: sets the three values at +0x1c..+0x24,
// passes the value on to FUN_004c2b20 and clears the active flag.

extern void __stdcall FUN_004c2b20(int handle);

struct Flags_004ab4e0 {
    unsigned int active : 1;
    unsigned int rest : 31;
};

struct Obj_004ab4e0 {
    char unknown_0[0x1c];
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    char unknown_30[0x54 - 0x30];
    int field_54;                      // +0x54
    char unknown_58[0x5c - 0x58];
    Flags_004ab4e0 flags_5c;           // +0x5c
};

// FUNCTION: 0x4ab4e0
void __stdcall FUN_004ab4e0(Obj_004ab4e0* p, int value)
{
    p->field_1c = value;
    p->field_20 = value;
    p->field_24 = value;
    FUN_004c2b20(value);
    p->flags_5c.active = 0;
    p->field_54 = 0;
    p->field_28 = 0;
    p->field_2c = 0;
}
