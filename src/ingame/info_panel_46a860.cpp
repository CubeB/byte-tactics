// Decompiled by deepseek-v4.1, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// Kept its own file: it matches only under /Gi, which the other functions do
// not use.
// Draws the selected unit / feature info panel (and the PFSTATE debug overlay).
#include <windows.h>
#include <stdio.h>

struct Game;
extern Game* g_game;

int GetScreenHeight();
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);
void __stdcall DrawFrame(void* dst, void* bmp, int x, int y);
void __stdcall SetTextColors(int param_1, int param_2);
int GetTextKeyColor();
void __stdcall SetFont(int param_1);
int GetFontHeight();
int __stdcall GetTextWidth(void* font, unsigned char* text);
void __stdcall DrawString(void* dst, unsigned char* text, int x, int y, int maxWidth);
char* __stdcall Translate(char* key);
int __stdcall GetBuildWeaponPercent(void* owner);
int __stdcall GetOrderTarget(void* unit);
int __stdcall GetOrderName(void* obj);
int __stdcall IsUnitVisibleToPlayer(void* map, void* u);
void __stdcall BlitSideLogoToRect(void* surf, void* player, void* rect, int dy);
unsigned short __stdcall FindUnitTypeId(const char* name);
int __stdcall FillRectangle(void* surface, void* rect, int color);

#include "../map/mission.h"

#pragma pack(push, 1)
struct Snapshot_0046a860 {
    int orderName;
    unsigned short selected, health, build;
    int weapons[3];
    float values[4];
    unsigned short targetType, targetHealth, feature;
    int button, width, height, reserved;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Player_0046a860 { char pad[0x27]; char* info; char pad2[331 - 0x2b]; };
#pragma pack(pop)

struct Rect_0046a860 {
    int left, top, right, bottom;
};

static void DrawBar_0046a860(void* surface, Rect_0046a860* bounds, int value, int maximum, int dy,
                             unsigned char* palette) {
    Rect_0046a860 rect = *bounds;
    rect.top = rect.top + dy;
    rect.bottom += dy;
    if (value < 0)
        value = 0;
    if (value > maximum)
        value = maximum;
    rect.right = rect.left + (bounds->right - bounds->left) * value / maximum;
    FillRectangle(surface, &rect, palette[10]);
    if (rect.right != bounds->right) {
        rect.left = rect.right + 1;
        rect.right = bounds->right;
        FillRectangle(surface, &rect, palette[4]);
    }
}

static float Positive_0046a860(float value) {
    float result = value > 0.0f ? value : 0.0f;
    return result;
}

struct Flags46b_3923b { unsigned short b0:1; unsigned short b1:1; };

// The game state this file reads, with the member names the other Game views
// use. The bit word shares a union with this view's bitfield type.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0xc];
    char* displayContext;                 // +0xc
    char unknown_10[0x531 - 0x10];
    int field_531;                        // +0x531
    char unknown_535[0x581 - 0x535];
    int selected;                         // +0x581
    char unknown_585[0xdcb - 0x585];
    unsigned char colors[16];             // +0xdcb
    char unknown_ddb[0x1b63 - 0xddb];
    union {                               // +0x1b63
        Player_0046a860 players[10];
        struct {
            char unknown_1b63[0x1cbe - 0x1b63];
            int field_1cbe;               // +0x1cbe, players[1] + 0x10
            char unknown_1cc2[0x1e09 - 0x1cc2];
            int field_1e09;               // +0x1e09, players[2] + 0x10
            char unknown_1e0d[0x1f54 - 0x1e0d];
            int field_1f54;               // +0x1f54, players[3] + 0x10
            char unknown_1f58[0x2851 - 0x1f58];
        };
    };
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;            // +0x2a43
    char unknown_2a44[0x2c8e - 0x2a44];
    unsigned short field_2c8e;            // +0x2c8e
    unsigned short field_2c90;            // +0x2c90
    char unknown_2c92[0x2cba - 0x2c92];
    unsigned short hoverUnitId;           // +0x2cba
    unsigned short cellFeature;           // +0x2cbc
    char unknown_2cbe[0x14233 - 0x2cbe];
    int mapWidthTiles;                    // +0x14233
    char unknown_14237[0x1426f - 0x14237];
    int features;                         // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    int cells;                            // +0x14287
    char unknown_1428b[0x1431f - 0x1428b];
    int scrollX;                          // +0x1431f
    int scrollY;                         // +0x14323
    char unknown_14327[0x14353 - 0x14327];
    int field_14353;                      // +0x14353
    char* units;                          // +0x14357
    char unknown_1435b[0x14367 - 0x1435b];
    int count;                            // +0x14367
    char unknown_1436b[0x1439b - 0x1436b];
    int unitDefs;                         // +0x1439b
    char unknown_1439f[0x147a7 - 0x1439f];
    int baseHeight;                       // +0x147a7
    char unknown_147ab[0x14833 - 0x147ab];
    unsigned short* sidePanelBotSeq[5];   // +0x14833
    char unknown_14847[0x37e1f - 0x14847];
    int width;                            // +0x37e1f
    int height;                           // +0x37e23
    char unknown_37e27[0x37e60 - 0x37e27];
    Snapshot_0046a860 selectionInfoCache; // +0x37e60
    char unknown_37e9c[0x37f3d - 0x37e9c];
    char sidePanels[5 * 0x232];           // +0x37f3d
    char unknown_38a37[0x38a3b - 0x38a37];
    int simStepsPending;                  // +0x38a3b
    char unknown_38a3f[0x38a47 - 0x38a3f];
    int ticks;                            // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* mapInfo;                     // +0x391e9
    char unknown_391ed[0x391f9 - 0x391ed];
    int fontComix;                        // +0x391f9
    char unknown_391fd[0x3923b - 0x391fd];
    union {                               // +0x3923b
        unsigned short flags_3923b;
        unsigned char flagsByte_3923b;
        Flags46b_3923b bits_3923b;
    };
};
#pragma pack(pop)

