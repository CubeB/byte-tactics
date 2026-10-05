// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Info_00408670 {
    char unknown_0[0x8c];
    float field_8c;                    // +0x8c
    float field_90;                    // +0x90
    float field_94;                    // +0x94
    float field_98;                    // +0x98
};

struct Unit_00408670 {
    char unknown_0[0x96];
    Info_00408670* info;               // +0x96
};
#pragma pack(pop)

class Class_0048b090 {
public:
    void SetStateBits(int param_1, int param_2);
};

float __stdcall FUN_00464ad0(void* param_1);
int __stdcall FUN_004b6c30(int range);

// FUNCTION: 0x408670
void __stdcall UpdateConverter(Unit_00408670* unit)
{
    Info_00408670* info = unit->info;
    if (info->field_98 + info->field_98 < info->field_8c) {
        if (FUN_00464ad0(info) > 0.0f && FUN_004b6c30(5) != 0) {
            ((Class_0048b090*)unit)->SetStateBits(1, 1);
        }
    } else {
        ((Class_0048b090*)unit)->SetStateBits(1, 0);
    }
}
