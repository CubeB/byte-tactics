// Decompiled by Space Bunny Free, space-bunny-free, deepseek-v4.1-flash, Claude Sonnet 5.5, GPT-6.1-sol, Haiku, Opus, Sonnet, DeepSeek V4.1 Flash and deepseek-v4.1. Names are provisional.
// Sound start-up and shutdown, the sound-name tables and the queue of unit speech.

#include <windows.h>
#include <dsound.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Sound {
public:
    int InitDirectSound(int rate, int bits, int channels, int handle);
    int HasNoDriver();
    void ReleaseDirectSound();
    void* LoadSample(char* path);
    void ReleaseSampleSet(IDirectSoundBuffer** set);
    int StreamSampleDelayed(char* name, int value, int delay);
    int PlayLooping(int sample, int volume);
    int PlaySampleSet(int a, int b, int c);
    int PlaySampleSet(int a, int b, void* c);
    int PlaySample(const char* name, int volume, int pos);
    int Is3DEnabled();
    void Set3DDistances(float a, float b);
    void StopAllBuffers();
    void OpenCdAudio();
    void RestoreMixerVolumes();
    // Unused here: the symbol ids this declaration takes keep PlaySoundAt's allocation (docs/c2-regalloc.md).
    void SetMaxBuffers(int count);
};

struct SoundParams_0047ed40 {
    char unknown_0[0x40];
    int field_40;
};

union Pos_0047f300 {
    struct {
        short xFrac;                   // +0x0
        short x;                       // +0x2
        short yFrac;                   // +0x4
        short y;                       // +0x6
        short zFrac;                   // +0x8
        short z;                       // +0xa
    };
    struct {
        int xVal;                      // +0x0
        int yVal;                      // +0x4
        int zVal;                      // +0x8
    };
};

struct Vector3_0047f300 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Packet_0047f0c0 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int index;                         // +0x2
    int unknown_6[3];                  // +0x6
};

struct Packet_0047f300 {
    unsigned char type;                // +0x0
    unsigned char flag;                // +0x1
    int index;                         // +0x2
    Pos_0047f300 pos;                  // +0x6
};

struct Unit {
    char unknown_0[0x92];
    char* name;                        // +0x92
    char unknown_96[0xa8 - 0x96];
    unsigned short id;           // +0xa8
    char unknown_aa[0xff - 0xaa];
    unsigned char owner;               // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
};

struct Slot_0047fd70 {
    int count;                         // +0x0
    char* table1;                      // +0x4
    char* table2;                      // +0x8
};

struct SoundCat_0047fd70 {
    char unknown_0[0x40];
    Slot_0047fd70 slots[24];           // +0x40
};

struct Player_0047f300 {
    char unknown_0[0x7c];
    unsigned char* explored;           // +0x7c
    unsigned int exploredWidth;        // +0x80
    unsigned int exploredHeight;       // +0x84
    char unknown_88[0x14b - 0x88];
};

struct Game {
    char unknown_0[0xc];
    SoundParams_0047ed40* displayContext;  // +0xc
    Sound* sound;                      // +0x10
    char unknown_14[0x1b63 - 0x14];
    Player_0047f300 players[10];       // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14233 - 0x2a44];
    int width;                         // +0x14233
    int height;                        // +0x14237
    int screenTilesX;                  // +0x1423b
    int screenTilesY;                  // +0x1423f
    char unknown_14243[0x14273 - 0x14243];
    unsigned short* visibilityMask;    // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned short mapFlags;           // +0x14281
    char unknown_14283[0x1431f - 0x14283];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x33a0f - 0x14327];
    int soundCount;                    // +0x33a0f
    int soundIds[256];                 // +0x33a13
    char soundNames[512][0x20];        // +0x33e13
    SoundCat_0047fd70* categories;     // +0x37e13
    char unknown_37e17[0x37f0c - 0x37e17];
    int volume1;                       // +0x37f0c
    char unknown_37f10[0x37f17 - 0x37f10];
    unsigned char unitChat;            // +0x37f17
    unsigned char unitChatText;        // +0x37f18
    unsigned char flags_37f19;         // +0x37f19
    char unknown_37f1a[0x38a47 - 0x37f1a];
    unsigned int frame;                // +0x38a47

    Player_0047f300* Current() { return &players[playerIndex]; }
    Player_0047f300* Current2() { int i = playerIndex; return &players[i]; }
};

