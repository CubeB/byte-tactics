// Decompiled by Haiku, deepseek-v4.1, deepseek-v4.1-flash, space-bunny-free, Opus, Sonnet, GPT-6.1-sol, mimo-v2.6-pro, Claude Opus 5.5 and LongCat 2.5 Preview Free. Names are provisional.
// The sound object's DirectSound sample and streaming methods and the
// module's mixer volume, sample file and timer helpers, in address order
// (the samples module of data/modules.csv). The object's CD audio methods
// are in cd_audio.cpp; the Sound class below is the view the gathered files
// shared.

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>
// <stdio.h> must stay: the header state decides StartStream's multiply register.
#include <stdio.h>
#include <dsound.h>
#include <float.h>
// Nothing here uses <io.h>: its symbols put InitDirectSound,
// CreateSampleFromMemory and StartStream in the symbol-id windows they match
// in (docs/c2-regalloc.md).
#include <io.h>

extern char __stdcall FindNextCdDrive(char drive);
extern int __stdcall GetVolumeSerial(char drive);

extern HWND g_cdPlayerWindow;

void __stdcall SleepMilliseconds(unsigned int ms);

extern int g_cdFadeVolume;
extern int g_cdCategorySavedTrack[];
extern int g_cdFadeTimer;
extern int g_cdNextTrackTimer;

extern int __stdcall RemoveTimer(int handle);
extern int __stdcall AddTimer(int delay, int id, void (__stdcall* callback)(void*));
extern void __stdcall OnCdFadeTimer(void* unused);

struct App_004b6220 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
};

extern App_004b6220* GetDisplay();

struct FileHandle;

void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);

void __cdecl GameFreeThunk(void* p);

extern int g_playBufferLooping;

extern const GUID IID_IDirectSound3DBuffer;

struct Pos_004cf570 {
    int x, y, z;
};

struct IDirectSound3DBuffer : public IUnknown {
    virtual HRESULT __stdcall GetAllParameters(void* p) = 0;
    virtual HRESULT __stdcall GetConeAngles(LPDWORD a, LPDWORD b) = 0;
    virtual HRESULT __stdcall GetConeOrientation(void* p) = 0;
    virtual HRESULT __stdcall GetConeOutsideVolume(LPLONG p) = 0;
    virtual HRESULT __stdcall GetMaxDistance(float* p) = 0;
    virtual HRESULT __stdcall GetMinDistance(float* p) = 0;
    virtual HRESULT __stdcall GetMode(LPDWORD p) = 0;
    virtual HRESULT __stdcall GetPosition(void* p) = 0;
    virtual HRESULT __stdcall GetVelocity(void* p) = 0;
    virtual HRESULT __stdcall SetAllParameters(void* p, DWORD apply) = 0;
    virtual HRESULT __stdcall SetConeAngles(DWORD a, DWORD b, DWORD apply) = 0;
    virtual HRESULT __stdcall SetConeOrientation(float x, float y, float z, DWORD apply) = 0;
    virtual HRESULT __stdcall SetConeOutsideVolume(LONG v, DWORD apply) = 0;
    virtual HRESULT __stdcall SetMaxDistance(float d, DWORD apply) = 0;
    virtual HRESULT __stdcall SetMinDistance(float d, DWORD apply) = 0;
    virtual HRESULT __stdcall SetMode(DWORD mode, DWORD apply) = 0;
    virtual HRESULT __stdcall SetPosition(float x, float y, float z, DWORD apply) = 0;
};

int __stdcall HAPI_CloseFile(FileHandle* file);
long __stdcall HAPI_TellFile(FileHandle* file);
long __stdcall HAPI_FileLength(FileHandle* file);
int __stdcall HAPI_readfromfile(FileHandle* file, void* buf, int size);

void __stdcall OnStreamTimer(int unused1);
int __stdcall AddTimer(int delay, int param, void (__stdcall* callback)(int));

extern int DAT_0051ff58;
extern char DAT_0051ff60[];

int __stdcall HAPI_SeekFile(void* file, int pos);
int __stdcall HAPI_readfromfile(void* file, void* buf, int size);

// The sound system: the CD audio player, the DirectSound sample channels and
// the streamed sample, at g_game+0x10.
#include "sound.h"

static inline int ClampVolume(int v)
{
    if (v < 0) {
        v = 0;
    }
    if (v > 0xffff) {
        v = 0xffff;
    }
    return v;
}

// The only caller (0x4d02a0) passes its object in ecx, so this is a method
// that ignores `this` (it compiles the same as a __stdcall free function).
struct Class_004d01b0 { int DetectSampleFormat(void* file); };

struct WaveFormat {
    unsigned short wFormatTag;
    unsigned short nChannels;
    int nSamplesPerSec;
    int nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short wBitsPerSample;
};

FileHandle* __stdcall HAPI_OpenFileRead(char* path);

