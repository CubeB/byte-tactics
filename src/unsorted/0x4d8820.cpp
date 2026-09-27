// Decompiled by Sonnet. Names are provisional.

class Class_004d8850 {
public:
    void FUN_004d8850(const char* name);
};

class Class_004d8820 {
public:
    int field_0;
    int field_4;
    int field_8;
    char unknown_c[0x2c - 0xc];
    int field_2c;

    Class_004d8820(int p1, int p2, int p3, int p4, const char* p5);
};

// FUNCTION: 0x4d8820
Class_004d8820::Class_004d8820(int p1, int p2, int p3, int p4, const char* p5)
{
    field_0 = p1;
    field_4 = p2;
    field_8 = p3;
    ((Class_004d8850*)this)->FUN_004d8850(p5);
    field_2c = p4;
}
