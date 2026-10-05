// Decompiled by Opus. Names are provisional.

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x147cf];
    void* unknown_147cf;            // +0x147cf
};
#pragma pack(pop)

extern Game* g_game;

// Declared returning int: the original uses the full eax without masking it.
int __stdcall FUN_004b7f60(void* ptr);

class Class_00471d70 {
public:
    void FUN_00471d70(int param_1);
};

class Class_00475150 {
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
    Vec3_00475150 pos;              // +0x28
    void FUN_00475150(Vec3_00475150* pos, int a, int b, int c);
};

// FUNCTION: 0x475150
void Class_00475150::FUN_00475150(Vec3_00475150* p, int a, int b, int c)
{
    ((Class_00471d70*)this)->FUN_00471d70(c);
    pos = *p;
    unknown_1c = a;
    unknown_24 = FUN_004b7f60(g_game->unknown_147cf) - 1;
    if (b != 0)
        unknown_20 = b;
    else
        unknown_20 = 7;
    v4();
}