// One queued speech (0x11 bytes).
struct SpeechEntry {
    int kind;                          // +0x0
    int frame;                         // +0x4, when it was queued
    Unit* unit;                        // +0x8
    char* data;                        // +0xc
    unsigned char priority;            // +0x10
};

void __cdecl GameFreeThunk(void* data);

class SpeechQueue {
public:
    SpeechEntry entries[9];            // +0x00
    int count;                         // +0x99
    unsigned int lastFrame;            // +0x9d, when the last speech played
    unsigned int interval;             // +0xa1
    int field_a5;                      // +0xa5

    void Remove(int i)
    {
        if (entries[i].data) {
            GameFreeThunk(entries[i].data);
            entries[i].data = 0;
        }
        for (int j = i; j < count; j++)
            entries[j] = entries[j + 1];
        count--;
    }

    SpeechQueue(int param_1, int param_2);
    void ClearSpeechList(void);
    void ClearSpeechEntries(void);
    void EnqueueSpeech(Unit* unit, int kind, char* text);
    void PlayNextSpeechEntry();
    void PlaySpeech(int index, int param_2, int param_3);
    void RemoveSpeechAt(int index);
    void RemoveSpeechOfId(Unit* unit);
};

// One of the 24 sound messages: priority, repeat delay, text and the frame it is next allowed.
struct Message_0047fad0 {
    int priority;                      // +0x0
    int cooldown;                      // +0x4
    int unknown_8;
    char* text;                        // +0xc
    unsigned int minFrame;             // +0x10
    int unknown_14;
};
#pragma pack(pop)

extern Game* g_game;
extern SpeechQueue* g_speechQueue;
extern Message_0047fad0 g_speechTypes[24];
extern int g_noDirectSound;            // NoDirectSound
extern int g_useWindowsSound;          // UseWindowsSound
extern int g_playLooping;
extern int DAT_0051e698;

extern char g_noDirectSoundKey[]; // "NoDirectSound"
extern char g_useWindowsSoundKey[]; // "UseWindowsSound"
extern char g_soundInitError[]; // "Error:  Sound system initialization failed."

unsigned int __stdcall GetPreferenceInt(char* key, int defaultValue);
int IsWindowsSoundAvailable(void);
void __stdcall FatalError(char* text);
void* __cdecl operator new(size_t size);
void __cdecl operator delete(void* p);
void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
char* __stdcall BuildDataPath(char* buf, const char* dir, const char* name, const char* ext);
void* __stdcall HAPI_LoadFile(char* path, int flags);
void __stdcall AddMessage(char* text, unsigned char key, unsigned short value, char last);
BOOL __stdcall PlayWavFromDisk(char* path);
int __stdcall PlayWavMemory(int sound);
int __stdcall PlayLoopingWavMemory(int sound);
void ResumeLoopingWav();
void StopWindowsSound();
int __cdecl GetLocalDpid();
int __stdcall BroadcastPacket(int player, void* data, int size);
struct Cell_0047f300;
Cell_0047f300* __stdcall GetMapCell(int x, int y);
char* __stdcall Translate(char* text);
int __stdcall IsUnitVisible(Unit* unit);

// Defined ahead of InitSound so that its allocation inlines the constructor.
// FUNCTION: 0x47f960
SpeechQueue::SpeechQueue(int param_1, int param_2)
{
    count = 0;
    lastFrame = 0;
    interval = param_1;
    field_a5 = param_2;
}

// The direct sound buffer's result goes into a local: comparing the call
// directly with 0 gives `test eax, eax`, the original compares with the
// zero register instead.
// FUNCTION: 0x47ed40
void InitSound(void)
{
    if (GetPreferenceInt(g_noDirectSoundKey, 0))
        g_noDirectSound = 1;
    if (GetPreferenceInt(g_useWindowsSoundKey, 0))
        g_useWindowsSound = 1;
    if (g_useWindowsSound) {
        if (!IsWindowsSoundAvailable())
            g_useWindowsSound = 0;
        g_noDirectSound = 1;
    }
    if (!g_noDirectSound) {
        int hr = g_game->sound->InitDirectSound(0x2b11, 0x10, 2, g_game->displayContext->field_40);
        if (hr == 0) {
            if (g_game->sound->HasNoDriver())
                g_noDirectSound = 1;
            else
                FatalError(g_soundInitError);
        }
    }
    g_game->sound->OpenCdAudio();
    g_speechQueue = new SpeechQueue(0x1e, 0x96);
}

