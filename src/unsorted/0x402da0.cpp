// Decompiled by Claude Opus 5.5, verified by GPT-6.1-sol, retried by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
#include <string.h>
//
// deepseek-v4.1-flash (2026-10-01, best 88.9%, 537 bytes): two independent
// levers found this pass, both needed.
//
// 1. The loop-head comparison written as `(node->kind.index ^ move.index) == 0`
//    instead of `==` shifts MSVC's rotating scratch allocator one step
//    earlier, so the two FUN_0043f0e0 argument blocks, the kind reload and the
//    FUN_0043adc0 pos lea (sites A B C D F) now match the original. Any form
//    that turns the byte comparison into a value first does it: the xor, and
//    `(a & 0xff) == (b & 0xff)`, both measured with
//    build/scratch/0x402da0/phase.py (compiles and disassembles a variant
//    without spending a check.py run; base phase = A=ecx B=edx C=eax D=eax
//    E=edx F=edx G=ecx, original = A=eax B=ecx C=edx D=edx E=ecx F=ecx G=edx).
//    Adding one live temp anywhere before A shifts the whole rotation one step
//    later, mod 3 (verified with prologue stores: 1 store +1, 3 stores +0).
//    Every code-identical spelling tried (casts, aliases, references,
//    accessors, const locals, negation forms, dead stores, unused functions,
//    dummy declarations, 128 header sets) leaves the phase unchanged; only the
//    value-first comparison forms move it.
// 2. `#include <string.h>` (any of string/windows/stdio/stdlib/math works)
//    changes MSVC's instruction selection for that xor: without it the loop
//    head is `mov dl,cl; xor dl,[esp+0x10]`, with it `xor dl,cl` (same size,
//    one instruction less), which lifts 88.4 to 88.9. headers.py reports 88.9
//    for all five single includes.
//
// Still differs: the callee-saved pair is swapped (ours order -> ebx, queued ->
// ebp; original order -> ebp, queued -> ebx: 5 instructions in the prologue and
// the `mov ebp,1`), the loop head's `xor dl,cl` where the original has
// `cmp cl,dl`, and the two flag-merge registers E and G (ours `mov edx,
// [ecx+0x110]` / `mov ecx,[eax+0xac]`, original `mov ecx,[ecx+0x110]` /
// `mov edx,[eax+0xac]`). E and G do not move with the loop's phase: they stayed
// put when the phase was fixed, and every merge spelling tried (references,
// deref, [0], pointer locals, comma, nested if, masks, extra temps around
// them) leaves them alone, so they look like a separate allocator/peephole
// decision. The natural next lead is the callee-saved ranking (see
// 0x419be0.cpp: MSVC 5 ranks live-across-call values by use count), which is
// what put order in ebx here; a spelling that puts order back in ebp without
// losing the phase would be the win.
//
// This pass also tried and measured (all with the phase oracle, scores are
// check.py --sym): ~60 loop-head spellings (casts, parenthesisation, negation,
// double negation, comma, ternary, duplicated comparisons, operator==,
// reference/pointer/accessor forms for node->kind, move and patrol, value-first
// forms on the second comparison and on the kind/active/queued/progress tests);
// ~40 prologue spellings (order/queued/move/patrol declaration order, alias and
// dead-store forms, constant locals, identity arithmetic on active/target/
// progress, float locals and references); ~30 merge-region spellings
// (references, deref, [0], pointer locals, comma, nested if, member helpers,
// masks, extra temps around the block). None moved E or G, and only the
// value-first comparison forms moved the loop phase. Also flat: an 0..40 sweep
// of dummy functions defined above the target (unlike 0x406300, this function's
// allocator state is not reachable that way).
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
                    if ((node->kind.index ^ move.index) == 0)
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
