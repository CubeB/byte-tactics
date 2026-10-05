// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Earlier attempts by space-bunny-free, deepseek-v4.1-flash and GPT-6.
// Fixed by deepseek-v4.1-flash: all 895 bytes now match (was 897 bytes at
// 56.9 percent). In the FindChunk helper the locals must be declared
// `unsigned int limit, id, offset, size;` with offset before size. With size
// first, MSVC picked edx as the destination of `size + offset` and emitted
// `lea eax,[edi+edx]` instead of the original `mov edx,[esp+0x38]; add
// edx,edi`, which then rotated every later lea register. All locals stay
// unsigned so the two chunk tests keep `jb` / `jle`.
// check.py reports 100.0% bytes but not MATCH: the only failing reference is
// the callee name (see NAMING NOTE below).
// NAMING NOTE: the original calls 0x4d01b0 as a __thiscall method
// (`push esi; mov ecx,edi; call 0x4d01b0`, and 0x4d01b0 never reads ecx), but
// data/symbols.csv names 0x4d01b0 as the bare `DetectSampleFormat`, so check.py
// reports our member reference as a name mismatch. The orchestrator should add
// the class-qualified name for 0x4d01b0 to data/symbols.csv.
#include <string.h>

struct FileHandle;

class Sound {
  public:
    int PlayFileSample(FileHandle* file, int bytes, int sampleRate, int bits, int channels, int f,
                     int g);
    void* CreateSampleFromFile(FileHandle* file, int bytes, int sampleRate, int bits, int channels);
    void StartStream(FileHandle* file, int sampleRate, int bits, int channels, int volume);
};
class Class_004d02a0 {
  public:
    int OpenSample(char* path, int mode, int p3, int p4);
};

struct Class_004d01b0 { int DetectSampleFormat(FileHandle* file); };
FileHandle* __stdcall HAPI_OpenFileRead(char* path);
int __stdcall HAPI_CloseFile(FileHandle* file);
int __stdcall HAPI_SeekFile(FileHandle* file, int pos);
int __stdcall HAPI_readfromfile(FileHandle* file, void* buf, int size);
int __stdcall HAPI_FileLength(FileHandle* file);

struct WaveFormat {
    unsigned short wFormatTag;
    unsigned short nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short wBitsPerSample;
};

static inline unsigned int FindChunk(FileHandle* file, const char* tag) {
    unsigned int limit, id, offset, size;
    HAPI_SeekFile(file, 4);
    HAPI_readfromfile(file, &limit, 4);
    limit += 8;
    HAPI_SeekFile(file, 12);
    HAPI_readfromfile(file, &id, 4);
    HAPI_readfromfile(file, &size, 4);
    offset = 20;
    for (;;) {
        if (strncmp((char*)&id, tag, 4) == 0)
            return size;
        HAPI_SeekFile(file, size + offset);
        offset += size;
        if (offset >= limit)
            return 0;
        HAPI_readfromfile(file, &id, 4);
        HAPI_readfromfile(file, &size, 4);
        offset += 8;
    }
}
// FUNCTION: 0x4d02a0
int Class_004d02a0::OpenSample(char* path, int mode, int p3, int p4) {
    int result = 0;
    FileHandle* file = HAPI_OpenFileRead(path);
    if (file == 0)
        return result;
    int kind = ((Class_004d01b0*)this)->DetectSampleFormat(file);
    int size = HAPI_FileLength(file);
    switch (kind) {
    case 0:
        HAPI_SeekFile(file, 0);
        switch (mode) {
        case 0:
            result = (int)((Sound*)this)->CreateSampleFromFile(file, size, 0x2b11, 8, 1);
            break;
        case 1:
            result = ((Sound*)this)->PlayFileSample(file, size, 0x2b11, 8, 1, p3, p4);
            break;
        case 2:
            ((Sound*)this)->StartStream(file, 0x2b11, 8, 1, p3);
            return 1;
        }
        break;
    case 1: {
        unsigned int x;
        HAPI_SeekFile(file, 0x16);
        HAPI_readfromfile(file, &x, 4);
        if (x == 0x2af8)
            x = 0x2b11;
        HAPI_SeekFile(file, 0x28);
        switch (mode) {
        case 0:
            result = (int)((Sound*)this)->CreateSampleFromFile(file, size - 0x28, x, 8, 1);
            break;
        case 1:
            result = ((Sound*)this)->PlayFileSample(file, size - 0x28, x, 8, 1, p3, p4);
            break;
        case 2:
            ((Sound*)this)->StartStream(file, x, 8, 1, p3);
            return 1;
        }
        break;
    }
    case 2: {
        unsigned int len = FindChunk(file, "fmt ");
        if (len < 0x10)
            goto end;
        WaveFormat wfx;
        HAPI_readfromfile(file, &wfx, 0x10);
        int bits = wfx.wBitsPerSample;
        int chans = wfx.nChannels;
        len = FindChunk(file, "data");
        if ((int)len <= 0)
            goto end;
        switch (mode) {
        case 0:
            result = (int)((Sound*)this)
                         ->CreateSampleFromFile(file, len, wfx.nSamplesPerSec, bits, chans);
            break;
        case 1:
            result = ((Sound*)this)
                         ->PlayFileSample(file, len, wfx.nSamplesPerSec, bits, chans, p3, p4);
            break;
        case 2:
            ((Sound*)this)->StartStream(file, wfx.nSamplesPerSec, bits, chans, p3);
            return 1;
        }
        break;
    }
    }
end:
    HAPI_CloseFile(file);
    return result;
}