// FUNCTION: 0x47ee30
void ResetSpeech()
{
    SpeechQueue* list = g_speechQueue;
    if (list->count > 0) {
        int* count = &list->count;
        do {
            int i = *count - 1;
            // Entries reached through list->entries, no separate entries local.
            SpeechEntry* last = &list->entries[i];
            if (last->data) {
                GameFreeThunk(last->data);
                last->data = 0;
            }
            for (int j = i; j < *count; j++) {
                // Re-taken at the top of the body: this is what stops MSVC 5
                // folding &list->count into [ebp+0x99], and because the
                // assignment is loop invariant the lea is hoisted into the
                // shift loop's preheader, which is where the original has it.
                count = &list->count;
                list->entries[j] = list->entries[j + 1];
            }
            (*count)--;
        } while (*count > 0);
    }
    // Must go through list, not the global.
    list->lastFrame = 0;
    for (int k = 0; k < 24; k++)
        g_speechTypes[k].minFrame = 0;
}

// FUNCTION: 0x47eee0
void ShutdownSound()
{
    SpeechQueue* list = g_speechQueue;
    if (list) {
        if (list->count > 0) {
            int* count = &list->count;
            do {
                int i = *count - 1;
                SpeechEntry* last = &list->entries[i];
                if (last->data) {
                    GameFreeThunk(last->data);
                    last->data = 0;
                }
                for (int j = i; j < *count; j++) {
                    // Re-taken at the top of the body: this is what stops MSVC 5
                    // folding &list->count into [ebp+0x99]. The assignment is
                    // loop invariant, so the lea is hoisted into the shift loop's
                    // preheader, which is exactly where the original has it.
                    count = &list->count;
                    list->entries[j] = list->entries[j + 1];
                }
                (*count)--;
            } while (*count > 0);
        }
        list->lastFrame = 0;
        operator delete(list);
        g_speechQueue = 0;
    }
    g_game->sound->RestoreMixerVolumes();
    Sound* sound = g_game->sound;
    if (sound) {
        sound->ReleaseDirectSound();
        operator delete(sound);
    }
    g_game->sound = 0;
}

// FUNCTION: 0x47efc0
void SetNoDirectSound()
{
    g_noDirectSound = 1;
}

// FUNCTION: 0x47efd0
void SetUseWindowsSound()
{
    g_useWindowsSound = 1;
    g_noDirectSound = 1;
}

// Loads sounds/<name>.WAV, either as a plain file or through the sound system.
// FUNCTION: 0x47efe0
void* __stdcall LoadSoundFile(const char* name)
{
    char path[256];
    if (g_noDirectSound != 0 && g_useWindowsSound == 0) {
        return 0;
    }
    BuildDataPath(path, "sounds", name, "WAV");
    if (g_useWindowsSound != 0) {
        return HAPI_LoadFile(path, 0);
    }
    return g_game->sound->LoadSample(path);
}

// Releases a set of sound buffers: frees the set directly when
// g_useWindowsSound is set, otherwise through the sound object's ReleaseSampleSet.
// FUNCTION: 0x47f060
void __stdcall FreeSoundSet(IDirectSoundBuffer** set)
{
    if (g_useWindowsSound != 0) {
        GameFreeThunk(set);
        return;
    }
    g_game->sound->ReleaseSampleSet(set);
}

// FUNCTION: 0x47f090
void __stdcall StreamSoundDelayed(char* param_1, int param_2, int param_3)
{
    if (g_noDirectSound == 0) {
        g_game->sound->StreamSampleDelayed(param_1, param_2, param_3);
    }
}

