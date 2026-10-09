// Order: one unit order record (0x56 bytes), a node of the unit's order lists
// at +0x5c and +0x60. The one declaration of the record for the files that call
// its methods or read its fields; order_list.cpp and order_queue_4384a0.cpp,
// which define the methods and own the polymorphic base, keep their own views
// (order_list.cpp needs `kind(k)` as a member initialiser, order_queue_4384a0.cpp
// a second base). The types behind the pointers stay private to their own files.
#ifndef ORDER_H
#define ORDER_H

#include "../util/vec3.h"

struct Unit;
class MissionType;
struct Attached_0043a1f0;
struct File_0043a970;
class HapiBank;
// Unused here: the symbol ids these declarations take keep the allocation of
// the files that share this header (docs/c2-regalloc.md). They are the types
// behind the pointers the order views carry.
struct Point;
struct Link_004895c0;
struct SaveDesc_0043a1f0;
struct Head_0043a1f0;
struct Entry_0043a1f0;
struct Vec3_0043a1f0;

#pragma pack(push, 1)
struct Order {
    char unknown_0[4];             // +0x00, the vpointer the polymorphic views model
    unsigned char kind;            // +0x04, index into the order-type table
    unsigned char state;           // +0x05
    union {
        unsigned int flags;        // +0x06
        struct {
            unsigned int low : 15;
            unsigned int waiting : 1;
            unsigned int high : 16;
        };
    };
    unsigned int wakeFrame;        // +0x0a
    Unit* unit;                    // +0x0e
    char unknown_12[4];
    Unit* target;                  // +0x16, the link's owner
    char unknown_1a[8];
    Vec3 pos;                      // +0x22
    union {
        Point16 start;             // +0x2e
        Point16 field_2e;
    };
    union {
        Point16 field_32;          // +0x32
        Point16 cached;
    };
    union {
        int type;                  // +0x36
        int id;
        int wait;
        int weapon;
        int elapsed;
        int unitType;
        int ticks;
        int mode;
        int done;
        int attempts;
        int time;
        int guardRange;
        int field_36;
        int angle;
        int side;
        int piece;
    };
    union {
        int radius;                // +0x3a
        int step;
        int waitLimit;
        int count;
        int unused;
        int duration;
        int countdown;
        int parity;
        int misses;
    };
    union {
        int progress;              // +0x3e
        int range;
        int retries;
        int capabilities;
    };
    unsigned int flags_42;         // +0x42
    unsigned int created;          // +0x46
    Order* next;                   // +0x4a
    unsigned int field_4e;         // +0x4e
    void* attached;                // +0x52

    Order(unsigned char kind, Unit* unit, Vec3* pos, int a, int b, int c);
    Order(MissionType kind, Unit* unit, void* pos, int a, int b, int c);
    Order(unsigned char kind, int a, int b, int c, int d, int e);
    Order(Unit* unit, HapiBank* file, char* name);
    ~Order();
    void SetDeadlineTicks(int param);
    void ReattachFxToUnit();
    void MergeFlagsFromTable(int k);
    void OrStatusFlags(unsigned int flags);
    void SetAttachedFx(Attached_0043a1f0* obj);
    void AttachApproachRadiusGoal(Vec3* pos, int n);
    void AttachRingApproachGoal(Vec3* pos, int radius1, int radius2);
    void AttachBuildFootprintMarker(Point16 cell, Point16 size);
    void AnnounceStatusIfFlagged(const char* text);
    Unit* Target();
    void Wait();
    Vec3* Position();
    int Advance(int distance);
    int SerializeToSave(Unit* unit, File_0043a970* file, char* name);
};
#pragma pack(pop)

#endif
