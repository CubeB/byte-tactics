// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, continued by GPT-6.1-sol, continued by Space Bunny Free. Names are provisional.

#pragma pack(push, 1)
struct Entry_0047f8c0 {                // 0x11 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int* data;                         // +0xc
    char field_10;                     // +0x10
};

struct List_0047f8c0 {
    Entry_0047f8c0 entries[9];         // +0x0
    int count;                         // +0x99
    int field_9d;                      // +0x9d
};
#pragma pack(pop)

class Class_004d0130 {
public:
    void RestoreMixerVolumes();
};

// The callee of 0x4ceee0 is a different class in data/symbols.csv, so the
// object g_game->sound is cast to call it.
class Sound {
public:
    void ReleaseDirectSound();
};

struct Game {
    char unknown_0[0x10];
    Class_004d0130* sound;             // +0x10
};

extern List_0047f8c0* DAT_0051e68c;
extern Game* g_game;

void __cdecl operator delete(void* p);
void __cdecl FUN_004d85a0(int* data);

// FUNCTION: 0x47eee0
void ShutdownSound()
{
    List_0047f8c0* list = DAT_0051e68c;
    if (list) {
        if (list->count > 0) {
            int* count = &list->count;
            do {
                int i = *count - 1;
                Entry_0047f8c0* last = &list->entries[i];
                if (last->data) {
                    FUN_004d85a0(last->data);
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
        list->field_9d = 0;
        operator delete(list);
        DAT_0051e68c = 0;
    }
    g_game->sound->RestoreMixerVolumes();
    Class_004d0130* sound = g_game->sound;
    if (sound) {
        ((Sound*)sound)->ReleaseDirectSound();
        operator delete(sound);
    }
    g_game->sound = 0;
}
