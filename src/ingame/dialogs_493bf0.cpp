// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Sonnet 5.5, edited by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, checked by GPT-6, finished by Claude Opus 5.5. Names are provisional.
//
// MATCH (Claude Opus 5.5, #5294). The last 0.6% was the SIB base/index order
// of the two byte stores `g_game->field_2bf1[n] = v` and
// `g_game->field_2bf1[d] = 1`: the original has `[edx + esi + 0x2bf1]` and
// `[edx + eax + 0x2bf1]` (g_game in the base slot), every earlier version had
// the index there. The rule, measured with `tools/c2prio.py --symbols` and
// enum padding in scratch copies:
//
//   In the address add, the operand whose front-end symbol id is larger
//   (compared modulo 65536) goes into the SIB base slot.
//
// With `extern g_game;` at file scope its id (29010) is below every local of
// the function (n 29077, d 29113), so n and d take the base slot. The
// measurements that pin it down:
//   * padding before the file-scope `extern g_game`: MATCH only while g_game
//     stays below 65536 and n wraps past it (an enum of 36458 to 36524
//     entries); at 36440 only d has wrapped and only the d store is fixed
//     (99.7%); padding of 36530 wraps g_game too and both stores flip back;
//   * the same padding between g_game and the function: MATCH at 36500,
//     40000, 50000 and 60000 entries (n and d wrapped, g_game not);
//   * padding at the end of the file (the file total alone): no effect.
// A function-scope extern declared after n and d is the plausible spelling
// that gives g_game the larger id: the front end numbers the global when it
// first sees a declaration, so with no file-scope declaration it comes after
// the locals. A block-scope `extern` added next to a file-scope one changes
// nothing (the symbol already exists), and n and d must be declared before
// the extern, so both are function-scope locals here. The other explanation
// is a lost header prefix that puts the 65536 wrap between g_game and this
// function, as in the 0x41b2e0 / 0x471de0 family (docs/c2-regalloc.md,
// "Symbol ids"), but no real header set reaches that.
//
// Spellings earlier passes found that the match still needs:
//  * The strlen block only lands when the four statements that open it are
//    in this order: oldmode, to, base, saved (all 24 orders scored; saved
//    must be last).
//  * The guard on p[1] is `if (!cond) goto skip0;` followed by the body, and
//    `skip0:;` immediately before `after:`. The natural `if (cond) { ... }`
//    or `goto after` costs 9 points; the empty statement is load bearing.
//  * The 0x37f2f bit test materialises the byte (`mov dl, [ecx + 0x37f2f];
//    shr dl, 1; test al, dl`) and pushes the flags dword, so `flags` is an
//    `int`, the field is an `unsigned short`, and the shifted value is cast
//    back to `(unsigned char)`. Every byte-field spelling folds to
//    `test byte ptr [ecx + 0x37f2f], 2`.
//  * `mode` and `oldmode` are both `int` (oldmode wins ebx, mode lives at
//    [esp+0x10]); `gadget->field_60` is read directly rather than through an
//    `id` local; the digit case computes `to` before the memset;
//    `sizeof(Player_00493bf0)` is 0x14b.
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
    int oldmode;
    int mode;
    int n;
    int d;
    // Declared here, after n and d, not at file scope: see the notes above.
    extern Game* g_game;
    Entry_00493bf0* entries = gadget->layer->entries;
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
