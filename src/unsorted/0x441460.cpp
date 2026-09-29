// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (61.5%). Structure transcribed: provider-GUID message selection,
// the FUN_004abd90/FUN_004ab170/FUN_004c9e50 setup, the per-game fill loop
// over the 15 list buffers (p[1..15] = g_game->data[1..15] at +0x2a47), and
// the GAMENAME callback. What still differs:
//  * local frame: original `sub esp,0x1b4`; our locals only reach ~0xfc, so
//    every [esp+N] operand in the loop is at the wrong displacement. The
//    original layout is settings at S+0x1a1, temp S+0x88, names S+0x68,
//    pointers S+0x18.., rec S+0x14, count S+0x10, with an unexplained ~0x99
//    byte local between settings and temp.
//  * first block (0x441460-0x44151d): the original inlines the provider
//    memcmp chain TWICE (m0,m1,m2,m3 each compared again) and stores the raw
//    memcmp result of the 4th compare to a dead slot; our version uses a
//    single chain.
//  * the settings struct is read as four dwords from desc+4 and tested with
//    dword loads/shr, ours is typed as eight ushorts.
//  * register allocation in the loop body (esi/ebp/ebx picks).
#include <stdio.h>
#include <string.h>

struct Guid_00441460 {
    unsigned long d1;
    unsigned long d2;
    unsigned long d3;
    unsigned long d4;
};

struct Net_00441460 {
    char unknown_0[0x4cd];
};

struct Sub_00441460 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Game_00441460 {
    char unknown_0[0x14];
    Net_00441460 net;                  // +0x14
    char unknown_4e1[0x4fd - 0x4e1];
    int field_4fd;                     // +0x4fd
    char unknown_501[0x519 - 0x501];
    Sub_00441460 sub;                  // +0x519
    char unknown_529[0x2a47 - 0x529];
    void* data[16];                    // +0x2a47
    char unknown_2a87[0x2aa7 - 0x2a87];
    char* desc;                        // +0x2aa7
    char unknown_2aab[0x37e1b - 0x2aab];
    int field_37e1b;                   // +0x37e1b
    char unknown_37e1f[0x39201 - 0x37e1f];
    char provider[0x10];               // +0x39201
    char unknown_39211[1];
};
#pragma pack(pop)

struct Settings_00441460 {
    unsigned short field_0;            // +0x00
    unsigned short flags;              // +0x02
    unsigned short field_2;            // +0x04
    unsigned short players;            // +0x06
    unsigned short energy;             // +0x08
    unsigned short metal;              // +0x0a
    unsigned short field_4;            // +0x0c
    unsigned short version;            // +0x0e
};

struct Desc_00441460 {
    char unknown_0[4];
    Settings_00441460 settings;        // +0x04
    int count;                         // +0x14
    char name[0x10];                   // +0x18
    char map[0x10];                    // +0x28
    char unknown_38[0x54 - 0x38];
};

struct Entry_00441460 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_1[0x15b - 1];
};

struct Gadget_00441460 {
    int unknown_0;
    Entry_00441460* entries;           // +0x04
};

extern Game_00441460* g_game;
extern Guid_00441460 DAT_004fcdc8;
extern Guid_00441460 DAT_004fcda8;
extern Guid_00441460 DAT_004fcd98;
extern Guid_00441460 DAT_004fcdb8;

char* __stdcall FUN_004c5740(const char* text);
void __stdcall FUN_004abd90(Sub_00441460* sub, char* text, int a, int b, int c);
void __stdcall FUN_004ab170(Sub_00441460* sub, int a, int b);
void __stdcall FUN_004c69a0(int a);
void FUN_004c63a0();
int __stdcall FUN_004c9e50(Net_00441460* net, char* desc, int a);
void __stdcall FUN_004a9660(Sub_00441460* sub);
void __stdcall FUN_004a32a0(Sub_00441460* sub, char* name, char* text, int count, int flag);
char* FUN_0049f580(const char* key = 0);
int __stdcall FUN_0049fdf0(Entry_00441460* entries, const char* name, int type);
void __stdcall FUN_00441220(Sub_00441460* sub, Entry_00441460* entry);

