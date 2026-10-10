// Decompiled by Opus, DeepSeek V4.1 Flash, Claude Opus 5.5, mimo-v2.6-flash, space-bunny-free, deepseek-v4.1-flash, deepseek-v4.1, Sonnet 5.5, Space Bunny Free, GPT-6, Haiku and Sonnet. Names are provisional.
// The in-game dialogs: the metal and energy readouts, giving selected units
// away, the resource sharing screen (SHARE.GUI), the chat target screen
// (TALK.GUI), the unit info screen, the tab menu (TABMENU.GUI), the frame
// pacing, the game step loop and the mission-script event dispatcher.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>
#include <vector>

#include "../map/mission.h"

#include "../sound/sound.h"

unsigned int __cdecl GetMilliseconds(void);

#include "../network/packet_manager.h"

class FrameTimers {                    // frame-time profile, embedded at g_game+0x38d85
public:
    int last;                           // +0x00
    int total;                          // +0x04
    int values[9];                      // +0x08
    int acc[9];                         // +0x2c

    void AccumulateProfileTime(int i) {
        unsigned int t = GetMilliseconds();
        int d = t - last;
        int v = acc[i];
        v = v + d;
        acc[i] = v;
        last = t;
    }
};

struct Game;

#pragma pack(push, 1)

#include "../graphics/rect.h"

#include "../util/point.h"

struct Quad {
    Point p[4];
};

struct Gui;

#include "../gui/gadget.h"

#include "../gui/layer.h"

#include "../gui/gui.h"

#include "../network/player.h"
#include "../network/player_info.h"

struct Unit {
    char unknown_0[0x86];
    int carrier;                      // +0x86
    int cargo;                      // +0x8a
    char unknown_8e[0xa6 - 0x8e];
    unsigned short unitDefIndex;       // +0xa6
    char unknown_a8[0xf0 - 0xa8];
    int attacker;                      // +0xf0
    unsigned char lastAttackerSlot;            // +0xf4
    char unknown_f5[0x110 - 0xf5];
    union {
        unsigned int flags;            // +0x110
        struct {
            unsigned int bits_110 : 14;
            unsigned int flag_110 : 1;
        };
    };
    char unknown_114[0x118 - 0x114];
};

struct Saved {                         // 10 bytes
    int a;                             // +0x00
    int b;                             // +0x04
    short c;                           // +0x08
};

struct Flags16 {
    unsigned short bits0_7 : 8;
    unsigned short bit8 : 1;
    unsigned short bits9_15 : 7;
};

struct Game {
    char unknown_0[0x10];
    Sound* sound;                      // +0x10
    char unknown_14[0x511 - 0x14];
    Player* slowest;                   // +0x511
    int lag;                           // +0x515
    Gui gui;                           // +0x519
    char unknown_120f[0x1b63 - 0x120f];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char localPlayer;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    unsigned char flags_2a44;          // +0x2a44
    char unknown_2a45[0x2bee - 0x2a45];
    union {
        unsigned short lobbyUiDirtyFlags;  // +0x2bee
        Flags16 bits_2bee;
    };
    unsigned char chatMode;            // +0x2bf0
    unsigned char chatRecipients[11];  // +0x2bf1
    char unknown_2bfc[0x2c74 - 0x2bfc];
    unsigned char lockFlags;           // +0x2c74
    char unknown_2c75[0x2cba - 0x2c75];
    unsigned short hoverUnitId;        // +0x2cba
    char unknown_2cbc[0x14280 - 0x2cbc];
    unsigned char debugMode; // +0x14280
    char unknown_14281[0x14357 - 0x14281];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    char* unitDefs;                    // +0x1439b
    char unknown_1439f[0x148db - 0x1439f];
    int logos32;                   // +0x148db
    char unknown_148df[0x37ea0 - 0x148df];
    char mainHudLayoutName[0x1e];      // +0x37ea0
    union {
        unsigned short ordersPanelFlags;    // +0x37ebe
        unsigned char byte_37ebe;
        struct {
            unsigned short pad_37ebe : 6;
            unsigned short bit6_37ebe : 1;
            unsigned short rest_37ebe : 9;
        };
        struct {
            unsigned short low_37ebe : 11;
            unsigned short bit11_37ebe : 1;
            unsigned short high_37ebe : 4;
        };
    };
    char unknown_37ec0[0x37ef6 - 0x37ec0];
    int battleCommanderDeath;          // +0x37ef6
    char unknown_37efa[0x37f06 - 0x37efa];
    unsigned char visualFlags;         // +0x37f06
    char unknown_37f07[0x37f2f - 0x37f07];
    unsigned short uiOptionFlags;      // +0x37f2f, bit 1 is the "verbose" bit
    char unknown_37f31[0x38a37 - 0x37f31];
    unsigned int lastSimBudgetTick;    // +0x38a37
    int simStepsPending;               // +0x38a3b
    int simBudgetDtTicks;              // +0x38a3f
    float carry;                       // +0x38a43
    int gameTick;                      // +0x38a47
    unsigned short speedCtrl;          // +0x38a4b
    unsigned short effectiveGameSpeed; // +0x38a4d
    short speedHysteresis;             // +0x38a4f
    unsigned short paused : 1;         // +0x38a51
    unsigned short lagging : 1;
    unsigned short faster : 1;
    unsigned short rest : 13;
    char unknown_38a53[0x38d75 - 0x38a53];
    volatile unsigned short netFlags;  // +0x38d75, 16-bit flags word (volatile: see below)
    char unknown_38d77[0x38d85 - 0x38d77];
    FrameTimers profileTimingBars;     // +0x38d85
    char unknown_38dd5[0x391e9 - 0x38dd5];
    Mission* mapInfo;                  // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned short bit0_3923b : 1;     // +0x3923b
    unsigned short rest_3923b : 15;
};
#pragma pack(pop)

extern int g_shareDialogPlayerNetIds[10];
extern int g_nonCampaignGame;
extern char g_chatDraftText[];
extern char g_livePlayerPrefix[];      // "LIVEPLYR"
extern char g_smallButtonSoundName[];  // "SmallButton"
extern char g_sendTypeGadgetName[];    // "SENDTYPE"
extern char g_sendToGadgetName[];      // "SENDTO"
extern char g_talkGadgetName[];        // "TALK"
extern char g_chatTargetSeparators[];  // ",:;"
extern char g_enemiesChatTarget[];     // "Enemies"
extern char g_alliesChatTarget[];      // "Allies"
extern int g_chatDraftInitialized;
extern unsigned char g_scorePanelKillFlash[10];
extern unsigned char g_scorePanelLossFlash[10];
extern int g_scorePanelSlidePos;
extern int g_scorePanelFlashDecayTick;
extern unsigned int g_cdActivityLastSampleTick;
extern int g_cdActivityStableTicks;
extern int g_cdActivityRingWriteIdx;
extern int g_cdActivitySampleRing[];
extern int g_lastCdActivityMode;
extern int g_usePacketManager;
extern PacketManager g_packetManager;

