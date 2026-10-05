// Decompiled by space-bunny-free. Names are provisional.
// Opens the briefing dialog (BRIEFING.GUI) with FUN_0045f770 as its handler,
// then clears bit 4 of a flag in the MOREBAR and in the TextRegion gadget.
// The two writes are addressed as gadgets[i] + i * 0x15a, so the gadget index
// is added twice: the original does the same (see below), kept as it is.

struct Sub_0045f800 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game_0045f800 {
    char unknown_0[0x519];
    Sub_0045f800 sub;                  // +0x519
};

struct Entry_0045f800 {
    char unknown_0[0x1b];
    int flags;                         // +0x1b
    char unknown_1f[0x15a - 0x1f];
};
#pragma pack(pop)

struct Info_0045f800 {
    char unknown_0[0x4];
    Entry_0045f800* info;              // +0x4
    int (__stdcall* handler)(void*);   // +0x8
};

extern Game_0045f800* g_game;

Info_0045f800* __stdcall FUN_004aa8f0(Sub_0045f800* sub, const char* name, int flags);
int __stdcall FUN_0049fdf0(Entry_0045f800* info, const char* name, int type);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall FUN_004afc60(Sub_0045f800* sub, int value);
void __stdcall FUN_00476d80();
void __stdcall FUN_004a81e0(Sub_0045f800* sub, int value);
int __stdcall FUN_0045f770(void* gadget);

// FUNCTION: 0x45f800
void FUN_0045f800()
{
    Info_0045f800* g = FUN_004aa8f0(&g_game->sub, "BRIEFING.GUI", 0);
    Entry_0045f800* gadgets = g->info;
    g->handler = FUN_0045f770;
    int i = FUN_0049fdf0(gadgets, "MOREBAR", 0xe);
    // Suspected original bug: the entry is reached as gadgets + i + i * 0x15a
    // instead of gadgets + i * 0x15a, so this clears the flag of a different
    // gadget (or walks off the array) than the one just looked up.
    ((Entry_0045f800*)((char*)gadgets + i))[i].flags &= ~0x10;
    i = FUN_0049fdf0(gadgets, "TextRegion", 0xe);
    ((Entry_0045f800*)((char*)gadgets + i))[i].flags &= ~0x10;
    FUN_004288d0("igmbrief", 0, 0, 0);
    FUN_004afc60(&g_game->sub, 0xf);
    FUN_00476d80();
    FUN_004a81e0(&g_game->sub, 0x40);
}
