// Decompiled by Space Bunny Free. Names are provisional.
// Replaces the singleton at g_game+0x391e9 with a fresh Class_00434f70 when the
// current one belongs to a different owner, then stores it back (NULL when the
// allocation failed). The constructor body is inlined here.
//
// The original re-reads the global in the delete (the compiler keeps the
// delete's null check because its operand is a load, not the condition's
// value); loading it into a local first drops the check and is 4 bytes short.

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
    char unknown_dc4[0xec4 - 0xdc4];

    Class_00434f70(int owner_);
    ~Class_00434f70();
};

inline Class_00434f70::Class_00434f70(int owner_)
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

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Class_00434f70* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x434ab0
void __stdcall FUN_00434ab0(int owner)
{
    if (g_game->field_391e9 != 0) {
        if (g_game->field_391e9->owner == owner) {
            return;
        }
        delete g_game->field_391e9;
    }
    g_game->field_391e9 = new Class_00434f70(owner);
}