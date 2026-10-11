// Decompiled by Haiku, Sonnet, Opus, Space Bunny Free, deepseek-v4.1-flash, DeepSeek V4.1 Flash, Claude Opus 5.5 and GPT-6. Names are provisional.
// The order-target area classes: the OrderFx family (circles, rings
// and rectangles, with their hit tests, world-position writers, area loaders
// and constructors), the std::vector<Point_0044eec0> point-list builders and
// the class that writes a target to a bit stream, the AirManeuverOrder
// and PathOrder path-target classes, the PathGoal
// family with its AiSearchGoal path, the Pathfinder singleton, and the
// out-of-line std::vector<Point_0044eec0> members. The module's files
// gathered in address order; 0x44da00 and 0x44ec30 keep their own files
// (0x44da00's std::copy specialization changes the vector instantiations
// 0x44d0e0 and 0x44d560 share, and 0x44ec30 only matches compiled with /Gi,
// which moves 0x44e3c0's registers).
// Keep the header include: without it the operand order of 0x44d560's sum flips.
#include <memory.h>
#include "../util/hapi_bank.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>

struct Point_0044eec0 {
    short x;
    short y;
};

// The vtables the area classes store by hand; the base class's is 0x4fd2f8.
extern void* g_orderFxVtable[];
extern void* g_approachRadiusVtable[];
extern void* g_ringApproachVtable[];
extern void* g_pointMarkerVtable[];

extern void __cdecl operator delete(void* p);

struct Elem_0044ce90 {
    int unknown_0;
};

// The base of the family: vtable 0x4fd2f8 and, at +0x4, the source object
// (or its index) the subclass was built with. Not order_fx.h: the constructors
// store the vtable by hand, which a class with virtual functions cannot spell.
class OrderFx {
public:
    void* vtable;                      // +0x0
    int source;                        // +0x4

    OrderFx() {}
    OrderFx(int param_1)
    {
        vtable = g_orderFxVtable;
        source = param_1;
    }

    OrderFx(int arg1, int arg2, int arg3);
    void* Destroy(unsigned char flag);
    int GetType();
    int IsFxStyle();
    int ContainsCell(int, int);
    int ApproxDist(int, int);
    void AddFlags(int param_1);
    void WriteBits(int);
    int KeepAfterComplete();
    void FillGoalCells(std::vector<Elem_0044ce90*>* list);
};

// Unused here: forward declarations of real functions; their symbol ids keep
// the allocation the merged OrderFx-family views moved (docs/c2-regalloc.md).
void RegisterUnitOrders();
void RegisterGroundOrders();
void EnableAICommands();
void RegisterAICommands();
void ResetAIPlayers();
void RegisterVtolOrders();
void StepAllGafSequences();
void ResetNetStats();
void InitCommands();
int UpdatePlacementGhostValidity();
void RefreshSelectionOrders();
void DispatchOrdersPanelPageFlags();
void ResetCameraState();
void FindLocalCommander();
void ClampCameraPosition();
void ClampCameraTarget();
void UpdateScreenShake();
void UpdateCameraFollow();
void BeginMouseScroll();
void EndMouseScroll();
void UpdateMouseScroll();
void UpdateEdgeScroll();
void CenterCameraOnRadarClick();
void CenterCameraOnStartPosition();
void RegisterDataArchives();
int GetCdPathMismatch();
void CreateGameObject();
void InitMissionStatus();
void SetUpEndMissionScreen();

// Unused here: forward declarations of real functions, one id each, where the
// merged ArcOrder view stood; they keep the allocation of the classes declared
// after it (docs/c2-regalloc.md).
void StartScreenFade();
void StepScreenFade();
void ScheduleFadeTick();
int IsFadeDone();
void StepPaletteFade();
void FillEndGameStatistics();
int AreStatBarsComplete();
void EnableEndMissionButtons();
void ShowEndMissionScreen();
void OpenCdCheckDialog();

// A view of the order base whose six virtual slots are named after the base's: ContainsUnit
// (slot 4) is the function defined below, so its stand-in has the slot suffix.
class Class_0044cf00
{
public:
    virtual void Destroy();
    virtual void SerializeSave();
    virtual void GetType();
    virtual void IsFxStyle();
    virtual void ContainsUnitSlot();
    virtual void ContainsCell(int param_1, int param_2);

    void ContainsUnit(int param_1);
};

struct Point_0044cf60 {
    short x;
    short y;
};

struct Map_0044cf60 {
    char unknown_0[0x7e];
    Point_0044cf60 origin;             // +0x7e
};

#pragma pack(push, 2)
struct Source_0044cf60 {
    char unknown_0[0xe];
    Map_0044cf60* map;                 // +0xe
};
#pragma pack(pop)

struct Vec3_0044d010 {
    int x;
    int y;
    int z;
};

struct Header_0044d010 {
    int magic;                         // +0x0
    Vec3_0044d010 v;                   // +0x4
};

typedef std::vector<Point_0044eec0> Vec_0044d0e0;

struct Obj_0044d310 {
    char unknown_0[0x76];
    short x;                           // +0x76
    short y;                           // +0x78
};

// The circle area: the point at +0x8, radius +0xc and radiusSq +0x10
// ((radius / 16) squared). The file-loading constructor 0x44d010 and its
// Serialize method (0x44d090) read and write the union's Vec3, the saved
// view of the same bytes.
class ApproachRadius : public OrderFx {
public:
    union {
        struct {
            Point_0044eec0 pos;        // +0x8
            int radius;                // +0xc
            int radiusSq;              // +0x10
        };
        Vec3_0044d010 v;               // +0x8, the saved view of the same bytes
    };

    void AppendGoalCell(Vec_0044d0e0* list);
    int Serialize(int unused, HapiBank* file, char* name);
    ApproachRadius(Source_0044cf60* source, int x, int y, int r);
    ApproachRadius(int owner, HapiBank* file, char* name);
    int GetType();
    void* Destroy(unsigned char flag);
    int ContainsUnit(Obj_0044d310* obj);
    int ContainsCell(int px, int py);
    int FillWorldPos(int* out);
    int ApproxDistExcess(int px, int py);
};

struct Point_0044d2c0 {
    short x;
    short y;
};

struct Inner_0044d2c0 {
    char unknown_0[0x7e];
    Point_0044d2c0 pos;                // +0x7e
};

#pragma pack(push, 1)
struct Owner_0044d2c0 {
    char unknown_0[0xe];
    Inner_0044d2c0* inner;             // +0xe
};
#pragma pack(pop)

struct Point_0044d3b0 {
    short x;
    short y;
};

struct Map_0044d3b0 {
    char unknown_0[0x7e];
    Point_0044d3b0 origin;             // +0x7e
};

#pragma pack(push, 2)
struct Source_0044d3b0 {
    char unknown_0[0xe];
    Map_0044d3b0* map;                 // +0xe
};
#pragma pack(pop)

struct Data_0044d470 {
    int a;
    int b;
    int c;
    int d;
    int e;
};

struct Header_0044d470 {
    int magic;                         // +0x0
    Data_0044d470 data;                // +0x4
};

typedef std::vector<Point_0044eec0> Vec_0044d560;

struct Point_0044d720 {
    short x;
    short y;
};

struct Vec3_0044d720 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 2)
struct Inner_0044d720 {
    char unknown_0[0x6a];
    Vec3_0044d720 pos;                 // +0x6a
    char unknown_76[0x7e - 0x76];
    Point_0044d720 cell;               // +0x7e
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Owner_0044d720 {
    char unknown_0[0xe];
    Inner_0044d720* inner;             // +0xe
};
#pragma pack(pop)

unsigned short __stdcall GetHeadingBetween(void* from, void* to);

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

struct Vec3_0044e3c0 {
    int x;
    int y;
    int z;
};

struct Sub_0044e190 {
    char unknown_0[0xdc];
    int range;                         // +0xdc
};

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x21c];
    short altitude;                    // +0x21c
    char unknown_21e[0x241 - 0x21e];
    union {
        unsigned int flags;            // +0x241
        struct {
            unsigned int unused : 22;
            // Must be a 1-bit bitfield, not folded into a mask test.
            unsigned int flag22 : 1;   // bit 22 of the flags
        };
    };
};
#pragma pack(pop)

#pragma pack(push, 1)
// The second part's full view of the unit, with part 1's x and y at +0x76.
struct Unit {
    char unknown_0[0x10];
    Sub_0044e190* weapon;              // +0x10
    char unknown_14[0x66 - 0x14];
    short heading;                     // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3_0044e3c0 pos;                 // +0x6a
    short x;                           // +0x76
    short y;                           // +0x78
    char unknown_7a[0x82 - 0x7a];
    int spatialBucket;                      // +0x82
    char unknown_86[0x92 - 0x86];
    UnitDef* def;                      // +0x92
    char unknown_96[0xa8 - 0x96];
    short id;                          // +0xa8
    char unknown_aa[0x118 - 0xaa];
};
#pragma pack(pop)

// The donut area: the point at +0x8, inner radius +0xc, outer +0x10 and each
// (radius / 16) squared at +0x14 and +0x18. The two constructors' view (a
// point and two radii) and the bit-stream view (x, y, inner, outer) name the
// same bytes; the file-loading constructor 0x44d470 and its Serialize method
// (0x44d500) use the union's Data, the saved view.
class RingApproach : public OrderFx {
public:
    union {
        struct {
            Point_0044eec0 pos;        // +0x8
            int radius2;               // +0xc
            int radius1;               // +0x10
            int radius2Sq;             // +0x14
            int radius1Sq;             // +0x18
        };
        struct {
            short x;                   // +0x8
            short y;                   // +0xa
            int inner;                 // +0xc
            int outer;                 // +0x10
        };
        Data_0044d470 data;            // +0x8, the saved view of the same bytes
    };

    int ApproxDistExcess(int px, int py);
    int Serialize(int unused, HapiBank* file, char* name);
    RingApproach(Source_0044d3b0* source, int x, int y, int r1, int r2);
    RingApproach(int owner, HapiBank* file, char* name);
    int GetType();
    void* Destroy(unsigned char flag);
    int ContainsUnit(Unit* unit);
    bool ContainsCell(int px, int py);
    void AppendGoalCell(Vec_0044d560* list);
    int FillWorldPos(Vec3_0044d720* out);
};

struct View_0044d8a0 {
    char unknown_0[0x7e];
    short x;                           // +0x7e
    short y;                           // +0x80
};

#pragma pack(push, 2)
struct Owner_0044d8a0 {
    char unknown_0[0xe];
    View_0044d8a0* view;               // +0xe
};
#pragma pack(pop)

struct Point_0044d8a0 {
    short x;
    short y;
};