Gadget* __stdcall FindGadgetChecked_D(Gadget* entries, char* name);
int __stdcall ReadSliderValue(Gadget* entry);
void __stdcall SetTranslatedTextByName(Gui* menu, char* name, char* text, int param_4);
void __stdcall PlaySoundByName(char* name, int flag);
Gadget* __stdcall FindGadgetChecked(Gadget* entries, char* name);
int __stdcall IsCurrentGadgetNamed(Gui* menu, char* name);
void __cdecl GameFreeThunk(void* p);
void __stdcall MarkChanged(Gui* menu);
void __stdcall ClearSelectedGadget(Gui* menu);
void __stdcall TransferEnergy(unsigned char from, unsigned char to, float amount, int flag);
void __stdcall TransferMetal(unsigned char from, unsigned char to, float amount, int flag);
int __stdcall GetButtonStageByName(Gui* menu, char* name);
unsigned char __stdcall FindSlotByDpid(int id);
void __stdcall ShareMapInfo(unsigned char from, unsigned char to);
void __stdcall SendShareMapInfo(unsigned char from, unsigned char to);
void __stdcall CollectSelectedUnits(std::vector<Unit*>* list);
unsigned int* __stdcall GetCategoryMask(char* name);
void __stdcall GiveUnitToPlayer(Unit* unit, Player* player, int arg);
Layer* __stdcall LoadGuiLayer(Gui* menu, const char* name, int flags);
void __stdcall HandleShareDialogEvent(Gui* gadget);
int __stdcall FindGadgetIndex(Gadget* entries, char* name, int type);
void __stdcall SetSliderFromValue(Gadget* entry, int param_2);
void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __stdcall CloseTopScreen(Gui* menu);
void __stdcall ConfigureListBoxByName(Gui* menu, char* name, char* text, int count, int flag);
void __stdcall SetKeyboardInput(Gui* menu, int value);
void __stdcall RenderLayer(Gui* menu, int value);
void __stdcall SetGadgetActiveByName(Gui* menu, char* name, int value);
void __stdcall SetButtonStageByName(Gui* menu, char* name, int value);
int __stdcall GetButtonStage(Gui* menu, int index);
Gadget* __stdcall FindGadgetChecked_B(Gadget* entries, char* name);
void __stdcall GetGadgetText(Gui* menu, char* name, char* text);
void ResetPlayerGadgets();
void OpenTalkDialog();
int __stdcall ExecuteCommandLine(char* cmd, int flags);
void __stdcall TrySetFocus(Gui* menu, int index);
void __stdcall SendChatMessage(Player* from, char* text, int param_3, char* to);
void __stdcall HandleTalkDialogEvent(Gui* gadget);
void __stdcall RefreshAlliesScreen(int value);
Gadget* __stdcall FindGadgetChecked_E(Gadget* entries, char* name);
void __stdcall FreeSurface(void* param_1);
void __stdcall GetGadgetRect(Gadget* entry, Rect* rect);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall HandleUnitInfoDialogEvent(Gui* gadget);
void __stdcall DrawUnitInfoImage(Gui* gadget, Gadget* entry);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadPcx(char* path, int param);
char* __stdcall MakePropList(void* obj);
char* __stdcall Translate(char* text);
void __stdcall AddTextGadget(Layer* obj, char* name, char* text, int x, short y, int w, int flags);
int __stdcall IsUnitVisibleToPlayer(Player* player, Unit* unit);
unsigned short __stdcall FindUnitTypeId(char* name);
void __stdcall DisableKeyCommands(Gui* menu);
void OpenInGameOptions();
void OpenShareDialog();
void OpenControlDialog();
void OpenAlliesDialog();
void __stdcall HandleMain2LayoutEvent(Gui* gadget);
unsigned int GetTicks();
int GetScreenWidth();
int __stdcall IsKeyDown(int key);
void __stdcall FadeRectangle(void* surface, Rect* rect, int level);
void __stdcall DrawTextClipped(void* surface, char* text, int x, int y, int maxw, int style);
int __stdcall GetTextPixelWidth(char* text);
void* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrameQuad(void* surface, void* pic, Quad* dst, Quad* src);
void ReportIntervalTimer();
int __stdcall IsScreenNamed(Gui* menu, const char* name);
void HideSoftwareCursor();
void __stdcall HandleTabMenuEvent(Gui* gadget);
int IsHostLocal();
void __stdcall EnableKeyCommands(Gui* menu);
void ShowSoftwareCursor();
void HandleNetPackets(void);
void UpdateAllUnits(void);
void UpdateProjectiles(void);
void UpdateExplosions(void);
void UpdatePlayers(void);
void UpdateFeatures(void);
void StepAllGafSequences(void);
void UpdateWind(void);
void UpdateMeteors(void);
void UpdateCameraFollow(void);
void UpdateParticles(void);
void UpdateBlink(void);
void EmptyPostSimStepHook(void);
void EmptyPostSimStepHook_B(void);
void EmptyPostSimStepHook_C(void);
void ExpireOldestMessage(void);
void ExpireEyeballs(void);
void __stdcall UpdateResourceSharing(Player* player);
int __stdcall SetPageFlipping(int enable);

// FUNCTION: 0x493340
void __stdcall UpdateMetalReadout(Gui* obj, int unused)
{
    char buf[52];
    Gadget* value = FindGadgetChecked_D(obj->layer->entries, "METAL");
    if (value != 0) {
        sprintf(buf, "%d", ReadSliderValue(value));
        SetTranslatedTextByName(obj, "METAL#", buf, 0);
    }
}

// FUNCTION: 0x493390
void __stdcall UpdateEnergyReadout(Gui* obj, int unused)
{
    char buf[52];
    Gadget* value = FindGadgetChecked_D(obj->layer->entries, "ENERGY");
    if (value != 0) {
        sprintf(buf, "%d", ReadSliderValue(value));
        SetTranslatedTextByName(obj, "ENERGY#", buf, 0);
    }
}

// The same inlined bit-set test as 0x41c310.
static inline int TestBit(unsigned int* set, unsigned short n)
{
    return set[n >> 5] & (1 << (n & 0x1f));
}

// CollectSelectedUnits clears the vector and fills it with the local player's
// units that have bit 4 of +0x110 set (probably the selected units); each one
// whose low two flag bits are not 2, with +0x86 and +0x8a clear and whose type
// is not in the "Commander" set, is handed to the given player with
// GiveUnitToPlayer.
// The list is a real std::vector<Unit*> (0x48ca20 calls the vector's _Ucopy,
// _Ufill and _Destroy).

