// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Sub_004095d0 {
    char unknown_0[0xd4];
    unsigned short field_d4;       // +0xd4
    char unknown_d6[6];
    int field_dc;                  // +0xdc
    char unknown_e0[0x2a];
    char field_10a;                // +0x10a
};

#pragma pack(push, 1)
struct Class_004095d0 {
    char unknown_0[0x186];
    float field_186;               // +0x186
    float field_18a;               // +0x18a
    char unknown_18e[0x1ce - 0x18e];
    float field_1ce;               // +0x1ce
    char unknown_1d2[0x1ee - 0x1d2];
    Sub_004095d0* arr[3];          // +0x1ee
    char unknown_1fa[0x22d - 0x1fa];
    char field_22d;                // +0x22d
    char unknown_22e[0x245 - 0x22e];
    unsigned int bit0_3 : 4;       // +0x245
    unsigned int flag : 1;         // +0x245, bit 4
    unsigned int bit5_31 : 27;
};
#pragma pack(pop)

#define MIN(a, b) (((a) > (b)) ? (b) : (a))

extern float __stdcall GetEnergyUse(Class_004095d0* p);

// FUNCTION: 0x4095d0
int __stdcall FUN_004095d0(Class_004095d0* p)
{
    int result = 1;
    if (p->field_1ce != 0.0f)
        result = 0xb;
    if (p->field_22d != 0)
        result += 10;
    if (GetEnergyUse(p) < 0.0f)
        result += 10;
    result = (int)((int)(result - p->field_18a * -0.01f) - p->field_186 * -0.002f);
    int extra = 1;
    if (p->flag)
        extra = 0xb;
    Sub_004095d0** pp = p->arr;
    for (int i = 3; i != 0; i--) {
        Sub_004095d0* s = *pp;
        if (s->field_10a != 0)
            extra = extra + s->field_d4 / 40 + s->field_dc / 100 + 5;
        pp++;
    }
    result += (signed char)((MIN(extra, 100) < -100) ? -100 : MIN(extra, 100));
    if (MIN(result, 100) < -100)
        return -100;
    return MIN(result, 100);
}