// Plays the sound of a unit-type index (0xffff means "none"): directly through
// the sound object, or, in a network game, by sending the 0x13 message to the
// other players first.
// FUNCTION: 0x47f0c0
int __stdcall PlaySoundByIndex(int index, int param_2)
{
    if (index != 0xffff) {
        int sound = g_game->soundIds[index];
        if (g_useWindowsSound) {
            if (g_playLooping)
                return PlayLoopingWavMemory(sound);
            return PlayWavMemory(sound);
        }
        if (g_game->volume1 != 0 && (g_game->flags_37f19 & 7) != 0
            && g_noDirectSound == 0) {
            if (param_2) {
                Packet_0047f0c0 packet;
                packet.type = 0x13;
                packet.flag = 1;
                packet.index = index;
                BroadcastPacket(GetLocalDpid(), &packet, sizeof(packet));
            }
            if (g_playLooping)
                return g_game->sound->PlayLooping(sound, -585);
            return g_game->sound->PlaySampleSet(sound, -585, 0);
        }
    }
    return 0;
}

// Index of a sound in the game's 32-byte sound name table, case-insensitive;
// 0xffff when not found.
static inline int FindSound(char* name)
{
    for (int i = 0; i < g_game->soundCount; i++) {
        if (g_game->soundNames[i][0] && _strcmpi(g_game->soundNames[i], name) == 0)
            return i;
    }
    return 0xffff;
}

// Looks up a sound by name and passes its index to PlaySoundByIndex.
// FUNCTION: 0x47f1a0
void __stdcall PlaySoundByName(char* name, int param_2)
{
    PlaySoundByIndex(FindSound(name), param_2);
}

// The same with g_playLooping set for the duration of the call.
// FUNCTION: 0x47f210
void __stdcall PlayLoopingSoundByName(char* name, int param_2)
{
    g_playLooping = 1;
    PlaySoundByIndex(FindSound(name), param_2);
    g_playLooping = 0;
}

// Plays a sound by name: through PlayWavFromDisk when g_useWindowsSound is set,
// otherwise through the game's sound object when sound is enabled.
// FUNCTION: 0x47f290
int __stdcall PlaySoundFile(char* name)
{
    if (g_useWindowsSound)
        return PlayWavFromDisk(name);
    if (name && strlen(name) && g_game->volume1 && (g_game->flags_37f19 & 7) && !g_noDirectSound)
        return g_game->sound->PlaySample(name, -0x249, 0);
    return 0;
}

static inline int MapContains(unsigned int w, unsigned int h, int tx, int ty)
{
    return tx < w && ty < h;
}

// Plays the sound at soundIds[index] when the position is visible to the local
// player: explored (fog) map when g_game->mapFlags has bit 1 set, the shared
// per-player visibility mask otherwise. Sends the 0x13 packet first when
// param_3 is set, and picks the near (-585) or far (-1585) variant depending on
// whether the position is inside the screen rectangle.
// FUNCTION: 0x47f300
int __stdcall PlaySoundAt(int index, Pos_0047f300* pos, int param_3)
{
    if (g_useWindowsSound)
        return PlaySoundByIndex(index, param_3);
    if (index == 0xffff)
        return 0;
    if (g_game->volume1 == 0)
        return 0;
    if ((g_game->flags_37f19 & 7) == 0)
        return 0;
    if (g_noDirectSound != 0)
        return 0;

    int sound = g_game->soundIds[index];

    if (param_3) {
        Packet_0047f300 packet;
        packet.type = 0x13;
        packet.flag = 1;
        packet.index = index;
        packet.pos = *pos;
        BroadcastPacket(GetLocalDpid(), &packet, 0x12);
    }

    if (GetMapCell(pos->xVal / (1 << 20), pos->zVal / (1 << 20)) == 0)
        return 0;

    // pi stays an int; the player address goes through the inline Current() to get the original's lea.
    int pi = g_game->playerIndex;
    Player_0047f300* player = g_game->Current();
    Player_0047f300* player2 = g_game->Current();
    // vis stays an int (char or bool changes the 1/0 pair); the flag is compared to 2 explicitly.
    int vis;
    if ((g_game->mapFlags & 2) == 2) {
        int tx = pos->x >> 5;
        int ty = (pos->z - (pos->y >> 1)) >> 5;
        if (tx < player->exploredWidth && ty < player->exploredHeight
            && player->explored[player2->exploredWidth * ty + tx] != 0)
            vis = 1;
        else
            vis = 0;
    } else {
        int tx = pos->x >> 5;
        int ty = (pos->z - (pos->y >> 1)) >> 5;
        if (!MapContains(player->exploredWidth, player->exploredHeight, tx, ty))
            vis = 0;
        else
            vis = (g_game->visibilityMask[player2->exploredWidth * ty + tx]
                & (1 << pi)) ? 1 : 0;
    }
    if (vis != 0) {
        if (g_game->sound->Is3DEnabled()) {
            Vector3_0047f300 p;
            p.x = pos->x - g_game->scrollX - (g_game->screenTilesX / 2) * 16;
            p.z = g_game->scrollY + (g_game->screenTilesY / 2) * 16
                + (pos->y >> 1) - pos->z;
            // p.z is written before p.y: the store order follows the original.
            p.y = 0;
            g_game->sound->Set3DDistances(
                (float)(((g_game->screenTilesX + g_game->screenTilesY) / 2) * 16),
                (float)((g_game->width + g_game->height) * 16));
            return g_game->sound->PlaySampleSet(sound, -585, &p);
        } else {
            if (g_game->scrollX > pos->x || g_game->scrollY > pos->z
                || g_game->scrollX + g_game->screenTilesX * 16 < pos->x
                || g_game->scrollY + g_game->screenTilesY * 16 < pos->z)
                return g_game->sound->PlaySampleSet(sound, -1585, 0);
            return g_game->sound->PlaySampleSet(sound, -585, 0);
        }
    }
    return 0;
}

