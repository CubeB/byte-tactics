// Decompiled by Opus, DeepSeek V4.1 Flash, Claude Opus 5.5, deepseek-v4.1-flash and Haiku. Names are provisional.
// The in-game control panel's second part (0x41bd10 to 0x41ce90): the damage
// and build progress helpers, the build menu paging, the order-mode key
// handling, the commander lookup, the camera position, follow and scroll
// state, and the mouse and edge scrolling. 0x41bde0 keeps its own file: the
// merged file's include set and prelude move its sub-object pointer from esi
// to edx.
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)

struct Vec3 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

// The unit's type, the build menu's entry: the same 0x249-byte table part 1
// of the module declares in full.
struct UnitType {
    char unknown_0[0x186];
    float energyCost;                  // +0x186
    char unknown_18a[0x1ea - 0x18a];
    int buildTime;                     // +0x1ea
    char unknown_1ee[0x1fa - 0x1ee];
    unsigned int maxHp;                // +0x1fa
    char unknown_1fe[0x22e - 0x1fe];
    unsigned char field_22e;           // +0x22e
};

// The unit's resource account at +0xbc; its methods are 0x401180 and up.
class UnitResources {
public:
    int RequestEnergy(UnitResources* r, float amount);
};

struct UnitFlagsBits {
    unsigned int unknown_0 : 4;        // +0x110 bits 0-3
    unsigned int selected : 1;         // bit 4
    unsigned int unknown_5 : 6;        // bits 5-10
    unsigned int cloak : 1;            // bit 11
    unsigned int unknown_12 : 6;       // bits 12-17
    unsigned int moveOrder : 2;        // bits 18-19
    unsigned int fireOrder : 2;        // bits 20-21
    unsigned int buildPage : 1;        // bit 22
    unsigned int page : 3;             // bits 23-25
    unsigned int unknown_26 : 3;       // bits 26-28
    unsigned int flag_29 : 1;          // bit 29
    unsigned int unknown_30 : 2;       // bits 30-31
};

// lo is the byte view 0x41c110 tests; the bitfields are 0x41c060's view of
// the same word.
union UnitFlags {
    unsigned int raw;
    unsigned char lo;
    UnitFlagsBits bits;
};

struct Unit {
    char unknown_0[0x6a];
    Vec3 pos;                          // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType* type;                    // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    unsigned short field_a8;           // +0xa8
    char unknown_aa[0xbc - 0xaa];
    UnitResources store;               // +0xbc
    char unknown_bd[0xff - 0xbd];
    unsigned char field_ff;            // +0xff
    char unknown_100[0x108 - 0x100];
    short hp;                          // +0x108
    char unknown_10a[0x110 - 0x10a];
    UnitFlags flags;                   // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player {
    char unknown_0[0x67];
    Unit* units;                       // +0x67
    Unit* unitsEnd;                    // +0x6b (last unit, inclusive)
    char unknown_6f[0x14b - 0x6f];
};

struct Follow_0041ca10 {
    int unknown_0;                     // +0x0
    Vec3 pos;                          // +0x4
};

struct OrdersBits {
    unsigned short unknown_0 : 1;      // +0x37ebe bit 0
    unsigned short refresh : 1;        // bit 1
    unsigned short unknown_2 : 2;      // bits 2-3
    unsigned short flag_4 : 1;         // bit 4
    unsigned short unknown_5 : 2;      // bits 5-6
    unsigned short next : 1;           // bit 7
    unsigned short prev : 1;           // bit 8
    unsigned short orders : 1;         // bit 9
    unsigned short build : 1;          // bit 10
    unsigned short unknown_11 : 1;     // bit 11
    unsigned short fireOrder : 3;      // bits 12-14
    unsigned short unknown_15 : 1;     // bit 15
    unsigned short moveOrder : 3;      // +0x37ec0 bits 0-2
    unsigned short cloak : 2;          // bits 3-4
    unsigned short onOff : 2;          // bits 5-6
    unsigned short canMove : 1;        // bit 7
    unsigned short canStop : 1;        // bit 8
    unsigned short canAttack : 1;      // bit 9
    unsigned short canDefend : 1;      // bit 10
    unsigned short canPatrol : 1;      // bit 11
    unsigned short canLoad : 1;        // bit 12
    unsigned short canReclaim : 1;     // bit 13
    unsigned short canCapture : 1;     // bit 14
    unsigned short canRepair : 1;      // bit 15
    unsigned short canBlast : 1;       // +0x37ec2 bit 0
    unsigned short unknown_1 : 15;
};

