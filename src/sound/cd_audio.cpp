// Decompiled by Haiku, deepseek-v4.1-flash, space-bunny-free, Opus, Sonnet, GPT-6.1-sol, mimo-v2.6-pro, Claude Opus 5.5, LongCat 2.5 Preview Free, GPT-6 and DeepSeek V4.1 Flash. Names are provisional.
// The CD audio player: the methods of the sound object at g_game+0x10 that
// talk to the MCI cdaudio device, in address order. The object's sample and
// streaming methods are the samples module (data/modules.csv) and live in
// samples.cpp; the Sound class and the sample-side declarations here are the
// view the gathered files shared.

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

class Class_004cdb40 {
public:
    char unknown_0[0x20];
    int field_20;                      // +0x20
    char unknown_24[0x1fc - 0x24];
    int field_1fc;                     // +0x1fc
    int field_200;                     // +0x200 tracks on the disc
    int field_204;                     // +0x204
    int field_208;                     // +0x208 current track
    int field_20c;                     // +0x20c
    int field_210;                     // +0x210
    unsigned char arr_214[100];        // +0x214
    int field_278;                     // +0x278 mode
    int field_27c;                     // +0x27c
    int field_280;                     // +0x280
    int field_284;                     // +0x284
    char unknown_288[4];
    int field_28c;                     // +0x28c

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

    int GetDiscSerial();
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

class Class_004cd9d0 {
public:
    char unknown_0[0x28c];
    void (*callback)();                // +0x28c

    int SetCdCallback(void (*cb)());
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
    union { int field_284; int step; }; // +0x284
    char unknown_288[4];
    union { int field_28c; void (*callback)(); }; // +0x28c

    int OpenCdAudio();
};

// The CD player object g_cdPlayer points at: the same object as Class_004ce260
// views (the CD audio methods and the CD fields all use it).
extern Class_004ce260* g_cdPlayer;

// OpenCdAudio registers these; 0x4ce1e0 defines FindCdPlayerWindow.
extern void __stdcall SetMediaNotifyCallback(void (__stdcall*)(int, int, int));
extern BOOL __stdcall FindCdPlayerWindow(HWND, LPARAM);

#pragma pack(push, 1)
class Class_004ce3e0 {
public:
    char unknown_0[0x200];
    unsigned int size;                 // +0x200
    char unknown_204[0x215 - 0x204];
    char buf[1];                       // +0x215

    void CopyTrackTypeTable(const void* src);
};
#pragma pack(pop)

class Class_004ce450 {
public:
    int GetTrackCount();
};

class Class_004ce460 {
public:
    int IsFirstTrackData();
};

class Class_004ce580 {
public:
    char unknown_0[0x200];
    int limit;                         // +0x200
    int value;                         // +0x204

    void SetLockedTrack(int v);
};

class Class_004ce5a0 {
public:
    int GetLockedTrack();
};

class Class_004ce680 {
public:
    int GetTrackCategory();
};

struct Class_004ce7a0
{
public:
    char unknown_0[0x1fc];
    int field_1fc;

    int SetPlaybackOrder(int value);
};

class Class_004ce7c0 {
public:
    void SetCategoryOfTrack(int index, unsigned char value);
};

class Class_004ce7e0 {
public:
    char unknown_0[0x214];
    unsigned char field_214;

    unsigned char GetCategoryOfTrack(int param_1);
};

class Class_004ce8c0 {
public:
    char unknown_0[0x200];
    int count;          // +0x200
    char unknown_204[4];
    int current;        // +0x208
    int mode;           // +0x20c
    int SelectTrack(int index);
};

class Class_004ce910 {
public:
    int unknown_0[0x200 / 4];
    int field_200;                     // +0x200
    int unknown_204[2];
    int field_20c;                     // +0x20c
    int unknown_210[(0x27c - 0x210) / 4];
    int field_27c;                     // +0x27c

    int PauseCdAudio(int pause);
};

class Class_004ced40 {
public:
    char unknown_0[0x200];
    int unknown_200;                   // +0x200
    char unknown_204[0x208 - 0x204];
    int unknown_208;                   // +0x208
    int unknown_20c;                   // +0x20c
    char unknown_210[0x284 - 0x210];
    int unknown_284;                   // +0x284

    int StopCdAudio();
};

class Class_004cedc0 {
public:
    char unknown_0[0x200];
    int unknown_200;                   // +0x200
    char unknown_204[0x208 - 0x204];
    int unknown_208;                   // +0x208
    int unknown_20c;                   // +0x20c
    char unknown_210[0x27c - 0x210];
    int enabled;                       // +0x27c
    char unknown_280[0x284 - 0x280];
    int unknown_284;                   // +0x284

