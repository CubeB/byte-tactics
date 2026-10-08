// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1,
// finished by space-bunny-free, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by deepseek-v4.1-flash. Names are provisional.
// Include set <stdio.h> <string.h> <ddraw.h>: gives the original SIB operand order.
#include <stdio.h>
#include <string.h>
#include <ddraw.h>

struct Guid_00441460 {
    unsigned long d1, d2, d3, d4;
};

struct Sub_00441460 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
// The record header: field_0, a 16-bit flag word with bit-fields at offset 2, then
// field_4..version. Read by the original as unaligned dwords.
struct Settings_00441460 {
    unsigned short field_0;
    // Bit-fields: reproduce the original shifts and byte tests.
    unsigned short players : 4;
    unsigned short playing : 1;
    unsigned short pad5 : 3;
    unsigned short black : 1;
    unsigned short nocmd : 1;
    unsigned short pad10 : 1;
    unsigned short mode : 2;
    unsigned short pad13 : 2;
    unsigned short lock : 1;
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

struct Game {
    char unknown_0;
    signed char version;
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
    int screen;
    char unknown_37e1f[0x39201 - 0x37e1f];
    char provider[0x10];
    char unknown_39211[1];
};
#pragma pack(pop)

struct Gadget_00441460 {
    int unknown_0;
    char* entries;
};

extern Game* g_game;
extern Guid_00441460 DAT_004fcdc8;
extern Guid_00441460 DAT_004fcda8;
extern Guid_00441460 DAT_004fcd98;
extern Guid_00441460 DAT_004fcdb8;

char* __stdcall Translate(const char* text);
void __stdcall OpenMessageBox(Sub_00441460* sub, char* text, int a, int b, int c);
void __stdcall BlitMenuLayers(Sub_00441460* sub, int a, int b);
void __stdcall SetOffscreenSurface(int a);
void FlipScreen();
int __stdcall HAPINET_getgames(char* net, void* desc, int a);
void __stdcall CloseTopScreen(Sub_00441460* sub);
void __stdcall ConfigureListBoxByName(Sub_00441460* sub, const char* name, char* text, int count, int flag);
char* GetPreferredLanguage();
int __stdcall FindGadgetIndex(void* entries, const char* name, int type);
void __stdcall UpdateGameSelection(Sub_00441460* sub, char* entry);

// FUNCTION: 0x441460
int __stdcall ConnectToGame(Gadget_00441460* gadget) {
    int count;
    int left;
    int i;
    char* p[21];
    char names[0x20];
    char buf[0x80];
    // Never address-taken, bigger than temp and names so it lands after them.
    struct { char pre[0x99]; Settings_00441460 s; char pad[0x13]; } sb;
    const char* msg;
    char* lang;
#define temp (buf)

#define PE(g) (memcmp(g_game->provider, &(g), 0x10) == 0)
    if (!PE(DAT_004fcdc8) && !PE(DAT_004fcda8)) {
        if (PE(DAT_004fcd98))
            goto upd;
        // Stores through the unused p[20]: keeps both dead memcmp results.
        if (PE(DAT_004fcdb8))
            ;
        else
            *(int*)&p[20] = memcmp(g_game->provider, &DAT_004fcdb8, 0x10) != 0;
    }
    // The a8 case falls into upd:, which is followed by conn:.
    if (PE(DAT_004fcdc8))
        goto conn;
    if (!PE(DAT_004fcda8)) {
        if (!PE(DAT_004fcd98)) {
            if (PE(DAT_004fcdb8))
                ;
            else
                *(int*)&p[20] = memcmp(g_game->provider, &DAT_004fcdb8, 0x10) != 0;
        }
        goto conn;
    }
upd:
    msg = "Updating...";
    goto shown;
conn:
    msg = "Connecting  (ESC to abort)";
shown:
    OpenMessageBox(&g_game->sub, Translate(msg), 0x96, 0, 1);
    BlitMenuLayers(&g_game->sub, g_game->screen, 0);
    SetOffscreenSurface(g_game->screen);
    FlipScreen();
    FlipScreen();

    count = HAPINET_getgames((char*)&g_game->unknown_14, g_game->desc, 0);
    CloseTopScreen(&g_game->sub);
    if (count < 0)
        return 0;

    i = 0;
    do {
        i++;
        p[i] = (char*)g_game->data[i];
        memset(p[i], 0, 0xa00);
    } while (i < 15);

    // dsc is loaded before the if and used inside it.
    char* dsc = (char*)g_game->desc;
    if (count > 0) {
        p[0] = dsc + 0x18;
        // Separate counter: its store lands after the jle.
        left = count;
        do {
            char* e;
            sb.s = *(Settings_00441460*)(p[0] - 0x14);
            memcpy(names, p[0], 0x20);

            strncpy(p[1], names, 0x10);
            p[1][0x10] = 0;
            p[1] += strlen(p[1]) + 1;
            sprintf(p[2], "%d/%d", sb.s.players, ((Record_00441460*)(p[0] - 0x14))->field_10);
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
            if (GetPreferredLanguage() != 0) {
                if (_strcmpi(GetPreferredLanguage(), "english") != 0) {
                    _strlwr(temp);
                    lang = Translate(temp);
                    strncpy(temp, lang, 0x80);
                    temp[0x7f] = 0;
                }
            }
            strcpy(p[3], temp);
            p[3] += strlen(p[3]) + 1;

            if ((sb.s.version & 0xff) >= (int)g_game->version) {
                if (sb.s.lock)
                    msg = "Lock";
                else if (sb.s.playing)
                    msg = "Play";
                else
                    msg = "Open";
                sprintf(p[4], "%s", Translate(msg));
            } else {
                sprintf(p[4], "%s", Translate("VER!"));
            }
            p[4] += strlen(p[4]) + 1;
            sprintf(p[5], "%d", sb.s.field_0);
            p[5] += strlen(p[5]) + 1;
            sprintf(p[6], "%d", sb.s.field_a * 100);
            p[6] += strlen(p[6]) + 1;
            sprintf(p[7], "%d", sb.s.field_8 * 100);
            p[7] += strlen(p[7]) + 1;
            sprintf(p[8], "%d", sb.s.field_6);
            p[8] += strlen(p[8]) + 1;

            if (sb.s.mode != 0) {
                if (sb.s.mode == 1)
                    msg = "Yes";
                else
                    msg = "DM";
            } else {
                msg = "No";
            }
            sprintf(p[9], "%s", Translate(msg));
            p[9] += strlen(p[9]) + 1;

            sprintf(p[10], "%s", Translate(sb.s.black ? "Blk" : "Gray"));
            p[10] += strlen(p[10]) + 1;
            sprintf(p[11], "%s", Translate(sb.s.nocmd ? "No" : "Yes"));
            p[11] += strlen(p[11]) + 1;

            p[0] += 0x54;
        } while (--left);
    }

    ConfigureListBoxByName(&g_game->sub, "GAMENAME", (char*)g_game->data[1], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "PLAYERS", (char*)g_game->data[2], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "MAPNAME", (char*)g_game->data[3], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "STATUS", (char*)g_game->data[4], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "METAL", (char*)g_game->data[6], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "ENERGY", (char*)g_game->data[7], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "COMMANDER", (char*)g_game->data[9], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "LOS", (char*)g_game->data[11], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "PING", (char*)g_game->data[8], g_game->field_4fd, 0);
    ConfigureListBoxByName(&g_game->sub, "FULLMAP", (char*)g_game->data[10], g_game->field_4fd, 0);

    i = FindGadgetIndex(gadget->entries, "GAMENAME", 2);
    if (i != -1)
        UpdateGameSelection(&g_game->sub, gadget->entries + i * 0x15b);
    return 1;
}