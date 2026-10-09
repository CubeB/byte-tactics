// Decompiled by Opus, deepseek-v4.1-flash, GPT-6.1-sol, space-bunny-free, deepseek-v4.1 and Claude Fable 5.1. Names are provisional.
// The in-game keyboard commands (0x495e90) and the screenshot writer
// (0x495930/0x495a30) plus the STOP-order toggles (0x495860/0x4958c0), one
// translation unit.
//
// The Game views: 0x495860/0x4958c0 read 0x531 as a struct pointer with
// `value` at +0x4, while 0x495e90 reads the same pointer as an int and then
// `*(int*)(field_531 + 4)`. One pointer type covers both. The two byte flags
// 0x495a30 names as `uiOptionFlags`/`pauseFlags` are anonymous unions with the
// bitfield views 0x495e90 uses.
//
// Needed by 0x495a30: its frame layout depends on this header.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#pragma pack(push, 1)

// The object Game+0x531 points at, as 0x495860 named it.
struct Struct_00495860 {
    int unknown_0;
    int value;                       // +0x4
};

// 0x4958c0's name for the same object; the Game field below uses it.
// Keep the name: the extra front-end symbol decides 0x495a30's frame layout.
typedef Struct_00495860 Struct_004958c0;

class Mission;
class Class_00438760;

struct Sub_495e90 {
    char unknown_0[0x10];
};

struct Flags_00495e90_37ebe {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short rest : 13;
};

struct Flags_00495e90_37f06 {
    unsigned short b0 : 1;      // mask 0x0001
    unsigned short b1_6 : 6;
    unsigned short b7 : 1;      // mask 0x0080
    unsigned short b8 : 1;      // mask 0x0100
    unsigned short rest : 7;
};

struct Flags_00495e90_37f2f {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short rest : 14;
};

struct Flags_00495e90_38a51 {
    unsigned short b0 : 1;
    unsigned short rest : 15;
};

union Flags_00495e90_3923b {
    unsigned short raw;
    struct {
        unsigned short b0 : 1;
        unsigned short b1 : 1;
        unsigned short rest : 14;
    };
};

struct PlayerInfo {
    char unknown_0[0x9b];
    unsigned char gameFlags;            // +0x9b
};

