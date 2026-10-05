// Decompiled by Opus. Names are provisional.
// Resets a table of eight bytes {0, 40, 60, 80, 100, 80, 60, 40} copied from
// a constant, and four byte pairs to {16, 22}.

struct Table_0040d880 {
    unsigned char values[8];
};

struct Pair_0040d880 {
    unsigned char a;
    unsigned char b;
};

extern const Table_0040d880 DAT_004fca10;

class Class_0040d880 {
public:
    char unknown_0[0x68];
    Table_0040d880 table;              // +0x68
    Pair_0040d880 pairs[4];            // +0x70

    void InitCostTables();
};

// FUNCTION: 0x40d880
void Class_0040d880::InitCostTables()
{
    table = DAT_004fca10;
    pairs[0].a = pairs[1].a = pairs[2].a = pairs[3].a = 0x10;
    pairs[0].b = pairs[1].b = pairs[2].b = pairs[3].b = 0x16;
}
