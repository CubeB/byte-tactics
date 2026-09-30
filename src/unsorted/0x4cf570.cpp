// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry in #1928: 6 direct checks kept 80.2%. Delayed
// initialization, explicit self guard, and reusing bestidx for the zero guard
// did not improve the allocator rotation described below.
// Refinement in issue-1928-r1: nine more direct checks and the combined
// checker run kept 80.2%. Reordering variable declarations scored 77.2%; the
// best source was restored. No MATCH was reached.
// Picks a free sound channel from a four entry set of sound objects: a valid
// one in the set is taken as is, otherwise the one with the highest priority is
// recycled, or set[0] is cloned when the set has a free slot. The chosen object
// is then set up and filed in the channel table at +0x38.
// Layout: argument 1 is the four entry set, argument 2 is passed to the +0x3c
// method, argument 3 is a position (int x/y/z). Table fields: count +0x30,
// counter +0x34, buffers +0x38, priorities +0xb8, flags +0x138, factory +0x24.
//
// Not MATCH (80.2): the code is byte-identical to the original except for one
// allocator state. The original keeps this in esi and the shared zero constant
// in ebp, spilling this to [esp+0x18], so ebp later becomes bestidx and esi is
// reused as the loop index. This version keeps this in ebp and the zero in esi,
// so bestidx gets a stack slot at [esp+0x1c] and slot moves to [esp+0x18]. Every
// other difference (the [esp+0x18] this reload, mov edi/esi choices, the
// set[bestidx] index) follows from that one rotation.
// GPT-6.1-sol verification: direct check.py scored 80.2% before and after the
// 128 common-header sweep; no header set matched. The optional C++ header sweep
// was stopped after 271 of 768 combinations to stay within the worker timebox.
// Tried and did NOT change the rotation: the interface calls as virtual
// __stdcall methods (that fixed 24 points on its own; function-pointer members
// were wrong) versus data members; an explicit self pointer or reference alias
// for this; an inline helper for the DAT scan taking this as an argument; the
// declarations of unit/slot/best/bestidx in every order and at every scope;
// bestidx declared after the null check; an early-continue loop form; swapping
// the first loop's condition order; set aliased to a local; and Windows/stdio/
// stdlib/string/math/dsound headers.
extern int DAT_0051ff48;

struct Info_004fcf68 {
    int dummy;
};
extern const Info_004fcf68 DAT_004fcf68;

struct Pos_004cf570 {
    int x, y, z;
};

// Sound object interface. The original calls it through its vtable as a
// __stdcall method with the object as the first stack argument (COM style).
class Unit_004cf570 {
public:
    virtual int __stdcall FUN_004cf5e0(const Info_004fcf68* info, Unit_004cf570** out);
    virtual int __stdcall FUN_004cf5e4();
    virtual int __stdcall FUN_004cf5e8();
    virtual int __stdcall FUN_004cf5ec();
    virtual int __stdcall FUN_004cf5f0(unsigned int* a, unsigned int* b);
    virtual int __stdcall FUN_004cf5f4();
    virtual int __stdcall FUN_004cf5f8();
    virtual int __stdcall FUN_004cf5fc();
    virtual int __stdcall FUN_004cf600();
    virtual int __stdcall FUN_004cf604(Unit_004cf570** out);
    virtual int __stdcall FUN_004cf608();
    virtual int __stdcall FUN_004cf60c();
    virtual int __stdcall FUN_004cf610(int a, int b, int c);
    virtual int __stdcall FUN_004cf614(int a);
    virtual int __stdcall FUN_004cf618();
    virtual int __stdcall FUN_004cf61c(int a);
    virtual int __stdcall FUN_004cf620(int a, int b);
    virtual int __stdcall FUN_004cf624(int a, int b);
    virtual int __stdcall FUN_004cf628(int a, int b);
    virtual int __stdcall FUN_004cf62c(float x, float y, float z, float w);
};

// Factory interface, used to clone a set entry (method at vtable +0x14).
class Factory_004cf570 {
public:
    virtual int __stdcall FUN_004cf630();
    virtual int __stdcall FUN_004cf634();
    virtual int __stdcall FUN_004cf638();
    virtual int __stdcall FUN_004cf63c();
    virtual int __stdcall FUN_004cf640();
    virtual int __stdcall FUN_004cf644(Unit_004cf570* src, Unit_004cf570** out);
};

class Class_004cf180 {
public:
    void FUN_004cf180();
};

class Class_004cf570 {
public:
    int field_0;
    int field_4;
    int field_8;
    int field_c;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int field_20;
    Factory_004cf570* field_24;
    int field_28;
    int field_2c;
    int count;                              // +0x30
    int field_34;
    Unit_004cf570* buffers[0x20];           // +0x38
    int priority[0x20];                     // +0xb8
    int flags[0x20];                        // +0x138

    int FUN_004cf570(Unit_004cf570** set, int arg2, Pos_004cf570* pos);
};

// FUNCTION: 0x4cf570
int Class_004cf570::FUN_004cf570(Unit_004cf570** set, int arg2, Pos_004cf570* pos)
{
    Unit_004cf570* unit = 0;
    int slot = 0;
    int bestidx = 0;
    if (DAT_0051ff48 != 0) {
        for (int i = 0; i < 0x20; i++) {
            if (buffers[i] != 0 && flags[i] == 1)
                return 0;
        }
    }
    while (count >= field_2c)
        ((Class_004cf180*)this)->FUN_004cf180();
    unsigned int best = 0;
    if (set == 0)
        return 0;
    for (int i = 0; i < 4; i++) {
        if (set[i] != 0) {
            Unit_004cf570* c;
            if (set[i]->FUN_004cf604(&c) != 0)
                return 0;
            if (c == 0) {
                unit = set[i];
                break;
            }
            unsigned int a, b;
            set[i]->FUN_004cf5f0(&a, &b);
            if (a > best) {
                bestidx = i;
                best = a;
            }
        } else {
            slot = i;
        }
    }
    if (unit == 0) {
        if (slot > 0) {
            if (field_24->FUN_004cf644(set[0], &unit) != 0)
                return 0;
            set[slot] = unit;
        } else {
            unit = set[bestidx];
            unit->FUN_004cf614(0);
        }
    }
    Unit_004cf570* chan;
    if (unit->FUN_004cf5e0(&DAT_004fcf68, &chan) == 0) {
        if (field_4 == 0 || pos == 0) {
            chan->FUN_004cf628(2, 0);
        } else {
            chan->FUN_004cf62c((float)pos->x, (float)pos->y, (float)pos->z, 0.0f);
            chan->FUN_004cf624(field_8, 0);
            chan->FUN_004cf620(field_c, 0);
            chan->FUN_004cf628(0, 0);
        }
        chan->FUN_004cf5e8();
    }
    if (unit->FUN_004cf614(0) != 0)
        return 0;
    if (unit->FUN_004cf61c(arg2) != 0)
        return 0;
    if (unit->FUN_004cf610(0, 0, DAT_0051ff48 != 0) != 0)
        return 0;
    for (int j = 0; j < 0x20; j++) {
        if (buffers[j] == 0) {
            buffers[j] = unit;
            priority[j] = ++field_34;
            flags[j] = DAT_0051ff48 != 0;
            count++;
            return 1;
        }
    }
    return 1;
}