static inline unsigned int FindChunk(FileHandle* file, const char* tag) {
    // Declared in this order, all unsigned: offset before size, and the chunk tests keep jb/jle.
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

extern Sound* g_cdPlayer;

extern HANDLE g_squashThreadLockEvent;
extern int g_squashThreadLockTicket;

struct Entry_004d0a10 {
    short a;                 // +0x0
    short b;                 // +0x2
    short c;                 // +0x4
};

struct Struct_00526ff0 {
    Entry_004d0a10 entries[0x1000];
    short field_6000;        // +0x6000
    short field_6002;        // +0x6002
    short field_6004;        // +0x6004
};

extern Struct_00526ff0* g_squashDictTree;

extern void* g_squashWindow;

// The constructor of the 0x294-byte sound/volume object (allocated by
// 0x49ea3d); 0x4cff30 opens the devices and 0x4d0040 reads the aux volume.
// FUNCTION: 0x4cee50
Sound::Sound()
{
    open = 0;
    use3D = 0;
    noDriver = 0;
    minDistance = 1.0f;
    maxDistance = 1e20f;
    directSound = 0;
    primary = 0;
    for (int j = 0; j < 8; j++) {
        sets[j] = 0;
    }
    InitMixerVolumes();
    maxBuffers = 8;
    count = 0;
    sequence = 1;
    for (int i = 0; i < 32; i++) {
        buffers[i] = 0;
        priority[i] = 0;
        flags[i] = 0;
    }
    stream = 0;
    cdVolume = QueryAuxVolume();
    streamTimer = -1;
}

// Tears down the sound object: releases every buffer set, disables the sound
// handle, stops and releases the streamed buffer and closes its file, shuts
// down the CD audio device, then releases the two remaining DirectSound
// objects.
// FUNCTION: 0x4ceee0
void Sound::ReleaseDirectSound()
{
    int i;
    for (i = 0; i < 8; i++) {
        if (sets[i] != 0) {
            ReleaseSampleSet(sets[i]);
            sets[i] = 0;
        }
    }
    if (streamTimer != -1) {
        RemoveTimer(streamTimer);
        streamTimer = -1;
    }
    if (stream != 0) {
        stream->Stop();
        stream->Release();
        stream = 0;
        HAPI_CloseFile(streamFile);
    }
    CloseCdAudio();
    if (primary != 0)
        primary->Release();
    if (directSound != 0)
        directSound->Release();
    primary = 0;
    directSound = 0;
}

// FUNCTION: 0x4cef90
int Sound::InitDirectSound(int rate, int bits, int channels, HWND handle)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    HRESULT hr;

    if (directSound == 0) {
        hr = DirectSoundCreate(0, &directSound, 0);
        if (hr != 0)
            goto error;
        hr = directSound->SetCooperativeLevel(handle, 2);
        if (hr != 0)
            goto error;
        desc.dwSize = sizeof(desc);
        desc.dwFlags = 1;
        desc.dwBufferBytes = 0;
        desc.dwReserved = 0;
        desc.lpwfxFormat = 0;
        hr = directSound->CreateSoundBuffer(&desc, &primary, 0);
        if (hr != 0)
            goto error;
    }
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nSamplesPerSec = rate;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = rate * wfx.nBlockAlign;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;
    hr = primary->SetFormat(&wfx);
    if (hr != 0)
        goto error;
    sampleChannels = channels;
    sampleRate = rate;
    sampleBits = bits;
    return 1;
error:
    if (hr == (HRESULT)0x88780078)
        noDriver = 1;
    ReleaseDirectSound();
    return 0;
}

// Releases every sound buffer (and buffer set) that has stopped playing.
// FUNCTION: 0x4cf0b0
void Sound::ReapFinishedBuffers()
{
    DWORD status;
    int i;
    for (i = 0; i < 8; i++) {
        if (sets[i] != 0) {
            if (sets[i][0]->GetStatus(&status) != 0 || status == 0) {
                ReleaseSampleSet(sets[i]);
                sets[i] = 0;
            }
        }
    }
    for (i = 0; i < 0x20; i++) {
        if (buffers[i] != 0) {
            if (buffers[i]->GetStatus(&status) != 0 || status == 0) {
                buffers[i] = 0;
                count--;
            }
        }
    }
    if (stream != 0)
        UpdateStream();
}

// FUNCTION: 0x4cf150
void Sound::StopAllBuffers()
{
    IDirectSoundBuffer** slot = buffers;
    for (int i = 0x20; i != 0; i--) {
        IDirectSoundBuffer* obj = *slot;
        if (obj != 0) {
            obj->Stop();
            *slot = 0;
            count--;
        }
        slot++;
    }
}

// Frees a sound channel: picks the first active buffer whose flag bit 0 is
// clear, then the one of those with the lowest priority, stops it and
// removes it from the table (layout as in 0x4cee50 and 0x4cf0b0).
// FUNCTION: 0x4cf180
void Sound::StopOldestBuffer()
{
    int i;
    for (i = 0; i < 0x20; i++) {
        if (buffers[i] != 0 && (flags[i] & 1) == 0)
            break;
    }
    for (int j = i + 1; j < 0x20; j++) {
        if (buffers[j] != 0 && (flags[j] & 1) == 0 && priority[j] < priority[i])
            i = j;
    }
    buffers[i]->Stop();
    buffers[i] = 0;
    count--;
}

