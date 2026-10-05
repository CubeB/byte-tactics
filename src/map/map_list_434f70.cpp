// Decompiled by Opus. Names are provisional.
// Constructor: clears the state, stores the owner and resets the object with
// an empty name through LoadCampaign (which copies the name to +0x4).

class TdfFile {
public:
    int field_0;
    int field_4;
    int field_8;

    TdfFile();
};

extern char DAT_005119b8[];

class Mission {
public:
    int owner;                          // +0x0
    char unknown_4[0xa04 - 0x4];
    int field_a04;                      // +0xa04
    TdfFile field_a08;                  // +0xa08
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

    Mission(int owner_);
    void LoadCampaign(char* name);
};

// FUNCTION: 0x434f70
Mission::Mission(int owner_)
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
    ((Mission*)this)->LoadCampaign(DAT_005119b8);
}