// The byte view is the one 0x41bde0, 0x41bf10 and 0x41c060 use to set the
// redraw flag; 0x41c110 sets the same bit through raw, and 0x41c180 through
// bits.flag_4 (the bitfield write keeps the original's separate tail).
union Orders {
    unsigned short raw;
    unsigned char lo;
    OrdersBits bits;
};

// The camera flags word at +0x142f1: a byte in 0x41c4c0, 0x41c7c0, 0x41c8e0
// and 0x41ce90, a word in 0x41ca10 and 0x41cd50.
union CameraFlags {
    unsigned short raw;
    unsigned char lo;
};

struct Rect {
    int x;                             // +0x0
    int y;                             // +0x4
    int flags;                         // +0x8
    int unknown_c[3];                  // +0xc
};

struct CursorState {
    Rect rect;                         // +0x0
    int flag;                          // +0x18
    POINT center;                      // +0x1c
    POINT savedScroll;                 // +0x24
};

struct Mouse_0041ce90 {
    int x;                             // +0x0
    int y;                             // +0x4
    int unknown_8[4];
};

struct Display_0041ce90 {
    char unknown_0[0x40];
    HWND hwnd;                         // +0x40
    char unknown_44[0xf0 - 0x44];
    unsigned char flags_f0;            // +0xf0
};

struct Game {
    char unknown_0[0x519];
    char menu[0xcce];                  // +0x519, the GUI context TALK.GUI lives in
    char unknown_11e7[0x1b63 - 0x11e7];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x2c76 - 0x2a43];
    Rect view;                         // +0x2c76
    char unknown_2c8e[0x2cc7 - 0x2c8e];
    CursorState cursor;                // +0x2cc7
    char unknown_2cf3[0x1422b - 0x2cf3];
    int mapWidth;                      // +0x1422b
    int mapHeight;                     // +0x1422f
    char unknown_14233[0x14281 - 0x14233];
    unsigned short flags_14281;        // +0x14281
    char unknown_14283[0x142cb - 0x14283];
    char field_142cb[0x142f1 - 0x142cb]; // +0x142cb, the radar viewport rectangle
    CameraFlags flags_142f1;           // +0x142f1
    Unit* followUnit;                  // +0x142f3
    Follow_0041ca10* follow;           // +0x142f7
    char unknown_142fb[0x1431f - 0x142fb];
    int x;                             // +0x1431f
    int y;                             // +0x14323
    int x2;                            // +0x14327
    int y2;                            // +0x1432b
    int value_1432f;                   // +0x1432f
    int value_14333;                   // +0x14333
    int sum_x;                         // +0x14337
    int sum_y;                         // +0x1433b
    Vec3 jump;                         // +0x1433f
    unsigned short value_1434b;        // +0x1434b
    unsigned char scrollSpeed;         // +0x1434d
    unsigned char flags_1434e;         // +0x1434e
    char unknown_1434f[0x14357 - 0x1434f];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x37e1f - 0x1435b];
    int width;                         // +0x37e1f
    int height;                        // +0x37e23
    char unknown_37e27[0x37e37 - 0x37e27];
    int viewWidth;                     // +0x37e37
    int viewHeight;                    // +0x37e3b
    char unknown_37e3f[0x37e9c - 0x37e3f];
    unsigned short unitIndex;          // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    Orders orderState;                 // +0x37ebe
    char unknown_37ec4[0x37f2f - 0x37ec4];
    unsigned char flags_37f2f;         // +0x37f2f
    char unknown_37f30[0x38a3f - 0x37f30];
    int scrollScale;                   // +0x38a3f
};

#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

void __stdcall DamageUnit(Unit* obj, Unit* unit, int n, int kind, int flag);
void __stdcall PlaySoundByName(char* name, int param);
Unit* __stdcall FindNextSelectedUnit(Unit*, int);
int __stdcall RefreshBuildCountTexts(void* menu, void* unit);
unsigned int* __stdcall GetCategoryMask(char* name);
void __stdcall CalcRadarViewportRect(void* param_1);
int GetScreenWidth();
int GetScreenHeight();
void __stdcall SetCursorPosition(int x, int y);
void HideSoftwareCursor();
void ShowSoftwareCursor();
int __stdcall IsScreenNamed(void* obj, const char* name);
int __stdcall IsKeyDown(int key);
void __stdcall GetCurrentMouseEvent(Mouse_0041ce90* mouse);
Display_0041ce90* GetDisplay();