// FUNCTION: 0x4cf210
void Sound::SetMaxBuffers(int val)
{
    maxBuffers = val;
}

// FUNCTION: 0x4cf220
int Sound::GetMaxBuffers()
{
    return maxBuffers;
}

// Creates a DirectSound buffer from a block of raw sample data, locks it,
// memcpy's the data in and unlocks it, then wraps the buffer in a 4-slot set
// (only slot 0 is used) allocated with the tagged allocator. Releases the
// buffer and returns 0 on any failure.
// FUNCTION: 0x4cf230
IDirectSoundBuffer** Sound::CreateSampleFromMemory(void* src, DWORD bytes,
                                                  int sampleRate, int bits, int channels)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    IDirectSoundBuffer* buf = 0;

    desc.dwReserved = 0;
    desc.lpwfxFormat = &wfx;
    desc.dwSize = 0x14;
    desc.dwFlags = 0x92;
    desc.dwBufferBytes = bytes;
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nSamplesPerSec = sampleRate;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = sampleRate * wfx.nBlockAlign;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;

    if (directSound->CreateSoundBuffer(&desc, &buf, 0) != 0)
        goto error;

    {
        // The Lock outputs must be their own locals, not &channels/&bytes.
        void* ptr;
        DWORD size;
        if (buf->Lock(0, bytes, &ptr, &size, 0, 0, 0) != 0)
            goto error;
        memcpy(ptr, src, size);
        if (buf->Unlock(ptr, size, 0, 0) != 0)
            goto error;
    }

    {
        IDirectSoundBuffer** set = (IDirectSoundBuffer**)GameAllocIgnoreTag("Digital Audio Sample", 0x10);
        for (int i = 0; i < 4; i++)
            set[i] = 0;
        set[0] = buf;
        return set;
    }

error:
    if (buf != 0)
        buf->Release();
    return 0;
}

// Creates a DirectSound buffer for a block of a sound file. FP exceptions are
// masked while the buffer is created, the buffer is locked, its contents are
// read from the file with HAPI_readfromfile, and it is unlocked again. The buffer
// is then wrapped in a 4-slot set (only slot 0 is used) allocated with the
// tagged allocator. Releases the buffer and returns 0 on any failure.
// FUNCTION: 0x4cf370
IDirectSoundBuffer** Sound::CreateSampleFromFile(FileHandle* file, DWORD bytes,
                                                  int sampleRate, int bits, int channels)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    IDirectSoundBuffer* buf = 0;

    desc.dwReserved = 0;
    desc.lpwfxFormat = &wfx;
    desc.dwSize = 0x14;
    desc.dwFlags = 0x92;
    desc.dwBufferBytes = bytes;
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nSamplesPerSec = sampleRate;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = sampleRate * wfx.nBlockAlign;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;

    unsigned int oldcw = _controlfp(0, 0);
    _controlfp(0x18, 0x18);
    HRESULT hr = directSound->CreateSoundBuffer(&desc, &buf, 0);
    _controlfp(oldcw, 0x18);
    if (hr != 0)
        goto error;

    {
        void* ptr;
        DWORD size;
        hr = buf->Lock(0, bytes, &ptr, &size, 0, 0, 0);
        if (hr != 0)
            goto error;
        unsigned copied = (unsigned)HAPI_readfromfile(file, ptr, size);
        hr = buf->Unlock(ptr, size, 0, 0);
        if (hr != 0)
            goto error;
        if (copied < size)
            goto error;
    }

    {
        IDirectSoundBuffer** set = (IDirectSoundBuffer**)GameAllocIgnoreTag("Digital Audio Sample", 0x10);
        for (int i = 0; i < 4; i++)
            set[i] = 0;
        set[0] = buf;
        return set;
    }

error:
    if (buf != 0)
        buf->Release();
    return 0;
}

// FUNCTION: 0x4cf4d0
void Sound::ReleaseSampleSet(IDirectSoundBuffer** set)
{
    if (set == 0)
        return;
    for (int i = 0; i < 4; i++) {
        if (set[i] != 0) {
            set[i]->Stop();
            set[i]->Release();
            for (int j = 0; j < 0x20; j++) {
                if (buffers[j] == set[i]) {
                    buffers[j] = 0;
                    flags[j] = 0;
                    count--;
                    break;
                }
            }
        }
    }
    GameFreeThunk(set);
}

// Calls PlaySampleSet on the same object with the global flag at 0x51ff48 set
// for the duration of the call.
// FUNCTION: 0x4cf540
void Sound::PlayLooping(IDirectSoundBuffer** set, LONG volume)
{
    g_playBufferLooping = 1;
    PlaySampleSet(set, volume, 0);
    g_playBufferLooping = 0;
}

