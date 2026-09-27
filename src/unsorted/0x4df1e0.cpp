// Decompiled by Opus. Names are provisional.

extern char DAT_005119b8[];
extern int DAT_00529dcc;
extern void* DAT_00529df8;

extern double FUN_004e1730();
void FUN_004e1be0();

class Class_004e17c0 {
public:
    char unknown_0[0x14];
    Class_004e17c0* FUN_004e17c0();
};

class Class_004df250 {
public:
    void FUN_004df250();
};

class Id_004df1e0 {
public:
    const char* id;                    // +0x0
    char text[0x1f4];                  // +0x4
    Id_004df1e0(const char* s)
    {
        id = s;
        if (id == 0)
            id = DAT_005119b8;
        text[0] = 0;
    }
};

class Class_004df1e0 {
public:
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    int unknown_8;                     // +0x8
    int unknown_c;                     // +0xc
    double time;                       // +0x10
    int count;                         // +0x18
    void* table;                       // +0x1c
    char flag_20;                      // +0x20
    Id_004df1e0 ident;                 // +0x24
    Class_004e17c0 map;                // +0x21c

    Class_004df1e0();
};

// FUNCTION: 0x4df1e0
Class_004df1e0::Class_004df1e0()
    : ident("This is a unique identifier, isn't it - tell me the truth!")
{
    map.FUN_004e17c0();
    unknown_0 = 0;
    unknown_4 = -1;
    unknown_8 = -1;
    flag_20 = 0;
    FUN_004e1be0();
    count = DAT_00529dcc;
    table = DAT_00529df8;
    time = FUN_004e1730();
    ((Class_004df250*)this)->FUN_004df250();
}