// Plays a sound by name: looks the name up and passes its index (0xffff when
// not found) to PlaySoundAt.
// FUNCTION: 0x47f610
void __stdcall PlaySoundAtByName(char* name, int param_2, int param_3)
{
    PlaySoundAt(FindSound(name), (Pos_0047f300*)param_2, param_3);
}

// Ages the sound driver held in g_speechQueue: if the list is not empty, and
// the game frame has reached the driver's next expiry, it refreshes that
// expiry and plays the entry's sound. Either way it then drops the head entry
// of the nine-entry list and frees its data.
// The count is reached through a reference and the entries through the local
// pointer, the fields through the global: that mix is what keeps the driver's
// value in ecx for the two calls and copies it to ebx for the rest.
// FUNCTION: 0x47f680
void PlayNextSpeech()
{
    SpeechQueue* driver = g_speechQueue;
    int& count = g_speechQueue->count;
    if (count) {
        if (g_game->frame >= g_speechQueue->lastFrame + g_speechQueue->interval) {
            g_speechQueue->PlaySpeech(0, 1, 1);
            driver->lastFrame = g_game->frame;
        } else {
            g_speechQueue->PlaySpeech(0, 0, 1);
        }
        if (driver->entries[0].data) {
            GameFreeThunk(driver->entries[0].data);
            driver->entries[0].data = 0;
        }
        for (int i = 0; i < count; i++)
            driver->entries[i] = driver->entries[i + 1];
        count--;
    }
    if (g_useWindowsSound)
        ResumeLoopingWav();
}

// FUNCTION: 0x47f750
void StopAllSounds()
{
    if (g_noDirectSound == 0) {
        g_game->sound->StopAllBuffers();
    }
    if (g_useWindowsSound != 0) {
        StopWindowsSound();
    }
}

// FUNCTION: 0x47f780
void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text)
{
    if (unit->owner == g_game->playerIndex && (unit->flags & 0x10000000) && !(unit->flags & 0x4000)) {
        if (text == 0) {
            text = g_speechTypes[kind].text;
        }
        g_speechQueue->EnqueueSpeech(unit, kind, Translate(text));
    }
}

// QueueUnitSpeech is inlined here: the text argument is loaded early because it
// crosses the inline boundary.
// FUNCTION: 0x47f7e0
void __stdcall QueueUnitSpeechIfVisible(Unit* unit, int kind, char* text)
{
    if (IsUnitVisible(unit) != 0) {
        QueueUnitSpeech(unit, kind, text);
    }
}

// FUNCTION: 0x47f850
void __stdcall QueueUnitSpeechIfNotVisible(Unit* unit, int kind, char* text)
{
    if (IsUnitVisible(unit) == 0) {
        QueueUnitSpeech(unit, kind, text);
    }
}

