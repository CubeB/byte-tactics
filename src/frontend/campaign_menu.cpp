// Decompiled by Opus, Haiku, space-bunny-free, Space Bunny Free, Sonnet, deepseek-v4.1-flash, deepseek-v4.1, GPT-6, mimo-v2.6-pro, DeepSeek V4.1 Flash and longcat-2.5-preview-free. Names are provisional.
// The campaign menu module (0x476740 to 0x479620): the single-player and
// campaign front end. It builds the side and campaign name lists from the
// camps\*.TDF files, runs the SINGLE, NEWGAME and MSNBRIEF dialogs and the
// mission briefing page, animates the solar-system and taskbar gadgets, and
// keeps the player slots the skirmish setup uses. The module's files gathered
// in address order.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Included only for its symbol ids: DrawHelpPage matches only with it
// (docs/c2-regalloc.md).
#include <math.h>

#include "../util/tdf.h"

class Sound {
public:
    int IsStreamActive();
    void StopStream();
};

#include "../map/mission.h"

struct Info_00477510 {
    char unknown_0[0x30];
    char name[8];                      // +0x30
};

struct Object_00477510 {
    char unknown_0[0x18];
    Info_00477510* info;               // +0x18
};

// The frame sequencer at g_briefingPlanetFrameCursor.
struct Anim_00478b40 {
    short index;                       // +0x0
    char unknown_2[6];
};

#include "../graphics/gaf_frame.h"

#include "../graphics/rect.h"

struct Menu;
struct Layer;

#pragma pack(push, 1)

// One 0x15b-byte GUI record: a layout entry, a gadget, or the root of a
// dialog's gadget array. The views disagree about the bytes from +0xb6 on:
// 476ef0's text buffer and count, 478e80's callback at +0xb6 and gaf at
// +0xbe, 4779e0's selected line and text pointer, 478790's surface at +0xbc
// and gaf at +0xc0, 478240's callback at +0xce and stage index at +0x137. The
// anonymous union keeps every reading.
struct Gadget {                        // 0x15b bytes
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int attribs;                       // +0x1b
    int colour_1f;                     // +0x1f
    void* colours;                     // +0x23
    char unknown_27[0x28 - 0x27];
    unsigned char field_28;            // +0x28
    char unknown_29[0x60 - 0x29];
    int field_60;                      // +0x60
    char unknown_64[0xb6 - 0x64];
    union {                            // +0xb6
        short count;                   // 476ef0's text-gadget count
        char text_b6[0x80];            // 476ef0's text buffer
        void* field_b6;                // 478e80's per-gadget callback
        struct {
            char unknown_b6[4];
            short selected;            // +0xba, 4779e0's and 478240's
            union {                    // +0xbc
                struct {
                    void* surface;     // +0xbc, 478790's and 478b40's
                    void* gaf;         // +0xc0, 478790's and 478240's
                    char unknown_c4[2];
                };
                struct {
                    char unknown_bc[2];
                    void* field_be;    // +0xbe, 478e80's
                    char unknown_c2[4];
                };
                struct {
                    char unknown_c0[6];
                    char* text;        // +0xc2, 4779e0's and 478240's
                };
            };
            short frame;               // +0xc6, 478790's and 478e80's
            char unknown_c8[0xce - 0xc8];
            void (__stdcall* callback)();  // +0xce, 478240's
            char unknown_d2[0x136 - 0xd2];
            union {                    // +0x136
                short range;
                struct {
                    unsigned char stages;  // +0x136
                    unsigned char stageIndex;  // +0x137, 477410's and 478240's
                };
            };
            char unknown_138[0x144 - 0x138];
            void (__stdcall* sliderCallback)(Menu*, int); // +0x144
            char unknown_148[0x15b - 0x148];
        };
    };
};

// A dialog record: the root of a dialog's gadget array and what it carries.
struct Layer {
    int unknown_0;                     // +0x0
    Gadget* entries;                   // +0x4
    void (__stdcall* handler)(Menu*);  // +0x8
    void* data;                        // +0xc
    char unknown_10[0x3b - 0x10];
    void (__stdcall* textHandler)(Object_00477510*);  // +0x3b
};

// The GUI system object at g_game + 0x519.
struct Menu {
    char unknown_0[0x18];
    Layer* layer;                      // +0x18 (g_game + 0x531)
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
    char unknown_64[0x8b2 - 0x64];
    unsigned char colour;              // +0x8b2
    char unknown_8b3[0xcca - 0x8b3];
    int field_cca;                     // +0xcca
};

#include "../network/player_info.h"

struct PlayerEntry_004777a0 {          // 0x14b bytes
    PlayerInfo* info;                  // +0x0
    char unknown_4[0x14b - 4];
};

// A player slot, 0x18 bytes, as the skirmish setup's array at +0x29a0.
struct Item {
    union {
        int id;                        // +0x0, 0 = free slot
        int type;                      // CountComputerSlots'
        int flag;                      // AreAllSlotsEmpty's
        int active;                    // CountPlayersInAllyGroup's
    };
    int unknown_4;                     // +0x4
    int allyGroup;                     // +0x8, the ally group
    char unknown_c[0x14 - 0xc];
    int owner;                         // +0x14
};

struct Game {
    int unknown_0;
    char unknown_4[0x10 - 4];
    Sound* input;                      // +0x10
    char unknown_14[0x519 - 0x14];
    Menu menu;                         // +0x519
    char unknown_11e7[0x1b8a - 0x11e7];
    PlayerEntry_004777a0 players[10];  // +0x1b8a
    // MSVC 5 gives players[10] a total size 8 bytes larger than 10 * 0x14b,
    // so the pad below starts at 0x2878 and every later offset is right.
    char unknown_2878[0x29a0 - 0x2878];
    Item* items;                       // +0x29a0
    char unknown_29a4[0x2a42 - 0x29a4];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2bc0 - 0x2a43];
    unsigned char frontendSubstateRequest;  // +0x2bc0
    char unknown_2bc1[0x37e1b - 0x2bc1];
    int screen;                        // +0x37e1b
    char unknown_37e1f[0x37eee - 0x37e1f];
    int difficulty;                    // +0x37eee
    union {                            // +0x37ef2
        int flag_37ef2;
        unsigned char side;
    };
    char unknown_37ef6[0x37f39 - 0x37ef6];
    int count;                         // +0x37f39, the sides
    char names[1][0x232];              // +0x37f3d
    char unknown_3816f[0x38d7f - 0x3816f];
    unsigned short flags_38d7f;        // +0x38d7f
    int itemCount;                     // +0x38d81
    char unknown_38d85[0x391e9 - 0x38d85];
    Mission* net;                      // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int mode;                          // +0x391f1
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;
extern char DAT_005119b8[];            // ""
extern char* g_briefingWrappedText;
extern int g_briefingPageIndex;
extern int g_briefingPaginateReset;
extern int g_briefingWindSpeed;
extern char* g_campaignNameList;
extern char* g_missionNameList;
extern int g_anyMissionMode;
extern int g_briefingBaseGadgetCount;
extern int g_briefingWindTickCountdown;
extern short g_briefingPanoramaScrollX;
extern int g_briefingPanoramaNextTick;
extern unsigned int g_briefingPlanetLastTick;
extern unsigned int g_briefingPlanetNextTick;
extern Anim_00478b40 g_briefingPlanetFrameCursor;
extern unsigned char g_briefingTextColors[];
extern int g_campaignSimplifiedLayout;
extern char g_tdfExtension[];          // "TDF"
extern char DAT_0050372c[];            // "*"
extern char g_campsDirName[];          // "camps"
extern int DAT_00511de8;               // the same pointer as g_game

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall CountDirectoryEntries(const char* path, int flag);
void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);
int __stdcall ScanDirectory(char* path, void* buffer, char* p3, int p4, int p5, int p6);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall StreamSoundDelayed(char* text, int a, int b);
int __stdcall SetGadgetStatusByName(Menu* menu, const char* name, int value);
int __stdcall FindGadgetIndex(Gadget* entries, const char* name, int type);
Gadget* __stdcall FindGadgetChecked(Gadget* entries, const char* name);
Gadget* __stdcall FindGadgetOrNull(Gadget* entries, const char* name);
Gadget* __stdcall FindGadgetChecked_E(Gadget* entries, const char* name);
void __stdcall SelectFontForEntry(Gadget* entries, int index);
char* __stdcall WordWrapText(Menu* menu, char* text, int value, int index);
void DrawHelpPage();
char* __stdcall AllocColorMarkupText(char* text);
void InitBriefingText();
void __cdecl ApplyCampaignSideSelection();
void __stdcall UpdateSolarSystem(Menu* window, Gadget* item);
void __stdcall UpdatePlanet(Menu* window, Gadget* item);
void __stdcall HandleMissionBriefingClick(Menu* menu);
void __stdcall HandleNewGameClick(Menu* menu);
void __stdcall FillMissionList();
char* __stdcall Translate(const char* text);
void __stdcall AddTextGadget(Layer* dialog, const char* name, char* text, int x, int y,
                            int w, int flags);
