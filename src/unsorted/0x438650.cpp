// Decompiled by GPT-6. Names are provisional.
// Retains the best earlier partial by Opus.
// Retest: signed intermediate products, a scale helper and a widened
// multiplier did not preserve both load order and unsigned float conversion.
// Retain the original best partial, including its redundant 16-bit mask.

#pragma pack(push, 1)
struct UnitType_00438650 {
    char unknown_0[0x18a];
    float field_18a;                 // +0x18a
    char unknown_18e[0x1fa - 0x18e];
    unsigned int field_1fa;          // +0x1fa
    unsigned short field_1fe;        // +0x1fe
};

struct Unit_00438650 {
    char unknown_0[0x92];
    UnitType_00438650* type;         // +0x92
    char unknown_96[0xb8 - 0x96];
    unsigned short field_b8;         // +0xb8
};
#pragma pack(pop)

// Does not match yet (one extra "and edx, 0xffff"). MSVC re-sorts the
// multiplication chain: when it is unsigned (field_1fa is unsigned, and the
// result is converted to float as unsigned), the division is always evaluated
// before field_1fe; a signed chain gets the original order, but then the float
// conversion is signed. Narrowing the division result to unsigned short gives
// the original order and registers at the cost of the mask.
// FUNCTION: 0x438650
int __stdcall FUN_00438650(Unit_00438650* a, Unit_00438650* b, int n)
{
    UnitType_00438650* bt = b->type;
    float v = bt->field_18a > 10.0f ? bt->field_18a : 10.0f;
    int r = (int)((a->type->field_1fe * (unsigned short)((a->field_b8 + 5) / 5) * bt->field_1fa * n) / (v * 300.0f));
    if (r <= 1) {
        r = 1;
    }
    return r;
}
