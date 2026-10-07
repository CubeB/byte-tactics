// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Ends a multiplayer mission: loads the "Mission02WinBW" bitmap, frees the
// previous music, opens ENDMULTI.GUI with HandleEndMultiClick as its handler and puts
// "Victory" or "Failure" (bit 4 of the mission flags) into the RESULT field.

struct Sub_0044b020 {
    char unknown_0[0x10];
};

struct Gadget_0044b020 {
    char unknown_0[0x8];
    void (__stdcall* handler)(void*);  // +0x8
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_0044b020 sub;                  // +0x519
    char unknown_529[0x37e1b - 0x529];
    int field_37e1b;                   // +0x37e1b
    char unknown_37e1f[0x3923b - 0x37e1f];
    unsigned short bits0_3923b : 2;    // +0x3923b
    unsigned short flag2_3923b : 1;
    unsigned short bit3_3923b : 1;
    unsigned short flag4_3923b : 1;
    unsigned short rest_3923b : 11;
};
#pragma pack(pop)

extern Game* g_game;

void BlankScreen();
void* __stdcall LoadBitmapByName(char* name, unsigned char* palette);
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall SetOffscreenSurface(int param_1);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall FreeSurface(void* image);
void FlipScreen();
Gadget_0044b020* __stdcall LoadGuiLayer(Sub_0044b020* sub, const char* name, int flags);
void __stdcall HandleEndMultiClick(void* gadget);
int __stdcall Translate(const char* str);
void __stdcall FUN_004a0bf0(Sub_0044b020* sub, char* name, int param_3, int param_4);
void __stdcall RenderLayer(Sub_0044b020* sub, int value);
void ShowSoftwareCursor();

// FUNCTION: 0x44b020
void OpenEndMultiScreen()
{
    unsigned char palette[0x400];
    void* image;

    BlankScreen();
    image = LoadBitmapByName("Mission02WinBW", palette);
    SetPaletteColors(palette, 0, 0x100);
    SetOffscreenSurface(g_game->field_37e1b);
    DrawSurface(0, image, 0, 0);
    FreeSurface(image);
    FlipScreen();
    LoadGuiLayer(&g_game->sub, "ENDMULTI.GUI", 0x80)->handler = HandleEndMultiClick;
    FUN_004a0bf0(&g_game->sub, "RESULT",
                 Translate(g_game->flag4_3923b ? "Victory" : "Failure"), 0);
    RenderLayer(&g_game->sub, 0xc0);
    ShowSoftwareCursor();
}
