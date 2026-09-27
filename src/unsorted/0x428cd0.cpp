// Decompiled by Opus. Names are provisional.
// Returns the next token of the parser: 0x102 at the end, the pushed-back
// token if there is one, otherwise a freshly scanned one (FUN_00428d10),
// which is remembered.

class Class_00428d10 {
public:
    int FUN_00428d10();
};

class Class_00428c90 {
public:
    char flag;                          // +0
    char unknown_1[0x7f];
    char* field_80;                     // +0x80
    char* field_84;                     // +0x84
    char* field_88;                     // +0x88
    int field_8c;                       // +0x8c
    int field_90;                       // +0x90
    int field_94;                       // +0x94
    int FUN_00428cd0();
};

// FUNCTION: 0x428cd0
int Class_00428c90::FUN_00428cd0()
{
    if (field_8c != 0)
        return 0x102;
    if (field_90 != 0) {
        field_90 = 0;
        return field_94;
    }
    field_94 = ((Class_00428d10*)this)->FUN_00428d10();
    return field_94;
}