void __stdcall SetTranslatedTextByName(Menu* menu, const char* name, char* text, int value);
void __stdcall SetGadgetColorByName(Menu* menu, const char* name, int value);
void __stdcall AddBlinkWord(Menu* menu, char* text, int x, int y, int count,
                            int colour, float a, float b);
void __stdcall ClearBlinkWords(Menu* menu);
void __stdcall SetBlinkGadget(Menu* menu, int value);
void __stdcall DrawBlinkWords(Menu* menu);
void __stdcall FreeBlinkWords(Menu* menu);
void* __cdecl GetFont();
int __stdcall FontHeight(void* font);
int __stdcall GetTextWidth(void* font, const char* text);
int GetTextKeyColor();
void __stdcall SetTextColors(int param_1, int param_2);
void __stdcall DrawString(void* surface, const char* text, int x, int y, int maxWidth);
void __stdcall FillRectangle(void* surface, Rect* rect, int colour);
void __stdcall DrawFrame(void* surface, void* frame, int x, int y);
unsigned int __cdecl GetTicks();
int __stdcall GetGafFrame(void* gaf, int frame);
void* __stdcall FindGafEntry(void* gaf, const char* name);
int __stdcall LoadScreenGaf(Menu* menu, char* name);
void __stdcall InitGafSequence(void* state, void* gaf, int param_3);
int __stdcall StepGafSequence(Anim_00478b40* anim);
int __stdcall GetButtonStageByName(Menu* menu, char* name);
int __stdcall SetButtonStageByName(Menu* menu, char* name, char value);
void __stdcall AllocBlinkWords(Menu* menu, int param_2);
void __stdcall SetGadgetActiveByName(Menu* menu, const char* name, int value);
void __stdcall SetQuickKeyByName(Menu* menu, const char* name, char value);
void __stdcall SyncAssocGadgets(Menu* menu, int index);
void __stdcall ConfigureListBoxByName(Menu* menu, const char* name, void* data, int count, int flag);
void __stdcall MarkChanged(Menu* menu);
void __stdcall MarkLayerChanged(Menu* menu);
void __stdcall SetKeyboardInput(Menu* menu, int value);
void __stdcall RenderLayer(Menu* menu, int value);
Layer* __stdcall LoadGuiLayer(Menu* menu, const char* name, int flags);
void __stdcall SelectGadgetByName(Menu* menu, const char* name);
void __stdcall RemapPaletteToClosestIndices(Menu* menu, void* param_2, void* param_3);
void __stdcall PlaySoundByName(const char* name, int value);
char __stdcall FindGameCdDrive(int side);
void RegisterDataArchives();
void InitMissionStatus();
void SaveSettings();
void SaveAllMissionsSetting();
void ShowLoadGameScreen();
void OpenOptionsPanel();
void BlankScreen();
void HideSoftwareCursor();
void ShowSoftwareCursor();
void* __stdcall LoadBitmapByName(char* name, unsigned char* palette);
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall SetOffscreenSurface(int param_1);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);
void __stdcall FreeSurface(void* param_1);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall ClearSelectedGadget(void* menu);
void __stdcall SetCursorMode(int n);
int __stdcall IsCurrentGadgetNamed(Menu* menu, char* name);
int __stdcall BuildCampaignNameList(char** out, int side);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall SetMissionType(int owner);
int GetPreferredLanguage();

// FUNCTION: 0x476740
void __stdcall DrawBitmapBackground(char* name, int lock)
{
    char path[0x100];
    unsigned char palette[0x400];
    void* image;

    if (lock) {
        HideSoftwareCursor();
        BlankScreen();
    }
    BuildDataPath(path, "bitmaps", name, "PCX");
    image = LoadBitmapByName(name, palette);
    SetPaletteColors(palette, 0, 0x100);
    SetOffscreenSurface(g_game->screen);
    DrawSurface(0, image, 0, 0);
    FreeSurface(image);
    if (lock) {
        ShowSoftwareCursor();
    }
}

// FUNCTION: 0x4767e0
int GetSideCount(void)
{
    return g_game->count;
}

// Counts the campaign files (camps\*.TDF); compare 0x4769f0.
// FUNCTION: 0x4767f0
void CountCampaignFiles()
{
    char path[0x100];
    BuildDataPath(path, "camps", "*", "TDF");
    CountDirectoryEntries(path, 0);
}

// FUNCTION: 0x476830
char* BuildSideList()
{
    char* buf = (char*)GameAllocIgnoreTag("SideList", g_game->count * 30);
    char* p = buf;
    int i;
    int j;

    buf[0] = 0;
    for (i = 0; i < g_game->count; i++) {
        p = strcat(p, g_game->names[i]);
        p = p + strlen(p) + 1;
        p[0] = 0;
    }
    p = buf;
    for (j = 0; j < g_game->count; j++) {
        for (;;) {
            char c = *++p;
            if (c == 0) {
                break;
            }
            p[0] = c + 0x20;
        }
        p++;
    }
    return buf;
}

// FUNCTION: 0x476920
int __stdcall CampaignExists(const char* name)
{
    int found;
    char path[256];
    BuildDataPath(path, "camps", "*", "TDF");
    int count = CountDirectoryEntries(path, 0);
    char* names = (char*)GameAllocIgnoreTag("CAMPAIGN NAMES", count << 8);
    ScanDirectory(path, names, 0, 0, 1, 2);
    found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(name, SkipTextLines(names, i)) == 0) {
            found = 1;
        }
    }
    GameFreeThunk(names);
    return found;
}

// Lists the campaign files (camps\*.TDF) into a buffer of 256-byte names and
// returns how many there are.
// FUNCTION: 0x4769f0
int __stdcall ListCampaignFiles(void** names)
{
    char path[0x100];
    BuildDataPath(path, "camps", "*", "TDF");
    int count = CountDirectoryEntries(path, 0);
    void* buffer = GameAllocIgnoreTag("CAMPAIGN NAMES", count << 8);
    *names = buffer;
    ScanDirectory(path, buffer, 0, 0, 1, 2);
    return count;
}

