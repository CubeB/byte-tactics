// Decompiled by Claude Opus 5.5. Names are provisional.
// Constructor of Class_00408cb0, the owner of the Class_00407350 family (listed
// in 0x407350.cpp): one per player, it creates a timer object for nine of its
// ten slots, each given the player's unit group of the same index.
//
// All family constructors are defined in this file before it, as in the
// original translation unit. /Ob2 inlines the first seven; its inlining budget
// then runs out, so the eighth (Class_00407a90) is inlined without its base
// constructor, and Class_00407d40's constructor is called out of line. Without
// the Class_00407d40 body the budget does not run out and nothing matches.

struct Group_00408cb0 {                // 0x20 bytes
    char unknown_0[0x20];
};

#pragma pack(push, 1)
struct Player_00408cb0 {
    char unknown_0[0x78];
    Group_00408cb0* groups;            // +0x78
    char unknown_7c[0x146 - 0x7c];
    unsigned char index;               // +0x146
};

struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
};
#pragma pack(pop)

extern Game* g_game;

class Class_00407350;

#pragma pack(push, 1)
class Class_00408cb0 {                 // 0x3d bytes
public:
    Player_00408cb0* player;           // +0x0
    unsigned char field_4;             // +0x4
    int countdown;                     // +0x5
    int field_9;                       // +0x9
    int field_d;                       // +0xd
    Class_00407350* timers[10];        // +0x11
    void* cursor;                      // +0x39

    Class_00408cb0(Player_00408cb0* p);
};
#pragma pack(pop)

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class Class_00407350 {
public:
    Class_00408cb0* owner;             // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    Class_00407350(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0
    virtual ~Class_00407350() {}                    // slot 1
};

// Vtable 0x4fc988, constructor 0x407930, ??_G 0x407980.
class Class_00407930 : public Class_00407350 {
public:
    int field_14;                      // +0x14
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24

    Class_00407930(Class_00408cb0* p, void* q, int a, int b);
    virtual void FUN_00407380();                    // slot 0, 0x4077e0
};

// Vtable 0x4fc990, constructor 0x4079a0, ??_G 0x4079d0.
class Class_004079d0 : public Class_00407350 {
public:
    int field_14;                      // +0x14

    Class_004079d0(Class_00408cb0* p, void* q, int a);
    virtual void FUN_00407380();                    // slot 0, 0x4079f0
};

// Vtable 0x4fc998, constructor 0x407a90, ??_G 0x407ac0.
class Class_00407a90 : public Class_00407350 {
public:
    Class_00407a90(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x407ae0
};

struct Vec3_00407d40 {
    int x, y, z;

    Vec3_00407d40() {}
    Vec3_00407d40(Game* game) {
        int ax = (int)(game->baseX / 2 * 65536.0);
        *this = Vec3_00407d40(ax, 0, (int)(game->baseY / 2 * 65536.0));
    }
    Vec3_00407d40(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

// Vtable 0x4fc9a0, constructor 0x407d40, ??_G 0x407e70.
class Class_00407d40 : public Class_00407350 {
public:
    Vec3_00407d40 a;                   // +0x14
    Vec3_00407d40 b;                   // +0x20
    Vec3_00407d40 c;                   // +0x2c
    int field_38;                      // +0x38

    Class_00407d40(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x407e90
};

// Vtable 0x4fc9a8, constructor 0x4085d0, ??_G 0x408600.
class Class_004085d0 : public Class_00407350 {
public:
    Class_004085d0(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x408100
};

// Vtable 0x4fc9b0, constructor 0x4087e0, ??_G 0x408810.
class Class_00408810 : public Class_00407350 {
public:
    Class_00408810(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x4086d0
};

// The family constructors (matched in their own files) were defined in the
// same file, before this one.
Class_00407350::Class_00407350(Class_00408cb0* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4)
{
}

Class_00407930::Class_00407930(Class_00408cb0* p, void* q, int a, int b)
    : Class_00407350(p, q), field_1c(b), field_20(a)
{
    field_24 = 0;
    field_18 = 6;
    field_14 = 3;
}

Class_004079d0::Class_004079d0(Class_00408cb0* p, void* q, int a)
    : Class_00407350(p, q), field_14(a)
{
}

Class_00407a90::Class_00407a90(Class_00408cb0* p, void* q)
    : Class_00407350(p, q)
{
}

Class_00407d40::Class_00407d40(Class_00408cb0* p, void* q)
    : Class_00407350(p, q), a(g_game), b(g_game), c(g_game), field_38(0)
{
}

Class_004085d0::Class_004085d0(Class_00408cb0* p, void* q)
    : Class_00407350(p, q)
{
}

Class_00408810::Class_00408810(Class_00408cb0* p, void* q)
    : Class_00407350(p, q)
{
}

// FUNCTION: 0x408cb0
Class_00408cb0::Class_00408cb0(Player_00408cb0* p)
{
    player = p;
    field_4 = p->index;
    field_9 = 0;
    countdown = 30;
    cursor = 0;
    field_d = 0;
    for (int i = 0; i < 10; i++)
        timers[i] = 0;
    timers[1] = new Class_00408810(this, &player->groups[1]);
    timers[4] = new Class_004085d0(this, &player->groups[4]);
    timers[5] = new Class_00407350(this, &player->groups[5]);
    timers[2] = new Class_00407930(this, &player->groups[2], 3, 20000);
    timers[3] = new Class_004079d0(this, &player->groups[3], 2);
    timers[6] = new Class_00407930(this, &player->groups[6], 7, 50000);
    timers[7] = new Class_004079d0(this, &player->groups[7], 6);
    timers[8] = new Class_00407a90(this, &player->groups[8]);
    timers[9] = new Class_00407d40(this, &player->groups[9]);
}