// FUNCTION: 0x441460
int __stdcall FUN_00441460(Gadget_00441460* gadget)
{
    Guid_00441460* guid = (Guid_00441460*)g_game->provider;
    char* message;
    int unused;

    if (memcmp(guid, &DAT_004fcdc8, 0x10) != 0) {
        if (memcmp(guid, &DAT_004fcda8, 0x10) != 0) {
            if (memcmp(guid, &DAT_004fcd98, 0x10) == 0) {
                message = "Updating...";
                goto done;
            }
            if (memcmp(guid, &DAT_004fcdb8, 0x10) == 0)
                unused = 0;
            else
                unused = memcmp(guid, &DAT_004fcdb8, 0x10);
        }
    }
    if (memcmp(guid, &DAT_004fcdc8, 0x10) != 0) {
        if (memcmp(guid, &DAT_004fcda8, 0x10) == 0) {
            message = "Updating...";
            goto done;
        }
        if (memcmp(guid, &DAT_004fcd98, 0x10) != 0) {
            if (memcmp(guid, &DAT_004fcdb8, 0x10) == 0)
                unused = 0;
            else
                unused = memcmp(guid, &DAT_004fcdb8, 0x10);
        }
    }
    message = "Connecting  (ESC to abort)";
done:
    FUN_004abd90(&g_game->sub, FUN_004c5740(message), 0x96, 0, 1);

    FUN_004ab170(&g_game->sub, g_game->field_37e1b, 0);
    FUN_004c69a0(g_game->field_37e1b);
    FUN_004c63a0();
    FUN_004c63a0();

    Settings_00441460 s;
    char pad[0x9c];
    char temp[0x80];
    char names[0x20];
    char* q;
    int count = FUN_004c9e50(&g_game->net, g_game->desc, 0);
    FUN_004a9660(&g_game->sub);
    if (count < 0) {
        return 0;
    }

    char* p[16];
    int i;
    for (i = 1; i < 16; i++) {
        p[i] = (char*)g_game->data[i];
        memset(p[i], 0, 0xa00);
    }

    char* rec = (char*)g_game->desc + 0x18;
    for (i = 0; i < count; i++) {
        s = *(Settings_00441460*)(rec - 0x14);

        memcpy(names, rec, 0x20);

        strncpy(p[1], names, 0x10);
        p[1][0x10] = 0;
        p[1] += strlen(p[1]) + 1;

        sprintf(p[2], "%d/%d", *(int*)(rec - 4), s.flags & 0xf);
        p[2] += strlen(p[2]) + 1;

        memset(temp, 0, 0x80);
        strncpy(temp, names + 0x10, 0xf);
        q = temp + strlen(temp);
        while (q != temp) {
            q--;
            if (*q != ' ') break;
            *q = 0;
        }
        if (FUN_0049f580() != 0) {
            if (_strcmpi(FUN_0049f580("english"), "english") != 0) {
                _strlwr(temp);
                strncpy(temp, FUN_004c5740(temp), 0x80);
                temp[0x7f] = 0;
            }
        }
        strcpy(p[3], temp);
        p[3] += strlen(p[3]) + 1;

        if ((int)(s.version & 0xff) < (int)*(signed char*)((char*)g_game + 1)) {
            sprintf(p[4], "%s", FUN_004c5740("VER!"));
        } else {
            const char* t;
            if (s.flags & 0x8000)
                t = "Lock";
            else if (s.flags & 0x10)
                t = "Play";
            else
                t = "Open";
            sprintf(p[4], "%s", FUN_004c5740(t));
        }
        p[4] += strlen(p[4]) + 1;

        sprintf(p[5], "%d", s.field_0);
        p[5] += strlen(p[5]) + 1;

        sprintf(p[6], "%d", s.metal * 100);
        p[6] += strlen(p[6]) + 1;

        sprintf(p[7], "%d", s.energy * 100);
        p[7] += strlen(p[7]) + 1;

        sprintf(p[8], "%d", s.players);
        p[8] += strlen(p[8]) + 1;

        if ((s.flags & 0x1800) == 0) {
            sprintf(p[9], "%s", FUN_004c5740("No"));
        } else if ((s.flags & 0x1800) == 0x800) {
            sprintf(p[9], "%s", FUN_004c5740("Yes"));
        } else {
            sprintf(p[9], "%s", FUN_004c5740("DM"));
        }
        p[9] += strlen(p[9]) + 1;

        sprintf(p[10], "%s", FUN_004c5740((s.flags & 0x100) ? "Blk" : "Gray"));
        p[10] += strlen(p[10]) + 1;

        sprintf(p[11], "%s", FUN_004c5740((s.flags & 0x200) ? "No" : "Yes"));
        p[11] += strlen(p[11]) + 1;

        rec += 0x54;
    }

    FUN_004a32a0(&g_game->sub, "GAMENAME", (char*)g_game->data[1], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "PLAYERS", (char*)g_game->data[2], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "MAPNAME", (char*)g_game->data[3], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "STATUS", (char*)g_game->data[4], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "METAL", (char*)g_game->data[6], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "ENERGY", (char*)g_game->data[7], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "COMMANDER", (char*)g_game->data[9], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "LOS", (char*)g_game->data[11], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "PING", (char*)g_game->data[8], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "FULLMAP", (char*)g_game->data[10], g_game->field_4fd, 0);

    int idx = FUN_0049fdf0(gadget->entries, "GAMENAME", 2);
    if (idx != -1) {
        FUN_00441220(&g_game->sub, &gadget->entries[idx]);
    }
    return 1;
}