struct Player_495e90 {
    int valid;                          // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo* data;                   // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct FindData_00495930 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

// Unused here: a real function declared to keep the file's symbol count.
int GetDisplayFieldE4();

struct BmpWriter {
    char unknown_0[0xc];
    void* file;                             // +0xc
    BmpWriter* Init();
    bool Open(const char* name, int width, int height);
    bool WriteRows(void* image, int x, int rows, int unused_4, int y, int unused_6, int srcY);
    void Close();
};

// Unused here: the symbol ids these declarations take keep WriteScreenshot's
// allocation, standing in for the view classes merged above
// (docs/c2-regalloc.md).
int RIReport(int, int, int, int, int, int, int, int, int, int);
void WalkFrameChain(int*, int*, int, int, int*, int, int*, int*, int, int*);

// 24 bytes, not 20: keeps the trailing 4 bytes of the sprite record.
struct GafFrame {
    unsigned short a;                       // +0x0
    unsigned short b;                       // +0x2
    unsigned short e;                       // +0x4
    unsigned short f;                       // +0x6
    unsigned char flag8;                    // +0x8
    unsigned char flag9;                    // +0x9
    unsigned char flaga;                    // +0xa
    unsigned char flagb;                    // +0xb
    char unknown_c[4];
    int d;                                  // +0x10
    int scratch;                            // +0x14
};

struct Src_004b8ae0 {
    char unknown_0[0xbc];
};

struct SrcHolder_00495a30 {
    char unknown_0[0xbc];
    Src_004b8ae0* frame;                    // +0xbc
};

struct Rect_00495a30 {
    int left;                               // +0x0
    int top;                                // +0x4
    int right;                              // +0x8
    int bottom;                             // +0xc
};

struct Surface {
    int width;                              // +0x0
    int height;                             // +0x4
    int pitch;                              // +0x8
    unsigned char* bits;                    // +0xc
    int zPriority;                          // +0x10
    int colorKey;                           // +0x14
    unsigned short x;                       // +0x18
    unsigned short y;                       // +0x1a
    char unknown_1c[0x10];
    unsigned int flag0 : 1;                 // +0x2c
    unsigned int flag1 : 1;
    void SetClipRect(Rect_00495a30 r);
};

struct Game {
    char unknown_0[0x519];
    Sub_495e90 gui;                     // +0x519
    char unknown_529[0x531 - 0x529];
    Struct_004958c0* field_531;         // +0x531
    char unknown_535[0x1b63 - 0x535];
    Player_495e90 players[10];          // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x2c76 - 0x2a43];
    char orders_2c76[0x2cba - 0x2c76];  // +0x2c76
    unsigned short hoverUnitId;         // +0x2cba
    char unknown_2cbc[0x2cc3 - 0x2cbc];
    unsigned char orderMode;            // +0x2cc3
    char unknown_2cc4[0x2cc6 - 0x2cc4];
    unsigned char inputFlags;           // +0x2cc6
    char unknown_2cc7[0x1423b - 0x2cc7];
    int screenTilesX;                   // +0x1423b
    int screenTilesY;                   // +0x1423f
    char unknown_14243[0x14280 - 0x14243];
    unsigned char debugMode;            // +0x14280
    unsigned short mapFlags;            // +0x14281
    char unknown_14283[0x1431f - 0x14283];
    int scrollX;                        // +0x1431f
    int scrollY;                        // +0x14323
    char unknown_14327[0x37e27 - 0x14327];
    int viewCullMinX;                   // +0x37e27
    int viewCullMinY;                   // +0x37e2b
    char unknown_37e2f[0x37e9c - 0x37e2f];
    unsigned short unitIndex;           // +0x37e9c
    char unknown_37e9e[0x37ea0 - 0x37e9e];
    char guiName[0x37ebe - 0x37ea0];
    Flags_00495e90_37ebe flags_37ebe;   // +0x37ebe
    char unknown_37ec0[0x37f06 - 0x37ec0];
    Flags_00495e90_37f06 flags_37f06;   // +0x37f06
    char unknown_37f08[0x37f27 - 0x37f08];
    int textLines;                      // +0x37f27
    char unknown_37f2b[0x37f2f - 0x37f2b];
    // Both views of the flag word are used: raw `& 2` tests and the b1 bitfield.
    union {
        unsigned short uiOptionFlags;   // +0x37f2f
        Flags_00495e90_37f2f flags_37f2f;
    };
    char unknown_37f31[0x38a47 - 0x37f31];
    int ticks;                          // +0x38a47
    unsigned short gameSpeed;           // +0x38a4b
    char unknown_38a4d[0x38a51 - 0x38a4d];
    union {
        unsigned short pauseFlags;      // +0x38a51
        Flags_00495e90_38a51 flags_38a51;
    };
    char field_38a53[0x38b53 - 0x38a53];
    char field_38b53[0x38c53 - 0x38b53];
    int movieCaptureIndex;              // +0x38c53
    int movieOutputRate;
    int nextMovieFrameTick;             // +0x38c5b
    char unknown_38c5f[0x391b3 - 0x38c5f];
    int unitInfoHoverTrack;             // +0x391b3
    unsigned short unitInfoHoverUnitId;  // +0x391b7
    int unitBuilderHoverTrack;          // +0x391b9
    unsigned short unitBuilderHoverUnitId;  // +0x391bd
    char unknown_391bf[0x391e9 - 0x391bf];
    Mission* net;                       // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    Flags_00495e90_3923b flags_3923b;   // +0x3923b
};

#pragma pack(pop)

#include "../map/mission.h"

class Class_00438760 {
public:
    Class_00438760(const char* name);
    unsigned char index;                // +0
};

extern Game* g_game;
extern char g_pathSepBackslash[];
extern char DAT_005119b8[];

int __stdcall FindGadgetIndexBySubstring(int value, const char* name);
void __stdcall ClearGroupStatus(Sub_495e90* gui, int handle);
int __stdcall HAPI_FindFirst(const char* path, void* findData, int a, int b);
int __stdcall HAPI_FindNext(int handle, void* findData);
void __stdcall HAPI_FindClose(int handle);
void __stdcall BuildScreenshotPath(char* out, const char* dir, const char* name, const char* ext);
GafFrame* __stdcall AllocFrame(const char* name, int width, int height);
void __cdecl SetOutOfMemoryHandler(int param);
void __stdcall InstallOutOfMemoryHandler();
void __stdcall SurfaceFromFrame(Surface* dst, void* src);
void* __stdcall GetDisplay();
void __stdcall FrameFromSurface(GafFrame* dst, Src_004b8ae0* src);
void __stdcall SetCameraPosition(int x, int y, int z);
void __stdcall CollectVisibleUnitIds();
void __stdcall DrawBattleFrame(int param_1, int param_2);
void __stdcall DrawFrame(Surface* surf, GafFrame* pal, int x, int y);
void __stdcall ClearFrame(void* b, int color);
void __stdcall RecalculateLineOfSight(int param);
void __cdecl GameFreeThunk(void* b);
int PopKey(void);
int __stdcall IsKeyDown(int key);
void ClearSelection(void);
void __stdcall PopUntilNamedLayout(int param);
void __stdcall CloseTopScreen(Sub_495e90* gui);
int __stdcall IsScreenNamed(Sub_495e90* gui, char* name);
void SaveSettings(void);
void __stdcall PlaySoundByName(const char* name, int param);
void OpenTalkDialog(void);
void __stdcall StepBuildMenuPageBack(int param);
void __stdcall StepBuildMenuPage(int param);
void __stdcall OpenBuildMenuPage(int param);
void __stdcall SelectSquad(int index, int key);
void __stdcall CycleCameraFollow(int param);
void __stdcall ExecuteCommandLine(int param_1, int param_2);
void OpenUnitInfoDialog(void);
void OpenShareDialog(void);
void FocusNextLocalUnit(void);
void SelectAllIdleUnits(void);
void SelectUnitsOfSameTypes(void);
void __stdcall SelectUnitsByCategory(const char* name, int key);
void FindLocalCommander(void);
void SelectAllVisibleUnits(void);
void __stdcall CollectSelectedUnits(void* param);
void __stdcall CreateSquad(int param);
void __stdcall SaveCameraPosition(int param);
void __stdcall RestoreCameraPosition(int param);
void CycleMessageUnits(void);
void ResetChatHudIndices(void);
void __stdcall SetGameSpeed(int param_1, int param_2);
void __stdcall SetDescListCleanupFlag(Sub_495e90* gui, int param);
int GetLocalDpid(void);
void __stdcall BroadcastPacket(int param_1, void* param_2, int param_3);
void ToggleTabMenu(void);
void __stdcall HandleDebugHotkey(int eventType);
void __stdcall OpenInGameOptions(void);
void __stdcall MakeDirectoryPath(char* path);
void __stdcall SaveScreenshot(char* param_1, const char* param_2);
void __stdcall IssueOrderToSelection(void* a, int b, Class_00438760 kind, int d, int e, int f);
int __stdcall FindOrderByType(int unit, Class_00438760 kind);
void __stdcall DeleteOrder(int unit, int arg);
void __cdecl operator delete(void* p);

// FUNCTION: 0x495860
void SelectStopOrder(void)
{
    int index;

    g_game->orderMode = 1;
    g_game->inputFlags &= 0xdf;
    index = FindGadgetIndexBySubstring(g_game->field_531->value, "STOP");
    if (index != -1) {
        ClearGroupStatus(&g_game->gui, index);
    }
}

// The else branch is the body of SelectStopOrder.
// FUNCTION: 0x4958c0
void __stdcall SetOrSelectStopOrder(int set)
{
    int index;

    if (set) {
        g_game->inputFlags |= 0x20;
        return;
    }
    g_game->orderMode = 1;
    g_game->inputFlags &= 0xdf;
    index = FindGadgetIndexBySubstring(g_game->field_531->value, "STOP");
    if (index != -1) {
        ClearGroupStatus(&g_game->gui, index);
    }
}

// FUNCTION: 0x495930
void __stdcall BuildScreenshotPath(char* out, const char* dir, const char* name, const char* ext)
{
    bool needSep = false;
    FindData_00495930 fd;
    int max = 0;

    if (*dir != 0) {
        if (dir[strlen(dir) - 1] != '\\') {
            needSep = true;
        }
    }
    sprintf(out, "%s%s%s*.%s", dir, needSep ? g_pathSepBackslash : DAT_005119b8, name, ext);
    int handle = HAPI_FindFirst(out, &fd, -1, 1);
    if (handle >= 0) {
        do {
            int val = atoi(fd.name + strlen(name));
            if (val > max) {
                max = val;
            }
        } while (HAPI_FindNext(handle, &fd) == 0);
        HAPI_FindClose(handle);
    }
    sprintf(out, "%s%s%s%04i.%s", dir, needSep ? g_pathSepBackslash : DAT_005119b8, name, max + 1, ext);
}

// Screenshot writer: renders the map in screen-sized tiles into an offscreen
// bitmap and appends each band to a .bmp file.
// Unused here: real declarations from the in-game interface whose symbol ids
// keep the allocation (docs/c2-regalloc.md).
void UpdateBuildMenuIfFocusUnit(void*);
void ActivatePlayerGadgets(char*);
// Unused here: real zero-argument functions whose symbol ids keep
// WriteScreenshot's allocation after Surface_00495a30 and Surface became one
// (docs/c2-regalloc.md).
void ResetCameraState(void);
void ClampCameraPosition(void);
void ClampCameraTarget(void);
void UpdateScreenShake(void);
void UpdateCameraFollow(void);
void BeginMouseScroll(void);
void EndMouseScroll(void);
void UpdateMouseScroll(void);
void UpdateEdgeScroll(void);
// FUNCTION: 0x495a30
void __stdcall WriteScreenshot(char* dir, char* name, int x, int y, int w, int h)
{
    int var24;
    unsigned int savedC;
    unsigned short fl;
    char filename[260];
    int bw, off27, sy;
    BuildScreenshotPath(filename, dir, name, "bmp");

    int y2;
    BmpWriter bmp;
    bmp.Init();
    if (bmp.Open(filename, w, h)) {
        GafFrame* bm;
        off27 = g_game->viewCullMinX;
        int bh, off2b;
        bw = g_game->screenTilesX * 16;
        off2b = g_game->viewCullMinY;
        bh = (g_game->screenTilesY * 16) - 1;

        SetOutOfMemoryHandler(0);
        bm = AllocFrame("ScreenShot", w, bh);
        if (bm == 0) {
            bh /= 2;
            bm = AllocFrame("ScreenShot", w, bh);
        }
        InstallOutOfMemoryHandler();
        if (bm != 0) {
            int scrollX;
            Surface surf;
            scrollX = g_game->scrollX;
            GafFrame pal;
            fl = g_game->mapFlags;
            int savedbit0, scrollY = g_game->scrollY, bit6;
            int savedbit1;
            savedbit0 = fl & 1;
            savedbit1 = (fl >> 1) & 1;

            int savedA;
            g_game->mapFlags = fl & ~1;
            g_game->mapFlags &= ~2;
            RecalculateLineOfSight(1);
            savedA = g_game->pauseFlags & 1;
            g_game->pauseFlags = (unsigned short)(g_game->pauseFlags & ~1);
            int savedB = (g_game->uiOptionFlags >> 6) & 1;

            g_game->uiOptionFlags = (unsigned short)(g_game->uiOptionFlags & ~0x40);
            savedC = g_game->textLines;

            g_game->textLines = 0;
            SurfaceFromFrame(&surf, bm);
            FrameFromSurface(&pal, ((SrcHolder_00495a30*)GetDisplay())->frame);

            pal.flag8 = 0;
            int row = 0;

            if (h > row) {
                int var1c, negbh;
                var24 = bh;
                unsigned int var10 = 0;
                negbh = -bh;
                var1c = -1;
                while (1) {
                    ClearFrame(bm, 0);
                    int col, rows;
                    col = 0;
                    if (col < w) do {
                        int right;
                        Rect_00495a30 box;
                        box.left = col;
                        SetCameraPosition(x + col, y + row, 0);
                        CollectVisibleUnitIds();
                        DrawBattleFrame(1, 0);
                        right = (col + bw) - 1;
                        if (right >= surf.width) right = surf.width - 1;
                        box.top = 0;
                        box.right = right;
                        box.bottom = bh - 1;
                        surf.SetClipRect(box);
                        DrawFrame(&surf, &pal, g_game->scrollX - x - off27,
                                     g_game->scrollY - row - y - off2b);
                    } while (((col += bw), (col < w)));
                    sy = var10;
                    int srcY = 0;
                    rows = bh;
                    y2 = row;
                    if (var1c < -1) {
                        y2 = row + 1;
                        sy = var1c;
                        rows = bh - 1;
                        srcY = 1;
                    }
                    if (y2 + rows > h) rows = h + sy;
                    if (!bmp.WriteRows(&surf, w, rows, 0, row, 0, srcY)) break;
                    if (var24 < h) {
                        row = row - 1;
                        var10++;
                        var1c = var1c + 1;
                        var24 = var24 - 1;
                    }
                    row += bh;
                    var10 += negbh;
                    var1c = var1c + negbh;
                    var24 += bh;
                    if (row >= h) break;
                }
            }
            GameFreeThunk(bm);
            g_game->pauseFlags = (unsigned short)(g_game->pauseFlags ^ ((savedA ^ g_game->pauseFlags) & 1));
            bit6 = (savedB & 1) << 6;
            g_game->uiOptionFlags = (unsigned short)((((unsigned short)g_game->uiOptionFlags) & ~0x40) | bit6);
            g_game->textLines = savedC;
            SetCameraPosition(scrollX, scrollY, 0);
            g_game->mapFlags = (unsigned short)(g_game->mapFlags ^ ((savedbit0 ^ g_game->mapFlags) & 1));
            g_game->mapFlags = (unsigned short)((unsigned short)((g_game->mapFlags & ~2) | ((savedbit1 & 1) << 1)));
            RecalculateLineOfSight(1);
            CollectVisibleUnitIds();
            DrawBattleFrame(1, 1);
        }
    }
    bmp.Close();
}

// In-game keyboard command dispatcher: PopKey returns the event (0 means
// return), IsKeyDown(0xf9) the "key down" flag. The switch is value sorted
// The key == 0 arms are written as `if (key != 0) { ... } else` to stay out of line.
// FUNCTION: 0x495e90
void HandleGameKey(void)
{
    int event = PopKey();
    if (event == 0)
        return;

    int key = IsKeyDown(0xf9);

    // Case bodies stay in the original physical order: the jump table depends on it.
    switch (event) {
    case 0x1b:
        if (g_game->flags_37ebe.b0) {
            g_game->flags_37ebe.b0 = 0;
            int r = IsScreenNamed(&g_game->gui, g_game->guiName);
            if (r == 0) {
                g_game->unitIndex = 0;
                CloseTopScreen(&g_game->gui);
            }
        } else {
            if (g_game->orderMode != 1) {
                g_game->orderMode = 1;
                g_game->inputFlags = g_game->inputFlags & 0xdf;
                int handle = FindGadgetIndexBySubstring(g_game->field_531->value, "STOP");
                if (handle != -1)
                    ClearGroupStatus(&g_game->gui, handle);
            } else {
                ClearSelection();
                PopUntilNamedLayout(1);
            }
        }
        break;

    case 0xc5:
    case 0xc6:
    case 0xc7:
    case 0xc8:
    case 0xc9:
    case 0xca:
    case 0xcb:
    case 0xcc:
    case 0xcd:
        CreateSquad(event - 0xc4);
        PlaySoundByName("CreateSquad", 0);
        break;

    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
        if (g_game->flags_37f06.b8) {
            if (IsKeyDown(0xfb) != 0) {
                OpenBuildMenuPage(event - 0x31);
            } else {
                SelectSquad(event - 0x30, key);
                PlaySoundByName("SelectSquad", 0);
            }
        } else {
            if (IsKeyDown(0xfb) != 0) {
                SelectSquad(event - 0x30, key);
                PlaySoundByName("SelectSquad", 0);
            } else
                OpenBuildMenuPage(event - 0x31);
        }
        break;

    case 0xd2:
    case 0xd3:
    case 0xd4:
    case 0xd5:
        PlaySoundByName("SelectSquad", 0);
        SaveCameraPosition(event - 0xd2);
        break;

    case 0xe6:
    case 0xe7:
    case 0xe8:
    case 0xe9:
        PlaySoundByName("SelectSquad", 0);
        RestoreCameraPosition(event - 0xe6);
        break;

    case 0x21:
    case 0x23:
    case 0x2a:
    case 0x60:
    case 0x7e:
        g_game->flags_37f06.b0 = !g_game->flags_37f06.b0;
        SaveSettings();
        break;

    case 0x2c:
        StepBuildMenuPageBack(1);
        break;

    case 0x2e:
        StepBuildMenuPage(1);
        break;

    case 0xf8: {
        unsigned char data[4];
        data[0] = 0x19;
        data[1] = 0;
        g_game->flags_38a51.b0 = !g_game->flags_38a51.b0;
        data[2] = (unsigned char)(g_game->flags_38a51.b0);
        // One expression: the literal 3 must be pushed before the toggle.
        BroadcastPacket(GetLocalDpid(), data, 3);
        break;
    }

    case 0xe2:
        if (key != 0) {
            if (g_game->hoverUnitId != 0) {
                g_game->unitInfoHoverTrack = 1;
                g_game->unitInfoHoverUnitId = g_game->hoverUnitId;
            } else {
                g_game->unitInfoHoverTrack = 0;
            }
        } else {
            OpenUnitInfoDialog();
        }
        break;

    case 9:
        if (g_game->net->GetGameType() == 3) {
            if (!g_game->flags_37ebe.b2)
                ToggleTabMenu();
            break;
        }
        // fall through
    case 0xe3:
        if (key != 0) {
            if (g_game->hoverUnitId != 0) {
                g_game->unitBuilderHoverTrack = 1;
                g_game->unitBuilderHoverUnitId = g_game->hoverUnitId;
            } else {
                g_game->unitBuilderHoverTrack = 0;
            }
        } else {
            if (!g_game->flags_37ebe.b0) {
                OpenInGameOptions();
                g_game->flags_37ebe.b0 = 1;
            }
        }
        break;

    case 0xe4:
        CycleMessageUnits();
        break;

    case 0xd7: {
        if (g_game->flags_37f2f.b1) {
            if (g_game->movieCaptureIndex != 0) {
                g_game->movieCaptureIndex = 0;
            } else {
                // Reset in both arms: the compiler hoists the common store.
                g_game->movieCaptureIndex = 0;
                // path[0x100] and data[4] (case 0xf8) pin the frame size.
                char path[0x100];
                char findData[0x118];
                sprintf(path, "%s\\MOVIE*", g_game->field_38a53);
                int h = HAPI_FindFirst(path, findData, -1, 1);
                if (h >= 0) {
                    do {
                        int n = atoi(&findData[0x19]);
                        if (n > g_game->movieCaptureIndex)
                            g_game->movieCaptureIndex = n;
                    } while (HAPI_FindNext(h, findData) == 0);
                    HAPI_FindClose(h);
                }
                g_game->movieCaptureIndex++;
                sprintf(g_game->field_38b53, "%s\\MOVIE%03i",
                        g_game->field_38a53, g_game->movieCaptureIndex);
                MakeDirectoryPath(g_game->field_38b53);
                DrawBattleFrame(0, 1);
                SaveScreenshot(g_game->field_38b53, "FRAM");
                g_game->nextMovieFrameTick = g_game->ticks;
            }
        }
        break;
    }

    case 0xec:
        if (g_game->flags_37f2f.b1) {
            g_game->flags_3923b.b1 = !g_game->flags_3923b.b1;
            if (g_game->flags_3923b.b1) {
                SetDescListCleanupFlag(&g_game->gui, 0);
            } else {
                g_game->flags_3923b.b0 = 0;
                g_game->debugMode = 0;
                SetDescListCleanupFlag(&g_game->gui, 1);
            }
        }
        break;

    case 0x5c:
        if (g_game->flags_37f2f.b1)
            ExecuteCommandLine(0, -1);
        break;

    case 0xe5:
        g_game->flags_37f06.b7 = !g_game->flags_37f06.b7;
        break;

    case 0xed:
        ResetChatHudIndices();
        break;

    case 0xaa:
        SelectAllIdleUnits();
        break;

    case 0xab:
    case 0xae:
    case 0xaf:
    case 0xb0:
    case 0xb1:
    case 0xb2:
    case 0xb3:
    case 0xb4:
    case 0xb5:
    case 0xb6:
    case 0xb7:
    case 0xb8:
    case 0xb9:
    case 0xba:
    case 0xbb:
    case 0xbd:
    case 0xbe:
    case 0xbf:
    case 0xc0:
    case 0xc1:
    case 0xc2: {
        // Exactly 7 bytes: a larger buffer moves the frame layout.
        char buf[7];
        sprintf(buf, "CTRL_%c", event - 0x69);
        SelectUnitsByCategory(buf, key);
        break;
    }

    case 0xac:
        SelectUnitsByCategory("CTRL_C", key);
        FindLocalCommander();
        break;

    case 0xc3:
        SelectUnitsOfSameTypes();
        break;

    case 0xad: {
        std::vector<int> sel;
        CollectSelectedUnits(&sel);
        int found = 0;
        Class_00438760 order("SELFDESTRUCT");
        for (std::vector<int>::iterator it = sel.begin(); it != sel.end(); ++it) {
            int r = FindOrderByType(*it, order);
            if (r != 0) {
                found = 1;
                DeleteOrder(*it, r);
            }
        }
        if (found == 0)
            IssueOrderToSelection(g_game->orders_2c76, 0, order, 0, 0, 0);
        break;
    }

    case 0x68:
        if (g_game->net->GetGameType() == 3)
            OpenShareDialog();
        break;

    case 0x6e:
        FocusNextLocalUnit();
        break;

    case 0xbc:
        SelectAllVisibleUnits();
        break;

    case 0x74:
        CycleCameraFollow(0);
        break;

    case 0x54:
        CycleCameraFollow(1);
        break;

    case 0xd:
        PlaySoundByName("SmallButton", 0);
        OpenTalkDialog();
        break;

    case 0x2d:
    case 0x5f:
        if (!(g_game->flags_3923b.raw & 2)) {
            Player_495e90* pl = &g_game->players[g_game->localPlayer];
            if (pl->valid != 0 && (pl->data->gameFlags & 0x40) != 0)
                break;
            if (g_game->gameSpeed <= 1)
                break;
            SetGameSpeed(g_game->gameSpeed - 1, 1);
        }
        break;

    case 0x2b:
    case 0x3d:
        if (!(g_game->flags_3923b.raw & 2)) {
            Player_495e90* pl = &g_game->players[g_game->localPlayer];
            if (pl->valid != 0 && (pl->data->gameFlags & 0x40) != 0)
                break;
            if (g_game->gameSpeed >= 0x14)
                break;
            SetGameSpeed(g_game->gameSpeed + 1, 1);
        }
        break;

    default:
        break;
    }

    if (g_game->flags_3923b.b1)
        HandleDebugHotkey(event);
}
