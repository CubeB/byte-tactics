// Decompiled by Sonnet. Names are provisional.

struct Class_00490880_target {
    char unknown_0[0x2e];
    unsigned char field_2e;
};

struct Class_00490880_link {
    Class_00490880_target* ptr;
};

class Class_00490880 {
public:
    char unknown_0[8];
    Class_00490880_link* field_8;
    char unknown_c[0x27 - 0xc];
    unsigned char field_27;

    void FUN_00490690();
    void FUN_00490880();
};

// FUNCTION: 0x490880
void Class_00490880::FUN_00490880()
{
    unsigned char al = field_27;
    unsigned char bl = al;
    bl = bl >> 1;
    bl = bl & 3;

    unsigned char dl = field_8->ptr->field_2e;
    dl = dl & 3;

    if (dl != bl) {
        al = al | 1;
        field_27 = al;
    }

    FUN_00490690();
}
