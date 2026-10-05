// Decompiled by space-bunny-free. Names are provisional.
// Streams the next block of a sound file into a DirectSound buffer: Lock the
// buffer, then either fill it with silence (8-bit PCM is 0x80, 16-bit is 0) or,
// when the file offset at +0x1f8 is negative, seek the file to where the last
// block ended, read at most one buffer's worth of bytes into it and pad the
// rest with silence, then Unlock it. When Lock fails the sound is torn down
// instead: the sound handle is released and the buffer stopped, released and
// its file closed. The read offset is then wrapped: at the end of the file it
// goes back to 0, otherwise it moves on by the buffer size.
#include <windows.h>
#include <dsound.h>
#include <string.h>

struct FileHandle;

int __stdcall RemoveTimer(int i);
int __stdcall HAPI_CloseFile(FileHandle* file);
long __stdcall HAPI_TellFile(FileHandle* file);
long __stdcall HAPI_FileLength(FileHandle* file);
int __stdcall HAPI_readfromfile(FileHandle* file, void* buf, int size);

class Class_004cfb40 {
public:
    char unknown_0[0x1e4];
    IDirectSoundBuffer* stream;             // +0x1e4
    FileHandle* file;                       // +0x1e8
    int bits;                               // +0x1ec  wBitsPerSample
    int size;                               // +0x1f0  bytes per buffer
    int pos;                                // +0x1f4  file offset of the next byte
    int off;                                // +0x1f8  offset read, negative at the start
    char unknown_1fc[0x288 - 0x1fc];
    int handle;                             // +0x288

    void FillStreamHalf();
};

// FUNCTION: 0x4cfca0
void Class_004cfb40::FillStreamHalf()
{
    unsigned long flags;
    char* buffer;
    if (stream->Lock(pos, size, &buffer, &flags, 0, 0, 0) != 0) {
        if (handle != -1) {
            RemoveTimer(handle);
            handle = -1;
        }
        if (stream != 0) {
            stream->Stop();
            stream->Release();
            stream = 0;
            HAPI_CloseFile(file);
        }
        return;
    }
    if (off >= 0) {
        int b = bits;
        unsigned n = size;
        if (b != 8) {
            if (b == 16) {
                memset(buffer, 0, n);
            }
        } else {
            memset(buffer, 0x80, n);
        }
    } else {
        long seek = HAPI_TellFile(file);
        long avail = HAPI_FileLength(file) - seek;
        unsigned n = (unsigned)avail < (unsigned)size ? (unsigned)avail : (unsigned)size;
        HAPI_readfromfile(file, buffer, n);
        if (n < (unsigned)size) {
            off = n + pos;
            unsigned len = size - n;
            char* p = buffer + n;
            int b = bits;
            if (b != 8) {
                if (b == 16) {
                    memset(p, 0, len);
                }
            } else {
                memset(p, 0x80, len);
            }
        }
    }
    stream->Unlock(buffer, size, 0, 0);
    if (pos == 0) {
        pos = size;
    } else {
        pos = 0;
    }
}
