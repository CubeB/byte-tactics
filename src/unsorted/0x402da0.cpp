// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol. Names are provisional.
// Order handler: when the unit has just been built (progress 0), copies the
// QMove/QPatrol orders queued on the factory (order->target) to the new unit
// and its fire/move states, else parks it; otherwise a small wait state machine.
//
// Retry by GPT-6.1-sol: unchanged best is 87.9%; reversing fire/move stores scored 87.4%, caching target scored 73.9%. Neither matches.
// Partial (87.9%): only register choices differ. Every hunk is one rotation of
// the same scratch-register sequence (ours ecx/edx, eax/ecx; original eax/ecx,
// edx/eax): the first FUN_0043f0e0 argument block, the FUN_0043adc0 call, and
// the second flag copy (ours mov edx,[ecx+0x110], original reuses ecx; ours
// mov ecx,[eax+0xac] for the value copy, original edx). Retried with no change:
// kind declared before the loop, do/while, while(1)+break at top, node outside
// the init clause, node->pos / &node->pos[0], braces around the branches,
// `node != 0`, `kind.index != 0`, swapped operands, queued |= 1 / queued++,
// two throwaway pos loads at loop head (dead-code eliminated; the guide's
// throwaway-load probe does not shift this rotation). The case-2 tail is fixed:
// one Wait() after an if/else-if lets MSVC duplicate the tail itself and keeps
// 0x8000 out of a register (two Wait() calls plus the test hoist it into ebx).
// No-change rewrites: headers.py, preceding 0x402d10 in the file, while loop,
// operand swaps, operator==, operator=, local pos/node/k copies, ternaries,
// bitfield Ready(), IsHuman helper, nested ifs.
// Also no effect (deepseek-v4.1-flash): a 101-point sweep of unused
// `extern int dummyN;` (N = 0..400 step 4) left every point at 87.9%, so this
// rotation is not reachable through the compiler-state probe; manual
// `(flags & ~mask) | (target->flags & mask)` masks and an explicit
// `unsigned char k = node->kind.index` are also 87.9%; a shared
// `unsigned int f` accumulator for the two bitfield merges drops to 84.7%.
class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() : index(0) {}
};
class Class_00439e80 { public: void FUN_00439e80(int); };
class Class_004898b0 { public: void FUN_004898b0(int); };
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
void __stdcall FUN_0043adc0(Class_00438760, int, Unit*, Unit*, void*, int, int);
void __stdcall FUN_0041bcd0(Unit*, int);
// FUNCTION: 0x402da0
int __stdcall FUN_00402da0(Unit* unit, Order* order, unsigned int flags)
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
                    if (node->kind.index == move.index)
                        kind = FUN_0043f0e0(2, unit, 0, node->Position());
                    else if (node->kind.index == patrol.index)
                        kind = FUN_0043f0e0(9, unit, 0, node->Position());
                    if (kind.index) {
                        FUN_0043adc0(kind, 1, unit, 0, node->Position(), 0, 0);
                        queued = 1;
                    }
                }
                if (unit->Ready() && order->target->Ready()) {
                    unit->fire = order->target->fire;
                    unit->move = order->target->move;
                    if (unit->owner->active && unit->owner->type == 1)
                        unit->value = order->target->value;
                }
            }
            if (!queued)
                FUN_0043adc0("PARK", 1, unit, 0, 0, 0, 0);
        }
        return 5;
    }
    unsigned int state = 0;
    state = order->state;
    switch (state) {
    case 0:
        ((Class_004898b0*)unit)->FUN_004898b0(3);
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
