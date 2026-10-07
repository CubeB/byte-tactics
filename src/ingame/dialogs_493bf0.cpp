// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, edited by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, checked by GPT-6, finished by Claude Opus 5.5. Names are provisional.
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

struct Flags16_00493bf0 {
    unsigned short bits0_7 : 8;
    unsigned short bit8 : 1;
    unsigned short bits9_15 : 7;
};

struct Game {
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
    unsigned short field_37f2f;        // +0x37f2f, bit 1 is the "verbose" bit
};
#pragma pack(pop)

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

void __stdcall PlaySoundByName(char* name, int flag);
void __stdcall SetButtonStageByName(Gadget_00493bf0* obj, char* name, int value);
int __stdcall GetButtonStage(Gadget_00493bf0* obj, int index);
int __stdcall GetButtonStageByName(Gadget_00493bf0* obj, char* name);
Entry_00493bf0* __stdcall FUN_004a0010(Entry_00493bf0* entries, char* name);
void __stdcall GetGadgetText(Gadget_00493bf0* obj, char* name, char* text);
void __stdcall CloseTopScreen(Gadget_00493bf0* obj);
void __stdcall FUN_0049fa90(void* obj);
void __stdcall FUN_004ab0a0(Gadget_00493bf0* obj);
void ResetPlayerGadgets();
void OpenTalkDialog();
int __stdcall FUN_00417b50(char* cmd, int flags);
int __stdcall IsCurrentGadgetNamed(Gadget_00493bf0* gadget, char* name);
int __stdcall FindGadgetIndex(Entry_00493bf0* entries, char* name, int type);
void __stdcall FUN_0049fc50(Gadget_00493bf0* obj, int index);
void __stdcall SendChatMessage(Player_00493bf0* from, char* text, int param_3, char* to);

// FUNCTION: 0x493bf0
void __stdcall HandleTalkDialogEvent(Gadget_00493bf0* gadget)
{
    char buf2[0x12c];
    char buf[0x100];
    // mode and oldmode must both be int.
    int oldmode;
    int mode;
    int n;
    int d;
    // Declared here, after n and d, not at file scope: g_game's symbol id sets
    // the base/index order of the byte stores.
    extern Game* g_game;
    Entry_00493bf0* entries = gadget->layer->entries;
    // field_60 is read directly, not through an id local.
    if (gadget->field_60 == -1) {
        g_game->flags_37ebe &= ~4;
        return;
    }
    if (_strnicmp(entries[gadget->field_60].name, DAT_0050940c, 8) == 0) {
        PlaySoundByName(DAT_00503130, 0);
        g_game->mode_2bf0 = 3;
        SetButtonStageByName(gadget, DAT_00509400, g_game->mode_2bf0);
        n = atoi(&entries[gadget->field_60].name[8]);
        // Kept as the original has it: n is never range checked before it
        // indexes the 11-byte selection mask, so a "LIVEPLYR42" style name
        // writes outside field_2bf1. The neighbouring mode_2bf0 is clamped
        // (`if (g_game->mode_2bf0 >= 4) g_game->mode_2bf0 = 0;`), so the
        // omission looks like an oversight rather than a deliberate choice.
        unsigned char v = (unsigned char)GetButtonStage(gadget, gadget->field_60);
        g_game->field_2bf1[n] = v;
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
        goto tail;
    }
    if (IsCurrentGadgetNamed(gadget, DAT_005093f8)) {
        PlaySoundByName(DAT_00503130, 0);
        unsigned char v = (unsigned char)GetButtonStage(gadget, gadget->field_60);
        g_game->field_2bee.bit8 = v & 1;
        GetGadgetText(gadget, DAT_00506578, DAT_0051e788);
        CloseTopScreen(gadget);
        OpenTalkDialog();
        FUN_004ab0a0(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, DAT_00509400)) {
        PlaySoundByName(DAT_00503130, 0);
        g_game->mode_2bf0 = (unsigned char)GetButtonStageByName(gadget, DAT_00509400);
        if (g_game->mode_2bf0 >= 4)
            g_game->mode_2bf0 = 0;
        ResetPlayerGadgets();
        FUN_004ab0a0(gadget);
        goto tail;
    }
    if (IsCurrentGadgetNamed(gadget, DAT_00506578)) {
        Entry_00493bf0* talk = FUN_004a0010(entries, DAT_00506578);
        mode = g_game->mode_2bf0;
        lstrcpynA(buf, (char*)talk + 0xb6, 0x100);
        char* p = buf;
        while (*p && *p == ' ')
            p++;
        if (*p == '+') {
            int flags = 1;
            // The cast keeps the shift a 16-bit one, which is what stops MSVC
            // folding this into `test byte ptr [g_game + 0x37f2f], 2`.
            if (flags & (unsigned char)(g_game->field_37f2f >> 1))
                flags = 7;
            if (DAT_005091cc)
                flags |= 2;
            int r = FUN_00417b50(p + 1, flags);
            entries = gadget->layer->entries;
            if (r & 2)
                mode = 0;
        }
        if (strlen(p) != 0) {
            // These four statements stay in this order: oldmode, to, base, saved.
            oldmode = g_game->mode_2bf0;
            char* to = 0;
            Player_00493bf0* base = &g_game->players[g_game->localPlayer];
            Saved_00493bf0 saved = *(Saved_00493bf0*)g_game->field_2bf1;
            // Early out rather than a positive `if`, and the empty statement
            // at skip0 below is load bearing: either change costs 9 points.
            if (!(' ' < p[1] && strchr(DAT_005093f4, p[1]) != 0)) goto skip0;
            if (isdigit(p[0])) {
                d = p[0] - '0';
                if (d < 0 || d > 9 || g_game->players[d].field_4 == 0)
                    goto clear;
                p += 2;
                mode = 3;
                // to is computed before the memset.
                to = (char*)&g_game->players[d] + 0x2b;
                memset(g_game->field_2bf1, 0, 11);
                g_game->field_2bf1[d] = 1;
            } else {
                int c = tolower(p[0]);
                if (c != 'a') {
                    if (c == 'e') {
                        mode = 2;
                        to = DAT_005093ec;
                    } else {
                        goto after;
                    }
                } else {
                    mode = 1;
                    to = DAT_00508384;
                }
                p += 2;
            }
skip0:;
after:
            g_game->mode_2bf0 = mode;
            memset(buf2, 0, sizeof(buf2));
            SendChatMessage(base, p, 4, to);
            *(Saved_00493bf0*)g_game->field_2bf1 = saved;
            g_game->mode_2bf0 = oldmode;
        }
clear:
        memset(DAT_0051e788, 0, 0x81);
        g_game->field_2bee.bit8 = 0;
    }
tail:
    int index = FindGadgetIndex(entries, DAT_00506578, 3);
    FUN_0049fc50(&g_game->gadget, index);
    g_game->gadget.layer->field_20 = FindGadgetIndex(entries, DAT_00506578, 3);
}