struct Rect_0044d930 {
    int a;
    int b;
    int c;
    int d;
};

struct Header_0044d930 {
    int magic;                         // +0x0
    Rect_0044d930 r;                   // +0x4
};

typedef std::vector<Point_0044eec0> Vec_0044da00;

struct Pos16_44dc60 {
    short x;
    short z;
};

struct MapInfo_44dc60 {
    char unknown_0[0x7e];
    Pos16_44dc60 pos;   // +0x7e
};

// The rectangle area: x1/x2 at +8/+0xc and y1/y2 at +0x10/+0x14. The
// constructor 0x44d8a0's a/b/c/d view and the file-loading constructor
// 0x44d930's Rect view name the same bytes.
class PointMarker : public OrderFx {
public:
    union {
        struct {
            int x1;                    // +0x8
            int x2;                    // +0xc
            int y1;                    // +0x10
            int y2;                    // +0x14
        };
        struct {
            int a;                     // +0x8
            int b;                     // +0xc
            int c;                     // +0x10
            int d;                     // +0x14
        };
        Rect_0044d930 r;               // +0x8, the saved view of the same bytes
    };

    int FillWorldPos(int* out);
    int Serialize(int unused, HapiBank* file, char* name);
    PointMarker(Owner_0044d8a0* owner, Point_0044d8a0 pos, Point_0044d8a0 size);
    PointMarker(int owner, HapiBank* file, char* name);
    int GetType();
    void* Destroy(unsigned char should_delete);
    void AppendGoalCell(Vec_0044da00* list);
    int ContainsCell(int x, int y);
    int ApproxDist(int x, int y);
};

#pragma pack(push, 2)

class BitWriter {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10
    void GrowBuffer();
    void WriteBits(int value, int bits);
};

// The path target: the flag word at +8, the linked order at +0x16 and the
// position at +0x26. One class in several files' views: the constructors and
// setters declared below are its own methods, so the views merge here
// (docs/c2-regalloc.md). The field names each view needs coexist in unions.
class BitReader;
struct Order;
struct Owner_0044e080;
struct Source_0044e250;
struct Source_0044e2d0;
struct Vec3_0044e2d0;
struct Source_0044e330;
struct Vec3_0044e330;
#include "path_order_attach.h"

class PathOrder : public OrderFx {
public:
    union {
        unsigned short flags;              // +0x8
        short field_8;                     // +0x8, the setters' word
        struct {
            unsigned short : 6;
            unsigned short headingFlag : 1;  // +0x8 bit 6, SetHeading
            unsigned short : 9;
        };
        struct {
            unsigned short : 4;
            unsigned short radiusFlag : 1;   // +0x8 bit 4, SetApproachRadius
            unsigned short : 11;
        };
    };
    short approachRadius;              // +0xa
    short altitude;                    // +0xc
    short heading;                     // +0xe
    short piece;                       // +0x10
    union {
        Unit* unit;                    // +0x12
        Owner_0044e080* owner;         // +0x12, the bit-stream loader's view
    };
    PathOrderAttach ref;               // +0x16
    union {
        Vec3_0044e3c0 pos;             // +0x26
        struct {
            int pos_x;                 // +0x26, the bit-stream loader's view
            int pos_y;                 // +0x2a
            int pos_z;                 // +0x2e
        };
    };
    int followOffset;                  // +0x32

    PathOrder(int owner, HapiBank* file, char* name);
    PathOrder(Owner_0044e080* owner_, BitReader* reader);
    PathOrder(Order* order, Unit* unit);
    PathOrder(Source_0044e250* source, int unit, short value);
    PathOrder(Source_0044e2d0* source, const Vec3_0044e2d0& p);
    PathOrder(Source_0044e330* source, int unit, const Vec3_0044e330& p);
    void SetAltitude(int param_1);
    void SetHeading(short v);
    void SetApproachRadius(short v);

    void SerializeToBits(BitWriter* stream);
    int GetType();
    int IsFxStyle();
    void* Destroy(unsigned char flag);
    int SerializeToSave(int unused, HapiBank* file, char* name);
    int KeepAfterComplete();
    int FillWorldPos(Vec3_0044e3c0* out);
    int GetDesiredHeading(unsigned short* out);
};

#pragma pack(pop)

// The second part's own views and externs: the path targets, the
// Pathfinder, the bit reader and the game the loaders read.
struct Vec3_0044de80 {
    int x;
    int y;
    int z;
};

class PathOrderAttach;

#pragma pack(push, 2)
struct Owner_004895c0 {
    char unknown_0[0x92];
    UnitDef* def;                      // +0x92
    char unknown_96[0xa2 - 0x96];
    PathOrderAttach* head;             // +0xa2
    short flag;                        // +0xa6
};
#pragma pack(pop)

#include "path_order_attach.h"

class UnitRef {
public:
    void Unlink();
};

extern void* g_pathOrderVtable[];
extern void* g_airManeuverOrderVtable[];

#pragma pack(push, 2)
struct Rec_0044de80 {
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    short id1;                         // +0x8
    void* ref_vt;                      // +0xa
    void* ref_owner;                   // +0xe
    void* ref_next;                    // +0x12
    int ref_value;                     // +0x16
    short id2;                         // +0x1a
    short f1;                          // +0x1c
    short f2;                          // +0x1e
    short f3;                          // +0x20
    short f4;                          // +0x22
    short f5;                          // +0x24
    Vec3_0044de80 pos;                 // +0x26
    int i4;                            // +0x32
};

#pragma pack(pop)

// Reads bit fields from an array of dwords, lowest bits first.
// Bit reader, see src/network/net_stats.cpp.
class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int ReadBits(int bits);

    int ReadBit()
    {
        int r = (data[index] & (1 << bit)) != 0;
        if (++bit == 32) {
            bit = 0;
            index++;
        }
        return r;
    }
};

struct Owner_0044e080;
class Pathfinder;

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14207];
    Pathfinder* pathfinder;            // +0x14207
    char unknown_1420b[0x1427f - 0x1420b];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x142b7 - 0x14280];
    int overflowBucket;                // +0x142b7
    char unknown_142bb[0x14357 - 0x142bb];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x38a47 - 0x1435b];
    unsigned int gameTick;             // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class Pathfinder {
public:
    char unknown_0[0xc9];

    Pathfinder();
    ~Pathfinder();
    void AbortIfGoalMatch(void* param);
};

#pragma pack(push, 2)
struct Order {
    char unknown_0[0xe];
    Unit* unit;                        // +0xe
};

struct Source_0044e250 {
    char unknown_0[0xe];
    Unit* unit;                        // +0xe
};

struct Source_0044e2d0 {
    char unknown_0[0xe];
    int unit;                          // +0xe
};

struct Vec3_0044e2d0 {
    int x;
    int y;
    int z;
};

struct Source_0044e330 {
    char unknown_0[0xe];
    int unit;                          // +0xe
};

struct Vec3_0044e330 {
    int x;
    int y;
    int z;
};

#pragma pack(pop)

#pragma pack(push, 1)
struct Vec3_0044e5b0 {
    int x, y, z;
};

struct Object_0044e5b0 {
    char unknown_0[0x66];
    unsigned short heading;            // +0x66
    char unknown_68[2];
    Vec3_0044e5b0 pos;                 // +0x6a
};

class Class_0044e5b0 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8(Vec3_0044e5b0* out);   // vtable +0x20
    char unknown_4[4];                 // +0x4
    unsigned short flags;              // +0x8
    short approachRadius;              // +0xa
    char unknown_c[0x1a - 0xc];        // +0xc
    Object_0044e5b0* target;           // +0x1a

    int IsComplete(Object_0044e5b0* arg);
};
#pragma pack(pop)

struct Pos_0044e6c0 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

#pragma pack(push, 2)
#pragma pack(pop)

struct Vec3_0044e740 {
    int x;
    int y;
    int z;
    Vec3_0044e740() {}
    Vec3_0044e740(int ax, int ay, int az)
    {
        x = ax;
        y = ay;
        z = az;
    }
};

#pragma pack(push, 1)
struct Object_0044e740 {
    char unknown_0[0x6a];
    Vec3_0044e740 pos;                 // +0x6a
    char unknown_76[0xa8 - 0x76];
    unsigned short id;                 // +0xa8
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Source_0044e740 {
    char unknown_0[0xe];
    Object_0044e740* unit;             // +0xe
};

// The 0x2a-byte save record. The reader takes the id as a dword but only its
// low word is used (0x487080 masks with 0xffff); the high word is the flag.
struct Header_0044e740 {
    char unknown_0[8];                 // not read or written by name
    union {
        int id;                        // +0x08
        struct {
            unsigned short unit_id;    // +0x08
            unsigned short flag;       // +0x0a
        };
    };
    Vec3_0044e740 target;              // +0x0c
    Vec3_0044e740 other;               // +0x18
    short value_24;                    // +0x24
    unsigned short heading;            // +0x26
    unsigned short value_28;           // +0x28
};

#pragma pack(pop)

struct Owner_0044e9c0;

#pragma pack(push, 2)

struct Vec3_0044ea60 {
    int x;
    int y;
    int z;

    Vec3_0044ea60() {}
    Vec3_0044ea60(int ax, int ay, int az)
    {
        x = ax;
        y = ay;
        z = az;
    }
};

#pragma pack(push, 1)
struct UnitType_0044ea60 {
    char unknown_0[0x1ba];
    unsigned short max_turn;           // +0x1ba
};

struct Object_0044ea60 {
    char unknown_0[0x92];
    UnitType_0044ea60* type;           // +0x92
};
#pragma pack(pop)

#pragma pack(push, 2)
// Not air_maneuver_order.h: this view derives from the hand-vtable OrderFx above.
// The air order: the target and other points, the heading and the owner unit.
// FillWorldPos (0x44ea60) advances the target towards other. The bit-stream
// loader 0x44e9c0 stores its owner in self and reads the two points into it.
class AirManeuverOrder : public OrderFx {
public:
    union {
        unsigned short flags;          // +0x8, bit 0: heading set
        struct {
            unsigned short headingSet : 1;
            unsigned short unknown_rest : 15;
        };
    };
    Vec3_0044e740 target;              // +0xa
    Vec3_0044e740 other;               // +0x16
    short saveOnly22;                  // +0x22
    unsigned short heading;            // +0x24
    unsigned short value_26;           // +0x26
    Object_0044e740* self;             // +0x28

