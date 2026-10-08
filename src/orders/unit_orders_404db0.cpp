// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>

struct Vec3 {
    int x, y, z;
};

struct Box {
    Vec3 lo;
    Vec3 hi;
};

struct Point16 {
    short x;
    short z;
};

struct Rot16 {
    short x, y, z;
};

struct Unit;

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

#pragma pack(push, 2)
class Class_0043a1f0 {
public:
    char unknown_0[0x56];
    Class_0043a1f0(Class_00438760 type, Unit* target, void* pos, int c, int d, int e);
};
#pragma pack(pop)

class Class_004895c0 {
public:
    Unit* owner;                       // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    virtual ~Class_004895c0();
    void SetUnit(Unit* o);
};

class Class_00438ad0 {
public:
    void AttachBuildFootprintMarker(Point16 cell, Point16 size);
};

class Class_00439e80 {
public:
    void SetDeadlineTicks(int ticks);
};

#include "../map/mission.h"

#pragma pack(push, 1)
struct Feature {
    char name[0x94];                   // +0x0
    Point16 footprint;                 // +0x94
    char unknown_98[0xfa - 0x98];
    unsigned char height;              // +0xfa
    char unknown_fb[0xfe - 0xfb];
    unsigned char flags;               // +0xfe
    char unknown_ff[0x100 - 0xff];
};

struct FeatureSpot {
    char unknown_0[0x20];
    Rot16 rot;                         // +0x20
    char unknown_26[0x30 - 0x26];
};

struct Cell {
    char unknown_0[8];
    unsigned short feature;            // +0x8
    unsigned short spot;               // +0xa
    char unknown_c;
};

struct UnitDef {
    char unknown_0[0x1ea];
    int buildTime;                     // +0x1ea
    char unknown_1ee[0x1fe - 0x1ee];
    unsigned short workerTime;         // +0x1fe
    char unknown_200[0x245 - 0x200];
    unsigned int flags;                // +0x245
};

struct PlayerInfo {
    char unknown_0[4];
    int id;                            // +0x4
};

struct Unit {
    int active;                        // +0x0
    char unknown_4[0x64 - 0x4];
    Rot16 rot;                         // +0x64
    Vec3 pos;                          // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef* type;                     // +0x92
    PlayerInfo* player;                // +0x96
    char unknown_9a[0xb0 - 0x9a];
    int workTime;                      // +0xb0
    char unknown_b4[0xff - 0xb4];
    unsigned char playerIndex;         // +0xff
    char unknown_100[0x104 - 0x100];
    float buildLeft;                   // +0x104
    short health;                      // +0x108
};

struct Order {
    char unknown_0[5];
    unsigned char state;               // +0x5
    unsigned int flags;                // +0x6
    char unknown_a[0x12 - 0xa];
    Class_004895c0 target;             // +0x12
    Vec3 pos;                          // +0x22
    char unknown_2e[0x36 - 0x2e];
    int unitType;                      // +0x36
    int time;                          // +0x3a
};

struct Game {
    char unknown_0[0x1420b];
    FeatureSpot* spots;                // +0x1420b
    char unknown_1420f[0x14233 - 0x1420f];
    int width;                         // +0x14233
    char unknown_14237[0x1426f - 0x14237];
    Feature* features;                 // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell* cells;                       // +0x14287
    char unknown_1428b[0x1439b - 0x1428b];
    UnitDef* unitTypes;                // +0x1439b
    char unknown_1439f[0x38a47 - 0x1439f];
    int ticks;                         // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Mission* net;                      // +0x391e9
};

struct Packet {
    unsigned char type;
    unsigned char sub;
    short x;
    short z;
};
#pragma pack(pop)

extern Game* g_game;

unsigned short __stdcall FindFeatureAtPos(Vec3* pos, Point16* cell, Point16* size);
void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
int __stdcall RandomInt(int range);
int __stdcall GetGroundHeight(Vec3* pos);
unsigned short __stdcall GetHeadingBetween(Vec3* from, Vec3* to);
void __stdcall StartBuildingScript(Unit* unit, Order* order, short turn);
int __stdcall WaitIfNotInBuildStance(Unit* unit, Order* order, int flags);
unsigned short __stdcall FindUnitTypeId(char* name);
void __stdcall GetNanoPiecePosition(Unit* unit, Vec3* out);
void __stdcall EmitNanoParticles(Vec3* from, Box* to, int count);
Unit* __stdcall CreateUnit(unsigned char player, unsigned short type, Vec3 pos, int a, int b, int c);
Cell* __stdcall GetOriginCellAtPosition(Vec3* pos);
unsigned short __stdcall GetCellFeature(Cell* cell);
Cell* __stdcall GetMapCellAtPosition(Vec3* pos);
void __stdcall RemoveFeature(void* target, int flag);
int __stdcall BroadcastPacket(int player, void* data, int size);
void __stdcall MarkSelectionOrdersDirty(Unit* unit);
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit* unit, Unit* target, int flags);
void __stdcall AppendOrder(Unit* owner, Class_0043a1f0* node);

