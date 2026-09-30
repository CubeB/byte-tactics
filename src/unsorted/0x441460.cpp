// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1,
// finished by space-bunny-free. Names are provisional.
// 80.5%, not a MATCH (was 80.4%). Exact so far: the whole local frame (buf sized
// 0x139 reserves the original's 0x1b4 frame and the parameter reads at
// [esp+0x1d0]), the settings copy at SETBUF = buf+0x119, the record walk reading
// field_10 straight from p[0]-4 (keeping a `rec` local alive made MSVC park it in
// ebp and cost 2%), and the strlwr block (the FUN_004c5740 result must be read
// into a local before the strncpy, else MSVC pushes the 0x80 first).
// Still differs:
//   * the provider-guid chain: the original keeps a dead-looking `count = memcmp`
//     store at the end of the FIRST chain (sbb edx,edx / sbb edx,-1 / mov
//     [esp+0x10],edx) and falls through to the cd98 compare, while MSVC deletes
//     that store here. Writing the chain as an if/else-if or as
//     `A != 0 && B != 0` is worse: MSVC proves both arms dead and drops the 98
//     and b8 compares entirely (5 compares instead of 8, 1796 bytes).
//   * count is stored to [esp+0x10] before the FUN_004c9e50 call here, while the
//     original stores ebx only in the count>0 branch just before the loop
//     (a separate loop local `int n = count` does not change that).
//   * the zeroing loop loads g_game->data[i] as [edx+eax+0x2a47] against the
//     original's [eax+edx+0x2a47] (same registers, swapped ModRM base/index).
//   * the p[4..11] sprintf block re-loads the settings copy instead of keeping
//     field_0 in ebp (the original's struct copy uses ebp for word 0 and the
//     value stays live to `and ebp,0xffff`), and (flags >> 15) & 1 folds to
//     `test ah,0x80` where the original keeps `mov edx,eax / shr edx,0xf /
//     test dl,1`.
#include <string.h>
#include <stdio.h>

struct Guid_00441460 {
    unsigned long d1, d2, d3, d4;
};

struct Sub_00441460 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
struct Settings_00441460 {
    unsigned short field_0;
    unsigned short flags;
    unsigned short field_4;
    unsigned short field_6;
    unsigned short field_8;
    unsigned short field_a;
    unsigned short field_c;
    unsigned short version;
};

struct Record_00441460 {
    Settings_00441460 settings;
    int field_10;
    char name[0x20];
    char name2[0x20];
};

