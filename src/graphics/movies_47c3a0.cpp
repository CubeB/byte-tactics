// Decompiled by space-bunny-free. Names are provisional.
#include <windows.h>
#include <ddraw.h>

// The open Smacker movie (smackw32.dll's SMK struct), and the offscreen
// display the frames are drawn to.
struct Smk_0047c3a0 {
    char unknown_0[4];
    unsigned int width;                 // +0x4
    unsigned int height;                // +0x8
    unsigned int frames;                // +0xc
    char unknown_10[0x68 - 0x10];
    unsigned int field_68;              // +0x68
    unsigned char rgb[256][3];          // +0x6c
    char unknown_36c[0x374 - 0x36c];
    int frameNum;                       // +0x374
    char unknown_378[0x380 - 0x378];
    int lastLeft;                       // +0x380
    int lastTop;                        // +0x384
    int lastWidth;                      // +0x388
    int lastHeight;                     // +0x38c
};

struct Display_0047c3a0 {
    char unknown_0[8];
    unsigned int width;                 // +0x8
    unsigned int height;                // +0xc
    IDirectDrawPalette* palette;        // +0x10
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1b];
    Display_0047c3a0* display;          // +0x37e1b
};
#pragma pack(pop)

extern Game* g_game;

// smackw32.dll, imported by ordinal: 19 SmackDoFrame, 21 SmackNextFrame,
// 23 SmackToBuffer, 28 SmackToBufferRect.
extern "C" __declspec(dllimport) void __stdcall SmackDoFrame(Smk_0047c3a0* smk);
extern "C" __declspec(dllimport) void __stdcall SmackNextFrame(Smk_0047c3a0* smk);
extern "C" __declspec(dllimport) void __stdcall SmackToBuffer(Smk_0047c3a0* smk, unsigned int left, unsigned int top, unsigned int width, unsigned int height, unsigned int bufferHeight, void* buffer);
extern "C" __declspec(dllimport) int __stdcall SmackToBufferRect(Smk_0047c3a0* smk, int flag);

void __stdcall SetOffscreenSurface(Display_0047c3a0* display);
void FlipScreen();

class Class_0047c3a0 {
public:
    Smk_0047c3a0* video;                // +0x00
    int counter;                        // +0x04
    int done;                           // +0x08
    HWND hwnd;                          // +0x0c
    PALETTEENTRY entries[256];          // +0x10
    char unknown_410[0x544 - 0x410];
    Display_0047c3a0* display;          // +0x544

    void PlayFrame(HWND hwnd);
};

// FUNCTION: 0x47c3a0
void Class_0047c3a0::PlayFrame(HWND hwnd)
{
    if (GetFocus() != hwnd)
        return;
    if (done)
        return;
    if (video->field_68) {
        unsigned char* src = video->rgb[0];
        for (int i = 0; i < 256; i++) {
            entries[i].peRed = *src++;
            entries[i].peGreen = *src++;
            entries[i].peBlue = *src++;
        }
        display->palette->SetEntries(0, 0, 256, entries);
    }
    SetOffscreenSurface(g_game->display);
    SmackToBuffer(video, 0, (0x1e0 - video->height) >> 1, g_game->display->width, video->height, g_game->display->height, 0);
    SmackDoFrame(video);
    FlipScreen();
    if (SmackToBufferRect(video, 1) && video->lastLeft == 0 && video->lastTop == 0
        && video->lastWidth == video->width && video->lastHeight == video->height) {
        counter = video->frameNum + 1;
    }
    if (video->frameNum == video->frames - 1) {
        done = 1;
        return;
    }
    SmackNextFrame(video);
}
