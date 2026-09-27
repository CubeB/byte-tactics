// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Walks the 12 "SLIDER%d" gadgets of the "DESCLIST" block, enables/grays each
// one from the per-player slider array (DAT_005129b4), sets its position from
// the array value (or the gadget's own max when the value is -1), then tells
// the menu about it and calls the gadget's own callback.
//
// STILL DIFFERS (82.1%): the flags store's address association. The original
// computes (flags + base) and then indexes by the loop counter i
// (`add edx, ecx; mov [edx+edi], al`). With a plain `int base` MSVC 5 instead
// computes (flags + i) and indexes by base (`add edx, edi; mov [edx+ecx], al`),
// and that also rotates the later scratch registers (the DAT value load, the
// name lea and the callback argument). Narrowing the index with `(short)base`
// restores the original association, but MSVC then reloads base with
// `movsx ecx, word ptr [esp+0x18]` instead of `mov ecx, dword ptr [esp+0x18]`
// and hoists that load above the enabled test. The local `Locals` struct only
// pins the three locals to the original stack slots (human 0x10, desc 0x14,
// base 0x18); the plain declarations of the narrow form swap desc and base.
// Dropping the `(short)` cast gives the natural int form, which keeps the dword
// reload and the slot order but has the association backwards.
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_0044bfd0 {                // 0x15b-byte GUI entry
    char unknown_0[0xbc];
    short field_bc;                    // +0xbc base index
    char unknown_be[0xd6 - 0xbe];
    char* flags;                       // +0xd6 enabled flags
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

Entry_0044bfd0* __stdcall FUN_0049ff90(void* gadgets, char* name);
Entry_0044bfd0* __stdcall FUN_004a0200(void* gadgets, char* name);
int FUN_00457a50();
void __stdcall FUN_0045b9b0(Entry_0044bfd0* slider, int value);
void __stdcall FUN_004a1450(Menu_0044bfd0* obj, char* name, int param_3);

// FUNCTION: 0x44bfd0
void __stdcall FUN_0044bfd0(Menu_0044bfd0* param_1, int unused)
{
    struct Locals { int human; Entry_0044bfd0* desc; int base; } loc;
    loc.desc = FUN_0049ff90(param_1->inner->gadgets, "DESCLIST");
    loc.human = FUN_00457a50();
    loc.base = loc.desc->field_bc;

    for (int i = 0; i < 12; i++) {
        char name[20];
        sprintf(name, "SLIDER%d", i);
        Entry_0044bfd0* slider = FUN_004a0200(param_1->inner->gadgets, name);
        if (slider != 0) {
            int en;
            if (loc.human == 0 || DAT_005129b4[loc.base + i].enabled == 0)
                en = 1;
            else
                en = 0;
            loc.desc->flags[(short)loc.base + i] = en != 0;
            int value = DAT_005129b4[loc.base + i].value;
            if (value == -1)
                value = slider->field_13c;
            FUN_0045b9b0(slider, value);
            FUN_004a1450(param_1, name, en);
            slider->field_144(param_1, slider->field_14a);
        }
    }
}
