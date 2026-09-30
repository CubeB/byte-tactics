// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by
// space-bunny-free. Names are provisional.
// Partial: 68.5% (1102 of 1116 bytes). Still differing:
//  * entries lives in ESI where the original keeps it in EBX. Every other
//    saved-mode difference here (the atoi index in ESI, oldmode in BL, mode in
//    [esp+0x10] rather than EBX, and the tail's ebx/ecx/edx) is downstream of
//    that one rotation. A throwaway `lay` local and moving the `n` and `mode`
//    declarations around do not demote it.
//  * the 0x37f2f bit 1 test: the original materialises `shr dl,1` and does
//    `test al,dl`; every spelling tried folds to `test byte ptr [ecx+x],2`.
//  * the players[d] address in the digit branch: the original splits it as
//    base g_game+d with index 330*d, ours as base g_game with index 331*d.
//  * `lea ecx,[eax*8]` versus `mov ecx,eax / shl ecx,3` at the _strnicmp site.
//  * the mode/oldmode and g_game scratch registers of the tail block.
// What did work (kept below): the third argument of FUN_004a1080 is an int,
// not a char, so the reload of mode_2bf0 zero-extends through `xor edx,edx`
// and `mov dl`; and `saved` is a 10-byte struct copy, which leaves the three
// loads ahead of the three stores the original has.
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#pragma pack(push, 1)
struct Entry_00493bf0 {                // 0x15b bytes
    char state;                        // +0x00
    char unknown_1;                    // +0x01
    char name[0x10];                   // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6
    char unknown_b8[0x137 - 0xb8];
    unsigned char field_137;           // +0x137
    char unknown_138[0x15b - 0x138];
};

struct Layer_00493bf0 {
    char unknown_0[4];
    Entry_00493bf0* entries;           // +0x04
    char unknown_8[0x20 - 0x8];
    int field_20;                      // +0x20
};

struct Gadget_00493bf0 {
    char unknown_0[0x18];
    Layer_00493bf0* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
};

struct Player_00493bf0 {               // 0x14b bytes
    char unknown_0[4];
    int field_4;                       // +0x04
    char unknown_8[0x14b - 0x8];
};

struct Saved_00493bf0 {                 // 10 bytes
    int a;                            // +0x00
    int b;                            // +0x04
    short c;                          // +0x08
};

struct Bit_00493bf0 {
    unsigned char bit0 : 1;            // +0x00
    unsigned char bit1 : 1;
    unsigned char bits2_7 : 6;
};

struct Flags16_00493bf0 {
    unsigned short bits0_7 : 8;
    unsigned short bit8 : 1;
    unsigned short bits9_15 : 7;
};

struct Game_00493bf0 {
    char unknown_0[0x519];
    Gadget_00493bf0 gadget;            // +0x519
    char unknown_57d[0x1b63 - 0x57d];
    Player_00493bf0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bee - 0x2a43];
    Flags16_00493bf0 field_2bee;       // +0x2bee
    unsigned char mode_2bf0;           // +0x2bf0
    unsigned char field_2bf1[11];      // +0x2bf1
    char unknown_2bfc[0x37ebe - 0x2bfc];
    unsigned short flags_37ebe;        // +0x37ebe
    char unknown_37ec0[0x37f2f - 0x37ec0];
    Bit_00493bf0 field_37f2f;          // +0x37f2f
};
#pragma pack(pop)

extern Game_00493bf0* g_game;
extern int DAT_005091cc;
extern char DAT_0051e788[];
extern char DAT_0050940c[];            // "LIVEPLYR"
extern char DAT_00503130[];            // "SmallButton"
extern char DAT_00509400[];            // "SENDTYPE"
extern char DAT_005093f8[];            // "SENDTO"
extern char DAT_00506578[];            // "TALK"
extern char DAT_005093f4[];            // ",:;"
extern char DAT_005093ec[];            // "Enemies"
extern char DAT_00508384[];            // "Allies"

void __stdcall FUN_0047f1a0(char* name, int flag);
void __stdcall FUN_004a1080(Gadget_00493bf0* obj, char* name, int value);
int __stdcall FUN_004a0ff0(Gadget_00493bf0* obj, int index);
int __stdcall FUN_004a0f60(Gadget_00493bf0* obj, char* name);
Entry_00493bf0* __stdcall FUN_004a0010(Entry_00493bf0* entries, char* name);
void __stdcall FUN_004a0d00(Gadget_00493bf0* obj, char* name, char* text);
void __stdcall FUN_004a9660(Gadget_00493bf0* obj);
void __stdcall FUN_0049fa90(void* obj);
void __stdcall FUN_004ab0a0(Gadget_00493bf0* obj);
void FUN_00493ae0();
void FUN_00494050();
int __stdcall FUN_00417b50(char* cmd, int flags);
int __stdcall FUN_0049fd60(Gadget_00493bf0* gadget, char* name);
int __stdcall FUN_0049fdf0(Entry_00493bf0* entries, char* name, int type);
void __stdcall FUN_0049fc50(Gadget_00493bf0* obj, int index);
void __stdcall FUN_00463e50(Player_00493bf0* from, char* text, int param_3, char* to);

