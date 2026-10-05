// Decompiled by space-bunny-free. Names are provisional.

struct Rect_0046c620 {                  // 16 bytes, the rect at net+0x465
    int x0;
    int y0;
    int x1;
    int y1;
};

struct Name_0046c620 {                  // 17 bytes, the player's name
    char text[16];
    char flag;
};

struct Class_0046c620 {                 // the object at g_game+0x14
    Name_0046c620 name;                 // +0x00
    char unknown_11[0x4c9 - 0x11];
    int field_4c9;                      // +0x4c9
    char unknown_4cd[4];
};

class Class_00435c30 {
public:
    int FUN_00435c30();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14];
    Class_0046c620 net;                 // +0x14
    char unknown_25[0x2a42 - 0x14 - sizeof(Class_0046c620)];
    unsigned char player;               // +0x2a42
    char unknown_2a43[0x391e9 - 0x2a43];
    Class_00435c30* field_391e9;        // +0x391e9
    char unknown_391ed[0x39201 - 0x391ed];
    char field_39201[1];                // +0x39201
};
#pragma pack(pop)

extern Game* g_game;

typedef int (__stdcall *SendFn_0046c620)(int, Rect_0046c620*, void*, int, Name_0046c620*,
                                         int, int, int, void*, void*);

extern int DAT_0051e550;
extern Name_0046c620 g_reportPlayerName;
extern void* DAT_0051e574;
extern void* DAT_0051e57c;
extern SendFn_0046c620 DAT_0051e584;
extern int DAT_0051e58c;
extern int DAT_0051e590;
extern int g_reportFlags;

Rect_0046c620* __stdcall FUN_004ca9e0(Class_0046c620* p);
int __stdcall FUN_004ca9d0(Class_0046c620* p);
int __stdcall FUN_004ca9f0(int mode);
int __stdcall RIReport(int, Rect_0046c620*, void*, int, Name_0046c620*, int,
                           int, int, void*, void*);
int FUN_0046c2a0();

// FUNCTION: 0x46c620
int __stdcall ReportGameEvent(int msg)
{
    if (!DAT_0051e590 && !DAT_0051e58c)
        return 4;
    if (!DAT_0051e574 || !DAT_0051e57c || !DAT_0051e550)
        return 1;

    Rect_0046c620 rect = *FUN_004ca9e0(&g_game->net);
    int thing = FUN_004ca9d0(&g_game->net);

    if (msg == 1) {
        g_reportPlayerName = g_game->net.name;
        char* p = g_reportPlayerName.text + 15;
        while (p > g_reportPlayerName.text && *p == ' ')
            *p-- = 0;
    }

    int id = FUN_0046c2a0();

    if (DAT_0051e590) {
        if (msg == 1 || msg == 6 || msg == 7)
            g_reportFlags = FUN_004ca9f0(msg == 1 ? 1 : 2 + (msg != 6));
        if (g_reportFlags & 3) {
            // its own statement, not an argument: the call has to be emitted
            // ahead of the other nine arguments being set up
            int team = g_game->field_391e9->FUN_00435c30();
            if (RIReport(msg, &rect, (char*)&g_game->field_39201, thing, &g_reportPlayerName,
                             team, g_game->player,
                             id, DAT_0051e574, DAT_0051e57c))
                DAT_0051e590 = 0;
        }
    }

    if (DAT_0051e58c) {
        int team = g_game->field_391e9->FUN_00435c30();
        DAT_0051e584(msg, &rect, (char*)&g_game->field_39201, thing, &g_reportPlayerName,
                     team, g_game->player,
                     id, DAT_0051e574, DAT_0051e57c);
    }

    return 0;
}
