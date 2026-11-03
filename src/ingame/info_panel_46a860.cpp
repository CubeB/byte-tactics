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
// The selection info cache at g_game+0x37e60 (Thaldren's SelectionInfoCache,
// 0x3c bytes): the copy of the hovered entry the panel is drawn from.
struct SelectionInfoCache {
    int orderName;
    unsigned short selected, health, build;
    int weapons[3];
    float values[4];
    unsigned short targetType, targetHealth, feature;
    int button;
    int totalUnitsPanelSlide;         // +0x30
    unsigned short* lightbarFrame;    // +0x34, cached lightbar frame
    int optionsLightbarDirty;         // +0x38
};
#pragma pack(pop)

#include "../network/player_info.h"
#include "../network/player.h"

#include "../graphics/rect.h"
#include "../map/cell.h"
#include "../map/feature.h"

struct Unit_0046a860;

#include "../units/unit_def.h"

#include "../weapons/weapon_def.h"

#pragma pack(push, 1)

// A unit, 0x118 bytes, the elements of the array at g_game+0x14357. The two
// signednesses of +0x108 both appear in this file, so the field is a union.
struct Unit_0046a860 {
    char unknown_0[4];
    char weaponSlots[3 * 0x1c];       // +0x4, Thaldren's UnitWeaponSlot rows: weapon +0xc, reloadTimer +0x14, flags +0x1b
    char unknown_58[0x92 - 0x58];
    UnitDef* def;                     // +0x92
    Player* player;                   // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short unitDefIndex;      // +0xa6
    unsigned short id;                // +0xa8
    char unknown_aa[0xb8 - 0xaa];
    unsigned short killCount;         // +0xb8
    char unknown_ba[0xcc - 0xba];
    float energyMakePrev;             // +0xcc
    float energyUsePrev;              // +0xd0
    char unknown_d4[0xe4 - 0xd4];
    float metalMakePrev;              // +0xe4
    float metalUsePrev;               // +0xe8
    char unknown_ec[0xff - 0xec];
    unsigned char playerIndex;        // +0xff
    char unknown_100[0x108 - 0x100];
    union {
        short health;                 // +0x108
        unsigned short healthRaw;     // +0x108, the unsigned reads
    };
    char unknown_10a[0x110 - 0x10a];
    unsigned int flags;               // +0x110
    char unknown_114[0x118 - 0x114];
};

#include "../graphics/gaf_frame.h"

#include "../gui/layer.h"

#include "../gui/gadget.h"
#include "../graphics/display_context.h"

#pragma pack(pop)

