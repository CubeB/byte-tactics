// Decompiled by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// Partial, 63.3%. Clears 15 buffers, displays flags/count in native order and calls the
// locale getter with no arguments. Packed settings retain the native field offsets.
// Frame, dead comparison stores and record-setting loads remain different.
#include <vector>

struct Guid_00441460 {
    unsigned long d1, d2, d3, d4;
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
    Net_00441460 net;
    char unknown_4e1[0x4fd - 0x4e1];
    int field_4fd;
    char unknown_501[0x519 - 0x501];
    Sub_00441460 sub;
    char unknown_529[0x2a47 - 0x529];
    void* data[16];
    char unknown_2a87[0x2aa7 - 0x2a87];
    char* desc;
    char unknown_2aab[0x37e1b - 0x2aab];
    int field_37e1b;
    char unknown_37e1f[0x39201 - 0x37e1f];
    char provider[0x10];
    char unknown_39211[1];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Settings_00441460 {
    unsigned short field_0;
    unsigned short flags;
    unsigned short field_2;
    unsigned short players;
    unsigned short energy;
    unsigned short metal;
    unsigned short field_4;
    unsigned short version;
};
#pragma pack(pop)

struct Gadget_00441460 {
    int unknown_0;
    char* entries;
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
char* FUN_0049f580();
int __stdcall FUN_0049fdf0(void* entries, const char* name, int type);
void __stdcall FUN_00441220(Sub_00441460* sub, void* entry);

// FUNCTION: 0x441460
int __stdcall FUN_00441460(Gadget_00441460* gadget) {
    int count;
    char* p[21];
    char names[0x20];
    char temp[0x129];
    Settings_00441460 settings;
    char* message;
    int i;

    {
        Guid_00441460* guid = (Guid_00441460*)g_game->provider;
        if (memcmp(guid, &DAT_004fcdc8, 0x10) == 0 || memcmp(guid, &DAT_004fcda8, 0x10) == 0)
            goto chain2;
        if (memcmp(guid, &DAT_004fcd98, 0x10) == 0) {
            message = "Updating...";
            goto done;
        }
        count = memcmp(guid, &DAT_004fcdb8, 0x10);
    chain2:
        if (memcmp(guid, &DAT_004fcdc8, 0x10) == 0) {
            message = "Connecting  (ESC to abort)";
            goto done;
        }
        if (memcmp(guid, &DAT_004fcda8, 0x10) == 0) {
            message = "Updating...";
            goto done;
        }
        if (memcmp(guid, &DAT_004fcd98, 0x10) == 0) {
            message = "Connecting  (ESC to abort)";
            goto done;
        }
        count = memcmp(guid, &DAT_004fcdb8, 0x10);
        message = "Connecting  (ESC to abort)";
    done:;
    }

    FUN_004abd90(&g_game->sub, FUN_004c5740(message), 0x96, 0, 1);
    FUN_004ab170(&g_game->sub, g_game->field_37e1b, 0);
    FUN_004c69a0(g_game->field_37e1b);
    FUN_004c63a0();
    FUN_004c63a0();

    count = FUN_004c9e50(&g_game->net, g_game->desc, 0);
    FUN_004a9660(&g_game->sub);
    if (count < 0) {
        return 0;
    }

    for (i = 0; i < 15; i++) {
        p[i + 1] = (char*)g_game->data[i + 1];
        memset(p[i + 1], 0, 0xa00);
    }

    p[0] = (char*)g_game->desc + 0x18;
    for (i = 0; i < count; i++) {
        char* q;
        char* rec;
        settings = *(Settings_00441460*)(p[0] - 0x14);
        memcpy(names, p[0], 0x20);
        rec = names;

        strncpy(p[1], names, 0x10);
        p[1][0x10] = 0;
        p[1] += strlen(p[1]) + 1;

        sprintf(p[2], "%d/%d", settings.flags & 0xf, *(int*)(p[0] - 4));
        p[2] += strlen(p[2]) + 1;

        memset(temp, 0, 0x80);
        strncpy(temp, names + 0x10, 0xf);
        q = temp + strlen(temp);
        while (q != temp) {
            q--;
            if (*q != ' ')
                break;
            *q = 0;
        }
        if (FUN_0049f580() != 0) {
            if (_strcmpi(FUN_0049f580(), "english") != 0) {
                _strlwr(temp);
                strncpy(temp, FUN_004c5740(temp), 0x80);
                temp[0x7f] = 0;
            }
        }
        strcpy(p[3], temp);
        p[3] += strlen(p[3]) + 1;

        if ((int)(settings.version & 0xff) < (int)*(signed char*)((char*)g_game + 1)) {
            sprintf(p[4], "%s", FUN_004c5740("VER!"));
        } else {
            const char* t;
            if (settings.flags & 0x8000)
                t = "Lock";
            else if (settings.flags & 0x10)
                t = "Play";
            else
                t = "Open";
            sprintf(p[4], "%s", FUN_004c5740(t));
        }
        p[4] += strlen(p[4]) + 1;

        sprintf(p[5], "%d", settings.field_0);
        p[5] += strlen(p[5]) + 1;

        sprintf(p[6], "%d", settings.metal * 100);
        p[6] += strlen(p[6]) + 1;

        sprintf(p[7], "%d", settings.energy * 100);
        p[7] += strlen(p[7]) + 1;

        sprintf(p[8], "%d", settings.players);
        p[8] += strlen(p[8]) + 1;

        if ((settings.flags & 0x1800) == 0) {
            sprintf(p[9], "%s", FUN_004c5740("No"));
        } else if ((settings.flags & 0x1800) == 0x800) {
            sprintf(p[9], "%s", FUN_004c5740("Yes"));
        } else {
            sprintf(p[9], "%s", FUN_004c5740("DM"));
        }
        p[9] += strlen(p[9]) + 1;

        sprintf(p[10], "%s", FUN_004c5740((settings.flags & 0x100) ? "Blk" : "Gray"));
        p[10] += strlen(p[10]) + 1;

        sprintf(p[11], "%s", FUN_004c5740((settings.flags & 0x200) ? "No" : "Yes"));
        p[11] += strlen(p[11]) + 1;

        p[0] += 0x54;
        (void)rec;
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

    {
        int idx = FUN_0049fdf0(gadget->entries, "GAMENAME", 2);
        if (idx != -1) {
            FUN_00441220(&g_game->sub, gadget->entries + idx * 0x15b);
        }
    }
    return 1;
}