// Removes every entry whose unit field equals id from the list at g_speechQueue,
// freeing its data and shifting the later entries down.
// The count is used through its own pointer: accessed as list->count, MSVC
// keeps the list pointer live instead of reusing its register for the entries.
// FUNCTION: 0x47f8c0
void __stdcall RemoveSpeechOfUnit(int id)
{
    int* count = &g_speechQueue->count;
    SpeechEntry* entries = g_speechQueue->entries;
    int i = 0;
    while (i < *count) {
        if (entries[i].unit == (Unit*)id) {
            if (entries[i].data) {
                GameFreeThunk(entries[i].data);
                entries[i].data = 0;
            }
            for (int j = i; j < *count; j++)
                entries[j] = entries[j + 1];
            (*count)--;
        } else {
            i++;
        }
    }
}

// FUNCTION: 0x47f990
void SpeechQueue::ClearSpeechList(void)
{
    while (count > 0)
        Remove(count - 1);
    lastFrame = 0;
}

// FUNCTION: 0x47fa30
void SpeechQueue::ClearSpeechEntries(void)
{
    while (count > 0)
        Remove(count - 1);
    lastFrame = 0;
}

// Queues a speech message for a unit: drops it if the message is not due yet or
// already queued, makes room by playing the last entry when the list already
// holds eight, then inserts the new entry in priority order.
// FUNCTION: 0x47fad0
void SpeechQueue::EnqueueSpeech(Unit* unit, int kind, char* text)
{
    if (g_game->frame < g_speechTypes[kind].minFrame)
        return;

    for (int i = 0; i < count; i++)
        if (entries[i].kind == kind)
            return;

    if (count == 8) {
        PlaySpeech(7, 0, 1);
        if (entries[7].data) {
            GameFreeThunk(entries[7].data);
            entries[7].data = 0;
        }
        for (int i = 7; i < count; i++)
            entries[i] = entries[i + 1];
        count--;
    }

    int j;
    if (count != 0) {
        for (j = 0; j < count; j++)
            if (g_speechTypes[entries[j].kind].priority < g_speechTypes[kind].priority)
                break;

        for (int i = count; i > j; i--)
            entries[i] = entries[i - 1];
    } else {
        j = 0;
    }

    entries[j].kind = kind;
    entries[j].frame = g_game->frame;
    entries[j].unit = unit;
    entries[j].priority = g_speechTypes[kind].priority;
    if (text) {
        entries[j].data = (char*)GameAllocIgnoreTag("Speech Text", strlen(text) + 1);
        strcpy(entries[j].data, text);
    } else {
        entries[j].data = 0;
    }
    count++;
}

// Pops the front entry of the list: fires the sound for entry 0 (re-arming the
// repeat timer when it is due), frees its data, shifts the rest down.
// FUNCTION: 0x47fca0
void SpeechQueue::PlayNextSpeechEntry()
{
    if (count == 0) {
        return;
    }
    if (g_game->frame >= interval + lastFrame) {
        this->PlaySpeech(0, 1, 1);
        lastFrame = g_game->frame;
    } else {
        this->PlaySpeech(0, 0, 1);
    }
    if (entries[0].data) {
        GameFreeThunk(entries[0].data);
        entries[0].data = 0;
    }
    for (int i = 0; i < count; i++) {
        entries[i] = entries[i + 1];
    }
    count--;
}

