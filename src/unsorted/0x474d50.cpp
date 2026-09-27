// Decompiled by Opus. Names are provisional.
// Sibling of 0x475150 (same base call, position copy and virtual call).
struct Vec3_00474d50 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Game_00474d50 {
    char unknown_0[0x147cf];
    void* unknown_147cf;            // +0x147cf
    void* unknown_147d3;            // +0x147d3
};
#pragma pack(pop)

extern Game_00474d50* g_game;

// Declared returning int: the original uses the full eax without masking it.
int __stdcall FUN_004b7f60(void* ptr);

class Class_00471d70 {
public:
    void FUN_00471d70(int param_1);
};

class Class_00474d50 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    char unknown_4[0x1c - 4];
    int unknown_1c;                 // +0x1c
    int unknown_20;                 // +0x20
    int unknown_24;                 // +0x24
    int unknown_28;                 // +0x28
    Vec3_00474d50 pos;              // +0x2c
    void FUN_00474d50(Vec3_00474d50* p, int limit, int a, int b, int c, int alt);
};

// FUNCTION: 0x474d50
void Class_00474d50::FUN_00474d50(Vec3_00474d50* p, int limit, int a, int b, int c, int alt)
{
    ((Class_00471d70*)this)->FUN_00471d70(c);
    pos = *p;
    unknown_1c = a;
    unknown_28 = alt;
    if (alt)
        unknown_24 = FUN_004b7f60(g_game->unknown_147d3) - 1;
    else
        unknown_24 = FUN_004b7f60(g_game->unknown_147cf) - 1;
    if (limit != 0)
        unknown_24 = limit < unknown_24 ? limit : unknown_24;
    if (b != 0)
        unknown_20 = b;
    else
        unknown_20 = 7;
    v4();
}
