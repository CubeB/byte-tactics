// Decompiled by deepseek-v4.1, deepseek-v4.1-flash, GPT-6.1-sol, mimo-v2.6-pro, DeepSeek V4.1 Flash, GPT-6, space-bunny-free, Haiku, Opus, Sonnet, Space Bunny Free, claude-sonnet-5-5 and Claude Opus 5.5. Names are provisional.
//
// The in-game information and status panels: the unit flags pass over the unit
// array, the radar and sonar jams, the light bars, the kill count, the debug
// unit and builder probes, the network statistics, the status panel tick, the
// game view overlays (the resource panel, the fog, the selection box), the hit
// point bar, the selection box corners, the map cell drawing, the selected
// unit's info panel, the profile timers, the tile footprint and the message
// line. The module's files gathered in address order; 0x467440, 0x468cf0,
// 0x46a610 and 0x46a860 keep their own files (their symbol counts, the include
// set or /Gi decide their matches).
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <ddraw.h>

#pragma pack(push, 1)

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Point {
    int x;
    int y;
};

// The unit position at +0x6a: a vector, or the six shorts of the grid cell.
union UnitPos_00467440 {
    Vec3 vec;
    struct {
        short f6a;
        short f6c;
        short f6e;
        short f70;
        short f72;
        short f74;
    } half;
};

struct Rot16 {
    short x, y, z;
};

struct Point16 {
    short x, z;
};

struct UnitDef {
    char unknown_0[0x16e];
    int modelMaxY;                     // +0x16e
    char unknown_172[0x1fa - 0x172];
    unsigned int maxHealth;            // +0x1fa
    char unknown_1fe[0x204 - 0x1fe];
    short radardistance;               // +0x204
    short sonardistance;               // +0x206
    short mincloakdistance;            // +0x208
    short radardistancejam;            // +0x20a
    short sonardistancejam;            // +0x20c
    char unknown_20e[0x241 - 0x20e];
    struct {
        unsigned int bit0_7 : 8;       // +0x241
        unsigned int bit8 : 1;         // tested by the detection visitor
        unsigned int bit9_31 : 23;
    } flags;
    unsigned int flags2;               // +0x245
};

struct UnitType_004685a0 {
    char name[0x152];                  // +0x0
    int count;                         // +0x152
    unsigned short* types;             // +0x156
    char unknown_15a[0x22f - 0x15a];
    unsigned char mobile;              // +0x22f
};

struct PlayerInfo {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
    char unknown_98[0x9b - 0x98];
    unsigned char gameFlags;           // +0x9b
};

struct Owner_00467440 {
    void* field_0;                     // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerInfo* data;                  // +0x27
    char unknown_2b[0x73 - 0x2b];
    char field_73;                     // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[1];        // +0x108
};

struct PlayerInfo_004685a0 {
    int f_0;                           // +0x0
    char unknown_4[0x2b - 4];
    char name[0x73 - 0x2b];            // +0x2b
    unsigned char controller;          // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char player;              // +0x146
    char unknown_147[0x22f - 0x147];
    unsigned char mobile;              // +0x22f
};

struct SpotState {
    char unknown_0[0xc];
    void* owner;                       // +0xc
};

// Unused here: its symbol ids keep DrawRotatedQuadOutline (0x467a50) matching.
struct Unit_00467960 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Object_004cb650;

struct Unit {
    char unknown_0[0x1f];
    unsigned char f_1f;                // +0x1f
    char unknown_20[0x3b - 0x20];
    unsigned char f_3b;                // +0x3b
    char unknown_3c[0x57 - 0x3c];
    unsigned char f_57;                // +0x57
    char unknown_58[0x5c - 0x58];
    void* f_5c;                        // +0x5c, the mission queue
    void* f_60;                        // +0x60, the background mission queue
    Rot16 rot;                         // +0x64
    union {
        UnitPos_00467440 pos;          // +0x6a
        struct {
            int x;                     // +0x6a
            int y;                     // +0x6e
            int z;                     // +0x72
        };
    };
    char unknown_76[0x92 - 0x76];
    // The unit definition; 0x46a530 indexes the model table with the short at
    // the same offset, 0x467e50 reads it as a string pointer.
    union {
        UnitDef* def;                  // +0x92
        UnitType_004685a0* type;
        char* f_92;
        struct {
            char unknown_92[3];
            unsigned char side;        // +0x95
        };
    };
    union {
        Owner_00467440* field_96;      // +0x96
        PlayerInfo_004685a0* player;
        int* f_96;
    };
    char unknown_9a[0x9e - 0x9a];
    SpotState* state;                  // +0x9e
    char unknown_a2[0xa6 - 0xa2];
    unsigned short unitDefIndex;           // +0xa6
    unsigned short f_a8;               // +0xa8
    char unknown_aa[0xb0 - 0xaa];
    int workTime;                      // +0xb0
    char unknown_b4[0xff - 0xb4];
    unsigned char playerIndex;            // +0xff
    char unknown_100[0x104 - 0x100];
    float f_104;                       // +0x104
    union {
        short health;                  // +0x108
        short f_108;
    };
    char unknown_10a[0x10e - 0x10a];
    unsigned char activateFlags;           // +0x10e
    char unknown_10f[0x110 - 0x10f];
    union {
        unsigned int flags;            // +0x110
        int f_110;
    };
    char unknown_114[0x118 - 0x114];
};

