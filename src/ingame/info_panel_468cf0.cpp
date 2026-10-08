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
extern char* g_game;
unsigned long GetMilliseconds();
int __stdcall FormatNetStats(int);
int __stdcall DrawSelectedGoal(int,int);
int __stdcall DrawMapDebugOverlay(int);
int __stdcall DrawExplosions(int);
struct Mission { int GetGameType(); };
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

struct MapGrid { int *buf; int **cursor; ushort *count; char pad1[0x2c]; int width; int height; char pad2[0x10]; int stride; int rows; char pad3[0x34]; int tiles; };
struct Bits8 { ushort b0:1; ushort b1:1; ushort b2:1; ushort b3:1; ushort b4:1; ushort b5:1; ushort b6:1; ushort b7:1; };
struct UnitFlags { uint kind:2; uint b2:1; uint b3:1; uint b4:1; uint b5:1; uint b6:1; uint b7:1; };

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
static void DrawResourcePanel(Surface *ctx, int pl, Resources *res)
{
  OverlayRect bar, box;
  char text[32];
  int side = *(byte *)(*(int *)(pl + 0x27) + 0x95);
  int sd = (int)g_game + 0x37f3d + side * 0x232;
  SetFont(*(int *)(sd + 0x22e));
  GetFontHeight();
  byte *pal = (byte *)(g_game + 0xdcb);
  SetTextColors(pal[0xf], GetTextKeyColor());
  int bx = 0x81;
  // do/while, not a for loop.
  do {
    ushort *gaf = (ushort *)GetGafFrame(*(int *)(g_game + 0x1481f + (side + (bx > 0x81) * 5) * 4), 0);
    BlitGafFrameAtOffset((int)ctx, (int)gaf, bx, 0);
    bx += *gaf;
  } while (bx < *(int *)(g_game + 0x37e1f));
  BlitSideLogoToRect((int)ctx, pl, sd + 0x42, 0);
  OverlayRect *r = (OverlayRect *)(sd + 0x52);
  bar = *r;
  if (*(float *)(pl + 0xa4) > 0.0f) {
    bar.right = (int)((bar.right - bar.left) * res->metal / *(float *)(pl + 0xa4) + bar.left);
    FillRectangle((int)ctx, (int)&bar, *(int *)(sd + 0x222));
    if (*(float *)(pl + 0xe8) > 0.0f && *(float *)(pl + 0x8c) > *(float *)(pl + 0xe8)) {
      box = *r;
      box.left = (int)((box.right - box.left) * *(float *)(pl + 0xe8) / *(float *)(pl + 0xa4) + box.left);
      box.right = box.left + 2;
      FillRectangle((int)ctx, (int)&box, pal[0xc]);
    }
  }
  sprintf(text, "%d", (int)res->metal);
  DrawString((int)ctx, (int)text, *(int *)(sd + 0x62), *(int *)(sd + 0x66), -1);
  DrawString((int)ctx, (int)"0", *(int *)(sd + 0xd2), *(int *)(sd + 0xd6), -1);
  sprintf(text, "%d", (int)*(float *)(pl + 0xa4));
  int w = GetTextWidth(*(int *)(sd + 0x22e), (int)text);
  DrawString((int)ctx, (int)text, *(int *)(sd + 0xb2) - w, *(int *)(sd + 0xb6), -1);
  if (res->metalIncome > 99999.0f)
    sprintf(text, "%dK", (int)res->metalIncome / 1000);
  else
    sprintf(text, "%d", (int)res->metalIncome);
  SetTextColors(pal[0xa], GetTextKeyColor());
  DrawString((int)ctx, (int)text, *(int *)(sd + 0xf2), *(int *)(sd + 0xf6), -1);
  if (res->metalUse < -99999.0f)
    sprintf(text, "%dK", (int)res->metalUse / 1000);
  else
    sprintf(text, "%d", abs((int)res->metalUse));
  SetTextColors(pal[0xc], GetTextKeyColor());
  DrawString((int)ctx, (int)text, *(int *)(sd + 0x102), *(int *)(sd + 0x106), -1);
  r = (OverlayRect *)(sd + 0x72);
  bar = *r;
  if (*(float *)(pl + 0xa8) > 0.0f) {
    bar.right = (int)((bar.right - bar.left) * res->energy / *(float *)(pl + 0xa8) + bar.left);
    FillRectangle((int)ctx, (int)&bar, *(int *)(sd + 0x226));
    if (*(float *)(pl + 0xe4) > 0.0f && *(float *)(pl + 0x98) > *(float *)(pl + 0xe4)) {
      box = *r;
      box.left = (int)((box.right - box.left) * *(float *)(pl + 0xe4) / *(float *)(pl + 0xa8) + box.left);
      box.right = box.left + 2;
      FillRectangle((int)ctx, (int)&box, pal[0xc]);
    }
  }
  SetTextColors(pal[0xf], GetTextKeyColor());
  sprintf(text, "%d", (int)res->energy);
  DrawString((int)ctx, (int)text, *(int *)(sd + 0x82), *(int *)(sd + 0x86), -1);
  DrawString((int)ctx, (int)"0", *(int *)(sd + 0xe2), *(int *)(sd + 0xe6), -1);
  sprintf(text, "%d", (int)*(float *)(pl + 0xa8));
  w = GetTextWidth(*(int *)(sd + 0x22e), (int)text);
  DrawString((int)ctx, (int)text, *(int *)(sd + 0xc2) - w, *(int *)(sd + 0xc6), -1);
  sprintf(text, "%.1f", res->energyIncome);
  SetTextColors(pal[0xa], GetTextKeyColor());
  DrawString((int)ctx, (int)text, *(int *)(sd + 0x112), *(int *)(sd + 0x116), -1);
  sprintf(text, "%.1f", fabs(res->energyUse));
  SetTextColors(pal[0xc], GetTextKeyColor());
  DrawString((int)ctx, (int)text, *(int *)(sd + 0x122), *(int *)(sd + 0x126), -1);
}

