// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Save method of CobScript (the object at Unit+0x9a), the counterpart of
// the loader 0x4b2040. Writes the eight 0xa4-byte records at +0x1c, the block
// at ptr10, then one 0x6c-byte record per element of the array at ptr14.

// Chunked file writer / seeker (see src/util/hapibank_4b4cf0.cpp).
class Class_004b4c10 {
public:
    void SeekBox(int pos);
};

class Class_004b4cf0 {
public:
    int WriteBox(void* src, int len);
};

struct Elem_4b0610 {
    int value;                         // +0x0
    char pad[0xa0];                    // pad to stride 0xa4
};

struct Table_004b1ec0 {
    char unknown_0[8];
    int count;                         // +0x8
    char unknown_c[4];
    int size;                          // +0x10
};

struct Vec3_004b1ec0 {
    int v[3];
};

struct Block_004b1ec0 {                // 0x48, six 3-int vectors
    Vec3_004b1ec0 e[6];
};

struct Data_004b1ec0 {                 // 0x4c
    int flag;                          // +0x0
    Block_004b1ec0 block;              // +0x4
};

struct Rec_004b1ec0 {                  // 0xa4
    int value;                         // +0x0
    char unknown_4[0x1c];
    int field_20;                      // +0x20
    char unknown_24[0x80];
};

struct Big_004b1ec0 {                  // 0x528
    int magic;                         // +0x0
    Rec_004b1ec0 recs[8];              // +0x4
    int tail;                          // +0x524
};

struct Rec2_004b1ec0 {                 // 0x6c
    Block_004b1ec0 block;              // +0x0
    int a[3];                          // +0x48
    int b[3];                          // +0x54
    int h0;                            // +0x60
    int h1;                            // +0x64
    int h2;                            // +0x68
};

class CobScript {
public:
    int field_4;                       // +0x4
    Table_004b1ec0* field_8;           // +0x8
    int unknown_c;                     // +0xc
    void* ptr10;                       // +0x10
    Data_004b1ec0* ptr14;              // +0x14
    char unknown_18[0x1c - 0x18];
    Elem_4b0610 arr[8];                // +0x1c
    int field_53c;                     // +0x53c

    virtual void SetPieceTranslation(int, int, int) = 0;  // slot 0
    virtual void SetPieceRotation(int, int, int) = 0;  // slot 1
    virtual void SetPieceVisible(int, int) = 0;       // slot 2
    virtual void SetPieceCached(int, int) = 0;        // slot 3
    virtual void SetPieceShaded(int, int) = 0;        // slot 4
    virtual int GetPieceTranslation(int, int) = 0;    // slot 5
    virtual int GetPieceRotation(int, int) = 0;       // slot 6
    virtual int IsPieceVisible(int);                  // slot 7
    virtual int IsPieceCached(int);                   // slot 8
    virtual int IsPieceShaded(int);                   // slot 9

    void SaveScriptState(Class_004b4cf0* file);
};

// FUNCTION: 0x4b1ec0
void CobScript::SaveScriptState(Class_004b4cf0* file)
{
    ((Class_004b4c10*)file)->SeekBox(0);

    Big_004b1ec0 big;
    big.magic = unknown_c;
    for (int n = 0; n < 8; n++) {
        big.recs[n] = ((Rec_004b1ec0*)((char*)this + 0x1c))[n];
        big.recs[n].field_20 = 0;
    }
    big.tail = field_53c;
    file->WriteBox(&big, 0x528);
    file->WriteBox(ptr10, field_8->size * 4);

    for (int i = 0; i < field_8->count; i++) {
        Rec2_004b1ec0 rec;
        rec.h0 = IsPieceVisible(i);
        rec.h1 = IsPieceCached(i);
        rec.h2 = IsPieceShaded(i);
        for (int j = 0; j <= 2; j++) {
            rec.block.e[0].v[j] = ptr14[i].block.e[0].v[j];
            rec.block.e[1].v[j] = ptr14[i].block.e[1].v[j];
            rec.block.e[2].v[j] = ptr14[i].block.e[2].v[j];
            rec.block.e[3].v[j] = ptr14[i].block.e[3].v[j];
            rec.block.e[4].v[j] = ptr14[i].block.e[4].v[j];
            rec.block.e[5].v[j] = ptr14[i].block.e[5].v[j];
            rec.a[j] = GetPieceTranslation(i, j);
            rec.b[j] = GetPieceRotation(i, j);
        }
        file->WriteBox(&rec, 0x6c);
    }
}
