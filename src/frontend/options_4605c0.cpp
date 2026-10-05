// Decompiled by Opus. Names are provisional.

struct GadgetOwner_004605c0 {
    int unknown_0;
    int field_4;                       // +0x4
};

struct Gadget_004605c0 {
    char unknown_0[0x18];
    GadgetOwner_004605c0* owner;       // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

class Class_004ce690 {
public:
    void FUN_004ce690(int);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    Class_004ce690* field_10;          // +0x10
    char unknown_14[0x519 - 0x14];
    char field_519[0x3923b - 0x519];   // +0x519
    unsigned char flags_3923b;         // +0x3923b
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_00512ff8;

void __stdcall FUN_0047f1a0(char* str, int flag);
int __stdcall IsGadgetNamed(int param1, int param2, char* name);
void __stdcall FUN_004ab0a0(Gadget_004605c0* gadget);
void FUN_00491b60();
void FUN_00491c60();
int __stdcall FUN_00491d70(int force);
void __stdcall CloseTopScreen(void* queue);
void FUN_004257a0();
void __stdcall FUN_00490b30(int a);

// FUNCTION: 0x4605c0
void __stdcall FUN_004605c0(Gadget_004605c0* gadget)
{
    int owner = gadget->owner->field_4;
    if (gadget->field_60 == -1)
        return;
    FUN_0047f1a0("Exit", 0);
    if (IsGadgetNamed(owner, gadget->field_60, "CHOICE1")) {
        g_game->field_10->FUN_004ce690(4);
        switch (DAT_00512ff8) {
        case 0:
        case 1:
            FUN_00491b60();
            FUN_00491d70(1);
            CloseTopScreen(g_game->field_519);
            FUN_004257a0();
            FUN_00490b30(1);
            return;
        case 2:
            g_game->flags_3923b |= 4;
            FUN_00491c60();
            return;
        }
    } else if (!IsGadgetNamed(owner, gadget->field_60, "CHOICE2")) {
        FUN_004ab0a0(gadget);
    }
}