// Copies one name onto the end of the result list and steps the cursor past it.
static inline char* AppendName_00476a60(char* p, char* s)
{
    strcpy(p, s);
    return p + strlen(p) + 1;
}

// FUNCTION: 0x476a60
int __stdcall BuildCampaignNameList(char** out, int side)
{
    int found = 0;
    TdfFile parser;
    char name[0x40];
    char path[0x100];
    name[0] = '0';
    BuildDataPath(path, "camps", "*", "TDF");
    int n = CountDirectoryEntries(path, 0);
    char* names = (char*)GameAllocIgnoreTag("CAMPAIGN NAMES1", n << 8);
    *out = (char*)GameAllocIgnoreTag("CAMPAIGN NAMES2", n << 8);
    ScanDirectory(path, names, 0, 0, 1, 2);
    // q declared before p, and the append goes through the inline helper.
    char* q = names;
    char* p = *out;
    for (int i = 0; i < n; i++) {
        BuildDataPath(path, "camps", q, "tdf");
        if ((&parser)->LoadFile(path)) {
            if ((&parser)->SelectRecord("HEADER")) {
                parser.current->GetFieldString(name, "campaignside", 0x40, DAT_005119b8);
                if (strcmp(g_game->names[side], name) == 0 || strcmp("ALL", name) == 0) {
                    found++;
                    p = AppendName_00476a60(p, q);
                }
            }
            q += strlen(q) + 1;
        }
    }
    GameFreeThunk(names);
    return found;
}

// FUNCTION: 0x476c70
void StartBriefingNarration()
{
    if (g_game->mode != 6) {
        char* name = (char*)g_game->net->GetNameSlot(3);
        if (name) {
            StreamSoundDelayed(name, 0, 0x3c);
        }
    }
}

// FUNCTION: 0x476ca0
void StartGlamourSound()
{
    if (g_game->mode != 6) {
        char* name = (char*)g_game->net->GetNameSlot(8);
        if (name) {
            StreamSoundDelayed(name, 0, 0x3c);
        }
    }
}

// FUNCTION: 0x476cd0
char* __stdcall AllocColorMarkupText(char* s)
{
    char* buf = (char*)GameAllocIgnoreTag("TempStorage", strlen(s) + 0x32);
    int inquote = 0;
    memset(buf, 0, strlen(s) + 0x32);
    unsigned char* p = (unsigned char*)s;
    if (*p) {
        char* d = buf;
        while (1) {
            char quote;
            if (*p == 0xff)
                break;
            *d = *p;
            if (*p == '&') {
                quote = p[1];
                inquote ^= 1;
            }
            if (*p == '\n' && inquote == 1) {
                d[-1] = '&';
                *d++ = '\r';
                *d++ = '\n';
                *d++ = '&';
                *d = quote;
            }
            d++;
            p++;
            if (*p == 0)
                break;
        }
    }
    GameFreeThunk(s);
    return buf;
}

// FUNCTION: 0x476d80
void InitBriefingText()
{
    char* text = g_game->net->GetBriefing();
    if (text) {
        Gadget* gadgets = g_game->menu.layer->entries;
        int i = FindGadgetIndex(gadgets, "SOLARSYSTEM", 0xe);
        if (i != -1) {
            gadgets[i].field_28 = g_game->side + 1;
        }
        int j = FindGadgetIndex(gadgets, "TextRegion", 0xe);
        gadgets[j].field_28 = g_game->side + 1;
        SelectFontForEntry(gadgets, j);
        g_briefingPaginateReset = 1;
        g_briefingWrappedText = WordWrapText(&g_game->menu, text, gadgets[j].w, j);
        g_briefingWrappedText = AllocColorMarkupText(g_briefingWrappedText);
        DrawHelpPage();
    }
    if (g_game->mode != 6) {
        char* name = (char*)g_game->net->GetNameSlot(3);
        if (name) {
            StreamSoundDelayed(name, 0, 0x3c);
        }
    }
}

// FUNCTION: 0x476e90
char* __stdcall SkipToPageOffset(char* param_1, int param_2, int param_3)
{
    if (param_3 == 0) {
        return param_1;
    }

    char* p = param_1;
    int found = 0;
    int count = 0;

    while (*p != 0) {
        char c = *p;
        if (c == (char)0xff) break;
        if (found) break;
        p++;
        if (c == '\n') {
            count++;
            if (count == param_3 * param_2) {
                found = 1;
            }
        }
    }

    return found ? p : 0;
}

// Inlined twice into DrawHelpPage; two pointers walk in step, found and
// count declared first.
static inline char* FindPageStart(char* start, int lines, int page)
{
    if (!page) return start;
    int found = 0;
    int count = 0;
    char* p = start;
    char* q = start;
    while (*p) {
        char c = *q;
        if (c == (char)0xff) break;
        if (found) break;
        ++p;
        ++q;
        if (c == '\n') {
            ++count;
            if (count == page * lines) found = 1;
        }
    }
    return found ? q : 0;
}

// FUNCTION: 0x476ef0
void DrawHelpPage()
{
    Layer* dialog = g_game->menu.layer;
    Gadget* gadgets = dialog->entries;
    int count;
    int colourState = 1;
    if (g_briefingWrappedText == 0)
        return;

    if (g_briefingPaginateReset != 0) {
        g_briefingBaseGadgetCount = gadgets[0].count;
        g_briefingPageIndex = -1;
        g_briefingPaginateReset = 0;
    } else {
        gadgets[0].count = (short)g_briefingBaseGadgetCount;
    }

    ClearBlinkWords(&g_game->menu);
    SetBlinkGadget(&g_game->menu,
                 FindGadgetIndex(gadgets, "TextRegion", 5));
    int idx = FindGadgetIndex(gadgets, "TextRegion", 0xe);
    Gadget* gp = &gadgets[idx];
    gp->field_28 = g_game->flag_37ef2 + 1;
    SelectFontForEntry(gadgets, idx);

    int divisor = FontHeight(GetFont()) + 2;
    int linesPerPage = gp->h / divisor;
    int textX = gp->x + 5;
    int y = gp->y + divisor / 2;
    count = gadgets[0].count;
    g_briefingPageIndex++;

    char* lineStart = FindPageStart(g_briefingWrappedText, linesPerPage, g_briefingPageIndex);
    if (!lineStart) {
        g_briefingPageIndex = 0;
        lineStart = g_briefingWrappedText;
    }
    char* nextPage = FindPageStart(g_briefingWrappedText, linesPerPage, g_briefingPageIndex + 1);

    if (nextPage == 0) {
        if (g_briefingPageIndex == 0)
            SetTranslatedTextByName(&g_game->menu, "MOREBAR", DAT_005119b8, 0);
        else
            SetTranslatedTextByName(&g_game->menu, "MOREBAR",
                         Translate("BACK TO START"), 0);
    } else {
        SetTranslatedTextByName(&g_game->menu, "MOREBAR", Translate("MORE..."), 0);
    }
    SetGadgetColorByName(&g_game->menu, "MOREBAR",
                 g_briefingTextColors[g_game->flag_37ef2 * 4 + 1]);

    char buf[0x80];

    for (int i = linesPerPage * g_briefingPageIndex;
         i < (g_briefingPageIndex + 1) * linesPerPage; i++) {
        AddTextGadget(dialog, "TextRegion", DAT_005119b8, textX, y, -1, 2);
        // The gadget index is count itself, incremented here.
        count++;
        char* dst = gadgets[count].text_b6;
        gadgets[count].attribs = 0x411;
        gadgets[count].field_28 = gp->field_28;
        gadgets[count].colour_1f = g_briefingTextColors[g_game->flag_37ef2 * 4];
        memset(gadgets[count].text_b6, 0, 0x80);

        y += divisor;
        while (*lineStart != '\n') {
            char c = *lineStart;
            if (c == 0)
                break;
            if (c == (char)0xff)
                break;
            if (c == '&') {
                if (colourState != 0) {
                    // Increment then read, not lineStart[1]: fixes the pointer registers.
                    lineStart++;
                    char code = *lineStart;
                    colourState = 0;
                    int sel = code == 'R' ? 3
                            : code == 'Y' ? 2
                            : code == 'G' ? 1 : 3;
                    lineStart++;
                    // Own local, not the gadget count: it shares count's slot once count is dead.
                    int colour =
                        g_briefingTextColors[g_game->flag_37ef2 * 4 + sel];
                    int x = GetTextWidth(GetFont(), gadgets[count].text_b6) + textX;
                    int ey = gadgets[count].y;
                    int k = 0;
                    char* q = lineStart;
                    while (k < 0x7f) {
                        if (*q == '&')
                            break;
                        buf[k] = *q;
                        k++;
                        q++;
                    }
                    buf[k] = 0;
                    AddBlinkWord(&g_game->menu, buf, x, ey, colour, 0x5e,
                                 1.0f, 0.25f);
                } else if (*lineStart == '&') {
                    colourState = 1;
                    lineStart++;
                }
            }
            *dst++ = *lineStart++;
        }
        lineStart++;
    }
}

