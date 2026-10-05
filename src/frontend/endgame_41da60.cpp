// Decompiled by Claude Opus 5.5. Names are provisional.
// Sets up the end-of-mission screen: allocates the fade table and palette
// buffers, forces the display's +0x614 value to 1.0 (saving the old one for
// HandleEndMissionClick to restore), then either loads the campaign's "glamour"
// picture (falling back to glamour\Arm01.PCX) or opens the Outcome1 or
// Outcome0 screen.
#include <string.h>

class Class_004356c0 {
public:
    char* FUN_004356c0(int index);
};

class Class_00435100 {
public:
    int FUN_00435100();
};

struct Display_0041da60 {
    char unknown_0[0x614];
    float field_614;                   // +0x614
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x3906f];
    float field_3906f;                 // +0x3906f
    char unknown_39073[0x3907b - 0x39073];
    void* image_3907b;                 // +0x3907b
    void* palette_3907f;               // +0x3907f
    void* currentPalette;              // +0x39083
    void* desiredPalette;              // +0x39087
    void* fadeTable;                   // +0x3908b
    char glamour[0x391e9 - 0x3908f];   // +0x3908f
    Class_004356c0* campaign;          // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned char flags_3923b;         // +0x3923b
};
#pragma pack(pop)

extern Game* g_game;

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
Display_0041da60* GetDisplay();
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
void* __stdcall LoadBitmapByName(char* name, unsigned char* palette);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);

// FUNCTION: 0x41da60
void SetUpEndMissionScreen()
{
    char path[256];
    g_game->fadeTable = FUN_004d83b0("FadeTable", 0x400);
    g_game->desiredPalette = FUN_004d83b0("desiredPalette", 0x400);
    g_game->currentPalette = FUN_004d83b0("currentPalette", 0x400);
    Display_0041da60* display = GetDisplay();
    g_game->field_3906f = display->field_614;
    display->field_614 = 1.0f;
    unsigned char* palette = (unsigned char*)FUN_004d83b0("Palette", 0x400);
    char* name = g_game->campaign->FUN_004356c0(5);
    if (name == 0)
        g_game->image_3907b = 0;
    if (((Class_00435100*)g_game->campaign)->FUN_00435100() == 1
        && (g_game->flags_3923b & 0x10) && name != 0) {
        BuildDataPath(path, "bitmaps\\glamour", name + 1, "PCX");
        if (HAPI_FileLengthByName(path) == 0)
            strncpy(g_game->glamour, "glamour\\Arm01.PCX", 0x100);
        else
            strncpy(g_game->glamour, path + 8, 0x100);
        g_game->image_3907b = LoadBitmapByName(g_game->glamour, palette);
        g_game->palette_3907f = palette;
        return;
    }
    if (((Class_00435100*)g_game->campaign)->FUN_00435100() == 1)
        LoadPictureCached("Outcome1", 0, 0, 1);
    else
        LoadPictureCached("Outcome0", 0, 0, 1);
}