// Plays a sample on one of the four DirectSound buffers in `set`: a buffer that
// has stopped is reused as it is; otherwise a free entry gets a duplicate of
// set[0], or else the buffer furthest through its playback is restarted. The
// buffer is then given its 3D settings, volume and play flags and filed in the
// channel table (count +0x30, sequence +0x34, buffers +0x38, priorities +0xb8,
// looping flags +0x138, IDirectSound +0x24). IDirectSound3DBuffer is declared
// by hand because the toolchain's <dsound.h> is DirectX 3; IID_IDirectSound3DBuffer is its
// IID.
// FUNCTION: 0x4cf570
int Sound::PlaySampleSet(IDirectSoundBuffer** set, LONG volume, Pos_004cf570* pos)
{
    IDirectSoundBuffer* unit = 0;
    int slot = 0;
    if (g_playBufferLooping != 0) {
        for (int i = 0; i < 0x20; i++) {
            if (buffers[i] != 0 && flags[i] == 1)
                return 0;
        }
    }
    while (count >= maxBuffers)
        StopOldestBuffer();
    // best = 0 stays before the null test; bestidx must be the last variable set to 0.
    DWORD best = 0;
    if (set == 0)
        return 0;
    int bestidx = 0;
    for (int i = 0; i < 4; i++) {
        if (set[i] != 0) {
            DWORD status;
            if (set[i]->GetStatus(&status) != 0)
                return 0;
            if (status == 0) {
                unit = set[i];
                break;
            }
            DWORD play, write;
            set[i]->GetCurrentPosition(&play, &write);
            if (play > best) {
                best = play;
                bestidx = i;
            }
        } else {
            slot = i;
        }
    }
    if (unit == 0) {
        if (slot > 0) {
            if (directSound->DuplicateSoundBuffer(set[0], &unit) != 0)
                return 0;
            set[slot] = unit;
        } else {
            unit = set[bestidx];
            unit->SetCurrentPosition(0);
        }
    }
    IDirectSound3DBuffer* chan;
    if (unit->QueryInterface(IID_IDirectSound3DBuffer, (void**)&chan) == 0) {
        if (use3D == 0 || pos == 0) {
            chan->SetMode(2, 0);
        } else {
            chan->SetPosition((float)pos->x, (float)pos->y, (float)pos->z, 0);
            chan->SetMinDistance(minDistance, 0);
            chan->SetMaxDistance(maxDistance, 0);
            chan->SetMode(0, 0);
        }
        chan->Release();
    }
    if (unit->SetCurrentPosition(0) != 0)
        return 0;
    if (unit->SetVolume(volume) != 0)
        return 0;
    if (unit->Play(0, 0, g_playBufferLooping != 0) != 0)
        return 0;
    for (int j = 0; j < 0x20; j++) {
        if (buffers[j] == 0) {
            buffers[j] = unit;
            priority[j] = ++sequence;
            flags[j] = g_playBufferLooping != 0;
            count++;
            break;
        }
    }
    return 1;
}