// The call site sets ecx to the resource block and also pushes it, so
// RequestEnergy is a __thiscall method that takes the block explicitly too
// (its body never reads ecx).

// FUNCTION: 0x41bd10
int __stdcall FUN_0041bd10(Unit* builder, Unit* unit, float f)
{
    int result = 0;
    UnitType* s = unit->type;
    int max = s->maxHp;
    int v = s->buildTime;
    if (unit->hp >= max)
        return 0;
    int n1 = (int)((max * f - 1.0f) / v + 1.0f);
    int n2 = (int)((s->energyCost * f - 1.0f) / v + 1.0f);
    if (n1 >= 1)
        n1 = 1;
    if (n2 >= 1)
        n2 = 1;
    if (builder->store.RequestEnergy(&builder->store, (float)n2)) {
        DamageUnit(builder, unit, n1, 10, 0);
        result = 1;
    }
    return result;
}

// The 3-bit page field (bits 23-25) is set from the type's field_22e, or
// stepped down with wraparound when it is already past the first page.
static inline unsigned int SetPage(Unit* u, unsigned int f)
{
    return (((u->type->field_22e + 0x1ff) << 23) ^ f) & 0x3800000 ^ f;
}

static inline unsigned int StepPage(unsigned int f)
{
    return (((f & 0xff800000) - 1) ^ f) & 0x3800000 ^ f;
}

// FUNCTION: 0x41bf10
void __stdcall StepBuildMenuPageBack(int param_1)
{
    unsigned short index = g_game->unitIndex;
    Unit* u;
    if (index == 0)
        goto nextbuild;
    u = &g_game->units[index];
    if (u->field_a6 == 0)
        u = 0;
    if (u == 0)
        goto nextbuild;

    // Each branch declares its own f: a single shared local made MSVC put the
    // field in eax and the unit in ecx, swapping every later operand.
    if (param_1) {
        unsigned int f = u->flags.raw;
        if ((f & 0x400000) == 0) {
            f |= 0x400000;
            u->flags.raw = f;
            u->flags.raw = SetPage(u, f);
        } else if ((f & 0x3800000) == 0x800000) {
            u->flags.raw = f & 0xffbfffff;
        } else {
            u->flags.raw = StepPage(f);
        }
    } else {
        unsigned int f = u->flags.raw;
        if ((f & 0x3800000) < 0x1000000)
            u->flags.raw = SetPage(u, f);
        else
            u->flags.raw = StepPage(f);
        u->flags.raw |= 0x400000;
    }
    g_game->orderState.lo |= 0x10;

nextbuild:
    PlaySoundByName("nextbuildmenu", 0);
}

// Opens the selected unit's build menu at page param_1 (closes it for 0),
// if the unit has that many pages.
// Needed: without it the page count is loaded before param_1.

// FUNCTION: 0x41c060
void __stdcall OpenBuildMenuPage(int param_1)
{
    // Unit lookup written out in place: a GetSelectedUnit helper changes the code.
    unsigned short index = g_game->unitIndex;
    if (index != 0) {
        Unit* unit = &g_game->units[index];
        if (unit->field_a6 == 0)
            unit = 0;
        if (unit != 0) {
            if (param_1 < unit->type->field_22e) {
                unit->flags.bits.buildPage = param_1 > 0;
                if (param_1 != 0)
                    unit->flags.bits.page = param_1;
                g_game->orderState.lo |= 0x10;
                PlaySoundByName("nextbuildmenu", 0);
            }
        }
    }
}

// FUNCTION: 0x41c110
void __stdcall FUN_0041c110(Unit* unit)
{
    if (unit->field_ff == g_game->localPlayer && (unit->flags.lo & 0x10)) {
        g_game->orderState.raw |= 0x10;
    }
}

// FUNCTION: 0x41c150
void __stdcall FUN_0041c150(Unit* unit)
{
    Game* g = g_game;
    unsigned short val1 = g->unitIndex;
    unsigned short val2 = unit->field_a8;
    if (val1 == val2) {
        RefreshBuildCountTexts(g->menu, unit);
    }
}

