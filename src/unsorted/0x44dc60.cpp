// Decompiled by Sonnet. Names are provisional.

struct Pos16_44dc60 {
    short x;
    short z;
};

struct MapInfo_44dc60 {
    char unknown_0[0x7e];
    Pos16_44dc60 pos;   // +0x7e
};

struct Class_0044dc60 {
    char unknown_0[4];
    char* mapPtr;            // +4
    int f8;                  // +8
    int fc;                  // +0xc
    char unknown_1[0x14 - 0xc - 4];
    short f14;               // +0x14

    int FUN_0044dc60(int* out);
};

// FUNCTION: 0x44dc60
int Class_0044dc60::FUN_0044dc60(int* out)
{
    short avg = (short)((f8 + fc) / 2);
    short z = f14;
    MapInfo_44dc60* info = *(MapInfo_44dc60**)(mapPtr + 0xe);
    Pos16_44dc60 pos = info->pos;
    out[0] = (pos.x + avg * 2) << 0x13;
    out[2] = (pos.z + z * 2) << 0x13;
    return 1;
}
