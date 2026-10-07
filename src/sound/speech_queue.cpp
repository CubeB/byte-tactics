// Decompiled by Sonnet, Opus, Space Bunny Free and deepseek-v4.1-flash. Names are provisional.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* data);

struct Message_0047fad0 {              // 0x18 bytes
    int priority;                      // +0x0
    char unknown_4[8];
    char* text;                        // +0xc
    unsigned int minFrame;             // +0x10
    char unknown_14[4];
};

extern Message_0047fad0 DAT_005086dc[];

class Sound {
public:
    void PlaySample(const char* param1, int param2, int param3);
};

#pragma pack(push, 1)
struct Emitter_0047fd70 {
    char unknown_0[0x92];
    char* name;                            // +0x92 (its +0x20e is the category)
    char unknown_96[0xa8 - 0x96];
    unsigned short field_a8;               // +0xa8
    char unknown_aa[0x110 - 0xaa];
    int field_110;                         // +0x110
};

struct Slot_0047fd70 {
    int count;                             // +0x0
    char* table1;                          // +0x4
    char* table2;                          // +0x8
};

struct SoundCat_0047fd70 {
    char unknown_0[0x40];
    Slot_0047fd70 slots[24];               // +0x40
};

struct Table_0047fd70 {
    int field_0;                           // +0x0
    int unknown_4;
    int unknown_8;
    int field_c;                           // +0xc
    int unknown_10;
    int unknown_14;
};

// One queued speech (0x11 bytes).
struct SpeechEntry {
    int kind;                              // +0x0
    int frame;                             // +0x4, when it was queued
    Emitter_0047fd70* unit;                // +0x8
    char* data;                            // +0xc
    unsigned char priority;                // +0x10
};

extern Table_0047fd70 DAT_005086e0[24];
extern int g_noDirectSound;
extern int g_useWindowsSound;
extern int DAT_0051e698;

char* __stdcall BuildDataPath(char* buf, char* dir, char* name, char* ext);
void __stdcall AddMessage(char* text, unsigned char key, unsigned short value, char last);
int __stdcall PlayWavFromDisk(char* path);

struct Game {
    char unknown_0[0x10];
    Sound* sound;                          // +0x10
    char unknown_14[0x37e13 - 0x14];
    SoundCat_0047fd70* categories;         // +0x37e13
    char unknown_37e17[0x37f0c - 0x37e17];
    int field_37f0c;                       // +0x37f0c
    char unknown_37f10[0x37f17 - 0x37f10];
    unsigned char field_37f17;             // +0x37f17
    unsigned char field_37f18;             // +0x37f18
    unsigned char field_37f19;             // +0x37f19
    char unknown_37f1a[0x38a47 - 0x37f1a];
    unsigned int frame;                    // +0x38a47
};

extern Game* g_game;

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
            FUN_004d85a0(entries[i].data);
            entries[i].data = 0;
        }
        for (int j = i; j < count; j++)
            entries[j] = entries[j + 1];
        count--;
    }

    SpeechQueue(int param_1, int param_2);
    void ClearSpeechList(void);
    void ClearSpeechEntries(void);
    void EnqueueSpeech(Emitter_0047fd70* unit, int kind, char* text);
    void PlayNextSpeechEntry();
    void PlaySpeech(int index, int param_2, int param_3);
    void RemoveSpeechAt(int index);
    void RemoveSpeechOfId(Emitter_0047fd70* unit);
};
#pragma pack(pop)

// FUNCTION: 0x47f960
SpeechQueue::SpeechQueue(int param_1, int param_2)
{
    count = 0;
    lastFrame = 0;
    interval = param_1;
    field_a5 = param_2;
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
void SpeechQueue::EnqueueSpeech(Emitter_0047fd70* unit, int kind, char* text)
{
    if (g_game->frame < DAT_005086dc[kind].minFrame)
        return;

    for (int i = 0; i < count; i++)
        if (entries[i].kind == kind)
            return;

    if (count == 8) {
        PlaySpeech(7, 0, 1);
        if (entries[7].data) {
            FUN_004d85a0(entries[7].data);
            entries[7].data = 0;
        }
        for (int i = 7; i < count; i++)
            entries[i] = entries[i + 1];
        count--;
    }

    int j;
    if (count != 0) {
        for (j = 0; j < count; j++)
            if (DAT_005086dc[entries[j].kind].priority < DAT_005086dc[kind].priority)
                break;

        for (int i = count; i > j; i--)
            entries[i] = entries[i - 1];
    } else {
        j = 0;
    }

    entries[j].kind = kind;
    entries[j].frame = g_game->frame;
    entries[j].unit = unit;
    entries[j].priority = DAT_005086dc[kind].priority;
    if (text) {
        entries[j].data = (char*)FUN_004d83b0("Speech Text", strlen(text) + 1);
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
        ((SpeechQueue*)this)->PlaySpeech(0, 1, 1);
        lastFrame = g_game->frame;
    } else {
        ((SpeechQueue*)this)->PlaySpeech(0, 0, 1);
    }
    if (entries[0].data) {
        FUN_004d85a0(entries[0].data);
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
// (entry +0x8/+0xc, emitter +0x92, game +0x37e13).
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

    if ((int)e->priority > 10 - g_game->field_37f17 && count > 0 && param_2 != 0
        && (g_game->field_37f19 & 0x40)) {
        char* name;
        if (DAT_0051e698)
            name = ((g_game->frame / 30) & 7) ? "sing" : "honk";
        else
            name = cat->slots[slot].table1 + idx * 0x40;

        char path[256];
        BuildDataPath(path, "sounds", name, "WAV");
        if (g_useWindowsSound) {
            PlayWavFromDisk(path);
        } else if (path && strlen(path) && g_game->field_37f0c
                   && (g_game->field_37f19 & 7) && g_noDirectSound == 0) {
            g_game->sound->PlaySample(path, -0x249, 0);
        }
        DAT_005086e0[slot].field_c =
            g_game->frame + DAT_005086e0[slot].field_0 * 0x1e;
    }

    if (param_3 != 0 && (int)e->priority > 10 - g_game->field_37f18) {
        char* text = e->data;
        if (text == 0) {
            if (idx != -1)
                text = cat->slots[slot].table2 + idx * 0x40;
            if (text == 0)
                return;
        }
        if (*text == 0)
            return;
        if (e->unit->field_110 & 0x10000000) {
            char msg[100];
            sprintf(msg, "%s: %s", e->unit->name, text);
            AddMessage(msg, 1, e->unit->field_a8, '\n');
        }
    }
}

// FUNCTION: 0x47ffa0
void SpeechQueue::RemoveSpeechAt(int index)
{
    SpeechEntry* e = &entries[index];
    if (e->data != 0) {
        FUN_004d85a0(e->data);
        e->data = 0;
    }
    for (int i = index; i < count; i++) {
        entries[i] = entries[i + 1];
    }
    count--;
}

// FUNCTION: 0x480020
void SpeechQueue::RemoveSpeechOfId(Emitter_0047fd70* unit)
{
    int i = 0;
    while (i < count) {
        if (entries[i].unit == unit) {
            if (entries[i].data) {
                FUN_004d85a0(entries[i].data);
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