// The original calls this out of line: with the definition visible
// the compiler inlines it into OpenNewGameMenu (0x478240).
#pragma auto_inline(off)

// FUNCTION: 0x477360
void __cdecl ApplyCampaignSideSelection()
{
    if (g_game->flag_37ef2 == 0) {
        g_game->players[0].info->side = 0;
        g_game->players[1].info->side = 1;
        SetGadgetStatusByName(&g_game->menu, "Arm", 1);
        SetGadgetStatusByName(&g_game->menu, "Side0", 1);
    } else {
        g_game->players[0].info->side = 1;
        g_game->players[1].info->side = 0;
        SetGadgetStatusByName(&g_game->menu, "Core", 1);
        SetGadgetStatusByName(&g_game->menu, "Side1", 1);
    }
}

#pragma auto_inline(on)

// FUNCTION: 0x477410
void ApplyDifficultyButtons()
{
    Gadget* gadget = FindGadgetOrNull(g_game->menu.layer->entries, "Difficulty");
    if (g_game->difficulty == 0) {
        gadget->stageIndex = 0;
        SetGadgetStatusByName(&g_game->menu, "Easy", 1);
    }
    if (g_game->difficulty == 1) {
        gadget->stageIndex = 1;
        SetGadgetStatusByName(&g_game->menu, "Medium", 1);
    }
    if (g_game->difficulty == 2) {
        gadget->stageIndex = 2;
        SetGadgetStatusByName(&g_game->menu, "Hard", 1);
    }
    MarkChanged(&g_game->menu);
}

// FUNCTION: 0x4774d0
int HasFewerThanThreeCampaignFiles(void)
{
    char local_100[256];
    BuildDataPath(local_100, g_campsDirName, DAT_0050372c, g_tdfExtension);
    int n = CountDirectoryEntries(local_100, 0);
    return n <= 2;
}

// FUNCTION: 0x477510
void __stdcall ToggleAnyMission(Object_00477510* obj)
{
    if (strncmp(obj->info->name, "DRDEATH", 7) == 0) {
        if (!(g_game->flags_38d7f & 1)) {
            SetGadgetActiveByName(&g_game->menu, "AnyMsn", 1);
            g_game->flags_38d7f |= 1;
        } else {
            SetGadgetActiveByName(&g_game->menu, "AnyMsn", 0);
            g_game->flags_38d7f &= ~1;
        }
        SaveAllMissionsSetting();
        MarkChanged(&g_game->menu);
    }
}

// FUNCTION: 0x4775a0
void __stdcall HandleSingleMenuClick(Menu* gadget)
{
    if (gadget->field_60 == -1)
        return;
    if (IsCurrentGadgetNamed(gadget, "NewCamp")) {
        if (FindGameCdDrive(0)) {
            RegisterDataArchives();
            PlaySoundByName("BigButton", 0);
            g_game->frontendSubstateRequest = 10;
            SetCursorMode(0x14);
        } else {
            OpenMessageBox((char*)&g_game->menu, Translate("Please insert the Campaign CD (Disc 2) and try again"), 200, 1, 1);
            ClearSelectedGadget((char*)&g_game->menu);
        }
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "Skirmish")) {
        if (FindGameCdDrive(1)) {
            RegisterDataArchives();
            PlaySoundByName("skirmish", 0);
            g_game->frontendSubstateRequest = 11;
            SetCursorMode(0x14);
        } else {
            OpenMessageBox((char*)&g_game->menu, Translate("Please insert the Multiplayer CD (Disc 1) and try again"), 200, 1, 1);
            ClearSelectedGadget((char*)&g_game->menu);
        }
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "LoadGame")) {
        PlaySoundByName("BigButton", 0);
        SetCursorMode(0x14);
        ShowLoadGameScreen();
        ClearSelectedGadget(gadget);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "Options")) {
        PlaySoundByName("options", 0);
        ClearSelectedGadget(gadget);
        SetCursorMode(0x14);
        OpenOptionsPanel();
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "PrevMenu")) {
        PlaySoundByName("Previous", 0);
        SetCursorMode(0x14);
        g_game->frontendSubstateRequest = 3;
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "AnyMsn")) {
        if (FindGameCdDrive(0)) {
            RegisterDataArchives();
            PlaySoundByName("bigButton", 0);
            g_game->frontendSubstateRequest = 14;
            SetCursorMode(0x14);
        } else {
            OpenMessageBox((char*)&g_game->menu, Translate("Please insert the Campaign CD (Disc 2) and try again"), 200, 1, 1);
            ClearSelectedGadget((char*)&g_game->menu);
        }
        return;
    }
    ClearSelectedGadget(gadget);
}

// FUNCTION: 0x4777a0
void OpenSingleMenu()
{
    Layer* gui = LoadGuiLayer(&g_game->menu, "SINGLE.GUI", 0);
    gui->handler = HandleSingleMenuClick;
    gui->data = g_game;
    BlankScreen();
    LoadPictureCached("singlebg", 0, 0, 0);
    switch (g_game->flag_37ef2) {
    case 0:
        g_game->players[g_game->localPlayer].info->side = 0;
        g_game->players[g_game->localPlayer + 1].info->side = 1;
        break;
    case 1:
        g_game->players[g_game->localPlayer].info->side = 1;
        g_game->players[g_game->localPlayer + 1].info->side = 0;
        break;
    }
    SetMissionType(1);
    if (g_game->flags_38d7f & 1) {
        SetGadgetActiveByName(&g_game->menu, "AnyMsn", 1);
        // Original oddity, kept: the test guards an |= of the same bit.
        g_game->flags_38d7f |= 1;
    }
    g_game->menu.layer->textHandler = ToggleAnyMission;
    if (GetPreferredLanguage() && _strcmpi((char*)GetPreferredLanguage(), "spanish") == 0) {
        SetQuickKeyByName(&g_game->menu, "Skirmish", 0x73);
    }
    SetKeyboardInput(&g_game->menu, 1);
    SetCursorMode(0x13);
    RenderLayer(&g_game->menu, 0x40);
}