struct MapSize_00467440 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00467440 {
    unsigned char* data;               // +0x0
    MapSize_00467440 size;             // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct PlayerInfo_00467440 {
    void* field_0;                     // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerInfo* data;                  // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit* field_67;                    // +0x67
    Unit* field_6b;                    // +0x6b
    char unknown_6f[0x7c - 0x6f];
    ByteMap_00467440 explored;         // +0x7c
    char unknown_88[0x146 - 0x88];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Team_004689c0 {                 // 347 bytes
    char unknown_0[4];
    unsigned char* data;               // +0x4
    char unknown_8[347 - 8];
};

struct Player_004689c0 {               // 331 bytes
    char unknown_0[0x119];
    unsigned short field_119;          // +0x119
    char unknown_11b[331 - 0x11b];
};

struct Player_467d70 {
    Unit* unit;                        // +0x00
    char unknown_4[0x14b - 4];
};

struct Sub_004679a0_a {
    char unknown_0[1];
    int field_1;                       // +0x01
    char unknown_5[0xd - 0x5];
    int field_d;                       // +0x0d
    char unknown_11[0x19 - 0x11];
    int field_19;                      // +0x19
    int field_1d;                      // +0x1d
};

struct Sub_004679a0_b {
    char unknown_0[0x30];
    int field_30;                      // +0x30
    unsigned short* lightbar;          // +0x34
};

struct Handle {
    unsigned short index;
    char unknown_2[6];
    void* table;
};

struct Feature {
    char name[0x94];
    Point16 footprint;                 // +0x94
    char unknown_98[0xac - 0x98];
    unsigned short* animTable;         // +0xac
    unsigned short* shadowTable;       // +0xb0
    char unknown_b4[0xcc - 0xb4];
    Handle anim;                       // +0xcc
    Handle shadowAnim;                 // +0xd8
    char unknown_e4[0xfe - 0xe4];
    unsigned short drawn : 1;          // +0xfe
    unsigned short over : 1;
    unsigned short flipAnim : 1;
    unsigned short flipShadow : 1;
    unsigned short unknown_bits : 4;
};

// A spot, 0x30 bytes: either a pair of animation handles or the live state of
// a moving feature (its state, position and velocity).
struct FeatureSpot {
    short next;                        // +0x0
    short prev;                        // +0x2
    union {
        struct {
            Handle anim;               // +0x4
            Handle shadow;             // +0x10
        };
        struct {
            SpotState* state;          // +0x4
            UnitPos_00467440 pos;      // +0x8
            UnitPos_00467440 vel;      // +0x14
        };
    };
    Rot16 rot;                         // +0x20
    char unknown_26[0x2f - 0x26];
    unsigned char spotFlags;           // +0x2f
};

struct Def_004685a0 {
    char unknown_0[0x20];
    char name[0x229];                  // +0x20
};

struct Game {
    char unknown_0[0x519];
    char message[0x51d - 0x519];       // +0x519, the game's message line
    void* gaf;                         // +0x51d
    int field_521;                     // +0x521
    int field_525;                     // +0x525
    char unknown_529[4];
    int field_52d;                     // +0x52d
    Team_004689c0* teams;              // +0x531
    char unknown_535[0x57d - 0x535];
    int team_index;                    // +0x57d
    char unknown_581[0xdcb - 0x581];
    // The 16-colour palette; 0x467a50 reads the byte at +0xdd5, 0x468310 and
    // 0x468380 the one at +0xdda, which is colours[15].
    union {
        unsigned char colors[16];      // +0xdcb
        struct {
            char unknown_dcb[0xdd5 - 0xdcb];
            unsigned char color_00467a50; // +0xdd5
            char unknown_dd6[0xdda - 0xdd6];
            unsigned char color;       // +0xdda
        };
    };
    // The ten player entries at +0x1b63, and the two other views of them the
    // later functions use: 0x467d70 reads them from +0x1b8a, 0x4689c0 from +0x1b8e.
    char unknown_ddb[0x1b63 - 0xddb];
    union {
        PlayerInfo_00467440 players[10];          // +0x1b63, stride 0x14b
        struct {
            char unknown_1b63[0x1b8a - 0x1b63];
            union {
                Player_467d70 players_00467d70[10];  // +0x1b8a
                struct {
                    char unknown_1b8a[0x1b8e - 0x1b8a];
                    Player_004689c0 players_004689c0[11]; // +0x1b8e
                };
            };
        };
    };
    char unknown_29c7[0x2a3c - 0x29c7];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2a42 - 0x2a3e];
    unsigned char team_number;         // +0x2a42
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x2bee - 0x2a44];
    unsigned short flag0 : 1;          // +0x2bee, bit 0
    unsigned short bits1 : 15;
    char unknown_2bf0[0x1420b - 0x2bf0];
    FeatureSpot* spots;                // +0x1420b
    Unit* unit;                        // +0x1420f
    char unknown_14213[0x14233 - 0x14213];
    int width;                         // +0x14233
    char unknown_14237[0x1426f - 0x14237];
    Feature* features;                 // +0x1426f
    unsigned short* field_14273;       // +0x14273
    char unknown_14277[0x1427f - 0x14277];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x14281 - 0x14280];
    unsigned char field_14281;         // +0x14281
    char unknown_14282[0x1431f - 0x14282];
    int scroll_x;                      // +0x1431f
    int scroll_y;                      // +0x14323
    char unknown_14327[0x14357 - 0x14327];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x14377 - 0x1435f];
    Object_004cb650** types;           // +0x14377
    char unknown_1437b[0x14383 - 0x1437b];
    Vec3* scratch;                     // +0x14383
    Point* points;                     // +0x14387
    Point* vertices;                   // +0x1438b
    char unknown_1438f[0x1439b - 0x1438f];
    Def_004685a0* defs;                // +0x1439b
    char unknown_1439f[0x1481f - 0x1439f];
    unsigned short* sidePanelTopSeq[5];  // +0x1481f
    unsigned short* sidePanelBotSeq[5];  // +0x14833
    unsigned short* sidePanelSideSeq[5];  // +0x14847
    char unknown_1485b[0x148db - 0x1485b];
    void* logos32;                     // +0x148db
    char unknown_148df[0x37e1b - 0x148df];
    void* surface;                     // +0x37e1b
    int width_0046b900;                // +0x37e1f
    int f_37e23;                       // +0x37e23
    char unknown_37e27[0x37e3f - 0x37e27];
    union {
        struct {
            Sub_004679a0_a a;          // +0x37e3f
            Sub_004679a0_b b;          // +0x37e60
        };
        struct {
            char unknown_37e3f[0x37e90 - 0x37e3f];
            int panel;                 // +0x37e90
            void* sprite;              // +0x37e94
        };
    };
    char unknown_37e98[0x37ee6 - 0x37e98];
    unsigned short max_units;          // +0x37ee6
    char unknown_37ee8[0x37f06 - 0x37ee8];
    unsigned short drawFlags;          // +0x37f06
    char unknown_37f08[0x37f2f - 0x37f08];
    unsigned short flags;              // +0x37f2f
    char unknown_37f31[0x38a47 - 0x37f31];
    union {
        int field_38a47;               // +0x38a47
        unsigned int tick;
    };
    unsigned short speed;              // +0x38a4b
    unsigned short speed2;             // +0x38a4d
    char unknown_38a4f[0x38d89 - 0x38a4f];
    int total;                         // +0x38d89
    int values[8];                     // +0x38d8d
    char unknown_38dad[0x391b3 - 0x38dad];
    int f_391b3;                       // +0x391b3
    unsigned short f_391b7;            // +0x391b7
    int f_391b9;                       // +0x391b9
    unsigned short f_391bd;            // +0x391bd
    char unknown_391bf[0x391f9 - 0x391bf];
    int f_391f9;                       // +0x391f9

    PlayerInfo_00467440* Current() { return &players[playerIndex]; }
};

