// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, claude-sonnet-5-5, Space Bunny Free and Claude Opus 5.5. Names are provisional.
// Kept its own file: info_panel.cpp includes <windows.h> and <ddraw.h> for
// 0x4685a0 and 0x46a610, which flips this function's imul operand order.
// Draws the game view's overlays into a copy of the screen context: the
// cursor cross, the resource panel (metal and energy bars and counters, only
// redrawn when the smoothed values change), the features and units of the
// visible map rows (first bucketed per row into the unit lists at
// g_game+0x141fb), the unit group numbers, the selection box and the debug and
// timing text. Profile marks go to g_game+0x38d85 (phases 8, 3, 4 and 5); the
// last mark (phase 3) is the out-of-line AccumulateProfileTime, which the
// original file defines after this function.
// No <windows.h>: it flips the imul operand order in the row and unit loops.
#include <stdio.h>
#include <math.h>
#include <memory.h>
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
struct Game;
extern Game* g_game;
unsigned long GetMilliseconds();
int __stdcall FormatNetStats(int);
int __stdcall DrawSelectedGoal(int,int);
int __stdcall DrawMapDebugOverlay(int);
int __stdcall DrawExplosions(int);
#include "../map/mission.h"

int __stdcall DrawUnit(int,int);
int __stdcall DrawOptionsScrollBar(int);
int __stdcall DrawMessages(int);
float __stdcall GetEnergyIncome(int);
float __stdcall GetEnergyUsage(int);
float __stdcall GetMetalIncome(int);
float __stdcall GetMetalUsage(int);
int __stdcall IsFootprintVisible(int,int,int,int,int,int);
int __stdcall DrawRadar(int);
int __stdcall BlitGafFrameAtOffset(int,int,int,int);
int __stdcall BlitSideLogoToRect(int,int,int,int);
int __stdcall DrawNetworkStats(int);
int __stdcall DrawStatusPanel(int);
int __stdcall DrawHitPointBar(int,int,int,int);
int __stdcall DrawSelectionBox(int,int);
int __stdcall BlitFeatureGaf(int,int,int,int);
int __stdcall DrawUnitInfoPanel(int);
int __stdcall DrawProfileBarLine(int,int,int);
int __stdcall DrawParticleList(int,int);
int __stdcall DrawMapTiles(int);
int __stdcall DrawFogOfWar(int);
int __stdcall FindNextSelectedUnit(int,int);
int __stdcall DrawSelectedUnitOrderOverlays(int,int);
int __stdcall DrawScorePanel(int);
int __stdcall DrawProjectiles(int);
int __stdcall BlitMenuLayers(int,int,int);
int GetFrameRate();
int GetScreenHeight();
int __stdcall PointInRect(int,int,int);
int __stdcall GetGafFrame(int,int);
int __stdcall DrawFrame(int,int,int,int);
int __stdcall DrawLine(int,int,int,int,int,int);
int __stdcall FillRectangle(int,int,int);
int __stdcall DrawRectangle(int,int,int);
int __stdcall SetTextColors(int,int);
int GetTextKeyColor();
int __stdcall SetFont(int);
int GetFontHeight();
int __stdcall GetTextWidth(int,int);
int __stdcall DrawString(int,int,int,int,int);
int __stdcall IsKeyDown(int);
int HideSoftwareCursor();
int ShowSoftwareCursor();
int __stdcall Translate(int);
int FlipScreen();
int __stdcall SetOffscreenSurface(int);
int __stdcall ResetClipRect(int);

struct OverlayRect { int left, top, right, bottom; };
struct Surface { int data[12]; int SetClipRect(OverlayRect); };

// The frame-time profile at g_game+0x38d85 (FrameTimers): last tick at +0,
// one accumulator per phase at +0x2c. AccumulateProfileTime itself is defined after
// this function in the original file, so only the hand-inlined copies below
// were expanded; the last call stays out of line.
struct FrameTimers {
  unsigned long last;
  int total;
  int values[9];
  int acc[9];
  void AccumulateProfileTime(int i);
};
static inline void ProfileMark(FrameTimers *p, int i)
{
  unsigned long t = GetMilliseconds();
  p->acc[i] += t - p->last;
  p->last = t;
}

