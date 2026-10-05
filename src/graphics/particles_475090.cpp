// Decompiled by Haiku. Names are provisional.

class Class_00475090 {
public:
    char unknown_0[0x10];
    int field_10;
    int field_14;

    int IsExpired(int unused);
};

// FUNCTION: 0x475090
int Class_00475090::IsExpired(int unused)
{
    return field_14 >= field_10 ? 1 : 0;
}
