// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Click handler of the TCP dialog (TCP.GUI, opened by FUN_004421f0). OK and a
// direct-connect address both set the multiplayer connection-mode bits of
// g_game (+0x2aaf) and connect; JOIN sets only the second bit; ADDRESS selects
// the address gadget; PREV goes back; anything else resets the gadget. The
// address text is then written to the registry under TCPADDR.

struct Entry_00442050 {
    char unknown_0[0xba];
    short field_ba;                  // +0xba
};

struct Layer_00442050 {
    int unknown_0;
    Entry_00442050* entries;         // +0x04
};

struct Gadget_00442050 {
    char unknown_0[0x18];
    Layer_00442050* layer;           // +0x18
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
extern int DAT_00512c84;
extern char DAT_00512d90[];

int __stdcall FUN_00441c30(int* a, int* b);
int __stdcall FindGadgetIndex(void* entries, const char* name, int flag);
int __stdcall IsCurrentGadgetNamed(Gadget_00442050* gadget, const char* name);
void __stdcall FUN_0047f1a0(const char* name, int param_2);
void FUN_004257a0();
void __stdcall FUN_004ab0a0(Gadget_00442050* gadget);
void __stdcall FUN_00425860(int state, int line, const char* file);
void __stdcall FUN_0042f960(void* key, void* buf, int value);
char* __stdcall GetGadgetText(Gadget_00442050* gadget, const char* key, void* out);

// The original build called this helper from both connection paths and /Ob2
// inlined it at each one; 0x442000 is the out-of-line copy of the same body.
// Writing the two zero stores inside the helper, rather than inline in the
// caller, is what makes MSVC 5 schedule the first store after the argument
// push (`mov [esp+0x1c], ebx`), matching the original. Spelling the same
// statements inline in FUN_00442050 hoists that store one instruction early.
static int TryConnect_00442050()
{
    int a = 0;
    int b = 0;
    int result = FUN_00441c30(&a, &b);
    if (result >= 0) {
        g_game->field_39211 = a;
        g_game->field_39215 = b;
    }
    return result;
}

// FUNCTION: 0x442050
void __stdcall FUN_00442050(Gadget_00442050* gadget)
{
    Entry_00442050* entries = gadget->layer->entries;
    if (DAT_00512d90[0] != 0) {
        if (DAT_00512c84 == 0)
            DAT_00512d90[0] = 0;
    }
    else if (gadget->field_60 == -1) {
        return;
    }
    else if (FindGadgetIndex(entries, "OK", 0xe) == gadget->field_60) {
        goto connect;
    }
    else if (FindGadgetIndex(entries, "JOIN", 0xe) == gadget->field_60) {
        g_game->bit1 = 1;
        g_game->bit0 = 0;
        TryConnect_00442050();
        FUN_0047f1a0("Smlbutton", 0);
        goto tcpaddr;
    }
    else if (IsCurrentGadgetNamed(gadget, "ADDRESS") != 0) {
    }
    else {
        if (IsCurrentGadgetNamed(gadget, "PREV") != 0) {
            FUN_00425860(0xf, 0x305, "c:\\cavedog\\wargame\\multi.cpp");
            FUN_0047f1a0("Previous", 0);
            return;
        }
        FUN_004ab0a0(gadget);
        return;
    }
connect:
    g_game->bit0 = 1;
    g_game->bit1 = 1;
    TryConnect_00442050();
    FUN_0047f1a0("Smlbutton", 0);
    FUN_004257a0();
tcpaddr:
    FUN_0042f960("TCPADDR", GetGadgetText(gadget, "ADDRESS", 0), 0x80);
}
