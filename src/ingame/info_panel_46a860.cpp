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
    int button;
    int nTotalUnitsPanelSlide;        // +0x30
    unsigned short* pLightbarFrame;   // +0x34, cached lightbar frame
    int optionsLightbarDirty;         // +0x38
};
#pragma pack(pop)

#pragma pack(push, 1)
// A player's entry at g_game+0x1b63, stride 0x14b (Thaldren: PlayerState).
struct SideData_0046a860 {
    char unknown_0[0x95];
    unsigned char bSideId;            // +0x95
    char unknown_96[0xb9 - 0x96];
};

struct Player_0046a860 {
    char unknown_0[0x10];
    int nIncomingPacketCount;         // +0x10
    char unknown_14[0x27 - 0x14];
    SideData_0046a860* info;          // +0x27, pSideData
    char name[0x1e];                  // +0x2b
    char unknown_49[0x146 - 0x49];
    unsigned char index;              // +0x146
    char unknown_147[331 - 0x147];
};
#pragma pack(pop)

struct Rect_0046a860 {
    int left, top, right, bottom;
};

struct WeaponDef_0046a860;
struct UnitDef_0046a860;
struct Unit_0046a860;

#pragma pack(push, 1)
// A weapon type (Thaldren: WeaponDef); only the reload time is read here.
struct WeaponDef_0046a860 {
    char unknown_0[0xe4];
    unsigned short reloadTime;        // +0xe4
};

// A unit's type (Thaldren: UnitDef, 0x249 bytes), the elements of the table
// at g_game+0x1439b.
struct UnitDef_0046a860 {
    char name[0x40];                  // +0x000
    char description[0x40];           // +0x040
    char unknown_80[0x186 - 0x80];
    float energyCost;                 // +0x186
    float metalCost;                  // +0x18a
    char unknown_18e[0x1fa - 0x18e];
    unsigned int maxHealth;           // +0x1fa
    char unknown_1fe[0x241 - 0x1fe];
    unsigned int f241;                // +0x241
    unsigned int f245;                // +0x245
};

