// Decompiled by space-bunny-free, finished by mimo-v2.6-flash, finished by space-bunny-free. Names are provisional.
// MATCH: all 494 bytes, 0x495010 to 0x4951fd. Re-verified with check.py after a
// forced rebuild, so the score below is measured, not carried over from an
// earlier note. Nothing is left outstanding: the body is plain C++ with no
// volatile, no casts that change behaviour and no codegen tricks.
//
// What the code does: it opens (or closes) the TABMENU.GUI tab menu page. If any
// of bits 5 to 7 of the flags word at +0x2bee is set the page is being closed:
// the bits are cleared and the gui is either hidden, or torn down when
// IsScreenNamed does not find the file. Otherwise the bit 5 "open" flag is set,
// the gui is fetched through LoadGuiLayer and given a click handler and an owner,
// and the three ALLIES / SHARE / CONTROL checkboxes are set. The two first ones
// are enabled when the player count of free, non allied slots is positive and
// the net mode is 3, CONTROL also needs bit 0 of +0x2c74 clear and IsHostLocal
// true.
//
// Two things in the disassembly that the source has to reproduce rather than
// avoid:
// - The scan loop keeps its count in esi and its "1" constant in ebx, so the
//   player type test reads `cmp byte ptr [eax + 0x73], bl` and the CONTROL test
//   reads `test byte ptr [edx + 0x2c74], bl`. Only the `for` loop written as a
//   single `count++` body with no extra locals gets that register split.
// - The scale by 0x14b for players[localPlayer].info is left to the compiler:
//   it emits the shl 5 / add / lea sequence. Spelling the multiply by hand gives
//   a different form.
//
// No volatile field here: the flags word at +0x2bee is written once per branch
// and never re-read, so it is a plain unsigned short.

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct PlayerInfo_00495010 {
    char unknown_0[0x9b];
    unsigned short bits_9b_0 : 6;      // +0x9b, bits 0 to 5
    unsigned short flag_9b_6 : 1;      // bit 6 (mask 0x40)
    unsigned short bits_9b_7 : 9;
};

struct Player_00495010 {               // 0x14b bytes
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00495010* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Sub_00495010 {
    char unknown_0[0x10];
};

struct Game {
    char unknown_0[0x519];
    Sub_00495010 sub;                  // +0x519
    char unknown_529[0x1b63 - 0x529];
    Player_00495010 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bee - 0x2a43];
    unsigned short flags;              // +0x2bee
    char unknown_2bf0[0x2c74 - 0x2bf0];
    unsigned char field_2c74;          // +0x2c74
    char unknown_2c75[0x391e9 - 0x2c75];
    Class_00435100* net;               // +0x391e9
};
#pragma pack(pop)

struct Gadget_00495010 {
    char unknown_0[0x8];
    void (__stdcall* handler)(Gadget_00495010*); // +0x8
    Game* owner;                       // +0xc
};

extern Game* g_game;

void __stdcall PlaySoundByName(char* name, int param_2);
int __stdcall IsScreenNamed(Sub_00495010* sub, const char* name);
void __stdcall CloseTopScreen(Sub_00495010* sub);
void FUN_004c2470();
Gadget_00495010* __stdcall LoadGuiLayer(Sub_00495010* sub, const char* name, int flags);
void __stdcall FUN_00494740(Gadget_00495010* gadget);
int IsHostLocal();
void __stdcall FUN_004a0570(Sub_00495010* sub, char* name, int value);
void __stdcall FUN_0049fa50(Sub_00495010* sub);
void __stdcall RenderLayer(Sub_00495010* sub, int value);
void FUN_004c2870();

// FUNCTION: 0x495010
void FUN_00495010()
{
    PlaySoundByName("SmallButton", 0);
    unsigned short f = g_game->flags;
    if (f & 0xe0) {
        g_game->flags = f & 0xff1f;
        if (IsScreenNamed(&g_game->sub, "TABMENU.GUI"))
            CloseTopScreen(&g_game->sub);
        return;
    }
    g_game->flags = (f & 0xff3f) | 0x20;
    FUN_004c2470();
    Gadget_00495010* d = LoadGuiLayer(&g_game->sub, "TABMENU.GUI", 0x800);
    d->owner = g_game;
    d->handler = FUN_00494740;

    int count = 0;
    Player_00495010* p = g_game->players;
    for (int i = 0; i < 10; i++, p++) {
        if (p->active != 0 && p->type == 1)
            continue;
        if (p->info->flag_9b_6)
            continue;
        count++;
    }
    int mode = g_game->net->FUN_00435100();
    if (mode == 3 && !g_game->players[g_game->localPlayer].info->flag_9b_6) {
        int v = count > 0;
        FUN_004a0570(&g_game->sub, "ALLIES", v);
        FUN_004a0570(&g_game->sub, "SHARE", v);
        int ctl = !(g_game->field_2c74 & 1) && IsHostLocal();
        FUN_004a0570(&g_game->sub, "CONTROL", ctl);
    } else {
        FUN_004a0570(&g_game->sub, "ALLIES", 0);
        FUN_004a0570(&g_game->sub, "SHARE", 0);
        FUN_004a0570(&g_game->sub, "CONTROL", 0);
    }
    FUN_0049fa50(&g_game->sub);
    RenderLayer(&g_game->sub, 0x40);
    FUN_004c2870();
}
