// Decompiled by Opus. Names are provisional.
// Handler for a two-choice dialog gadget (same shape as 0x446020): plays
// the button sound, "CHOICE1" closes the game window, "CHOICE2" does
// nothing, anything else is passed on.

struct GadgetOwner_00426190 {
    int unknown_0;
    int field_4;                       // +0x4
};

struct Gadget_00426190 {
    char unknown_0[0x18];
    GadgetOwner_00426190* owner;       // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

// 0x4ce190 is called as a method: this, its only caller, loads ecx from
// g_game+0x10 just before the call. data/symbols.csv names it as the free
// function FUN_004ce190 (its body never reads ecx), so the checker reports
// this reference as wrong although the bytes match.
class Class_004ce190 {
public:
    void FUN_004ce190();
};

struct Game {
    char unknown_0[0x10];
    Class_004ce190* field_10;          // +0x10
};

extern Game* g_game;

void __stdcall FUN_0047f1a0(char* str, int flag);
int __stdcall FUN_004a0300(int param1, int param2, char* name);
void __stdcall FUN_004ab0a0(Gadget_00426190* gadget);

// FUNCTION: 0x426190
void __stdcall FUN_00426190(Gadget_00426190* gadget)
{
    int owner = gadget->owner->field_4;
    if (gadget->field_60 == -1)
        return;
    FUN_0047f1a0("SmallButton", 0);
    if (FUN_004a0300(owner, gadget->field_60, "CHOICE1")) {
        g_game->field_10->FUN_004ce190();
    } else if (!FUN_004a0300(owner, gadget->field_60, "CHOICE2")) {
        FUN_004ab0a0(gadget);
    }
}