// Order handler "Resurrecting": raises the unit a wreck (feature) came from.
// FUNCTION: 0x404db0
int __stdcall ResurrectOrder(Unit* unit, Order* order, int flags)
{
    // Point16, not a short or int: a plain local changes how x is moved.
    Point16 cell;
    Point16 size;
    // Set before the state test although no path reads it: the original
    // computes it there.
    Feature* f = &g_game->features[0xffff];
    if (order->state <= 5) {
        unsigned short index = FindFeatureAtPos(&order->pos, &cell, &size);
        if (index == 0xffff) {
            QueueUnitSpeech(unit, 7, "Resurrection failed");
            return 8;
        }
        f = &g_game->features[index];
        if (!(f->flags & 0x80))
            return 8;
    }
    switch (order->state) {
    case 0:
        if (unit->active && (unit->type->flags & 0x800)) {
            ((Class_00438ad0*)order)->AttachBuildFootprintMarker(cell, size);
            order->flags = 0xe0;
            return 1;
        }
        return 7;
    case 1: {
        if (flags & 0x40)
            return 8;
        Vec3 pos;
        pos.x = (size.x + cell.x * 2) << 19;
        pos.z = (size.z + cell.z * 2) << 19;
        pos.y = (RandomInt(f->height) + GetGroundHeight(&pos)) << 16;
        StartBuildingScript(unit, order, GetHeadingBetween(&unit->pos, &pos) - unit->rot.y);
        return 1;
    }
    case 2:
        return WaitIfNotInBuildStance(unit, order, 0);
    case 3: {
        char name[0x40];
        lstrcpynA(name, f->name, 0x40);
        for (int i = 0; i < 0x40; i++) {
            if (name[i] == '_') {
                name[i] = 0;
                break;
            }
        }
        order->unitType = FindUnitTypeId(name);
        if (!order->unitType) {
            QueueUnitSpeech(unit, 7, "Ressurection failed");
            return 8;
        }
        order->time = (int)(g_game->unitTypes[order->unitType].buildTime * 0.3 / (unit->type->workerTime / 30));
        QueueUnitSpeech(unit, 11, 0);
        return 1;
    }
    case 4:
        if (order->time--) {
            Vec3 nano;
            GetNanoPiecePosition(unit, &nano);
            Box box;
            box.lo.x = cell.x << 20;
            box.lo.z = cell.z << 20;
            box.lo.y = GetGroundHeight(&box.lo) << 16;
            box.hi = box.lo;
            box.hi.x += f->footprint.x << 20;
            box.hi.z += f->footprint.z << 20;
            box.hi.y += f->height << 16;
            EmitNanoParticles(&nano, &box, 6);
            unit->workTime = g_game->ticks + 300;
            ((Class_00439e80*)order)->SetDeadlineTicks(1);
            return 2;
        }
        break;
    case 5: {
        order->target.SetUnit(CreateUnit(unit->playerIndex, order->unitType, order->pos, 0, 1, 0));
        if (!order->target.owner) {
            QueueUnitSpeech(unit, 7, "Unable to create any more units");
            ((Class_00439e80*)order)->SetDeadlineTicks(300);
            return 2;
        }
        Cell* c = GetOriginCellAtPosition(&order->pos);
        if (GetCellFeature(c) >= 0xfffb)
            return 8;
        FeatureSpot* spot = &g_game->spots[c->spot];
        if (spot)
            order->target.owner->rot = spot->rot;
        RemoveFeature(GetMapCellAtPosition(&order->pos), 0);
        if (g_game->net->GetGameType() == 3) {
            Packet packet;
            int n = c - g_game->cells;
            int w = g_game->width;
            packet.type = 0x0f;
            packet.sub = 0xff;
            Point16 xz;
            xz.x = n % w;
            xz.z = n / w;
            packet.x = xz.x;
            packet.z = xz.z;
            BroadcastPacket(unit->player->id, &packet, 6);
        }
        order->target.owner->buildLeft = 0;
        order->target.owner->health = 1;
        MarkSelectionOrdersDirty(unit);
        break;
    }
    case 6: {
        QueueUnitSpeech(unit, 8, "Resurrection complete");
        Class_00438760 kind = GetOrderType(8, unit, order->target.owner, 0);
        if (kind.index)
            AppendOrder(unit, new Class_0043a1f0(kind, order->target.owner, 0, 0, 0, 0));
        return 5;
    }
    default:
        return 7;
    }
    return 1;
}