// Handles one entry of the nine-entry sound request list: picks a random
// sample from the entry's sound category slot, plays "sounds/<name>.WAV" when
// the entry is due, re-arms the repeat timer, and (when logging is enabled)
// appends "<unit name>: <sound text>" to the message log.
//
// The structs are packed: pointer fields sit at odd offsets in the original
// (entry +0x8/+0xc, unit +0x92, game +0x37e13).
// FUNCTION: 0x47fd70
void SpeechQueue::PlaySpeech(int index, int param_2, int param_3)
{
    SpeechEntry* e = &entries[index];
    // The first two accesses use entries[index], not e: the entry address then forms as one lea.
    int slot = entries[index].kind;
    unsigned int catIndex = 0;
    catIndex = *(unsigned short*)(entries[index].unit->name + 0x20e);
    SoundCat_0047fd70* cat = &g_game->categories[catIndex];
    int count = cat->slots[slot].count;
    int idx = (int)((__int64)rand() * count / 0x8000);

    if ((int)e->priority > 10 - g_game->unitChat && count > 0 && param_2 != 0
        && (g_game->flags_37f19 & 0x40)) {
        char* name;
        if (DAT_0051e698)
            name = ((g_game->frame / 30) & 7) ? "sing" : "honk";
        else
            name = cat->slots[slot].table1 + idx * 0x40;

        char path[256];
        BuildDataPath(path, "sounds", name, "WAV");
        if (g_useWindowsSound) {
            PlayWavFromDisk(path);
        } else if (path && strlen(path) && g_game->volume1
                   && (g_game->flags_37f19 & 7) && g_noDirectSound == 0) {
            g_game->sound->PlaySample(path, -0x249, 0);
        }
        g_speechTypes[slot].minFrame =
            g_game->frame + g_speechTypes[slot].cooldown * 0x1e;
    }

    if (param_3 != 0 && (int)e->priority > 10 - g_game->unitChatText) {
        char* text = e->data;
        if (text == 0) {
            if (idx != -1)
                text = cat->slots[slot].table2 + idx * 0x40;
            if (text == 0)
                return;
        }
        if (*text == 0)
            return;
        if (e->unit->flags & 0x10000000) {
            char msg[100];
            sprintf(msg, "%s: %s", e->unit->name, text);
            AddMessage(msg, 1, e->unit->id, '\n');
        }
    }
}

// FUNCTION: 0x47ffa0
void SpeechQueue::RemoveSpeechAt(int index)
{
    SpeechEntry* e = &entries[index];
    if (e->data != 0) {
        GameFreeThunk(e->data);
        e->data = 0;
    }
    for (int i = index; i < count; i++) {
        entries[i] = entries[i + 1];
    }
    count--;
}

// FUNCTION: 0x480020
void SpeechQueue::RemoveSpeechOfId(Unit* unit)
{
    int i = 0;
    while (i < count) {
        if (entries[i].unit == unit) {
            if (entries[i].data) {
                GameFreeThunk(entries[i].data);
                entries[i].data = 0;
            }
            for (int j = i; j < count; j++)
                entries[j] = entries[j + 1];
            count--;
        } else {
            i++;
        }
    }
}

#include <vector>
#include <algorithm>

class Class_004800c0 {
public:
    typedef std::vector<Unit*> UnitVector;
    UnitVector units;       // allocator +0x0, _First +0x4, _Last +0x8, _End +0xc

    UnitVector::iterator EraseSwapBack(UnitVector::iterator where);
};

// Unordered erase: overwrites *where with the last element, drops the last
// element (erase(end() - 1), whose inlined copy loop and _Destroy dead store
// remain) and returns where, which now holds the moved element. The only
// caller (0x40b8b2) passes a slot of this same vector.
// FUNCTION: 0x4800c0
Class_004800c0::UnitVector::iterator Class_004800c0::EraseSwapBack(UnitVector::iterator where)
{
    UnitVector::iterator last = units.end() - 1;
    *where = *last;
    units.erase(last);
    return where;
}

class IntDynArray {
public:
    std::vector<int> items;            // +0x0 (_First +0x4, _Last +0x8)

    int EraseByValue(int value);
};

// Removes the first occurrence of a value from a std::vector by moving the
// last element into its slot and erasing the last element.
// FUNCTION: 0x480100
int IntDynArray::EraseByValue(int value)
{
    std::vector<int>::iterator it = std::find(items.begin(), items.end(), value);
    if (it == items.end()) {
        return 0;
    }
    std::vector<int>::iterator last = items.end() - 1;
    *it = *last;
    items.erase(last);
    return 1;
}

class Squad {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    std::vector<int> items;            // +0x10

    Squad(int a, int b);
};

// Constructor of a class with two values, two zeroed fields and a
// std::vector (allocator byte +0x10, _First +0x14, _Last +0x18, _End +0x1c).
// The vector's default allocator argument is a temporary copied into the
// allocator byte; MSVC keeps it in the first parameter's stack slot.
// FUNCTION: 0x480160
Squad::Squad(int a, int b)
    : field_0(a), field_4(b), field_8(0), field_c(0)
{
}
