// Decompiled by Opus. Names are provisional.
// Reads the "MUSICVOL" slider into the music volume setting, then applies the
// brightness and both volume levels (inlined FUN_0045ba20 and FUN_0045bcc0).

class Class_004d0070 {
public:
    void FUN_004d0070(int level);
};

class Class_004d00d0 {
public:
    void FUN_004d00d0(int level, int flag);
};

#pragma pack(push, 1)
struct Entry_4a0200 {
    char unknown_0[0x136];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
};

struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x37f08 - 0x14];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
};
#pragma pack(pop)

struct Holder_0045bea0 {
    int unknown_0;
    Entry_4a0200* entries;             // +0x4
};

struct Object_0045bea0 {
    char unknown_0[0x18];
    Holder_0045bea0* holder;           // +0x18
};

extern Game* g_game;

Entry_4a0200* __stdcall FUN_004a0200(Entry_4a0200* entries, char* name);
void __stdcall FUN_004ba590(float value);

static inline int SliderValue(Entry_4a0200* e)
{
    if (e->steps <= 1)
        return 0;
    return (int)((float)e->pos / (e->steps - 1) * e->max);
}

static inline void ApplySound()
{
    FUN_004ba590(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->FUN_004d0070(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->FUN_004d00d0(g_game->volume2 << 10, 0);
}

// FUNCTION: 0x45bea0
void __stdcall FUN_0045bea0(Object_0045bea0* obj, int unused)
{
    Entry_4a0200* e = FUN_004a0200(obj->holder->entries, "MUSICVOL");
    if (e != 0) {
        g_game->volume2 = SliderValue(e);
        ApplySound();
    }
}
