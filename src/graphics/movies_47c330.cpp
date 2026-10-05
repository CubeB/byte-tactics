// Decompiled by Opus. Names are provisional.
// WM_PAINT handler of the Smacker video player: clears the video area to
// black, then, when a full frame has been seen, seeks back to it with the
// sound muted so the picture is redrawn.
#include <windows.h>

struct Smack_0047c330 {
    unsigned int Version;              // +0x0
    unsigned int Width;                // +0x4
    unsigned int Height;               // +0x8
};

// smackw32.dll ordinals 17 (SmackSoundOnOff) and 27 (SmackGoto), called
// through their import slots. Ordinal 17 is called as (smk, directSound != 0)
// right after SmackOpen (ordinal 14) at 0x47be53; ordinal 27 gets the frame
// number recorded after a full-frame SmackToBufferRect (ordinal 28).
extern "C" __declspec(dllimport) unsigned int __stdcall SmackSoundOnOff(Smack_0047c330* smk, unsigned int on);
extern "C" __declspec(dllimport) void __stdcall SmackGoto(Smack_0047c330* smk, unsigned int frame);

class MoviePlayer {
public:
    Smack_0047c330* smack;             // +0x0
    unsigned int frame;                // +0x4

    void OnPaint(HWND hwnd);
};

// FUNCTION: 0x47c330
void MoviePlayer::OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(hwnd, &ps);
    PatBlt(dc, 0, 0, smack->Width, smack->Height, BLACKNESS);
    EndPaint(hwnd, &ps);
    if (frame) {
        SmackSoundOnOff(smack, 0);
        SmackGoto(smack, frame);
        SmackSoundOnOff(smack, 1);
    }
}
