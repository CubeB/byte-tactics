// Decompiled by Claude Opus 5.5. Names are provisional.
// Per-tick path search scheduler: shares this tick's step budget among the
// active players, then spends it on their path requests in turn, starting a
// new search (0x40e630) for the next unit of the current player or expanding
// the open heap of the running one until it reaches a goal or runs dry.
//
// Matching notes: the expansion loop is `while (1)` with the empty test
// inside, under an `else if (Size() != 0)` whose else is the failure path;
// that keeps the loop tested at the top and the failure block after it.
// Two oddities are kept as the original has them: the `r < 3` case and the
// final `else` both reset the scale to baseScale, and the second
// FUN_0040ef20 path can never run because the pop above already cleared
// topPopped.

struct Point_0040eb70 {
    short x;
    short y;
};

struct Cell_0040eb70 {
    unsigned char flags;               // +0 bit 2 goal
    char dir;                          // +1
    short node;                        // +2
};

struct NodeData_0040eb70 {
    Point_0040eb70 pos;                // +0x0
    int g;                             // +0x4
    int f;                             // +0x8
    short unknown_c;                   // +0xc
    short depth;                       // +0xe
};

struct Node_0040eb70 {
    int index;                         // +0x0 heap slot, or next free node
    NodeData_0040eb70 data;            // +0x4
};

class Class_0040f060 {
public:
    void FUN_0040f060(int i);
};

class Class_0040f1e0 {
public:
    void FUN_0040f1e0(int index);
};

class Class_0040ef20 {
public:
    void FUN_0040ef20(int k);
};

class Class_0040da70 {
public:
    void FUN_0040da70(NodeData_0040eb70* from, Cell_0040eb70* cell, int turn);
};

class Class_0040e050 {
public:
    void FUN_0040e050();
};

class Target_0040eb70;

class Class_0040e630 {
public:
    void FUN_0040e630(Target_0040eb70* t);
};

class Class_0044f010 {
public:
    char unknown_0[4];
    Target_0040eb70* target;           // +0x4

    void FUN_0044f080(Point_0040eb70* points, int count);
};

class Planner_0040eb70 {
public:
    virtual void unused0();
    virtual void unused1();
    virtual void unused2();
    virtual void unused3();
    virtual void unused4();
    virtual void unused5();
    virtual Class_0044f010* GetPath();
};

struct Owner_0040eb70 {
    Planner_0040eb70* planner;         // +0x0
    void* owner;                       // +0x4
};

#pragma pack(push, 1)
struct Unit_0040eb70 {
    Owner_0040eb70* owner;             // +0x0
    char unknown_4[0xa6 - 0x4];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

struct Player_0040eb70 {
    int active;                        // +0x0
    char unknown_4[0x67 - 0x4];
    Unit_0040eb70* unitsBegin;         // +0x67
    Unit_0040eb70* unitsEnd;           // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char type;                // +0x73
    char unknown_74[0x146 - 0x74];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_0040eb70 {
    char unknown_0[0x1b63];
    Player_0040eb70 players[10];       // +0x1b63
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short field_2a3c;         // +0x2a3c
    char unknown_2a3e[0x1434f - 0x2a3e];
    unsigned short field_1434f;        // +0x1434f
};
#pragma pack(pop)

extern Game_0040eb70* g_game;

extern int DAT_00511a38;
extern int DAT_00511a10[10];
extern int DAT_005119e8[10];

class Dummy_00440be0 {
public:
    void FUN_00440be0(Unit_0040eb70* p);
};

#pragma pack(push, 1)
class Class_0040eb70 {
public:
    Node_0040eb70* pool;               // +0x0
    Node_0040eb70** items;             // +0x4
    int freeHead;                      // +0x8
    int used;                          // +0xc
    int capacity;                      // +0x10
    int count;                         // +0x14
    int topPopped;                     // +0x18
    Cell_0040eb70* cells;              // +0x1c
    unsigned int width;                // +0x20
    char unknown_24[0x34 - 0x24];
    Point_0040eb70 found;              // +0x34
    char unknown_38[0x44 - 0x38];
    int range;                         // +0x44
    int stepsPerTick;                  // +0x48
    int steps;                         // +0x4c
    int costScale;                     // +0x50
    int baseScale;                     // +0x54
    Unit_0040eb70* object;             // +0x58
    Class_0044f010* path;              // +0x5c
    Target_0040eb70* target;           // +0x60
    Dummy_00440be0* owner;             // +0x64
    char unknown_68[0x78 - 0x68];
    unsigned char player;              // +0x78
    Unit_0040eb70* cursor[10];         // +0x79
    int budget[10];                    // +0xa1