// 0x41bde0 keeps its own file, so its declaration stands in for the definition.
void __stdcall StepBuildMenuPage(int param_1);

// The unit at g_game->unitIndex, or 0 when the index is empty or the slot is
// not live (field_a6 == 0). Inlined at both call sites below.
static Unit* GetSelectedUnit()
{
    unsigned short index = g_game->unitIndex;
    if (index) {
        Unit* unit = &g_game->units[index];
        if (unit->field_a6)
            return unit;
    }
    return 0;
}

// FUNCTION: 0x41c180
void FUN_0041c180()
{
    if (g_game->orderState.bits.next) {
        g_game->orderState.bits.next = 0;
        StepBuildMenuPage(0);
        return;
    }
    if (g_game->orderState.bits.prev) {
        g_game->orderState.bits.prev = 0;
        StepBuildMenuPageBack(0);
        return;
    }
    if (g_game->orderState.bits.build) {
        g_game->orderState.bits.build = 0;
        Unit* unit = GetSelectedUnit();
        if (unit) {
            unit->flags.raw |= 0x400000;
            g_game->orderState.bits.flag_4 = 1;
        }
        return;
    }
    if (g_game->orderState.bits.orders) {
        g_game->orderState.bits.orders = 0;
        Unit* unit = GetSelectedUnit();
        if (unit) {
            unit->flags.raw &= ~0x400000;
            g_game->orderState.bits.flag_4 = 1;
        }
    }
}

// FUNCTION: 0x41c2b0
void FUN_0041c2b0()
{
    char saved = g_game->scrollSpeed;
    memset(&g_game->followUnit, 0, 0x5c);
    g_game->scrollSpeed = saved;
}

// FUNCTION: 0x41c2e0
void __stdcall FUN_0041c2e0(int param)
{
    Unit* val = g_game->followUnit;
    Unit* result = FindNextSelectedUnit(val, param);
    g_game->followUnit = result;
}

// Returns the bit set of the unit types in the named category.
// The bit test was an inlined helper taking the type as unsigned short.
static inline int TestBit(unsigned int* set, unsigned short n)
{
    return set[n >> 5] & (1 << (n & 0x1f));
}

// Finds the local player's commander: the last of the player's units whose
// type is in the "Commander" unit-type set.
// FUNCTION: 0x41c310
void FindLocalCommander()
{
    Player* p = &g_game->players[g_game->localPlayer];
    unsigned int* set = GetCategoryMask("Commander");
    for (Unit* u = p->units; u <= p->unitsEnd; u++) {
        if (TestBit(set, u->field_a6)) {
            g_game->followUnit = u;
        }
    }
}

// FUNCTION: 0x41c390
void __cdecl FUN_0041c390()
{
    g_game->value_1434b = 0;
    g_game->followUnit = 0;
    g_game->follow = 0;
}

// The original calls this out of line from the camera functions.
#pragma auto_inline(off)
// FUNCTION: 0x41c3c0
void ClampCameraPosition()
{
    int maxX = g_game->mapWidth - g_game->viewWidth;
    int maxY = g_game->mapHeight - g_game->viewHeight;
    if (g_game->x < 0) {
        g_game->x = 0;
    } else if (g_game->x > maxX) {
        g_game->x = maxX;
    }
    if (g_game->y < 0) {
        g_game->y = 0;
    } else if (g_game->y > maxY) {
        g_game->y = maxY;
    }
    CalcRadarViewportRect(g_game->field_142cb);
}
#pragma auto_inline(on)

// Same clamp as ClampCameraPosition, on the second position pair.
// FUNCTION: 0x41c450
void ClampCameraTarget()
{
    int maxX = g_game->mapWidth - g_game->viewWidth;
    int maxY = g_game->mapHeight - g_game->viewHeight;
    if (g_game->x2 < 0) {
        g_game->x2 = 0;
    } else if (g_game->x2 > maxX) {
        g_game->x2 = maxX;
    }
    if (g_game->y2 < 0) {
        g_game->y2 = 0;
    } else if (g_game->y2 > maxY) {
        g_game->y2 = maxY;
    }
}

