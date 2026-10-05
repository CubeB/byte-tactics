// Decompiled by Sonnet. Names are provisional.

struct ArrayB {
    char unknown_0[8];
    int value;                          // +0x8
    char unknown_c[8];
};

struct ArrayA {
    char unknown_0[0xc];
    int subIndex;                       // +0xc
    char unknown_10[4];
    ArrayB* ptr2;                       // +0x14
};

struct Table_004b4bf0 {
    char unknown_0[4];
    ArrayA* base;                       // +0x4
    int index;                          // +0x8
};

class Class_004b4bf0 {
public:
    Table_004b4bf0* table;              // +0x0

    int FUN_004b4bf0();
};

// FUNCTION: 0x4b4bf0
int Class_004b4bf0::FUN_004b4bf0()
{
    Table_004b4bf0* p = table;
    ArrayA* a = &p->base[p->index];
    int subIdx = p->base[p->index].subIndex;
    return a->ptr2[subIdx].value;
}
