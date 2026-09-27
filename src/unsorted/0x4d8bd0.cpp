// Decompiled by Haiku. Names are provisional.

class Class_004d87f0 {
public:
    int field_0;
    int field_4;
    int field_8;

    Class_004d87f0(void);
};

class Class_004d8870 {
public:
    char data[0x8c];
    Class_004d8870(void);
};

class Class_004d8bd0 : public Class_004d87f0 {
public:
    char unknown_c[0x24];
    Class_004d8870 member_30;
    Class_004d8870 member_bc;
    char unknown_148[0x1];

    Class_004d8bd0(void);
};

// FUNCTION: 0x4d8bd0
Class_004d8bd0::Class_004d8bd0(void) :
    Class_004d87f0(),
    member_30(),
    member_bc()
{
    unknown_148[0] = 0;
}
