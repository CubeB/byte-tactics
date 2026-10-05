// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2a43];
    unsigned char field_2a43;          // +0x2a43
};

struct Unit {
    char unknown_0[0xff];
    unsigned char owner;               // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

struct Message_0047f780 {              // 0x18 bytes
    char* text;                        // +0x0
    char unknown_4[0x14];
};

class SpeechQueue {
public:
    void EnqueueSpeech(Unit* unit, int kind, char* text);
};

extern Game* g_game;
extern SpeechQueue* DAT_0051e68c;
extern Message_0047f780 DAT_005086e8[];

char* __stdcall Translate(char* text);

// FUNCTION: 0x47f780
void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text)
{
    if (unit->owner == g_game->field_2a43 && (unit->flags & 0x10000000) && !(unit->flags & 0x4000)) {
        if (text == 0) {
            text = DAT_005086e8[kind].text;
        }
        DAT_0051e68c->EnqueueSpeech(unit, kind, Translate(text));
    }
}
