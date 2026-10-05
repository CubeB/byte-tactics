// Decompiled by Opus. Names are provisional.
// A second global object of the class at 0x5129f8 (see 0x44f720.cpp), with
// the same inline constructor and destructor. The compiler generates its
// initialiser (0x44f7e0) and the destructor it registers with atexit
// (0x44f860).

#pragma pack(push, 1)
class Class_005129f8 {
public:
    char* buffer;                      // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    char* table_10;                    // +0x10
    char* table_14;                    // +0x14
    char* table_18;                    // +0x18
    char field_1c;                     // +0x1c
    int field_1d;                      // +0x1d
    int field_21;                      // +0x21

    Class_005129f8()
    {
        buffer = new char[0x6d60];
        field_4 = 0;
        field_8 = 0;
        field_c = 0;
        table_10 = new char[0xaf0];
        table_18 = new char[0xaf0];
        table_14 = new char[0xaf0];
        field_1c = 0;
        field_1d = 0;
        field_21 = 0;
    }
    ~Class_005129f8()
    {
        delete[] buffer;
        delete[] table_10;
        delete[] table_18;
        delete[] table_14;
    }
};
#pragma pack(pop)

// FUNCTION: 0x44f7e0 _$E4
// FUNCTION: 0x44f860 _$E2
Class_005129f8 DAT_005129d0;
