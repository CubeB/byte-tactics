// Decompiled by Opus. Names are provisional.

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
    int x1;                  // +8
    int x2;                  // +0xc
    int y1;                  // +0x10
    int y2;                  // +0x14

    int FUN_0044dc60(int* out);
};

static inline int MidX_44dc60(Class_0044dc60* w)
{
    return (w->x1 + w->x2) / 2;
}

// FUNCTION: 0x44dc60
int Class_0044dc60::FUN_0044dc60(int* out)
{
    short avg = (short)MidX_44dc60(this);
    short z = (short)y2;
    MapInfo_44dc60* info = *(MapInfo_44dc60**)(mapPtr + 0xe);
    Pos16_44dc60 pos = info->pos;
    out[0] = (pos.x + avg * 2) << 0x13;
    out[2] = (pos.z + z * 2) << 0x13;
    return 1;
}