class DetectionVisitor {
public:
    virtual void MarkUnitsInRadarOrSonarRadius(Unit* unit);
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    Vec3 pos;                          // +0xc
};

class RadarJamVisitor {
public:
    virtual void ApplyRadarJamFlag(Unit* unit);
};

class SonarJamVisitor {
public:
    virtual void ApplySonarJamFlag(Unit* unit);
};

struct Rect_004b0510 {
    int x1;                            // +0x0
    int y1;                            // +0x4
    int x2;                            // +0x8
    int y2;                            // +0xc
};

struct Colors_00467b60 {
    char unknown_0[4];
    unsigned char empty;               // +0x4
    char unknown_5[5];
    unsigned char full;                // +0xa
};

struct PlayerData_467c00 {
    char unknown_0[0x96];
    unsigned char color;               // +0x96
};

struct Player_467c00 {
    char unknown_0[0x27];
    PlayerData_467c00* data;           // +0x27
};

struct Rect_467c00 {
    int left;                          // +0
    int top;                           // +4
    int right;                         // +8
    int bottom;                        // +0xc
};

struct Quad_467c00 {
    Point p[4];
};

struct Entry_467c00 {
    unsigned short w;                  // +0
    unsigned short h;                  // +2
};

struct Player_00467cb0 {
    char unknown_0[0xb8];
    unsigned short kills;              // +0xb8
};

struct Mission_00467e50 {
    char pad_0[5];
    unsigned char state;               // +0x5
    char pad_6[0x16 - 0x6];
    void* target;                      // +0x16
    char pad_1a[0x4a - 0x1a];
    void* next;                        // +0x4a
};

struct MissionName_00467e50 {
    char pad_0[0x15];
    char* name;                        // +0x15
};

struct OrderType {
    MissionName_00467e50* GetTableEntry();
};

struct Rect_004685a0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Rect_004689c0 {
    int left;
    int top;
    int right;
    int bottom;
};

class Surface {
public:
    char unknown_0[0x1c];
    Rect_004689c0 clip;                // +0x1c
    Rect_004689c0* GetClipRect(Rect_004689c0* out);
};

// The frame-time profile at g_game+0x38d85: last tick at +0, one accumulator
// per phase at +0x2c.
struct FrameTimers {
    unsigned long last;
    int total;
    int values[9];
    int acc[9];
    void AccumulateProfileTime(int i);
};

// Bit 2 of the flags word at g_game+0x37f2f (like 0x416e00).
struct Flags_0046a530 {
    unsigned short low : 2;
    unsigned short flag : 1;
    unsigned short rest : 13;
};

struct Rect_0046b900 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    int d;                             // +0xc
};

struct Size_0046b9d0 {
    short w;
    short h;
};

struct Rect_0046b9d0 {
    int x1;
    int y1;
    int x2;
    int y2;
};

class Prim_0046bae0 {                  // a model primitive, 0x20 bytes
public:
    int field_0;                       // +0x00 color
    int count;                         // +0x04 number of vertices
    int field_8;                       // +0x08
    unsigned short* indices;           // +0x0c
    int field_10;                      // +0x10 texture pointer or index ref
    char unknown_14[8];
    // Dword bitfield, not an int tested with >> and &: the read codegen differs.
    unsigned int flag0 : 1;            // +0x1c bit 0
    unsigned int texIndexed : 1;       // +0x1c bit 1
    unsigned int rest : 30;            // +0x1c
};

class Object_0046bae0 {                // the model or piece being drawn
public:
    int field_0;                       // +0x00
    int count;                         // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    char unknown_10[0x24 - 0x10];
    Vec3* verts;                       // +0x24
    Prim_0046bae0* prims;              // +0x28
};

#pragma pack(pop)

extern Game* g_game;

extern int g_probePanelBottom;
extern int g_statusPanelNextTick;

unsigned short* __stdcall FindGafEntry(void* gaf, const char* name);
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);

void __stdcall DrawFrame(void* param_1, short* param_2, int x, int y);
void __stdcall DrawFrame(void* dst, void* bmp, int x, int y);

void __stdcall RotateByAngles(Vec3* in, Vec3* out, short* angles);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int color);

void __stdcall FillRectangle(void* surface, Rect_004b0510* rect, int color);
int __stdcall FillRectangle(void* surface, void* rect, int color);
void __stdcall FillRectangle(int surface, Rect_0046b900* rect, int color);

void __stdcall DrawRectangle(void* surface, Rect_004b0510* rect, int color);
void __stdcall DrawRectangle(void* surface, void* rect, int color);
void __stdcall DrawRectangle(int surface, Rect_0046b900* rect, int color);

void __stdcall DrawFrameQuad(void* surf, void* entry, Quad_467c00* dst, Quad_467c00* src);

char* __stdcall Translate(char* text);
int GetTextKeyColor();
void __stdcall SetTextColors(int param_1, int param_2);
void __stdcall SetFont(int param_1);
int GetFontHeight();
void __stdcall DrawString(void* surface, const char* text, int x, int y, int maxWidth);
void __stdcall DrawString(int surface, const char* text, int x, int y, int maxWidth);

int GetScreenHeight();
int GetScreenWidth();
void __stdcall SetOffscreenSurface(void* surf);
void __stdcall FillSurface(void* surf, int mode);
void FlipScreen();
void GetDisplayFieldE4();
unsigned long GetMilliseconds();

void __stdcall FadeRectangle(void* surface, Rect_004b0510* rect, int level);
void __stdcall FadeRectangle(void* surface, void* rect, int level);
void __stdcall GetByteRates(unsigned int* sent, unsigned int* received);
int __stdcall GetBuildRating(int player, unsigned short type);

void __stdcall PlaySoundByName(char* name, int param_2);
int __stdcall IsKeyDown(int key);
void __stdcall DrawTextClipped(void* surf, void* text, int x, int y, int color, int just);