// FUNCTION: 0x477940
void __stdcall FillCampaignList(char* param_1)
{
    Layer* layer = g_game->menu.layer;
    if (g_campaignNameList != 0) {
        GameFreeThunk(g_campaignNameList);
        g_campaignNameList = 0;
    }
    PlaySoundByName("smlbutton", 0);
    int count = BuildCampaignNameList(&g_campaignNameList, (int)param_1);
    ConfigureListBoxByName(&g_game->menu, "Campaign",
                 g_campaignNameList, count, 0);
    int index = FindGadgetIndex(layer->entries,
                             "Campaign", 2);
    SyncAssocGadgets(&g_game->menu, index);
    MarkChanged(&g_game->menu);
}

// FUNCTION: 0x4779e0
void __stdcall FillMissionList(Menu* menu, int unused)
{
    Layer* layer = menu->layer;
    if (g_missionNameList != 0) {
        GameFreeThunk((int*)g_missionNameList);
        g_missionNameList = 0;
    }
    Gadget* layout =
        FindGadgetChecked(g_game->menu.layer->entries, "Campaign");
    g_game->net->LoadCampaign(SkipTextLines(layout->text, layout->selected));
    int count = g_game->net->BuildMissionList(&g_missionNameList);
    ConfigureListBoxByName(menu, "Missions", g_missionNameList, count, 0);
    SyncAssocGadgets(&g_game->menu,
                 FindGadgetIndex(layer->entries, "Missions", 2));
    MarkChanged(&g_game->menu);
}

// Inlined copies of FillCampaignList (0x477940) and FillMissionList (0x4779e0):
// the compiler inlined these bodies into HandleNewGameClick, and the copies'
// argument shapes are what the inlined code needs.
static inline void FillCampaignListInline(int side)
{
    Layer* layer = g_game->menu.layer;
    if (g_campaignNameList != 0) {
        GameFreeThunk(g_campaignNameList);
        g_campaignNameList = 0;
    }
    PlaySoundByName("smlbutton", 0);
    int count = BuildCampaignNameList(&g_campaignNameList, side);
    ConfigureListBoxByName(&g_game->menu, "Campaign", g_campaignNameList, count, 0);
    int index = FindGadgetIndex(layer->entries, "Campaign", 2);
    SyncAssocGadgets(&g_game->menu, index);
    MarkChanged(&g_game->menu);
}

static inline void FillMissionListInline(Menu* menu, Gadget* unused)
{
    Layer* layer = menu->layer;
    if (g_missionNameList != 0) {
        GameFreeThunk(g_missionNameList);
        g_missionNameList = 0;
    }
    Gadget* layout =
        FindGadgetChecked(g_game->menu.layer->entries, "Campaign");
    g_game->net->LoadCampaign(
        SkipTextLines(layout->text, layout->selected));
    int count = g_game->net->BuildMissionList(&g_missionNameList);
    ConfigureListBoxByName(menu, "Missions", g_missionNameList, count, 0);
    SyncAssocGadgets(&g_game->menu,
                 FindGadgetIndex(layer->entries, "Missions", 2));
    MarkChanged(&g_game->menu);
}

// FUNCTION: 0x477ab0
void __stdcall HandleNewGameClick(Menu* menu)
{
    Gadget* entries = menu->layer->entries;
    char* playerInfo = (char*)g_game + 0x14b * g_game->localPlayer;
    int index;

    if (menu->field_60 == -1) {
        GameFreeThunk(g_campaignNameList);
        GameFreeThunk(g_missionNameList);
        g_campaignNameList = 0;
        g_missionNameList = 0;
        return;
    }

    if ((g_anyMissionMode != 0 && (IsCurrentGadgetNamed(menu, "Missions") || IsCurrentGadgetNamed(menu, "Start"))) ||
        (g_anyMissionMode == 0 && (IsCurrentGadgetNamed(menu, "Campaign") || IsCurrentGadgetNamed(menu, "Start")))) {
        index = 0;
        PlaySoundByName("bigButton", 0);
        if (!FindGameCdDrive(0)) {
            OpenMessageBox((char*)&g_game->menu,
                         Translate("Please insert the Campaign CD (Disc 2) and try again"),
                         200, 1, 1);
            ClearSelectedGadget(&g_game->menu);
            return;
        }
        RegisterDataArchives();
        InitMissionStatus();
        {
            char* name;
            if (g_campaignSimplifiedLayout == 0) {
                Gadget* e = FindGadgetChecked(entries, "Campaign");
                name = SkipTextLines(e->text, e->selected);
                g_game->net->LoadCampaign(name);
            } else if (*(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) == 0) {
                g_game->net->LoadCampaign("Arm Campaign");
            } else {
                g_game->net->LoadCampaign("Core Campaign");
            }
        }
        if (g_anyMissionMode != 0) {
            Gadget* e = FindGadgetChecked(entries, "Missions");
            index = e->selected;
        }
        if (g_game->net->SelectMission(index) != 0) {
            SetCursorMode(0x14);
            *(unsigned char*)(*(int*)((char*)g_game + 0x1b8a) + 0x96) = 0;
            *(unsigned char*)(*(int*)((char*)g_game + 0x1cd5) + 0x96) = 1;
            SaveSettings();
            if (g_anyMissionMode != 0) {
                *(unsigned char*)((char*)g_game + 0x2bc0) = 0x10;
                return;
            }
            *(unsigned char*)((char*)g_game + 0x2bc0) = 0x0f;
            return;
        }
        goto End;
    } else {
        if (IsCurrentGadgetNamed(menu, "PrevMenu")) {
            PlaySoundByName("Previous", 0);
            *(unsigned char*)((char*)g_game + 0x2bc0) = 3;
            SetCursorMode(0x14);
            return;
        }
        if (IsCurrentGadgetNamed(menu, "Difficulty")) {
            PlaySoundByName("SmlButton", 0);
            // If/else-if chain into one shared exit: a switch or per-arm return changes registers.
            int diff = *(int*)((char*)g_game + 0x37eee);
            if (diff == 0) {
                *(int*)((char*)g_game + 0x37eee) = 1;
            } else if (diff == 1) {
                *(int*)((char*)g_game + 0x37eee) = 2;
            } else if (diff == 2) {
                *(int*)((char*)g_game + 0x37eee) = 0;
            } else {
                goto End;
            }
            ClearSelectedGadget(menu);
            return;
        }
        if (IsCurrentGadgetNamed(menu, "Side0") || IsCurrentGadgetNamed(menu, "Arm"))
            goto ArmSide;
        if (!IsCurrentGadgetNamed(menu, "Side1") && !IsCurrentGadgetNamed(menu, "Core"))
            goto End;

        SetGadgetStatusByName(&g_game->menu, "Core", 1);
        SetGadgetStatusByName(&g_game->menu, "Side1", 1);
        PlaySoundByName("SideSelect2", 0);
        index = FindGadgetIndex(entries, "Side1", 1);
        *(int*)((char*)entries + index * 0x15b + 0x1f) = 0x1f;
        *(int*)((char*)g_game + 0x37ef2) = 1;
        *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) = 1;
        *(unsigned char*)(*(int*)(playerInfo + 0x1cd5) + 0x95) = 0;
        if (g_campaignSimplifiedLayout == 0)
            FillCampaignListInline(*(unsigned char*)(*(int*)((char*)g_game + 0x14b * g_game->localPlayer + 0x1b8a) + 0x95));
        if (g_anyMissionMode != 0) {
            FillMissionListInline(&g_game->menu, FindGadgetChecked(entries, "Campaign"));
            ClearSelectedGadget(menu);
            return;
        }
        goto End;

ArmSide:
        SetGadgetStatusByName(&g_game->menu, "Arm", 1);
        SetGadgetStatusByName(&g_game->menu, "Side0", 1);
        PlaySoundByName("SideSelect", 0);
        index = FindGadgetIndex(entries, "Side0", 1);
        *(int*)((char*)entries + index * 0x15b + 0x1f) = 0x1f;
        *(int*)((char*)g_game + 0x37ef2) = 0;
        *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) = 0;
        *(unsigned char*)(*(int*)(playerInfo + 0x1cd5) + 0x95) = 1;
        FillCampaignListInline(*(unsigned char*)(*(int*)((char*)g_game + 0x14b * g_game->localPlayer + 0x1b8a) + 0x95));
        SetGadgetActiveByName(menu, "Campaign", g_campaignSimplifiedLayout == 0);
        if (g_anyMissionMode != 0)
            FillMissionListInline(&g_game->menu, FindGadgetChecked(entries, "Campaign"));

    }