// FUNCTION: 0x493bf0
void __stdcall FUN_00493bf0(Gadget_00493bf0* gadget)
{
    char buf2[0x12c];
    char buf[0x100];
    Entry_00493bf0* entries = gadget->layer->entries;
    int id = gadget->field_60;
    if (id == -1) {
        g_game->flags_37ebe &= ~4;
        return;
    }
    if (_strnicmp(entries[id].name, DAT_0050940c, 8) == 0) {
        FUN_0047f1a0(DAT_00503130, 0);
        g_game->mode_2bf0 = 3;
        FUN_004a1080(gadget, DAT_00509400, g_game->mode_2bf0);
        int n = atoi(&entries[gadget->field_60].name[8]);
        unsigned char v = (unsigned char)FUN_004a0ff0(gadget, gadget->field_60);
        g_game->field_2bf1[n] = v;
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        goto tail;
    }
    if (FUN_0049fd60(gadget, DAT_005093f8)) {
        FUN_0047f1a0(DAT_00503130, 0);
        unsigned char v = (unsigned char)FUN_004a0ff0(gadget, gadget->field_60);
        g_game->field_2bee.bit8 = v & 1;
        FUN_004a0d00(gadget, DAT_00506578, DAT_0051e788);
        FUN_004a9660(gadget);
        FUN_00494050();
        FUN_004ab0a0(gadget);
        return;
    }
    if (FUN_0049fd60(gadget, DAT_00509400)) {
        FUN_0047f1a0(DAT_00503130, 0);
        g_game->mode_2bf0 = (unsigned char)FUN_004a0f60(gadget, DAT_00509400);
        if (g_game->mode_2bf0 >= 4)
            g_game->mode_2bf0 = 0;
        FUN_00493ae0();
        FUN_004ab0a0(gadget);
        goto tail;
    }
    if (FUN_0049fd60(gadget, DAT_00506578)) {
        Entry_00493bf0* talk = FUN_004a0010(entries, DAT_00506578);
        int mode = g_game->mode_2bf0;
        lstrcpynA(buf, (char*)talk + 0xb6, 0x100);
        char* p = buf;
        while (*p && *p == ' ')
            p++;
        if (*p == '+') {
            int flags = 1;
            if (flags & g_game->field_37f2f.bit1)
                flags = 7;
            if (DAT_005091cc)
                flags |= 2;
            int r = FUN_00417b50(p + 1, flags);
            entries = gadget->layer->entries;
            if (r & 2)
                mode = 0;
        }
        if (strlen(p) != 0) {
            unsigned char oldmode = g_game->mode_2bf0;
            Player_00493bf0* base = &g_game->players[g_game->localPlayer];
            Saved_00493bf0 saved = *(Saved_00493bf0*)g_game->field_2bf1;
            char* to = 0;
            if (p[1] > ' ' && strchr(DAT_005093f4, p[1]) != 0) {
                if (isdigit(p[0])) {
                    int d = p[0] - '0';
                    if (d < 0 || d > 9 || g_game->players[d].field_4 == 0)
                        goto clear;
                    p += 2;
                    mode = 3;
                    memset(g_game->field_2bf1, 0, 11);
                    g_game->field_2bf1[d] = 1;
                    to = (char*)&g_game->players[d] + 0x2b;
                } else {
                    int c = tolower(p[0]);
                    if (c == 'a') {
                        mode = 1;
                        to = DAT_00508384;
                    } else if (c == 'e') {
                        mode = 2;
                        to = DAT_005093ec;
                    } else {
                        goto after;
                    }
                    p += 2;
                }
            }
after:
            g_game->mode_2bf0 = (unsigned char)mode;
            memset(buf2, 0, sizeof(buf2));
            FUN_00463e50(base, p, 4, to);
            *(Saved_00493bf0*)g_game->field_2bf1 = saved;
            g_game->mode_2bf0 = oldmode;
        }
clear:
        memset(DAT_0051e788, 0, 0x81);
        g_game->field_2bee.bit8 = 0;
    }
tail:
    int index = FUN_0049fdf0(entries, DAT_00506578, 3);
    FUN_0049fc50(&g_game->gadget, index);
    g_game->gadget.layer->field_20 = FUN_0049fdf0(entries, DAT_00506578, 3);
}