// FUNCTION: 0x4933e0
void __stdcall GiveSelectedUnitsToPlayer(unsigned char player)
{
    extern Game* g_game;
    // Must be a real std::vector: its constructor leaves the player index in place.
    std::vector<Unit*> list;
    CollectSelectedUnits(&list);
    Player* p = &g_game->players[player];
    unsigned int* set = GetCategoryMask("Commander");
    for (std::vector<Unit*>::iterator it = list.begin(); it != list.end(); it++) {
        Unit* unit = *it;
        if ((unit->flags & 3) != 2 && unit->cargo == 0 && unit->carrier == 0
            && !TestBit(set, unit->unitDefIndex)) {
            GiveUnitToPlayer(unit, p, 0);
        }
    }
}

static inline int IsPlaying_4934b0(Player* p)
{
    return p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
        && p->index != 10;
}

static inline int IsCounted_4934b0(Player* p)
{
    return (p->type == 1 || p->type == 2 || p->type == 3)
        && (p->unitCount != 0 || p->unitsCreated == 0);
}

// FUNCTION: 0x4934b0
void __stdcall HandleShareDialogEvent(Gui* obj)
{
    extern Game* g_game;
    Gadget* data = obj->layer->entries;

    if (obj->hotGadgetIndex == -1) {
        Gadget* e = FindGadgetChecked(data, "PLYRLIST");
        GameFreeThunk(e->u.list.records);
        g_game->ordersPanelFlags &= ~0x40;
        return;
    }
    if (IsCurrentGadgetNamed(obj, "MAPINFO")) {
        MarkChanged(obj);
        PlaySoundByName("Options", 0);
        ClearSelectedGadget(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, "SHARUNIT")) {
        MarkChanged(obj);
        PlaySoundByName("Options", 0);
        ClearSelectedGadget(obj);
        return;
    }
    if (IsCurrentGadgetNamed(obj, "OK")) {
        PlaySoundByName("Options", 0);
        Gadget* plyr = FindGadgetChecked(data, "PLYRLIST");
        short idx = plyr->u.list.field_ba;
        if (idx < 0)
            return;
        int pi = FindSlotByDpid(g_shareDialogPlayerNetIds[idx]);
        Player* p = &g_game->players[pi];
        if (IsPlaying_4934b0(p) && !(p->info->flags_9b & 0x40) && IsCounted_4934b0(p)) {
            TransferEnergy(g_game->localPlayer, pi,
                         (float)ReadSliderValue(FindGadgetChecked_D(data, "METAL")), 1);
            TransferMetal(g_game->localPlayer, pi,
                         (float)ReadSliderValue(FindGadgetChecked_D(data, "ENERGY")), 1);
            if (GetButtonStageByName(obj, "SHARUNIT"))
                GiveSelectedUnitsToPlayer(pi);
            if (GetButtonStageByName(obj, "MAPINFO")) {
                ShareMapInfo(g_game->localPlayer, pi);
                SendShareMapInfo(g_game->localPlayer, pi);
            }
        }
        return;
    }
    if (IsCurrentGadgetNamed(obj, "CANCEL")) {
        PlaySoundByName("Previous", 0);
        return;
    }
    if (obj->hotGadgetIndex != -1)
        ClearSelectedGadget(obj);
}

// Opens the resource sharing screen (SHARE.GUI): it walks the ten player
// records, finds the local player's entry (flagged 0x40), points the METAL and
// ENERGY sliders at the local counts, and finishes with the menu setup calls.
// A resource's owner is flagged 0x40 in the player record; the two sliders are
// the "METAL#" and "ENERGY#" texts.

// FUNCTION: 0x4936f0
void OpenShareDialog()
{
    extern Game* g_game;
    if (g_game->players[g_game->localPlayer].info->bit6)
        return;
    Layer* layer = LoadGuiLayer(&g_game->gui, "SHARE.GUI", 0x800);
    g_game->bit6_37ebe = 1;
    Gadget* entries = layer->entries;
    layer->handler = (void (__stdcall*)(void*))HandleShareDialogEvent;
    layer->data = g_game;
    int idx = FindGadgetIndex(entries, "METAL", 0xe);
    if (idx != -1) {
        Gadget* e = &entries[idx];
        e->knobSize = layer->entries[idx].height;
        e->range = layer->entries[idx].width - e->knobSize;
        e->max = (int)g_game->players[g_game->localPlayer].metal;
        e->sliderCallback = (void (__stdcall*)(Gui*, int))UpdateMetalReadout;
        e->knobPos = 0;
        SetSliderFromValue(e, 0);
        e->sliderUser = (int)g_game;
    }
    idx = FindGadgetIndex(layer->entries, "ENERGY", 0xe);
    if (idx != -1) {
        Gadget* e = FindGadgetChecked_D(layer->entries, "ENERGY");
        e->knobSize = layer->entries[idx].height;
        e->range = layer->entries[idx].width - e->knobSize;
        e->max = (int)g_game->players[g_game->localPlayer].energy;
        e->sliderCallback = (void (__stdcall*)(Gui*, int))UpdateEnergyReadout;
        e->knobPos = 0;
        SetSliderFromValue(e, 0);
        e->sliderUser = (int)g_game;
    }

    char* names = (char*)GameAllocIgnoreTag("PLAYERS", g_game->numPlayers * 30);
    char* np = names;
    *np = 0;
    memset(g_shareDialogPlayerNetIds, -1, sizeof(g_shareDialogPlayerNetIds));
    int* ids = g_shareDialogPlayerNetIds;
    int count = 0;
    for (int i = 0; i < 10; i++) {
        Player* p = &g_game->players[i];
        if (p->active && (p->type == 1 || p->type == 2 || p->type == 3) && p->index != 10 &&
            (p->unitCount != 0 || p->unitsCreated == 0) && p->type != 1 && !p->info->bit6) {
            strcpy(np, p->name);
            np += strlen(p->name) + 1;
            *ids = p->id;
            count++;
            ids++;
        }
    }
    if (count == 0) {
        CloseTopScreen(&g_game->gui);
        return;
    }
    ConfigureListBoxByName(&g_game->gui, "PLYRLIST", names, count, 0);
    char text[0x34];
    // Both tail blocks go through the local menu pointer, menu first, with no
    // self-comparison: this fixes their register choice.
    Gui* menu = &g_game->gui;
    Layer* lyr = menu->layer;
    Gadget* ents = lyr->entries;
    Gadget* e = FindGadgetChecked_D(ents, "METAL");
    if (e) {
        sprintf(text, "%d", ReadSliderValue(e));
        SetTranslatedTextByName(menu, "METAL#", text, 0);
    }
    menu = &g_game->gui;
    lyr = menu->layer;
    ents = lyr->entries;
    e = FindGadgetChecked_D(ents, "ENERGY");
    if (e) {
        sprintf(text, "%d", ReadSliderValue(e));
        SetTranslatedTextByName(menu, "ENERGY#", text, 0);
    }
    MarkChanged(&g_game->gui);
    SetKeyboardInput(&g_game->gui, 1);
    RenderLayer(&g_game->gui, 0x40);
}