// A unit, 0x118 bytes, the elements of the array at g_game+0x14357. The two
// signednesses of +0x108 both appear in this file, so the field is a union.
struct Unit_0046a860 {
    char unknown_0[0x92];
    UnitDef_0046a860* def;            // +0x92
    Player_0046a860* player;          // +0x96
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

// A feature's 0x100-byte record, the elements of the table at g_game+0x1426f
// (Thaldren: FeatureDef).
struct Feature_0046a860 {
    char name[0x80];                  // +0x000
    char description[0x14];           // +0x080
    char unknown_94[0xec - 0x94];
    float energy;                     // +0x0ec
    float metal;                      // +0x0f0
    char unknown_f4[0xff - 0xf4];
    unsigned char flags;              // +0x0ff
};

// One map cell, 13 bytes, the elements of the table at g_game+0x14287.
struct Cell_0046a860 {
    char unknown_0[4];
    unsigned char shade;              // +0x04
    char unknown_5[0x0d - 0x5];
};

#include "../graphics/gaf_frame.h"

// The display context at g_game+0xc; only two fields are read here.
struct Display_0046a860 {
    char unknown_0[0x9c];
    int field_9c;                     // +0x09c
    char unknown_a0[0xf0 - 0xa0];
    unsigned char flags;              // +0x0f0, low byte of the state flags word
};

// The object g_game+0x531 points at (0x495860's shape).
struct Struct_0046a860 {
    int unknown_0;
    int value;                        // +0x4
};
#pragma pack(pop)

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
// A side's panel layout at g_game+0x37f3d, stride 0x232 (Thaldren: SideDef).
struct SideDef_0046a860 {
    char aName[0x1e];                   // +0x000
    char aNamePrefix[4];                // +0x01e
    char aCommander[0x20];              // +0x022
    Rect_0046a860 rcLogo;               // +0x042
    Rect_0046a860 rcEnergyBar;          // +0x052
    Rect_0046a860 rcEnergyNum;          // +0x062
    Rect_0046a860 rcMetalBar;           // +0x072
    Rect_0046a860 rcMetalNum;           // +0x082
    Rect_0046a860 rcTotalUnits;         // +0x092
    Rect_0046a860 rcTotalTime;          // +0x0a2
    Rect_0046a860 rcEnergyMax;          // +0x0b2
    Rect_0046a860 rcMetalMax;           // +0x0c2
    Rect_0046a860 rcEnergy0;            // +0x0d2
    Rect_0046a860 rcMetal0;             // +0x0e2
    Rect_0046a860 rcEnergyProduced;     // +0x0f2
    Rect_0046a860 rcEnergyConsumed;     // +0x102
    Rect_0046a860 rcMetalProduced;      // +0x112
    Rect_0046a860 rcMetalConsumed;      // +0x122
    Rect_0046a860 rcLogo2;              // +0x132
    Rect_0046a860 rcUnitName;           // +0x142
    Rect_0046a860 rcDamageBar;          // +0x152
    Rect_0046a860 rcUnitEnergyMake;     // +0x162
    Rect_0046a860 rcUnitEnergyUse;      // +0x172
    Rect_0046a860 rcUnitMetalMake;      // +0x182
    Rect_0046a860 rcUnitMetalUse;       // +0x192
    Rect_0046a860 rcMissionText;        // +0x1a2
    Rect_0046a860 rcUnitName2;          // +0x1b2
    Rect_0046a860 rcDamageBar2;         // +0x1c2
    Rect_0046a860 rcName;               // +0x1d2
    Rect_0046a860 rcDescription;        // +0x1e2
    Rect_0046a860 aReloadRects[3];      // +0x1f2
    int nEnergyColor;                   // +0x222
    int nMetalColor;                    // +0x226
    int nSideIndex;                     // +0x22a
    char* pFontData;                    // +0x22e
};

struct Game {
    char unknown_0[0xc];
    Display_0046a860* displayContext;    // +0xc
    char unknown_10[0x531 - 0x10];
    Struct_0046a860* layer;               // +0x531
    char unknown_535[0x581 - 0x535];
    int selected;                         // +0x581
    char unknown_585[0xdcb - 0x585];
    unsigned char colors[16];             // +0xdcb
    char unknown_ddb[0x1b63 - 0xddb];
    Player_0046a860 players[10];          // +0x1b63, stride 0x14b
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
    Feature_0046a860* features;           // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_0046a860* heightMap;             // +0x14287
    char unknown_1428b[0x1431f - 0x1428b];
    int scrollX;                          // +0x1431f
    int scrollY;                         // +0x14323
    char unknown_14327[0x14353 - 0x14327];
    int field_14353;                      // +0x14353
    Unit_0046a860* units;                 // +0x14357
    char unknown_1435b[0x14367 - 0x1435b];
    int count;                            // +0x14367
    char unknown_1436b[0x1439b - 0x1436b];
    UnitDef_0046a860* unitDefs;           // +0x1439b
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
    SideDef_0046a860 sideDefs[5];         // +0x37f3d, stride 0x232
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
        SideData_0046a860* table = g_game->players[player].info;
        int idx = table->bSideId;
        do {
            int dy = GetScreenHeight() - 0x20;
            unsigned short* icon = g_game->sidePanelBotSeq[idx];
            GafFrame* bmp = (GafFrame*)GetGafFrame(icon, 0);
            DrawFrame(surface, (void*)bmp, (short)bmp->xOffset + y,
                         (short)bmp->yOffset + dy);
            y += bmp->width;
        } while (y < g_game->width);

        SetFont(g_game->fontComix);
        SetTextColors(0x53, GetTextKeyColor());
        // No pfable or field_c locals: the PFSTATE sprintf re-reads g_game->displayContext per argument.
        int pfstate = g_game->height - GetFontHeight() - 1;
        sprintf(text, "PFSTATE %d, PFABLE %d\n", g_game->displayContext->flags & 1,
                g_game->displayContext->field_9c);
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

        sprintf(text, "GAMETIME: %d\n", g_game->ticks);
        DrawString(surface, (unsigned char*)text, 0x208, pfstate, -1);

        pfstate -= 0x10;
        sprintf(text, "X: %d  Y: %d\n", g_game->scrollX, g_game->scrollY);
        DrawString(surface, (unsigned char*)text, 0x82, pfstate, -1);

        sprintf(text, "UNITS %d\\%d\n", g_game->field_14353, g_game->count);
        DrawString(surface, (unsigned char*)text, 0x108, pfstate, -1);

        sprintf(text, "PACKETS: %d %d %d\n", g_game->players[1].nIncomingPacketCount,
                g_game->players[2].nIncomingPacketCount, g_game->players[3].nIncomingPacketCount);
        DrawString(surface, (unsigned char*)text, 0x190, pfstate, -1);

