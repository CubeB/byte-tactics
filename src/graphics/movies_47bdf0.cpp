// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Smacker movie player constructor: opens the .smk file, hands the Smack
// library the game's DirectSound object, then paints a black frame into the
// surface the player blits through. That surface is the display's own
// DirectDraw surface when the game has one, otherwise a private one made by
// FUN_0047bf70.
#include <windows.h>
#include <ddraw.h>

// smackw32.dll ordinals: 15 SmackSoundEnable, 38 SmackSoundUseDirectSound,
// 14 SmackOpen, 17 SmackSoundOnOff. The exe imports them by ordinal, so the
// names are ours.
extern "C" __declspec(dllimport) int __stdcall SmackSoundEnable(HWND hwnd);
extern "C" __declspec(dllimport) int __stdcall SmackSoundUseDirectSound(void* sound);
extern "C" __declspec(dllimport) void* __stdcall SmackOpen(char* path, unsigned int flags, int extra);
extern "C" __declspec(dllimport) int __stdcall SmackSoundOnOff(void* smack, int on);

struct Surfaces_0047bdf0 {
    IDirectDraw* ddraw;              // +0x0
    IDirectDrawSurface* primary;     // +0x4
    IDirectDrawSurface* back;        // +0x8
    IDirectDrawSurface* field_c;     // +0xc
    IDirectDrawPalette* palette;     // +0x10
};

struct Display_0047bdf0 {
    char unknown_0[0x24];
    void* sound;             // +0x24
    char unknown_28[0x40 - 0x28];
    HWND hwnd;                       // +0x40
    char unknown_44[0x84 - 0x44];
    Surfaces_0047bdf0 surfaces;      // +0x84
    char unknown_98[0xf0 - 0x98];
    unsigned short flags;            // +0xf0
};

struct Game {
    char unknown_0[0x10];
    Display_0047bdf0* display;       // +0x10
};

extern Game* g_game;                 // 0x511de8

Display_0047bdf0* FUN_004b6220();
void __stdcall FUN_004b6290(char* message);

class Class_0047bf70 {
public:
    int FUN_0047bf70();
};

class Class_0047bdf0 {
public:
    void* smack;                     // +0x0
    int frame;                       // +0x4
    int field_8;                     // +0x8
    HWND hwnd;                       // +0xc
    char unknown_10[0x414 - 0x10];
    int hasSurfaces;                 // +0x414
    char unknown_418[0x528 - 0x418];
    Surfaces_0047bdf0 surfaces;      // +0x528
    char unknown_53c[0x544 - 0x53c];
    Surfaces_0047bdf0* wrapper;      // +0x544

    Class_0047bdf0(char* path, int a, int b, int c, int d, int e);
};

// FUNCTION: 0x47bdf0
Class_0047bdf0::Class_0047bdf0(char* path, int a, int b, int c, int d, int e)
{
    field_8 = 0;
    frame = 0;
    if (a)
        SmackSoundEnable((HWND)b);
    void* sound = g_game->display->sound;
    SmackSoundUseDirectSound(sound);
    smack = SmackOpen(path, 0xfe100, -1);
    if (!smack)
        FUN_004b6290("Could not open movie file, please check filename in INI.");
    SmackSoundOnOff(smack, sound != 0);

    Display_0047bdf0* disp = FUN_004b6220();
    hwnd = disp->hwnd;
    if ((disp->flags & 2) && disp->surfaces.ddraw) {
        wrapper = &disp->surfaces;
        hasSurfaces = 0;
    } else if (!(disp->flags & 2) && !disp->surfaces.ddraw) {
        wrapper = &surfaces;
        wrapper->ddraw = 0;
        wrapper->primary = 0;
        wrapper->back = 0;
        wrapper->palette = 0;
        wrapper->field_c = 0;
        if (!((Class_0047bf70*)this)->FUN_0047bf70())
            FUN_004b6290("Could not setup Direct Draw to play movie.");
        hasSurfaces = 1;
    }

    DDBLTFX fx;
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0;
    wrapper->primary->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
}
