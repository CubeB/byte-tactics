// Decompiled by Haiku, deepseek-v4.1-flash, space-bunny-free, Opus, Sonnet, GPT-6.1-sol, mimo-v2.6-pro, Claude Opus 5.5 and LongCat 2.5 Preview Free. Names are provisional.

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
extern int DAT_0051ff20[];
extern int g_cdFadeTimer;
extern int g_cdNextTrackTimer;

extern int __stdcall RemoveTimer(int handle);
extern int __stdcall AddTimer(int delay, int id, void (__stdcall* callback)(void*));
extern void __stdcall OnCdFadeTimer(void* unused);

class Class_004cdb40 {
public:
    void PlayNextTrack();
};

class Class_004d00d0 {
public:
    int SetAuxVolume(int volume, int temporary);
};

struct App_004b6220 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
};

extern App_004b6220* GetDisplay();

class Class_004cff30 {
public:
    void InitMixerVolumes();
};

class Class_004d0040 {
public:
    int QueryAuxVolume();
};

struct FileHandle;

class Class_004ce410 {
public:
    int open;                          // +0x0

    void CloseCdAudio();
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

void __cdecl FUN_004d85a0(void* p);

extern int g_playBufferLooping;

extern const GUID DAT_004fcf68;

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

class Class_004d02a0 {
public:
    void OpenSample(const char* name, int mode, int a, int b);
};

void __stdcall OnStreamTimer(int unused1);
int __stdcall AddTimer(int delay, int param, void (__stdcall* callback)(int));

extern int DAT_0051ff58;
extern char DAT_0051ff60[];

int __stdcall HAPI_SeekFile(void* file, int pos);
int __stdcall HAPI_readfromfile(void* file, void* buf, int size);

// The sound system: the CD audio player, the DirectSound sample channels and
// the streamed sample, at g_game+0x10.
class Sound {
public:
    int field_0;                       // +0x00
    int use3D;                         // +0x04
    float minDistance;                 // +0x08
    float maxDistance;                 // +0x0c
    int waveDevices;                   // +0x10
    unsigned int auxDevice;            // +0x14
    int waveVolume;                    // +0x18
    int auxVolume;                     // +0x1c
    int cdVolume;                      // +0x20
    IDirectSound* directSound;         // +0x24
    IDirectSoundBuffer* primary;       // +0x28
    int maxBuffers;                    // +0x2c
    int count;                         // +0x30, of the playing buffers
    int sequence;                      // +0x34
    IDirectSoundBuffer* buffers[0x20]; // +0x38
    int priority[0x20];                // +0xb8
    int flags[0x20];                   // +0x138, looping
    int sampleRate;                    // +0x1b8
    int sampleBits;                    // +0x1bc
    int sampleChannels;                      // +0x1c0
    IDirectSoundBuffer** sets[8];      // +0x1c4
    IDirectSoundBuffer* stream;        // +0x1e4
    FileHandle* streamFile;                  // +0x1e8, the streamed sample's
    int streamBits;                          // +0x1ec
    int streamSize;                          // +0x1f0
    int streamPos;                           // +0x1f4
    int streamOffset;                           // +0x1f8
    int field_1fc;                     // +0x1fc
    int trackCount;                    // +0x200
    int field_204;                     // +0x204
    int currentTrack;                  // +0x208
    int playState;                     // +0x20c
    int discSerial;                    // +0x210
    char arr_214[100];                 // +0x214
    int trackCategory;                 // +0x278
    int field_27c;                     // +0x27c
    int dataTrack;                     // +0x280, track 1 is not audio
    int field_284;                     // +0x284
    int streamTimer;                        // +0x288, the stream's timer
    int field_28c;                     // +0x28c
    int noDriver;                      // +0x290

