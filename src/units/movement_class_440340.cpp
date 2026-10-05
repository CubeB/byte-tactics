// Decompiled by space-bunny-free. Names are provisional.
// Reads the movement fields from a TDF section (the object at src+4) into the
// 32-byte entry whose layout 0x440290.cpp declares as MovementClass. The four
// slope fields are clamped so that each is never larger than its limit.

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

struct Source_00440340 {
    char unknown_0[4];
    Class_004c46c0* tdf;               // +0x4
};

struct MovementClass {
    int* field_0;
    short field_4;                     // FootPrintX
    short field_6;                     // FootPrintZ
    short field_8;                     // maxwaterdepth
    short field_a;                     // minwaterdepth
    unsigned char field_c;             // maxslope
    unsigned char field_d;             // badslope
    unsigned char field_e;             // maxwaterslope
    unsigned char field_f;             // badwaterslope
    int field_10;
    int field_14;
    void* field_18;
    int field_1c;

    void ReadMoveInfo(Source_00440340* src);
};

// FUNCTION: 0x440340
void MovementClass::ReadMoveInfo(Source_00440340* src)
{
    field_4 = src->tdf->FUN_004c46c0("FootPrintX", 0);
    field_6 = src->tdf->FUN_004c46c0("FootPrintZ", 0);
    field_8 = src->tdf->FUN_004c46c0("maxwaterdepth", field_8);
    field_a = src->tdf->FUN_004c46c0("minwaterdepth", field_a);
    field_c = src->tdf->FUN_004c46c0("maxslope", field_c);
    field_d = src->tdf->FUN_004c46c0("badslope", field_c / 2);
    field_e = src->tdf->FUN_004c46c0("maxwaterslope", field_e);
    field_f = src->tdf->FUN_004c46c0("badwaterslope", field_e / 2);
    if (field_c > field_e)
        field_c = field_e;
    if (field_d > field_c)
        field_d = field_c;
    if (field_f > field_e)
        field_f = field_e;
}