// FUNCTION: 0x41c4c0
void __stdcall SetCameraPosition(int x, int y, int instant)
{
    if (instant != 0) {
        g_game->x2 = x;
        g_game->y2 = y;
        int maxX = g_game->mapWidth - g_game->viewWidth;
        int maxY = g_game->mapHeight - g_game->viewHeight;
        if (g_game->x2 < 0) {
            g_game->x2 = 0;
        } else if (g_game->x2 > maxX) {
            g_game->x2 = maxX;
        }
        if (g_game->y2 < 0) {
            g_game->y2 = 0;
        } else if (g_game->y2 > maxY) {
            g_game->y2 = maxY;
        }
    } else {
        g_game->x = x;
        g_game->y = y;
        g_game->flags_142f1.lo |= 2;
        ClampCameraPosition();
        g_game->x2 = g_game->x;
        g_game->y2 = g_game->y;
    }
    g_game->flags_14281 &= 0xfff7;
}

// FUNCTION: 0x41c5e0
void __stdcall StartScreenShake(int dx, int dy, int value)
{
    if ((g_game->flags_37f2f & 0x10) == 0) {
        g_game->value_1432f = value;
        g_game->value_14333 = value;
        if (dx != 0) {
            g_game->sum_x = dx;
        }
        if (dy != 0) {
            g_game->sum_y = dy;
        }
        if (value > 0) {
            g_game->flags_1434e |= 1;
        }
    }
}

// FUNCTION: 0x41c640
void __stdcall FUN_0041c640(int dx, int dy, int value)
{
    if ((g_game->flags_37f2f & 0x10) == 0) {
        if ((g_game->flags_1434e & 1) == 0) {
            g_game->sum_x = 0;
            g_game->sum_y = 0;
        }
        g_game->value_1432f = (g_game->value_1432f + value) / 2;
        g_game->value_14333 = g_game->value_1432f;
        g_game->sum_x += dx;
        g_game->sum_y += dy;
        if (g_game->value_1432f > 0) {
            g_game->flags_1434e |= 1;
        }
    }
}