End:
    ClearSelectedGadget(menu);
}

// FUNCTION: 0x478240
void __stdcall OpenNewGameMenu(int param_1)
{
    char buf[256];

    BlankScreen();
    g_anyMissionMode = param_1;
    Layer* layer =
        LoadGuiLayer(&g_game->menu, "NEWGAME.GUI", 0x400);
    Gadget* entries = layer->entries;
    layer->handler = HandleNewGameClick;
    layer->data = g_game;

    if (param_1 != 0) {
        LoadPictureCached("playanygame4", 0, 0, 0);
        g_campaignSimplifiedLayout = 0;
    } else {
        BuildDataPath(buf, "camps", "*", "TDF");
        if (CountDirectoryEntries(buf, 0) <= 2) {
            LoadPictureCached("newcampaign4x", 0, 0, 0);
            g_campaignSimplifiedLayout = 1;
        } else {
            LoadPictureCached("newcampaign4", 0, 0, 0);
            g_campaignSimplifiedLayout = 0;
        }
    }

    if (param_1 != 0) {
        int i = FindGadgetIndex(entries, "Campaign", 2);
        if (i != -1) {
            Gadget* e = (Gadget*)((char*)entries + i * 0x15b);
            e->y = 0x134;
            e->h = 0x30;
        }
        i = FindGadgetIndex(entries, "CampaignKnob", 4);
        if (i != -1) {
            Gadget* e = (Gadget*)((char*)entries + i * 0x15b);
            e->y = 0x134;
            e->h = 0x30;
        }
        i = FindGadgetIndex(entries, "Missions", 2);
        if (i != -1)
            ((Gadget*)((char*)entries + i * 0x15b))->h = 0x3e;
    }

    HideSoftwareCursor();
    RenderLayer(&g_game->menu, 1);
    ShowSoftwareCursor();

    unsigned short* p =
        (unsigned short*)FindGafEntry(entries->gaf, "Side0");
    int n = 0;
    while (n < *p) {
        char* r = (char*)GetGafFrame(p, 0);
        *(short*)(r + 6) = 0;
        *(short*)(r + 4) = 0;
        n++;
    }
    p = (unsigned short*)FindGafEntry(entries->gaf, "Side1");
    n = 0;
    while (n < *p) {
        char* r = (char*)GetGafFrame(p, 0);
        *(short*)(r + 6) = 0;
        *(short*)(r + 4) = 0;
        n++;
    }

    ApplyCampaignSideSelection();

    Gadget* diff =
        FindGadgetOrNull(g_game->menu.layer->entries, "Difficulty");
    if (g_game->difficulty == 0) {
        diff->stageIndex = 0;
        SetGadgetStatusByName(&g_game->menu, "Easy", 1);
    }
    if (g_game->difficulty == 1) {
        diff->stageIndex = 1;
        SetGadgetStatusByName(&g_game->menu, "Medium", 1);
    }
    if (g_game->difficulty == 2) {
        diff->stageIndex = 2;
        SetGadgetStatusByName(&g_game->menu, "Hard", 1);
    }

    MarkChanged(&g_game->menu);

    if (g_campaignSimplifiedLayout == 0 || param_1 != 0) {
        SetGadgetActiveByName(&g_game->menu, "Campaign", 1);
        SetGadgetActiveByName(&g_game->menu, "CampaignKnob", 1);
        Gadget* c = FindGadgetChecked(entries, "Campaign");
        // Original bug, kept: c is dereferenced on both branches even if null.
        if (c != 0 && g_anyMissionMode != 0)
            c->callback = FillMissionList;
        else
            c->callback = 0;

        int side = g_game->players[g_game->localPlayer].info->side;
        Layer* cur = g_game->menu.layer;
        if (g_campaignNameList != 0) {
            GameFreeThunk(g_campaignNameList);
            g_campaignNameList = 0;
        }
        PlaySoundByName("smlbutton", 0);
        int count = BuildCampaignNameList(&g_campaignNameList, side);
        ConfigureListBoxByName(&g_game->menu, "Campaign", g_campaignNameList, count, 0);
        int ci = FindGadgetIndex(cur->entries, "Campaign", 2);
        SyncAssocGadgets(&g_game->menu, ci);
        MarkChanged(&g_game->menu);

        if (param_1 != 0) {
            SetGadgetActiveByName(&g_game->menu, "Missions", 1);
            SetGadgetActiveByName(&g_game->menu, "MissionsKnob", 1);
            FindGadgetChecked(entries, "Missions");

            Menu* menu = &g_game->menu;
            Layer* mlayer = menu->layer;
            if (g_missionNameList != 0) {
                GameFreeThunk(g_missionNameList);
                g_missionNameList = 0;
            }
            Gadget* m = FindGadgetChecked(g_game->menu.layer->entries,
                                             "Campaign");
            char* text = SkipTextLines(m->text, m->selected);
            g_game->net->LoadCampaign(text);
            int mc = g_game->net->BuildMissionList(
                &g_missionNameList);
            ConfigureListBoxByName(menu, "Missions", g_missionNameList, mc, 0);
            int mi = FindGadgetIndex(mlayer->entries, "Missions", 2);
            SyncAssocGadgets(&g_game->menu, mi);
            MarkChanged(&g_game->menu);
        }
    }

    if (param_1 != 0)
        SelectGadgetByName(&g_game->menu, "Missions");
    else if (g_campaignSimplifiedLayout != 0)
        SelectGadgetByName(&g_game->menu, "Difficulty");
    else
        SelectGadgetByName(&g_game->menu, "Campaign");

    SetKeyboardInput(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    SetCursorMode(0x13);
}

// FUNCTION: 0x478790
void __stdcall UpdateSolarSystem(Menu* arg1, Gadget* arg2)
{
    int windMin = g_game->net->minWindSpeed;
    int windMax = g_game->net->maxWindSpeed;
    void* surface = arg1->layer->entries->surface;

    if (--g_briefingWindTickCountdown <= 0) {
        g_briefingWindSpeed += rand() % 5 - 2;
        if (g_briefingWindSpeed < windMin)
            g_briefingWindSpeed = windMin;
        if (g_briefingWindSpeed > windMax)
            g_briefingWindSpeed = windMax;
        g_briefingWindTickCountdown = rand() % 0x3f;
    }

    int i = FindGadgetIndex(arg1->layer->entries, "SOLARSYSTEM", 0xe);
    Gadget* g = FindGadgetChecked_E(arg1->layer->entries, "SOLARSYSTEM");

    Rect rect;
    rect.left = g->x;
    rect.top = g->y;
    rect.right = rect.left + g->w - 1;
    rect.bottom = rect.top + g->h - 1;

    FillRectangle(surface, &rect, arg1->colour);
    SelectFontForEntry(arg1->layer->entries, i);

    char text[0x34];
    sprintf(text, "%s : %d", Translate("Wind Speed"), g_briefingWindSpeed);
    SetTextColors(g_briefingTextColors[g_game->flag_37ef2 * 4], GetTextKeyColor());
    DrawString(surface, text, rect.left + 0x50, rect.top + 0x14,
                 rect.right - rect.left - 0x50);

    sprintf(text, "%s : %.1f", Translate("Gravity"),
            (double)g_game->net->gravity * 0.008928571428571428);
    DrawString(surface, text, rect.left + 0x50, rect.top + 0x28,
                 rect.right - rect.left - 0x50);

    DrawBlinkWords(&g_game->menu);

    if (arg2->field_be != 0) {
        int total = 0;
        for (int j = 0; j < *(unsigned short*)arg2->field_be; j++)
            total += ((GafFrame*)GetGafFrame((unsigned short*)arg2->field_be, j))->width;

        if (g_briefingPanoramaNextTick < (int)GetTicks()) {
            g_briefingPanoramaScrollX++;
            if ((int)g_briefingPanoramaScrollX >= total)
                g_briefingPanoramaScrollX = 0;
            g_briefingPanoramaNextTick = GetTicks() + 2;
        }

        MarkLayerChanged(arg1);

        Rect rect2;
        rect2.left = arg2->x;
        rect2.top = arg2->y;
        rect2.right = arg2->x + arg2->w - 1;
        rect2.bottom = arg2->y + arg2->h - 1;
        void* gaf = arg2->field_be;
        surface = arg1->layer->entries->surface;

        int now = (int)GetTicks();
        int idx = now / 3 % *(unsigned short*)gaf;
        arg2->frame = (short)idx;
        GafFrame* f = (GafFrame*)GetGafFrame((unsigned short*)gaf, arg2->frame);
        if (f == 0)
            return;

        // Original quirk, kept: indexed by the window pointer, not a palette slot.
        unsigned char colour =
            ((unsigned char*)arg2->colours)[(int)arg1 + 0x8b2];
        FillRectangle(surface, &rect2, colour);

        int x = arg2->x - g_briefingPanoramaScrollX;
        int y = arg2->y;
        int n = *(unsigned short*)arg2->field_be;
        // Original quirk, kept: `<=` runs one frame past the count.
        for (int k = 0; k <= n; k++) {
            GafFrame* fr =
                (GafFrame*)GetGafFrame((unsigned short*)arg2->field_be, k % n);
            fr->xOffset = 0;
            fr->yOffset = 0;
            DrawFrame(surface, fr, x, y);
            x += fr->width;
            n = *(unsigned short*)arg2->field_be;
        }

        void* panGaf = arg1->layer->entries->gaf;
        void* pan = FindGafEntry(panGaf, "Panmask");
        PlayerInfo* info =
            g_game->players[g_game->localPlayer].info;
        GafFrame* pf = (GafFrame*)GetGafFrame((unsigned short*)pan, info->side);
        pf->yOffset = 0;
        pf->xOffset = 0;
        DrawFrame(surface, pf, 0, 0);
    } else {
        MarkChanged(arg1);
    }
}

// FUNCTION: 0x478b40
void __stdcall UpdatePlanet(Menu* arg1, Gadget* arg2)
{
    void* surface = arg1->layer->entries->surface;
    if (g_briefingPlanetNextTick <= GetTickCount()) {
        g_briefingPlanetNextTick = GetTickCount() + 0x19;
        if (g_game->input->IsStreamActive() == 0) {
            if (GetButtonStageByName(arg1, "SHUTUP")) {
                SetButtonStageByName(arg1, "SHUTUP", 0);
                MarkChanged(arg1);
            }
        }
        if (arg2->field_be != 0) {
            MarkLayerChanged(arg1);

            int x1 = arg2->x;
            int x2 = x1 + arg2->w - 1;
            int y1 = arg2->y;
            int y2 = y1 + arg2->h - 1;
            Rect rect;
            rect.left = x1;
            rect.right = x2;
            rect.top = y1;
            rect.bottom = y2;

            if (GetTicks() != g_briefingPlanetLastTick) {
                StepGafSequence(&g_briefingPlanetFrameCursor);
                arg2->frame = g_briefingPlanetFrameCursor.index;
            }
            GafFrame* frame = (GafFrame*)GetGafFrame(arg2->field_be, arg2->frame);
            if (frame == 0) {
                return;
            }
            frame->xOffset = 0;
            int px = arg2->x + arg2->w / 2 - frame->width / 2;
            frame->yOffset = 0;
            int py = arg2->y + arg2->h / 2 - frame->height / 2;
            unsigned char colour =
                ((unsigned char*)arg2->colours)[(int)arg1 + 0x8b2];
            FillRectangle(surface, &rect, colour);
            DrawFrame(surface, frame, px, py);
        } else {
            MarkChanged(arg1);
        }
    }
}

// FUNCTION: 0x478cb0
void __stdcall HandleMissionBriefingClick(Menu* menu)
{
    if (menu->field_60 == -1) {
        FreeBlinkWords(&g_game->menu);
        GameFreeThunk(g_briefingWrappedText);
        g_briefingWrappedText = 0;
        return;
    }
    if (IsCurrentGadgetNamed(menu, "Start")) {
        PlaySoundByName("BigButton", 0);
        if (FindGameCdDrive(0)) {
            RegisterDataArchives();
            SetCursorMode(0x14);
            g_game->input->StopStream();
            BlankScreen();
            g_game->frontendSubstateRequest = 2;
            return;
        }
        OpenMessageBox((char*)&g_game->menu,
                     Translate("Please insert the Campaign CD (Disc 2) and try again"),
                     200, 1, 1);
        ClearSelectedGadget(&g_game->menu);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "SHUTUP")) {
        PlaySoundByName("Options", 0);
        if (!GetButtonStageByName(menu, "SHUTUP")) {
            g_game->input->StopStream();
        } else if (g_game->mode != 6) {
            char* text = (char*)g_game->net->GetNameSlot(3);
            if (text) {
                StreamSoundDelayed(text, 0, 0x3c);
            }
        }
        ClearSelectedGadget(menu);
        PlaySoundByName("SmallButton", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "PrevMenu")) {
        g_game->input->StopStream();
        PlaySoundByName("Previous", 0);
        BlankScreen();
        g_game->frontendSubstateRequest = 3;
        SetCursorMode(0x14);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "TextRegion") || IsCurrentGadgetNamed(menu, "MOREBAR")) {
        if (g_briefingWrappedText) {
            PlaySoundByName("More", 0);
            DrawHelpPage();
            MarkChanged(menu);
        }
    }
    ClearSelectedGadget(menu);
}