// FUNCTION: 0x46a860
void __stdcall DrawUnitInfoPanel(void* surface) {
    int yOffset = g_game->height - g_game->baseHeight;
    Snapshot_0046a860 snapshot;
    memset(&snapshot, 0, sizeof(snapshot));
    char text[256];

    unsigned short flags = g_game->flags_3923b;
    if (flags & 1 && flags & 2) {
        int y = 0x81;
        unsigned char player = g_game->playerIndex;
        char* table = *(char**)((char*)g_game + player * 331 + 0x1b8a);
        int idx = *(unsigned char*)(table + 0x95);
        do {
            int dy = GetScreenHeight() - 0x20;
            unsigned short* icon = *(unsigned short**)((char*)g_game + idx * 4 + 0x14833);
            int bmp = GetGafFrame(icon, 0);
            DrawFrame(surface, (void*)bmp, (short)*(unsigned short*)(bmp + 4) + y,
                         (short)*(unsigned short*)(bmp + 6) + dy);
            y += *(unsigned short*)bmp;
        } while (y < g_game->width);

        SetFont(g_game->fontComix);
        SetTextColors(0x53, GetTextKeyColor());
        // No pfable or field_c locals: the PFSTATE sprintf re-reads g_game->displayContext per argument.
        int pfstate = g_game->height - GetFontHeight() - 1;
        sprintf(text, "PFSTATE %d, PFABLE %d\n", *(unsigned char*)(g_game->displayContext + 0xf0) & 1,
                *(int*)(g_game->displayContext + 0x9c));
        DrawString(surface, (unsigned char*)text, 0x82, pfstate, -1);

        if (g_game->hoverUnitId != 0) {
            int idx = g_game->hoverUnitId & 0xffff;
            char* unit = g_game->units + idx * 0x118;
            sprintf(text, "MOVEORD: %d FIREORD: %d\n", (*(unsigned int*)(unit + 0x110) >> 0x12) & 3,
                    (*(unsigned int*)(unit + 0x110) >> 0x14) & 3);
            DrawString(surface, (unsigned char*)text, 0x108, pfstate, -1);
        }

        sprintf(text, "DELTATIME: %d\n", g_game->simStepsPending);
        DrawString(surface, (unsigned char*)text, 0x190, pfstate, -1);

        sprintf(text, "GAMETIME: %d\n", g_game->ticks);
        DrawString(surface, (unsigned char*)text, 0x208, pfstate, -1);

        pfstate -= 0x10;
        sprintf(text, "X: %d  Y: %d\n", g_game->scrollX, g_game->scrollY);
        DrawString(surface, (unsigned char*)text, 0x82, pfstate, -1);

        sprintf(text, "UNITS %d\\%d\n", g_game->field_14353, g_game->count);
        DrawString(surface, (unsigned char*)text, 0x108, pfstate, -1);

        sprintf(text, "PACKETS: %d %d %d\n", g_game->field_1cbe, g_game->field_1e09,
                g_game->field_1f54);
        DrawString(surface, (unsigned char*)text, 0x190, pfstate, -1);

        int v = g_game->mapWidthTiles;
        v *= (short)g_game->field_2c90;
        v += (short)g_game->field_2c8e;
        unsigned char c = *(unsigned char*)(g_game->cells + v * 13 + 4);
        sprintf(text, "XYH: %d %d %d\n", (short)g_game->field_2c8e,
                (short)g_game->field_2c90, c);
        DrawString(surface, (unsigned char*)text, 0x208, pfstate, -1);
        return;
    }

    snapshot.button = g_game->selected;
    snapshot.selected = g_game->hoverUnitId;
    snapshot.feature = g_game->cellFeature;
    snapshot.height = *(int*)((char*)g_game + 0x37e94);
    snapshot.width = *(int*)((char*)g_game + 0x37e90);

    if (snapshot.selected != 0) {
        char* unit = g_game->units + snapshot.selected * 0x118;
        snapshot.health = *(unsigned short*)(unit + 0x108);
        snapshot.build = *(unsigned short*)(unit + 0xb8);
        snapshot.orderName = GetOrderName(unit);
        // Copied through float temporaries; direct assignment gives integer moves.
        float f0 = *(float*)(unit + 0xd0);
        float f1 = *(float*)(unit + 0xcc);
        float f2 = *(float*)(unit + 0xe8);
        float f3 = *(float*)(unit + 0xe4);
        snapshot.values[0] = f0;
        snapshot.values[1] = f1;
        snapshot.values[2] = f2;
        snapshot.values[3] = f3;
        char* slot = unit + 0x1f;
        int* out = snapshot.weapons;
        // Keep both `*out = -1` stores (nested ifs): the extra use sets the register of out.
        for (int i = 0; i < 3; i++, out++, slot += 0x1c) {
            char* weapon = *(char**)(slot - 0xf);
            if (*(unsigned short*)(weapon + 0xe4) > 0x1e) {
                if (*(unsigned char*)slot & 2)
                    *out = *(unsigned short*)(slot - 7);
                else
                    *out = -1;
            } else
                *out = -1;
        }

        snapshot.targetType = 0;
        if (*(unsigned char*)(unit + 0xff) == g_game->playerIndex) {
            int result = GetOrderTarget(unit);
            if (result != 0) {
                snapshot.targetType = *(unsigned short*)(result + 0xa8);
                snapshot.targetHealth = *(unsigned short*)(result + 0x108);
            }
        }
    }

    char* saved = (char*)&g_game->selectionInfoCache;
    if (memcmp(saved, &snapshot, sizeof(snapshot)) == 0)
        return;
    memcpy(saved, &snapshot, sizeof(snapshot));

    char* palette = (char*)&g_game->colors[0];
    SetTextColors(0x53, GetTextKeyColor());

    int player = g_game->playerIndex;
    Player_0046a860* playerInfo = &g_game->players[0] + player;
    char* info = playerInfo->info;
    int side = *(unsigned char*)(info + 0x95);
    char* panel = (char*)g_game + side * 562 + 0x37f3d;
    SetFont(*(int*)(panel + 0x22e));
    int y = 0x81;
    do {
        int dy = GetScreenHeight() - 0x20;
        char* loopInfo = playerInfo->info;
        int loopSide = *(unsigned char*)(loopInfo + 0x95);
        unsigned short* icon = *(unsigned short**)((char*)g_game + loopSide * 4 + 0x14833);
        int bmp = GetGafFrame(icon, 0);
        DrawFrame(surface, (void*)bmp, (short)*(unsigned short*)(bmp + 4) + y,
                     (short)*(unsigned short*)(bmp + 6) + dy);
        y += *(unsigned short*)bmp;
    } while (y < g_game->width);

    if (snapshot.button != -1) {
        strncpy(text, (char*)(snapshot.button * 0x15b + *(int*)(g_game->field_531 + 4) + 2), 0x10);
        text[0x10] = 0;
        unsigned short type = FindUnitTypeId(text);
        if (type != 0) {
            char* entry = (char*)(0x249 * type + g_game->unitDefs);
            if (_strcmpi(text, "CORBUILD") != 0) {
                sprintf(text, "%s  M:%d E:%d", entry, (int)*(float*)(entry + 0x18a),
                        (int)*(float*)(entry + 0x186));
                DrawString(surface, (unsigned char*)text, *(int*)(panel + 0x1d2),
                             *(int*)(panel + 0x1d6) + yOffset, -1);
                DrawString(surface, (unsigned char*)(entry + 0x40), *(int*)(panel + 0x1e2),
                             *(int*)(panel + 0x1e6) + yOffset, -1);
                return;
            }
        }
    } else if (snapshot.selected != 0) {
        char* unit = g_game->units + snapshot.selected * 0x118;
        if (*(unsigned short*)(unit + 0xa6) != 0) {
            char* playerMap = (char*)g_game + g_game->playerIndex * 331 + 0x1b63;
            if (IsUnitVisibleToPlayer(playerMap, unit)) {
                char* definition = *(char**)(unit + 0x92);
                unsigned int unitFlags = *(unsigned int*)(definition + 0x245);
                int flagsOk = ((unitFlags & 0x20000) | ((unitFlags >> 1) & 0x20000)) >> 0x11;
                int gameMode = ((Mission*)*(void**)((char*)g_game + 0x391e9))->GetGameType();
                if (gameMode == 3 && flagsOk)
                    strcpy(text, *(char**)(unit + 0x96) + 0x2b);
                else
                    strcpy(text, *(char**)(unit + 0x92));
                int titleX = *(int*)(panel + 0x142) -
                             GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)text) / 2;
                SetTextColors(0x53, GetTextKeyColor());
                DrawString(surface, (unsigned char*)text, titleX, yOffset + *(int*)(panel + 0x146), -1);
                if (*(unsigned char*)(*(char**)(unit + 0x96) + 0x146) == g_game->playerIndex ||
                    !(*(unsigned int*)(*(char**)(unit + 0x92) + 0x241) & 0x4000)) {
                    int maximum = *(int*)(*(char**)(unit + 0x92) + 0x1fa);
                    DrawBar_0046a860(surface, (Rect_0046a860*)(panel + 0x152), *(short*)(unit + 0x108),
                                     maximum, yOffset, (unsigned char*)palette);
                }
                BlitSideLogoToRect(surface, *(void**)(unit + 0x96), panel + 0x132, yOffset);
                if (*(unsigned char*)(unit + 0xff) == g_game->playerIndex ||
                    (g_game->flagsByte_3923b & 2)) {
                    // Declared in this block, not at function scope: affects load scheduling.
                    char amount[100];
                    char killsText[100];
                    SetTextColors((unsigned char)palette[10], GetTextKeyColor());
                    sprintf(amount, "+%.1f", Positive_0046a860(snapshot.values[3]));
                    DrawString(surface, (unsigned char*)amount, *(int*)(panel + 0x182),
                                 *(int*)(panel + 0x186) + yOffset, -1);
                    sprintf(amount, "+%.0f", Positive_0046a860(snapshot.values[1]));
                    DrawString(surface, (unsigned char*)amount, *(int*)(panel + 0x162),
                                 *(int*)(panel + 0x166) + yOffset, -1);
                    SetTextColors((unsigned char)palette[12], GetTextKeyColor());
                    sprintf(amount, "-%.1f", Positive_0046a860(snapshot.values[2]));
                    DrawString(surface, (unsigned char*)amount, *(int*)(panel + 0x192),
                                 *(int*)(panel + 0x196) + yOffset, -1);
                    sprintf(amount, "-%.0f", Positive_0046a860(snapshot.values[0]));
                    DrawString(surface, (unsigned char*)amount, *(int*)(panel + 0x172),
                                 *(int*)(panel + 0x176) + yOffset, -1);
                    if ((*(unsigned int*)(unit + 0x110) & 0x80000000) && *(unsigned short*)(unit + 0xb8)) {
                        int killsY = *(int*)(panel + 0x15e) + yOffset + 2, killsX = *(int*)(panel + 0x152);
                        char* plural = Translate("kills");
                        char* singular = Translate("kill");
                        if (*(unsigned short*)(unit + 0xb8) > 4)
                            sprintf(killsText, "%d %s - %s", *(unsigned short*)(unit + 0xb8),
                                    *(unsigned short*)(unit + 0xb8) == 1 ? singular : plural,
                                    Translate("Veteran"));
                        else
                            sprintf(killsText, "%d %s", *(unsigned short*)(unit + 0xb8),
                                    *(unsigned short*)(unit + 0xb8) == 1 ? singular : plural);
                        SetTextColors(g_game->colors[15], GetTextKeyColor());
                        DrawString(surface, (unsigned char*)killsText, killsX, killsY, -1);
                    }
                    if (snapshot.orderName) {
                        strcpy(amount, Translate((char*)snapshot.orderName));
                        int orderX = *(int*)(panel + 0x1a2) -
                                     GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)amount) / 2;
                        SetTextColors(0x53, GetTextKeyColor());
                        DrawString(surface, (unsigned char*)amount, orderX, *(int*)(panel + 0x1a6) + yOffset, -1);
                    }
                }
                int progress = GetBuildWeaponPercent(unit);
                if (progress) {
                    if (*(unsigned char*)(*(char**)(unit + 0x96) + 0x146) != g_game->playerIndex)
                        return;
                    char* weaponText = Translate("Weapon");
                    int progressX = *(int*)(panel + 0x1b2) -
                                    GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)weaponText) / 2;
                    SetTextColors(0x53, GetTextKeyColor());
                    DrawString(surface, (unsigned char*)weaponText, progressX, *(int*)(panel + 0x1b6) + yOffset, -1);
                    DrawBar_0046a860(surface, (Rect_0046a860*)(panel + 0x1c2), progress, 100, yOffset,
                                     (unsigned char*)palette);
                    return;
                }
                if (snapshot.targetType) {
                    char* target = g_game->units + snapshot.targetType * 0x118;
                    if (!*(unsigned short*)(target + 0xa6) || !IsUnitVisibleToPlayer(playerMap, target))
                        return;
                    int targetX = *(int*)(panel + 0x1b2) -
                                  GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)*(char**)(target + 0x92)) / 2;
                    SetTextColors(0x53, GetTextKeyColor());
                    DrawString(surface, (unsigned char*)*(char**)(target + 0x92), targetX,
                                 *(int*)(panel + 0x1b6) + yOffset, -1);
                    if (*(unsigned char*)(*(char**)(target + 0x96) + 0x146) == g_game->playerIndex ||
                        !(*(unsigned int*)(*(char**)(target + 0x92) + 0x241) & 0x4000)) {
                        int maximum = *(int*)(*(char**)(target + 0x92) + 0x1fa);
                        DrawBar_0046a860(surface, (Rect_0046a860*)(panel + 0x1c2), *(short*)(target + 0x108),
                                         maximum, yOffset, (unsigned char*)palette);
                    }
                }
            } else {
                char* prefix = ((unsigned char)(*(unsigned int*)(unit + 0x110) >> 9) & 1) != 0 ? "S: " : "R: ";
                char* s = Translate("Unidentified object");
                sprintf(text, "%s%s", prefix, s);
                int w = GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)text);
                int x = *(int*)(panel + 0x142) - w / 2;
                SetTextColors(0x53, GetTextKeyColor());
                DrawString(surface, (unsigned char*)text, x, *(int*)(panel + 0x146) + yOffset, -1);
                return;
            }
        }
    } else if (snapshot.feature != 0xffff) {
        char* feature = (char*)(g_game->features + snapshot.feature * 0x100);
        if ((*(unsigned char*)(feature + 0xff) & 4) == 0 || (g_game->flagsByte_3923b & 2) != 0) {
            char mText[16];
            if (*(float*)(feature + 0xf0) != 0.0f)
                sprintf(mText, " M:%d", (int)*(float*)(feature + 0xf0));
            else
                mText[0] = 0;
            char eText[16];
            if (*(float*)(feature + 0xec) != 0.0f)
                sprintf(eText, " E:%d", (int)*(float*)(feature + 0xec));
            else
                eText[0] = 0;
            char* name = g_game->bits_3923b.b1 ? feature : feature + 0x80;
            if ((*(unsigned char*)(feature + 0xff) & 2) == 0)
                sprintf(text, "%s %s%s", Translate(name), mText, eText);
            else
                strcpy(text, Translate(name));
            SetTextColors(0x53, GetTextKeyColor());
            DrawString(surface, (unsigned char*)text, *(int*)(panel + 0x1d2), *(int*)(panel + 0x1d6) + yOffset, -1);
        }
    }
}