static inline int ShowSelectBox(int drawObjects)
{
  if (drawObjects == 0)
    return 0;
  if (*(byte *)(g_game + 0x2cc6) & 8)
    return 1;
  if (*(char *)(g_game + 0x2cc3) == '\x0e')
    return PointInRect((int)(g_game + 0x37e27), *(int *)(g_game + 0x2c76), *(int *)(g_game + 0x2c7a)) != 0;
  return 0;
}

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
  int x, player;
  int i, k;

  cx = (*(int *)(g_game + 0x37e1f) + 0x80) / 2;
  cy = *(int *)(g_game + 0x37e23) / 2;
  SetOffscreenSurface(*(int *)(g_game + 0x37e1b));
  ctx = **(Surface **)(g_game + 0x37e1b);
  colors = (byte *)(g_game + 0xdcb);
  HideSoftwareCursor();
  ctx.SetClipRect(*(OverlayRect *)(g_game + 0x37e27));
  ProfileMark((FrameTimers *)(g_game + 0x38d85), 8);
  DrawMapTiles((int)&ctx);
  DrawMapDebugOverlay((int)&ctx);
  x = *(short *)(g_game + 0x2cac) - *(int *)(g_game + 0x1431f) + 0x80;
  // Compiler state, not meaning: reading viewY through this alias of g_game
  // is what makes MSVC subtract (h >> 1) first, as the original does.
  char *&game = g_game;
  int *viewY = (int *)(game + 0x14323);
  y = *(short *)(g_game + 0x2cb4) - (*(short *)(g_game + 0x2cb0) >> 1) - *viewY + 0x20;
  if (*(char *)(g_game + 0x14280) == '\x02') {
    DrawLine((int)&ctx, x - 2, y, x + 2, y, colors[0xf]);
    DrawLine((int)&ctx, x, y - 2, x, y + 2, colors[0xf]);
  }
  ResetClipRect((int)&ctx);

  // resource bars
  {
    int pl = (int)g_game + *(byte *)(g_game + 0x2a43) * 0x14b + 0x1b63;
    res = *(Resources *)(g_game + 0x37e3f);
    // owner is read through pl: gives the folded addresses.
    res.owner = *(char *)(pl + 0x146);
    res.metal = Approach(res.metal, *(float *)(pl + 0x8c));
    res.energy = Approach(res.energy, *(float *)(pl + 0x98));
    res.maxMetal = *(float *)(pl + 0xa4);
    res.maxEnergy = *(float *)(pl + 0xa8);
    if (res.metal > res.maxMetal)
      res.metal = res.maxMetal;
    if (res.energy > res.maxEnergy)
      res.energy = res.maxEnergy;
    if (*(uint *)(pl + 0xf8) < *(uint *)(g_game + 0x38a47)) {
      *(uint *)(pl + 0xf8) += 0x1e;
      res.metalIncome = GetEnergyIncome(pl);
      res.metalUse = GetEnergyUsage(pl);
      res.energyIncome = GetMetalIncome(pl);
      res.energyUse = GetMetalUsage(pl);
    }
    if (memcmp(g_game + 0x37e3f, &res, sizeof(res)) != 0) {
      *(Resources *)(g_game + 0x37e3f) = res;
      DrawResourcePanel(&ctx, pl, &res);
    }
  }
  DrawUnitInfoPanel((int)&ctx);
  DrawRadar((int)&ctx);
  ctx.SetClipRect(*(OverlayRect *)(g_game + 0x37e27));
  ProfileMark((FrameTimers *)(g_game + 0x38d85), 3);

  // features and units on the visible part of the map
  idx = *(byte *)(g_game + 0x2a43);
  mv = (MapGrid *)(g_game + 0x141fb);
  player = (int)g_game + idx * 0x14b + 0x1b63;
  SetFont(*(int *)(g_game + 0x3816b + *(byte *)(*(int *)(player + 0x27) + 0x95) * 0x232));
  SetTextColors(*(byte *)(g_game + 0xdda), GetTextKeyColor());
  {
    int vx = *(int *)(g_game + 0x1431f) / 16, vy = *(int *)(g_game + 0x14323) / 16, h, w, x0, y0, skip;
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
    ushort *pIdx = *(ushort **)(g_game + 0x1435f);
    k = 0;
    while (k < *(int *)(g_game + 0x14367)) {
      int unit = *(int *)(g_game + 0x14357) + *pIdx * 0x118;
      int row = ((int)*(short *)(unit + 0x74) - *(int *)(g_game + 0x14323)) / 16 + 0x10;
      if (row >= 0 && row < mv->rows) {
        int **pCur = &mv->cursor[row];
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
      int tile = (mv->width * y + x) * 0xd + mv->tiles;
      int c = 0;
      while (c < w) {
        *(byte *)(tile + 0xc) &= 0xfb;
        if (*(ushort *)(tile + 8) < 0xfffb) {
          int feat = *(int *)(g_game + 0x1426f) + *(ushort *)(tile + 8) * 0x100;
          if (*(byte *)(feat + 0xfa) < 10) {
            if ((*(byte *)(feat + 0xff) & 8) && ((*(byte *)(tile + 0xc) >> 3 & 0xf) != idx)) {
              if (IsFootprintVisible(player, x, y, *(short *)(feat + 0x94), *(short *)(feat + 0x96), *(byte *)(tile + 4)))
                BlitFeatureGaf((int)&ctx, tile, x, y);
            }
            else
              BlitFeatureGaf((int)&ctx, tile, x, y);
          }
          else
            *(byte *)(tile + 0xc) |= 4;
        }
        c++;
        x++;
        tile += 0xd;
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
      int *pUnit = mv->buf + row * mv->stride;
      for (k = 0; k < mv->count[row]; k++, pUnit++) {

        int u = *pUnit;
        if (((UnitFlags *)(u + 0x110))->kind == 1 && param_1 != 0) {
          if (((UnitFlags *)(u + 0x110))->b4)
            DrawSelectionBox((int)&ctx, u);
          if (*(int *)(u + 0x9a) != 0)
            DrawUnit((int)&ctx, u);
        }
      }
      int tile = (y * mv->width + x0) * 0xd + mv->tiles;
      for (int c = 0; c < w; c++, tile += 0xd) {
        x = x0 + c;

        if (*(byte *)(tile + 0xc) & 4) {
          int feat = *(int *)(g_game + 0x1426f) + *(ushort *)(tile + 8) * 0x100;
          if ((*(byte *)(feat + 0xff) & 8) && ((*(byte *)(tile + 0xc) >> 3 & 0xf) != idx)) {
            if (IsFootprintVisible(player, x, y, (int)*(short *)(feat + 0x94), *(short *)(feat + 0x96), *(byte *)(tile + 4)))
              BlitFeatureGaf((int)&ctx, tile, x, y);
          }
          else
            BlitFeatureGaf((int)&ctx, tile, x, y);
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
      int *pUnit = mv->buf + mv->stride * i;
      k = 0;
      while (k < mv->count[i]) {
        int u = *pUnit;
        if (((UnitFlags *)(u + 0x110))->kind != 1) {
          if (((UnitFlags *)(u + 0x110))->b4)
            DrawSelectionBox((int)&ctx, u);
          if (*(int *)(u + 0x9a) != 0)
            DrawUnit((int)&ctx, u);
        }
        k++;
        pUnit++;
      }
    }
  }
  DrawParticleList((int)&ctx, 8);
  if (IsKeyDown(0xf9))
    DrawSelectedUnitOrderOverlays((int)&ctx, (int)(g_game + 0x142f3));

  // unit group numbers. Suspected original bug: the outer test lets a unit
  // with a group number (+0xac) through when the 0x37f06 bit is clear, but the
  // inner test requires that bit for the number as well, so the position is
  // computed and nothing is drawn (0x469c4a to 0x469c9a).
  if (param_1 != 0) {
    ushort *pIdx = *(ushort **)(g_game + 0x1435f);
    for (k = 0; k < *(int *)(g_game + 0x14367); k++, pIdx++) {
      int unit = *(int *)(g_game + 0x14357) + *pIdx * 0x118;
      if ((*(byte *)(g_game + 0x37f06) & 1) || *(int *)(unit + 0xac) != 0) {
        char str[2];
        str[1] = 0;
        x = *(short *)(unit + 0x6c) - *(int *)(g_game + 0x1431f) + 0x80;
        y = *(short *)(unit + 0x74) - *(int *)(g_game + 0x14323) - (*(short *)(unit + 0x70) >> 1) + 0x20;
        if (*(byte *)(g_game + 0x37f06) & 1) {
          if (*(char *)(*(int *)(unit + 0x96) + 0x146) == (char)idx)
            DrawHitPointBar((int)&ctx, unit, x, y + 10);
          if (*(char *)(*(int *)(unit + 0x96) + 0x146) == (char)idx && *(int *)(unit + 0xac) != 0) {
            str[0] = *(char *)(unit + 0xac) + '0';
            DrawString((int)&ctx, (int)str, x, y + 0xe, -1);
          }
        }
      }
    }
    DrawParticleList((int)&ctx, 9);
  }
  ProfileMark((FrameTimers *)(g_game + 0x38d85), 4);
  if ((*(ushort *)(g_game + 0x3923b) & 1) && (*(ushort *)(g_game + 0x3923b) & 2) && param_1 != 0)
    DrawSelectedGoal((int)&ctx, FindNextSelectedUnit(0, 0));
  if (param_1 != 0)
    DrawFogOfWar((int)&ctx);
  ProfileMark((FrameTimers *)(g_game + 0x38d85), 5);

  // selection box
  if (ShowSelectBox(param_1)) {
    OverlayRect box;
    int x1 = *(int *)(g_game + 0x2c92) - *(int *)(g_game + 0x1431f) + 0x80;
    int y1 = *(int *)(g_game + 0x2c9a) - (*(int *)(g_game + 0x2c96) >> 1) - *(int *)(g_game + 0x14323) + 0x20;
    int x2 = *(int *)(g_game + 0x2c9e) - *(int *)(g_game + 0x1431f) + 0x80;
    int y2 = *(int *)(g_game + 0x2ca6) - (*(int *)(g_game + 0x2ca2) >> 1) - *(int *)(g_game + 0x14323) + 0x20;
    int ci;
    if (*(char *)(g_game + 0x2cc3) == '\x0e')
      ci = ((*(byte *)(g_game + 0x2cc6) & 0x40) ? 6 : 0) + 4;
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
    if (*(char *)(g_game + 0x2cc3) == '\x0e')
      DrawRectangle((int)&ctx, (int)&box, c);
    else
      DrawRectangle((int)&ctx, (int)&box, *colors);
  }
  if ((*(Mission **)(g_game + 0x391e9))->GetGameType() == 3 ||
      (*(Mission **)(g_game + 0x391e9))->GetGameType() == 2) {
    ResetClipRect((int)&ctx);
    DrawScorePanel((int)&ctx);
    ctx.SetClipRect(*(OverlayRect *)(g_game + 0x37e27));
  }
  DrawStatusPanel((int)&ctx);
  if (*(int *)(g_game + 0x391c3) != 0)
    DrawNetworkStats((int)&ctx);
  if (param_1 != 0)
    DrawMessages((int)&ctx);
  if ((*(byte *)(g_game + 0x3923b) & 2) && param_1 != 0) {
    SetTextColors(colors[0xf], GetTextKeyColor());
    SetFont(*(int *)(g_game + 0x391f9));
    int ty = GetFontHeight() * 3 - 10;
    sprintf(debugText, "FRATE: %d\n", GetFrameRate());
    DrawString((int)&ctx, (int)debugText, 0x83, ty, -1);
    DrawString((int)&ctx, (int)"[Release]", 0xbc, ty, -1);
    sprintf(debugText, "MODE %s INFO %s", ((Bits8 *)(g_game + 0x3923b))->b1 ? "DEBUG" : "NORMAL",
            ((Bits8 *)(g_game + 0x3923b))->b0 ? "ON" : "OFF");
    DrawString((int)&ctx, (int)debugText, 0x1ee, ty, -1);
    ty += GetFontHeight();
    if (*(byte *)(g_game + 0x2a44) & 1) {
      FormatNetStats((int)debugText);
      DrawString((int)&ctx, (int)debugText, 0xbc, ty, -1);
    }
  }
  if (*(byte *)(g_game + 0x38a51) & 1)
    DrawFrame((int)&ctx, GetGafFrame(*(int *)(g_game + 0x1481b), 0), cx, cy);
  if ((*(byte *)(*(int *)(g_game + *(byte *)(g_game + 0x2a42) * 0x14b + 0x1b8a) + 0x9b) & 0x40) == 0) {
    if (((Bits8 *)(g_game + 0x3923b))->b5)
      DrawFrame((int)&ctx, GetGafFrame(*(int *)(g_game + 0x14813), 0), cx, cy);
    if (((Bits8 *)(g_game + 0x3923b))->b6)
      DrawFrame((int)&ctx, GetGafFrame(*(int *)(g_game + 0x14817), 0), cx, cy);
  }
  if (((Bits8 *)(g_game + 0x37f2f))->b6) {
    uint ticks = *(uint *)(g_game + 0x38a47);
    uint hours = ticks / 108000;
    int rest = ticks - hours * 108000;
    int minutes = rest / 1800;
    int seconds = (rest - minutes * 1800) / 30;
    sprintf(gameTime, "%s : %02d:%02d:%02d", (char *)Translate((int)"Game Time"), hours, minutes, seconds);
    SetTextColors(colors[0xf], GetTextKeyColor());
    DrawString((int)&ctx, (int)gameTime, 0x82, -0x22 - GetFontHeight() + GetScreenHeight(), -1);
  }
  if (((Bits8 *)(g_game + 0x38a51))->b1)
    DrawFrame((int)&ctx, GetGafFrame(*(int *)(g_game + 0x148cf), 0), *(int *)(g_game + 0x37e1f) - 0x10, *(int *)(g_game + 0x37e23) - 0x50);
  ResetClipRect((int)&ctx);
  BlitMenuLayers((int)(g_game + 0x519), (int)&ctx, (int)(g_game + 0x37e27));
  if (*(int *)(g_game + 0x38dd5) != 0 && param_1 != 0) {
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
  ((FrameTimers *)(g_game + 0x38d85))->AccumulateProfileTime(3);
}