// Sound object (see 0x4cf8a0): releases finished buffers, then creates a new
// buffer set in the first free slot and starts it; frees the set again if
// starting fails. Returns 1 on success.
// FUNCTION: 0x4cf800
int Sound::PlayMemorySample(void* src, DWORD bytes, int sampleRate, int bits, int channels, LONG volume, Pos_004cf570* pos)
{
    ReapFinishedBuffers();
    for (int i = 0; i < 8; i++) {
        if (sets[i] == 0) {
            sets[i] = CreateSampleFromMemory(src, bytes, sampleRate, bits, channels);
            if (sets[i] == 0)
                return 0;
            if (PlaySampleSet(sets[i], volume, pos) == 0) {
                ReleaseSampleSet(sets[i]);
                sets[i] = 0;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

// Sound object (see 0x4cf0b0): releases finished buffers, then creates a new
// buffer set in the first free slot and starts it; frees the set again if
// starting fails. Returns 1 on success.
// FUNCTION: 0x4cf8a0
int Sound::PlayFileSample(FileHandle* file, DWORD bytes, int sampleRate, int bits, int channels, LONG volume, Pos_004cf570* pos)
{
    ReapFinishedBuffers();
    for (int i = 0; i < 8; i++) {
        if (sets[i] == 0) {
            sets[i] = CreateSampleFromFile(file, bytes, sampleRate, bits, channels);
            if (sets[i] == 0)
                return 0;
            if (PlaySampleSet(sets[i], volume, pos) == 0) {
                ReleaseSampleSet(sets[i]);
                sets[i] = 0;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

// Streamed sound init (see 0x4cfb40 / 0x4cfbc0 / 0x4cfca0): tears down any
// previous handle and streaming buffer, then creates a new looping
// DirectSound buffer twice the size of one block, sets its volume and starts
// it. On any failure the buffer is stopped, released and its file closed.
// FUNCTION: 0x4cf940
void Sound::StartStream(FileHandle* file, int sampleRate, int bits,
                                  int channels, LONG volume)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC desc;
    // Signed: unsigned would change the shift.
    int n;

    if (streamTimer != -1) {
        RemoveTimer(streamTimer);
        streamTimer = -1;
    }
    if (stream != 0) {
        stream->Stop();
        stream->Release();
        stream = 0;
        HAPI_CloseFile(streamFile);
    }
    if (streamTimer != -1) {
        RemoveTimer(streamTimer);
        streamTimer = -1;
    }

    desc.dwSize = 0x14;
    desc.dwFlags = 0x82;
    n = channels * sampleRate * (bits / 8) * 2;
    streamSize = n / 2;
    desc.dwReserved = 0;
    // dwBufferBytes stays early in this group of stores, or the scheduler sinks it.
    desc.dwBufferBytes = n;
    desc.lpwfxFormat = &wfx;
    wfx.wFormatTag = 1;
    wfx.nChannels = (WORD)channels;
    wfx.nSamplesPerSec = sampleRate;
    wfx.nBlockAlign = (WORD)(bits / 8 * channels);
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * sampleRate;
    wfx.wBitsPerSample = (WORD)bits;
    wfx.cbSize = 0x12;

    if (directSound->CreateSoundBuffer(&desc, &stream, 0) != 0) {
        stream = 0;
        return;
    }

    streamBits = bits;
    streamPos = 0;
    streamOffset = -1;
    streamFile = file;
    FillStreamHalf();
    if (stream->SetVolume(volume) != 0) {
        if (streamTimer != -1) {
            RemoveTimer(streamTimer);
            streamTimer = -1;
        }
        if (stream != 0) {
            stream->Stop();
            stream->Release();
            stream = 0;
            HAPI_CloseFile(streamFile);
        }
        return;
    }
    if (stream->Play(0, 0, 1) != 0) {
        if (streamTimer != -1) {
            RemoveTimer(streamTimer);
            streamTimer = -1;
        }
        if (stream != 0) {
            stream->Stop();
            stream->Release();
            stream = 0;
            HAPI_CloseFile(streamFile);
        }
    }
}

// Stops the streamed sound: frees the handle at +0x288, then stops and
// releases the streaming sound buffer and closes its file.
// FUNCTION: 0x4cfb40
void Sound::StopStream()
{
    if (streamTimer != -1) {
        RemoveTimer(streamTimer);
        streamTimer = -1;
    }
    if (stream != 0) {
        stream->Stop();
        stream->Release();
        stream = 0;
        HAPI_CloseFile(streamFile);
    }
}

// FUNCTION: 0x4cfba0
int Sound::IsStreamActive(void)
{
    if (stream == 0 && streamTimer == -1) {
        return 0;
    }
    return 1;
}

// Polls a streamed sound's DirectSound buffer: reads the current play cursor
// and, when the buffer is due, either tears the sound down (releasing the
// handle, stopping and releasing the buffer and closing the file) or loads the
// next block into the buffer (0x4cfca0).
// FUNCTION: 0x4cfbc0
void Sound::UpdateStream()
{
    unsigned long play;
    unsigned long write;
    stream->GetCurrentPosition(&play, &write);
    if (streamOffset >= 0) {
        if (streamPos == 0 && streamOffset < streamSize && play >= streamSize) {
            if (streamTimer != -1) {
                RemoveTimer(streamTimer);
                streamTimer = -1;
            }
            if (stream != 0) {
                stream->Stop();
                stream->Release();
                stream = 0;
                HAPI_CloseFile(streamFile);
            }
            return;
        }
        if (streamPos != 0 && streamOffset >= streamSize && play < streamSize) {
            if (streamTimer != -1) {
                RemoveTimer(streamTimer);
                streamTimer = -1;
            }
            if (stream != 0) {
                stream->Stop();
                stream->Release();
                stream = 0;
                HAPI_CloseFile(streamFile);
            }
            return;
        }
    }
    if (streamPos >= streamSize) {
        if (play >= streamSize) {
            return;
        }
        FillStreamHalf();
    } else {
        if (play < streamSize) {
            return;
        }
        FillStreamHalf();
    }
}

// Streams the next block of a sound file into a DirectSound buffer: Lock the
// buffer, then either fill it with silence (8-bit PCM is 0x80, 16-bit is 0) or,
// when the file offset at +0x1f8 is negative, seek the file to where the last
// block ended, read at most one buffer's worth of bytes into it and pad the
// rest with silence, then Unlock it. When Lock fails the sound is torn down
// instead: the sound handle is released and the buffer stopped, released and
// its file closed. The read offset is then wrapped: at the end of the file it
// goes back to 0, otherwise it moves on by the buffer size.
// FUNCTION: 0x4cfca0
void Sound::FillStreamHalf()
{
    unsigned long flags;
    char* buffer;
    if (stream->Lock(streamPos, streamSize, &buffer, &flags, 0, 0, 0) != 0) {
        if (streamTimer != -1) {
            RemoveTimer(streamTimer);
            streamTimer = -1;
        }
        if (stream != 0) {
            stream->Stop();
            stream->Release();
            stream = 0;
            HAPI_CloseFile(streamFile);
        }
        return;
    }
    if (streamOffset >= 0) {
        int b = streamBits;
        unsigned n = streamSize;
        if (b != 8) {
            if (b == 16) {
                memset(buffer, 0, n);
            }
        } else {
            memset(buffer, 0x80, n);
        }
    } else {
        long seek = HAPI_TellFile(streamFile);
        long avail = HAPI_FileLength(streamFile) - seek;
        unsigned n = (unsigned)avail < (unsigned)streamSize ? (unsigned)avail : (unsigned)streamSize;
        HAPI_readfromfile(streamFile, buffer, n);
        if (n < (unsigned)streamSize) {
            streamOffset = n + streamPos;
            unsigned len = streamSize - n;
            char* p = buffer + n;
            int b = streamBits;
            if (b != 8) {
                if (b == 16) {
                    memset(p, 0, len);
                }
            } else {
                memset(p, 0x80, len);
            }
        }
    }
    stream->Unlock(buffer, streamSize, 0, 0);
    if (streamPos == 0) {
        streamPos = streamSize;
    } else {
        streamPos = 0;
    }
}

// FUNCTION: 0x4cfe40
void Sound::FillSilence(void* dest, unsigned int size)
{
    if (streamBits != 8) {
        if (streamBits != 0x10) {
            return;
        }
        memset(dest, 0, size);
        return;
    }
    memset(dest, 0x80, size);
}

// FUNCTION: 0x4cfe80
void Sound::Enable3D()
{
    use3D = 1;
}

// FUNCTION: 0x4cfe90
void Sound::Disable3D()
{
    use3D = 0;
}

// FUNCTION: 0x4cfea0
int Sound::Is3DEnabled()
{
    return use3D;
}

// FUNCTION: 0x4cfeb0
void Sound::Set3DDistances(float minimum, float maximum)
{
    minDistance = minimum;
    maxDistance = maximum;
}

// FUNCTION: 0x4cfed0
int NullStubRet0(void)
{
    return 0;
}

// FUNCTION: 0x4cfee0
void NopRet_B(void)
{
}

// FUNCTION: 0x4cfef0
int NullStubRet0_B(void)
{
    return 0;
}

// FUNCTION: 0x4cff00
void NopRet_C(void)
{
}

// FUNCTION: 0x4cff10
int NullStubRet0_C(void)
{
    return 0;
}

// FUNCTION: 0x4cff20
int Sound::HasNoDriver()
{
    return noDriver;
}

// The two volume readers the original inlined into InitMixerVolumes (their
// out-of-line twins are 0x4cfff0 and 0x4d0040). They stay free helpers because
// sound.h cannot include <mmsystem.h>.
static inline int GetWaveOutVolume(Sound* sound)
{
    DWORD volume;
    for (int i = 0; i < sound->waveDevices; i++) {
        if (waveOutGetVolume((HWAVEOUT)i, &volume) == 0)
            return volume & 0xffff;
    }
    return -1;
}

static inline int GetAuxOutVolume(Sound* sound)
{
    DWORD volume;
    if (sound->auxDevice >= 0 && auxGetVolume(sound->auxDevice, &volume) == 0)
        return volume & 0xffff;
    return -1;
}

// Picks the first CD-audio auxiliary device, then caches the wave-out and
// auxiliary volumes (-1 when unavailable).
// The original calls this out of line from the constructor (0x4cee50).
#pragma auto_inline(off)
// FUNCTION: 0x4cff30
void Sound::InitMixerVolumes()
{
    AUXCAPSA caps;

    waveDevices = waveOutGetNumDevs();
    auxDevice = -1;

    int aux_count = auxGetNumDevs();
    for (int i = 0; i < aux_count; i++) {
        // Result held in a named local: sets the callee-saved register order.
        int result = auxGetDevCapsA(i, &caps, sizeof(caps));
        if (result == 0 && caps.wTechnology == AUXCAPS_CDAUDIO) {
            auxDevice = i;
            break;
        }
    }

    waveVolume = GetWaveOutVolume(this);
    auxVolume = GetAuxOutVolume(this);
}
#pragma auto_inline(on)

// Returns the left-channel volume of the first wave-out device that reports
// one, or -1. The dllimport call is what makes MSVC keep the import address
// in ebx for the loop.
// FUNCTION: 0x4cfff0
int Sound::QueryWaveVolume()
{
    DWORD volume;
    for (int i = 0; i < waveDevices; i++) {
        if (waveOutGetVolume((HWAVEOUT)i, &volume) == 0)
            return volume & 0xffff;
    }
    return -1;
}

// Returns the aux (CD) volume of the left channel, 0..0xffff, or -1 when
// there is no aux device or its volume cannot be read. The setter is
// 0x4d00d0. The original calls this out of line from the constructor (0x4cee50).
#pragma auto_inline(off)
// FUNCTION: 0x4d0040
int Sound::QueryAuxVolume()
{
    DWORD volume;
    if (auxDevice >= 0 && auxGetVolume(auxDevice, &volume) == 0) {
        return volume & 0xffff;
    }
    return -1;
}
#pragma auto_inline(on)

// Sets the volume of every wave-out device, clamped to 0..0xffff. Returns
// nonzero if any device failed.
// FUNCTION: 0x4d0070
int Sound::SetWaveVolume(int volume)
{
    int v = ClampVolume(volume);
    int failed = 0;
    for (int i = 0; i < waveDevices; i++) {
        if (waveOutSetVolume((HWAVEOUT)i, (v << 16) | v) != 0) {
            failed = 1;
        }
    }
    return failed;
}

// Sets the aux (CD) volume, clamped to 0..0xffff; unless `temporary`, it is
// also remembered. Returns nonzero on success.
// FUNCTION: 0x4d00d0
int Sound::SetAuxVolume(int volume, int temporary)
{
    if (step != 0 && temporary == 0) {
        return 1;
    }
    int v = ClampVolume(volume);
    if (temporary == 0) {
        cdVolume = v;
    }
    return auxSetVolume(auxDevice, (v << 16) | v) == 0;
}

// FUNCTION: 0x4d0130
void Sound::RestoreMixerVolumes()
{
    if (waveVolume >= 0) {
        int v = ClampVolume(waveVolume);
        for (int i = 0; i < waveDevices; i++) {
            waveOutSetVolume((HWAVEOUT)i, (v << 16) | v);
        }
    }
    if (auxVolume >= 0 && step == 0) {
        int v = ClampVolume(auxVolume);
        cdVolume = v;
        auxSetVolume(auxDevice, (v << 16) | v);
    }
}

// Sniffs the head of a sound file: 0x4d01b0 returns 1 for a DIGI/HSHD/SDAT
// file, 2 for a RIFF/WAVE file and 0 for anything else. The four byte tag is
// read into one local buffer that MSVC lays over the dead parameter slot.
// FUNCTION: 0x4d01b0
int Class_004d01b0::DetectSampleFormat(void* file)
{
    char tag[4];
    HAPI_SeekFile(file, 0);
    HAPI_readfromfile(file, tag, 4);
    if (strncmp(tag, "DIGI", 4) == 0) {
        HAPI_SeekFile(file, 8);
        HAPI_readfromfile(file, tag, 4);
        if (strncmp(tag, "HSHD", 4) == 0) {
            HAPI_SeekFile(file, 0x20);
            HAPI_readfromfile(file, tag, 4);
            if (strncmp(tag, "SDAT", 4) == 0) {
                return 1;
            }
        }
    }
    if (strncmp(tag, "RIFF", 4) == 0) {
        HAPI_SeekFile(file, 8);
        HAPI_readfromfile(file, tag, 4);
        if (strncmp(tag, "WAVE", 4) == 0) {
            return 2;
        }
    }
    return 0;
}

// FUNCTION: 0x4d02a0
int Sound::OpenSample(char* path, int mode, int p3, int p4) {
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
            result = ((Sound*)this)->PlayFileSample(file, size, 0x2b11, 8, 1, p3, (Pos_004cf570*)p4);
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
            result = ((Sound*)this)->PlayFileSample(file, size - 0x28, x, 8, 1, p3, (Pos_004cf570*)p4);
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
            result = (int)CreateSampleFromFile(file, len, wfx.nSamplesPerSec, bits, chans);
            break;
        case 1:
            result = PlayFileSample(file, len, wfx.nSamplesPerSec, bits, chans, p3, (Pos_004cf570*)p4);
            break;
        case 2:
            StartStream(file, wfx.nSamplesPerSec, bits, chans, p3);
            return 1;
        }
        break;
    }
    }
end:
    HAPI_CloseFile(file);
    return result;
}

// FUNCTION: 0x4d0620
void Sound::LoadSample(char* param_1)
{
    OpenSample(param_1, 0, 0, 0);
}

// FUNCTION: 0x4d0640
void Sound::PlaySample(const char* param1, int param2, int param3)
{
    OpenSample((char*)param1, 1, param2, param3);
}

// FUNCTION: 0x4d0660
void Sound::StreamSample(char* a, int b)
{
    OpenSample(a, 2, b, 0);
}

// FUNCTION: 0x4d0680
void __stdcall OnStreamTimer(int unused1)
{
    RemoveTimer(g_cdPlayer->streamTimer);
    g_cdPlayer->streamTimer = -1;
    g_cdPlayer->OpenSample(DAT_0051ff60, 2, DAT_0051ff58, 0);
}

// Remembers a name and a value, then starts a timer whose callback
// (0x4d0680) hands them back to this object.
// FUNCTION: 0x4d06c0
int Sound::StreamSampleDelayed(char* name, int value, int delay)
{
    strcpy(DAT_0051ff60, name);
    DAT_0051ff58 = value;
    streamTimer = AddTimer(delay, 0, OnStreamTimer);
    return 1;
}

// Walks a chunked file's marker table: the header holds the table size (plus
// the 8 bytes of the two header fields) and the first marker, then the table
// is a run of [4 byte name][4 byte offset] pairs. Returns the offset of the
// named marker, or 0 when the table runs out.
// Must stay a class method: the original is a this-less __thiscall.
// FUNCTION: 0x4d0720
int Sound::FindChunkSize(void* file, char* target)
{
    char name[4];
    unsigned int total;
    unsigned int pos;
    unsigned int off;

    HAPI_SeekFile(file, 4);
    HAPI_readfromfile(file, &total, 4);
    total += 8;
    HAPI_SeekFile(file, 0xc);
    HAPI_readfromfile(file, name, 4);
    HAPI_readfromfile(file, &off, 4);
    pos = 0x14;
    for (;;) {
        if (strncmp(name, target, 4) == 0)
            return off;
        HAPI_SeekFile(file, pos + off);
        pos += off;
        if (pos >= total)
            return 0;
        HAPI_readfromfile(file, name, 4);
        HAPI_readfromfile(file, &off, 4);
        pos += 8;
    }
}

// Must stay a class method: the original is a this-less __thiscall.
// FUNCTION: 0x4d07f0
int Sound::ReadWaveFormat(void* file, int* sampleRate, int* bitsPerSample, int* channels)
{
    unsigned int total;
    char tag[4];
    unsigned int pos;
    unsigned int len;
    unsigned int n;
    char fmt[0x10];

    HAPI_SeekFile(file, 4);
    HAPI_readfromfile(file, &total, 4);
    total = total + 8;
    HAPI_SeekFile(file, 0xc);
    HAPI_readfromfile(file, tag, 4);
    HAPI_readfromfile(file, &len, 4);
    // Set after the len read, just before the loop.
    pos = 0x14;
    for (;;) {
        if (strncmp(tag, "fmt ", 4) == 0) {
            n = len;
            break;
        }
        HAPI_SeekFile(file, pos + len);
        pos += len;
        if (pos >= total) {
            n = 0;
            break;
        }
        HAPI_readfromfile(file, tag, 4);
        HAPI_readfromfile(file, &len, 4);
        pos += 8;
    }
    if (n < 0x10)
        return 0;
    HAPI_readfromfile(file, fmt, 0x10);
    *sampleRate = *(int*)(fmt + 4);
    *bitsPerSample = *(unsigned short*)(fmt + 14);
    *channels = *(unsigned short*)(fmt + 2);
    return 1;
}

// Walks a chunked file's marker table looking for the record tagged "data" and
// returns that record's 4 byte header field (the record length), or 0 when the
// walk runs past the table.
// Must stay a class method: the original is a this-less __thiscall.
// FUNCTION: 0x4d0910
int Sound::FindDataChunkSize(void* file)
{
    char tag[4];
    unsigned int size;
    unsigned int pos;
    unsigned int len;

    HAPI_SeekFile(file, 4);
    HAPI_readfromfile(file, &size, 4);
    size += 8;
    HAPI_SeekFile(file, 0xc);
    HAPI_readfromfile(file, tag, 4);
    HAPI_readfromfile(file, &len, 4);
    // Set after the len read, just before the loop.
    pos = 0x14;
    for (;;) {
        if (strncmp(tag, "data", 4) == 0)
            return len;
        HAPI_SeekFile(file, pos + len);
        pos += len;
        if (pos >= size)
            return 0;
        HAPI_readfromfile(file, tag, 4);
        HAPI_readfromfile(file, &len, 4);
        pos += 8;
    }
}

// FUNCTION: 0x4d09e0
void InitLzssLock()
{
    g_squashThreadLockTicket = 0;
    if (g_squashThreadLockEvent != 0) {
        ResetEvent(g_squashThreadLockEvent);
        return;
    }
    g_squashThreadLockEvent = CreateEventA(0, 0, 0, 0);
}

// FUNCTION: 0x4d0a10
void __stdcall LzssInitTree(int index)
{
    g_squashDictTree->field_6000 = 0;
    g_squashDictTree->field_6004 = (short)index;
    g_squashDictTree->field_6002 = 0;
    g_squashDictTree->entries[index].a = 0x1000;
    g_squashDictTree->entries[index].c = 0;
    g_squashDictTree->entries[index].b = 0;
}

// FUNCTION: 0x4d0a70
int __cdecl LzssAllocWindow(void)
{
    void* eax = calloc(1, 0x1011);
    g_squashWindow = eax;
    if (eax != 0) return 0;
    return -1;
}

// FUNCTION: 0x4d0a90
int LzssAllocTree()
{
    void* p = calloc(0x1001, 6);
    g_squashDictTree = (Struct_00526ff0*)p;
    return p ? 0 : -1;
}