void __stdcall GetObjectBounds(Object_004cb650* obj, Vec3* lo, Vec3* hi, int arg);
void __stdcall DrawRotatedQuadOutline(void* surface, Vec3* offset, Vec3* corners, short* angles);
void __stdcall DrawRotatedQuadOutline(Vec3* view, Vec3* pos, Vec3* corners, void* unit);

int __stdcall GetGafSequenceFrame(short* ref);
void __stdcall FillPolygon(void* surface, Point* points, int count, int color);
void __stdcall DrawFrameQuad(void* surface, void* texture, Point* points, void* src);

int __stdcall GetCellHeight(void* p);

void __stdcall ReportGameEvent(int param_1);
void __stdcall AddMessage(char* param_1, int param_2, int param_3, int param_4);
int __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

// Sets flag 0x200 on a unit that lies below the sea level line and flag 0x100
// on one that reaches it, when it is close enough in the x/z plane to this
// object. Same unit flag field as the neighbours 0x467960 and 0x467980.

// FUNCTION: 0x467840
void DetectionVisitor::MarkUnitsInRadarOrSonarRadius(Unit* unit)
{
    if (unit->unitDefIndex == 0 || unit->playerIndex == g_game->playerIndex) {
        return;
    }

    UnitDef* def = unit->def;
    if (def->flags.bit8) {
        return;
    }

    int dz = unit->z - this->pos.z;
    int dx = unit->x - this->pos.x;
    int dist = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);

    if (unit->y <= ((int)g_game->seaLevel << 16) && dist < this->field_8) {
        unit->flags |= 0x200;
    }

    if (def->modelMaxY + unit->y >= ((int)g_game->seaLevel << 16) && dist < this->field_4) {
        unit->flags |= 0x100;
    }
}

// FUNCTION: 0x467950
void __stdcall MarkRecentlyDamaged(void* param_1)
{
    *(char*)((char*)param_1 + 0xfa) = 0xf0;
}

// Same as ApplySonarJamFlag but clears flag 0x100 instead of 0x200.

// FUNCTION: 0x467960
void RadarJamVisitor::ApplyRadarJamFlag(Unit* unit)
{
    unit->flags = (unit->flags & ~0x100) | 0x400;
}

// FUNCTION: 0x467980
void SonarJamVisitor::ApplySonarJamFlag(Unit* unit)
{
    unit->flags = (unit->flags & ~0x200) | 0x400;
}

// FUNCTION: 0x4679a0
void LoadLightBar()
{
    Sub_004679a0_a* a = &g_game->a;
    a->field_1 = a->field_d = a->field_19 = a->field_1d = 0;
    Sub_004679a0_b* b = &g_game->b;
    b->field_30 = 0;
    b->lightbar = 0;
    unsigned short* frames = FindGafEntry(g_game->gaf, "LIGHTBAR");
    g_game->b.lightbar = (unsigned short*)GetGafFrame(frames, 1);
    g_game->b.lightbar[3] = 0;
    g_game->b.lightbar[2] = 0;
}

// FUNCTION: 0x467a20
void __stdcall BlitGafFrameAtOffset(void* param_1, short* param_2, int param_3, int param_4)
{
    DrawFrame(param_1, param_2, param_2[2] + param_3, param_2[3] + param_4);
}

// Rotates the four corners of a box by the given angles, projects each rotated
// corner to screen space (16.16 fixed point down to short) and outlines the
// resulting quad on the surface.

// FUNCTION: 0x467a50
void __stdcall DrawRotatedQuadOutline(void* surface, Vec3* offset,
                            Vec3* corners, short* angles)
{
    Vec3* scratch = g_game->scratch;
    Point* points = g_game->points;
    unsigned char color = g_game->color_00467a50;
    // corners is copied to c, which is the loop's induction variable: this
    // fixes the stack slot of the loop counter.
    Vec3* c = corners;

    for (int i = 0; i < 4; i++) {
        RotateByAngles(c, scratch, angles);
        // Separate int locals, evaluated in the order y, z, x.
        int y = (short)((scratch->y + offset->y) >> 16);
        int z = (short)((offset->z - scratch->z) >> 16);
        int x = (short)((scratch->x + offset->x) >> 16);
        points->x = x + 0x80;
        points->y = z - (y >> 1) + 0x20;
        c++;
        scratch++;
        points++;
    }

    Point* quad = g_game->points;
    DrawLine(surface, quad[0].x, quad[0].y, quad[1].x, quad[1].y, color);
    DrawLine(surface, quad[1].x, quad[1].y, quad[2].x, quad[2].y, color);
    DrawLine(surface, quad[2].x, quad[2].y, quad[3].x, quad[3].y, color);
    DrawLine(surface, quad[3].x, quad[3].y, quad[0].x, quad[0].y, color);
}

// Draws a horizontal progress bar: the filled part of `rect` (moved down by
// `dy`) in one colour and, when not full, the rest in another.

// FUNCTION: 0x467b60
void __stdcall DrawProgressBar(void* surface, int value, int max, Rect_004b0510* rect, Colors_00467b60* colors, int dy)
{
    Rect_004b0510 r = *rect;
    r.y1 += dy;
    r.y2 += dy;
    if (value < 0)
        value = 0;
    if (value > max)
        value = max;
    r.x2 = (rect->x2 - rect->x1) * value / max + r.x1;
    FillRectangle(surface, &r, colors->full);
    if (r.x2 != rect->x2) {
        r.x1 = r.x2 + 1;
        r.x2 = rect->x2;
        FillRectangle(surface, &r, colors->empty);
    }
}

// Blits a player's logo entry (indexed by data->color) from the logos32
// table onto a destination rectangle shifted down by dy. src is the full
// texture rectangle, dst the screen rectangle.

// FUNCTION: 0x467c00
void __stdcall BlitSideLogoToRect(void* surf, Player_467c00* player, Rect_467c00* rect, int dy)
{
    unsigned char idx = player->data->color;
    Entry_467c00* entry = (Entry_467c00*)*(void**)((char*)g_game->logos32 + idx * 8 + 0x28);

    Quad_467c00 src;
    src.p[0].x = 0;
    src.p[0].y = 0;
    src.p[1].x = entry->w;
    src.p[1].y = 0;
    src.p[2].x = entry->w;
    src.p[2].y = entry->h;
    src.p[3].x = 0;
    src.p[3].y = entry->h;

    Quad_467c00 dst;
    dst.p[0].x = rect->left;
    dst.p[0].y = rect->top + dy;
    dst.p[1].x = rect->right;
    dst.p[1].y = rect->top + dy;
    dst.p[2].x = rect->right;
    dst.p[2].y = rect->bottom + dy;
    dst.p[3].x = rect->left;
    dst.p[3].y = rect->bottom + dy;

    DrawFrameQuad(surf, entry, &dst, &src);
}

