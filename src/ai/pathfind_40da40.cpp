// Decompiled by Sonnet. Names are provisional.

class Target_0040da40 {
public:
    virtual int unused0(int, int);
    virtual int unused1(int, int);
    virtual int unused2(int, int);
    virtual int unused3(int, int);
    virtual int unused4(int, int);
    virtual int unused5(int, int);
    virtual int unused6(int, int);
    virtual int Func(int param1, int param2);
};

class Class_0040da40 {
public:
    char unknown_0[0x50];
    int field_50;                       // +0x50
    char unknown_54[0x60 - 0x50 - 4];
    Target_0040da40* field_60;          // +0x60

    __int64 Estimate(int param1, int param2);
};

// FUNCTION: 0x40da40
__int64 Class_0040da40::Estimate(int param1, int param2)
{
    int a = field_50;
    return ((__int64)field_60->Func(param1, param2) * (__int64)a) >> 0x10;
}