    int Size()
    {
        return count - topPopped;
    }
    void Release()
    {
        owner->FUN_00440be0(object);
        object = 0;
        owner = 0;
    }

    void FUN_0040eb70();
};
#pragma pack(pop)

static int IsPlaying(unsigned char i)
{
    if (i < 10) {
        Player_0040eb70* p = &g_game->players[i];
        if (p->active != 0 && (p->type == 1 || p->type == 2 || p->type == 3)
            && p->field_146 != 10)
            return 1;
    }
    return 0;
}

// FUNCTION: 0x40eb70
void Class_0040eb70::FUN_0040eb70()
{
    unsigned short players = g_game->field_2a3c;
    if (players == 0)
        return;
    int share = stepsPerTick / players;
    int total = 0;
    if (++DAT_00511a38 >= 150) {
        DAT_00511a38 = 0;
        for (int i = 0; i < 10; i++) {
            int r = DAT_00511a10[i] / g_game->field_1434f;
            if (r < 1)
                DAT_005119e8[i] = baseScale * 6;
            else if (r < 2)
                DAT_005119e8[i] = baseScale * 3;
            else if (r < 3)
                DAT_005119e8[i] = baseScale;
            else
                DAT_005119e8[i] = baseScale;
            DAT_00511a10[i] = 0;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (IsPlaying(i)) {
            budget[i] += share;
            total += budget[i];
        }
    }
    while (total > 0) {
        steps = 0;
        if (object == 0) {
            steps = 1;
            while (budget[player] <= 0) {
                if (++player >= 10)
                    player = 0;
            }
            Player_0040eb70* pl = &g_game->players[player];
            DAT_00511a10[player]++;
            Unit_0040eb70** c = &cursor[player];
            if (*c == pl->unitsEnd)
                *c = pl->unitsBegin;
            else
                (*c)++;
            Unit_0040eb70* u = cursor[player];
            if (u->field_a6 != 0 && u->owner != 0 && u->owner->owner != 0) {
                path = u->owner->planner->GetPath();
                if (path != 0) {
                    object = u;
                    steps += 100;
                    costScale = DAT_005119e8[player];
                    ((Class_0040e630*)this)->FUN_0040e630(path->target);
                }
            }
        } else if (Size() != 0) {
            while (1) {
                if (Size() == 0)
                    break;
                steps++;
                if (topPopped) {
                    topPopped = 0;
                    int k = items[0] - pool;
                    int idx = pool[k].index;
                    ((Class_0040f1e0*)this)->FUN_0040f1e0(k);
                    count--;
                    if (idx < count) {
                        items[idx] = items[count];
                        items[idx]->index = idx;
                        ((Class_0040f060*)this)->FUN_0040f060(idx);
                    }
                }
                NodeData_0040eb70 d = items[0]->data;
                if (!topPopped)
                    topPopped = 1;
                else
                    ((Class_0040ef20*)this)->FUN_0040ef20(items[0] - pool);
                Cell_0040eb70* cell = &cells[width * d.pos.y + d.pos.x];
                if (cell->flags & 4) {
                    found = d.pos;
                    ((Class_0040e050*)this)->FUN_0040e050();
                    Release();
                    break;
                }
                cell->flags = 2;
                for (int k = -range; k <= range; k++)
                    ((Class_0040da70*)this)->FUN_0040da70(&d, cell, k & 7);
                range = 2;
                if (steps >= 100)
                    break;
            }
        } else {
            path->FUN_0044f080(0, 0);
            Release();
        }
        total -= steps;
        budget[player] -= steps;
    }
}
