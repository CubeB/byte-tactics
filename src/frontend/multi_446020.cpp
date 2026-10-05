// Decompiled by Opus. Names are provisional.
// Handler for a two-choice dialog gadget: "CHOICE1" acts on the local
// player, "CHOICE2" does nothing, anything else is passed on.

struct GadgetOwner_00446020 {
    int unknown_0;
    int field_4;                       // +0x4
};

struct Gadget_00446020 {
    char unknown_0[0x18];
    GadgetOwner_00446020* owner;       // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

extern unsigned char DAT_00505510;

int __stdcall IsGadgetNamed(int param1, int param2, char* name);
void __stdcall FUN_004ab0a0(Gadget_00446020* gadget);
int __stdcall GetSlotDpid(unsigned char player);
void __stdcall RejectPlayer(int param_1, int param_2);

// FUNCTION: 0x446020
void __stdcall HandleRejectChoice(Gadget_00446020* gadget)
{
    int owner = gadget->owner->field_4;
    if (gadget->field_60 == -1)
        return;
    if (IsGadgetNamed(owner, gadget->field_60, "CHOICE1")) {
        RejectPlayer(GetSlotDpid(DAT_00505510), 1);
    } else if (!IsGadgetNamed(owner, gadget->field_60, "CHOICE2")) {
        FUN_004ab0a0(gadget);
    }
}