// Draws a kill count ("N kill(s)", plus " - Veteran" past level 4) in the
// current text colour. The singular/plural ternary sits in both arms, so the
// > 4 arm keeps a test the level can never satisfy.

// FUNCTION: 0x467cb0
void __stdcall DrawKillCount(void* surface, Player_00467cb0* player, int x, int y)
{
    char buf[100];
    char* kills = Translate("kills");
    char* kill = Translate("kill");
    if (player->kills > 4) {
        sprintf(buf, "%d %s - %s", player->kills,
                player->kills == 1 ? kill : kills, Translate("Veteran"));
    } else {
        sprintf(buf, "%d %s", player->kills,
                player->kills == 1 ? kill : kills);
    }
    SetTextColors(g_game->colors[15], GetTextKeyColor());
    DrawString(surface, buf, x, y, -1);
}

// Draws the local player's three light bar frames (one frame table per side)
// onto the blit surface: the first bar twice, the second copy also shifted
// down by the status bar height, then the third bar where it sits.

// FUNCTION: 0x467d70
void DrawLightBars()
{
    void* surf = g_game->surface;
    SetOffscreenSurface(surf);
    FillSurface(surf, 0);

    int side = g_game->players_00467d70[g_game->playerIndex].unit->side;

    short* bar = (short*)GetGafFrame(g_game->sidePanelTopSeq[side], 0);
    DrawFrame(surf, bar, bar[2] + 0x81, bar[3]);

    int dy = GetScreenHeight() - 0x20;
    bar = (short*)GetGafFrame(g_game->sidePanelBotSeq[side], 0);
    DrawFrame(surf, bar, bar[2] + 0x81, bar[3] + dy);

    bar = (short*)GetGafFrame(g_game->sidePanelSideSeq[side], 0);
    DrawFrame(surf, bar, bar[2], bar[3]);

    FlipScreen();
}

// Debug overlay: dumps the state of the unit selected by the game's "unit
// probe" cursor. Draws a fading panel, then a column of lines ("Unit State
// Probe", uid, player, controller, build time left, damage, occupy, autotarget
// weights and the mission queues) at x 0x86. The running y is also written to
// g_probePanelBottom so the next overlay stacks below this one.

// FUNCTION: 0x467e50
int __stdcall DrawUnitStateProbe(void* surface)
{
    char buf[0x80];
    char* names[3];
    unsigned char* colors;
    Unit* unit;
    Rect_004b0510 r;
    int lineH;
    int y;
    int prev;
    Mission_00467e50* m;

    if (g_game->f_391b3 == 0 || g_game->f_391b7 == 0)
        return 0;
    unit = (Unit*)(*(int*)((char*)g_game + 0x14357)
                            + g_game->f_391b7 * 0x118);
    if ((unit->f_110 & 0x10000000) == 0 || (unit->f_110 & 0x4000) != 0) {
        g_game->f_391b3 = 0;
        g_game->f_391b7 = 0;
    }
    colors = &g_game->colors[0];
    SetTextColors(colors[15], GetTextKeyColor());
    SetFont(g_game->f_391f9);
    lineH = GetFontHeight() + 3;
    y = lineH * 7;
    GetDisplayFieldE4();
    prev = g_probePanelBottom;
    r.x1 = 0x83;
    r.x2 = 0x191;
    r.y1 = y;
    r.y2 = prev == 0 ? lineH * 20 : prev;
    FadeRectangle(surface, &r, -0x18);
    r.x2++;
    r.y2++;
    DrawRectangle(surface, &r, colors[5]);
    y += 3;
    DrawString(surface, "Unit State Probe", 0x86, y, -1);
    y += lineH;
    DrawString(surface, "================", 0x86, y, -1);
    y += lineH;
    sprintf(buf, "uid: %03d/%04x '%s'\n", unit->f_a8, unit->f_a8, unit->f_92);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    sprintf(buf, "playerno: %d '%s' %s - %s\n",
            *(unsigned char*)((char*)unit->f_96 + 0x146),
            (char*)unit->f_96 + 0x2b,
            (*unit->f_96 != 0
             && (*(unsigned char*)((char*)unit->f_96 + 0x73) == 1
                 || *(unsigned char*)((char*)unit->f_96 + 0x73) == 2))
                ? "LOCAL" : "REMOTE",
            *(unsigned char*)(unit->f_92 + 0x22f) != 0 ? "MOBILE" : "BUILDING");
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    sprintf(buf, "controller: %d\n", *(unsigned char*)((char*)unit->f_96 + 0x73));
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    sprintf(buf, "buildtimeleft: %1.3f\n", unit->f_104);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    sprintf(buf, "damage: %d\n", unit->f_108);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    names[0] = "NONE";
    names[1] = "GROUND";
    names[2] = "AIR";
    sprintf(buf, "occupy: %s\n", names[unit->f_110 & 3]);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineH;
    if (*unit->f_96 != 0
        && (*(unsigned char*)((char*)unit->f_96 + 0x73) == 1
            || *(unsigned char*)((char*)unit->f_96 + 0x73) == 2)) {
        sprintf(buf, "autotarget w[pri:sec:spe]: w[%c:%c:%c]\n",
                (unit->f_1f & 0x10) ? 'X' : '-',
                (unit->f_3b & 0x10) ? 'X' : '-',
                (unit->f_57 & 0x10) ? 'X' : '-');
        DrawString(surface, buf, 0x86, y, -1);
        y += lineH;
        if (unit->f_5c != 0) {
            DrawString(surface, "Mission Q:", 0x86, y, -1);
            y += lineH;
            for (m = (Mission_00467e50*)unit->f_5c; m != 0;
                 m = (Mission_00467e50*)m->next) {
                if (m->target != 0)
                    sprintf(buf, "    '%s' state: %d  tgt: '%s'\n",
                            ((OrderType*)((char*)m + 4))->GetTableEntry()->name,
                            m->state, *(char**)((char*)m->target + 0x92));
                else
                    sprintf(buf, "    '%s' state: %d\n",
                            ((OrderType*)((char*)m + 4))->GetTableEntry()->name,
                            m->state);
                DrawString(surface, buf, 0x86, y, -1);
                y += lineH;
            }
        }
        if (unit->f_60 != 0) {
            DrawString(surface, "Background Mission Q:", 0x86, y, -1);
            y += lineH;
            for (m = (Mission_00467e50*)unit->f_60; m != 0;
                 m = (Mission_00467e50*)m->next) {
                if (m->target != 0)
                    sprintf(buf, "    '%s' state: %d  tgt: '%s'\n",
                            ((OrderType*)((char*)m + 4))->GetTableEntry()->name,
                            m->state, *(char**)((char*)m->target + 0x92));
                else
                    sprintf(buf, "    '%s' state: %d\n",
                            ((OrderType*)((char*)m + 4))->GetTableEntry()->name,
                            m->state);
                DrawString(surface, buf, 0x86, y, -1);
                y += lineH;
            }
        }
    }
    g_probePanelBottom = y;
    return 1;
}

