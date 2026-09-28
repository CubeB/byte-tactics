// Decompiled by GPT-5.6-Terra. Names are provisional.
// Best attempt: 86.8%. The loader's record loop still differs in its source
// pointer base and MSVC's register allocation for the inner record fields.

class Class_004b4bf0 {
public:
    int FUN_004b4bf0();
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* dst, int len);
};

char* FUN_004d83b0(const char* text, int value);
void FUN_004d85a0(void* ptr);

struct Table_004b2040 {
    char unknown_0[8];
    int count;
    char unknown_c[4];
    int size;
};

struct Vec3_004b2040 {
    int v[3];
};

struct Block_004b2040 {
    Vec3_004b2040 e[6];
};

struct Data_004b2040 {
    int flag;
    Block_004b2040 block;
};

struct Rec_004b2040 {
    int value;
    char unknown_4[0x1c];
    int field_20;
    char unknown_24[0x80];
};

struct Big_004b2040 {
    int magic;
    Rec_004b2040 recs[8];
    int tail;
};

struct Rec2_004b2040 {
    Block_004b2040 block;
    int a[3];
    int b[3];
    int h0;
    int h1;
    int h2;
};

class Class_004b0610 {
public:
    int field_4;
    Table_004b2040* field_8;
    int unknown_c;
    void* ptr10;
    Data_004b2040* ptr14;
    int field_18;
    Rec_004b2040 arr[8];
    int field_53c;

    virtual void FUN_00480c50(int, int, int) = 0;
    virtual void FUN_00480ce0(int, int, int) = 0;
    virtual int FUN_00480d50(int, int) = 0;
    virtual int FUN_00480db0(int, int) = 0;
    virtual int FUN_00480df0(int, int) = 0;
    virtual int FUN_00480c30(int, int) = 0;
    virtual int FUN_00480cb0(int, int) = 0;

    int FUN_004b2040(Class_004b4c80* file);
};

// FUNCTION: 0x4b2040
int Class_004b0610::FUN_004b2040(Class_004b4c80* file)
{
    int size = field_8->size * 4;
    int bytes = field_8->count * 0x6c;
    if (((Class_004b4bf0*)file)->FUN_004b4bf0() != bytes + 0x528 + size) {
        return 0;
    }
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    Big_004b2040 big;
    if (file->FUN_004b4c80(&big, 0x528) != 0x528) {
        return 0;
    }
    if (unknown_c != big.magic) {
        return 0;
    }
    for (int n = 0; n < 8; n++) {
        arr[n] = big.recs[n];
        arr[n].field_20 = 0;
    }
    field_53c = big.tail;
    if (file->FUN_004b4c80(ptr10, size) != size) {
        return 0;
    }
    char* buffer = FUN_004d83b0("Piece States", bytes);
    if (file->FUN_004b4c80(buffer, bytes) != bytes) {
        return 0;
    }
    char* piece = buffer;
    piece += 0x64;
    for (int i = 0; i < field_8->count; i++) {
        Rec2_004b2040* rec = (Rec2_004b2040*)piece;
        ptr14[i].flag = 1;
        FUN_00480d50(i, rec->h0);
        FUN_00480db0(i, rec->h1);
        FUN_00480df0(i, rec->h2);
        for (int j = 0; j <= 2; j++) {
            ptr14[i].block.e[0].v[j] = rec->block.e[0].v[j];
            ptr14[i].block.e[1].v[j] = rec->block.e[1].v[j];
            ptr14[i].block.e[2].v[j] = rec->block.e[2].v[j];
            ptr14[i].block.e[3].v[j] = rec->block.e[3].v[j];
            ptr14[i].block.e[4].v[j] = rec->block.e[4].v[j];
            ptr14[i].block.e[5].v[j] = rec->block.e[5].v[j];
            FUN_00480c50(i, j, rec->a[j]);
            FUN_00480ce0(i, j, rec->b[j]);
        }
        piece += 0x6c;
    }
    field_18 = 1;
    FUN_004d85a0(buffer);
    return 1;
}
