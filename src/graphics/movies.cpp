// Decompiled by space-bunny-free, deepseek-v4.1-flash, Opus, Sonnet, GPT-6-Luna, Space Bunny Free and Haiku. Names are provisional.
// The Smacker movie player.
#include <stdio.h>
#include <windows.h>
#include <ddraw.h>

// The open Smacker movie (smackw32.dll's SMK struct).
struct Smk_0047c3a0 {
    unsigned int version;               // +0x0
    unsigned int width;                 // +0x4
    unsigned int height;                // +0x8
    unsigned int frames;                // +0xc
    char unknown_10[0x68 - 0x10];
    unsigned int field_68;              // +0x68, the palette changed
    unsigned char rgb[256][3];          // +0x6c
    char unknown_36c[0x374 - 0x36c];
    int frameNum;                       // +0x374
    char unknown_378[0x380 - 0x378];
    int lastLeft;                       // +0x380
    int lastTop;                        // +0x384
    int lastWidth;                      // +0x388
    int lastHeight;                     // +0x38c
};

// smackw32.dll, imported by ordinal, so the names are ours: 14 SmackOpen,
// 15 SmackSoundEnable, 17 SmackSoundOnOff, 19 SmackDoFrame, 21
// SmackNextFrame, 23 SmackToBuffer, 27 SmackGoto, 28 SmackToBufferRect,
// 38 SmackSoundUseDirectSound.
extern "C" __declspec(dllimport) int __stdcall SmackSoundEnable(HWND hwnd);
extern "C" __declspec(dllimport) int __stdcall SmackSoundUseDirectSound(void* sound);
extern "C" __declspec(dllimport) Smk_0047c3a0* __stdcall SmackOpen(char* path, unsigned int flags, int extra);
extern "C" __declspec(dllimport) unsigned int __stdcall SmackSoundOnOff(Smk_0047c3a0* smk, unsigned int on);
extern "C" __declspec(dllimport) void __stdcall SmackGoto(Smk_0047c3a0* smk, unsigned int frame);
extern "C" __declspec(dllimport) void __stdcall SmackDoFrame(Smk_0047c3a0* smk);
extern "C" __declspec(dllimport) void __stdcall SmackNextFrame(Smk_0047c3a0* smk);
extern "C" __declspec(dllimport) void __stdcall SmackToBuffer(Smk_0047c3a0* smk, unsigned int left, unsigned int top, unsigned int width, unsigned int height, unsigned int bufferHeight, void* buffer);
extern "C" __declspec(dllimport) int __stdcall SmackToBufferRect(Smk_0047c3a0* smk, int flag);
// Ordinal 32: SmackWait, the DLL's export name.
extern "C" __declspec(dllimport) unsigned int __stdcall SmackWait(Smk_0047c3a0* smack);

// smackw32.dll ordinal 18 (SmackClose), called through its import slot.
extern "C" __declspec(dllimport) void __stdcall SmackClose(void* smack);

// The 0x54 byte statistics block SmackSummary fills in. Six of the twenty-one
// fields are never printed, and the seven printed before TotalBlitTime are
// read from further in than the DLL writes.
struct SmkStats_0047c530 {
    unsigned int totalTime;            // +0x00 divisor of the frame rate
    unsigned int unknown_04;           // +0x04
    unsigned int openTime;             // +0x08 "Time to Open File"
    unsigned int frames;               // +0x0c "Total Frames Played"
    unsigned int framesSkipped;        // +0x10 "SkippedFrames"
    unsigned int unknown_14;           // +0x14
    unsigned int timeBlit;             // +0x18 "TotalBlitTime"
    unsigned int readTime;             // +0x1c "TotalReadTime"
    unsigned int decompTime;           // +0x20 "TotalDecompTime"
    unsigned int unknown_24;           // +0x24
    unsigned int readSpeed;            // +0x28 "TotalReadSpeed"
    unsigned int slowestFrameTime;     // +0x2c "SlowestFrameTime"
    unsigned int slowest2FrameTime;    // +0x30 "Slowest2FrameTime"
    unsigned int unknown_34;           // +0x34
    unsigned int unknown_38;           // +0x38
    unsigned int averageFrameSize;     // +0x3c "AverageFrameSize"
    unsigned int highest1SecRate;      // +0x40 "Highest1SecRate"
    unsigned int unknown_44;           // +0x44
    unsigned int highestMemAmount;     // +0x48 "HighestMemAmount"
    unsigned int totalExtraMemory;     // +0x4c "TotalExtraMemory"
    unsigned int highestExtraUsed;     // +0x50 "HighestExtraUsed"
};