// FUNCTION: 0x478e80
void OpenMissionBriefing(void)
{
    char* names[16];
    char* briefs[16];
    char* pans[16];
    char* rotates[16];
    char buf[20];
    char* name;
    Layer* dialog;
    Gadget* gadgets;
    int i;
    unsigned char side;

    side = g_game->players[g_game->localPlayer].info->side;
    sprintf(buf, "mbrief%s", (char*)g_game + 0x37f5b + side * 0x232);

    dialog = LoadGuiLayer(&g_game->menu, "MSNBRIEF.GUI", 0x80);
    dialog->handler = HandleMissionBriefingClick;
    dialog->data = g_game;

    LoadPictureCached(buf, 1, 1, 0);
    RemapPaletteToClosestIndices(&g_game->menu, (char*)g_game + 0x143a7, (char*)g_game + 0x5cb);

    gadgets = g_game->menu.layer->entries;
    strcpy((char*)gadgets + 0xcc, "Start");
    strcpy((char*)gadgets + 0xdc, "PrevMenu");

    i = FindGadgetIndex(gadgets, "MOREBAR", 0xe);
    gadgets[i].attribs &= ~0x10;
    i = FindGadgetIndex(gadgets, "TextRegion", 0xe);
    gadgets[i].attribs &= ~0x10;

    SetGadgetActiveByName(&g_game->menu, "SOLARSYSTEM", 0);
    SetButtonStageByName(&g_game->menu, "SHUTUP", 1);

    g_briefingWindSpeed = g_game->net->minWindSpeed +
                   rand() % (g_game->net->maxWindSpeed - g_game->net->minWindSpeed + 1);
    g_briefingWindTickCountdown = rand() % 64;

    name = g_game->net->GetPlanet();
    if (strcmp(name, "Lunar") == 0 && g_game->flag_37ef2 != 0)
        strcat(name, "2");

    names[0] = "Green planet";
    names[1] = "Archipelago";
    names[2] = "Wet Desert";
    names[3] = "Desert";
    names[4] = "Lava";
    names[5] = "Red Planet";
    names[6] = "Lunar";
    names[7] = "Metal";
    names[8] = "Lunar2";
    names[9] = "Ice";
    names[10] = "Lush";
    names[11] = "Slate";
    names[12] = "Water World";
    names[13] = "Acid";
    names[14] = "Crystal";
    names[15] = 0;

    briefs[0] = "Greenbrief";
    briefs[1] = "Archibrief";
    briefs[2] = "WDesertbrief";
    briefs[3] = "Desertbrief";
    briefs[4] = "Lavabrief";
    briefs[5] = "Marsbrief";
    briefs[6] = "Lunarbrief";
    briefs[7] = "Metalbrief";
    briefs[8] = "Lunar2brief";
    briefs[9] = "Icebrief";
    briefs[10] = "Lushbrief";
    briefs[11] = "Slatebrief";
    briefs[12] = "Waterbrief";
    briefs[13] = "Acidbrief";
    briefs[14] = "Crystalbrief";
    briefs[15] = 0;

    pans[0] = "GreenPan";
    pans[1] = "ArchiPan";
    pans[2] = "WDesPan";
    pans[3] = "DDesPan";
    pans[4] = "LavaPan";
    pans[5] = "MarsPan";
    pans[6] = "LunarPan";
    pans[7] = "MetalPan";
    pans[8] = "Lunar2Pan";
    pans[9] = "IcePan";
    pans[10] = "LushPan";
    pans[11] = "SlatePan";
    pans[12] = "WaterPan";
    pans[13] = "AcidPan";
    pans[14] = "CrystPan";
    pans[15] = 0;

    rotates[0] = "GreenRotate";
    rotates[1] = "ArchiRotate";
    rotates[2] = "WDesertRotate";
    rotates[3] = "DDesRotate";
    rotates[4] = "LavaRotate";
    rotates[5] = "MarsRotate";
    rotates[6] = "LunarRotate";
    rotates[7] = "MetalRotate";
    rotates[8] = "Lunar2Rotate";
    rotates[9] = "IceRotate";
    rotates[10] = "LushRotate";
    rotates[11] = "SlateRotate";
    rotates[12] = "WaterRotate";
    rotates[13] = "AcidRotate";
    rotates[14] = "CrystalRotate";
    rotates[15] = 0;

    i = 0;
    while (briefs[i] != 0 && strcmp(name, names[i]) != 0)
        i++;
    if (briefs[i] == 0)
        i = 0;

    if (LoadScreenGaf(&g_game->menu, briefs[i])) {
        int idx = FindGadgetIndex(gadgets, "PANORAMA", 6);
        if (idx != -1) {
            Gadget* g = &gadgets[idx];
            g->frame = 0;
            void* gaf = FindGafEntry(gadgets->gaf, pans[i]);
            if (gaf != 0) {
                g->field_be = gaf;
                g->field_b6 = (void*)UpdateSolarSystem;
            }
        }
        idx = FindGadgetIndex(gadgets, "PLANET", 6);
        if (idx != -1) {
            void* gaf = FindGafEntry(gadgets->gaf, rotates[i]);
            if (gaf != 0) {
                InitGafSequence(&g_briefingPlanetFrameCursor, gaf, 0);
                Gadget* g = &gadgets[idx];
                g->frame = 0;
                g->field_be = gaf;
                g->field_b6 = (void*)UpdatePlanet;
            }
        }
    }

    SetGadgetActiveByName(&g_game->menu, "SOLARSYSTEM", 0);
    AllocBlinkWords(&g_game->menu, 0xf);
    InitBriefingText();
    SetKeyboardInput(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0xc0);
    SetCursorMode(0x13);
}