// Resets every player's "PLAYER%d" gadget and publishes a "LIVEPLYR%d" value
// for each occupied slot except the local player's: 1, whether the local
// player considers that slot an ally, or the flag at g_game+0x2bf1, depending
// on the chat mode at g_game+0x2bf0.

// FUNCTION: 0x493ae0
void ResetPlayerGadgets(void)
{
    extern Game* g_game;
    char buf[52];
    unsigned char* p = g_game->players[g_game->localPlayer].allied;
    for (int i = 0; i < 10; i++, p++) {
        sprintf(buf, "PLAYER%d", i);
        SetGadgetActiveByName(&g_game->gui, buf, 0);
        unsigned char state = g_game->players[i].type;
        if (state != 0 && state != 4 && i != g_game->localPlayer) {
            sprintf(buf, "LIVEPLYR%d", i);
            int value = 0;
            unsigned char* flags = g_game->chatRecipients;
            switch (g_game->chatMode) {
            case 1:
                value = *p;
                break;
            case 2:
                value = (*p == 0);
                break;
            case 0:
                value = 1;
                break;
            case 3:
                value = flags[i];
                break;
            }
            SetButtonStageByName(&g_game->gui, buf, value);
        }
    }
}

// FUNCTION: 0x493bf0
void __stdcall HandleTalkDialogEvent(Gui* gadget)
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
    Gadget* entries = gadget->layer->entries;
    // current is read directly, not through an id local.
    if (gadget->hotGadgetIndex == -1) {
        g_game->ordersPanelFlags &= ~4;
        return;
    }
    if (_strnicmp(entries[gadget->hotGadgetIndex].name, g_livePlayerPrefix, 8) == 0) {
        PlaySoundByName(g_smallButtonSoundName, 0);
        g_game->chatMode = 3;
        SetButtonStageByName(gadget, g_sendTypeGadgetName, g_game->chatMode);
        n = atoi(&entries[gadget->hotGadgetIndex].name[8]);
        // Kept as the original has it: n is never range checked before it
        // indexes the 11-byte selection mask, so a "LIVEPLYR42" style name
        // writes outside chatRecipients. The neighbouring chatMode is clamped
        // (`if (g_game->chatMode >= 4) g_game->chatMode = 0;`), so the
        // omission looks like an oversight rather than a deliberate choice.
        unsigned char v = (unsigned char)GetButtonStage(gadget, gadget->hotGadgetIndex);
        g_game->chatRecipients[n] = v;
        MarkChanged(gadget);
        ClearSelectedGadget(gadget);
        goto tail;
    }
    if (IsCurrentGadgetNamed(gadget, g_sendToGadgetName)) {
        PlaySoundByName(g_smallButtonSoundName, 0);
        unsigned char v = (unsigned char)GetButtonStage(gadget, gadget->hotGadgetIndex);
        g_game->bits_2bee.bit8 = v & 1;
        GetGadgetText(gadget, g_talkGadgetName, g_chatDraftText);
        CloseTopScreen(gadget);
        OpenTalkDialog();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, g_sendTypeGadgetName)) {
        PlaySoundByName(g_smallButtonSoundName, 0);
        g_game->chatMode = (unsigned char)GetButtonStageByName(gadget, g_sendTypeGadgetName);
        if (g_game->chatMode >= 4)
            g_game->chatMode = 0;
        ResetPlayerGadgets();
        ClearSelectedGadget(gadget);
        goto tail;
    }
    if (IsCurrentGadgetNamed(gadget, g_talkGadgetName)) {
        Gadget* talk = FindGadgetChecked_B(entries, g_talkGadgetName);
        mode = g_game->chatMode;
        lstrcpynA(buf, (char*)talk + 0xb6, 0x100);
        char* p = buf;
        while (*p && *p == ' ')
            p++;
        if (*p == '+') {
            int flags = 1;
            // The cast keeps the shift a 16-bit one, which is what stops MSVC
            // folding this into `test byte ptr [g_game + 0x37f2f], 2`.
            if (flags & (unsigned char)(g_game->uiOptionFlags >> 1))
                flags = 7;
            if (g_nonCampaignGame)
                flags |= 2;
            int r = ExecuteCommandLine(p + 1, flags);
            entries = gadget->layer->entries;
            if (r & 2)
                mode = 0;
        }
        if (strlen(p) != 0) {
            // These four statements stay in this order: oldmode, to, base, saved.
            oldmode = g_game->chatMode;
            char* to = 0;
            Player* base = &g_game->players[g_game->localPlayer];
            Saved saved = *(Saved*)g_game->chatRecipients;
            // Early out rather than a positive `if`, and the empty statement
            // at skip0 below is load bearing: either change costs 9 points.
            if (!(' ' < p[1] && strchr(g_chatTargetSeparators, p[1]) != 0)) goto skip0;
            if (isdigit(p[0])) {
                d = p[0] - '0';
                if (d < 0 || d > 9 || g_game->players[d].id == 0)
                    goto clear;
                p += 2;
                mode = 3;
                // to is computed before the memset.
                to = g_game->players[d].name;
                memset(g_game->chatRecipients, 0, 11);
                g_game->chatRecipients[d] = 1;
            } else {
                int c = tolower(p[0]);
                if (c != 'a') {
                    if (c == 'e') {
                        mode = 2;
                        to = g_enemiesChatTarget;
                    } else {
                        goto after;
                    }
                } else {
                    mode = 1;
                    to = g_alliesChatTarget;
                }
                p += 2;
            }
skip0:;
after:
            g_game->chatMode = mode;
            memset(buf2, 0, sizeof(buf2));
            SendChatMessage(base, p, 4, to);
            *(Saved*)g_game->chatRecipients = saved;
            g_game->chatMode = oldmode;
        }
clear:
        memset(g_chatDraftText, 0, 0x81);
        g_game->bits_2bee.bit8 = 0;
    }
tail:
    int index = FindGadgetIndex(entries, g_talkGadgetName, 3);
    TrySetFocus(&g_game->gui, index);
    g_game->gui.layer->current = FindGadgetIndex(entries, g_talkGadgetName, 3);
}