// smackw32.dll ordinal 20, called through its import slot.
extern "C" __declspec(dllimport) void __stdcall SmackSummary(Smk_0047c3a0* smk, SmkStats_0047c530* stats);

struct Surfaces_0047bdf0 {
    IDirectDraw* ddraw;              // +0x0
    IDirectDrawSurface* primary;     // +0x4
    IDirectDrawSurface* back;        // +0x8
    IDirectDrawSurface* field_c;     // +0xc
    IDirectDrawPalette* palette;     // +0x10
};

struct Display_0047bdf0 {
    char unknown_0[0x24];
    void* sound;                     // +0x24
    char unknown_28[0x40 - 0x28];
    HWND hwnd;                       // +0x40
    char unknown_44[0x84 - 0x44];
    Surfaces_0047bdf0 surfaces;      // +0x84
    char unknown_98[0xf0 - 0x98];
    unsigned short flags;            // +0xf0
};

// The offscreen screen the frames are drawn to.
struct Display_0047c3a0 {
    char unknown_0[8];
    unsigned int width;                 // +0x8
    unsigned int height;                // +0xc
    IDirectDrawPalette* palette;        // +0x10
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    Display_0047bdf0* display;          // +0x10
    char unknown_14[0x37e1b - 0x14];
    Display_0047c3a0* screen;           // +0x37e1b
    char unknown_37e1f[0x39241 - 0x37e1f];
    int introMovieLoop;                 // +0x39241
};
#pragma pack(pop)

extern Game* g_game;                    // 0x511de8

Display_0047bdf0* GetDisplay();
void __stdcall FatalError(char* message);
void __stdcall SetOffscreenSurface(Display_0047c3a0* display);
void FlipScreen();

// The DirectDraw wrapper the player holds: the interface, the primary surface
// and the palette built from it, plus the mode it was set to.
class DDraw_0047bf70 {
public:
    IDirectDraw *lpDD;                 // +0x00
    IDirectDrawSurface *lpSurface;     // +0x04
    int width;                         // +0x08
    int height;                        // +0x0c
    IDirectDrawPalette *lpPalette;     // +0x10
};

// The game's DDSURFACEDESC is 0x6c bytes, so it is not the DirectDraw 1 layout
// in <ddraw.h> here; only the fields this function writes are named.
struct SurfaceDesc_0047bf70 {
    DWORD dwSize;                      // +0x00
    DWORD dwFlags;                     // +0x04
    DWORD dwHeight;                    // +0x08
    DWORD dwWidth;                     // +0x0c
    DWORD dwPitch;                     // +0x10
    char unknown_14[0x68 - 0x14];
    DWORD dwCaps;                      // +0x68, DDSCAPS_PRIMARYSURFACE
};

// The open Smacker movie (smackw32.dll's SMK struct).
struct Smk_0047bf70 {
    DWORD unknown_0;                   // +0x00
    DWORD width;                       // +0x04
    DWORD height;                      // +0x08
    char unknown_c[0x68 - 0xc];
    DWORD newPalette;                  // +0x68
    unsigned char rgb[256][3];         // +0x6c
    DWORD unknown_36c;                 // +0x36c
    unsigned short field_370;          // +0x370
};

// The object smackw32.dll ordinal 2 hands back; its fields are not named
// because no other call site reads them.
struct Smk_0047bf70_b {
    char unknown_0[0x2c];
    DWORD field_2c;                    // +0x2c
    char unknown_30[0x3c - 0x30];
    DWORD field_3c;                    // +0x3c
    char unknown_40[0x43c - 0x40];
    DWORD field_43c;                   // +0x43c
};

extern int __stdcall DirectDrawCreateThunk(int guid, void *display, int zero);

// smackw32.dll's buffer API, imported by ordinal: ordinal 2 opens the decoder
// state (HWND, HDC, 640, 480, 0, 0), ordinal 5 takes the movie's 256 colour
// palette, and ordinal 25 is given the movie and three fields of that state.
extern "C" __declspec(dllimport) Smk_0047bf70_b *__stdcall SmackBufferOpen(HWND hwnd, HDC hdc, int width, int height, int flags, int background);
extern "C" __declspec(dllimport) void __stdcall SmackBufferNewPalette(Smk_0047bf70_b *smk, unsigned char *rgb, unsigned short flag);
extern "C" __declspec(dllimport) void __stdcall SmackColorRemap(Smk_0047bf70 *smk, DWORD *field_3c, DWORD field_2c, DWORD field_43c);