// FUNCTION: 0x4794d0
int CountComputerSlots()
{
    int count = 0;
    int num_items = *(int*)((char*)g_game + 0x38d81);
    if (num_items > 0) {
        Item* ptr = g_game->items;
        do {
            if (ptr->type == 2) {
                count++;
            }
            ptr++;
        } while (--num_items);
    }
    return count;
}

// FUNCTION: 0x479500
int __cdecl CountHumanSlots()
{
    int* g_game = (int*)DAT_00511de8;
    int count = 0;
    int num = *(int*)((int)g_game + 0x38d81);

    if (num > 0) {
        int* arr = *(int**)((int)g_game + 0x29a0);
        while (num != 0) {
            if (*arr == 1) {
                count++;
            }
            arr = (int*)((char*)arr + 0x18);
            num--;
        }
    }

    return count;
}

// FUNCTION: 0x479530
int FindOpenSlot()
{
    for (int i = 0; i < g_game->itemCount; i++) {
        if (g_game->items[i].id == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x479560
int AreAllSlotsEmpty()
{
    int result = 1;
    Game* g = g_game;
    int n = g->itemCount;
    if (n > 0) {
        Item* p = g->items;
        do {
            if (p->flag != 0) {
                result = 0;
            }
            p++;
            n--;
        } while (n != 0);
    }
    return result;
}

// FUNCTION: 0x479590
int __stdcall IsColorTaken(int owner, int skip)
{
    for (int i = 0; i < g_game->itemCount; i++) {
        if (g_game->items[i].owner == owner && g_game->items[i].id != 0 && i != skip) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: 0x4795e0
int FindFreeColor()
{
    for (int owner = 0; owner < 10; owner++) {
        int i;
        for (i = 0; i < g_game->itemCount; i++) {
            if (g_game->items[i].owner == owner)
                break;
        }
        if (i == g_game->itemCount)
            return owner;
    }
    return -1;
}

// FUNCTION: 0x479620
int __stdcall CountPlayersInAllyGroup(int owner)
{
    int n = 0;
    for (int i = 0; i < g_game->itemCount; i++) {
        if (g_game->items[i].allyGroup == owner && g_game->items[i].active != 0) {
            n++;
        }
    }
    return n;
}