// The functions before 0x493bf0 declare g_game inside their bodies: its symbol
// id has to come after the locals of 0x493bf0 (the order of its byte stores).
// GLOBAL: 0x511de8
extern Game* g_game;
// FUNCTION: 0x494050
void OpenTalkDialog()
{
    PlayerInfo* info = g_game->players[g_game->localPlayer].info;
    if (info->bit6)
        return;
    if (g_chatDraftInitialized == 0) {
        g_chatDraftInitialized = 1;
        memset(g_chatDraftText, 0, 0x81);
    }
    if (g_game->ordersPanelFlags & 0x800)
        return;
    int multi = (g_game->lobbyUiDirtyFlags & 0x100)
                && g_game->mapInfo->GetGameType() == 3;
    Layer* d = LoadGuiLayer(&g_game->gui,
                            multi ? "TALK2.GUI" : "TALK.GUI",
                            multi ? 0x800 : 0x880);
    Gadget* entries = d->entries;
    d->handler = (void (__stdcall*)(void*))HandleTalkDialogEvent;
    g_game->ordersPanelFlags |= 4;
    SetTranslatedTextByName(&g_game->gui, "TALK", g_chatDraftText, 0);
    SetButtonStageByName(&g_game->gui, "SENDTO", multi);
    if (g_game->mapInfo->GetGameType() != 3) {
        SetGadgetActiveByName(&g_game->gui, "SENDTO", 0);
    } else if (multi) {
        SetButtonStageByName(&g_game->gui, "SENDTYPE", g_game->chatMode);
        RefreshAlliesScreen(1);
        ResetPlayerGadgets();
    }
    TrySetFocus(&g_game->gui, FindGadgetIndex(entries, "TALK", 3));
    d->current = FindGadgetIndex(entries, "TALK", 3);
    d->data = g_game;
    RenderLayer(&g_game->gui, 0x40 | (multi ? 0 : 0x80));
}

// FUNCTION: 0x494220
void __stdcall HandleUnitInfoDialogEvent(Gui* gadget)
{
    if (gadget->hotGadgetIndex == -1) {
        Gadget* e = FindGadgetChecked_E(gadget->layer->entries, "HOTR");
        FreeSurface((void*)e->u.anim.value);
        g_game->ordersPanelFlags &= ~0x800;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "DONE")) {
        PlaySoundByName("smlbutton", 0);
        return;
    }
    ClearSelectedGadget(&g_game->gui);
}

// Draws a gadget's image at the gadget's position (GetGadgetRect fills its
// bounding rectangle).

// FUNCTION: 0x494290
void __stdcall DrawUnitInfoImage(Gui* gadget, Gadget* entry)
{
    if (entry->u.anim.value) {
        Rect r;
        GetGadgetRect(entry, &r);
        DrawSurface(gadget->layer->entries->u.assets.surface, (void*)entry->u.anim.value, r.left, r.top);
    }
}

// FUNCTION: 0x4942e0
void __stdcall OpenUnitInfoDialog(void)
{
    if (g_game->bit11_37ebe)
        return;

    int y;
    char* stats;
    char name[0x20];
    char buf[0x100];
    unsigned short type = 0;

    if (g_game->gui.hoverGadgetIndex != -1) {
        strncpy(name, g_game->gui.layer->entries[g_game->gui.hoverGadgetIndex].name, 0x10);
        name[0x10] = 0;
        type = FindUnitTypeId(name);
    } else {
        unsigned short t = g_game->hoverUnitId;
        if (t != 0) {
            Unit* unit = &g_game->units[t];
            Player* owner = &g_game->players[g_game->playerIndex];
            if (IsUnitVisibleToPlayer(owner, unit) == 0)
                type = 0;
            else
                type = unit->unitDefIndex;
        }
    }

    if (type == 0)
        return;

    Layer* layer = LoadGuiLayer(&g_game->gui, "UNITINFOx.GUI", 0x1000);
    Gadget* entries = layer->entries;
    layer->handler = (void (__stdcall*)(void*))HandleUnitInfoDialogEvent;
    layer->data = g_game;
    Gadget* hotr = FindGadgetChecked_E(entries, "HOTR");
    hotr->u.hotspot.callback = (void (__stdcall*)(Gui*, Gadget*))DrawUnitInfoImage;
    char* def = g_game->unitDefs + 0x249 * (unsigned)type;
    BuildDataPath(buf, "unitpics", def + 0x20, "PCX");
    hotr->u.anim.value = (int)LoadPcx(buf, 0);
    stats = MakePropList(def);
    int n = entries->u.count;

    AddTextGadget(g_game->gui.layer, "TEXT", Translate("Cost"), 0x82, 0x20, -1, 2);
    n++;
    entries[n].attribs = 0x411;
    AddTextGadget(g_game->gui.layer, "TEXT", Translate("Energy"), 0x8c, 0x2f, -1, 2);
    n++;
    entries[n].attribs = 0x411;
    AddTextGadget(g_game->gui.layer, "TEXT", Translate("Metal"), 0x8c, 0x3e, -1, 2);
    n++;
    entries[n].attribs = 0x411;
    AddTextGadget(g_game->gui.layer, "TEXT", Translate("Build Time"), 0x8c, 0x4d, -1, 2);
    n++;
    entries[n].attribs = 0x411;
    AddTextGadget(g_game->gui.layer, "TEXT", Translate("Statistics"), 0x82, 0x5c, -1, 2);
    n++;
    entries[n].attribs = 0x411;
    AddTextGadget(g_game->gui.layer, "TEXT", Translate("Max Velocity"), 0x8c, 0x6b, -1, 2);
    n++;
    entries[n].attribs = 0x411;
    AddTextGadget(g_game->gui.layer, "TEXT", Translate("Acceleration"), 0x8c, 0x7a, -1, 2);
    n++;
    entries[n].attribs = 0x411;
    AddTextGadget(g_game->gui.layer, "TEXT", Translate("Turn Rate"), 0x8c, 0x89, -1, 2);
    n++;
    entries[n].attribs = 0x411;

    char* s = stats;
    if (*s != 0) {
        y = 0x20;
        do {
            AddTextGadget(g_game->gui.layer, "TEXT", s, 0xf0, y, -1, 2);
            n++;
            entries[n].attribs = 0x411;
            s += strlen(s) + 1;
            y += 0xf;
        } while (*s != 0);
    }

    GameFreeThunk(stats);
    SetTranslatedTextByName(&g_game->gui, "NAME", def, 0x80);
    MarkChanged(&g_game->gui);
}

// FUNCTION: 0x494740
void __stdcall HandleTabMenuEvent(Gui* gadget)
{
    if (gadget->hotGadgetIndex == -1) {
        g_game->lobbyUiDirtyFlags &= 0xff1f;
        DisableKeyCommands(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OPTIONS")) {
        g_game->byte_37ebe |= 1;
        PlaySoundByName("BigButton", 0);
        OpenInGameOptions();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "SHARE")) {
        PlaySoundByName("BigButton", 0);
        OpenShareDialog();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "CONTROL")) {
        PlaySoundByName("BigButton", 0);
        OpenControlDialog();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "ALLIES")) {
        PlaySoundByName("BigButton", 0);
        OpenAlliesDialog();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "CANCEL")) {
        CloseTopScreen(gadget);
        return;
    }
    ClearSelectedGadget(gadget);
}

