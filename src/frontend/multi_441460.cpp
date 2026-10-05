// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1,
// finished by space-bunny-free, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by deepseek-v4.1-flash. Names are provisional.
//
// MATCH (1874 of 1874 bytes). The last residual was the zeroing loop's load
// g_game->data[i]: [edx+eax+0x2a47] where the original has [eax+edx+0x2a47]
// (SIB base and index swapped, same registers). About 25 source spellings were
// tried (struct element `.ptr`, `i[arr]`, pointer arithmetic with i*4, i<<2, a
// byte-offset loop var, for vs do-while, unsigned/char index, a cached `g_game`
// local, a block-local pointer with a reference as in 0x409730) and all emit
// the same bytes. The fix is the include set: `#include <stdio.h>
// #include <string.h> #include <ddraw.h>`. tools/headers.py found 22 header
// sets that make the function MATCH (smallest: <stdio.h> <ddraw.h>), so the
// swapped SIB byte was translation-unit state from the original's header list,
// not a source difference (same family as 0x408f30's wall).
//
// What made the big jump (84.1% -> 99.8%), so nobody has to rediscover it:
//   * SETTINGS ARE A SEPARATE, NEVER-ADDRESS-TAKEN LOCAL STRUCT (`sb`), not a slice of
//     the temp buffer. Then the struct copy `sb.s = *(Settings*)(p[0] - 0x14)` compiles
//     to the original's `lea ecx,[esi-0x14]` + four `mov reg,[ecx+N]` / store pairs, the
//     later reads of field_0 and field_8 are value-numbered to ebp and ebx by the
//     compiler (no `f0`/`f8` locals, no `rdw` pointer needed), and the flags dword in
//     ebx for p[9..11] appears by itself. The old 'buf + 0x119' macro made the whole
//     buffer escape (strncpy(temp, ...)), so every read was reloaded from the frame.
//   * The struct is `{ char pre[0x99]; Settings s; char pad[0x13]; }` (0x1a1 offset,
//     0x23-byte tail) because MSVC orders frame objects by size: it must be bigger than
//     `temp` (0x80) and `names` (0x20) to land after them, as in the original frame.
//   * The settings flags word is a set of `unsigned short` BIT-FIELDS (players:4,
//     playing:1, black:1 (bit 8), nocmd:1 (bit 9), mode:2 (bits 11-12), lock:1 (bit
//     15)). That reproduces `mov edx,eax / shr edx,0xf / test dl,1`, `shr al,4`,
//     `test ax,ax / cmp ax,0x800` and the bh/bl byte tests exactly; plain masks and
//     shifts fold to `test ah,0x80`.
//   * `p[4] += strlen(p[4]) + 1;` (the earlier `p[5] = p[4] + ...` trick wrote the
//     status text into the wrong column buffer, a semantic bug that only helped the score).
//   * `char* dsc = (char*)g_game->desc;` before `if (count > 0)` and `p[0] = dsc + 0x18;`
//     inside it gives the original's `mov ecx,[eax+0x2aa7] / jle / lea esi,[ecx+0x18]`.
//   * a separate loop counter `left` (`left = count;` at loop entry) gives the original's
//     `mov [esp+0x10],ebx` after the `jle` instead of before the 0x4a9660 call.
//   * THE DEAD memcmp STORES: the original keeps both `x = memcmp(.., DAT_004fcdb8)` stores
//     (sbb/sbb/mov [esp+0x10]) although nothing reads them. MSVC deletes a dead store to
//     a plain local, and deletes the first of two. Writing them as an int store through
//     an element of the p[] array (`*(int*)&p[20] = ...`, element 20 is never used) keeps
//     both, as a compiler temp that shares the [esp+0x10] slot with `left`, exactly like
//     the original. `if (PE(b8)) ; else store` (the compare once for both) is the shape
//     that keeps the `je`-over-`sbb` form.
//   * THE BLOCK LAYOUT of the provider chain ([store][CONN: mov+jmp][UPD][shown], every
//     jump to the two shared blocks) comes from structuring the second chain as
//     `if (c8) goto conn; if (!a8) { if (!98) {store} goto conn; }` and letting the a8
//     case FALL INTO the `upd:` label (which is followed by `conn:`). With a plain
//     `if (a8) goto upd;` MSVC moves a copy of the UPD block next to the a8 test (97.9%).
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
// field_4..version. Read by the original as unaligned dwords (see the notes above).
struct Settings_00441460 {
    unsigned short field_0;
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

extern Game* g_game;
extern Guid_00441460 DAT_004fcdc8;
extern Guid_00441460 DAT_004fcda8;
extern Guid_00441460 DAT_004fcd98;
extern Guid_00441460 DAT_004fcdb8;

char* __stdcall FUN_004c5740(const char* text);
void __stdcall FUN_004abd90(Sub_00441460* sub, char* text, int a, int b, int c);
void __stdcall FUN_004ab170(Sub_00441460* sub, int a, int b);
void __stdcall SetOffscreenSurface(int a);
void FlipScreen();
int __stdcall HAPINET_getgames(char* net, void* desc, int a);
void __stdcall FUN_004a9660(Sub_00441460* sub);
void __stdcall FUN_004a32a0(Sub_00441460* sub, const char* name, char* text, int count, int flag);
char* FUN_0049f580();
int __stdcall FUN_0049fdf0(void* entries, const char* name, int type);
void __stdcall FUN_00441220(Sub_00441460* sub, char* entry);

// FUNCTION: 0x441460
int __stdcall FUN_00441460(Gadget_00441460* gadget) {
    int count;
    int left;
    int i;
    char* p[21];
    char names[0x20];
    char buf[0x80];
    struct { char pre[0x99]; Settings_00441460 s; char pad[0x13]; } sb;
    const char* msg;
    char* lang;
#define temp (buf)

#define PE(g) (memcmp(g_game->provider, &(g), 0x10) == 0)
    if (!PE(DAT_004fcdc8) && !PE(DAT_004fcda8)) {
        if (PE(DAT_004fcd98))
            goto upd;
        if (PE(DAT_004fcdb8))
            ;
        else
            *(int*)&p[20] = memcmp(g_game->provider, &DAT_004fcdb8, 0x10) != 0;
    }
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
    FUN_004abd90(&g_game->sub, FUN_004c5740(msg), 0x96, 0, 1);
    FUN_004ab170(&g_game->sub, g_game->field_37e1b, 0);
    SetOffscreenSurface(g_game->field_37e1b);
    FlipScreen();
    FlipScreen();

    count = HAPINET_getgames((char*)&g_game->unknown_14, g_game->desc, 0);
    FUN_004a9660(&g_game->sub);
    if (count < 0)
        return 0;

    i = 0;
    do {
        i++;
        p[i] = (char*)g_game->data[i];
        memset(p[i], 0, 0xa00);
    } while (i < 15);

    char* dsc = (char*)g_game->desc;
    if (count > 0) {
        p[0] = dsc + 0x18;
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

            if ((sb.s.version & 0xff) >= (int)g_game->field_1) {
                if (sb.s.lock)
                    msg = "Lock";
                else if (sb.s.playing)
                    msg = "Play";
                else
                    msg = "Open";
                sprintf(p[4], "%s", FUN_004c5740(msg));
            } else {
                sprintf(p[4], "%s", FUN_004c5740("VER!"));
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
            sprintf(p[9], "%s", FUN_004c5740(msg));
            p[9] += strlen(p[9]) + 1;

            sprintf(p[10], "%s", FUN_004c5740(sb.s.black ? "Blk" : "Gray"));
            p[10] += strlen(p[10]) + 1;
            sprintf(p[11], "%s", FUN_004c5740(sb.s.nocmd ? "No" : "Yes"));
            p[11] += strlen(p[11]) + 1;

            p[0] += 0x54;
        } while (--left);
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