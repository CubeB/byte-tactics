// Decompiled by Opus. Names are provisional.
// Stops the streamed sound: frees the handle at +0x288, then stops and
// releases the streaming sound buffer and closes its file.
#include <windows.h>
#include <dsound.h>

struct File_004bb5d0;

int __stdcall FUN_004b64d0(int i);
int __stdcall FUN_004bb5d0(File_004bb5d0* file);

class Class_004cfb40 {
public:
    char unknown_0[0x1e4];
    IDirectSoundBuffer* stream;        // +0x1e4
    File_004bb5d0* file;               // +0x1e8
    char unknown_1ec[0x288 - 0x1ec];
    int handle;                        // +0x288

    void FUN_004cfb40();
};

// FUNCTION: 0x4cfb40
void Class_004cfb40::FUN_004cfb40()
{
    if (handle != -1) {
        FUN_004b64d0(handle);
        handle = -1;
    }
    if (stream != 0) {
        stream->Stop();
        stream->Release();
        stream = 0;
        FUN_004bb5d0(file);
    }
}
