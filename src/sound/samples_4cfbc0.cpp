// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Polls a streamed sound's DirectSound buffer: reads the current play cursor
// and, when the buffer is due, either tears the sound down (releasing the
// handle, stopping and releasing the buffer and closing the file) or loads the
// next block into the buffer (0x4cfca0).
#include <windows.h>
#include <dsound.h>

struct File_004bb5d0;

int __stdcall RemoveTimer(int i);
int __stdcall FUN_004bb5d0(File_004bb5d0* file);

class Class_004cfb40 {
public:
    char unknown_0[0x1e4];
    IDirectSoundBuffer* stream;             // +0x1e4
    File_004bb5d0* file;                    // +0x1e8
    int bits;                               // +0x1ec
    int size;                               // +0x1f0
    int pos;                                // +0x1f4
    int off;                                // +0x1f8
    char unknown_1fc[0x288 - 0x1fc];
    int handle;                             // +0x288

    void FUN_004cfbc0();
    void FUN_004cfca0();
};

// FUNCTION: 0x4cfbc0
void Class_004cfb40::FUN_004cfbc0()
{
    unsigned long play;
    unsigned long write;
    stream->GetCurrentPosition(&play, &write);
    if (off >= 0) {
        if (pos == 0 && off < size && play >= size) {
            if (handle != -1) {
                RemoveTimer(handle);
                handle = -1;
            }
            if (stream != 0) {
                stream->Stop();
                stream->Release();
                stream = 0;
                FUN_004bb5d0(file);
            }
            return;
        }
        if (pos != 0 && off >= size && play < size) {
            if (handle != -1) {
                RemoveTimer(handle);
                handle = -1;
            }
            if (stream != 0) {
                stream->Stop();
                stream->Release();
                stream = 0;
                FUN_004bb5d0(file);
            }
            return;
        }
    }
    if (pos >= size) {
        if (play >= size) {
            return;
        }
        FUN_004cfca0();
    } else {
        if (play < size) {
            return;
        }
        FUN_004cfca0();
    }
}
