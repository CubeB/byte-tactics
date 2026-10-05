// Decompiled by Space Bunny Free. Names are provisional.
// Pumps Windows messages while the Smacker movie plays: every message is
// translated and dispatched, and two message codes end the movie, one of them
// with a mouse move that also quits the game. When the queue is empty the
// player is asked for the next frame, and if there is none yet the frame on
// screen is redrawn (FUN_0047c3a0).
#include <windows.h>

// smackw32.dll, imported by ordinal (32) and by no name, so it has no
// symbol of its own here.
extern "C" __declspec(dllimport) unsigned int __stdcall DAT_004fc40c(void* smack);

#pragma pack(push, 1)
struct Game_0047c6c0 {
    char unknown_0[0x39241];
    int field_39241;                    // +0x39241
};
#pragma pack(pop)

extern Game_0047c6c0* g_game;

class Class_0047c3a0 {
public:
    void* smack;                        // +0x00
    int unknown_4;                      // +0x04
    int stopped;                        // +0x08
    HWND hwnd;                          // +0x0c
    void FUN_0047c3a0(HWND hwnd);
};

class Class_0047c6c0 {
public:
    void* smack;                        // +0x00
    int unknown_4;                      // +0x04
    int stopped;                        // +0x08
    HWND hwnd;                          // +0x0c
    void FUN_0047c6c0();
};

// FUNCTION: 0x47c6c0
void Class_0047c6c0::FUN_0047c6c0()
{
    MSG msg;
    while (!stopped) {
        if (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
            // 0x104 and 0x102 are the movie's own stop codes, not the Win95
            // WM_MOUSEMOVE (0x200) and WM_LBUTTONDOWN (0x201).
            if (msg.message == 0x104 && msg.wParam == 0x73) {
                stopped = 1;
                g_game->field_39241 = 0;
                PostQuitMessage(0);
                return;
            }
            if (msg.message == 0x102) {
                stopped = 1;
                g_game->field_39241 = 0;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        } else if (!DAT_004fc40c(smack)) {
            ((Class_0047c3a0*)this)->FUN_0047c3a0(hwnd);
        }
    }
}