    int FUN_004cd9c0();
    int QueryDisc();
    int GetPlayState();
    void CloseCdPlayerWindow();
    bool HasCdPlayerWindow();
    void SetTrackCategory(int mode);
    int GetCurrentTrack();
    int IsCdPlaying();
    int PlayCdTrack(int index, int flag);
    Sound();
    void ReleaseDirectSound();
    int InitDirectSound(int rate, int bits, int channels, HWND handle);
    void ReapFinishedBuffers();
    void StopAllBuffers();
    void StopOldestBuffer();
    void SetMaxBuffers(int val);
    int GetMaxBuffers();
    IDirectSoundBuffer** CreateSampleFromMemory(void* src, DWORD bytes, int sampleRate, int bits, int channels);
    IDirectSoundBuffer** CreateSampleFromFile(FileHandle* file, DWORD bytes, int sampleRate, int bits, int channels);
    void ReleaseSampleSet(IDirectSoundBuffer** set);
    void PlayLooping(IDirectSoundBuffer** set, LONG volume);
    int PlaySampleSet(IDirectSoundBuffer** set, LONG volume, Pos_004cf570* pos);
    int PlayMemorySample(void* src, DWORD bytes, int sampleRate, int bits, int channels, LONG volume, Pos_004cf570* pos);
    int PlayFileSample(FileHandle* file, DWORD bytes, int sampleRate, int bits, int channels, LONG volume, Pos_004cf570* pos);
    void StartStream(FileHandle* file, int sampleRate, int bits, int channels, LONG volume);
    void StopStream();
    int IsStreamActive(void);
    void UpdateStream();
    void FillStreamHalf();
    void FillSilence(void* dest, unsigned int size);
    void Enable3D();
    void Disable3D();
    int Is3DEnabled();
    void Set3DDistances(float minimum, float maximum);
    int HasNoDriver();
    void LoadSample(char* param_1);
    void PlaySample(const char* param1, int param2, int param3);
    void StreamSample(char* a, int b);
    int StreamSampleDelayed(char* name, int value, int delay);
    int FindChunkSize(void* file, char* target);
    int ReadWaveFormat(void* file, int* sampleRate, int* bitsPerSample, int* channels);
    int FindDataChunkSize(void* file);
};

// FUNCTION: 0x4cd9c0
int Sound::FUN_004cd9c0()
{
    return discSerial;
}

// FUNCTION: 0x4cda00
int Sound::QueryDisc()
{
    int other;
    int i;
    char type[32];
    char buf[32];

    currentTrack = 0;
    playState = 0;
    int drive = FindNextCdDrive(0);
    if (drive != 0)
        discSerial = GetVolumeSerial(drive);
    // Both mciSendStringA results are assigned to an int local before being tested.
    int hr = mciSendStringA("status cdaudio number of tracks", buf, 0x20, 0);
    // The whole body, tail included, nests in this if; the failure is the
    // trailing return 0 after it.
    if (hr == 0) {
        trackCount = atoi(buf);
        mciSendStringA("set cdaudio time format tmsf", 0, 0, 0);
        hr = mciSendStringA("status cdaudio type track 1", type, 0x20, 0);
        if (hr != 0)
            goto notAudio;
        if (strcmp(type, "audio") != 0) {
            // other is a dead store; the no-op loop keeps its stack slot and the frame size.
            other = strcmp(type, "other");
            for (i = 0; i < other; i++) {
            }
            goto notAudio;
        } else {
            dataTrack = 0;
            goto done;
        }
notAudio:
        dataTrack = 1;
        if (--trackCount < 0)
            trackCount = 0;
done:
        // The tail stays a single return statement.
        if (trackCount != 0)
            currentTrack = 1;
        return trackCount;
    }
    return 0;
}

// FUNCTION: 0x4ce020
int Sound::GetPlayState()
{
    return playState;
}

// FUNCTION: 0x4ce190
void Sound::CloseCdPlayerWindow()
{
    if (g_cdPlayerWindow) {
        SendMessageA(g_cdPlayerWindow, WM_CLOSE, 0, 0);
        SendMessageA(g_cdPlayerWindow, WM_QUIT, 0, 0);
        SleepMilliseconds(500);
        g_cdPlayerWindow = 0;
    }
}

// A method of the object at g_game+0x10 (its one caller, 0x4263b0, loads ecx
// from there) that never uses `this`, like 0x4ce190.
// FUNCTION: 0x4ce1d0
bool Sound::HasCdPlayerWindow()
{
    return g_cdPlayerWindow != 0;
}

// FUNCTION: 0x4ce690
void Sound::SetTrackCategory(int mode)
{
    int old = trackCategory;
    if (old == mode)
        return;
    if (old >= 0)
        DAT_0051ff20[old] = currentTrack;
    trackCategory = mode;
    if (field_1fc == 4 || mode == 2 || mode == 3) {
        g_cdFadeVolume = cdVolume;
        if (old == 4) {
            if (g_cdFadeTimer >= 0) {
                RemoveTimer(g_cdFadeTimer);
                g_cdFadeTimer = -1;
            }
            if (g_cdNextTrackTimer >= 0) {
                RemoveTimer(g_cdNextTrackTimer);
                g_cdNextTrackTimer = -1;
            }
            ((Class_004d00d0*)this)->SetAuxVolume(cdVolume, 0);
            ((Class_004cdb40*)this)->PlayNextTrack();
        } else {
            if (g_cdFadeTimer >= 0) {
                RemoveTimer(g_cdFadeTimer);
                g_cdFadeTimer = -1;
                RemoveTimer(g_cdNextTrackTimer);
                g_cdNextTrackTimer = -1;
                ((Class_004cdb40*)this)->PlayNextTrack();
            } else {
                field_284 = cdVolume / -18;
                g_cdFadeTimer = AddTimer(2, 0, OnCdFadeTimer);
            }
        }
    }
}

// FUNCTION: 0x4ce7f0
int Sound::GetCurrentTrack()
{
    return currentTrack;
}

// Returns 1 when MCI reports the CD audio device as "playing". A method of
// the object at g_game+0x10 (its one caller, 0x497f40, loads ecx from
// there) that never uses `this`.
// FUNCTION: 0x4ce800
int Sound::IsCdPlaying()
{
    char buf[64];
    if (mciSendStringA("status cdaudio mode", buf, 64, 0) == 0)
        return strcmp(buf, "playing") == 0;
    return 0;
}

// Sends one MCI "play cdaudio" command for the CD player object at +0 of the
// sound class: "status cdaudio mode" first, and if the drive is already
// playing the same track (which is remembered in the field at +0x208) the
// command is skipped and 1 is returned. A seek of 0 stops the CD through
// PlayNextTrack and returns 1. Otherwise the position is offset by the field
// at +0x280, the CD volume is set for the duration, the time format is
// switched to tmsf, "play cdaudio from %i" is built (with " to %i" when the
// position is inside the last track) plus " notify" for the main window, and
// the time format is switched back to milliseconds. Returns whether the
// mciSendStringA of the play command succeeded.
// FUNCTION: 0x4ceb60
int Sound::PlayCdTrack(int index, int flag)
{
    char to[20];
    char status[64];
    char cmd[200];
    int same;
    MCIERROR err;
    HWND hwnd;

    if (field_27c == 0)
        return 1;
    playState = 1;
    if (index == 0) {
        ((Class_004cdb40*)this)->PlayNextTrack();
        return 1;
    }
    // A conditional expression, not &&: keeps the strcmp's two separate exits.
    same = mciSendStringA("status cdaudio mode", status, 0x40, 0) == 0
            ? strcmp(status, "playing") == 0
            : 0;
    if (same && index == currentTrack)
        return 1;
    currentTrack = index;
    index += dataTrack;
    hwnd = GetDisplay()->hwnd;
    ((Class_004d00d0*)this)->SetAuxVolume(cdVolume, 1);
    if (mciSendStringA("set cdaudio time format tmsf", 0, 0, 0) != 0)
        return 0;
    sprintf(cmd, "play cdaudio from %i", index);
    if (index < trackCount) {
        sprintf(to, " to %i", index + 1);
        strcat(cmd, to);
    }
    // " notify" is its own strcat, not an argument of mciSendStringA.
    strcat(cmd, " notify");
    // The result goes through the err local: comparing the call itself changes the epilogue.
    err = mciSendStringA(cmd, 0, 0, hwnd);
    mciSendStringA("set cdaudio time format milliseconds", 0, 0, 0);
    return err == 0;
}

// The constructor of the 0x294-byte sound/volume object (allocated by
// 0x49ea3d); 0x4cff30 opens the devices and 0x4d0040 reads the aux volume.
// FUNCTION: 0x4cee50
Sound::Sound()
{
    field_0 = 0;
    use3D = 0;
    noDriver = 0;
    minDistance = 1.0f;
    maxDistance = 1e20f;
    directSound = 0;
    primary = 0;
    for (int j = 0; j < 8; j++) {
        sets[j] = 0;
    }
    ((Class_004cff30*)this)->InitMixerVolumes();
    maxBuffers = 8;
    count = 0;
    sequence = 1;
    for (int i = 0; i < 32; i++) {
        buffers[i] = 0;
        priority[i] = 0;
        flags[i] = 0;
    }
    stream = 0;
    cdVolume = ((Class_004d0040*)this)->QueryAuxVolume();
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
    ((Class_004ce410*)this)->CloseCdAudio();
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
        IDirectSoundBuffer** set = (IDirectSoundBuffer**)FUN_004d83b0("Digital Audio Sample", 0x10);
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
        IDirectSoundBuffer** set = (IDirectSoundBuffer**)FUN_004d83b0("Digital Audio Sample", 0x10);
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
    FUN_004d85a0(set);
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
// by hand because the toolchain's <dsound.h> is DirectX 3; DAT_004fcf68 is its
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
    if (unit->QueryInterface(DAT_004fcf68, (void**)&chan) == 0) {
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

// FUNCTION: 0x4cff20
int Sound::HasNoDriver()
{
    return noDriver;
}

// FUNCTION: 0x4d0620
void Sound::LoadSample(char* param_1)
{
    ((Class_004d02a0*)this)->OpenSample(param_1, 0, 0, 0);
}

// FUNCTION: 0x4d0640
void Sound::PlaySample(const char* param1, int param2, int param3)
{
    ((Class_004d02a0*)this)->OpenSample(param1, 1, param2, param3);
}

// FUNCTION: 0x4d0660
void Sound::StreamSample(char* a, int b)
{
    ((Class_004d02a0*)this)->OpenSample(a, 2, b, 0);
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