// Draws a percentage bar: DrawRectangle on the whole rectangle, then, when the
// percentage (capped at 100) is positive, fills that fraction of its width.
// Both calls take the colour byte at g_game+0xdda through one reference.

// FUNCTION: 0x468310
void __stdcall DrawPercentBar(void* surface, Rect_004b0510* rect, int percent)
{
    unsigned char& color = g_game->color;
    DrawRectangle(surface, rect, color);
    if (percent >= 100)
        percent = 100;
    if (percent > 0) {
        rect->x2 = (rect->x2 - rect->x1) * percent / 100 + rect->x1;
        FillRectangle(surface, rect, color);
    }
}

// Network statistics: two rows, each a "Send"/"Receive" rate label with a
// 64x8 bar under it. The bar is full at 56 K/s (the rate times 100 over 5600,
// capped at 100 inside the bar helper).
// Both rows sit at x 0x81..0xc1; the first row's y comes from the game,
// the second one starts just under the first bar.

// DrawPercentBar, the out-of-line bar, written here so both calls inline.
static void Bar(void* surface, Rect_004b0510* rect, int percent)
{
    unsigned char& color = g_game->color;
    DrawRectangle(surface, rect, color);
    if (percent >= 100)
        percent = 100;
    if (percent > 0) {
        rect->x2 = (rect->x2 - rect->x1) * percent / 100 + rect->x1;
        FillRectangle(surface, rect, color);
    }
}

// FUNCTION: 0x468380
void __stdcall DrawNetworkStats(void* surface)
{
    unsigned int sent;
    unsigned int received;
    char buf[0x20];
    Rect_004b0510 r;
    r.x1 = 0x81;
    r.x2 = 0xc1;
    r.y1 = g_game->f_37e23 - 0x5f;
    r.y2 = r.y1 + 8;
    int h = GetFontHeight();
    GetByteRates(&sent, &received);
    sprintf(buf, "Send - %1.1f K/s", sent * 0.001);
    DrawString(surface, buf, r.x1, r.y1, -1);
    r.y1 += h;
    r.y2 = r.y1 + 8;
    Bar(surface, &r, sent * 100 / 5600);
    r.x1 = 0x81;
    r.x2 = 0xc1;
    r.y1 = r.y2 + 1;
    sprintf(buf, "Receive - %1.1f K/s", received * 0.001);
    DrawString(surface, buf, r.x1, r.y1, -1);
    r.y1 += h;
    r.y2 = r.y1 + 8;
    Bar(surface, &r, received * 100 / 5600);
}

static void Bar_004685a0(void* surface, Rect_004685a0* rect, int percent)
{
    unsigned char& color = g_game->colors[15];
    DrawRectangle(surface, rect, color);
    percent = (percent >= 100) ? 100 : percent;
    if (percent > 0) {
        rect->right = (rect->right - rect->left) * percent / 100 + rect->left;
        FillRectangle(surface, rect, color);
    }
}

// FUNCTION: 0x4685a0
int __stdcall DrawUnitBuilderProbe(void* surface)
{
    if (g_game->f_391b9 == 0 || g_game->f_391bd == 0)
        return 0;
    Unit* unit = &g_game->units[g_game->f_391bd];
    if ((unit->f_110 & 0x10000000) == 0 || (unit->f_110 & 0x4000) != 0
            || unit->type->types == 0) {
        g_game->f_391b3 = 0;
        g_game->f_391b7 = 0;
    }
    unsigned char* colors = g_game->colors;
    SetTextColors(colors[15], GetTextKeyColor());
    SetFont(g_game->f_391f9);
    int lineHeight = GetFontHeight() + 3;
    int y = lineHeight * 3;
    GetDisplayFieldE4();
    Rect_004685a0 r;
    r.left = 0x83;
    r.right = 0x191;
    r.top = y;
    if (g_probePanelBottom == 0)
        r.bottom = lineHeight * 20;
    else
        r.bottom = g_probePanelBottom;
    FadeRectangle(surface, &r, -0x18);
    r.right++;
    r.bottom++;
    DrawRectangle(surface, &r, colors[5]);
    char buf[0x80];
    y += 3;
    DrawString(surface, "Unit Builder Probe", 0x86, y, -1);
    y += lineHeight;
    DrawString(surface, "==================", 0x86, y, -1);
    y += lineHeight;
    sprintf(buf, "uid: %03d '%s'\n", unit->f_a8, (char*)unit->type);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineHeight;
    char* mobile = unit->type->mobile ? "MOBILE" : "BUILDING";
    char* remote;
    if (unit->player->f_0 != 0 && (unit->player->controller == 1 || unit->player->controller == 2))
        remote = "LOCAL";
    else
        remote = "REMOTE";
    sprintf(buf, "playerno: %d '%s' %s - %s\n", unit->player->player, unit->player->name, remote, mobile);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineHeight;
    sprintf(buf, "controller: %d\n\n", unit->player->controller);
    DrawString(surface, buf, 0x86, y, -1);
    y += lineHeight;
    if (unit->player->f_0 != 0 && (unit->player->controller == 1 || unit->player->controller == 2)) {
        sprintf(buf, "Units I can build, and the probabilities:\n",
                ((unit->f_1f & 0x10) ? 'X' : '-'),
                ((unit->f_3b & 0x10) ? 'X' : '-'),
                ((unit->f_57 & 0x10) ? 'X' : '-'));
        DrawString(surface, buf, 0x86, y, -1);
        int i = 0;
        y += lineHeight;
        if (unit->type->count > 0) {
            do {
                unsigned short id = unit->type->types[i];
                int prob = GetBuildRating(unit->playerIndex, id);
                Def_004685a0* def = &g_game->defs[id];
                Rect_004685a0 bar;
                bar.left = 0x88;
                bar.top = y + 1;
                bar.right = 0xa2;
                bar.bottom = bar.top + lineHeight - 6;
                Bar_004685a0(surface, &bar, prob);
                char* name = def->name;
                sprintf(buf, "       %3d %% - '%s'\n", prob, name);
                DrawString(surface, buf, 0x86, y, -1);
                y += lineHeight;
                i++;
            } while (i < unit->type->count);
        }
    }
    g_probePanelBottom = y;
    return 1;
}