// The original calls this out of line from UpdateCameraFollow.
#pragma auto_inline(off)
// FUNCTION: 0x41c6f0
void UpdateScreenShake()
{
    if ((g_game->flags_1434e & 1) != 0) {
        int count = g_game->value_14333;
        if (count > 0) {
            int dx = g_game->sum_x * count / g_game->value_1432f;
            int dy = g_game->sum_y * count / g_game->value_1432f;
            int rx = (int)(((__int64)rand() * dx) / 32768) - dx / 2;
            int ry = (int)(((__int64)rand() * dy) / 32768) - dy / 2;
            g_game->x += rx;
            g_game->y += ry;
            g_game->value_14333--;
        } else {
            g_game->flags_1434e &= 0xfe;
        }
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x41c7c0
void __stdcall CenterCameraOnPoint(int a, int b, int c)
{
    int cy = b - g_game->viewHeight / 2;
    int cx = a - g_game->viewWidth / 2;
    if (c != 0) {
        g_game->x2 = cx;
        g_game->y2 = cy;
        int maxX = g_game->mapWidth - g_game->viewWidth;
        int maxY = g_game->mapHeight - g_game->viewHeight;
        if (g_game->x2 < 0) {
            g_game->x2 = 0;
        } else if (g_game->x2 > maxX) {
            g_game->x2 = maxX;
        }
        if (g_game->y2 < 0) {
            g_game->y2 = 0;
        } else if (g_game->y2 > maxY) {
            g_game->y2 = maxY;
        }
    } else {
        g_game->x = cx;
        g_game->y = cy;
        g_game->flags_142f1.lo |= 2;
        ClampCameraPosition();
        g_game->x2 = g_game->x;
        g_game->y2 = g_game->y;
    }
    g_game->flags_14281 &= 0xfff7;
}

// FUNCTION: 0x41c8e0
void __stdcall CenterCameraOnMapPosition(Vec3* p, int param_2)
{
    int cy = (short)((p->z - (p->y >> 1)) >> 16) - g_game->viewHeight / 2;
    int cx = (short)(p->x >> 16) - g_game->viewWidth / 2;
    if (param_2 != 0) {
        g_game->x2 = cx;
        g_game->y2 = cy;
        int maxX = g_game->mapWidth - g_game->viewWidth;
        int maxY = g_game->mapHeight - g_game->viewHeight;
        if (g_game->x2 < 0) {
            g_game->x2 = 0;
        } else if (g_game->x2 > maxX) {
            g_game->x2 = maxX;
        }
        if (g_game->y2 < 0) {
            g_game->y2 = 0;
        } else if (g_game->y2 > maxY) {
            g_game->y2 = maxY;
        }
    } else {
        g_game->x = cx;
        g_game->y = cy;
        g_game->flags_142f1.lo |= 2;
        ClampCameraPosition();
        g_game->x2 = g_game->x;
        g_game->y2 = g_game->y;
    }
    g_game->flags_14281 &= 0xfff7;
}

// SetTarget/ClampTarget stay helpers: they give the original's load order.
static inline void SetTarget(int x, int y)
{
    g_game->x2 = x;
    g_game->y2 = y;
}

static inline void ClampTarget()
{
    int maxX = g_game->mapWidth - g_game->viewWidth;
    int maxY = g_game->mapHeight - g_game->viewHeight;
    if (g_game->x2 < 0) {
        g_game->x2 = 0;
    } else if (g_game->x2 > maxX) {
        g_game->x2 = maxX;
    }
    if (g_game->y2 < 0) {
        g_game->y2 = 0;
    } else if (g_game->y2 > maxY) {
        g_game->y2 = maxY;
    }
}

// Stays an inline helper written as `cur + -320`: other forms change the arithmetic.
static inline int Approach(int cur, int target)
{
    int d = cur - target;
    if (d > 0) {
        if (d > 320)
            return cur + -320;
    } else {
        if (d < -320)
            return cur + 320;
    }
    return cur - d / 2;
}

// Camera follow: picks a target position (a pending jump at +0x1433f while
// its counter runs, else the followed object at +0x142f7, else the followed
// unit at +0x142f3 while it is still alive), centres the scroll target on it
// and clamps it to the map, then moves the scroll position halfway towards
// the target (at most 320 pixels per axis) and re-clamps it.

// Needed: without it the view centre x is computed in the wrong order.
// FUNCTION: 0x41ca10
void UpdateCameraFollow()
{
    Vec3* p = 0;
    if (g_game->value_1434b != 0) {
        g_game->value_1434b--;
        p = &g_game->jump;
    } else if (g_game->follow != 0) {
        p = &g_game->follow->pos;
    } else if (g_game->followUnit != 0) {
        if (g_game->followUnit->flags.raw & 0x10000000) {
            p = &g_game->followUnit->pos;
        } else {
            g_game->value_1434b = 0;
            g_game->followUnit = 0;
            g_game->follow = 0;
        }
    }
    if (p != 0) {
        SetTarget((short)(p->x >> 16) - g_game->viewWidth / 2,
                  (short)((p->z - (p->y >> 1)) >> 16) - g_game->viewHeight / 2);
        ClampTarget();
        g_game->flags_14281 &= 0xfff7;
    }
    if (g_game->x != g_game->x2) {
        g_game->flags_142f1.raw |= 2;
        g_game->flags_14281 &= 0xfff7;
        g_game->x = Approach(g_game->x, g_game->x2);
    }
    if (g_game->y != g_game->y2) {
        g_game->flags_142f1.raw |= 2;
        g_game->flags_14281 &= 0xfff7;
        g_game->y = Approach(g_game->y, g_game->y2);
    }
    UpdateScreenShake();
    ClampCameraPosition();
}

// FUNCTION: 0x41cc60
void BeginMouseScroll()
{
    g_game->value_1434b = 0;
    g_game->followUnit = 0;
    g_game->follow = 0;
    HideSoftwareCursor();
    CursorState* cursor = &g_game->cursor;
    cursor->flag = 1;
    cursor->rect = g_game->view;
    cursor->savedScroll.x = g_game->x / 16;
    cursor->savedScroll.y = g_game->y / 16;
    cursor->center.x = GetScreenWidth() / 2;
    cursor->center.y = GetScreenHeight() / 2;
    SetCursorPosition(cursor->center.x, cursor->center.y);
}

// Clears a flag in the cursor state held in the game object and warps the
// mouse back to the saved position. The state is reached through a pointer
// local, which keeps the `add eax, 0x2cc7` the original has.

// FUNCTION: 0x41cd20
void EndMouseScroll()
{
    CursorState* cursor = &g_game->cursor;
    cursor->flag = 0;
    SetCursorPosition(cursor->rect.x, cursor->rect.y);
    ShowSoftwareCursor();
}

// The same scroll-and-clamp sequence is written out in 0x41c7c0, 0x41c8e0,
// 0x41d0f0 and 0x41d1f0.
static inline void ScrollTo(int x, int y)
{
    g_game->x = x;
    g_game->y = y;
    g_game->flags_142f1.raw |= 2;
    ClampCameraPosition();
    g_game->x2 = g_game->x;
    g_game->y2 = g_game->y;
    g_game->flags_14281 &= 0xfff7;
}

// Mouse scrolling: moves the view by a quarter of the mouse offset from the
// centre point, saves the new scroll position (in 16-pixel units), warps the
// OS cursor back to the centre, and ends the scroll (restoring the cursor to
// where it started) once the button is released. 0x41cc60 starts it.

// FUNCTION: 0x41cd50
void UpdateMouseScroll()
{
    // All fields go through one pointer c, and the quarter offsets keep their own
    // locals dx, dy: both fix the load order and register use.
    CursorState* c = &g_game->cursor;
    Rect r = g_game->view;
    int dx = (r.x - c->center.x) / 4;
    int x = (dx + c->savedScroll.x) * 16;
    int dy = (r.y - c->center.y) / 4;
    int y = (dy + c->savedScroll.y) * 16;
    ScrollTo(x, y);
    c->savedScroll.x = g_game->x / 16;
    c->savedScroll.y = g_game->y / 16;
    SetCursorPosition(c->center.x, c->center.y);
    if (!(r.flags & 2)) {
        CursorState* cursor = &g_game->cursor;
        cursor->flag = 0;
        SetCursorPosition(cursor->rect.x, cursor->rect.y);
        ShowSoftwareCursor();
    }
}

// Unused, but needed: changes how the scroll speed multiply is compiled.
#include <string>

// Edge and arrow-key scrolling: moves the view by the scroll speed (capped at
// 0x80) when an arrow key is held (unless the TALK.GUI chat box is open) or
// the mouse sits on the edge of the screen, then clamps and saves it.

// FUNCTION: 0x41ce90
void UpdateEdgeScroll()
{
    Mouse_0041ce90 mouse;
    POINT pt;
    // Through a local: the direct spelling loads the scale into ebx.
    Game* g = g_game;
    int speed = g->scrollSpeed * g->scrollScale;
    if (speed > 0x80)
        speed = 0x80;
    if (speed == 0)
        return;
    GetCurrentMouseEvent(&mouse);
    Display_0041ce90* d = GetDisplay();
    if (!(d->flags_f0 & 2)) {
        GetCursorPos(&pt);
        int w = g_game->width;
        int h = g_game->height;
        if ((pt.x >= w || pt.y >= h) && pt.x < w + 100 && pt.y < h + 100
            && GetFocus() == d->hwnd) {
            mouse.x = pt.x;
            if (pt.x >= w)
                mouse.x = w - 1;
            mouse.y = pt.y;
            if (pt.y >= h)
                mouse.y = h - 1;
        }
    } else {
        int w = g_game->width;
        int h = g_game->height;
        if (mouse.x >= w)
            mouse.x = w - 1;
        if (mouse.y >= h)
            mouse.y = h - 1;
    }
    int talk = IsScreenNamed(g_game->menu, "TALK.GUI");
    int x = g_game->x;
    int y = g_game->y;
    if ((IsKeyDown(0xf4) && !talk) || (mouse.x == 0 && mouse.y < g_game->height))
        x -= speed;
    else if ((IsKeyDown(0xf6) && !talk) || mouse.x == g_game->width - 1)
        x += speed;
    if ((IsKeyDown(0xf5) && !talk) || (mouse.y == 0 && mouse.x < g_game->width))
        y -= speed;
    else if ((IsKeyDown(0xf7) && !talk) || mouse.y == g_game->height - 1)
        y += speed;
    if (g_game->x != x || g_game->y != y) {
        g_game->x = x;
        g_game->y = y;
        g_game->flags_142f1.lo |= 2;
        ClampCameraPosition();
        g_game->x2 = g_game->x;
        g_game->y2 = g_game->y;
        g_game->flags_14281 &= 0xfff7;
        g_game->value_1434b = 0;
        g_game->followUnit = 0;
        g_game->follow = 0;
    }
}
