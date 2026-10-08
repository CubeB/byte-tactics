// Sound: the game's audio system (Thaldren's AudioEngine), the CD audio
// player, the DirectSound sample channels and the streamed sample, at
// g_game+0x10. The one declaration of the class, for cd_audio.cpp,
// samples.cpp and every file that calls it; the types behind the pointers
// stay private to the sound module.
#ifndef SOUND_H
#define SOUND_H

struct IDirectSound;
struct IDirectSoundBuffer;
struct FileHandle;
struct Pos_004cf570;
// HWND__ has no use with STRICT off; the declaration stays because
// frontend/endgame.cpp matches only at this symbol count (docs/c2-regalloc.md).
struct HWND__;

class Sound {
public:
    int open;                          // +0x00
    int use3D;                         // +0x04
    float minDistance;                 // +0x08
    float maxDistance;                 // +0x0c
    int waveDevices;                   // +0x10
    int auxDevice;                     // +0x14
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
    int sampleChannels;                // +0x1c0
    IDirectSoundBuffer** sets[8];      // +0x1c4
    IDirectSoundBuffer* stream;        // +0x1e4
    FileHandle* streamFile;            // +0x1e8, the streamed sample's
    int streamBits;                    // +0x1ec
    int streamSize;                    // +0x1f0
    int streamPos;                     // +0x1f4
    int streamOffset;                  // +0x1f8
    int playbackOrder;                 // +0x1fc
    int trackCount;                    // +0x200
    int lockedTrack;                   // +0x204
    int currentTrack;                  // +0x208
    int playState;                     // +0x20c
    int discSerial;                    // +0x210
    unsigned char arr_214[100];        // +0x214
    int trackCategory;                 // +0x278
    int cdEnabled;                     // +0x27c
    int dataTrack;                     // +0x280, track 1 is not audio
    int step;                          // +0x284
    int streamTimer;                   // +0x288, the stream's timer
    void (*callback)();                // +0x28c
    int noDriver;                      // +0x290

    int SetCdCallback(void (*cb)());
    int OpenCdAudio();
    void CloseCdAudio();
    int IsFirstTrackData();
    int GetTrackCategory();
    void PlayNextTrack();
    void CopyTrackTypeTable(const void* src);
    int GetTrackCount();
    void SetLockedTrack(int v);
    int GetLockedTrack();
    int SetPlaybackOrder(int value);
    void SetCategoryOfTrack(int index, unsigned char value);
    unsigned char GetCategoryOfTrack(int param_1);
    int SelectTrack(int index);
    int PauseCdAudio(int pause);
    int StopCdAudio();
    void EnableCdAudio(int on);
    void InitMixerVolumes();
    int QueryWaveVolume();
    int QueryAuxVolume();
    int SetWaveVolume(int volume);
    int SetAuxVolume(int volume, int temporary);
    void RestoreMixerVolumes();
    int OpenSample(char* path, int mode, int a, int b);
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
    // HWND, DWORD and LONG are spelled out so the header needs no include:
    // windows.h, without STRICT, has HWND as void*, DWORD as unsigned long
    // and LONG as long.
    int InitDirectSound(int rate, int bits, int channels, void* handle);
    void ReapFinishedBuffers();
    void StopAllBuffers();
    void StopOldestBuffer();
    void SetMaxBuffers(int val);
    int GetMaxBuffers();
    IDirectSoundBuffer** CreateSampleFromMemory(void* src, unsigned long bytes, int sampleRate, int bits, int channels);
    IDirectSoundBuffer** CreateSampleFromFile(FileHandle* file, unsigned long bytes, int sampleRate, int bits, int channels);
    void ReleaseSampleSet(IDirectSoundBuffer** set);
    void PlayLooping(IDirectSoundBuffer** set, long volume);
    int PlaySampleSet(IDirectSoundBuffer** set, long volume, Pos_004cf570* pos);
    int PlayMemorySample(void* src, unsigned long bytes, int sampleRate, int bits, int channels, long volume, Pos_004cf570* pos);
    int PlayFileSample(FileHandle* file, unsigned long bytes, int sampleRate, int bits, int channels, long volume, Pos_004cf570* pos);
    void StartStream(FileHandle* file, int sampleRate, int bits, int channels, long volume);
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
    int DetectSampleFormat(void* file);
};

#endif
