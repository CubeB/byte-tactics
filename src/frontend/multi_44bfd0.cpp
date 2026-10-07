// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Walks the 12 "SLIDER%d" gadgets of the "DESCLIST" block, enables/grays each
// one from the per-player slider array (DAT_005129b4), sets its position from
// the array value (or the gadget's own max when the value is -1), then tells
// the menu about it and calls the gadget's own callback.
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_0044bfd0 {                // 0x15b-byte GUI entry
    char unknown_0[0xbc];
    short field_bc;                    // +0xbc first visible item
    char unknown_be[0xd6 - 0xbe];
    char* flags;                       // +0xd6 per-item flags
    char unknown_da[0x13c - 0xda];
    int field_13c;                     // +0x13c maximum
    char unknown_140[0x144 - 0x140];
    void (__stdcall* field_144)(void*, int);  // +0x144 callback
    char unknown_148[0x14a - 0x148];
    int field_14a;                     // +0x14a callback argument
};
#pragma pack(pop)

struct Inner_0044bfd0 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_0044bfd0 {
    char unknown_0[0x18];
    Inner_0044bfd0* inner;             // +0x18
};

#pragma pack(push, 1)
struct SliderInfo_005129b4 {           // 0x62-byte element of DAT_005129b4
    char unknown_0[0x5a];
    int value;                         // +0x5a
    int enabled;                       // +0x5e
};
#pragma pack(pop)

extern SliderInfo_005129b4* DAT_005129b4;

Entry_0044bfd0* __stdcall FindGadgetChecked(void* gadgets, char* name);
Entry_0044bfd0* __stdcall FUN_004a0200(void* gadgets, char* name);
int IsHostLocal();
void __stdcall SetSliderFromValue(Entry_0044bfd0* slider, int value);
void __stdcall FUN_004a1450(Menu_0044bfd0* obj, char* name, int param_3);

// FUNCTION: 0x44bfd0
void __stdcall UpdateUnitSliders(Menu_0044bfd0* param_1, int unused)
{
    // C-style locals, loop counter first: sets the operand order of the flags store.
    int i;
    Entry_0044bfd0* desc;
    int human;
    int base;
    Entry_0044bfd0* slider;
    int en;
    int value;
    char name[20];

    desc = FindGadgetChecked(param_1->inner->gadgets, "DESCLIST");
    human = IsHostLocal();
    base = desc->field_bc;

    for (i = 0; i < 12; i++) {
        sprintf(name, "SLIDER%d", i);
        slider = FUN_004a0200(param_1->inner->gadgets, name);
        if (slider != 0) {
            if (human == 0 || DAT_005129b4[base + i].enabled == 0)
                en = 1;
            else
                en = 0;
            desc->flags[base + i] = en != 0;
            value = DAT_005129b4[base + i].value;
            if (value == -1)
                value = slider->field_13c;
            SetSliderFromValue(slider, value);
            FUN_004a1450(param_1, name, en);
            slider->field_144(param_1, slider->field_14a);
        }
    }
}