class MoviePlayer {
public:
    Smk_0047c3a0* smack;               // +0x0
    unsigned int frame;                // +0x4, the last full frame seen
    int field_8;                       // +0x8, the movie is done or stopped
    HWND hwnd;                         // +0xc
    PALETTEENTRY entries[256];         // +0x10
    char unknown_410[4];
    int hasSurfaces;                   // +0x414
    char unknown_418[0x528 - 0x418];
    Surfaces_0047bdf0 surfaces;        // +0x528
    char unknown_53c[0x544 - 0x53c];
    Surfaces_0047bdf0* wrapper;        // +0x544

    MoviePlayer(char* path, int a, int b, int c, int d, int e);
    void ReadSystemPalette(int unused);
    void UpdatePalette(void);
    void ClearScreen();
    void OnPaint(HWND hwnd);
    void PlayFrame(HWND hwnd);
    void WriteSmackStats();
    void Play();
    void Close();
};

class Class_0047bf70 {
public:
    Smk_0047bf70 *video;               // +0x000
    int counter;                       // +0x004
    int stopped;                       // +0x008
    HWND hwnd;                         // +0x00c
    PALETTEENTRY entries[256];         // +0x010
    int paletteResult;                 // +0x410
    char unknown_414[0x544 - 0x414];
    DDraw_0047bf70 *ddraw;             // +0x544
    SurfaceDesc_0047bf70 surfaceDesc;  // +0x548

    int SetupDirectDraw();
};

typedef void (__stdcall *GetPixelFormatFn)(void* self, DDPIXELFORMAT* format);

struct SmackerSurface {
    GetPixelFormatFn* methods;      // +0x00, slot 21 is the pixel format query
};

struct SmackerSurfaces {
    char unknown_0[4];
    SmackerSurface* surface;        // +0x04
};

// The same object seen as the class the pixel format query belongs to
// (0x47c150): it is a member of Class_0047bf70 in the original.
class Class_0047c150 {
public:
    char unknown_0[0x544];
    SmackerSurfaces* surfaces;      // +0x544

    int GetBlitMode(SmackerSurface* unused);
};

extern int g_lastPlaceMetalSum;

extern int g_lastPlaceHeight;

// The constructor: opens the .smk file, hands the Smack library the game's
// DirectSound object, then paints a black frame into the surface the player
// blits through. That surface is the display's own DirectDraw surface when the
// game has one, otherwise a private one made by SetupDirectDraw.
// FUNCTION: 0x47bdf0
MoviePlayer::MoviePlayer(char* path, int a, int b, int c, int d, int e)
{
    field_8 = 0;
    frame = 0;
    if (a)
        SmackSoundEnable((HWND)b);
    void* sound = g_game->display->sound;
    SmackSoundUseDirectSound(sound);
    smack = SmackOpen(path, 0xfe100, -1);
    if (!smack)
        FatalError("Could not open movie file, please check filename in INI.");
    SmackSoundOnOff(smack, sound != 0);

    Display_0047bdf0* disp = GetDisplay();
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
        if (!((Class_0047bf70*)this)->SetupDirectDraw())
            FatalError("Could not setup Direct Draw to play movie.");
        hasSurfaces = 1;
    }

    DDBLTFX fx;
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0;
    wrapper->primary->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
}

// FUNCTION: 0x47bf20
void MoviePlayer::Close()
{
    SmackClose(smack);
    if (hasSurfaces) {
        if (wrapper->palette)
            wrapper->palette->Release();
        if (wrapper->primary)
            wrapper->primary->Release();
        if (wrapper->ddraw)
            wrapper->ddraw->Release();
    }
}

