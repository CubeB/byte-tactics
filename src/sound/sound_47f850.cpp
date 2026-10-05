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

struct Message_0047f850 {              // 0x18 bytes
    char* text;                        // +0x0
    char unknown_4[0x14];
};

class SpeechQueue {
public:
    void EnqueueSpeech(Unit* unit, int kind, char* text);
};

extern Game* g_game;
extern SpeechQueue* DAT_0051e68c;
extern Message_0047f850 DAT_005086e8[];

char* __stdcall Translate(char* text);
int __stdcall FUN_0048bcb0(Unit* unit);

// Inlined copy of QueueUnitSpeech: the text argument is loaded early because it
// crosses the inline boundary.
static inline void Report_0047f780(Unit* unit, int kind, char* text)
{
    if (unit->owner == g_game->field_2a43 && (unit->flags & 0x10000000) && !(unit->flags & 0x4000)) {
        if (text == 0) {
            text = DAT_005086e8[kind].text;
        }
        DAT_0051e68c->EnqueueSpeech(unit, kind, Translate(text));
    }
}

// FUNCTION: 0x47f850
void __stdcall FUN_0047f850(Unit* unit, int kind, char* text)
{
    if (FUN_0048bcb0(unit) == 0) {
        Report_0047f780(unit, kind, text);
    }
}
