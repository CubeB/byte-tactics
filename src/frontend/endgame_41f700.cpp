// Decompiled by Opus. Names are provisional.
// Opens the CD check dialog (CDCHECK.GUI) with HandleCdCheckClick as its handler.

struct Sub_0041f700 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_0041f700 sub;                  // +0x519
};
#pragma pack(pop)

struct Gadget_0041f700 {
    char unknown_0[0x8];
    int (__stdcall* handler)(void*);   // +0x8
};

extern Game* g_game;

Gadget_0041f700* __stdcall LoadGuiLayer(Sub_0041f700* sub, const char* name, int flags);
void __stdcall SetCursorOverlayEnabled(int param);
void __stdcall FUN_0049fb10(Sub_0041f700* sub, int value);
void __stdcall RenderLayer(Sub_0041f700* sub, int value);
int __stdcall HandleCdCheckClick(void* gadget);

// FUNCTION: 0x41f700
void OpenCdCheckDialog()
{
    LoadGuiLayer(&g_game->sub, "CDCHECK.GUI", 0x101)->handler = HandleCdCheckClick;
    SetCursorOverlayEnabled(1);
    FUN_0049fb10(&g_game->sub, 1);
    RenderLayer(&g_game->sub, 0x40);
}
