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

extern int g_delayedSampleVolume;
extern char g_delayedSampleName[];

int __stdcall HAPI_SeekFile(void* file, int pos);
int __stdcall HAPI_readfromfile(void* file, void* buf, int size);

// The sound system: the CD audio player, the DirectSound sample channels and
// the streamed sample, at g_game+0x10.
#include "sound.h"

// The sound object g_cdPlayer points at: the same object as g_game+0x10.
extern Sound* g_cdPlayer;

// OpenCdAudio registers these; 0x4ce1e0 defines FindCdPlayerWindow.
extern void __stdcall SetMediaNotifyCallback(void (__stdcall*)(int, int, int));
extern BOOL __stdcall FindCdPlayerWindow(HWND, LPARAM);

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
int Sound::SetCdCallback(void (*cb)())
{
    callback = cb;
    QueryDisc();
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
void Sound::PlayNextTrack()
{
    char buf[64];
    int playing;
    int r;
    int count;
    int i;
    int j;
    int zero = 0;
    int res;

    if (trackCount == zero)
        return;
    if (trackCategory == 4) {
        mciSendStringA("stop cdaudio", (LPSTR)zero, 0, (HWND)zero);
        if (trackCount != zero)
            currentTrack = 1;
        else
            currentTrack = zero;
        playState = zero;
        step = zero;
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        // Chained store, as in the original: g_cdNextTrackTimer is written first.
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
        return;
    }
    if (playState == 2)
        return;
    if (trackCategory != 2 && trackCategory != 3) {
        int one = 1;
        switch (playbackOrder) {
        case 0:
            {
                int none = 0;
                if (playState == zero)
                    return;
                playState = zero;
                // Result kept in a local so the call is not folded into test eax,eax.
                res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
                playing = res == none ? strcmp(buf, "playing") == none : none;
                if (playing == none)
                    return;
                mciSendStringA("stop cdaudio", (LPSTR)none, 0, (HWND)none);
                if (trackCount != none)
                    currentTrack = 1;
                else
                    currentTrack = none;
                playState = none;
                step = none;
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
            if (currentTrack < one)
                currentTrack = one;
            else
                currentTrack++;
            this->PlayCdTrack(currentTrack, trackCount - currentTrack + 1);
            if (currentTrack > trackCount)
                currentTrack = one;
            goto done;
            }
        case 2:
            {
            res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
            playing = res == zero ? strcmp(buf, "playing") == zero : zero;
            if (playing != zero)
                goto done;
            this->PlayCdTrack(rand() % trackCount + 1, one);
            goto done;
            }
        case 3:
            {
            res = mciSendStringA("status cdaudio mode", buf, 0x40, (HWND)zero);
            playing = res == zero ? strcmp(buf, "playing") == zero : zero;            if (playing == zero || currentTrack != lockedTrack) {
                if (lockedTrack == zero)
                    lockedTrack = one;
                this->PlayCdTrack(lockedTrack, one);
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
    if (playing != 0 && arr_214[currentTrack] == trackCategory)
        goto done;
    count = (r + 1) * trackCount;
    i = currentTrack;
    while (count > 0) {
            i++;
            if (i > trackCount)
                i = 1;
            if (arr_214[i] == trackCategory) {
                if (--r <= 0) {
                    j = i;
                    while (j <= trackCount && arr_214[j] == trackCategory)
                        j++;
                    this->PlayCdTrack(i, j - i);
                    break;
                }
            }
            count--;
    }
    if (count > 0)
        goto done;
stop:
    mciSendStringA("stop cdaudio", (LPSTR)zero, 0, (HWND)zero);
    playState = 0;
    currentTrack = (trackCount != 0);
    step = 0;
    RemoveTimer(g_cdNextTrackTimer);
    RemoveTimer(g_cdFadeTimer);
    g_cdNextTrackTimer = g_cdFadeTimer = -1;
done:
    SetAuxVolume(cdVolume, 1);
    playState = 1;
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
        Sound* obj = g_cdPlayer;
        mciSendStringA("stop cdaudio", 0, 0, 0);
        if (obj->trackCount)
            obj->currentTrack = 1;
        else
            obj->currentTrack = 0;
        obj->playState = 0;
        obj->step = 0;
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
        if (param_2 == 0x8000) {
            g_cdPlayer->QueryDisc();
            if (g_cdPlayer->callback)
                g_cdPlayer->callback();
        }
        break;
    }
    case 0x3b9:
        if (param_2 == 1 && g_cdPlayer->playState == 1) {
            int playing;
            if (mciSendStringA("status cdaudio mode", buf, 0x40, 0) == 0)
                playing = strcmp(buf, "playing") == 0;
            else
                playing = 0;
            if (!playing)
                g_cdPlayer->PlayNextTrack();
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
int Sound::OpenCdAudio()
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
    lockedTrack = 1;
    currentTrack = 0;
    discSerial = 0;
    callback = 0;
    playbackOrder = 1;
    trackCategory = 0;
    open = 0;
    trackCount = 0;
    dataTrack = 0;
    hr = mciSendStringA("open cdaudio", 0, 0, 0);
    if (hr != 0) {
        EnumWindows((WNDENUMPROC)FindCdPlayerWindow, 0);
        hr = mciSendStringA("open cdaudio", 0, 0, 0);
        if (hr != 0)
            return 0;
    }
    mciSendStringA("stop cdaudio", 0, 0, 0);
    playState = 0;
    currentTrack = (trackCount != 0);
    step = 0;
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
    discSerial = 0;
    trackCount = QueryDisc();
    SetMediaNotifyCallback((void (__stdcall*)(int, int, int))HandleCdMessage);
    open = 1;
    lockedTrack = 1;
    currentTrack = 0;
    callback = 0;
    playbackOrder = 1;
    trackCategory = 0;
    cdEnabled = 1;
    return 1;
}

// FUNCTION: 0x4ce3e0
void Sound::CopyTrackTypeTable(const void* src)
{
    memcpy(arr_214 + 1, src, trackCount);
}

// ReleaseDirectSound (0x4ceee0) calls this out of line; in one file /Ob2
// would inline it into its caller.
#pragma auto_inline(off)
// FUNCTION: 0x4ce410
void Sound::CloseCdAudio()
{
    if (open != 0) {
        mciSendStringA("stop cdaudio", 0, 0, 0);
        mciSendStringA("close cdaudio", 0, 0, 0);
        open = 0;
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x4ce450
int Sound::GetTrackCount()
{
    return trackCount;
}

// FUNCTION: 0x4ce460
int Sound::IsFirstTrackData()
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
void Sound::SetLockedTrack(int v)
{
    if (v <= trackCount) {
        lockedTrack = v;
    }
}

// FUNCTION: 0x4ce5a0
int Sound::GetLockedTrack()
{
    return this->lockedTrack;
}

// FUNCTION: 0x4ce5b0
void __stdcall OnNextTrackTimer(void*)
{
    int temp = g_cdNextTrackTimer;
    RemoveTimer(temp);
    g_cdNextTrackTimer = 0xffffffff;
    g_cdPlayer->PlayNextTrack();
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
        g_cdPlayer->SetAuxVolume(g_cdFadeVolume, 1);
        if (g_cdPlayer->trackCategory == 0)
            g_cdNextTrackTimer = AddTimer(0x78, 0, OnNextTrackTimer);
        else
            g_cdPlayer->PlayNextTrack();
    } else {
        g_cdPlayer->SetAuxVolume(g_cdFadeVolume, 1);
    }
}

// FUNCTION: 0x4ce680
int Sound::GetTrackCategory()
{
    return this->trackCategory;
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
    if (playbackOrder == 4 || mode == 2 || mode == 3) {
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
            SetAuxVolume(cdVolume, 0);
            PlayNextTrack();
        } else {
            if (g_cdFadeTimer >= 0) {
                RemoveTimer(g_cdFadeTimer);
                g_cdFadeTimer = -1;
                RemoveTimer(g_cdNextTrackTimer);
                g_cdNextTrackTimer = -1;
                PlayNextTrack();
            } else {
                step = cdVolume / -18;
                g_cdFadeTimer = AddTimer(2, 0, OnCdFadeTimer);
            }
        }
    }
}

// FUNCTION: 0x4ce7a0
int Sound::SetPlaybackOrder(int value)
{
    playbackOrder = value;
    return 1;
}

// FUNCTION: 0x4ce7c0
void Sound::SetCategoryOfTrack(int index, unsigned char value)
{
    arr_214[index] = value;
}

// FUNCTION: 0x4ce7e0
unsigned char Sound::GetCategoryOfTrack(int param_1)
{
    return arr_214[param_1];
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
int Sound::SelectTrack(int index)
{
    if (trackCount == 0)
        return 0;
    if (index > trackCount)
        index = index % trackCount;
    if (playState == 1) {
        PlayCdTrack(index, 1);
        return currentTrack;
    }
    currentTrack = index;
    return index;
}

// FUNCTION: 0x4ce910
int Sound::PauseCdAudio(int pause)
{
    char cmd[100];
    char buf[200];
    char ret[200];
    int track;

    if (cdEnabled == 0)
        return 1;
    if (playState == 0)
        return 1;

    HWND hwnd = GetDisplay()->hwnd;

    if (pause == 0) {
        sprintf(cmd, "status cdaudio current track");
        mciSendStringA(cmd, ret, 200, hwnd);
        track = atoi(ret);
        sprintf(buf, "play cdaudio");
        if (track < trackCount) {
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
        playState = 1;
    } else {
        sprintf(buf, "pause cdaudio");
        playState = 2;
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

    if (cdEnabled == 0)
        return 1;
    playState = 1;
    if (index == 0) {
        PlayNextTrack();
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
    SetAuxVolume(cdVolume, 1);
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
int Sound::StopCdAudio()
{
    MCIERROR err = mciSendStringA("stop cdaudio", 0, 0, 0);
    if (trackCount)
        currentTrack = 1;
    else
        currentTrack = 0;
    playState = 0;
    step = 0;
    RemoveTimer(g_cdNextTrackTimer);
    RemoveTimer(g_cdFadeTimer);
    g_cdNextTrackTimer = g_cdFadeTimer = -1;
    return err == 0 ? 1 : 0;
}

// FUNCTION: 0x4cedc0
void Sound::EnableCdAudio(int on)
{
    cdEnabled = on;
    if (on == 0) {
        mciSendStringA("stop cdaudio", 0, 0, 0);
        if (trackCount)
            currentTrack = 1;
        else
            currentTrack = 0;
        playState = 0;
        step = 0;
        RemoveTimer(g_cdNextTrackTimer);
        RemoveTimer(g_cdFadeTimer);
        g_cdNextTrackTimer = g_cdFadeTimer = -1;
    }
}

// FUNCTION: 0x4cee40
void __stdcall NopRet4_B(int)
{
}
