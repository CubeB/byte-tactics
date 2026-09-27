// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_0047f7e0 {
    char unknown_0[0x2a43];
    unsigned char field_2a43;          // +0x2a43
};

struct Unit_0047f7e0 {
    char unknown_0[0xff];
    unsigned char owner;               // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

struct Message_0047f7e0 {              // 0x18 bytes
    char* text;                        // +0x0
    char unknown_4[0x14];
};

class Class_0047fad0 {
public:
    void FUN_0047fad0(Unit_0047f7e0* unit, int kind, char* text);
};

extern Game_0047f7e0* g_game;
extern Class_0047fad0* DAT_0051e68c;
extern Message_0047f7e0 DAT_005086e8[];

char* __stdcall FUN_004c5740(char* text);
int __stdcall FUN_0048bcb0(Unit_0047f7e0* unit);

// Inlined copy of FUN_0047f780: the text argument is loaded early because it
// crosses the inline boundary.
static inline void Report_0047f780(Unit_0047f7e0* unit, int kind, char* text)
{
    if (unit->owner == g_game->field_2a43 && (unit->flags & 0x10000000) && !(unit->flags & 0x4000)) {
        if (text == 0) {
            text = DAT_005086e8[kind].text;
        }
        DAT_0051e68c->FUN_0047fad0(unit, kind, FUN_004c5740(text));
    }
}

// FUNCTION: 0x47f7e0
void __stdcall FUN_0047f7e0(Unit_0047f7e0* unit, int kind, char* text)
{
    if (FUN_0048bcb0(unit) != 0) {
        Report_0047f780(unit, kind, text);
    }
}
