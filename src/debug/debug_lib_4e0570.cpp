// Decompiled by Opus. Names are provisional.
#include <string.h>

extern double GetTimeSeconds();

class Class_004e0520 {
public:
    void FUN_004e0520(int param);
};

class Class_004e05c0 {
public:
    void CreateMemoryStatusDialog();
};

struct Triple_004e0570 {
    int a;
    int b;
    int c;
    Triple_004e0570() { a = 0; b = 0; c = 0; }
};

struct Sub_004e0570 {
    int table[20];                     // +0x00
    Triple_004e0570 triple;            // +0x50
    Sub_004e0570() { memset(table, 0, sizeof(table)); }
};

class Class_004e0570 {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    double time;                       // +0x10
    Sub_004e0570 sub;                  // +0x18
    int unknown_74;                    // +0x74
    unsigned char flag_78;             // +0x78
    Class_004e0570();
};

// FUNCTION: 0x4e0570
Class_004e0570::Class_004e0570()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = -1;
    field_c = -1;
    flag_78 = 0;
    time = GetTimeSeconds();
    ((Class_004e0520*)this)->FUN_004e0520(1);
    ((Class_004e05c0*)this)->CreateMemoryStatusDialog();
}