// FUNCTION: 0x494840
void __stdcall HandleTabMenuCancel(Gui* gadget)
{
    if (gadget->hotGadgetIndex == -1) {
        g_game->lobbyUiDirtyFlags &= 0xff1f;
        DisableKeyCommands(&g_game->gui);
        return;
    }
    if (!IsCurrentGadgetNamed(gadget, "CANCEL"))
        ClearSelectedGadget(gadget);
}

// FUNCTION: 0x494890
void __stdcall HandleMain2LayoutEvent(Gui* gadget)
{
    if (gadget->hotGadgetIndex != -1) {
        ClearSelectedGadget(gadget);
    }
}

// FUNCTION: 0x4948b0
void __stdcall FlashScorePanelKillLoss(int param_1, int param_2)
{
    if (param_1 >= 0) {
        g_scorePanelKillFlash[param_1] = 0x1e;
    }
    if (param_2 >= 0) {
        g_scorePanelLossFlash[param_2] = 0x1e;
    }
}

// FUNCTION: 0x4948e0
void __stdcall DrawScorePanel(void* surface)
{
    if (g_scorePanelFlashDecayTick < (int)GetTicks()) {
        g_scorePanelFlashDecayTick = GetTicks() + 1;
        for (int i = 0; i < 10; i++) {
            if (g_scorePanelKillFlash[i] > 0)
                g_scorePanelKillFlash[i] -= 2;
            if (g_scorePanelLossFlash[i] > 0)
                g_scorePanelLossFlash[i] -= 2;
        }
    }

    if (!(g_game->visualFlags & 0x80)
        && (IsKeyDown(0x20) == 0
            || (g_game->gui.focus != -1
                && g_game->gui.layer->entries[g_game->gui.focus].type == 3))) {
        if (g_scorePanelSlidePos <= 0)
            return;
        if (g_scorePanelSlidePos == 0x7d)
            PlaySoundByName("Panel", 0);
        int q = g_scorePanelSlidePos / 4;
        if (q <= 1)
            q = 1;
        g_scorePanelSlidePos -= q;
        if (g_scorePanelSlidePos <= 0) {
            g_scorePanelSlidePos = 0;
            PlaySoundByName("Options", 0);
        }
    } else if (g_scorePanelSlidePos < 0x7d) {
        if (g_scorePanelSlidePos == 0)
            PlaySoundByName("Panel", g_scorePanelSlidePos);
        int q = (0x7d - g_scorePanelSlidePos) / 4;
        if (q <= 1)
            q = 1;
        g_scorePanelSlidePos += q;
        if (g_scorePanelSlidePos >= 0x7d) {
            g_scorePanelSlidePos = 0x7d;
            PlaySoundByName("Options", 0);
        }
    }

    Rect panel;
    panel.left = GetScreenWidth() - g_scorePanelSlidePos;
    panel.top = 0x20;
    panel.right = panel.left + 0x7d;
    panel.bottom = g_game->numPlayers * 0x28 + 0x2e;
    FadeRectangle(surface, &panel, -0x18);

    panel.right = panel.left + 0x7d;
    int y = panel.top;
    int maxw = panel.right - panel.left - 6;
    char buf[100];
    Quad src;
    src.p[0].x=1; src.p[0].y=1; src.p[3].x=1; src.p[1].y=1;
    strcpy(buf, Translate("Kills"));
    DrawTextClipped(surface, buf, panel.left + 2, y, maxw, 0);
    strcpy(buf, Translate("Losses"));
    DrawTextClipped(surface, buf, panel.right - GetTextPixelWidth(buf) - 2, y, maxw, 0);
    y += 0xf;

    for (int i = 0; i < (int)g_game->numPlayers; i++) {
        Player* p = g_game->players;
        // Chained assignments fix the store order; the right edge is
        // panel.left + maxw.
        Quad dst;
        dst.p[0].x = dst.p[3].x = panel.left + 7;
        dst.p[2].x = dst.p[1].x = panel.left + maxw;
        dst.p[0].y = dst.p[1].y = y + 1;
        dst.p[2].y = dst.p[3].y = y + 0x25;

        // The draw block stays inside the search loop; cleanup follows the loop.
        int n;
        for (n = 0; n < 10; n++, p++) {
            if (p->active == 0)
                continue;
            unsigned char c = p->type;
            if (c != 1 && c != 2 && c != 3)
                continue;
            if (p->index == 0xa)
                continue;
            if (p->unitCount == 0 && p->unitsCreated != 0)
                continue;
            if (p->info->flags_9b & 0x40)
                continue;
            if (p->rank != i)
                continue;
            // Assigned in the order left, right, top, bottom.
            Rect hr;
            hr.left = panel.left + 4;
            hr.right = panel.right - 4;
            hr.top = y - 1;
            hr.bottom = y + 0x26;
            if (n == g_game->localPlayer) {
                FadeRectangle(surface, &hr, 0x1f);
                FadeRectangle(surface, &hr, 0x14);
            }
            unsigned short* frame = (unsigned short*)GetGafFrame(
                (void*)g_game->logos32, p->info->color);

            src.p[1].x = frame[0] - 1;
            src.p[2].x = frame[0] - 1;
            src.p[2].y = frame[1] - 1;
            src.p[3].y = frame[1] - 1;
            DrawFrameQuad(surface, frame, &dst, &src);

            DrawTextClipped(surface, p->name, dst.p[0].x + 2, dst.p[0].y + 5, maxw, 0);
            int kills = g_game->battleCommanderDeath == 2 ? p->commanderKills : p->kills;
            sprintf(buf, "%d", kills);
            DrawTextClipped(surface, buf, dst.p[0].x + 2, dst.p[0].y + 0x14, maxw,
                         g_scorePanelKillFlash[n]);
            int losses = g_game->battleCommanderDeath == 2 ? p->commanderLosses : p->losses;
            sprintf(buf, "%d", losses);
            DrawTextClipped(surface, buf, dst.p[1].x - GetTextPixelWidth(buf) - 2,
                         dst.p[0].y + 0x14, maxw, g_scorePanelLossFlash[n]);
            y += 0x28;
            break;
        }
        if (n == 10) {
            Player* q = g_game->players;
            // Countdown k = n gives the frame its stack slot.
            for (int k = n; k != 0; k--, q++) {
                if (q->active == 0)
                    continue;
                unsigned char c = q->type;
                if (c != 1 && c != 2 && c != 3)
                    continue;
                if (q->index == 0xa)
                    continue;
                if (q->unitCount == 0 && q->unitsCreated != 0)
                    continue;
                if (q->info->flags_9b & 0x40)
                    continue;
                if (q->rank > i)
                    q->rank--;
            }
        }
    }
}

