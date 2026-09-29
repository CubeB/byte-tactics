// Decompiled by GPT-5.6-Terra, finished by space-bunny-free and deepseek-v4.1-flash. Names are
// provisional.
// Finishing note (deepseek-v4.1-flash): the loader walks the source buffer with an INDEX
// (buffer[i]), not with a walking pointer.  As a pointer the two loop-carried values came
// out in the other stack slots and MSVC picked e[2] (0x18) as the block pivot, giving
// "add esi,-0x4c"; as an index the record is the plain 0x6c struct (int e[6][3] at 0,
// a[3] at 0x48, b[3] at 0x54, h[3] at 0x60), MSVC picks e[1] at 0x0c and emits the
// original's "add esi,-0x58".  The esi bias is per outer iteration only: the latch stores
// the unbiased buffer pointer back into [esp+0x10], so the j loop's "+4" never carries.
// The three virtual calls before the j loop read the record's h[3] at 0x60; the two in the
// j loop read a[j] at 0x48 and b[j] at 0x54.

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

struct Data_004b2040 {
    int flag;
    int e[6][3];
};

struct Rec2_004b2040 {
    int e[6][3];
    int a[3];
    int b[3];
    int h[3];
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
    Rec2_004b2040* buffer = (Rec2_004b2040*)FUN_004d83b0("Piece States", bytes);
    if (file->FUN_004b4c80(buffer, bytes) != bytes) {
        return 0;
    }
    for (int i = 0; i < field_8->count; i++) {
        ptr14[i].flag = 1;
        FUN_00480d50(i, buffer[i].h[0]);
        FUN_00480db0(i, buffer[i].h[1]);
        FUN_00480df0(i, buffer[i].h[2]);
        for (int j = 0; j <= 2; j++) {
            ptr14[i].e[0][j] = buffer[i].e[0][j];
            ptr14[i].e[1][j] = buffer[i].e[1][j];
            ptr14[i].e[2][j] = buffer[i].e[2][j];
            ptr14[i].e[3][j] = buffer[i].e[3][j];
            ptr14[i].e[4][j] = buffer[i].e[4][j];
            ptr14[i].e[5][j] = buffer[i].e[5][j];
            FUN_00480c50(i, j, buffer[i].a[j]);
            FUN_00480ce0(i, j, buffer[i].b[j]);
        }
    }
    field_18 = 1;
    FUN_004d85a0(buffer);
    return 1;
}
