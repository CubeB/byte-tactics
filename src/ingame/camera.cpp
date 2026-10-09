// Decompiled by DeepSeek V4.1 Flash, deepseek-v4.1-flash, Opus, Sonnet and Haiku. Names are provisional.

#pragma pack(push, 1)
struct Rect_0041d0f0 {
    int x;                             // +0x0
    int y;                             // +0x4
    int unknown_8[4];
};

struct Entry_0041d1f0 {
    int type;                          // +0x0
    int id;                            // +0x4
    short x;                           // +0x8
    short z;                           // +0xa
};

class Class_0041d1f0_net {
public:
    char unknown_0[0xdb4];
    Entry_0041d1f0* entries;           // +0xdb4
    int entry_count;                   // +0xdb8
};

struct Game {
    char unknown_0[0x2c76];
    Rect_0041d0f0 view;                // +0x2c76
    char unknown_2c8e[0x1422b - 0x2c8e];
    int world_w;                       // +0x1422b
    int world_h;                       // +0x1422f
    char unknown_14233[0x14281 - 0x14233];
    unsigned short mapFlags;           // +0x14281
    char unknown_14283[0x142e7 - 0x14283];
    short origin_x;                    // +0x142e7
    short origin_y;                    // +0x142e9
    short screen_w;                    // +0x142eb
    short screen_h;                    // +0x142ed
    char unknown_142ef[0x142f1 - 0x142ef];
    unsigned char flags_142f1;         // +0x142f1
    char unknown_142f2;
    int value_142f3;                   // +0x142f3
    int value_142f7;                   // +0x142f7
    int xs[4];                         // +0x142fb
    int ys[4];                         // +0x1430b
    unsigned char valid[4];            // +0x1431b
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    int x2;                            // +0x14327
    int y2;                            // +0x1432b
    char unknown_1432f[0x1434b - 0x1432f];
    short value_1434b;                 // +0x1434b
    char unknown_1434d[0x37e37 - 0x1434d];
    int viewWidth;                     // +0x37e37
    int viewHeight;                    // +0x37e3b
    char unknown_37e3f[0x391e9 - 0x37e3f];
    Class_0041d1f0_net* net;           // +0x391e9
};
#pragma pack(pop)

#include "../util/hapi_bank.h"

// GLOBAL: 0x511de8
extern Game* g_game;

extern char g_cameraAccount[]; // "Camera"
extern char g_cameraZPosition[]; // "Z Position"
extern char g_cameraXPosition[]; // "X Position"

extern int g_commandLineUnusedFlagL;
extern int g_cdBypassDriveScan;

void ClampCameraPosition(void);

static inline void SetPos(int x, int y)
{
    g_game->scrollX = x;
    g_game->scrollY = y;
}

// Centres the camera on the screen position stored at +0x2c76, then clamps it.
// FUNCTION: 0x41d0f0
void CenterCameraOnRadarClick()
{
    Rect_0041d0f0 r = g_game->view;
    int y = g_game->world_h * (r.y - g_game->origin_y) / g_game->screen_h - g_game->viewHeight / 2;
    int x = g_game->world_w * (r.x - g_game->origin_x) / g_game->screen_w - g_game->viewWidth / 2;
    g_game->scrollX = x;
    g_game->scrollY = y;
    g_game->flags_142f1 |= 2;
    ClampCameraPosition();
    g_game->x2 = g_game->scrollX;
    g_game->y2 = g_game->scrollY;
    g_game->mapFlags &= 0xfff7;
    g_game->value_1434b = 0;
    g_game->value_142f3 = 0;
    g_game->value_142f7 = 0;
}

// FUNCTION: 0x41d1f0
void CenterCameraOnStartPosition()
{
    int i = 0;
    Entry_0041d1f0* e = g_game->net->entries;
    int count = g_game->net->entry_count;
    for (; i < count; i++, e++) {
        if (e->type == 1 && e->id == 0) {
            SetPos(e->x - g_game->viewWidth / 2, e->z - g_game->viewHeight / 2);
            g_game->flags_142f1 |= 2;
            ClampCameraPosition();
            g_game->x2 = g_game->scrollX;
            g_game->y2 = g_game->scrollY;
            g_game->mapFlags &= 0xfff7;
            return;
        }
    }
}

// Reads the camera X and Z positions from the "Camera" section of a parsed
// text file (defaults to the current position), then clamps the view.
// FUNCTION: 0x41d2b0
void __stdcall ReadCameraPosition(HapiBank* file)
{
    file->OpenAccount(g_cameraAccount);
    int z = file->GetIntegerItem(g_cameraZPosition, g_game->scrollY);
    int x = file->GetIntegerItem(g_cameraXPosition, g_game->scrollX);
    g_game->scrollX = x;
    g_game->scrollY = z;
    g_game->flags_142f1 |= 2;
    ClampCameraPosition();
    g_game->x2 = g_game->scrollX;
    g_game->y2 = g_game->scrollY;
    g_game->mapFlags &= 0xfff7;
}

// Writes the camera position to a section ("Camera", "X Position", "Z Position").
// FUNCTION: 0x41d360
void __stdcall WriteCameraPosition(HapiBank* file)
{
    file->OpenAccount(g_cameraAccount);
    file->SetIntegerItem(g_cameraXPosition, g_game->scrollX);
    file->SetIntegerItem(g_cameraZPosition, g_game->scrollY);
}

// FUNCTION: 0x41d3b0
void __stdcall SaveCameraPosition(int param_1)
{
    g_game->xs[param_1] = g_game->scrollX;
    g_game->ys[param_1] = g_game->scrollY;
    g_game->valid[param_1] = 1;
}

// FUNCTION: 0x41d3f0
void __stdcall RestoreCameraPosition(int index)
{
    if (g_game->valid != 0) {
        g_game->value_1434b = 0;
        g_game->value_142f3 = 0;
        g_game->value_142f7 = 0;
        SetPos(g_game->xs[index], g_game->ys[index]);
        g_game->flags_142f1 |= 2;
        ClampCameraPosition();
        g_game->x2 = g_game->scrollX;
        g_game->y2 = g_game->scrollY;
        g_game->mapFlags &= 0xfff7;
    }
}

// FUNCTION: 0x41d4a0
void __stdcall SetCommandLineUnusedFlagL(int val)
{
    g_commandLineUnusedFlagL = val;
}

// FUNCTION: 0x41d4b0
void __stdcall SetBypassDriveScan(int val)
{
    g_cdBypassDriveScan = val;
}
