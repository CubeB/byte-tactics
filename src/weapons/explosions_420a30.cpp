// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Appends one entry (up to 300) to the table at g_game+0x1491b: a position and
// two references built by InitGafSequence.

struct Src_00420a30 {
    unsigned short count;              // +0x0
    unsigned char kind;                // +0x2
    char unknown_3[0x2c - 3];
    struct { unsigned short value; char unknown_2[6]; } entries[1]; // +0x2c
};

struct Ref_00420a30 {
    unsigned short index;              // +0x0
    unsigned short value;              // +0x2
    unsigned char kind;                // +0x4
    char unknown_5[3];
    Src_00420a30* src;                 // +0x8
};

struct Pos_00420a30 {
    int x;                             // +0x0
    short y_lo;                        // +0x4
    short field_6;                     // +0x6
    int z;                             // +0x8
};

struct Entry_00420a30 {
    int field_0;                       // +0x0
    Ref_00420a30 ref1;                 // +0x4
    Ref_00420a30 ref2;                 // +0x10
    Pos_00420a30 pos;                  // +0x1c
    char unknown_28[0x54 - 0x28];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1427f];
    unsigned char field_1427f;         // +0x1427f
    char unknown_14280[0x1491b - 0x14280];
    int count;                         // +0x1491b
    Entry_00420a30 entries[300];       // +0x1491f
    Src_00420a30* sources[3];          // +0x1ab8f
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall InitGafSequence(Ref_00420a30* ref, Src_00420a30* src, int index);
void __stdcall EmitSmoke(int* pos, int a, int b, int c);

// FUNCTION: 0x420a30
void __stdcall AddExplosionEffect(Pos_00420a30* pos, Src_00420a30* src, int index, int flag)
{
    int* pCount = &g_game->count;
    if (*pCount < 300) {
        Entry_00420a30* e = (Entry_00420a30*)(pCount + 1) + (*pCount)++;
        e->pos = *pos;
        if (src != 0)
            InitGafSequence(&e->ref1, src, 0);
        else
            e->ref1.src = 0;
        if (index >= 0)
            InitGafSequence(&e->ref2, *(Src_00420a30**)((char*)pCount + index * 4 + 0x6274), 0);
        else
            e->ref2.src = 0;
        if (flag == 0 && pos->field_6 > (short)g_game->field_1427f)
            EmitSmoke((int*)pos, 7, 0xf, 9);
        e->field_0 = 0;
    }
}