struct Game_00441460 {
    char unknown_0;
    signed char field_1;
    char unknown_2[0x14 - 2];
    char unknown_14[0x4cd];
    char unknown_4e1[0x4fd - 0x4e1];
    int field_4fd;
    char unknown_501[0x519 - 0x501];
    Sub_00441460 sub;
    char unknown_529[0x2a47 - 0x529];
    void* data[16];
    char unknown_2a87[0x2aa7 - 0x2a87];
    void* desc;
    char unknown_2aab[0x37e1b - 0x2aab];
    int field_37e1b;
    char unknown_37e1f[0x39201 - 0x37e1f];
    char provider[0x10];
    char unknown_39211[1];
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
int __stdcall FUN_004c9e50(char* net, void* desc, int a);
void __stdcall FUN_004a9660(Sub_00441460* sub);
void __stdcall FUN_004a32a0(Sub_00441460* sub, const char* name, char* text, int count, int flag);
char* FUN_0049f580();
int __stdcall FUN_0049fdf0(void* entries, const char* name, int type);
void __stdcall FUN_00441220(Sub_00441460* sub, char* entry);

// FUNCTION: 0x441460
int __stdcall FUN_00441460(Gadget_00441460* gadget) {
    int count;
    int i;
    char* p[21];
    char names[0x20];
    char buf[0x139];
    const char* msg;
    char* lang;
#define temp (buf)
#define SETBUF ((Settings_00441460*)(buf + 0x119))

    if (memcmp(g_game->provider, &DAT_004fcdc8, 0x10) == 0
        || memcmp(g_game->provider, &DAT_004fcda8, 0x10) == 0)
        goto second;
    if (memcmp(g_game->provider, &DAT_004fcd98, 0x10) == 0) {
        msg = "Updating...";
        goto shown;
    }
    count = memcmp(g_game->provider, &DAT_004fcdb8, 0x10) != 0;
second:
    if (memcmp(g_game->provider, &DAT_004fcdc8, 0x10) == 0) {
        msg = "Connecting  (ESC to abort)";
        goto shown;
    }
    if (memcmp(g_game->provider, &DAT_004fcda8, 0x10) == 0) {
        msg = "Updating...";
        goto shown;
    }
    if (memcmp(g_game->provider, &DAT_004fcd98, 0x10) == 0) {
        msg = "Connecting  (ESC to abort)";
        goto shown;
    }
    count = memcmp(g_game->provider, &DAT_004fcdb8, 0x10) != 0;
    msg = "Connecting  (ESC to abort)";
shown:
    FUN_004abd90(&g_game->sub, FUN_004c5740(msg), 0x96, 0, 1);
    FUN_004ab170(&g_game->sub, g_game->field_37e1b, 0);
    FUN_004c69a0(g_game->field_37e1b);
    FUN_004c63a0();
    FUN_004c63a0();

    count = FUN_004c9e50((char*)&g_game->unknown_14, g_game->desc, 0);
    FUN_004a9660(&g_game->sub);
    if (count < 0)
        return 0;

    i = 0;
    do {
        i++;
        p[i] = (char*)g_game->data[i];
        memset(p[i], 0, 0xa00);
    } while (i < 15);

    p[0] = (char*)g_game->desc + 0x18;
    if (count > 0) {
        do {
            char* e;
            *SETBUF = ((Record_00441460*)(p[0] - 0x14))->settings;
            memcpy(names, p[0], 0x20);

            strncpy(p[1], names, 0x10);
            p[1][0x10] = 0;
            p[1] += strlen(p[1]) + 1;
            sprintf(p[2], "%d/%d", SETBUF->flags & 0xf, ((Record_00441460*)(p[0] - 0x14))->field_10);
            p[2] += strlen(p[2]) + 1;

            memset(temp, 0, 0x80);
            strncpy(temp, names + 0x10, 0xf);
            e = temp + strlen(temp);
            while (e != temp) {
                e--;
                if (*e != ' ')
                    break;
                *e = 0;
            }
            if (FUN_0049f580() != 0) {
                if (_strcmpi(FUN_0049f580(), "english") != 0) {
                    _strlwr(temp);
                    lang = FUN_004c5740(temp);
                    strncpy(temp, lang, 0x80);
                    temp[0x7f] = 0;
                }
            }
            strcpy(p[3], temp);
            p[3] += strlen(p[3]) + 1;

            if ((SETBUF->version & 0xff) >= (int)g_game->field_1) {
                if ((SETBUF->flags >> 15) & 1)
                    msg = "Lock";
                else if ((SETBUF->flags >> 4) & 1)
                    msg = "Play";
                else
                    msg = "Open";
                sprintf(p[4], "%s", FUN_004c5740(msg));
            } else {
                sprintf(p[4], "%s", FUN_004c5740("VER!"));
            }
            p[5] = p[4] + strlen(p[4]) + 1;
            sprintf(p[5], "%d", SETBUF->field_0);
            p[5] += strlen(p[5]) + 1;
            sprintf(p[6], "%d", SETBUF->field_a * 100);
            p[6] += strlen(p[6]) + 1;
            sprintf(p[7], "%d", SETBUF->field_8 * 100);
            p[7] += strlen(p[7]) + 1;
            sprintf(p[8], "%d", SETBUF->field_6);
            p[8] += strlen(p[8]) + 1;

            if ((SETBUF->flags & 0x1800) == 0)
                msg = "No";
            else if ((SETBUF->flags & 0x1800) == 0x800)
                msg = "Yes";
            else
                msg = "DM";
            sprintf(p[9], "%s", FUN_004c5740(msg));
            p[9] += strlen(p[9]) + 1;

            sprintf(p[10], "%s", FUN_004c5740((SETBUF->flags >> 8) & 1 ? "Blk" : "Gray"));
            p[10] += strlen(p[10]) + 1;
            sprintf(p[11], "%s", FUN_004c5740((SETBUF->flags >> 9) & 1 ? "No" : "Yes"));
            p[11] += strlen(p[11]) + 1;

            p[0] += 0x54;
        } while (--count);
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

    i = FUN_0049fdf0(gadget->entries, "GAMENAME", 2);
    if (i != -1)
        FUN_00441220(&g_game->sub, gadget->entries + i * 0x15b);
    return 1;
}