// The status panel's scroll/refresh tick. When the space bar is held, `panel`
// moves a third of the way toward 0 (opening "Options") or toward -31
// (reopening "Panel"), so the panel slides instead of snapping. The rate limit
// g_statusPanelNextTick (GetTickCount() + 15) keeps one move per frame. Everything drawn
// below is positioned relative to bounds.bottom + `panel`, so the text scrolls
// with it. The three lines are drawn at fixed offsets from bounds.left (0x19,
// 0xbe, 0x17c), each sprintf'ed into the same 256-byte `buf` before it is drawn,
// so the earlier text is gone by the time the next line is written. The speed
// line is built in `num` and then appended to in place, with strlen, so the
// " (%+d)" of a second, differing speed value lands at its end.

// FUNCTION: 0x4689c0
void __stdcall DrawStatusPanel(Surface* win)
{
    char buf[0x100];
    char num[0x34];
    int v = g_game->panel;
    if (g_statusPanelNextTick < (int)GetMilliseconds()) {
        g_statusPanelNextTick = GetMilliseconds() + 15;
        if (!IsKeyDown(0x20) || (g_game->team_index != -1 && ((unsigned char*)g_game->teams->data)[g_game->team_index * 347] == 3)) {
            if (v < 0) {
                if (v == -31)
                    PlaySoundByName("Panel", 0);
                int q = (0 - v) / 3;
                if (q <= 1)
                    q = 1;
                v += q;
                if (v == 0)
                    PlaySoundByName("Options", 0);
                g_game->panel = v;
            }
        } else {
            if (v > -31) {
                if (v == 0)
                    PlaySoundByName("Panel", 0);
                int q = (v + 31) / 3;
                if (q <= 1)
                    q = 1;
                v -= q;
                if (v == -31)
                    PlaySoundByName("Options", 0);
            }
            g_game->panel = v;
        }
    }
    if (v == 0)
        return;
    g_game->field_52d = g_game->field_525;
    Rect_004689c0 bounds;
    win->GetClipRect(&bounds);
    int left = bounds.left;
    int bottom = bounds.bottom + v;
    DrawFrame(win, g_game->sprite, left, bottom);
    unsigned int tick = g_game->tick;
    int hours = tick / 108000;
    int rest = tick - hours * 108000;
    int minutes = rest / 1800;
    int seconds = (rest - minutes * 1800) / 30;
    sprintf(buf, "%s : %02d:%02d:%02d", Translate("Game Time"), hours, minutes, seconds);
    DrawTextClipped(win, buf, left + 0x19, bottom + 0xa, -1, 0);
    int team = g_game->team_number;
    sprintf(buf, "%s : %d  (Max %d)", Translate("Total Units"),
            g_game->players_004689c0[team].field_119, g_game->max_units);
    DrawTextClipped(win, buf, left + 0xbe, bottom + 0xa, -1, 0);
    if (g_game->speed2 == 10)
        sprintf(num, Translate("Normal"));
    else
        sprintf(num, "%+d", (int)g_game->speed2 - 10);
    sprintf(buf, "%s %s", Translate("Game Speed"), num);
    if (g_game->speed2 != g_game->speed)
        sprintf(buf + strlen(buf), " (%+d)", (int)g_game->speed - 10);
    DrawTextClipped(win, buf, left + 0x17c, bottom + 0xa, -1, 0);
    g_game->field_52d = g_game->field_521;
}

// FUNCTION: 0x46a400
void FrameTimers::AccumulateProfileTime(int param_1)
{
    unsigned int result = GetMilliseconds();
    int edi = *(int*)((char*)this);
    int edx = result - edi;
    int field_val = *(int*)((char*)this + param_1 * 4 + 0x2c);
    field_val = field_val + edx;
    *(int*)((char*)this + param_1 * 4 + 0x2c) = field_val;
    *(int*)((char*)this) = result;
}

// Hit point bar drawn under a unit: a 34x4 box in the frame colour, then a
// bar 32 pixels wide at full health, green above two thirds of maxHealth,
// yellow above one third and red below.

// FUNCTION: 0x46a430
void __stdcall DrawHitPointBar(void* surface, Unit* unit, int x, int y)
{
    if (unit->health <= 0)
        return;
    unsigned char* colors = g_game->colors;
    Rect_004b0510 r;
    r.x1 = x - 17;
    r.y1 = y - 2;
    r.x2 = x + 17;
    r.y2 = y + 2;
    FillRectangle(surface, &r, colors[0]);
    // The width has to be a separate local: written inline, MSVC tail merges
    // the three colour calls below into one.
    int width;
    r.x1++;
    r.y1++;
    r.y2--;
    width = unit->health * 32 / unit->def->maxHealth;
    r.x2 = r.x1 + width;
    if (unit->health > (int)(unit->def->maxHealth / 3) * 2)
        FillRectangle(surface, &r, colors[10]);
    else if (unit->health > (int)(unit->def->maxHealth / 3))
        FillRectangle(surface, &r, colors[14]);
    else
        FillRectangle(surface, &r, colors[12]);
}

// Draws one unit's selection box: the four corners of its model bounding box,
// projected to the screen. The corners are the box footprint on the ground, so
// all four take the low y of the box, not the high one.

