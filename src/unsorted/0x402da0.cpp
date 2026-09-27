// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: scratch-register choices differ in order calls and flag copying;
// the state-2 flag test uses test ax/jge instead of test ah/je.
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
        if ((short)flags < 0) {
            ((Class_00439e80*)order)->FUN_00439e80(30);
            order->Wait();
            return 2;
        }
        if (flags & 1) {
            ((Class_00439e80*)order)->FUN_00439e80(11);
            FUN_0041bcd0(unit, 11);
        }
        order->Wait();
        return 2;
    default:
        return 7;
    }
}