// Frame-time / desync watchdog: keeps a 30-entry ring of per-frame counters at
// g_cdActivitySampleRing, and when the ring total (or the last five entries) grows too
// large it flips the network state g_lastCdActivityMode between 0 and 1 and resets the
// counter g_cdActivityStableTicks.
//
// Matches with netFlags declared `volatile`. Without it, only the two tests
// on g_game->netFlags at 0x494e8b differ. The original emits two memory-operand bit tests
// (`test byte ptr [eax+0x38d75],1` then `...,2`); this source makes MSVC load
// the byte once (`mov al,[eax+0x38d75]; test al,1; test al,2`), 4 bytes
// shorter. The same pair appears in 0x452800 and in 0x453d40 (at 0x4550c2).
//
// Notes from Claude Opus 5.5 (#436): the difference is not compiler state.
// It scores 78.9% for 0 to 400 unused `extern int`s, for 500 to 8000 unused
// prototypes, for every headers.py set and with <windows.h> plus <string>,
// <vector> + <map>, <iostream>, <list> or the DirectX headers. In scratch
// tests MSVC 5 shares the load across the two tests for every non-volatile
// spelling (bitfields, masks, byte/word/dword union views, inline helpers
// taking the object or a reference, ternaries, switch, separate ifs).
// Adding `volatile` to the declaration of netFlags (a 16-bit flags word)
// makes this function MATCH with no other change; adding `volatile` to
// 0x452800's declaration of the field makes that function MATCH as well.
// Evidence that Cavedog really declared this word volatile: every write to
// it in the game (0x496861, 0x4975c0 and
// 0x497c57) is a word read-modify-write through a register
// (`mov cx,[m]; or ecx,4; mov [m],cx`), which MSVC 5 emits only for a
// volatile field; every non-volatile form (`w |= 4`, `w = w & ~4`, a 1-bit
// bitfield store, a struct copy) compiles to `or byte ptr [m],4` or
// `and word ptr [m],0xfffb` straight to memory. Accepted as the original
// declaration (network state, likely also touched from DirectPlay's thread);
// see docs/consolidation.md.

// FUNCTION: 0x494e70
void UpdateCdCategoryByActivity()
{
    if ((g_game->flags_2a44 & 4)
        && (!(g_game->netFlags & 1) || (g_game->netFlags & 2))
        && GetTicks() > g_cdActivityLastSampleTick + 0x1e) {
        int state = g_game->sound->GetTrackCategory();
        if (++g_cdActivityStableTicks > 10) {
            int recent = 0;
            int n = 5;
            int i = g_cdActivityRingWriteIdx - 1;
            int total = 0;
            int* p = &g_cdActivitySampleRing[i];
            for (;;) {
                if (i < 0) {
                    i += 30;
                    p += 30;
                }
                int v = *p;
                total += v;
                if (n != 0) {
                    n--;
                    recent += v;
                }
                if (i == g_cdActivityRingWriteIdx)
                    break;
                i--;
                p--;
            }
            int newstate = g_lastCdActivityMode;
            if (state == 0 && (total > 0x32 || recent > 0x1e)
                && g_game->players[g_game->localPlayer].unitCount > 0x1e)
                newstate = 1;
            else if (state == 1 && total < 10 && recent == 0 && g_cdActivityStableTicks > 0x3c)
                newstate = 0;
            if (newstate != g_lastCdActivityMode) {
                g_game->sound->SetTrackCategory(newstate);
                g_cdActivityStableTicks = 0;
                g_lastCdActivityMode = newstate;
            }
        }
        if (++g_cdActivityRingWriteIdx >= 30)
            g_cdActivityRingWriteIdx = 0;
        g_cdActivitySampleRing[g_cdActivityRingWriteIdx] = 0;
        ReportIntervalTimer();
        g_cdActivityLastSampleTick = GetTicks();
    }
}

// FUNCTION: 0x494ff0
void __stdcall AddCdActivitySample(int param_1)
{
    g_cdActivitySampleRing[g_cdActivityRingWriteIdx] += param_1;
}

// What the code does: it opens (or closes) the TABMENU.GUI tab menu page. If any
// of bits 5 to 7 of the flags word at +0x2bee is set the page is being closed:
// the bits are cleared and the gui is either hidden, or torn down when
// IsScreenNamed does not find the file. Otherwise the bit 5 "open" flag is set,
// the gui is fetched through LoadGuiLayer and given a click handler and an owner,
// and the three ALLIES / SHARE / CONTROL checkboxes are set. The two first ones
// are enabled when the player count of free, non allied slots is positive and
// the net mode is 3, CONTROL also needs bit 0 of +0x2c74 clear and IsHostLocal
// true.

// FUNCTION: 0x495010
void ToggleTabMenu()
{
    PlaySoundByName("SmallButton", 0);
    unsigned short f = g_game->lobbyUiDirtyFlags;
    if (f & 0xe0) {
        g_game->lobbyUiDirtyFlags = f & 0xff1f;
        if (IsScreenNamed(&g_game->gui, "TABMENU.GUI"))
            CloseTopScreen(&g_game->gui);
        return;
    }
    g_game->lobbyUiDirtyFlags = (f & 0xff3f) | 0x20;
    HideSoftwareCursor();
    Layer* d = LoadGuiLayer(&g_game->gui, "TABMENU.GUI", 0x800);
    d->data = g_game;
    d->handler = (void (__stdcall*)(void*))HandleTabMenuEvent;

    // Single count++ body, no extra locals: sets the register split of the scan.
    int count = 0;
    Player* p = g_game->players;
    for (int i = 0; i < 10; i++, p++) {
        if (p->active != 0 && p->type == 1)
            continue;
        if (p->info->bit6)
            continue;
        count++;
    }
    // The players[localPlayer] scale by 0x14b is left to the compiler.
    int mode = g_game->mapInfo->GetGameType();
    if (mode == 3 && !g_game->players[g_game->localPlayer].info->bit6) {
        int v = count > 0;
        SetGadgetActiveByName(&g_game->gui, "ALLIES", v);
        SetGadgetActiveByName(&g_game->gui, "SHARE", v);
        int ctl = !(g_game->lockFlags & 1) && IsHostLocal();
        SetGadgetActiveByName(&g_game->gui, "CONTROL", ctl);
    } else {
        SetGadgetActiveByName(&g_game->gui, "ALLIES", 0);
        SetGadgetActiveByName(&g_game->gui, "SHARE", 0);
        SetGadgetActiveByName(&g_game->gui, "CONTROL", 0);
    }
    EnableKeyCommands(&g_game->gui);
    RenderLayer(&g_game->gui, 0x40);
    ShowSoftwareCursor();
}