    int FillWorldPos(Vec3_0044e740* out);
    AirManeuverOrder(Source_0044e740* source, const Vec3_0044e740& a, const Vec3_0044e740& b);
    AirManeuverOrder(int owner, HapiBank* file, char* name);
    AirManeuverOrder(Owner_0044e9c0* owner, BitReader* reader);
    int SerializeToSave(int unused, HapiBank* file, char* name);
    void SerializeToBits(BitWriter* stream);
    int IsComplete(Object_0044e740* unit);
    void SetAltitude(int);
    int GetType();
    int IsFxStyle();
    int KeepAfterComplete();
    void* Destroy(int param_1);
    int GetDesiredHeading(unsigned short* out);
    void SetHeading(short v);
};
#pragma pack(pop)

// Unused here: the symbol ids these declarations take keep the allocation
// after the AirManeuverOrder views were joined (docs/c2-regalloc.md).
int RIReport(int, int, int, int, int, int, int, int, int, int);
int DrawWrappedText(char*, char*, int, int, int, int, int);
void CmdMem(int);

// The same vector as 0x44ee90 (_Ucopy) and 0x44eec0 (_Ufill): 0x44d0e0,
// 0x44d560 and 0x44da00 call 0x44ee90 and then this with ecx set to it.
typedef std::vector<Point_0044eec0> Vec_0044ee60;
typedef void (Vec_0044ee60::*DestroyFn_0044ee60)(Vec_0044ee60::iterator, Vec_0044ee60::iterator);

// _Destroy is protected: a derived class takes its address so it is emitted out of line.
struct Access_0044ee60 : Vec_0044ee60 {
    static DestroyFn_0044ee60 fn;
};

typedef std::vector<Point_0044eec0> Vec_0044ee70;
typedef Vec_0044ee70::size_type (Vec_0044ee70::*SizeFn_0044ee70)() const;

typedef std::vector<Point_0044eec0> Vec_0044ee90;
typedef Vec_0044ee90::iterator (Vec_0044ee90::*UcopyFn_0044ee90)(
    Vec_0044ee90::const_iterator, Vec_0044ee90::const_iterator, Vec_0044ee90::iterator);

// _Ucopy is protected: a derived class takes its address so it is emitted out of line.
struct Access_0044ee90 : Vec_0044ee90 {
    static UcopyFn_0044ee90 fn;
};

typedef std::vector<Point_0044eec0> Vec_0044eec0;
typedef void (Vec_0044eec0::*UfillFn_0044eec0)(
    Vec_0044eec0::iterator, Vec_0044eec0::size_type, const Point_0044eec0&);

// _Ufill is protected: a derived class takes its address so it is emitted out of line.
struct Access_0044eec0 : Vec_0044eec0 {
    static UfillFn_0044eec0 fn;
};

struct Point_0044eef0 {
    short x;
    short y;
};

struct Target_0044f1a0 {
    char unknown_0[0x2e];
    unsigned char occupyFlags;         // +0x2e
};

struct Target_0044f2a0 {
    char unknown_0[0x42];
    unsigned int flags;                // +0x42
};

// A 16.16 fixed-point coordinate: the low word is the fraction, the high
// word the signed integer part the path points store.
union Coord_0044f2a0 {
    int fixed;
    short half[2];
};

struct Vec3_004907e0 {
    Coord_0044f2a0 x;
    Coord_0044f2a0 y;
    Coord_0044f2a0 z;
};

#pragma pack(push, 2)
struct Struct_004907e0 {               // the owner
    Target_0044f1a0* target;           // +0x0
    char unknown_4[0x5c - 0x4];
    Target_0044f2a0* list;             // +0x5c
    char unknown_60[0x6a - 0x60];
    Vec3_004907e0 pos;                 // +0x6a
};
#pragma pack(pop)

// The object at +0x4 (see victory_490940.cpp); vtable 0x4fd2f8. Slot 8 is the
// "give me your position as a Vec3" call (an implementation is 0x44dc60); slot
// 8 of the base holds _purecall, slot 11 the stub KeepAfterComplete, which returns 0.
class Base_00490a10 {
public:
    virtual ~Base_00490a10();                           // slot 0
    virtual void SerializeSave();                       // slot 1
    virtual void GetType();                             // slot 2
    virtual void IsFxStyle();                           // slot 3
    virtual int ContainsUnit(Struct_004907e0* owner);   // slot 4
    virtual int ContainsCell(int x, int y);             // slot 5
    virtual void FillGoalCells();                       // slot 6
    virtual void ApproxDist();                          // slot 7
    virtual int FillWorldPos(Vec3_004907e0* out);       // slot 8
    virtual int TryGetDesiredHeading();                 // slot 9
    virtual void WriteBits();                           // slot 10
    virtual int KeepAfterComplete();                    // slot 11
};

class AiSearchGoal;

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
class PathGoal {
public:
    Base_00490a10* target;             // +0x4
    Struct_004907e0* owner;            // +0x8

    PathGoal(Struct_004907e0* p);
    virtual ~PathGoal() {}                          // slot 0
    virtual void SetPathOrder(void* param);         // slot 1
    virtual void TickTowardGoal();                  // slot 2
    virtual void FillWaypointWorldPos(Vec3_004907e0*, int, int);  // slot 3
    virtual void ExportGoalPose(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4
    virtual int HasReadyWaypoints();                // slot 5
    virtual AiSearchGoal* TryClaimRepath();         // slot 6
    virtual int HasNetUnitState();                  // slot 7
    virtual void SerializeNetUnitState(BitWriter*);  // slot 8
    virtual void DeserializeNetUnitState(BitReader*);  // slot 9
    virtual void DrawOnSurface(void*);              // slot 10
};

struct Point_0044f080 {
    short x;
    short y;
};

// Vtable 0x4fd458, constructor 0x44f010, destructor 0x44f450, ??_G 0x44f040.
// Slots 4 and 9 are inherited.
class AiSearchGoal : public PathGoal {
public:
    Point_0044f080 points[20];         // +0xc
    int count;                         // +0x5c
    unsigned int repathClaimTick;      // +0x60
    union {
        struct {
            unsigned char active : 1;  // +0x64 bit 0
            unsigned char flag_1 : 1;  // bit 1
            // flag_2 and flag_3 stay 1-bit bitfields: an int mode store gives and/or.
            unsigned char flag_2 : 1;  // bit 2
            unsigned char flag_3 : 1;  // bit 3, the path changed
        };
        unsigned char flags;
    };

    AiSearchGoal(Struct_004907e0* p);
    virtual ~AiSearchGoal();                        // slot 0
    virtual void SetPathOrder(void* param);         // slot 1, 0x44f2a0
    virtual void TickTowardGoal();                  // slot 2, 0x44f1a0
    virtual void FillWaypointWorldPos(Vec3_004907e0*, int, int);  // slot 3, 0x44f150
    virtual int HasReadyWaypoints();                // slot 5, 0x44f290
    virtual AiSearchGoal* TryClaimRepath();         // slot 6, 0x44f260
    virtual int HasNetUnitState();                  // slot 7, 0x44f480
    virtual void SerializeNetUnitState(BitWriter*);  // slot 8, 0x44f4a0
    // In console_commands_417e00.cpp: it was compiled with the console code,
    // far from the rest of the class.
    virtual void DrawOnSurface(void*);              // slot 10, 0x417e00
    void SetWaypoints(Point_0044f080* src, int n);
    void TruncateWaypointsFrom(int n);
};

struct Target_0044f5c0 {
    char unknown_0[0x2e];
    unsigned char flag_0 : 1;          // +0x2e bit 0
    unsigned char flag_1 : 1;          // +0x2e bit 1
    unsigned char flag_2 : 1;          // +0x2e bit 2
    unsigned char flag_rest : 5;
};

struct Owner_0044f5c0 {
    Target_0044f5c0* target;           // +0x0
};

struct Point_0044f5c0 {
    short x;                           // +0x0
    short y;                           // +0x2
};

// Vtable 0x4fd488, constructor 0x44f570, ??_G 0x44f590.
class PatrolGoal : public PathGoal {
public:
    Point_0044f5c0 points[3];          // +0xc
    int count;                         // +0x18