// FUNCTION: 0x46a530
void __stdcall DrawSelectionBox(Vec3* view, Unit* unit)
{
    Flags_0046a530* f = (Flags_0046a530*)((char*)g_game + 0x37f2f);
    if (f->flag) {
        Vec3 lo;
        Vec3 hi;
        GetObjectBounds(g_game->types[unit->unitDefIndex], &lo, &hi, 0);
        Vec3 corners[4];
        corners[0].x = lo.x;
        corners[0].y = lo.y;
        corners[0].z = lo.z;
        corners[1].x = hi.x;
        corners[1].y = lo.y;
        corners[1].z = lo.z;
        corners[2].x = hi.x;
        corners[2].y = lo.y;
        corners[2].z = hi.z;
        corners[3].x = lo.x;
        corners[3].y = lo.y;
        corners[3].z = hi.z;
        Vec3 pos;
        pos.x = unit->x - (g_game->scroll_x << 16);
        pos.y = unit->y;
        pos.z = unit->z - (g_game->scroll_y << 16);
        DrawRotatedQuadOutline(view, &pos, corners, (char*)&unit->rot);
    }
}

// Draws one labelled bar: GetFontHeight returns the current text line height
// (c); the empty bar is outlined in white, the label is drawn, then the fill
// bar for g_game->values[index] is drawn at 100/total scale.

// FUNCTION: 0x46b900
void __stdcall DrawProfileBarLine(int surface, const char* text, int index)
{
    Rect_0046b900 rect;
    int x = g_game->width_0046b900 - 0x5a;
    int c = GetFontHeight();

    rect.a = x - 0xc8;
    rect.c = 0x27f;
    rect.b = 0x26;
    rect.d = c * 9 + 0x29;
    DrawRectangle(surface, &rect, 0xff);

    // No named local for c * index + 0x28: it must stay a compiler temp.
    DrawString(surface, text, x + 5, c * index + 0x28, -1);

    int w = g_game->values[index] * 100 / g_game->total * 2;
    // Stored first: any order with rect.c first changes register use.
    rect.a = x - w;
    rect.c = x;
    rect.d = c * index + 0x28;
    rect.b = rect.d;
    rect.d = rect.d + c;
    FillRectangle(surface, &rect, index + 1);
}

// Draws the screen-space footprint of a map point: the 16x16 tile position is
// scaled to pixels, offset by the scroll and the object's height, and kind
// selects a colour from the table at g_game+0xdcb.

// FUNCTION: 0x46b9d0
void __stdcall DrawMapTileSelectionOutline(void* surface, short* pos, Size_0046b9d0 size, int kind)
{
    unsigned char* colors = (unsigned char*)g_game + 0xdcb;
    int h = GetCellHeight(pos);
    int y1 = (pos[1] << 4) - g_game->scroll_y;
    int x1 = ((pos[0] + 8) << 4) - g_game->scroll_x;
    Rect_0046b9d0 rect;
    rect.x1 = x1;
    rect.y1 = y1 - (h >> 1) + 0x20;
    rect.x2 = x1 + (size.w << 4);
    rect.y2 = rect.y1 + (size.h << 4);
    if (kind == 4) {
        rect.x1++;
        rect.y1++;
        rect.x2--;
        rect.y2--;
    }
    DrawRectangle(surface, &rect, colors[kind]);
}

// Draws a closed polygon: a line between each pair of consecutive points,
// then one from the first point to the last.

// FUNCTION: 0x46ba80
void __stdcall DrawClosedPolygon(void* surface, Point* points, int count, int color)
{
    Point* p = points;
    for (int i = count - 1; i > 0; i--) {
        DrawLine(surface, p[0].x, p[0].y, p[1].x, p[1].y, color);
        p++;
    }
    DrawLine(surface, points->x, points->y, p->x, p->y, color);
}

// Draws a piece of a 3D model: projects obj->count vertices through
// RotateByAngles (the same rotate/project idiom as the matched 0x467a50) into
// screen points, then draws each primitive in obj->prims (from index 1 when
// obj->field_c is not -1, otherwise from 0). A primitive whose bit 0 is set
// is a flat filled polygon (FillPolygon); otherwise a 4-vertex textured quad
// (DrawFrameQuad), whose texture is either the direct pointer at +0x10 or, when
// bit 1 is set, the entry GetGafSequenceFrame looks up from the reference at +0x10.

// FUNCTION: 0x46bae0
void __stdcall DrawModel3doProjected(void* surface, Vec3* offset,
                            Object_0046bae0* obj, short* angles)
{
    Vec3* v = obj->verts;
    Vec3* scratch = g_game->scratch;
    Point* points = g_game->points;
    int i = 0;
    // Every increment of both loops stays in the for header.
    for (; i < obj->count; i++, v++, scratch++, points++) {
        RotateByAngles(v, scratch, angles);
        int y = (short)((scratch->y + offset->y) >> 16);
        int z = (short)((offset->z - scratch->z) >> 16);
        int x = (short)((scratch->x + offset->x) >> 16);
        points->x = x + 0x80;
        points->y = z - (y >> 1) + 0x20;
    }

    int j;
    Prim_0046bae0* e = obj->prims;
    if (obj->field_c != -1) {
        e++;
        j = 1;
    } else {
        j = 0;
    }
    for (; j < obj->field_8; j++, e++) {
        unsigned short* idx = e->indices;
        Point* dst = g_game->vertices;
        for (int k = 0; k < e->count; k++, idx++, dst++) {
            *dst = g_game->points[*idx];
        }
        if (!e->flag0) {
            if (e->count == 4) {
                void* tex;
                if (e->texIndexed)
                    tex = (void*)GetGafSequenceFrame((short*)((char*)e + 0x10));
                else
                    tex = (void*)e->field_10;
                DrawFrameQuad(surface, tex, g_game->vertices, 0);
            }
        } else {
            FillPolygon(surface, g_game->vertices, e->count, e->field_0);
        }
    }
}

// FUNCTION: 0x46bc60
void __stdcall ReportGameEventCallback(int param_1)
{
    ReportGameEvent(param_1);
}

// Shows a message: mode 0 through AddMessage (and sets a game flag),
// mode 1 in the game's message line for half of GetScreenWidth's value.

// FUNCTION: 0x46bc70
int __stdcall ShowGameMessage(char* text, int mode)
{
    int result = 1;
    if (mode == 0) {
        AddMessage(text, 0x10, 0, 10);
        g_game->flag0 = 1;
        return 1;
    }
    if (mode == 1) {
        int t = GetScreenWidth();
        result = OpenMessageBox(g_game->message, text, (int)(t * 0.5), 1, 1);
    }
    return result;
}