        int v = g_game->mapWidthTiles;
        v *= (short)g_game->field_2c90;
        v += (short)g_game->field_2c8e;
        unsigned char c = g_game->heightMap[v].shade;
        sprintf(text, "XYH: %d %d %d\n", (short)g_game->field_2c8e,
                (short)g_game->field_2c90, c);
        DrawString(surface, (unsigned char*)text, 0x208, pfstate, -1);
        return;
    }

    snapshot.button = g_game->selected;
    snapshot.selected = g_game->hoverUnitId;
    snapshot.feature = g_game->cellFeature;
    snapshot.pLightbarFrame = g_game->selectionInfoCache.pLightbarFrame;
    snapshot.nTotalUnitsPanelSlide = g_game->selectionInfoCache.nTotalUnitsPanelSlide;

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
        char* slot = (char*)unit + 0x1f;
        int* out = snapshot.weapons;
        // Keep both `*out = -1` stores (nested ifs): the extra use sets the register of out.
        for (int i = 0; i < 3; i++, out++, slot += 0x1c) {
            WeaponDef_0046a860* weapon = *(WeaponDef_0046a860**)(slot - 0xf);
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
    Player_0046a860* playerInfo = &g_game->players[player];
    SideData_0046a860* info = playerInfo->info;
    int side = info->bSideId;
    SideDef_0046a860* panel = &g_game->sideDefs[side];
    SetFont((int)panel->pFontData);
    int y = 0x81;
    do {
        int dy = GetScreenHeight() - 0x20;
        SideData_0046a860* loopInfo = playerInfo->info;
        int loopSide = loopInfo->bSideId;
        unsigned short* icon = g_game->sidePanelBotSeq[loopSide];
        GafFrame* bmp = (GafFrame*)GetGafFrame(icon, 0);
        DrawFrame(surface, (void*)bmp, (short)bmp->xOffset + y,
                     (short)bmp->yOffset + dy);
        y += bmp->width;
    } while (y < g_game->width);

    if (snapshot.button != -1) {
        strncpy(text, (char*)(snapshot.button * 0x15b + g_game->layer->value + 2), 0x10);
        text[0x10] = 0;
        unsigned short type = FindUnitTypeId(text);
        if (type != 0) {
            UnitDef_0046a860* entry = g_game->unitDefs + type;
            if (_strcmpi(text, "CORBUILD") != 0) {
                sprintf(text, "%s  M:%d E:%d", entry, (int)entry->metalCost,
                        (int)entry->energyCost);
                DrawString(surface, (unsigned char*)text, panel->rcName.left,
                             panel->rcName.top + yOffset, -1);
                DrawString(surface, (unsigned char*)entry->description, panel->rcDescription.left,
                             panel->rcDescription.top + yOffset, -1);
                return;
            }
        }
    } else if (snapshot.selected != 0) {
        Unit_0046a860* unit = g_game->units + snapshot.selected;
        if (unit->unitDefIndex != 0) {
            char* playerMap = (char*)&g_game->players[g_game->playerIndex];
            if (IsUnitVisibleToPlayer(playerMap, unit)) {
                UnitDef_0046a860* definition = unit->def;
                unsigned int unitFlags = definition->f245;
                int flagsOk = ((unitFlags & 0x20000) | ((unitFlags >> 1) & 0x20000)) >> 0x11;
                int gameMode = g_game->mapInfo->GetGameType();
                if (gameMode == 3 && flagsOk)
                    strcpy(text, unit->player->name);
                else
                    strcpy(text, (char*)unit->def);
                int titleX = panel->rcUnitName.left -
                             GetTextWidth((void*)panel->pFontData, (unsigned char*)text) / 2;
                SetTextColors(0x53, GetTextKeyColor());
                DrawString(surface, (unsigned char*)text, titleX, yOffset + panel->rcUnitName.top, -1);
                if (unit->player->index == g_game->playerIndex ||
                    !(unit->def->f241 & 0x4000)) {
                    int maximum = unit->def->maxHealth;
                    DrawBar_0046a860(surface, &panel->rcDamageBar, unit->health,
                                     maximum, yOffset, (unsigned char*)palette);
                }
                BlitSideLogoToRect(surface, unit->player, &panel->rcLogo2, yOffset);
                if (unit->playerIndex == g_game->playerIndex ||
                    (g_game->flagsByte_3923b & 2)) {
                    // Declared in this block, not at function scope: affects load scheduling.
                    char amount[100];
                    char killsText[100];
                    SetTextColors((unsigned char)palette[10], GetTextKeyColor());
                    sprintf(amount, "+%.1f", Positive_0046a860(snapshot.values[3]));
                    DrawString(surface, (unsigned char*)amount, panel->rcUnitMetalMake.left,
                                 panel->rcUnitMetalMake.top + yOffset, -1);
                    sprintf(amount, "+%.0f", Positive_0046a860(snapshot.values[1]));
                    DrawString(surface, (unsigned char*)amount, panel->rcUnitEnergyMake.left,
                                 panel->rcUnitEnergyMake.top + yOffset, -1);
                    SetTextColors((unsigned char)palette[12], GetTextKeyColor());
                    sprintf(amount, "-%.1f", Positive_0046a860(snapshot.values[2]));
                    DrawString(surface, (unsigned char*)amount, panel->rcUnitMetalUse.left,
                                 panel->rcUnitMetalUse.top + yOffset, -1);
                    sprintf(amount, "-%.0f", Positive_0046a860(snapshot.values[0]));
                    DrawString(surface, (unsigned char*)amount, panel->rcUnitEnergyUse.left,
                                 panel->rcUnitEnergyUse.top + yOffset, -1);
                    if ((unit->flags & 0x80000000) && unit->killCount) {
                        int killsY = panel->rcDamageBar.bottom + yOffset + 2, killsX = panel->rcDamageBar.left;
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
                        int orderX = panel->rcMissionText.left -
                                     GetTextWidth((void*)panel->pFontData, (unsigned char*)amount) / 2;
                        SetTextColors(0x53, GetTextKeyColor());
                        DrawString(surface, (unsigned char*)amount, orderX, panel->rcMissionText.top + yOffset, -1);
                    }
                }
                int progress = GetBuildWeaponPercent(unit);
                if (progress) {
                    if (unit->player->index != g_game->playerIndex)
                        return;
                    char* weaponText = Translate("Weapon");
                    int progressX = panel->rcUnitName2.left -
                                    GetTextWidth((void*)panel->pFontData, (unsigned char*)weaponText) / 2;
                    SetTextColors(0x53, GetTextKeyColor());
                    DrawString(surface, (unsigned char*)weaponText, progressX, panel->rcUnitName2.top + yOffset, -1);
                    DrawBar_0046a860(surface, &panel->rcDamageBar2, progress, 100, yOffset,
                                     (unsigned char*)palette);
                    return;
                }
                if (snapshot.targetType) {
                    Unit_0046a860* target = g_game->units + snapshot.targetType;
                    if (!target->unitDefIndex || !IsUnitVisibleToPlayer(playerMap, target))
                        return;
                    int targetX = panel->rcUnitName2.left -
                                  GetTextWidth((void*)panel->pFontData, (unsigned char*)target->def) / 2;
                    SetTextColors(0x53, GetTextKeyColor());
                    DrawString(surface, (unsigned char*)target->def, targetX,
                                 panel->rcUnitName2.top + yOffset, -1);
                    if (target->player->index == g_game->playerIndex ||
                        !(target->def->f241 & 0x4000)) {
                        int maximum = target->def->maxHealth;
                        DrawBar_0046a860(surface, &panel->rcDamageBar2, target->health,
                                         maximum, yOffset, (unsigned char*)palette);
                    }
                }
            } else {
                char* prefix = ((unsigned char)(unit->flags >> 9) & 1) != 0 ? "S: " : "R: ";
                char* s = Translate("Unidentified object");
                sprintf(text, "%s%s", prefix, s);
                int w = GetTextWidth((void*)panel->pFontData, (unsigned char*)text);
                int x = panel->rcUnitName.left - w / 2;
                SetTextColors(0x53, GetTextKeyColor());
                DrawString(surface, (unsigned char*)text, x, panel->rcUnitName.top + yOffset, -1);
                return;
            }
        }
    } else if (snapshot.feature != 0xffff) {
        Feature_0046a860* feature = g_game->features + snapshot.feature;
        if ((feature->flags & 4) == 0 || (g_game->flagsByte_3923b & 2) != 0) {
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
            if ((feature->flags & 2) == 0)
                sprintf(text, "%s %s%s", Translate(name), mText, eText);
            else
                strcpy(text, Translate(name));
            SetTextColors(0x53, GetTextKeyColor());
            DrawString(surface, (unsigned char*)text, panel->rcName.left, panel->rcName.top + yOffset, -1);
        }
    }
}