// Sets the movie player up: asks DirectDraw for full screen exclusive on the
// player window, builds the primary surface and a palette out of the system
// palette, sizes the window to the movie, and when the movie has a new palette
// hands it to the Smacker decoder. Returns 1 unless DirectDraw refuses.
// FUNCTION: 0x47bf70
int Class_0047bf70::SetupDirectDraw()
{
    POINT point;
    int i;
    HDC hdc;

    point.x = 0;
    point.y = 0;
    ClientToScreen(hwnd, &point);
    if (DirectDrawCreateThunk(0, ddraw, 0) != 0)
        goto fail;
    if (ddraw->lpDD->SetCooperativeLevel(hwnd, 8) != 0) {
        ddraw->lpDD->Release();
        goto fail;
    }
    memset(&surfaceDesc, 0, sizeof(surfaceDesc));
    surfaceDesc.dwSize = sizeof(surfaceDesc);
    surfaceDesc.dwFlags = 1;
    surfaceDesc.dwCaps = 0x200;
    if (ddraw->lpDD->CreateSurface((LPDDSURFACEDESC)&surfaceDesc, &ddraw->lpSurface, 0) == 0) {
        paletteResult = ((Class_0047c150 *)this)->GetBlitMode((SmackerSurface *)ddraw->lpSurface);
        if (paletteResult == 0) {
            hdc = GetDC(hwnd);
            GetSystemPaletteEntries(hdc, 0, 0x100, entries);
            for (i = 0; i < 10; ++i) entries[i].peFlags = 0;
            for (i = 10; i < 246; ++i) entries[i].peFlags = 4;
            for (i = 246; i < 256; ++i) entries[i].peFlags = 0;
            ReleaseDC(hwnd, hdc);
            if (ddraw->lpDD->CreatePalette(4, entries, &ddraw->lpPalette, 0) == 0)
                ddraw->lpSurface->SetPalette(ddraw->lpPalette);
        }
        SetWindowPos(hwnd, 0, 0, 0, video->width, video->height, 2);
    }
    if (video->newPalette) {
        Smk_0047bf70_b *smk = SmackBufferOpen(hwnd, 0, 0x280, 0x1e0, 0, 0);
        SmackBufferNewPalette(smk, &video->rgb[0][0], video->field_370);
        SmackColorRemap(video, &smk->field_3c, smk->field_2c, smk->field_43c);
    }
    return 1;
fail:
    return 0;
}

// Asks the movie's output surface for its pixel format and returns the blit
// mode matching it, or 0 when the format is not one of the four 16-bit RGB
// layouts Smacker can blit to.
// FUNCTION: 0x47c150
int Class_0047c150::GetBlitMode(SmackerSurface* unused)
{
    DDPIXELFORMAT format;
    format.dwSize = sizeof(format);
    format.dwFlags = DDPF_RGB;
    SmackerSurface* surface = surfaces->surface;
    surface->methods[21](surface, &format);
    if (format.dwRGBBitCount == 8)
        return 0;
    if (format.dwRBitMask == 0xf800 && format.dwGBitMask == 0x7e0 && format.dwBBitMask == 0x1f)
        return 0xc0000000;
    if (format.dwRBitMask == 0x7c00 && format.dwGBitMask == 0x3e0 && format.dwBBitMask == 0x1f)
        return 0x80000000;
    if (format.dwRBitMask == 0xf800 && format.dwGBitMask == 0x7c0 && format.dwBBitMask == 0x3f)
        return 0xa0000000;
    if (format.dwRBitMask == 0xfc00 && format.dwGBitMask == 0x3e0 && format.dwBBitMask == 0x1f)
        return 0xe0000000;
    MessageBoxA(0, "Unsupported pixel format.", "Smacker Error", 0);
    return 0;
}

// Reads the system palette into entries[], marking the 20 static colours
// (10 at each end) as plain and the 236 in between as PC_NOCOLLAPSE.
// FUNCTION: 0x47c230
void MoviePlayer::ReadSystemPalette(int unused)
{
    HDC hdc = GetDC(hwnd);
    GetSystemPaletteEntries(hdc, 0, 256, entries);
    int i;
    for (i = 0; i < 10; i++)
        entries[i].peFlags = 0;
    for (i = 10; i < 246; i++)
        entries[i].peFlags = PC_NOCOLLAPSE;
    for (i = 246; i < 256; i++)
        entries[i].peFlags = 0;
    ReleaseDC(hwnd, hdc);
}

// Copies the movie's palette into entries[] and the display palette.
// FUNCTION: 0x47c2a0
void MoviePlayer::UpdatePalette(void)
{
    unsigned char* src = smack->rgb[0];
    for (int i = 0; i < 256; i++) {
        entries[i].peRed = *src++;
        entries[i].peGreen = *src++;
        entries[i].peBlue = *src++;
    }
    wrapper->palette->SetEntries(0, 0, 256, entries);
}

// FUNCTION: 0x47c2f0
void MoviePlayer::ClearScreen()
{
    DDBLTFX fx;
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0;
    wrapper->primary->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
}

// The WM_PAINT handler: clears the video area to black, then, when a full
// frame has been seen, seeks back to it with the sound muted so the picture
// is redrawn.
// FUNCTION: 0x47c330
void MoviePlayer::OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(hwnd, &ps);
    PatBlt(dc, 0, 0, smack->width, smack->height, BLACKNESS);
    EndPaint(hwnd, &ps);
    if (frame) {
        SmackSoundOnOff(smack, 0);
        SmackGoto(smack, frame);
        SmackSoundOnOff(smack, 1);
    }
}