// Opens a dialog whose GUI file name is kept in the game object, with
// HandleMain2LayoutEvent as its handler and the game object as its owner.

// FUNCTION: 0x495200
void OpenMain2Layout()
{
    Layer* gadget = LoadGuiLayer(&g_game->gui, g_game->mainHudLayoutName, 0x20);
    gadget->handler = (void (__stdcall*)(void*))HandleMain2LayoutEvent;
    gadget->data = g_game;
}

// Frame pacing: measures the time since the last call, finds the slowest
// active player (the one with the lowest tick counter that still has units),
// and turns the lag behind the fastest into a speed factor. That factor scales
// the elapsed time into a whole number of game ticks for this frame (the
// fraction is carried over in a float), which is clamped to 0..5 and to 0
// while paused. Too many capped frames in a row lowers the current speed step,
// a long run of idle ones raises it again.

// FUNCTION: 0x495230
void UpdateFramePacing()
{
    unsigned int now = GetTicks();
    g_game->simBudgetDtTicks = now - g_game->lastSimBudgetTick;
    g_game->lastSimBudgetTick = now;
    unsigned short speed = g_game->effectiveGameSpeed;
    double rate = speed * 0.1;
    g_game->faster = speed < g_game->speedCtrl;

    int best = g_game->gameTick;
    g_game->slowest = 0;
    for (Player* p = g_game->players; p != g_game->players + 10; p++) {
        if (p->active != 0 && p->type == 3 && p->unitCount > 0) {
            int tick = p->syncTick;
            if (tick < best) {
                g_game->slowest = p;
                best = tick;
            }
        }
    }
    g_game->lag = g_game->gameTick - best;

    if (g_game->lag >= 900) {
        int lag = g_game->lag;
        if (lag > 3600)
            lag = 3600;
        double f = (3600 - lag) * 0.00037037037037037035;
        if (f < 0.01)
            f = 0.01;
        g_game->lagging = 1;
        rate *= f;
    } else {
        g_game->lagging = 0;
    }

    double x = g_game->simBudgetDtTicks * rate + g_game->carry;
    double whole = floor(x);
    g_game->simStepsPending = (int)whole;
    // No (float) cast: with it the fsub moves after the steps store.
    g_game->carry = x - whole;
    if (g_game->simStepsPending < 0)
        g_game->simStepsPending = 0;
    if (g_game->paused) {
        g_game->simStepsPending = 0;
        return;
    }
    if (g_game->simStepsPending > 5) {
        g_game->simStepsPending = 5;
        g_game->speedHysteresis++;
        if (g_game->speedHysteresis > 10) {
            g_game->speedHysteresis = 0;
            if (g_game->effectiveGameSpeed > 1)
                g_game->effectiveGameSpeed--;
        }
    } else {
        g_game->speedHysteresis--;
        if (g_game->speedHysteresis < -100) {
            g_game->speedHysteresis = 0;
            if (g_game->effectiveGameSpeed < g_game->speedCtrl)
                g_game->effectiveGameSpeed++;
        }
    }
}

// Runs one game update per pending time step (g_game->simStepsPending), profiling each
// phase into g_game->profileTimingBars.acc[]; the profile object also lives at +0x38d85 with
// total at +4, the display copy at +8 and the accumulators at +0x2c.

// FUNCTION: 0x495490
void __stdcall RunGameSteps(int showStats)
{
    int n = g_game->simStepsPending;

    while (n--) {
        g_game->gameTick++;

        if (showStats) {
            HandleNetPackets();
            g_game->profileTimingBars.AccumulateProfileTime(0);
        }
        UpdateAllUnits();
        g_game->profileTimingBars.AccumulateProfileTime(1);
        UpdateProjectiles();
        g_game->profileTimingBars.AccumulateProfileTime(7);
        UpdateExplosions();
        g_game->profileTimingBars.AccumulateProfileTime(8);
        UpdatePlayers();
        g_game->profileTimingBars.AccumulateProfileTime(2);

        UpdateFeatures();
        StepAllGafSequences();
        UpdateWind();
        UpdateMeteors();
        UpdateCameraFollow();
        g_game->profileTimingBars.AccumulateProfileTime(8);

        UpdateParticles();
        g_game->profileTimingBars.AccumulateProfileTime(6);
        UpdateBlink();
        g_game->profileTimingBars.AccumulateProfileTime(8);

        if (showStats && g_usePacketManager != 0) {
            UpdateResourceSharing(&g_game->players[g_game->localPlayer]);
            g_packetManager.SendAllQueued(0);
            g_game->profileTimingBars.AccumulateProfileTime(0);
        }
    }

    EmptyPostSimStepHook();
    EmptyPostSimStepHook_B();
    EmptyPostSimStepHook_C();
    ExpireOldestMessage();
    ExpireEyeballs();
    g_game->profileTimingBars.AccumulateProfileTime(8);
}

// Mission-script event dispatcher: the argument selects one of six cases out
// of 0x3d..0x70. The 0x3d walk keeps a byte counter in ebx next to a separate
// byte-offset induction variable in eax, and the redundant `i < 10` guard
// inside the do-while is what leaves the preheader test in the binary.

// FUNCTION: 0x4956c0
void __stdcall HandleDebugHotkey(int eventType)
{
    switch (eventType) {
    case 0x69:
        g_game->bit0_3923b = !g_game->bit0_3923b;
        break;
    case 0x6d:
        g_game->debugMode++;
        if (g_game->debugMode == 5)
            g_game->debugMode = 0;
        break;
    case 0x50:
        SetPageFlipping(1);
        break;
    case 0x70:
        SetPageFlipping(0);
        break;
    case 0x5d: {
        Unit* u = &g_game->units[g_game->hoverUnitId];
        if (u->unitDefIndex != 0) {
            u->lastAttackerSlot = 10;
            u->flag_110 = 1;
            u->attacker = 0;
        }
        break;
    }
    case 0x3d: {
        int k = 0;
        unsigned char i = 0;
        do {
            if (i < 10) {
                if (g_game->players[k].active != 0) {
                    unsigned char kind = g_game->players[k].type;
                    if (kind == 1 || kind == 2 || kind == 3) {
                        if (g_game->players[k].index != 10) {
                            g_game->players[k].energy = g_game->players[k].energyCapacity;
                            g_game->players[k].metal = g_game->players[k].metalCapacity;
                        }
                    }
                }
            }
            i++;
            k++;
        } while (i < 10);
        break;
    }
    default:
        break;
    }
}
