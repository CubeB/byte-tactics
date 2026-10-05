// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Click handler of the serial-link dialog (SERIAL.GUI, opened by FUN_00442560).
// HOST and JOIN set the multiplayer connection-mode bits of g_game (+0x2aaf)
// and save the address returned by FUN_00441c30; PREV goes back; anything else
// resets the gadget. The chosen baud rate and COM port are then written back to
// the registry under SERBAUD and SERPORT.

struct Entry_004423c0 {
    char unknown_0[0xba];
    short field_ba;                  // +0xba
};

struct Layer_004423c0 {
    int unknown_0;
    Entry_004423c0* entries;         // +0x04
};

struct Gadget_004423c0 {
    char unknown_0[0x18];
    Layer_004423c0* layer;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2aaf];
    unsigned short bit0 : 1;         // +0x2aaf
    unsigned short bit1 : 1;
    unsigned short bits2 : 14;
    char unknown_2ab1[0x39211 - 0x2ab1];
    int field_39211;                 // +0x39211
    int field_39215;                 // +0x39215
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00441c30(int* a, int* b);
int __stdcall FindGadgetIndex(void* entries, const char* name, int flag);
int __stdcall IsCurrentGadgetNamed(Gadget_004423c0* gadget, const char* name);
Entry_004423c0* __stdcall FindGadgetChecked(void* entries, char* name);
void __stdcall FUN_0047f1a0(char* name, int param_2);
void FUN_004257a0();
void __stdcall FUN_004ab0a0(Gadget_004423c0* gadget);
void __stdcall FUN_00425860(int state, int line, char* file);
void __stdcall WriteGameRegistryValue(void* key, void* buf, int value);

// FUNCTION: 0x4423c0
void __stdcall FUN_004423c0(Gadget_004423c0* gadget)
{
    Entry_004423c0* entries = gadget->layer->entries;
    int a;
    int r;
    if (gadget->field_60 == -1)
        return;
    if (FindGadgetIndex(entries, "HOST", 0xe) == gadget->field_60) {
        g_game->bit0 = 1;
        g_game->bit1 = 1;
        a = 0;
        int b = 0;
        r = FUN_00441c30(&a, &b);
        if (r >= 0) {
            g_game->field_39211 = a;
            g_game->field_39215 = b;
        }
        FUN_0047f1a0("SMLBUTTON", 0);
        FUN_004257a0();
    } else if (FindGadgetIndex(entries, "JOIN", 0xe) == gadget->field_60) {
        g_game->bit1 = 1;
        g_game->bit0 = 0;
        int a = 0, b = 0;
        r = FUN_00441c30(&a, &b);
        if (r >= 0) {
            g_game->field_39211 = a;
            g_game->field_39215 = b;
        }
        FUN_0047f1a0("SMLBUTTON", 0);
    } else if (IsCurrentGadgetNamed(gadget, "PREV")) {
        FUN_00425860(0xf, 0x395, "c:\\cavedog\\wargame\\multi.cpp");
        FUN_0047f1a0("Previous", 0);
        return;
    } else {
        FUN_004ab0a0(gadget);
        return;
    }
    a = FindGadgetChecked(entries, "SPEEDS")->field_ba;
    WriteGameRegistryValue("SERBAUD", &a, 4);
    a = FindGadgetChecked(entries, "PORTS")->field_ba;
    WriteGameRegistryValue("SERPORT", &a, 4);
}