// FUNCTION: 0x47c3a0
void MoviePlayer::PlayFrame(HWND hwnd)
{
    if (GetFocus() != hwnd)
        return;
    if (field_8)
        return;
    if (smack->field_68) {
        unsigned char* src = smack->rgb[0];
        for (int i = 0; i < 256; i++) {
            entries[i].peRed = *src++;
            entries[i].peGreen = *src++;
            entries[i].peBlue = *src++;
        }
        wrapper->palette->SetEntries(0, 0, 256, entries);
    }
    SetOffscreenSurface(g_game->screen);
    SmackToBuffer(smack, 0, (0x1e0 - smack->height) >> 1, g_game->screen->width, smack->height, g_game->screen->height, 0);
    SmackDoFrame(smack);
    FlipScreen();
    if (SmackToBufferRect(smack, 1) && smack->lastLeft == 0 && smack->lastTop == 0
        && smack->lastWidth == smack->width && smack->lastHeight == smack->height) {
        frame = smack->frameNum + 1;
    }
    if (smack->frameNum == smack->frames - 1) {
        field_8 = 1;
        return;
    }
    SmackNextFrame(smack);
}

// Dumps the playback statistics smackw32.dll collected for the open movie into
// stats.txt, one per line. The frame rate and the playback time are worked out
// in integer arithmetic from the millisecond totals the DLL recorded, and the
// playback time comes out as 1000 for every movie, the original's arithmetic.
// FUNCTION: 0x47c530
void MoviePlayer::WriteSmackStats()
{
    SmkStats_0047c530 stats;
    SmackSummary(smack, &stats);
    FILE* file = fopen("stats.txt", "w");
    if (file) {
        fprintf(file, "Frames Per Sec\t%d\n", 1000 * stats.frames / stats.totalTime);
        fprintf(file, "Total Playback Time\t%d\n", 1000 * stats.totalTime / stats.totalTime);
        fprintf(file, "Time to Open File\t%d\n", stats.openTime);
        fprintf(file, "Total Frames Played\t%d\n", stats.frames);
        fprintf(file, "SkippedFrames\t%d\n", stats.framesSkipped);
        fprintf(file, "TotalBlitTime\t%d\n", stats.timeBlit);
        fprintf(file, "TotalReadTime\t%d\n", stats.readTime);
        fprintf(file, "TotalDecompTime\t%d\n", stats.decompTime);
        fprintf(file, "TotalReadSpeed\t%d bytes/sec\n", stats.readSpeed);
        fprintf(file, "SlowestFrameTime\t%d\n", stats.slowestFrameTime);
        fprintf(file, "Slowest2FrameTime\t%d\n", stats.slowest2FrameTime);
        fprintf(file, "AverageFrameSize\t%d\n", stats.averageFrameSize);
        fprintf(file, "Highest1SecRate\t%d\n", stats.highest1SecRate);
        fprintf(file, "HighestMemAmount\t%d\n", stats.highestMemAmount);
        fprintf(file, "TotalExtraMemory\t%d\n", stats.totalExtraMemory);
        fprintf(file, "HighestExtraUsed\t%d\n", stats.highestExtraUsed);
        fclose(file);
    }
}

// Pumps Windows messages while the Smacker movie plays: every message is
// translated and dispatched, and two message codes end the movie, one of them
// with a mouse move that also quits the game. When the queue is empty the
// player is asked for the next frame, and if there is none yet the frame on
// screen is redrawn (PlayFrame).
// FUNCTION: 0x47c6c0
void MoviePlayer::Play()
{
    MSG msg;
    while (!field_8) {
        if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            // 0x104 and 0x102 are the movie's own stop codes, not the Win95
            // WM_MOUSEMOVE (0x200) and WM_LBUTTONDOWN (0x201).
            if (msg.message == 0x104 && msg.wParam == 0x73) {
                field_8 = 1;
                g_game->introMovieLoop = 0;
                PostQuitMessage(0);
                return;
            }
            if (msg.message == 0x102) {
                field_8 = 1;
                g_game->introMovieLoop = 0;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        } else if (!SmackWait(smack)) {
            PlayFrame(hwnd);
        }
    }
}

// FUNCTION: 0x47c770
int GetBuildSiteMetal(void)
{
    return g_lastPlaceMetalSum;
}

// FUNCTION: 0x47c780
int GetBuildSiteHeight(void)
{
    return g_lastPlaceHeight;
}
