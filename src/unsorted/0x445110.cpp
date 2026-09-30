// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Sets up the multiplayer "select team logo" dialog (LOGOSEL.GUI). It opens
// the dialog, allocates a small layout object holding the list of logo
// pointers and a copy of the logo sequence records, then walks the ten player
// slots: a slot takes a logo index j when its type byte is neither 0 nor 4
// and its player data's logo byte is at least j. The pointer list and the
// byte array are filled in from the slots that no player claimed.
//
// Two source details were needed: the inner scan is written as array indexing
// (g_game->players[k]), which makes MSVC strength-reduce it to a pointer over
// the type byte at +0x73; and the bookkeeping at the end of each iteration
// increments n before cursor, which flips the eax/ecx roles in that block.

#pragma pack(push, 1)
struct PlayerData_00445110 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
};

struct Player_00445110 {
    char unknown_0[0x27];
    PlayerData_00445110* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct LogoEntry_00445110 {
    void* ptr;                         // +0x00
    int unknown_4;                     // +0x04
};

struct Logos_00445110 {
    unsigned short count;              // +0x00
    char unknown_2[0x28 - 0x2];
    LogoEntry_00445110 entries[1];     // +0x28
};

struct AnimSeq_00445110 {
    char unknown_0[0x28];
    void* field_28;                    // +0x28
    char unknown_2c[0x30 - 0x2c];
};

struct Layout_00445110 {
    char selected[0x18];
    void** ptrList;                    // +0x18
    AnimSeq_00445110* seqs;            // +0x1c
};

struct Sub_00445110 {
    char unknown_0[0x10];
};

struct Entry_00445110 {
    char unknown_0[0x1b];
    int flags_1b;                      // +0x1b
    char unknown_1f[0xce - 0x1f];
    void (__stdcall* field_ce)(void*); // +0xce
};

struct Gui_00445110 {
    char unknown_0[4];
    void* entries;                     // +0x04
    void (__stdcall* handler)(Gui_00445110*);  // +0x08
    Layout_00445110* layout;           // +0x0c
};

struct Game_00445110 {
    char unknown_0[0x519];
    Sub_00445110 sub;                  // +0x519
    char unknown_529[0x1b63 - 0x529];
    Player_00445110 players[10];       // +0x1b63
    char unknown_2851[0x148d7 - 0x2851];
    void* logos;                       // +0x148d7
    Logos_00445110* logos32;           // +0x148db
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game_00445110* g_game;

Gui_00445110* __stdcall FUN_004aa8f0(Sub_00445110* sub, const char* name, int flags);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
Entry_00445110* __stdcall FUN_0049ff90(void* entries, const char* name);
int __stdcall FUN_0049fdf0(void* entries, const char* name, int flag);
void __stdcall FUN_004a36a0(Gui_00445110* gui, const char* name, void** items, int count);
void __stdcall FUN_0049fb10(Sub_00445110* sub, int value);
void __stdcall FUN_004a81e0(Sub_00445110* sub, int value);
void __stdcall FUN_00444930(Gui_00445110* gui);
void __stdcall FUN_00444910(void* gadget);

// FUNCTION: 0x445110
void FUN_00445110()
{
    Gui_00445110* gui = FUN_004aa8f0(&g_game->sub, "LOGOSEL.GUI", 0x800);
    gui->handler = FUN_00444930;
    Layout_00445110* layout = (Layout_00445110*)FUN_004d83b0("SELECT TEAM LOGO", 0x20);
    gui->layout = layout;
    int count = g_game->logos32->count;
    layout->ptrList = (void**)FUN_004d83b0("ANIMSEQ PTR LIST", count * 4);
    layout->seqs = (AnimSeq_00445110*)FUN_004d83b0("ACTUAL ANIMSEQS", count * 0x30);
    void** cursor = layout->ptrList;
    int n = 0;
    for (int j = 0; j < count; j++) {
        int k;
        for (k = 0; k < 10; k++) {
            if (g_game->players[k].type != 0 && g_game->players[k].type != 4
                && g_game->players[k].data->field_96 >= j)
                break;
        }
        layout->seqs[j] = *(AnimSeq_00445110*)g_game->logos32;
        layout->seqs[j].field_28 = g_game->logos32->entries[j].ptr;
        if (k == 10) {
            *cursor = &layout->seqs[j];
            ((char*)layout)[n] = (char)j;
            n++;
            cursor++;
        }
    }
    Entry_00445110* logo = FUN_0049ff90(gui->entries, "LOGOS");
    if (logo != 0) {
        logo->field_ce = FUN_00444910;
    }
    int index = FUN_0049fdf0(gui->entries, "LOGOS", 2);
    if (index != -1) {
        ((Entry_00445110*)((char*)gui->entries + index * 0x15b))->flags_1b |= 0x40;
    }
    FUN_004a36a0(gui, "LOGOS", layout->ptrList, n);
    FUN_0049fb10(&g_game->sub, 1);
    FUN_004a81e0(&g_game->sub, 0x40);
}
