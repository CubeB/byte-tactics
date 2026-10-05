// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_00494220 {
    char unknown_0[0xba];
    void* field_ba;                    // +0xba
};
#pragma pack(pop)

struct Inner_00494220 {
    int unknown_0;
    Entry_00494220* entries;           // +0x4
};

struct Gadget_00494220 {
    char unknown_0[0x18];
    Inner_00494220* inner;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Menu_00494220 {
    char unknown_0[1];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_00494220 menu;                // +0x519
    char unknown_51a[0x37ebe - 0x51a];
    unsigned short flags;              // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

Entry_00494220* __stdcall FUN_004a0280(Entry_00494220* entries, char* name);
void __stdcall FreeSurface(void* param_1);
int __stdcall IsCurrentGadgetNamed(Gadget_00494220* gadget, char* name);
void __stdcall PlaySoundByName(char* str, int flag);
void __stdcall FUN_004ab0a0(Menu_00494220* menu);

// FUNCTION: 0x494220
void __stdcall FUN_00494220(Gadget_00494220* gadget)
{
    if (gadget->field_60 == -1) {
        Entry_00494220* e = FUN_004a0280(gadget->inner->entries, "HOTR");
        FreeSurface(e->field_ba);
        g_game->flags &= ~0x800;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "DONE")) {
        PlaySoundByName("smlbutton", 0);
        return;
    }
    FUN_004ab0a0(&g_game->menu);
}
