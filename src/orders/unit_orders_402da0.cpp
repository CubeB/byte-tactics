// Decompiled by Claude Opus 5.5, verified by GPT-6.1-sol, retried by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
#include <string.h>
//
// MATCH. Two levers, both found with tools/permute.py started from the
// `node->kind == move` form (87.9%, which has the original's callee-saved pair
// and loop layout but a scratch-register rotation one step late):
// * Class_00438760 has an operator== comparing the index bytes, so the loop
//   head is `cmp cl, dl` and `order` lands in ebp, `queued` in ebx. The older
//   `(a ^ b) == 0` spelling gave the wrong pair (88.9%).
// * Order::Target() is a trivial inline accessor and every `order->target->X`
//   in the flag-merge tail goes through it. The accessor adds a temporary that
//   shifts the rotation of scratch registers back into phase and also fixes
//   the two flag-merge registers. Using it on just one of the merges (fire or
//   move) is already enough to match; all four uses are written the same way.
class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() : index(0) {}
    int operator==(Class_00438760 o) const { return index == o.index; }
};
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004898b0 { public: void ClaimWeapons(int); };
#pragma pack(push, 1)
struct Unit;
struct Order {
    char pad0[4];
    Class_00438760 kind;
    unsigned char state;
    unsigned int low:15;
    unsigned int waiting:1;
    unsigned int high:16;
    char padA[0x16-0xa];
    Unit* target;
    char pad1A[0x22-0x1a];
    int pos[3];
    char pad2E[0x4a-0x2e];
    Order* next;
    Unit* Target() { return target; }
    void Wait() { waiting = 1; }
    int* Position() { return pos; }
};
struct Player {
    int active;
    char pad4[0x73-4];
    unsigned char type;
};
struct Unit {
    int active;
    char pad4[0x5c-4];
    Order* orders;
    char pad60[0x96-0x60];
    Player* owner;
    char pad9A[0xac-0x9a];
    int value;
    char padB0[0x104-0xb0];
    float progress;
    char pad108[8];
    union {
        unsigned int flags;
        struct { unsigned int low:18; unsigned int fire:2; unsigned int move:2; unsigned int high:10; };
    };
    int Ready() { return (flags & 0x10000000) && !(flags & 0x4000); }
};
#pragma pack(pop)
void __stdcall FUN_0041c110(Unit*);
Class_00438760 __stdcall FUN_0043f0e0(unsigned char, Unit*, Unit*, void*);
void __stdcall AddOrder(Class_00438760, int, Unit*, Unit*, void*, int, int);
void __stdcall FUN_0041bcd0(Unit*, int);
// FUNCTION: 0x402da0
int __stdcall GetBuiltOrder(Unit* unit, Order* order, unsigned int flags)
{
    if (unit->progress == 0.0f) {
        FUN_0041c110(unit);
        if (unit->active) {
            int queued = 0;
            if (order->target) {
                Class_00438760 move("QMove");
                Class_00438760 patrol("QPatrol");
                for (Order* node = order->target->orders; node; node = node->next) {
                    Class_00438760 kind;
                    if (node->kind == move)
                        kind = FUN_0043f0e0(2, unit, 0, node->Position());
                    else if (node->kind.index == patrol.index)
                        kind = FUN_0043f0e0(9, unit, 0, node->Position());
                    if (kind.index) {
                        AddOrder(kind, 1, unit, 0, node->Position(), 0, 0);
                        queued = 1;
                    }
                }
                if (unit->Ready() && order->Target()->Ready()) {
                    unit->fire = order->Target()->fire;
                    unit->move = order->Target()->move;
                    if (unit->owner->active && unit->owner->type == 1)
                        unit->value = order->Target()->value;
                }
            }
            if (!queued)
                AddOrder("PARK", 1, unit, 0, 0, 0, 0);
        }
        return 5;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        ((Class_004898b0*)unit)->ClaimWeapons(3);
        ((Class_00439e80*)order)->FUN_00439e80(300);
        order->Wait();
        return 1;
    case 1:
        ((Class_00439e80*)order)->FUN_00439e80(30);
        order->Wait();
        return 1;
    case 2:
        if (flags & 0x8000) {
            ((Class_00439e80*)order)->FUN_00439e80(30);
        } else if (flags & 1) {
            ((Class_00439e80*)order)->FUN_00439e80(11);
            FUN_0041bcd0(unit, 11);
        }
        order->Wait();
        return 2;
    default:
        return 7;
    }
}