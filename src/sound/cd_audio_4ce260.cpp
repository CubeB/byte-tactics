// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>
#include <mmsystem.h>

extern HWND g_cdPlayerWindow;
extern void* g_cdPlayer;
extern int g_cdNextTrackTimer;
extern int g_cdFadeTimer;

extern void __stdcall RemoveTimer(int);
extern void __stdcall FUN_004b6b60(void (__stdcall*)(int, int, int));
extern void __stdcall HandleCdMessage(int, int, int);
extern BOOL __stdcall FindCdPlayerWindow(HWND, LPARAM);

class Class_004cda00 {
public:
    int QueryDisc();
};

class Class_004ce260 {
public:
    int open;                          // +0x0
    char unknown_4[0x1fc - 4];
    int field_1fc;                     // +0x1fc
    int field_200;                     // +0x200
    int field_204;                     // +0x204
    int field_208;                     // +0x208
    int field_20c;                     // +0x20c
    int field_210;                     // +0x210
    unsigned char arr_214[100];        // +0x214
    int field_278;                     // +0x278
    int field_27c;                     // +0x27c
    int field_280;                     // +0x280
    int field_284;                     // +0x284
    char unknown_288[4];
    int field_28c;                     // +0x28c

    int OpenCdAudio();
};

// Initialises the CD player object and opens the MCI cdaudio device. The
// mciSendStringA results go through the `hr` local: comparing the call
// directly emits `test eax, eax` instead of the original's `cmp eax, ebp`.
// The store to arr_214[0] before the loop is overwritten by the loop's first
// iteration (0 % 4 + 1 == 1), so it is redundant in the original.
// FUNCTION: 0x4ce260
int Class_004ce260::OpenCdAudio()
{
    MCIERROR hr;
    if (open != 0)
        return 1;
    g_cdPlayerWindow = 0;
    g_cdPlayer = this;
    arr_214[0] = 1;
    for (int i = 0; i < 100; i++)
        arr_214[i] = (i % 4) + 1;
    field_204 = 1;
    field_208 = 0;
    field_210 = 0;
    field_28c = 0;
    field_1fc = 1;
    field_278 = 0;
    open = 0;
    field_200 = 0;
    field_280 = 0;
    hr = mciSendStringA("open cdaudio", 0, 0, 0);
    if (hr != 0) {
        EnumWindows((WNDENUMPROC)FindCdPlayerWindow, 0);
        hr = mciSendStringA("open cdaudio", 0, 0, 0);
        if (hr != 0)
            return 0;
    }
    mciSendStringA("stop cdaudio", 0, 0, 0);
    field_20c = 0;
    field_208 = (field_200 != 0);
    field_284 = 0;
    RemoveTimer(g_cdNextTrackTimer);
    RemoveTimer(g_cdFadeTimer);
    g_cdFadeTimer = -1;
    g_cdNextTrackTimer = -1;
    hr = mciSendStringA("set cdaudio time format milliseconds", 0, 0, 0);
    if (hr != 0) {
        if (open != 0) {
            mciSendStringA("stop cdaudio", 0, 0, 0);
            mciSendStringA("close cdaudio", 0, 0, 0);
            open = 0;
        }
        return 0;
    }
    field_210 = 0;
    field_200 = ((Class_004cda00*)this)->QueryDisc();
    FUN_004b6b60(HandleCdMessage);
    open = 1;
    field_204 = 1;
    field_208 = 0;
    field_28c = 0;
    field_1fc = 1;
    field_278 = 0;
    field_27c = 1;
    return 1;
}