static void DrawBar_0046a860(void* surface, Rect* bounds, int value, int maximum, int dy,
                             unsigned char* palette) {
    Rect rect = *bounds;
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
#include "../game/side_def.h"

struct Game {
    char unknown_0[0xc];
    DisplayContext* displayContext;       // +0xc
    char unknown_10[0x531 - 0x10];
    Layer* layer;                         // +0x531
    char unknown_535[0x581 - 0x535];
    int selected;                         // +0x581
    char unknown_585[0xdcb - 0x585];
    unsigned char colors[16];             // +0xdcb
    char unknown_ddb[0x1b63 - 0xddb];
    Player players[10];                // +0x1b63, stride 0x14b
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;            // +0x2a43
    char unknown_2a44[0x2c8e - 0x2a44];
    unsigned short cursorMapCellX;        // +0x2c8e
    unsigned short cursorMapCellZ;        // +0x2c90
    char unknown_2c92[0x2cba - 0x2c92];
    unsigned short hoverUnitId;           // +0x2cba
    unsigned short cellFeature;           // +0x2cbc
    char unknown_2cbe[0x14233 - 0x2cbe];
    int mapWidthTiles;                    // +0x14233
    char unknown_14237[0x1426f - 0x14237];
    Feature* features;                // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell* heightMap;                      // +0x14287
    char unknown_1428b[0x1431f - 0x1428b];
    int scrollX;                          // +0x1431f
    int scrollY;                         // +0x14323
    char unknown_14327[0x14353 - 0x14327];
    int activeUnitTickCount;              // +0x14353
    Unit_0046a860* units;                 // +0x14357
    char unknown_1435b[0x14367 - 0x1435b];
    int count;                            // +0x14367
    char unknown_1436b[0x1439b - 0x1436b];
    UnitDef* unitDefs;                    // +0x1439b
    char unknown_1439f[0x147a7 - 0x1439f];
    int baseHeight;                       // +0x147a7
    char unknown_147ab[0x14833 - 0x147ab];
    unsigned short* sidePanelBotSeq[5];   // +0x14833
    char unknown_14847[0x37e1f - 0x14847];
    int screenWidth;                      // +0x37e1f
    int screenHeight;                     // +0x37e23
    char unknown_37e27[0x37e60 - 0x37e27];
    SelectionInfoCache selectionInfoCache;  // +0x37e60
    char unknown_37e9c[0x37f3d - 0x37e9c];
    SideDef sideDefs[5];                  // +0x37f3d, stride 0x232
    char unknown_38a37[0x38a3b - 0x38a37];
    int simStepsPending;                  // +0x38a3b
    char unknown_38a3f[0x38a47 - 0x38a3f];
    int gameTick;                         // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* mapInfo;                     // +0x391e9
    char unknown_391ed[0x391f9 - 0x391ed];
    int fontComix;                        // +0x391f9
    char unknown_391fd[0x3923b - 0x391fd];
    union {                               // +0x3923b
        unsigned short endGameFlags;
        unsigned char flagsByte_3923b;
        Flags46b_3923b bits_3923b;
    };
};
#pragma pack(pop)

// FUNCTION: 0x46a860
void __stdcall DrawUnitInfoPanel(void* surface) {
    int yOffset = g_game->screenHeight - g_game->baseHeight;
    SelectionInfoCache snapshot;
    memset(&snapshot, 0, sizeof(snapshot));
    char text[256];

    unsigned short flags = g_game->endGameFlags;
    if (flags & 1 && flags & 2) {
        int y = 0x81;
        unsigned char player = g_game->playerIndex;
        PlayerInfo* table = g_game->players[player].info;
        int idx = table->side;
        do {
            int dy = GetScreenHeight() - 0x20;
            unsigned short* icon = g_game->sidePanelBotSeq[idx];
            GafFrame* bmp = (GafFrame*)GetGafFrame(icon, 0);
            DrawFrame(surface, (void*)bmp, (short)bmp->xOffset + y,
                         (short)bmp->yOffset + dy);
            y += bmp->width;
        } while (y < g_game->screenWidth);

        SetFont(g_game->fontComix);
        SetTextColors(0x53, GetTextKeyColor());
        // No pfable or field_c locals: the PFSTATE sprintf re-reads g_game->displayContext per argument.
        int pfstate = g_game->screenHeight - GetFontHeight() - 1;
        sprintf(text, "PFSTATE %d, PFABLE %d\n", g_game->displayContext->flags.word & 1,
                g_game->displayContext->draw.haveBackBuffer);
        DrawString(surface, (unsigned char*)text, 0x82, pfstate, -1);

        if (g_game->hoverUnitId != 0) {
            int idx = g_game->hoverUnitId & 0xffff;
            Unit_0046a860* unit = g_game->units + idx;
            sprintf(text, "MOVEORD: %d FIREORD: %d\n", (unit->flags >> 0x12) & 3,
                    (unit->flags >> 0x14) & 3);
            DrawString(surface, (unsigned char*)text, 0x108, pfstate, -1);
        }

        sprintf(text, "DELTATIME: %d\n", g_game->simStepsPending);
        DrawString(surface, (unsigned char*)text, 0x190, pfstate, -1);

        sprintf(text, "GAMETIME: %d\n", g_game->gameTick);
        DrawString(surface, (unsigned char*)text, 0x208, pfstate, -1);

        pfstate -= 0x10;
        sprintf(text, "X: %d  Y: %d\n", g_game->scrollX, g_game->scrollY);
        DrawString(surface, (unsigned char*)text, 0x82, pfstate, -1);

        sprintf(text, "UNITS %d\\%d\n", g_game->activeUnitTickCount, g_game->count);
        DrawString(surface, (unsigned char*)text, 0x108, pfstate, -1);

        sprintf(text, "PACKETS: %d %d %d\n", g_game->players[1].messages,
                g_game->players[2].messages, g_game->players[3].messages);
        DrawString(surface, (unsigned char*)text, 0x190, pfstate, -1);

        int v = g_game->mapWidthTiles;
        v *= (short)g_game->cursorMapCellZ;
        v += (short)g_game->cursorMapCellX;
        unsigned char c = g_game->heightMap[v].height;
        sprintf(text, "XYH: %d %d %d\n", (short)g_game->cursorMapCellX,
                (short)g_game->cursorMapCellZ, c);
        DrawString(surface, (unsigned char*)text, 0x208, pfstate, -1);
        return;
    }

    snapshot.button = g_game->selected;
    snapshot.selected = g_game->hoverUnitId;
    snapshot.feature = g_game->cellFeature;
    snapshot.lightbarFrame = g_game->selectionInfoCache.lightbarFrame;
    snapshot.totalUnitsPanelSlide = g_game->selectionInfoCache.totalUnitsPanelSlide;

    if (snapshot.selected != 0) {
        Unit_0046a860* unit = g_game->units + snapshot.selected;
        snapshot.health = unit->healthRaw;
        snapshot.build = unit->killCount;
        snapshot.orderName = GetOrderName(unit);
        // Copied through float temporaries; direct assignment gives integer moves.
        float f0 = unit->energyUsePrev;
        float f1 = unit->energyMakePrev;
        float f2 = unit->metalUsePrev;
        float f3 = unit->metalMakePrev;
        snapshot.values[0] = f0;
        snapshot.values[1] = f1;
        snapshot.values[2] = f2;
        snapshot.values[3] = f3;
        char* slot = &unit->weaponSlots[0x1b];  // each row's flags byte
        int* out = snapshot.weapons;
        // Keep both `*out = -1` stores (nested ifs): the extra use sets the register of out.
        for (int i = 0; i < 3; i++, out++, slot += 0x1c) {
            WeaponDef* weapon = *(WeaponDef**)(slot - 0xf);
            if (weapon->reloadTime > 0x1e) {
                if (*(unsigned char*)slot & 2)
                    *out = *(unsigned short*)(slot - 7);
                else
                    *out = -1;
            } else
                *out = -1;
        }

        snapshot.targetType = 0;
        if (unit->playerIndex == g_game->playerIndex) {
            Unit_0046a860* result = (Unit_0046a860*)GetOrderTarget(unit);
            if (result != 0) {
                snapshot.targetType = result->id;
                snapshot.targetHealth = result->healthRaw;
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
    Player* playerInfo = &g_game->players[player];
    PlayerInfo* info = playerInfo->info;
    int side = info->side;
    SideDef* panel = &g_game->sideDefs[side];
    SetFont((int)panel->font);
    int y = 0x81;
    do {
        int dy = GetScreenHeight() - 0x20;
        PlayerInfo* loopInfo = playerInfo->info;
        int loopSide = loopInfo->side;
        unsigned short* icon = g_game->sidePanelBotSeq[loopSide];
        GafFrame* bmp = (GafFrame*)GetGafFrame(icon, 0);
        DrawFrame(surface, (void*)bmp, (short)bmp->xOffset + y,
                     (short)bmp->yOffset + dy);
        y += bmp->width;
    } while (y < g_game->screenWidth);

    if (snapshot.button != -1) {
        strncpy(text, g_game->layer->entries[snapshot.button].name, 0x10);
        text[0x10] = 0;
        unsigned short type = FindUnitTypeId(text);
        if (type != 0) {
            UnitDef* entry = g_game->unitDefs + type;
            if (_strcmpi(text, "CORBUILD") != 0) {
                sprintf(text, "%s  M:%d E:%d", entry, (int)entry->metalCost,
                        (int)entry->energyCost);
                DrawString(surface, (unsigned char*)text, panel->nameBlock.left,
                             panel->nameBlock.top + yOffset, -1);
                DrawString(surface, (unsigned char*)entry->description, panel->description.left,
                             panel->description.top + yOffset, -1);
                return;
            }
        }
    } else if (snapshot.selected != 0) {
        Unit_0046a860* unit = g_game->units + snapshot.selected;
        if (unit->unitDefIndex != 0) {
            char* playerMap = (char*)&g_game->players[g_game->playerIndex];
            if (IsUnitVisibleToPlayer(playerMap, unit)) {
                UnitDef* definition = unit->def;
                unsigned int unitFlags = definition->flags2;
                int flagsOk = ((unitFlags & 0x20000) | ((unitFlags >> 1) & 0x20000)) >> 0x11;
                int gameMode = g_game->mapInfo->GetGameType();
                if (gameMode == 3 && flagsOk)
                    strcpy(text, unit->player->name);
                else
                    strcpy(text, (char*)unit->def);
                int titleX = panel->unitName.left -
                             GetTextWidth((void*)panel->font, (unsigned char*)text) / 2;
                SetTextColors(0x53, GetTextKeyColor());
                DrawString(surface, (unsigned char*)text, titleX, yOffset + panel->unitName.top, -1);
                if (unit->player->index == g_game->playerIndex ||
                    !(unit->def->flags1 & 0x4000)) {
                    int maximum = unit->def->maxHealth;
                    DrawBar_0046a860(surface, &panel->damageBar, unit->health,
                                     maximum, yOffset, (unsigned char*)palette);
                }
                BlitSideLogoToRect(surface, unit->player, &panel->logo2, yOffset);
                if (unit->playerIndex == g_game->playerIndex ||
                    (g_game->flagsByte_3923b & 2)) {
                    // Declared in this block, not at function scope: affects load scheduling.
                    char amount[100];
                    char killsText[100];
                    SetTextColors((unsigned char)palette[10], GetTextKeyColor());
                    sprintf(amount, "+%.1f", Positive_0046a860(snapshot.values[3]));
                    DrawString(surface, (unsigned char*)amount, panel->unitMetalMake.left,
                                 panel->unitMetalMake.top + yOffset, -1);
                    sprintf(amount, "+%.0f", Positive_0046a860(snapshot.values[1]));
                    DrawString(surface, (unsigned char*)amount, panel->unitEnergyMake.left,
                                 panel->unitEnergyMake.top + yOffset, -1);
                    SetTextColors((unsigned char)palette[12], GetTextKeyColor());
                    sprintf(amount, "-%.1f", Positive_0046a860(snapshot.values[2]));
                    DrawString(surface, (unsigned char*)amount, panel->unitMetalUse.left,
                                 panel->unitMetalUse.top + yOffset, -1);
                    sprintf(amount, "-%.0f", Positive_0046a860(snapshot.values[0]));
                    DrawString(surface, (unsigned char*)amount, panel->unitEnergyUse.left,
                                 panel->unitEnergyUse.top + yOffset, -1);
                    if ((unit->flags & 0x80000000) && unit->killCount) {
                        int killsY = panel->damageBar.bottom + yOffset + 2, killsX = panel->damageBar.left;
                        char* plural = Translate("kills");
                        char* singular = Translate("kill");
                        if (unit->killCount > 4)
                            sprintf(killsText, "%d %s - %s", unit->killCount,
                                    unit->killCount == 1 ? singular : plural,
                                    Translate("Veteran"));
                        else
                            sprintf(killsText, "%d %s", unit->killCount,
                                    unit->killCount == 1 ? singular : plural);
                        SetTextColors(g_game->colors[15], GetTextKeyColor());
                        DrawString(surface, (unsigned char*)killsText, killsX, killsY, -1);
                    }
                    if (snapshot.orderName) {
                        strcpy(amount, Translate((char*)snapshot.orderName));
                        int orderX = panel->missionText.left -
                                     GetTextWidth((void*)panel->font, (unsigned char*)amount) / 2;
                        SetTextColors(0x53, GetTextKeyColor());
                        DrawString(surface, (unsigned char*)amount, orderX, panel->missionText.top + yOffset, -1);
                    }
                }
                int progress = GetBuildWeaponPercent(unit);
                if (progress) {
                    if (unit->player->index != g_game->playerIndex)
                        return;
                    char* weaponText = Translate("Weapon");
                    int progressX = panel->unitName2.left -
                                    GetTextWidth((void*)panel->font, (unsigned char*)weaponText) / 2;
                    SetTextColors(0x53, GetTextKeyColor());
                    DrawString(surface, (unsigned char*)weaponText, progressX, panel->unitName2.top + yOffset, -1);
                    DrawBar_0046a860(surface, &panel->damageBar2, progress, 100, yOffset,
                                     (unsigned char*)palette);
                    return;
                }
                if (snapshot.targetType) {
                    Unit_0046a860* target = g_game->units + snapshot.targetType;
                    if (!target->unitDefIndex || !IsUnitVisibleToPlayer(playerMap, target))
                        return;
                    int targetX = panel->unitName2.left -
                                  GetTextWidth((void*)panel->font, (unsigned char*)target->def) / 2;
                    SetTextColors(0x53, GetTextKeyColor());
                    DrawString(surface, (unsigned char*)target->def, targetX,
                                 panel->unitName2.top + yOffset, -1);
                    if (target->player->index == g_game->playerIndex ||
                        !(target->def->flags1 & 0x4000)) {
                        int maximum = target->def->maxHealth;
                        DrawBar_0046a860(surface, &panel->damageBar2, target->health,
                                         maximum, yOffset, (unsigned char*)palette);
                    }
                }
            } else {
                char* prefix = ((unsigned char)(unit->flags >> 9) & 1) != 0 ? "S: " : "R: ";
                char* s = Translate("Unidentified object");
                sprintf(text, "%s%s", prefix, s);
                int w = GetTextWidth((void*)panel->font, (unsigned char*)text);
                int x = panel->unitName.left - w / 2;
                SetTextColors(0x53, GetTextKeyColor());
                DrawString(surface, (unsigned char*)text, x, panel->unitName.top + yOffset, -1);
                return;
            }
        }
    } else if (snapshot.feature != 0xffff) {
        Feature* feature = g_game->features + snapshot.feature;
        if ((feature->flagsHigh & 4) == 0 || (g_game->flagsByte_3923b & 2) != 0) {
            char mText[16];
            if (feature->metal != 0.0f)
                sprintf(mText, " M:%d", (int)feature->metal);
            else
                mText[0] = 0;
            char eText[16];
            if (feature->energy != 0.0f)
                sprintf(eText, " E:%d", (int)feature->energy);
            else
                eText[0] = 0;
            char* name = g_game->bits_3923b.b1 ? feature->name : feature->description;
            if ((feature->flagsHigh & 2) == 0)
                sprintf(text, "%s %s%s", Translate(name), mText, eText);
            else
                strcpy(text, Translate(name));
            SetTextColors(0x53, GetTextKeyColor());
            DrawString(surface, (unsigned char*)text, panel->nameBlock.left, panel->nameBlock.top + yOffset, -1);
        }
    }
}
