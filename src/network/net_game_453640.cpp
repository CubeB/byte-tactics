// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Chat message history refresh. Finds the "OUTPUT" list gadget, throttles the
// refresh with g_loungeRefreshTime (at most one every 2 ticks), clears the 0xa00-byte
// scratch buffer at g_loungeChatter and copies the last few chat lines out of the
// 30-entry ring buffer at g_game+0x12ef into it. Then, if the player named by
// g_timeoutPlayerDpid is active and in state 3, it shows the seconds left before that
// player is dropped from the game and rejects them once the timer has run out.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

struct Ring_00453640 {                 // 0x48 bytes, 30 of them at g_game+0x12ef
    char text[0x40];                   // +0x00
    unsigned int time;                 // +0x40
    unsigned short field_44;           // +0x44
    char field_46;                     // +0x46
    unsigned char field_47;            // +0x47
};

struct Entry_00453640 {                // gadget returned by FUN_0049ff90
    char unknown_0[0xc0];
    short count;                       // +0xc0
};

struct Holder_00453640 {
    int unknown_0;                     // +0x00
    Entry_00453640* entries;           // +0x04
};

struct Player_00453640 {               // 0x14b bytes, 10 of them at g_game+0x1b63
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0x1c - 0x08];
    int lastHeard;                     // +0x1c
    char unknown_20[0x73 - 0x20];
    char state;                        // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game {
    char unknown_0[0x519];
    char message[0x531 - 0x519];       // +0x519
    Holder_00453640* holder;           // +0x531
    char unknown_535[0x12ef - 0x535];
    Ring_00453640 ring[30];            // +0x12ef
    char unknown_1b5f[0x1b63 - 0x1b5f];
    Player_00453640 players[10];       // +0x1b63
    char unknown_2851[0x2a3e - 0x2851];
    unsigned short tail;               // +0x2a3e
    unsigned short head;               // +0x2a40
    char unknown_2a42[0x37f31 - 0x2a42];
    unsigned int field_37f31;          // +0x37f31
};

#pragma pack(pop)

extern Game* g_game;
extern char* g_loungeChatter;
extern int g_loungeRefreshTime;
extern int g_timeoutPlayerDpid;

int GetTicks();
Entry_00453640* __stdcall FUN_0049ff90(void* entries, char* name);
void __stdcall FUN_0049fa90(void* obj);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004a0bf0(void* obj, char* name, char* text, int param_4);
void __stdcall FUN_004a9660(void* obj);
void __stdcall RejectPlayer(int param_1, int param_2);

static inline int GetPlayerId_00453640(unsigned char i)
{
    if (i != 10 && g_game->players[i].state)
        return g_game->players[i].id;
    return -1;
}

static inline unsigned char FindPlayerIndex_00453640(int id)
{
    if (id == -1)
        return 10;
    for (unsigned char i = 0; i < 10; i++) {
        if (GetPlayerId_00453640(i) == id)
            return i;
    }
    return 10;
}

// FUNCTION: 0x453640
void UpdateTimeoutDialog()
{
    Entry_00453640* entry = FUN_0049ff90(g_game->holder->entries, "OUTPUT");

    if (g_loungeRefreshTime < GetTicks()) {
        g_loungeRefreshTime = GetTicks() + 2;
        FUN_0049fa90(g_game->message);
    }

    memset(g_loungeChatter, 0, 0xa00);

    int tail = g_game->tail;
    int count = entry->count;
    int idx = tail;
    for (int n = 1; n < count - 1; n++) {
        if (idx == g_game->head)
            break;
        idx--;
        if (idx < 0)
            idx = 29;
    }

    char* p = g_loungeChatter;
    if (idx != tail) {
        do {
            strcpy(p, g_game->ring[idx].text);
            p += strlen(g_game->ring[idx].text) + 1;
            idx++;
            if (idx >= 30)
                idx = 0;
        } while (idx != g_game->tail);
    }

    // The original searches for the player twice: once for the == 10 test and
    // again for the index used to fetch the record. Keep it; the bytes do this.
    unsigned char found = FindPlayerIndex_00453640(g_timeoutPlayerDpid);
    Player_00453640* player;
    if (found == 10)
        player = 0;
    else
        player = &g_game->players[FindPlayerIndex_00453640(g_timeoutPlayerDpid)];

    if (player != 0 && player->active != 0 && player->state == 3) {
        int elapsed = (GetTicks() - player->lastHeard) / 30;
        char buf[200];
        sprintf(buf, FUN_004c5740("will be rejected in %d seconds"),
                g_game->field_37f31 - elapsed + 0x78);
        FUN_004a0bf0(g_game->message, "TIMETEXT", buf, 0);
        FUN_0049fa90(g_game->message);
        if (elapsed < g_game->field_37f31 + 0x78)
            return;
        RejectPlayer(g_timeoutPlayerDpid, 6);
        FUN_004a9660(g_game->message);
    }
}
