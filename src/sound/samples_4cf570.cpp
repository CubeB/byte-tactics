// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.

// Plays a sample on one of the four DirectSound buffers in `set`: a buffer that
// has stopped is reused as it is; otherwise a free entry gets a duplicate of
// set[0], or else the buffer furthest through its playback is restarted. The
// buffer is then given its 3D settings, volume and play flags and filed in the
// channel table (count +0x30, sequence +0x34, buffers +0x38, priorities +0xb8,
// looping flags +0x138, IDirectSound +0x24). IDirectSound3DBuffer is declared
// by hand because the toolchain's <dsound.h> is DirectX 3; DAT_004fcf68 is its
// IID.
//
// #5115 (Claude Opus 5.5): MATCH. The last difference was C2's register
// allocation (docs/c2-regalloc.md): the original gives ebp, the fourth
// callee-saved register, to bestidx (sharing it with the zero constant) and
// splits `this` around the scan loop. Priorities read out of C2.EXE under gdb
// for the earlier plain-loop source: this 8, bestidx -50 (bestidx set at the
// top), so `this` took ebp and bestidx stayed in memory. Two changes that
// leave the bytes alone reverse the order: `bestidx = 0` after the null test
// (bestidx -7, no longer live through the earlier blocks) and the channel
// table loop ending in `break` with one `return 1` after it, instead of a
// `return 1` inside the loop (this -11). `best = 0` stays before the null
// test (its `xor ebx, ebx` comes before the cmp), and bestidx must be the
// last variable set to 0, or the zero constant shares best's register
// instead. The do-while of #4337 got the same order by pushing `this` to -104
// and bestidx to -78 through the doubled loop weight, at the cost of a test.
#include <windows.h>
#include <dsound.h>

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

class Sound {
public:
    int field_0;
    int field_4;
    float field_8;
    float field_c;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int field_20;
    IDirectSound* field_24;
    int field_28;
    int field_2c;
    int count;                              // +0x30
    int field_34;
    IDirectSoundBuffer* buffers[0x20];      // +0x38
    int priority[0x20];                     // +0xb8
    int flags[0x20];                        // +0x138

    int PlaySampleSet(IDirectSoundBuffer** set, LONG volume, Pos_004cf570* pos);
    void StopOldestBuffer();
};

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
    while (count >= field_2c)
        ((Sound*)this)->StopOldestBuffer();
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
            if (field_24->DuplicateSoundBuffer(set[0], &unit) != 0)
                return 0;
            set[slot] = unit;
        } else {
            unit = set[bestidx];
            unit->SetCurrentPosition(0);
        }
    }
    IDirectSound3DBuffer* chan;
    if (unit->QueryInterface(DAT_004fcf68, (void**)&chan) == 0) {
        if (field_4 == 0 || pos == 0) {
            chan->SetMode(2, 0);
        } else {
            chan->SetPosition((float)pos->x, (float)pos->y, (float)pos->z, 0);
            chan->SetMinDistance(field_8, 0);
            chan->SetMaxDistance(field_c, 0);
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
            priority[j] = ++field_34;
            flags[j] = g_playBufferLooping != 0;
            count++;
            break;
        }
    }
    return 1;
}