#pragma pack(push, 1)
struct Resources {          // g_game+0x37e3f, 33 bytes
  char owner;
  float metal;              // +0x01
  float metalIncome;        // +0x05
  float metalUse;           // +0x09
  float energy;             // +0x0d
  float energyIncome;       // +0x11
  float energyUse;          // +0x15
  float maxMetal;           // +0x19
  float maxEnergy;          // +0x1d
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Tile_00468cf0 {       // 0xd bytes
  char unknown_0[4];
  byte field_4;              // +0x04
  char unknown_5[3];
  ushort feature;            // +0x08, a feature index; 0xfffb and up is none
  char unknown_a[2];
  byte flags;                // +0x0c
};

struct Feature_00468cf0 {    // 0x100 bytes
  char unknown_0[0x94];
  short field_94;            // +0x94
  short field_96;            // +0x96
  char unknown_98[0xfa - 0x98];
  byte field_fa;             // +0xfa
  char unknown_fb[4];
  byte flags;                // +0xff
};
#pragma pack(pop)

struct Unit_00468cf0;
struct MapGrid { Unit_00468cf0 **buf; Unit_00468cf0 ***cursor; ushort *count; char pad1[0x2c]; int width; int height; char pad2[0x10]; int stride; int rows; char pad3[0x34]; Tile_00468cf0 *tiles; };
struct Bits8 { ushort b0:1; ushort b1:1; ushort b2:1; ushort b3:1; ushort b4:1; ushort b5:1; ushort b6:1; ushort b7:1; };
struct UnitFlags { uint kind:2; uint b2:1; uint b3:1; uint b4:1; uint b5:1; uint b6:1; uint b7:1; };

// The game state this file reads, with the member names the other Game views
// use. The bit words share a union with each view's bitfield type.
#pragma pack(push, 1)
// A player's entry at g_game+0x1b63, stride 0x14b (Thaldren: PlayerState).
struct SideData_00468cf0 {
  char unknown_0[0x95];
  unsigned char bSideId;              // +0x95
  char unknown_96[0x9b - 0x96];
  unsigned char gameOptFlags;         // +0x9b, low byte of wGameOptFlags
  char unknown_9c[0xb9 - 0x9c];
};

struct PlayerState_00468cf0 {
  char unknown_0[0x27];
  SideData_00468cf0* info;            // +0x27, pSideData
  char unknown_2b[0x8c - 0x2b];
  float flEnergyAmount;               // +0x8c
  char unknown_90[0x98 - 0x90];
  float flMetalAmount;                // +0x98
  char unknown_9c[0xa4 - 0x9c];
  float flEnergyStorage;              // +0xa4
  float flMetalStorage;               // +0xa8
  char unknown_ac[0xe4 - 0xac];
  float flShareThresholdMetal;        // +0xe4
  float flShareThresholdEnergy;       // +0xe8
  char unknown_ec[0xf8 - 0xec];
  unsigned int nDisplayTimer;         // +0xf8
  char unknown_fc[0x146 - 0xfc];
  unsigned char bSlotIndex;           // +0x146
  char unknown_147[0x14b - 0x147];
};

struct Unit_00468cf0 {                 // 0x118 bytes
  char unknown_0[0x6c];
  short field_6c;                     // +0x6c
  char unknown_6e[2];
  short field_70;                     // +0x70
  char unknown_72[2];
  short field_74;                     // +0x74
  char unknown_76[0x96 - 0x76];
  PlayerState_00468cf0* player;       // +0x96
  void* script;                       // +0x9a
  char unknown_9e[0xac - 0x9e];
  int group;                          // +0xac, the control group number
  char unknown_b0[0x110 - 0xb0];
  UnitFlags flags;                    // +0x110
  char unknown_111[0x118 - 0x111];
};

// A side's panel layout at g_game+0x37f3d, stride 0x232 (Thaldren: SideDef).
struct SideDef_00468cf0 {
  char aName[0x1e];                   // +0x000
  char aNamePrefix[4];                // +0x01e
  char aCommander[0x20];              // +0x022
  OverlayRect rcLogo;                 // +0x042
  OverlayRect rcEnergyBar;            // +0x052
  OverlayRect rcEnergyNum;            // +0x062
  OverlayRect rcMetalBar;             // +0x072
  OverlayRect rcMetalNum;             // +0x082
  OverlayRect rcTotalUnits;           // +0x092
  OverlayRect rcTotalTime;            // +0x0a2
  OverlayRect rcEnergyMax;            // +0x0b2
  OverlayRect rcMetalMax;             // +0x0c2
  OverlayRect rcEnergy0;              // +0x0d2
  OverlayRect rcMetal0;               // +0x0e2
  OverlayRect rcEnergyProduced;       // +0x0f2
  OverlayRect rcEnergyConsumed;       // +0x102
  OverlayRect rcMetalProduced;        // +0x112
  OverlayRect rcMetalConsumed;        // +0x122
  OverlayRect rcLogo2;                // +0x132
  OverlayRect rcUnitName;             // +0x142
  OverlayRect rcDamageBar;            // +0x152
  OverlayRect rcUnitEnergyMake;       // +0x162
  OverlayRect rcUnitEnergyUse;        // +0x172
  OverlayRect rcUnitMetalMake;        // +0x182
  OverlayRect rcUnitMetalUse;         // +0x192
  OverlayRect rcMissionText;          // +0x1a2
  OverlayRect rcUnitName2;            // +0x1b2
  OverlayRect rcDamageBar2;           // +0x1c2
  OverlayRect rcName;                 // +0x1d2
  OverlayRect rcDescription;          // +0x1e2
  OverlayRect aReloadRects[3];        // +0x1f2
  int nEnergyColor;                   // +0x222
  int nMetalColor;                    // +0x226
  int nSideIndex;                     // +0x22a
  char* pFontData;                    // +0x22e
};

struct Game {
  char unknown_0[0x519];
  int menu;                             // +0x519, the GUI system object
  char unknown_51d[0xdcb - 0x51d];
  unsigned char colors[16];             // +0xdcb
  char unknown_ddb[0x1b63 - 0xddb];
  PlayerState_00468cf0 players[11];     // +0x1b63, stride 0x14b
  char unknown_299c[0x2a42 - 0x299c];
  unsigned char localPlayer;            // +0x2a42
  unsigned char playerIndex;            // +0x2a43
  unsigned char flags_2a44;             // +0x2a44
  char unknown_2a45[0x2c76 - 0x2a45];
  int cursorScreenX;                    // +0x2c76
  int cursorScreenY;                    // +0x2c7a
  char unknown_2c7e[0x2c92 - 0x2c7e];
  int boxStartX;                        // +0x2c92
  int boxStartHeight;                   // +0x2c96
  int boxStartZ;                        // +0x2c9a
  int boxEndX;                          // +0x2c9e
  int boxEndHeight;                     // +0x2ca2
  int boxEndZ;                          // +0x2ca6
  char unknown_2caa[2];
  short field_2cac;                     // +0x2cac
  char unknown_2cae[2];
  short field_2cb0;                     // +0x2cb0
  char unknown_2cb2[2];
  short field_2cb4;                     // +0x2cb4
  char unknown_2cb6[0x2cc3 - 0x2cb6];
  char orderMode;                       // +0x2cc3
  char unknown_2cc4[0x2cc6 - 0x2cc4];
  unsigned char inputFlags;             // +0x2cc6
  char unknown_2cc7[0x141fb - 0x2cc7];
  union {                               // +0x141fb
    MapGrid sortUnits;                  // the map/sort grid view of this block
    struct {
      char unknown_141fb[0x1426f - 0x141fb];
      Feature_00468cf0* features;       // +0x1426f
      char unknown_14273[0x14280 - 0x14273];
      char debugMode;                   // +0x14280
      char unknown_14281[0x1428b - 0x14281];
    };
  };
  char unknown_1428b[0x142f3 - 0x1428b];
  void* followUnit;                     // +0x142f3
  char unknown_142f7[0x1431f - 0x142f7];
  int scrollX;                          // +0x1431f
  int scrollY;                         // +0x14323
  char unknown_14327[0x14357 - 0x14327];
  int units;                            // +0x14357
  char unknown_1435b[0x1435f - 0x1435b];
  unsigned short* visibleUnitIds;       // +0x1435f
  char unknown_14363[0x14367 - 0x14363];
  int count;                            // +0x14367
  char unknown_1436b[0x14813 - 0x1436b];
  int igvictory;                        // +0x14813
  int igdefeat;                         // +0x14817
  int igpaused;                         // +0x1481b
  // The side panel's GAF frames at +0x1481f: the top row's five entries
  // followed by the bottom row's (Thaldren: apSidePanelTopSeq, apSidePanelBotSeq).
  unsigned short* sidePanelRows[10];
  char unknown_14847[0x148cf - 0x14847];
  int cursorHourglass;                  // +0x148cf
  char unknown_148d3[0x37e1b - 0x148d3];
  int screen;                           // +0x37e1b
  int width;                            // +0x37e1f
  int height;                           // +0x37e23
  union {                               // +0x37e27
    OverlayRect lim;
    struct {
      int viewCullMinX;
      int viewCullMinY;
      int viewCullMaxX;
      int viewCullMaxY;
    };
  };
  char unknown_37e37[0x37e3f - 0x37e37];
  Resources resources;                  // +0x37e3f
  char unknown_37e60[0x37f06 - 0x37e60];
  union {                               // +0x37f06
    unsigned short visualFlags;
    unsigned char visualFlagsByte;
  };
  char unknown_37f08[0x37f2f - 0x37f08];
  union {                               // +0x37f2f
    unsigned short uiOptionFlags;
    Bits8 bits_37f2f;
  };
  char unknown_37f31[0x37f3d - 0x37f31];
  SideDef_00468cf0 sideDefs[5];         // +0x37f3d, stride 0x232
  char unknown_38a37[0x38a47 - 0x38a37];
  unsigned int ticks;                   // +0x38a47
  char unknown_38a4b[0x38a51 - 0x38a4b];
  union {                               // +0x38a51
    unsigned char pauseFlags;
    Bits8 pauseBits;
  };
  char unknown_38a53[0x38d85 - 0x38a53];
  FrameTimers prof;                     // +0x38d85
  int profileBarsEnabled;               // +0x38dd5
  char unknown_38dd9[0x391c3 - 0x38dd9];
  int showBps;                          // +0x391c3
  char unknown_391c7[0x391e9 - 0x391c7];
  Mission* mapInfo;                     // +0x391e9
  char unknown_391ed[0x391f9 - 0x391ed];
  int fontComix;                        // +0x391f9
  char unknown_391fd[0x3923b - 0x391fd];
  union {                               // +0x3923b
    unsigned short flags_3923b;
    unsigned char flagsByte_3923b;
    Bits8 bits_3923b;
  };
};
#pragma pack(pop)

extern Game* g_game;


// Takes and returns floats.
static inline float Approach(float fcur, float ftarget)
{
  int cur = (int)fcur;
  int d = (int)ftarget - cur;
  if (d < 0) {
    d /= 8;
    if (d == 0)
      d = -1;
  }
  else if (d > 0) {
    d /= 8;
    if (d == 0)
      d = 1;
  }
  else
    d = 0;
  return (float)(d + cur);
}

// pl must be a parameter: a load through it then schedules above the bar copy.
static void DrawResourcePanel(Surface *ctx, PlayerState_00468cf0 *pl, Resources *res)
{
  OverlayRect bar, box;
  char text[32];
  int side = pl->info->bSideId;
  SideDef_00468cf0 *sideDef = &g_game->sideDefs[side];
  SetFont((int)sideDef->pFontData);
  GetFontHeight();
  byte *pal = &g_game->colors[0];
  SetTextColors(pal[0xf], GetTextKeyColor());
  int bx = 0x81;
  // do/while, not a for loop.
  do {
    ushort *gaf = (ushort *)GetGafFrame((int)g_game->sidePanelRows[side + (bx > 0x81) * 5], 0);
    BlitGafFrameAtOffset((int)ctx, (int)gaf, bx, 0);
    bx += *gaf;
  } while (bx < g_game->width);
  BlitSideLogoToRect((int)ctx, (int)pl, (int)&sideDef->rcLogo, 0);
  OverlayRect *r = &sideDef->rcEnergyBar;
  bar = *r;
  if (pl->flEnergyStorage > 0.0f) {
    bar.right = (int)((bar.right - bar.left) * res->metal / pl->flEnergyStorage + bar.left);
    FillRectangle((int)ctx, (int)&bar, sideDef->nEnergyColor);
    if (pl->flShareThresholdEnergy > 0.0f && pl->flEnergyAmount > pl->flShareThresholdEnergy) {
      box = *r;
      box.left = (int)((box.right - box.left) * pl->flShareThresholdEnergy / pl->flEnergyStorage + box.left);
      box.right = box.left + 2;
      FillRectangle((int)ctx, (int)&box, pal[0xc]);
    }
  }
  sprintf(text, "%d", (int)res->metal);
  DrawString((int)ctx, (int)text, sideDef->rcEnergyNum.left, sideDef->rcEnergyNum.top, -1);
  DrawString((int)ctx, (int)"0", sideDef->rcEnergy0.left, sideDef->rcEnergy0.top, -1);
  sprintf(text, "%d", (int)pl->flEnergyStorage);
  int w = GetTextWidth((int)sideDef->pFontData, (int)text);
  DrawString((int)ctx, (int)text, sideDef->rcEnergyMax.left - w, sideDef->rcEnergyMax.top, -1);
  if (res->metalIncome > 99999.0f)
    sprintf(text, "%dK", (int)res->metalIncome / 1000);
  else
    sprintf(text, "%d", (int)res->metalIncome);
  SetTextColors(pal[0xa], GetTextKeyColor());
  DrawString((int)ctx, (int)text, sideDef->rcEnergyProduced.left, sideDef->rcEnergyProduced.top, -1);
  if (res->metalUse < -99999.0f)
    sprintf(text, "%dK", (int)res->metalUse / 1000);
  else
    sprintf(text, "%d", abs((int)res->metalUse));
  SetTextColors(pal[0xc], GetTextKeyColor());
  DrawString((int)ctx, (int)text, sideDef->rcEnergyConsumed.left, sideDef->rcEnergyConsumed.top, -1);
  r = &sideDef->rcMetalBar;
  bar = *r;
  if (pl->flMetalStorage > 0.0f) {
    bar.right = (int)((bar.right - bar.left) * res->energy / pl->flMetalStorage + bar.left);
    FillRectangle((int)ctx, (int)&bar, sideDef->nMetalColor);
    if (pl->flShareThresholdMetal > 0.0f && pl->flMetalAmount > pl->flShareThresholdMetal) {
      box = *r;
      box.left = (int)((box.right - box.left) * pl->flShareThresholdMetal / pl->flMetalStorage + box.left);
      box.right = box.left + 2;
      FillRectangle((int)ctx, (int)&box, pal[0xc]);
    }
  }
  SetTextColors(pal[0xf], GetTextKeyColor());
  sprintf(text, "%d", (int)res->energy);
  DrawString((int)ctx, (int)text, sideDef->rcMetalNum.left, sideDef->rcMetalNum.top, -1);
  DrawString((int)ctx, (int)"0", sideDef->rcMetal0.left, sideDef->rcMetal0.top, -1);
  sprintf(text, "%d", (int)pl->flMetalStorage);
  w = GetTextWidth((int)sideDef->pFontData, (int)text);
  DrawString((int)ctx, (int)text, sideDef->rcMetalMax.left - w, sideDef->rcMetalMax.top, -1);
  sprintf(text, "%.1f", res->energyIncome);
  SetTextColors(pal[0xa], GetTextKeyColor());
  DrawString((int)ctx, (int)text, sideDef->rcMetalProduced.left, sideDef->rcMetalProduced.top, -1);
  sprintf(text, "%.1f", fabs(res->energyUse));
  SetTextColors(pal[0xc], GetTextKeyColor());
  DrawString((int)ctx, (int)text, sideDef->rcMetalConsumed.left, sideDef->rcMetalConsumed.top, -1);
}

static inline int ShowSelectBox(int drawObjects)
{
  if (drawObjects == 0)
    return 0;
  if (g_game->inputFlags & 8)
    return 1;
  if (g_game->orderMode == '\x0e')
    return PointInRect((int)&g_game->lim, g_game->cursorScreenX, g_game->cursorScreenY) != 0;
  return 0;
}

// Unused here: these declarations take the symbol ids that keep DrawBattleFrame
// matching (docs/c2-regalloc.md).
int GetScreenWidth();
int DrawFrameRate();
void DrawSoftwareCursor();

// FUNCTION: 0x468cf0
void __stdcall DrawBattleFrame(int param_1, int param_2)
{
  // Plain locals in this order (an escaping struct changes the frame layout);
  // the tail also needs these while loops and (int) casts.
  char debugText[80];
  int y2;
  Surface ctx;
  char gameTime[256];
  int y;
  byte idx;
  int cy;
  Resources res;
  MapGrid *mv;
  byte *colors;
  int cx;
  int x;
  PlayerState_00468cf0 *player;
  int i, k;

  cx = (g_game->width + 0x80) / 2;
  cy = g_game->height / 2;
  SetOffscreenSurface(g_game->screen);
  ctx = **(Surface **)&g_game->screen;
  colors = &g_game->colors[0];
  HideSoftwareCursor();
  ctx.SetClipRect(g_game->lim);
  ProfileMark(&g_game->prof, 8);
  DrawMapTiles((int)&ctx);
  DrawMapDebugOverlay((int)&ctx);
  x = g_game->field_2cac - g_game->scrollX + 0x80;
  // Compiler state, not meaning: reading viewY through this char* alias of
  // g_game is what makes MSVC subtract (h >> 1) first, as the original does.
  char *&game = *(char**)&g_game;
  int *viewY = &((Game *)game)->scrollY;
  y = g_game->field_2cb4 - (g_game->field_2cb0 >> 1) - *viewY + 0x20;
  if (g_game->debugMode == '\x02') {
    DrawLine((int)&ctx, x - 2, y, x + 2, y, colors[0xf]);
    DrawLine((int)&ctx, x, y - 2, x, y + 2, colors[0xf]);
  }
  ResetClipRect((int)&ctx);

  // resource bars
  {
    PlayerState_00468cf0 *pl = &g_game->players[g_game->playerIndex];
    res = g_game->resources;
    // owner is read through pl: gives the folded addresses.
    res.owner = pl->bSlotIndex;
    res.metal = Approach(res.metal, pl->flEnergyAmount);
    res.energy = Approach(res.energy, pl->flMetalAmount);
    res.maxMetal = pl->flEnergyStorage;
    res.maxEnergy = pl->flMetalStorage;
    if (res.metal > res.maxMetal)
      res.metal = res.maxMetal;
    if (res.energy > res.maxEnergy)
      res.energy = res.maxEnergy;
    if (pl->nDisplayTimer < g_game->ticks) {
      pl->nDisplayTimer += 0x1e;
      res.metalIncome = GetEnergyIncome((int)pl);
      res.metalUse = GetEnergyUsage((int)pl);
      res.energyIncome = GetMetalIncome((int)pl);
      res.energyUse = GetMetalUsage((int)pl);
    }
    if (memcmp((char*)&g_game->resources, &res, sizeof(res)) != 0) {
      g_game->resources = res;
      DrawResourcePanel(&ctx, pl, &res);
    }
  }
  DrawUnitInfoPanel((int)&ctx);
  DrawRadar((int)&ctx);
  ctx.SetClipRect(g_game->lim);
  ProfileMark(&g_game->prof, 3);

  // features and units on the visible part of the map
  idx = g_game->playerIndex;
  mv = &g_game->sortUnits;
  player = &g_game->players[idx];
  SetFont((int)g_game->sideDefs[player->info->bSideId].pFontData);
  SetTextColors(g_game->colors[15], GetTextKeyColor());
  {
    int vx = g_game->scrollX / 16, vy = g_game->scrollY / 16, h, w, x0, y0, skip;
    i = 0;
    while (i < mv->rows) {
      mv->cursor[i] = mv->buf + i * mv->stride;
      mv->count[i] = 0;
      i++;
    }
    // y0 is assigned before h, and x0 before w.
    y0 = vy - 16;
    h = mv->rows;
    if (y0 < 0) {
      skip = -y0;
      h -= skip;
      y0 = 0;
    }
    else
      skip = 0;
    if (h + y0 > mv->height - 1)
      h = mv->height - y0 - 1;
    x0 = vx - 10;
    w = mv->stride;
    if (x0 < 0) {
      w += x0;
      x0 = 0;
    }
    if (w + x0 > mv->width - 1)
      w = mv->width - x0 - 1;
    ushort *pIdx = g_game->visibleUnitIds;
    k = 0;
    while (k < g_game->count) {
      Unit_00468cf0 *unit = (Unit_00468cf0 *)(g_game->units + *pIdx * 0x118);
      int row = ((int)unit->field_74 - g_game->scrollY) / 16 + 0x10;
      if (row >= 0 && row < mv->rows) {
        Unit_00468cf0 ***pCur = &mv->cursor[row];
        mv->count[row]++;
        if (*pCur != 0) {
          **pCur = unit;
          (*pCur)++;
        }
      }
      k++;
      pIdx++;
    }
    DrawParticleList((int)&ctx, 0);
    DrawParticleList((int)&ctx, 1);
    DrawParticleList((int)&ctx, 2);
    // y = y0 here and x = x0 per row, indexed (width * y + x).
    y = y0;
    i = 0;
    while (i < h) {
      x = x0;
      Tile_00468cf0 *tile = mv->tiles + (mv->width * y + x);
      int c = 0;
      while (c < w) {
        tile->flags &= 0xfb;
        if (tile->feature < 0xfffb) {
          Feature_00468cf0 *feat = g_game->features + tile->feature;
          if (feat->field_fa < 10) {
            if ((feat->flags & 8) && ((tile->flags >> 3 & 0xf) != idx)) {
              if (IsFootprintVisible((int)player, x, y, feat->field_94, feat->field_96, tile->field_4))
                BlitFeatureGaf((int)&ctx, (int)tile, x, y);
            }
            else
              BlitFeatureGaf((int)&ctx, (int)tile, x, y);
          }
          else
            tile->flags |= 4;
        }
        c++;
        x++;
        tile++;
      }
      i++;
      y++;
    }
    DrawParticleList((int)&ctx, 3);
    DrawParticleList((int)&ctx, 4);
    for (i = 0; i < h; i++) {
      // row and y are computed inside the loop: y becomes the induction variable.
      int row = i + skip;
      y = y0 + i;
      Unit_00468cf0 **pUnit = mv->buf + row * mv->stride;
      for (k = 0; k < mv->count[row]; k++, pUnit++) {

        Unit_00468cf0 *u = *pUnit;
        if (u->flags.kind == 1 && param_1 != 0) {
          if (u->flags.b4)
            DrawSelectionBox((int)&ctx, (int)u);
          if (u->script != 0)
            DrawUnit((int)&ctx, (int)u);
        }
      }
      Tile_00468cf0 *tile = mv->tiles + (y * mv->width + x0);
      for (int c = 0; c < w; c++, tile++) {
        x = x0 + c;

        if (tile->flags & 4) {
          Feature_00468cf0 *feat = g_game->features + tile->feature;
          if ((feat->flags & 8) && ((tile->flags >> 3 & 0xf) != idx)) {
            if (IsFootprintVisible((int)player, x, y, (int)feat->field_94, feat->field_96, tile->field_4))
              BlitFeatureGaf((int)&ctx, (int)tile, x, y);
          }
          else
            BlitFeatureGaf((int)&ctx, (int)tile, x, y);
        }
      }
    }
  }
  DrawParticleList((int)&ctx, 5);
  if (param_1 != 0) {
    DrawParticleList((int)&ctx, 6);
    DrawProjectiles((int)&ctx);
    DrawExplosions((int)&ctx);
    DrawParticleList((int)&ctx, 7);
    for (i = 0; i < mv->rows; i++) {
      Unit_00468cf0 **pUnit = mv->buf + mv->stride * i;
      k = 0;
      while (k < mv->count[i]) {
        Unit_00468cf0 *u = *pUnit;
        if (u->flags.kind != 1) {
          if (u->flags.b4)
            DrawSelectionBox((int)&ctx, (int)u);
          if (u->script != 0)
            DrawUnit((int)&ctx, (int)u);
        }
        k++;
        pUnit++;
      }
    }
  }
  DrawParticleList((int)&ctx, 8);
  if (IsKeyDown(0xf9))
    DrawSelectedUnitOrderOverlays((int)&ctx, (int)&g_game->followUnit);

  // unit group numbers. Suspected original bug: the outer test lets a unit
  // with a group number (group) through when the 0x37f06 bit is clear, but the
  // inner test requires that bit for the number as well, so the position is
  // computed and nothing is drawn (0x469c4a to 0x469c9a).
  if (param_1 != 0) {
    ushort *pIdx = g_game->visibleUnitIds;
    for (k = 0; k < g_game->count; k++, pIdx++) {
      Unit_00468cf0 *unit = (Unit_00468cf0 *)(g_game->units + *pIdx * 0x118);
      if ((g_game->visualFlagsByte & 1) || unit->group != 0) {
        char str[2];
        str[1] = 0;
        x = unit->field_6c - g_game->scrollX + 0x80;
        y = unit->field_74 - g_game->scrollY - (unit->field_70 >> 1) + 0x20;
        if (g_game->visualFlagsByte & 1) {
          // The slot compares as a signed char, as the original does.
          if ((char)unit->player->bSlotIndex == (char)idx)
            DrawHitPointBar((int)&ctx, (int)unit, x, y + 10);
          if ((char)unit->player->bSlotIndex == (char)idx && unit->group != 0) {
            str[0] = (char)unit->group + '0';
            DrawString((int)&ctx, (int)str, x, y + 0xe, -1);
          }
        }
      }
    }
    DrawParticleList((int)&ctx, 9);
  }
  ProfileMark(&g_game->prof, 4);
  if ((g_game->flags_3923b & 1) && (g_game->flags_3923b & 2) && param_1 != 0)
    DrawSelectedGoal((int)&ctx, FindNextSelectedUnit(0, 0));
  if (param_1 != 0)
    DrawFogOfWar((int)&ctx);
  ProfileMark(&g_game->prof, 5);

  // selection box
  if (ShowSelectBox(param_1)) {
    OverlayRect box;
    int x1 = g_game->boxStartX - g_game->scrollX + 0x80;
    int y1 = g_game->boxStartZ - (g_game->boxStartHeight >> 1) - g_game->scrollY + 0x20;
    int x2 = g_game->boxEndX - g_game->scrollX + 0x80;
    int y2 = g_game->boxEndZ - (g_game->boxEndHeight >> 1) - g_game->scrollY + 0x20;
    int ci;
    if (g_game->orderMode == '\x0e')
      ci = ((g_game->inputFlags & 0x40) ? 6 : 0) + 4;
    else
      ci = 0xf;
    int c = colors[ci];
    int t;
    if (x2 < x1) {
      t = x1;
      x1 = x2;
      x2 = t;
    }
    if (y2 < y1) {
      t = y1;
      y1 = y2;
      y2 = t;
    }
    box.left = x1;
    box.top = y1;
    box.right = x2;
    box.bottom = y2;
    DrawRectangle((int)&ctx, (int)&box, c);
    box.left++;
    box.top++;
    box.right--;
    box.bottom--;
    if (g_game->orderMode == '\x0e')
      DrawRectangle((int)&ctx, (int)&box, c);
    else
      DrawRectangle((int)&ctx, (int)&box, *colors);
  }
  if (g_game->mapInfo->GetGameType() == 3 ||
      g_game->mapInfo->GetGameType() == 2) {
    ResetClipRect((int)&ctx);
    DrawScorePanel((int)&ctx);
    ctx.SetClipRect(g_game->lim);
  }
  DrawStatusPanel((int)&ctx);
  if (g_game->showBps != 0)
    DrawNetworkStats((int)&ctx);
  if (param_1 != 0)
    DrawMessages((int)&ctx);
  if ((g_game->flagsByte_3923b & 2) && param_1 != 0) {
    SetTextColors(colors[0xf], GetTextKeyColor());
    SetFont(g_game->fontComix);
    int ty = GetFontHeight() * 3 - 10;
    sprintf(debugText, "FRATE: %d\n", GetFrameRate());
    DrawString((int)&ctx, (int)debugText, 0x83, ty, -1);
    DrawString((int)&ctx, (int)"[Release]", 0xbc, ty, -1);
    sprintf(debugText, "MODE %s INFO %s", g_game->bits_3923b.b1 ? "DEBUG" : "NORMAL",
            g_game->bits_3923b.b0 ? "ON" : "OFF");
    DrawString((int)&ctx, (int)debugText, 0x1ee, ty, -1);
    ty += GetFontHeight();
    if (g_game->flags_2a44 & 1) {
      FormatNetStats((int)debugText);
      DrawString((int)&ctx, (int)debugText, 0xbc, ty, -1);
    }
  }
  if (g_game->pauseFlags & 1)
    DrawFrame((int)&ctx, GetGafFrame(g_game->igpaused, 0), cx, cy);
  if ((g_game->players[g_game->localPlayer].info->gameOptFlags & 0x40) == 0) {
    if (g_game->bits_3923b.b5)
      DrawFrame((int)&ctx, GetGafFrame(g_game->igvictory, 0), cx, cy);
    if (g_game->bits_3923b.b6)
      DrawFrame((int)&ctx, GetGafFrame(g_game->igdefeat, 0), cx, cy);
  }
  if (g_game->bits_37f2f.b6) {
    uint ticks = g_game->ticks;
    uint hours = ticks / 108000;
    int rest = ticks - hours * 108000;
    int minutes = rest / 1800;
    int seconds = (rest - minutes * 1800) / 30;
    sprintf(gameTime, "%s : %02d:%02d:%02d", (char *)Translate((int)"Game Time"), hours, minutes, seconds);
    SetTextColors(colors[0xf], GetTextKeyColor());
    DrawString((int)&ctx, (int)gameTime, 0x82, -0x22 - GetFontHeight() + GetScreenHeight(), -1);
  }
  if (g_game->pauseBits.b1)
    DrawFrame((int)&ctx, GetGafFrame(g_game->cursorHourglass, 0), g_game->width - 0x10, g_game->height - 0x50);
  ResetClipRect((int)&ctx);
  BlitMenuLayers((int)&g_game->menu, (int)&ctx, (int)&g_game->lim);
  if (g_game->profileBarsEnabled != 0 && param_1 != 0) {
    DrawProfileBarLine((int)&ctx, (int)"Network", 0);
    DrawProfileBarLine((int)&ctx, (int)"Units", 1);
    DrawProfileBarLine((int)&ctx, (int)"Logic", 2);
    DrawProfileBarLine((int)&ctx, (int)"Render Static", 3);
    DrawProfileBarLine((int)&ctx, (int)"Render Stuff", 4);
    DrawProfileBarLine((int)&ctx, (int)"Render Fog", 5);
    DrawProfileBarLine((int)&ctx, (int)"SFX", 6);
    DrawProfileBarLine((int)&ctx, (int)"Weapon", 7);
    DrawProfileBarLine((int)&ctx, (int)"Misc", 8);
  }
  DrawOptionsScrollBar((int)&ctx);
  ShowSoftwareCursor();
  if (param_1 != 0 && param_2 != 0)
    FlipScreen();
  g_game->prof.AccumulateProfileTime(3);
}
