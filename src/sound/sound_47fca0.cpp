// Decompiled by space-bunny-free. Names are provisional.
// Pops the front entry of the list: fires the sound for entry 0 (re-arming the
// repeat timer when it is due), frees its data, shifts the rest down.

void __cdecl FUN_004d85a0(int* param_1);

#pragma pack(push, 1)
struct Entry_0047fca0 {                // 0x11 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int* data;                         // +0xc
    char field_10;                     // +0x10
};

class SpeechQueue {
public:
    Entry_0047fca0 entries[9];         // +0x0
    int count;                         // +0x99
    unsigned int field_9d;             // +0x9d
    unsigned int field_a1;             // +0xa1

    void PlayNextSpeechEntry();
    void PlaySpeech(int index, int param_2, int param_3);
};

struct Game {
    char unknown_0[0x38a47];
    int field_38a47;
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x47fca0
void SpeechQueue::PlayNextSpeechEntry()
{
    if (count == 0) {
        return;
    }
    if (g_game->field_38a47 >= field_a1 + field_9d) {
        ((SpeechQueue*)this)->PlaySpeech(0, 1, 1);
        field_9d = g_game->field_38a47;
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
