// Decompiled by Opus. Names are provisional.
// Opens the map view dialog (VIEWMAP.GUI) with HandleViewMapClick as its handler.

struct Sub_00444be0 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_00444be0 sub;                  // +0x519
};
#pragma pack(pop)

struct Gadget_00444be0 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);  // +0x8
};

// GLOBAL: 0x511de8
extern Game* g_game;

Gadget_00444be0* __stdcall LoadGuiLayer(Sub_00444be0* sub, const char* name, int flags);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void ShowSelectedMapInfo();
void __stdcall FUN_0049fb10(Sub_00444be0* sub, int value);
void __stdcall RenderLayer(Sub_00444be0* sub, int value);
void __stdcall HandleViewMapClick(void* gadget);

// FUNCTION: 0x444be0
void OpenViewMapDialog()
{
    LoadGuiLayer(&g_game->sub, "VIEWMAP.GUI", 0x900)->handler = HandleViewMapClick;
    LoadPictureCached("DVIEWMAP", 0, 0, 0);
    ShowSelectedMapInfo();
    FUN_0049fb10(&g_game->sub, 1);
    RenderLayer(&g_game->sub, 0x40);
}
