// Decompiled by Opus, edited by deepseek-v4.1, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.

#pragma pack(push, 1)
struct UnitType_00438650 {
    char unknown_0[0x18a];
    float field_18a;                 // +0x18a
    char unknown_18e[0x1fa - 0x18e];
    unsigned int field_1fa;          // +0x1fa
    unsigned short field_1fe;        // +0x1fe
};

struct Unit {
    char unknown_0[0x92];
    UnitType_00438650* type;         // +0x92
    char unknown_96[0xb8 - 0x96];
    unsigned short field_b8;         // +0xb8
};
#pragma pack(pop)

// FUNCTION: 0x438650
int __stdcall FUN_00438650(Unit* a, Unit* b, int n)
{
    UnitType_00438650* bt = b->type;
    float v = bt->field_18a > 10.0f ? bt->field_18a : 10.0f;
    // The 64-bit numerator is built by hand (signed 32-bit chain in lo, zero hi):
    // keeps the multiply order and the unsigned fild qword.
    union {
        __int64 q;
        struct {
            int lo;
            int hi;
        } w;
    } p;
    p.w.lo = a->type->field_1fe * ((a->field_b8 + 5) / 5) * (int)bt->field_1fa * n;
    p.w.hi = 0;
    int r = (int)((double)p.q / (v * 300.0f));
    if (r <= 1) {
        r = 1;
    }
    return r;
}