    PatrolGoal(Struct_004907e0* p);
    virtual void FillWaypointWorldPos(Vec3_004907e0*, int, int);  // slot 3, 0x44f650
    virtual int HasReadyWaypoints();                // slot 5, 0x44f5b0
    virtual void DeserializeNetUnitState(BitReader*);  // slot 9, 0x44f5c0
};

// Unused here: a forward declaration of a later function of this file; its
// symbol id keeps the allocation (docs/c2-regalloc.md).
void CreatePathfinder();

// Unused here: forward declarations of real functions; their symbol ids keep
// the allocation (docs/c2-regalloc.md).
void UpdateMenuSparks();
void OpenCloseCdPlayerDialog();
void OpenMainMenu();
void HandleFrontendDebugKey();
int ConnectToService();
void RunFrontendStateMachine();
void FreePictureCache();
void ClearPictureCache();
void OrLabelAttribs();
void EmptyPreFrontendInitHook();
int CodeChecksumFailed();
void EmptyPostSimStepHook();
void EmptyPostSimStepHook_B();
void EmptyPostSimStepHook_C();
int MainLoopContinueStub();
int MainLoopContinueStub_B();
int MainLoopContinueStub_C();
void EmptyMainLoopHook();
void EmptyMainLoopHook_B();
void EmptyMainLoopHook_C();
void ProtectUnitDefsReadOnly();
void ProtectUnitDefsReadWrite();
void CheckGpfVersion();
void LoadGameResources();
void FreeAnimFiles();
void LoadGameFonts();
void FreeGameFonts();
int LoadDefaultPalette();
void LoadTextureGafs();
void FreeTextureGafs();
void FreeUnitInfo();
void RefreshUnitInfo();
void CheckDownloadableFlags();
void AddDownloadBuildOptions();
void LoadUnitTypes();
void FreeUnitTypes();
void LoadDownloadMenus();
void FreeDownloadMenus();
// Two more, for the declaration PatrolGoal::DeserializeNetUnitState no longer needs.
void StartScreenFade();
void StepScreenFade();
// Unused here: one id each, to keep the allocation the merged PathOrder views
// moved (docs/c2-regalloc.md).
extern int Pad6260_01;
extern int Pad6260_02;
extern int Pad6260_03;
extern int Pad6260_04;
extern int Pad6260_05;
extern int Pad6260_06;
extern int Pad6260_07;
extern int Pad6260_08;
extern int Pad6260_09;
extern int Pad6260_10;
extern int Pad6260_11;
extern int Pad6260_12;
extern int Pad6260_13;
extern int Pad6260_14;
extern int Pad6260_15;
extern int Pad6260_16;
extern int Pad6260_17;
extern int Pad6260_18;
extern int Pad6260_19;
extern int Pad6260_20;
extern int Pad6260_21;
extern int Pad6260_22;
extern int Pad6260_23;
extern int Pad6260_24;
extern int Pad6260_25;
extern int Pad6260_26;
extern int Pad6260_27;
extern int Pad6260_28;
extern int Pad6260_29;
extern int Pad6260_30;
extern int Pad6260_31;
extern int Pad6260_32;
extern int Pad6260_33;
extern int Pad6260_34;
extern int Pad6260_35;
extern int Pad6260_36;
extern int Pad6260_37;
extern int Pad6260_38;
extern int Pad6260_39;
extern int Pad6260_40;
extern int Pad6260_41;
extern int Pad6260_42;
extern int Pad6260_43;
extern int Pad6260_44;
extern int Pad6260_45;
extern int Pad6260_46;
extern int Pad6260_47;
extern int Pad6260_48;
extern int Pad6260_49;
extern int Pad6260_50;
extern int Pad6260_51;
extern int Pad6260_52;
extern int Pad6260_53;
extern int Pad6260_54;
extern int Pad6260_55;
extern int Pad6260_56;
extern int Pad6260_57;
extern int Pad6260_58;
extern int Pad6260_59;
extern int Pad6260_60;
extern int Pad6260_61;
extern int Pad6260_62;
extern int Pad6260_63;
extern int Pad6260_64;
extern int Pad6260_65;
extern int Pad6260_66;
extern int Pad6260_67;

Unit* __stdcall LoadUnit(unsigned short index, HapiBank* file);
Vec3_0044e3c0 __stdcall GetPiecePosition(Unit* obj, int param);
int __stdcall GetGroundHeight(Pos_0044e6c0* pos);
void __cdecl FUN_004b7173(short angle, int* xy);

// FUNCTION: 0x44ce40
int OrderFx::GetType()
{
    return 1;
}

// FUNCTION: 0x44ce50
void* OrderFx::Destroy(unsigned char flag)
{
    vtable = g_orderFxVtable;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

// FUNCTION: 0x44ce70 ??0OrderFx@@QAE@HHH@Z
OrderFx::OrderFx(int arg1, int arg2, int arg3)
{
    vtable = g_orderFxVtable;
}

// FUNCTION: 0x44ce80
int __stdcall SerializeSave(int, int, int)
{
    return 0;
}

// Slot 6 of the vtable at 0x4fd2f8: the default implementation ignores the
// object and empties the caller's list (an inlined vector::clear()).
// FUNCTION: 0x44ce90
void OrderFx::FillGoalCells(std::vector<Elem_0044ce90*>* list)
{
    list->clear();
}

// FUNCTION: 0x44cec0
int OrderFx::ApproxDist(int, int)
{
    return 0;
}

// The second part's callers (0x44ef90, 0x44f080, 0x44f1a0, 0x44f2a0) call
// this out of line in the original; in one file MSVC would inline it.
#pragma auto_inline(off)
// FUNCTION: 0x44ced0
void OrderFx::AddFlags(int param_1)
{
    int val = source;
    if (val != 0) {
        int* target = (int*)(val + 0x4e);
        *target |= param_1;
    }
}
#pragma auto_inline(on)

// FUNCTION: 0x44cef0
int OrderFx::KeepAfterComplete()
{
    return 0;
}

// FUNCTION: 0x44cf00
void Class_0044cf00::ContainsUnit(int param_1)
{
    short esi = *(short*)(param_1 + 0x78);
    short eax = *(short*)(param_1 + 0x76);

    ContainsCell(eax, esi);
}

// FUNCTION: 0x44cf20
int OrderFx::ContainsCell(int, int)
{
    return 0;
}

// FUNCTION: 0x44cf30
int OrderFx::IsFxStyle()
{
    return 1;
}

// FUNCTION: 0x44cf40
int __stdcall TryGetDesiredHeading(int)
{
    return 0;
}

// FUNCTION: 0x44cf50
void OrderFx::WriteBits(int)
{
}

// Constructor of an OrderFx subclass that stores a point converted
// from fixed-point world coordinates relative to the map origin, a radius
// and (radius / 16) squared. It stores the same vtable as 0x44d010,
// so this is probably a second constructor of that class.
// FUNCTION: 0x44cf60 ??0ApproachRadius@@QAE@PAUSource_0044cf60@@HHH@Z
ApproachRadius::ApproachRadius(Source_0044cf60* source, int x, int y, int r)
    : OrderFx((int)source)
{
    vtable = g_approachRadiusVtable;
    Point_0044cf60 org = source->map->origin;
    Point_0044eec0 p;
    p.x = (x - (org.x << 19) + 0x80000) >> 20;
    p.y = (y - (org.y << 19) + 0x80000) >> 20;
    pos = p;
    radius = r;
    radiusSq = (r / 16) * (r / 16);
}

// FUNCTION: 0x44cfe0
int ApproachRadius::GetType()
{
    return 4;
}

// FUNCTION: 0x44cff0
void* ApproachRadius::Destroy(unsigned char flag)
{
    vtable = g_orderFxVtable;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

// Constructor of an OrderFx subclass that reads a 16-byte header from
// a named entry of an open file and keeps its last three dwords.
// FUNCTION: 0x44d010 ??0ApproachRadius@@QAE@HPAVHapiBank@@PAD@Z
ApproachRadius::ApproachRadius(int owner, HapiBank* file, char* name)
    : OrderFx(owner)
{
    vtable = g_approachRadiusVtable;
    int bad = 0;
    if (owner == 0 || file == 0)
        bad = 1;
    if (name == 0)
        bad = 1;
    if (!bad) {
        file->OpenNamedBox(name);
        file->SeekBox(0);
        Header_0044d010 hdr;
        if (file->ReadBox(&hdr, 16) == 16) {
            v.x = hdr.v.x;
            v.y = hdr.v.y;
            v.z = hdr.v.z;
        }
    }
}

// Saving counterpart of 0x44d010's constructor: writes a 16-byte
// header whose last three dwords are the stored values (the first dword is
// left uninitialised, as in the original).
// FUNCTION: 0x44d090
int ApproachRadius::Serialize(int unused, HapiBank* file, char* name)
{
    Header_0044d010 hdr;
    hdr.v.x = v.x;
    hdr.v.y = v.y;
    hdr.v.z = v.z;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 16);
    return 1;
}

// An inlined std::vector insert. The vector holds 4-byte points (two shorts,
// the element type of 0x44ee60/0x44ee90/0x44eec0/0x44eef0), and the method
// resets the list to a single point taken from this->+8, that is an inlined
// vector::assign(1, pos) or clear() plus push_back(pos):
//
//   list->clear();          // erase(begin(), end())
//   list->push_back(pos);   // inserts *(Point*)(this + 8) at the end
// FUNCTION: 0x44d0e0
void ApproachRadius::AppendGoalCell(Vec_0044d0e0* list)
{
    list->clear();
    list->push_back(pos);
}

// Point version of 0x44d310: whether (px, py) lies within the circle around
// (x, y).
// FUNCTION: 0x44d290
int ApproachRadius::ContainsCell(int px, int py)
{
    int dy = py - pos.y;
    int dx = px - pos.x;
    return dx * dx + dy * dy <= radiusSq;
}

// Virtual method of the circle area class (compare 0x44d290 and 0x44d310,
// which use the same centre at +0x8): writes the centre, offset by a point
// of the owner, as 16.16 world coordinates.

static inline void ToWorld(int* out, Point_0044eec0 a, Point_0044d2c0 b)
{
    out[0] = (a.x * 2 + b.x) << 19;
    out[2] = (a.y * 2 + b.y) << 19;
}

// FUNCTION: 0x44d2c0
int ApproachRadius::FillWorldPos(int* out)
{
    ToWorld(out, pos, ((Owner_0044d2c0*)source)->inner->pos);
    return 1;
}

// Whether obj lies within the circle around (x, y).
// FUNCTION: 0x44d310
int ApproachRadius::ContainsUnit(Obj_0044d310* obj)
{
    int dy = obj->y - pos.y;
    int dx = obj->x - pos.x;
    return dx * dx + dy * dy <= radiusSq;
}

// Approximate distance from (px, py) to the edge of the circle around
// (x, y), 0 when inside: 18 * major + 7 * minor axis distance.
// FUNCTION: 0x44d350
int ApproachRadius::ApproxDistExcess(int px, int py)
{
    int dx = abs(px - pos.x);
    int dy = abs(py - pos.y);
    int d;
    if (dx > dy)
        d = dy * 7 + dx * 18;
    else
        d = dx * 7 + dy * 18;
    if (d < radius)
        return 0;
    return d - radius;
}

// Constructor of an OrderFx subclass (vtable 0x4fd358, the same class
// as 0x44d470's file-loading constructor) that stores a point converted
// from fixed-point world coordinates relative to the map origin, two radii
// and each (radius / 16) squared. Two-radius version of 0x44cf60.
// FUNCTION: 0x44d3b0 ??0RingApproach@@QAE@PAUSource_0044d3b0@@HHHH@Z
RingApproach::RingApproach(Source_0044d3b0* source, int x, int y, int r1, int r2)
    : OrderFx((int)source)
{
    vtable = g_ringApproachVtable;
    Point_0044d3b0 org = source->map->origin;
    Point_0044eec0 p;
    p.x = (x - (org.x << 19) + 0x80000) >> 20;
    p.y = (y - (org.y << 19) + 0x80000) >> 20;
    pos = p;
    // Stays after the point conversion.
    radius2 = r2;
    radius1 = r1;
    radius1Sq = (r1 / 16) * (r1 / 16);
    radius2Sq = (r2 / 16) * (r2 / 16);
}

// FUNCTION: 0x44d440
int RingApproach::GetType()
{
    return 5;
}

// FUNCTION: 0x44d450
void* RingApproach::Destroy(unsigned char flag)
{
    vtable = g_orderFxVtable;
    if ((flag & 1) != 0) {
        operator delete(this);
    }
    return this;
}

// Constructor of an OrderFx subclass that reads a 24-byte header from
// a named entry of an open file and keeps its last five dwords.
// FUNCTION: 0x44d470 ??0RingApproach@@QAE@HPAVHapiBank@@PAD@Z
RingApproach::RingApproach(int owner, HapiBank* file, char* name)
    : OrderFx(owner)
{
    vtable = g_ringApproachVtable;
    int bad = 0;
    if (owner == 0 || file == 0)
        bad = 1;
    if (name == 0)
        bad = 1;
    if (!bad) {
        file->OpenNamedBox(name);
        file->SeekBox(0);
        Header_0044d470 hdr;
        if (file->ReadBox(&hdr, 24) == 24) {
            data.a = hdr.data.a;
            data.b = hdr.data.b;
            data.c = hdr.data.c;
            data.d = hdr.data.d;
            data.e = hdr.data.e;
        }
    }
}

// Saving counterpart of 0x44d470's constructor: writes a 24-byte header
// whose last five dwords are the stored values. The first dword is left
// uninitialised, exactly as in 0x44d090 and 0x44d9a0.
// FUNCTION: 0x44d500
int RingApproach::Serialize(int unused, HapiBank* file, char* name)
{
    Header_0044d470 hdr;
    hdr.data.a = data.a;
    hdr.data.b = data.b;
    hdr.data.c = data.c;
    hdr.data.d = data.d;
    hdr.data.e = data.e;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 24);
    return 1;
}

// Clears a std::vector<Point_0044eec0> and then appends a copy of the point
// at +0x8, with its y moved by (radius2 + radius1) / 32.
// FUNCTION: 0x44d560
void RingApproach::AppendGoalCell(Vec_0044d560* list)
{
    list->clear();
    Point_0044eec0 p = pos;
    // Sum written radius1 + radius2: the operand order follows the load order.
    p.y = p.y + (radius1 + radius2) / 32;
    list->push_back(p);
}

// Virtual method of the ring area class (compare 0x44d2c0, which writes the
// same centre): writes the centre as 16.16 world coordinates, then moves it
// back towards the owner by the mean of the two radii at +0xc and +0x10.
// The mean needs its own inline helper to load +0xc first.

static inline void ToWorld(Vec3_0044d720* out, Point_0044eec0 a, Point_0044d720 b)
{
    out->x = (a.x * 2 + b.x) << 19;
    out->z = (a.y * 2 + b.y) << 19;
}

// Inlined copy of DirectionFromAngle.
static inline Vec3_0044d720 Direction(short angle, int scale)
{
    Vec3_0044d720 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    // The casts stay: without them 0x44d720's radius loads come out in the other order.
    v.z = -FUN_004b7123((short)angle, (int)scale);
    return v;
}

static inline int MeanRadius(RingApproach* c)
{
    // The locals keep the loads of the inner radius first; a single sum commutes them.
    int r1 = c->inner;
    int r2 = c->outer;
    return (r1 + r2) / 2;
}

// FUNCTION: 0x44d720
int RingApproach::FillWorldPos(Vec3_0044d720* out)
{
    Inner_0044d720* inner = ((Owner_0044d720*)source)->inner;
    ToWorld(out, pos, inner->cell);
    short angle = GetHeadingBetween(&inner->pos, out);
    Vec3_0044d720 d = Direction(angle, MeanRadius(this) << 16);
    out->x -= d.x;
    out->y -= d.y;
    out->z -= d.z;
    return 1;
}

// FUNCTION: 0x44d7c0
bool RingApproach::ContainsCell(int px, int py)
{
    int dy = py - y;
    int dx = px - x;
    int distSq = dx * dx + dy * dy;
    return distSq <= radius1Sq && distSq >= radius2Sq;
}

// Same test as 0x44d7c0, on a unit's position: 1 when the squared distance
// from the centre lies within [radius2Sq, radius1Sq].
// FUNCTION: 0x44d800
int RingApproach::ContainsUnit(Unit* unit)
{
    int dy = unit->y - y;
    int dx = unit->x - x;
    int distSq = dx * dx + dy * dy;
    return distSq <= radius1Sq && distSq >= radius2Sq;
}

// Approximate distance from (px, py) to the ring around (x, y) with the
// inner radius +0xc and the outer radius +0x10, 0 between the radii.
// Same 18 * major + 7 * minor metric as 0x44d350.
// FUNCTION: 0x44d840
int RingApproach::ApproxDistExcess(int px, int py)
{
    int dx = abs(px - x);
    int dy = abs(py - y);
    int d;
    if (dx > dy)
        d = dy * 7 + dx * 18;
    else
        d = dx * 7 + dy * 18;
    if (d > outer)
        return d - outer;
    if (d < inner)
        return inner - d;
    return 0;
}

// Second constructor of the OrderFx subclass with vtable 0x4fd388
// (compare 0x44d930): the rectangle comes from a position, made relative to
// the owner's view origin, and a size.
// FUNCTION: 0x44d8a0 ??0PointMarker@@QAE@PAUOwner_0044d8a0@@UPoint_0044d8a0@@1@Z
PointMarker::PointMarker(Owner_0044d8a0* owner, Point_0044d8a0 pos, Point_0044d8a0 size)
    : OrderFx((int)owner)
{
    vtable = g_pointMarkerVtable;
    a = pos.x - owner->view->x;
    c = pos.y - owner->view->y;
    b = size.x + pos.x;
    d = size.y + pos.y;
}

// FUNCTION: 0x44d900
int PointMarker::GetType()
{
    return 6;
}

// FUNCTION: 0x44d910
void* PointMarker::Destroy(unsigned char should_delete)
{
    PointMarker* esi = this;
    esi->vtable = g_orderFxVtable;
    if (should_delete & 1) {
        operator delete(esi);
    }
    return esi;
}

// Constructor of an OrderFx subclass that reads a 20-byte header from
// a named entry of an open file and keeps its last four dwords.
// FUNCTION: 0x44d930 ??0PointMarker@@QAE@HPAVHapiBank@@PAD@Z
PointMarker::PointMarker(int owner, HapiBank* file, char* name)
    : OrderFx(owner)
{
    vtable = g_pointMarkerVtable;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    Header_0044d930 hdr;
    if (file->ReadBox(&hdr, 20) == 20) {
        r.a = hdr.r.a;
        r.b = hdr.r.b;
        r.c = hdr.r.c;
        r.d = hdr.r.d;
    }
}

// Saving counterpart of 0x44d930's constructor: writes a 20-byte
// header whose last four dwords are the stored values (the first dword is
// left uninitialised, as in the original), like 0x44d090.
// FUNCTION: 0x44d9a0
int PointMarker::Serialize(int unused, HapiBank* file, char* name)
{
    Header_0044d930 hdr;
    hdr.r.a = r.a;
    hdr.r.b = r.b;
    hdr.r.c = r.c;
    hdr.r.d = r.d;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 20);
    return 1;
}

static inline int MidX_44dc60(PointMarker* w)
{
    return (w->x1 + w->x2) / 2;
}

// FUNCTION: 0x44dc60
int PointMarker::FillWorldPos(int* out)
{
    short avg = (short)MidX_44dc60(this);
    short z = (short)y2;
    // The named view's mapPtr (+4) is the base class's owner field.
    MapInfo_44dc60* info = *(MapInfo_44dc60**)((char*)source + 0xe);
    Pos16_44dc60 pos = info->pos;
    out[0] = (pos.x + avg * 2) << 0x13;
    out[2] = (pos.z + z * 2) << 0x13;
    return 1;
}

// Same rectangle layout as 0x44dc60 (x1/x2 at +8/+0xc, y1/y2 at +0x10/+0x14):
// is the point (x, y) on the rectangle's border?
// FUNCTION: 0x44dcb0
int PointMarker::ContainsCell(int x, int y)
{
    if ((x == x1 || x == x2) && y >= y1 && y <= y2) {
        return 1;
    }
    if ((y == y1 || y == y2) && x >= x1 && x <= x2) {
        return 1;
    }
    return 0;
}

// Same rectangle layout as 0x44dc60/0x44dcb0 (x1/x2 at +8/+0xc, y1/y2 at
// +0x10/+0x14): approximate distance from (x, y) to the rectangle, 0 inside.
// Outside a corner it is 16 * major + 6 * minor axis distance (compare the
// 18/7 ring metric of 0x44d350/0x44d840); straight out from an edge it is
// 16 * distance; inside it is 16 * distance to the nearest edge.

// Stays a macro: as a function or an if chain the first argument gets another register.
#define Min_0044dd00(a, b) ((a) < (b) ? (a) : (b))

// FUNCTION: 0x44dd00
int PointMarker::ApproxDist(int x, int y)
{
    int dx;
    if (x < x1) {
        dx = x1 - x;
    }
    else if (x > x2) {
        dx = x - x2;
    }
    else {
        if (y < y1)
            return (y1 - y) * 16;
        if (y > y2)
            return (y - y2) * 16;
        int mx = Min_0044dd00(x - x1, x2 - x);
        int my = Min_0044dd00(y - y1, y2 - y);
        return Min_0044dd00(mx, my) * 16;
    }

    int dy;
    if (y < y1)
        dy = y1 - y;
    else if (y > y2)
        dy = y - y2;
    else
        return dx * 16;

    if (dx > dy)
        return dx * 16 + dy * 6;
    return dy * 16 + dx * 6;
}

// Slot 0 of vtable 0x4fd3e0: writes the flag word and, per flag bit, the
// short fields (or the three dwords) to a bit stream.
// FUNCTION: 0x44ddc0
void PathOrder::SerializeToBits(BitWriter* stream)
{
    stream->WriteBits(flags, 8);
    if ((flags & 1) != 0) {
        stream->WriteBits(piece, 0x10);
        stream->WriteBits((int)(unsigned short)(ref.owner == 0 ? 0 : (unsigned short)ref.owner->id), 0x10);
    }
    if ((flags & 0x10) != 0) {
        stream->WriteBits(approachRadius, 0x10);
    }
    if ((flags & 8) != 0) {
        stream->WriteBits(altitude, 0x10);
    }
    if ((flags & 0x40) != 0) {
        stream->WriteBits(heading, 0x10);
    }
    if ((flags & 0x20) != 0) {
        stream->WriteBits(pos.x, 0x20);
        stream->WriteBits(pos.y, 0x20);
        stream->WriteBits(pos.z, 0x20);
    }
}

// Loading counterpart of the virtual save method 0x44dfb0 (vtable
// g_pathOrderVtable, slot 1): reads a 0x36-byte record from a named entry of an
// open file and copies its fields into this object. The record it reads is
// built by 0x44dfb0 and by 0x44e330 / 0x44e250.
// FUNCTION: 0x44de80 ??0PathOrder@@QAE@HPAVHapiBank@@PAD@Z
PathOrder::PathOrder(int owner, HapiBank* file, char* name)
    : OrderFx(owner), ref(0, 0)
{
    vtable = g_pathOrderVtable;
    Rec_0044de80 rec;
    ((PathOrderAttach*)&rec.ref_vt)->PathOrderAttach::PathOrderAttach(0, 0);
    file->OpenNamedBox(name);
    file->SeekBox(0);
    if (file->ReadBox(&rec, 0x36) == 0x36) {
        unit = LoadUnit(rec.id1, file);
        ref.SetUnit((Unit*)LoadUnit(rec.id2, file));
        field_8 = rec.f1;
        approachRadius = rec.f2;
        altitude = rec.f3;
        heading = rec.f4;
        piece = rec.f5;
        pos = *(Vec3_0044e3c0*)&rec.pos;
        followOffset = rec.i4;
    }
    ((UnitRef*)&rec.ref_vt)->Unlink();
}

// FUNCTION: 0x44df70
int PathOrder::GetType()
{
    return 2;
}

// Scalar-deleting-destructor shape: unlink the embedded list node at +0x16
// (via UnitRef::Unlink, the same unlink method used elsewhere),
// restore this object's own vtable, conditionally operator delete, and
// return `this`.
// FUNCTION: 0x44df80
void* PathOrder::Destroy(unsigned char flag)
{
    ((UnitRef*)((char*)this + 0x16))->Unlink();
    vtable = g_orderFxVtable;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

#pragma pack(push, 2)
struct Rec_0044dfb0 {
    int unknown_0;                  // +0x0
    int unknown_4;                  // +0x4
    short id1;                      // +0x8
    void* ref_vt;                   // +0xa (PathOrderAttach)
    void* ref_owner;                // +0xe
    void* ref_next;                 // +0x12
    int ref_value;                  // +0x16
    short id2;                      // +0x1a
    short f1;                       // +0x1c
    short f2;                       // +0x1e
    short f3;                       // +0x20
    short f4;                       // +0x22
    short f5;                       // +0x24
    Vec3_0044e3c0 pos;              // +0x26
    int i4;                         // +0x32
};
#pragma pack(pop)

// Virtual save method (vtable 0x4fd3b8, slot 1) for the class built by
// 0x44e330 / 0x44e250. Builds a 0x36-byte record on the stack, writing the
// referenced unit's id, the embedded link's owner id, five flag shorts and
// the stored position, then writes it to the file via
// HapiBank::WriteBox. The read counterpart is 0x44de80.
//
// The record's first 8 bytes are never assigned and are still written out:
// 0x36 bytes of stack, 8 of them uninitialised, go to the save file. The read
// counterpart (0x44de80) reads all 0x36 bytes but never looks at 0..7, so the
// bytes are only leaked, never used.
// FUNCTION: 0x44dfb0
int PathOrder::SerializeToSave(int unused, HapiBank* file, char* name)
{
    // The first 8 bytes of rec stay unassigned, as in the original.
    Rec_0044dfb0 rec;
    PathOrderAttach* ref = (PathOrderAttach*)&rec.ref_vt;
    ref->PathOrderAttach::PathOrderAttach(0, 0);
    if (unit == 0)
        rec.id1 = 0;
    else
        rec.id1 = unit->id;
    if (this->ref.owner == 0)
        rec.id2 = 0;
    else
        rec.id2 = this->ref.owner->id;
    rec.f1 = flags;
    rec.f2 = approachRadius;
    rec.f3 = altitude;
    rec.f4 = heading;
    rec.f5 = piece;
    rec.pos = pos;
    rec.i4 = followOffset;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&rec, 0x36);
    ((UnitRef*)&rec.ref_vt)->Unlink();
    return 1;
}

// FUNCTION: 0x44e080 ??0PathOrder@@QAE@PAUOwner_0044e080@@PAVBitReader@@@Z
PathOrder::PathOrder(Owner_0044e080* owner_, BitReader* reader)
    : OrderFx(0), owner(owner_), ref(0, 0)
{
    vtable = g_pathOrderVtable;
    flags = reader->ReadBits(8);
    if (flags & 1) {
        piece = reader->ReadBits(0x10);
        unsigned short index = reader->ReadBits(0x10);
        ref.SetUnit(index == 0 ? 0 : (Unit*)&g_game->units[index]);
    }
    if (flags & 0x10)
        approachRadius = reader->ReadBits(0x10);
    else
        approachRadius = 0;
    if (flags & 8)
        altitude = reader->ReadBits(0x10);
    else
        altitude = 0;
    if (flags & 0x40)
        heading = reader->ReadBits(0x10);
    else
        heading = 0;
    if (flags & 0x20) {
        pos_x = reader->ReadBits(0x20);
        pos_y = reader->ReadBits(0x20);
        pos_z = reader->ReadBits(0x20);
    }
}

// Another constructor of the class built by 0x44e250 / 0x44e2d0 / 0x44e330
// (vtable g_pathOrderVtable): it copies the position from the order's unit and
// picks a type of 7 or 1 from the unit definition's flag bit 11.
// FUNCTION: 0x44e190 ??0PathOrder@@QAE@PAUOrder@@PAUUnit@@@Z
PathOrder::PathOrder(Order* order, Unit* unit)
    : OrderFx((int)order), ref(0, 0)
{
    vtable = g_pathOrderVtable;
    ref.SetUnit((Unit*)unit);
    approachRadius = 0;
    altitude = 0;
    this->unit = order->unit;
    pos = this->unit->pos;
    piece = -1;
    if ((unsigned char)(ref.owner->def->flags >> 11) & 1) {
        field_8 = 7;
        int t = this->unit->weapon->range;
        if (t != 0)
            followOffset = t << 16;
        else
            followOffset = 0x640000;
    } else {
        field_8 = 1;
    }
}

// A second constructor for the class built by 0x44e330 (same vtable
// g_pathOrderVtable): it takes its position from the source's unit instead.
// FUNCTION: 0x44e250 ??0PathOrder@@QAE@PAUSource_0044e250@@HF@Z
PathOrder::PathOrder(Source_0044e250* source, int unit, short value)
    : OrderFx((int)source), piece(value), ref(0, 0)
{
    vtable = g_pathOrderVtable;
    approachRadius = 0;
    altitude = 0;
    field_8 = 5;
    this->unit = source->unit;
    pos = this->unit->pos;
    ref.SetUnit((Unit*)unit);
}

// Another constructor of the class built by 0x44e330 and 0x44e250 (vtable
// g_pathOrderVtable), with a type of 0x20 and the position passed in.
// FUNCTION: 0x44e2d0 ??0PathOrder@@QAE@PAUSource_0044e2d0@@ABUVec3_0044e2d0@@@Z
PathOrder::PathOrder(Source_0044e2d0* source, const Vec3_0044e2d0& p)
    : OrderFx((int)source), ref(0, 0), pos(*(const Vec3_0044e3c0*)&p)
{
    vtable = g_pathOrderVtable;
    approachRadius = 0;
    altitude = 0;
    piece = -1;
    field_8 = 0x20;
    unit = (Unit*)source->unit;
}

// FUNCTION: 0x44e330 ??0PathOrder@@QAE@PAUSource_0044e330@@HABUVec3_0044e330@@@Z
PathOrder::PathOrder(Source_0044e330* source, int unit, const Vec3_0044e330& p)
    : OrderFx((int)source), ref(0, 0)
{
    vtable = g_pathOrderVtable;
    ref.SetUnit((Unit*)unit);
    pos = *(const Vec3_0044e3c0*)&p;
    approachRadius = 0;
    altitude = 0;
    piece = -1;
    this->unit = (Unit*)source->unit;
    field_8 = 0xa3;
}

// FUNCTION: 0x44e3a0
int PathOrder::KeepAfterComplete() {
    if ((flags & 1) == 0 || ref.owner == 0) {
        return 0;
    }
    return 1;
}

// Virtual method (g_pathOrderVtable slot) of the OrderFx family: works out
// where the object should be. When the flags say it is active and not fully
// set, it aims at the reference unit's predicted position, optionally adding
// a direction offset from the fixed-point trig helpers; otherwise it just
// places itself at terrain level using the unit definition's height.
static inline Vec3_0044e3c0 Direction_0044e3c0(short angle, int scale)
{
    Vec3_0044e3c0 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

// FUNCTION: 0x44e3c0
int PathOrder::FillWorldPos(Vec3_0044e3c0* out)
{
    unsigned short f = flags;
    if ((f & 1) && !(f & 0x80)) {
        if (ref.owner == 0 || ref.owner->spatialBucket == g_game->overflowBucket)
            return 0;
        // Through a local pointer: keeps the struct-assignment destination in edi.
        Vec3_0044e3c0* p = &pos;
        *p = GetPiecePosition(ref.owner, piece);
        if (flags & 2) {
            short angle = ref.owner->heading;
            if (flags & 0x40)
                angle += heading;
            Vec3_0044e3c0 d = Direction_0044e3c0(angle, followOffset);
            p->x -= d.x;
            p->y -= d.y;
            p->z -= d.z;
        }
        pos.y += altitude << 16;
    } else {
        if ((f & 8) == 0) {
            UnitDef* def = unit->def;
            if (def->flag22)
                pos.y = (def->altitude + g_game->seaLevel) << 16;
            else
                pos.y = (def->altitude + *(unsigned char*)(unit->spatialBucket + 1)) << 16;
        }
    }
    if (pos.y > 0x1ff0000)
        pos.y = 0x1ff0000;
    *out = pos;
    return 1;
}

// FUNCTION: 0x44e530
int PathOrder::GetDesiredHeading(unsigned short* out)
{
    unsigned short f = flags;
    if ((f & 0x84) && ref.owner != 0) {
        if (f & 2) {
            *out = GetHeadingBetween(&unit->pos, &ref.owner->pos);
            return 1;
        }
        if (f & 0x40) {
            *out = heading;
            return 1;
        }
        *out = ref.owner->heading;
        return 1;
    }
    if (f & 0x40) {
        *out = heading;
        return 1;
    }
    return 0;
}

// A virtual method of the big class whose vtable starts at 0x4fd2f8 (it shares
// that vtable with 0x44e530, 0x44e3a0 and friends). It fetches a position from
// itself through vtable slot +0x20, measures the horizontal (x/z) distance to
// `arg`, converts it from 16.16 to pixels, then tests range/flags.
// FUNCTION: 0x44e5b0
int Class_0044e5b0::IsComplete(Object_0044e5b0* arg)
{
    Vec3_0044e5b0 pos;
    v8(&pos);
    float dist = (float)_hypot((double)(arg->pos.x - pos.x), (double)(arg->pos.z - pos.z)) * 1.52587890625e-05f;
    unsigned short flags = this->flags;
    if (flags & 0x10) {
        return (double)approachRadius > dist;
    }
    else {
        if (dist > 0.5f)
            return 0;
        if ((flags & 1) && target == 0)
            return 0;
        if ((flags & 4) && arg->heading != target->heading)
            return 0;
        if ((flags & 8) && abs(arg->pos.y - pos.y) > 0x10000)
            return 0;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x44e6b0
int PathOrder::IsFxStyle()
{
    return 0;
}

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x44e6c0
void PathOrder::SetAltitude(int param_1)
{
    field_8 |= 8;
    altitude = param_1;
    if ((unsigned char)field_8 & 0x20) {
        pos.y = (max(GetGroundHeight((Pos_0044e6c0*)&pos), g_game->seaLevel) + param_1) << 16;
        if (pos.y > 0x1ff0000)
            pos.y = 0x1ff0000;
    }
}

// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.
// FUNCTION: 0x44e720
void PathOrder::SetHeading(short v)
{
    headingFlag = 1;
    heading = v;
}

// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.
// FUNCTION: 0x44e730
void PathOrder::SetApproachRadius(short v)
{
    radiusFlag = 1;
    approachRadius = v;
}

// The constructor.
// FUNCTION: 0x44e740 ??0AirManeuverOrder@@QAE@PAUSource_0044e740@@ABUVec3_0044e740@@1@Z
AirManeuverOrder::AirManeuverOrder(Source_0044e740* source, const Vec3_0044e740& a, const Vec3_0044e740& b)
    : OrderFx((int)source)
{
    vtable = g_airManeuverOrderVtable;
    self = source->unit;
    flags = 0;
    saveOnly22 = 0;
    target = a;
    other = b;
}

// FUNCTION: 0x44e7a0
int AirManeuverOrder::GetType()
{
    return 3;
}

// FUNCTION: 0x44e7b0
void* AirManeuverOrder::Destroy(int param_1)
{
    vtable = &g_orderFxVtable;
    if ((param_1 & 1) != 0) {
        operator delete(this);
    }
    return this;
}

// The load constructor: reads a 0x2a-byte
// header from a named entry of an open file, then looks up the unit it names.
// FUNCTION: 0x44e7d0 ??0AirManeuverOrder@@QAE@HPAVHapiBank@@PAD@Z
AirManeuverOrder::AirManeuverOrder(int owner, HapiBank* file, char* name)
{
    source = owner;
    vtable = g_airManeuverOrderVtable;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    Header_0044e740 hdr;
    if (file->ReadBox(&hdr, 0x2a) == 0x2a) {
        self = (Object_0044e740*)LoadUnit(hdr.id, file);
        flags = hdr.flag;
        target = hdr.target;
        other = hdr.other;
        saveOnly22 = hdr.value_24;
        heading = hdr.heading;
        value_26 = hdr.value_28;
    }
}

// Slot 1, the saving counterpart of the
// loading constructor 0x44e7d0: packs this object's fields into a 0x2a-byte
// header (whose first eight bytes are left as they are, exactly as 0x44d500
// leaves its magic dword alone) and appends it to the file. The reader takes
// the id as a dword but only its low word (0x487080 masks with 0xffff), so
// writing it as a word is harmless.
// FUNCTION: 0x44e880
int AirManeuverOrder::SerializeToSave(int unused, HapiBank* file, char* name)
{
    Header_0044e740 hdr;
    // if/else, not a ternary: puts the store of 0 on the fallthrough path.
    if (!self) {
        hdr.unit_id = 0;
    } else {
        hdr.unit_id = self->id;
    }
    hdr.flag = flags;
    hdr.target = target;
    hdr.other = other;
    hdr.value_24 = saveOnly22;
    hdr.heading = heading;
    hdr.value_28 = value_26;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    file->WriteBox(&hdr, 0x2a);
    return 1;
}

// Writes the flag word, the two
// points, and (when flag bit 0 is set) the word at +0x24 to a bit stream.
// FUNCTION: 0x44e930
void AirManeuverOrder::SerializeToBits(BitWriter* stream)
{
    stream->WriteBits(flags, 1);
    stream->WriteBits(target.x, 0x20);
    stream->WriteBits(target.y, 0x20);
    stream->WriteBits(target.z, 0x20);
    stream->WriteBits(other.x, 0x20);
    stream->WriteBits(other.y, 0x20);
    stream->WriteBits(other.z, 0x20);
    if ((flags & 1) != 0) {
        stream->WriteBits(heading, 0x10);
    }
}

// FUNCTION: 0x44e9c0 ??0AirManeuverOrder@@QAE@PAUOwner_0044e9c0@@PAVBitReader@@@Z
AirManeuverOrder::AirManeuverOrder(Owner_0044e9c0* owner, BitReader* reader)
{
    source = 0;
    self = (Object_0044e740*)owner;
    vtable = g_airManeuverOrderVtable;
    flags = reader->ReadBits(1);
    target.x = reader->ReadBits(0x20);
    target.y = reader->ReadBits(0x20);
    target.z = reader->ReadBits(0x20);
    other.x = reader->ReadBits(0x20);
    other.y = reader->ReadBits(0x20);
    other.z = reader->ReadBits(0x20);
    if (flags & 1) {
        heading = reader->ReadBits(0x10);
    }
}

// FUNCTION: 0x44ea50
int AirManeuverOrder::KeepAfterComplete()
{
    return 0;
}

// Slot 8 of AirManeuverOrder (vtable 0x4fd3f8, constructor 0x44e740), the slot
// before 0x44eb40: copies the first point out, advances it by the second point
// and, when flag bit 0 is set, turns the second point (a step per frame) towards
// the stored heading, by at most an eighth of the unit type's turn rate.
// The rotation helper takes a pair of ints, so the (x, z) step is built in the
// local's x and y fields and read back from y into other.z.
// FUNCTION: 0x44ea60
int AirManeuverOrder::FillWorldPos(Vec3_0044e740* out)
{
    *out = target;
    target.x += other.x;
    target.z += other.z;
    Vec3_0044e740 v;
    v = Vec3_0044e740(0, 0, 0);
    unsigned short h = GetHeadingBetween(&v, &other);
    if ((flags & 1) && h != heading) {
        short diff = heading - h;
        v.x = other.x;
        v.y = other.z;
        short step = diff;
        unsigned short max = ((Object_0044ea60*)self)->type->max_turn;
        if (step >= (max >> 3))
            step = max >> 3;
        else if (step <= -(max >> 3))
            step = -(max >> 3);
        FUN_004b7173(-step, (int*)&v);
        other.x = v.x;
        other.z = v.y;
        other.y = 0;
    }
    return 1;
}

// Virtual slot 9 (vtable at 0x4fd3f8), the same slot as 0x44e530: writes the
// heading from the owner's position (+0x6a) to the target point at +0xa.
// FUNCTION: 0x44eb40
int AirManeuverOrder::GetDesiredHeading(unsigned short* out)
{
    *out = GetHeadingBetween(&self->pos, &target);
    return 1;
}

// Slot 4: done
// when the unit is within 48 world units of the target point, or, when flag
// bit 0 is set, when the heading from the origin to the second point equals
// the stored heading.
// FUNCTION: 0x44eb60
int AirManeuverOrder::IsComplete(Object_0044e740* unit)
{
    if ((float)_hypot(unit->pos.x - target.x, unit->pos.z - target.z) / 65536.0f < 48.0f)
        return 1;
    if (flags & 1) {
        Vec3_0044e740 origin;
        origin = Vec3_0044e740(0, 0, 0);
        if (GetHeadingBetween(&origin, &other) == heading)
            return 1;
    }
    return 0;
}

// FUNCTION: 0x44ec00
int AirManeuverOrder::IsFxStyle()
{
    return 0;
}

// An empty method: its one caller (0x412d40) calls it on
// the object it has just built with that class's constructor (0x44e740).
// FUNCTION: 0x44ec10
void AirManeuverOrder::SetAltitude(int)
{
}

// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.
// FUNCTION: 0x44ec20
void AirManeuverOrder::SetHeading(short v)
{
    headingSet = 1;
    heading = v;
}

// std::vector<Point_0044eec0>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial.
// FUNCTION: 0x44ee60 ?_Destroy@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEXPAUPoint_0044eec0@@0@Z
DestroyFn_0044ee60 Access_0044ee60::fn = &Access_0044ee60::_Destroy;

// std::vector<Point_0044eec0>::size() for the 4-byte point (two shorts); its
// callers are the vector insert paths in 0x44d0e0 and 0x44d560.
// FUNCTION: 0x44ee70 ?size@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@QBEIXZ
SizeFn_0044ee70 g_size_0044ee70 = &Vec_0044ee70::size;

// std::vector<Point_0044eec0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copy-constructs [first, last) into raw storage at dest and
// returns the end of the copies. Its callers (around 0x44d190) set ecx to the
// vector whose _Ufill is 0x44eec0.
// FUNCTION: 0x44ee90 ?_Ucopy@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEPAUPoint_0044eec0@@PBU3@0PAU3@@Z
UcopyFn_0044ee90 Access_0044ee90::fn = &Access_0044ee90::_Ucopy;

// std::vector<Point_0044eec0>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage. Its callers (around
// 0x44d190) set ecx to the vector; its _Ucopy is 0x44ee90 and its copy is
// 0x44eef0 (4-byte elements, two shorts).
// FUNCTION: 0x44eec0 ?_Ufill@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEXPAUPoint_0044eec0@@IABU3@@Z
UfillFn_0044eec0 Access_0044eec0::fn = &Access_0044eec0::_Ufill;

// std::copy for 4-byte elements (two shorts), compiled with __stdcall as the
// default convention. Its one caller (0x44da00) is an inlined
// vector::erase(begin(), end()) whose _Destroy is 0x44ee60. Not a member of
// the PathGoal family next to it: it takes no `this`.
// FUNCTION: 0x44eef0
Point_0044eef0* __stdcall CopyDwordRangeUnchecked(Point_0044eef0* first, Point_0044eef0* last, Point_0044eef0* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}

// The base of a class family (vtable 0x4fd428, 11 slots; constructor
// 0x44ef20, ??_G 0x44ef60). Derived classes, all with this class's
// declaration copied verbatim:
//   AiSearchGoal  vtable 0x4fd458  ctor 0x44f010  dtor 0x44f450  ??_G 0x44f040
//   PatrolGoal  vtable 0x4fd488  ctor 0x44f570  ??_G 0x44f590
//   PackedGoal  vtable 0x4fd980  ctor 0x4905e0  ??_G 0x490630
//     LiteGoal  vtable 0x4fd9b0  ctor 0x4907e0  ??_G 0x490840
//     PackedPosGoal  vtable 0x4fd9e0  ctor 0x490940  dtor 0x4909e0  ??_G 0x4909a0

// The constructor.
// FUNCTION: 0x44ef20
// FUNCTION: 0x44ef60 ??_GPathGoal@@UAEPAXI@Z
PathGoal::PathGoal(Struct_004907e0* p)
{
    owner = p;
    target = 0;
}

// Slot 3: does nothing.
// FUNCTION: 0x44ef40
void PathGoal::FillWaypointWorldPos(Vec3_004907e0*, int, int)
{
}

// Slot 10: does nothing.
// FUNCTION: 0x44ef50
void PathGoal::DrawOnSurface(void*)
{
}

// Slot 5: whether there is an object at +0x4.
// FUNCTION: 0x44ef80
int PathGoal::HasReadyWaypoints()
{
    return target != 0;
}

// Slot 1: sets the object at +0x4, first telling the one it replaces 0x80.
// FUNCTION: 0x44ef90
void PathGoal::SetPathOrder(void* param)
{
    if (target != 0) {
        ((OrderFx*)target)->AddFlags(0x80);
    }
    target = (Base_00490a10*)param;
}

// Slot 2: does nothing.
// FUNCTION: 0x44efb0
void PathGoal::TickTowardGoal()
{
}

// Slot 8: does nothing.
// FUNCTION: 0x44efc0
void PathGoal::SerializeNetUnitState(BitWriter*)
{
}

// Slot 9: does nothing.
// FUNCTION: 0x44efd0
void PathGoal::DeserializeNetUnitState(BitReader*)
{
}

// Slot 7: 0.
// FUNCTION: 0x44efe0
int PathGoal::HasNetUnitState()
{
    return 0;
}

// Slot 6: none.
// FUNCTION: 0x44eff0
AiSearchGoal* PathGoal::TryClaimRepath()
{
    return 0;
}

// Slot 4: does nothing.
// FUNCTION: 0x44f000
void PathGoal::ExportGoalPose(Vec3_004907e0*, Vec3_004907e0*, short*)
{
}

// The constructor.
// FUNCTION: 0x44f010
AiSearchGoal::AiSearchGoal(Struct_004907e0* p)
    : PathGoal(p)
{
    count = 0;
    active = 0;
    flag_1 = 0;
    flag_3 = 1;
    repathClaimTick = 0;
}

// Sets the path points (at most 20) and marks it active, or with no
// points asks the object at +0x4 about the owner and flags it (0x40) when that
// fails.
// FUNCTION: 0x44f080
void AiSearchGoal::SetWaypoints(Point_0044f080* src, int n)
{
    if (n == 0) {
        if (target && target->ContainsUnit(owner) == 0)
            ((OrderFx*)target)->AddFlags(0x40);
        active = 0;
    } else {
        if (n >= 20)
            n = 20;
        count = n;
        std::copy(src, src + n, points);
        active = 1;
    }
    flag_1 = 0;
    flag_3 = 1;
}

// Drops the first n path points, clears the active flag when fewer
// than two are left and sets flag 3.
// FUNCTION: 0x44f100
void AiSearchGoal::TruncateWaypointsFrom(int n)
{
    if (n != 0) {
        std::copy(points + n, points + count, points);
        count -= n;
        if (count < 2)
            active = 0;
        flag_3 = 1;
    }
}

// Slot 3: fills n positions from the path points, repeating the last point past the
// end of the path.
// FUNCTION: 0x44f150
void AiSearchGoal::FillWaypointWorldPos(Vec3_004907e0* out, int unused, int n)
{
    for (int i = 0; i < n; i++) {
        int j = i < count ? i : count - 1;
        out[i].x.fixed = points[j].x << 16;
        out[i].y.fixed = 0;
        out[i].z.fixed = points[j].y << 16;
    }
}

// Slot 2: tells the object at +0x4 about the owner, then, when the unit has reached
// its second path point (within 5 units of the owner), drops that point with
// an overlapping std::copy and refreshes the flags.
// FUNCTION: 0x44f1a0
void AiSearchGoal::TickTowardGoal()
{
    if (target) {
        if (target->ContainsUnit(owner)) {
            ((OrderFx*)target)->AddFlags(0x20);
            if (!target->KeepAfterComplete())
                SetPathOrder(0);
        }
    }
    if (count >= 2) {
        int dx = owner->pos.z.half[1] - points[1].y;
        int dy = owner->pos.x.half[1] - points[1].x;
        if (dy * dy + dx * dx <= 25) {
            // std::copy, not memmove: memmove stays a library call.
            std::copy(points + 1, points + count, points);
            int n = --count;
            if (n < 2)
                active = 0;
            flag_3 = 1;
        }
    }
    if (target && (owner->target->occupyFlags & 4 || count < 2))
        flag_1 = 1;
}

// Slot 6: returns this object when its flag bit 1 is set and the counter at
// g_game+0x38a47 has reached repathClaimTick + 0x3c (storing the counter in
// repathClaimTick), or null. The path search scheduler (0x40eb70) calls it through
// slot 6 to pick the path to search for next.
// FUNCTION: 0x44f260
AiSearchGoal* AiSearchGoal::TryClaimRepath()
{
    if (flags & 2) {
        unsigned int limit = g_game->gameTick;
        if (limit >= repathClaimTick + 0x3c) {
            repathClaimTick = limit;
            return this;
        }
    }
    return 0;
}

// Slot 5: the active flag (bit 0 of +0x64).
// FUNCTION: 0x44f290
int AiSearchGoal::HasReadyWaypoints()
{
    return flags & 1;
}

// Slot 1: hands the object at +0x4 over to the
// path logic, then, unless the path is already active, steps the path
// forwards: it asks the object for its position (its slot 8) and marks the
// path active when the next path point is at most half as far from that
// object as the owner is. With no usable point left it resets the path to
// two points taken from the owner and the object's position.
// FUNCTION: 0x44f2a0
void AiSearchGoal::SetPathOrder(void* param)
{
    g_game->pathfinder->AbortIfGoalMatch(this);
    if (target)
        ((OrderFx*)target)->AddFlags(0x80);
    active = 0;
    target = (Base_00490a10*)param;
    if (param == 0) {
        flag_1 = 0;
    } else {
        flag_1 = 1;
        if (count >= 3) {
            if (target->ContainsCell(points[count - 1].x >> 4, points[count - 1].y >> 4)) {
                active = 1;
                flag_1 = 0;
            }
        }
        if (!active) {
            Vec3_004907e0 p;
            if (target->FillWorldPos(&p)) {
                if (count >= 3) {
                    int sx = points[count - 1].x << 16;
                    int sz = points[count - 1].y << 16;
                    int d1 = (int)_hypot(owner->pos.x.fixed - p.x.fixed,
                                         owner->pos.z.fixed - p.z.fixed);
                    int d2 = (int)_hypot(sx - p.x.fixed,
                                         sz - p.z.fixed);
                    if (d2 * 2 < d1)
                        active = 1;
                }
                if (!active) {
                    Target_0044f2a0* t = owner->list;
                    if (t && !(t->flags & 0x800000)) {
                        count = 2;
                        points[0].x = owner->pos.x.half[1];
                        points[0].y = owner->pos.z.half[1];
                        points[1].x = p.x.half[1];
                        points[1].y = p.z.half[1];
                        active = 1;
                    }
                }
            }
        }
    }
    if (repathClaimTick <= g_game->gameTick - 10)
        repathClaimTick = 0;
    flag_3 = 1;
}

// The out-of-line destructor: it stores its own vtable, unregisters the object, then the empty inline base destructor
// stores 0x4fd428.
// FUNCTION: 0x44f040 ??_GAiSearchGoal@@UAEPAXI@Z
// FUNCTION: 0x44f450
AiSearchGoal::~AiSearchGoal()
{
    g_game->pathfinder->AbortIfGoalMatch(this);
}

// Slot 7: true when there is something to send: flag 3 of +0x64 (the path changed) is
// set, or flag 2 no longer matches bit 2 of the owner's target. Slot 8
// (0x44f4a0) writes the path out and brings both flags up to date.
// FUNCTION: 0x44f480
int AiSearchGoal::HasNetUnitState()
{
    return (flags & 8) || ((owner->target->occupyFlags ^ flags) & 4);
}

// Slot 8, the write counterpart of the reader 0x44f5c0. It sets the stream's next bit when the
// unit's mode asks for it, writes the point count in 2 bits, then up to three
// points of 16 bits each, and finally copies the unit's mode into flag 2 while
// clearing flag 3 (the "changed" flag the readers rely on).
// FUNCTION: 0x44f4a0
void AiSearchGoal::SerializeNetUnitState(BitWriter* stream)
{
    int n;
    if (active) {
        n = count < 3 ? count : 3;
    } else {
        n = 0;
    }
    if (owner->target->occupyFlags & 4) {
        stream->data[stream->bit] |= 1 << stream->index;
    }
    stream->index++;
    if (stream->index == 0x20) {
        stream->index = 0;
        stream->bit++;
        if (stream->bit == stream->capacity) {
            stream->GrowBuffer();
        }
        stream->data[stream->bit] = 0;
    }
    stream->WriteBits(n, 2);
    for (int i = 0; i < n; i++) {
        stream->WriteBits(points[i].x, 0x10);
        stream->WriteBits(points[i].y, 0x10);
    }
    flag_2 = (owner->target->occupyFlags & 4) != 0;
    flag_3 = 0;
}

// The constructor, with the base constructor inlined. Its vtable reference
// makes the compiler emit the scalar deleting destructor here too: the
// destructor is trivial, so the dead store of this class's vtable disappears
// and only the inlined base destructor's store of 0x4fd428 is left.
// FUNCTION: 0x44f570
// FUNCTION: 0x44f590 ??_GPatrolGoal@@UAEPAXI@Z
PatrolGoal::PatrolGoal(Struct_004907e0* p)
    : PathGoal(p)
{
    count = 0;
}

// Slot 5: whether the patrol has at least two waypoints.
// FUNCTION: 0x44f5b0
int PatrolGoal::HasReadyWaypoints()
{
    return count >= 2;
}

// Slot 9: reads the owner's target flag (bit 2 of +0x2e) and up to three waypoints from the stream.
// FUNCTION: 0x44f5c0
void PatrolGoal::DeserializeNetUnitState(BitReader* reader)
{
    ((Owner_0044f5c0*)owner)->target->flag_2 = reader->ReadBit();
    count = reader->ReadBits(2);
    for (int i = 0; i < count; i++) {
        points[i].x = reader->ReadBits(16);
        points[i].y = reader->ReadBits(16);
    }
}

// Slot 3: converts up to `n` short 2D points into 16.16 fixed-point 3D vectors
// (x, 0, y), repeating the last point once the list runs out.
// FUNCTION: 0x44f650
void PatrolGoal::FillWaypointWorldPos(Vec3_004907e0* out, int unused, int n)
{
    for (int i = 0; i < n; i++) {
        int j = i < count ? i : count - 1;
        out[i].x.fixed = points[j].x << 16;
        out[i].y.fixed = 0;
        out[i].z.fixed = points[j].y << 16;
    }
}

// Creates the object at g_game+0x14207 (deleted again by 0x44f6e0).
// FUNCTION: 0x44f6a0
void CreatePathfinder()
{
    g_game->pathfinder = new Pathfinder;
}

// Deletes the object at g_game+0x14207 and clears the pointer.
// FUNCTION: 0x44f6e0
void DestroyPathfinder()
{
    delete g_game->pathfinder;
    g_game->pathfinder = 0;
}
