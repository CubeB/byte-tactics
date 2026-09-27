// Decompiled by Opus. Names are provisional.
// Constructor: clears the state, stores the owner and resets the object with
// an empty name through FUN_00435110 (which copies the name to +0x4).

class Class_004c2ea0 {
public:
    int field_0;
    int field_4;
    int field_8;

    Class_004c2ea0();
};

class Class_00435110 {
public:
    void FUN_00435110(char* name);
};

extern char DAT_005119b8[];

class Class_00434f70 {
public:
    int owner;                          // +0x0
    char unknown_4[0xa04 - 0x4];
    int field_a04;                      // +0xa04
    Class_004c2ea0 field_a08;           // +0xa08
    char text_a14[0x100];               // +0xa14
    char text_b14[0x100];               // +0xb14
    int field_c14;                      // +0xc14
    char unknown_c18[0xd24 - 0xc18];
    int field_d24;                      // +0xd24
    int field_d28;                      // +0xd28
    int field_d2c;                      // +0xd2c
    char unknown_d30[0xdac - 0xd30];
    int field_dac;                      // +0xdac
    int field_db0;                      // +0xdb0
    int field_db4;                      // +0xdb4
    int field_db8;                      // +0xdb8
    int field_dbc;                      // +0xdbc
    int field_dc0;                      // +0xdc0

    Class_00434f70(int owner_);
};

// FUNCTION: 0x434f70
Class_00434f70::Class_00434f70(int owner_)
{
    field_db8 = 0;
    field_db0 = 0;
    field_dc0 = 0;
    field_db4 = 0;
    field_dac = 0;
    field_dbc = 0;
    field_a04 = 0;
    field_d24 = 0;
    field_d28 = 0;
    field_d2c = 0;
    field_c14 = 0;
    text_a14[0] = 0;
    text_b14[0] = 0;
    owner = owner_;
    ((Class_00435110*)this)->FUN_00435110(DAT_005119b8);
}