    void EnableCdAudio(int on);
};

// FUNCTION: 0x4cd9b0
void __stdcall NopRet4(int)
{
}

// FUNCTION: 0x4cd9c0
int Sound::GetDiscSerial()
{
    return discSerial;
}

// FUNCTION: 0x4cd9d0
int Class_004cd9d0::SetCdCallback(void (*cb)())
{
    callback = cb;
    ((Sound*)this)->QueryDisc();
    if (callback != 0)
        callback();
    return 1;
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

// FUNCTION: 0x4cdb40
void Class_004cdb40::PlayNextTrack()
{
    char buf[64];
    int playing;
    int r;
    int count;
    int i;
    int j;
    int zero = 0;
    int res;

    if (field_200 == zero)
        return;
    if (field_278 == 4) {
        mciSendStringA("stop cdaudio", (LPSTR)zero, 0, (HWND)zero);
        if (field_200 != zero)
            field_208 = 1;
        else
            field_208 = zero;
        field_20c = zero;
        field_284 = zero;
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        // Chained store, as in the original: g_cdNextTrackTimer is written first.
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
        return;
    }
    if (field_20c == 2)
        return;
    if (field_278 != 2 && field_278 != 3) {
        int one = 1;
        switch (field_1fc) {
        case 0:
            {
                int none = 0;
                if (field_20c == zero)
                    return;
                field_20c = zero;
                // Result kept in a local so the call is not folded into test eax,eax.
                res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
                playing = res == none ? strcmp(buf, "playing") == none : none;
                if (playing == none)
                    return;
                mciSendStringA("stop cdaudio", (LPSTR)none, 0, (HWND)none);
                if (field_200 != none)
                    field_208 = 1;
                else
                    field_208 = none;
                field_20c = none;
                field_284 = none;
                RemoveTimer(g_cdNextTrackTimer);
                RemoveTimer(g_cdFadeTimer);
                g_cdNextTrackTimer = g_cdFadeTimer = -1;
                return;
            }
        case 1:
            {
            res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
            playing = res == zero ? strcmp(buf, "playing") == zero : zero;
            if (playing != zero)
                goto done;
            if (field_208 < one)
                field_208 = one;
            else
                field_208++;
            ((Sound*)this)->PlayCdTrack(field_208, field_200 - field_208 + 1);
            if (field_208 > field_200)
                field_208 = one;
            goto done;
            }
        case 2:
            {
            res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
            playing = res == zero ? strcmp(buf, "playing") == zero : zero;
            if (playing != zero)
                goto done;
            ((Sound*)this)->PlayCdTrack(rand() % field_200 + 1, one);
            goto done;
            }
        case 3:
            {
            res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
            playing = res == zero ? strcmp(buf, "playing") == zero : zero;            if (playing == zero || field_208 != field_204) {
                if (field_204 == zero)
                    field_204 = one;
                ((Sound*)this)->PlayCdTrack(field_204, one);
            }
            goto done;
            }
        case 4:
            break;
        default:
            goto done;
        }
    }
    r = rand() & 0xf;
    res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
    playing = res == zero ? strcmp(buf, "playing") == 0 : 0;
    if (playing != 0 && arr_214[field_208] == field_278)
        goto done;
    count = (r + 1) * field_200;
    i = field_208;
    while (count > 0) {
            i++;
            if (i > field_200)
                i = 1;
            if (arr_214[i] == field_278) {
                if (--r <= 0) {
                    j = i;
                    while (j <= field_200 && arr_214[j] == field_278)
                        j++;
                    ((Sound*)this)->PlayCdTrack(i, j - i);
                    break;
                }
            }
            count--;
    }
    if (count > 0)
        goto done;
stop:
    mciSendStringA("stop cdaudio", (LPSTR)zero, 0, (HWND)zero);
    field_20c = 0;
    field_208 = (field_200 != 0);
    field_284 = 0;
    RemoveTimer(g_cdNextTrackTimer);
    RemoveTimer(g_cdFadeTimer);
    g_cdNextTrackTimer = g_cdFadeTimer = -1;
done:
    ((Class_004d00d0*)this)->SetAuxVolume(field_20, 1);
    field_20c = 1;
    return;
}

// FUNCTION: 0x4ce020
int Sound::GetPlayState()
{
    return playState;
}

// FUNCTION: 0x4ce030
void __cdecl HandleCdMessage(int param_1, int param_2, int param_3)
{
    char buf[64];

    switch (param_1) {
    case 0x219: {
        Class_004ce260* obj = g_cdPlayer;
        mciSendStringA("stop cdaudio", 0, 0, 0);
        if (obj->field_200)
            obj->field_208 = 1;
        else
            obj->field_208 = 0;
        obj->field_20c = 0;
        obj->field_284 = 0;
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
        if (param_2 == 0x8000) {
            ((Sound*)g_cdPlayer)->QueryDisc();
            if (g_cdPlayer->callback)
                g_cdPlayer->callback();
        }
        break;
    }
    case 0x3b9:
        if (param_2 == 1 && g_cdPlayer->field_20c == 1) {
            int playing;
            if (mciSendStringA("status cdaudio mode", buf, 0x40, 0) == 0)
                playing = strcmp(buf, "playing") == 0;
            else
                playing = 0;
            if (!playing)
                ((Class_004cdb40*)g_cdPlayer)->PlayNextTrack();
        }
        break;
    }
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

// EnumWindows callback: remembers the window whose class is the CD player's.
// FUNCTION: 0x4ce1e0
BOOL __stdcall FindCdPlayerWindow(HWND hwnd, LPARAM param)
{
    char className[200];
    GetClassNameA(hwnd, className, sizeof(className) - 1);
    if (strcmp(className, "SJE_CdPlayerClass") == 0)
        g_cdPlayerWindow = hwnd;
    return TRUE;
}

// Initialises the CD player object and opens the MCI cdaudio device.
// The store to arr_214[0] before the loop is overwritten by the loop's first
// iteration (0 % 4 + 1 == 1), so it is redundant in the original.
// FUNCTION: 0x4ce260
int Class_004ce260::OpenCdAudio()
{
    // Results go through hr: comparing the calls directly changes the test emitted.
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
    field_200 = ((Sound*)this)->QueryDisc();
    SetMediaNotifyCallback((void (__stdcall*)(int, int, int))HandleCdMessage);
    open = 1;
    field_204 = 1;
    field_208 = 0;
    field_28c = 0;
    field_1fc = 1;
    field_278 = 0;
    field_27c = 1;
    return 1;
}

// FUNCTION: 0x4ce3e0
void Class_004ce3e0::CopyTrackTypeTable(const void* src)
{
    memcpy(buf, src, size);
}

// ReleaseDirectSound (0x4ceee0) calls this out of line; in one file /Ob2
// would inline it into its caller.
#pragma auto_inline(off)
// FUNCTION: 0x4ce410
void Class_004ce410::CloseCdAudio()
{
    if (open != 0) {
        mciSendStringA("stop cdaudio", 0, 0, 0);
        mciSendStringA("close cdaudio", 0, 0, 0);
        open = 0;
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x4ce450
int Class_004ce450::GetTrackCount()
{
    return *(int*)((char*)this + 0x200);
}

// FUNCTION: 0x4ce460
int Class_004ce460::IsFirstTrackData()
{
    int type;
    char buf[32];
    mciSendStringA("set cdaudio time format tmsf", 0, 0, 0);
    if (mciSendStringA("status cdaudio type track 1", buf, 0x20, 0) != 0)
        return 1;
    if (strcmp(buf, "audio") == 0) {
        return 0;
    } else {
        // Keep the type == 0 test even though both branches return 1.
        type = strcmp(buf, "other");
        if (type == 0) {
            return 1;
        }
        return 1;
    }
}

// FUNCTION: 0x4ce530
int __stdcall GetTrackLength(int track)
{
    char buf[32];
    char cmd[64];
    sprintf(cmd, "status cdaudio length track %i", track);
    if (mciSendStringA(cmd, buf, 0x20, 0) == 0)
        return atoi(buf);
    return 0;
}

// FUNCTION: 0x4ce580
void Class_004ce580::SetLockedTrack(int v)
{
    if (v <= limit) {
        value = v;
    }
}

// FUNCTION: 0x4ce5a0
int Class_004ce5a0::GetLockedTrack()
{
    return *(int*)((char*)this + 0x204);
}

// FUNCTION: 0x4ce5b0
void __stdcall OnNextTrackTimer(void*)
{
    int temp = g_cdNextTrackTimer;
    RemoveTimer(temp);
    g_cdNextTrackTimer = 0xffffffff;
    ((Class_004cdb40*)g_cdPlayer)->PlayNextTrack();
}

// Timer callback: steps the level by the object's step; once it reaches zero
// the timer is killed and either a new one is started or PlayNextTrack runs.
// FUNCTION: 0x4ce5e0
void __stdcall OnCdFadeTimer(void*)
{
    g_cdFadeVolume += g_cdPlayer->step;
    if (g_cdFadeVolume <= 0) {
        RemoveTimer(g_cdFadeTimer);
        g_cdFadeTimer = -1;
        g_cdFadeVolume = 0;
        g_cdPlayer->step = 0;
        ((Class_004d00d0*)g_cdPlayer)->SetAuxVolume(g_cdFadeVolume, 1);
        if (g_cdPlayer->field_278 == 0)
            g_cdNextTrackTimer = AddTimer(0x78, 0, OnNextTrackTimer);
        else
            ((Class_004cdb40*)g_cdPlayer)->PlayNextTrack();
    } else {
        ((Class_004d00d0*)g_cdPlayer)->SetAuxVolume(g_cdFadeVolume, 1);
    }
}

// FUNCTION: 0x4ce680
int Class_004ce680::GetTrackCategory()
{
    return *(int*)((char*)this + 0x278);
}

// FUNCTION: 0x4ce690
void Sound::SetTrackCategory(int mode)
{
    int old = trackCategory;
    if (old == mode)
        return;
    if (old >= 0)
        g_cdCategorySavedTrack[old] = currentTrack;
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

// FUNCTION: 0x4ce7a0
int Class_004ce7a0::SetPlaybackOrder(int value)
{
    field_1fc = value;
    return 1;
}

// FUNCTION: 0x4ce7c0
void Class_004ce7c0::SetCategoryOfTrack(int index, unsigned char value)
{
    *(unsigned char*)((char*)this + 0x214 + index) = value;
}

// FUNCTION: 0x4ce7e0
unsigned char Class_004ce7e0::GetCategoryOfTrack(int param_1)
{
    return *(unsigned char*)((char*)this + param_1 + 0x214);
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

// FUNCTION: 0x4ce880
int GetCdPosition()
{
    char buf[32];
    if (mciSendStringA("status cdaudio position", buf, 32, 0) == 0)
        return atoi(buf);
    return 0;
}

// FUNCTION: 0x4ce8c0
int Class_004ce8c0::SelectTrack(int index)
{
    if (count == 0)
        return 0;
    if (index > count)
        index = index % count;
    if (mode == 1) {
        ((Sound*)this)->PlayCdTrack(index, 1);
        return current;
    }
    current = index;
    return index;
}

// FUNCTION: 0x4ce910
int Class_004ce910::PauseCdAudio(int pause)
{
    char cmd[100];
    char buf[200];
    char ret[200];
    int track;

    if (field_27c == 0)
        return 1;
    if (field_20c == 0)
        return 1;

    HWND hwnd = GetDisplay()->hwnd;

    if (pause == 0) {
        sprintf(cmd, "status cdaudio current track");
        mciSendStringA(cmd, ret, 200, hwnd);
        track = atoi(ret);
        sprintf(buf, "play cdaudio");
        if (track < field_200) {
            strcat(buf, " from ");
            sprintf(cmd, "status cdaudio position");
            mciSendStringA(cmd, ret, 200, hwnd);
            strcat(buf, ret);
            sprintf(cmd, "status cdaudio position track %i", track + 1);
            mciSendStringA(cmd, ret, 200, hwnd);
            strcat(buf, " to ");
            strcat(buf, ret);
        }
        strcat(buf, " notify");
        field_20c = 1;
    } else {
        sprintf(buf, "pause cdaudio");
        field_20c = 2;
    }

    // The result goes through the err local: comparing the call itself changes the epilogue.
    MCIERROR err = mciSendStringA(buf, 0, 0, hwnd);
    return err == 0;
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

// Stops CD audio playback; returns 1 when the MCI command succeeded.
// FUNCTION: 0x4ced40
int Class_004ced40::StopCdAudio()
{
    MCIERROR err = mciSendStringA("stop cdaudio", 0, 0, 0);
    if (unknown_200)
        unknown_208 = 1;
    else
        unknown_208 = 0;
    unknown_20c = 0;
    unknown_284 = 0;
    RemoveTimer(g_cdNextTrackTimer);
    RemoveTimer(g_cdFadeTimer);
    g_cdNextTrackTimer = g_cdFadeTimer = -1;
    return err == 0 ? 1 : 0;
}

// FUNCTION: 0x4cedc0
void Class_004cedc0::EnableCdAudio(int on)
{
    enabled = on;
    if (on == 0) {
        mciSendStringA("stop cdaudio", 0, 0, 0);
        if (unknown_200)
            unknown_208 = 1;
        else
            unknown_208 = 0;
        unknown_20c = 0;
        unknown_284 = 0;
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
    }
}

// FUNCTION: 0x4cee40
void __stdcall NopRet4_B(int)
{
}
