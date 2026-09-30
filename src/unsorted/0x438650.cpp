// Decompiled by Opus, edited by deepseek-v4.1. Names are provisional.
// Codex / GPT-6 retest in #13:
// signed intermediate products, a scale helper and a widened
// multiplier did not preserve both load order and unsigned float conversion.
// Retain the original best partial, including its redundant 16-bit mask.
//
// deepseek-v4.1 in #1995: 0x437a50 is a byte-identical twin of this function
// (same unit offsets, same 10.0f / 300.0f constants, only the _ftol call and
// the jg target differ) and it has NO "and edx, 0xffff", so a mask-free source
// does exist. A cast-free product of the same four factors gives 52.9%: MSVC5
// computes the division into edx first and loads field_1fe afterwards, so the
// accumulator is edx, not edi. Every narrowing cast tried, (unsigned short),
// (short), (unsigned char), restores the original A-first order and the exact
// original registers, but always appends "and edx, 0xffff" (or the matching
// mask) and turns the file into 167 bytes vs 161. MSVC5 does not elide that
// mask even when the value range is obvious: a micro-test whose operand is an
// unsigned short parameter still emits it, so no cast spelling removes it.
// Still differs by that one instruction (and the jg target it shifts,
// 0x4386e9 vs 0x4386ef).
//
// deepseek-v4.1 retest in #1995: the "twin at 0x437a50" note above is wrong.
// 0x437a50 sits inside Class_00437a30 (a container grow routine) and the whole
// exe has exactly one "fcomp dword [0x4fd2a4]" site, at 0x438665, so there is
// no second copy of this function to copy a mask-free shape from. New variants
// tried this session (signed int field_1fa with an unsigned third parameter,
// the same with only the last factor cast, (unsigned int) around the whole
// product, a q local of int/unsigned int declared before the product, an
// assignment of q inside the product, and moving the (unsigned short) cast to
// the C factor) all reproduce one of two shapes: 96.6% with the mask, or the
// 52.9% shape where MSVC hoists the magic division by 5 above the field_1fe
// load (accumulator edx instead of edi, b->type homed in edi instead of esi).
// Every 16-bit narrowing of the quotient forces the original order and always
// pays the 6-byte and edx,0xffff, so the order and the mask appear to be the
// same front-end decision and this file stays at 96.6%.
// Tried and rejected: all 24 factor orders of (field_1fe, (field_b8+5)/5,
// field_1fa, n) and two-part groupings, an unsigned int numerator local
// (declared first, declared after the float, assigned later, ++-style `*=`
// chain), the operands read through extra locals and pointers, widened casts
// and redundant casts on either side, +0 / *1 / |0 / 0u pads, 5u and
// (unsigned short)5 divisors, float and double conversions of the product,
// and an unsigned short local for the division result.

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
