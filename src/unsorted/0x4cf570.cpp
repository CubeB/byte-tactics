// Decompiled by space-bunny-free. Names are provisional.
// Still short of MATCH (about 60 percent of instructions match). What differs:
// the callee saved register ROTATION, which is one allocator state and not a
// per block problem. The original holds the zero constant in ebp (which then
// becomes bestidx) and keeps this in esi, spilling this to [esp+0x18] and
// reusing esi for the list index. This version puts the zero constant in esi
// (which becomes the index) and keeps this in ebp, so bestidx lands in a
// stack slot and the frame is 8 dwords instead of 7. Everything downstream
// (the [esp+0x18] this reloads, the mov edi/edx/ecx choices, setne dl versus
// cl) follows from that one difference. Tried and did NOT change the
// rotation: declaring best/bestidx at the top of the function, declaring
// unit/slot after the while loop, swapping their declaration order, a
// function level loop index, and an extra reference to this.
// Layout facts used here: the four entry list is argument 2, the position is
// argument 3, argument 1 is never read, the table fields are count +0x30,
// counters +0x34, buffers +0x38, priorities +0xb8, flags +0x138, and the
// caller's f00 takes the constant at 0x4fcf68 plus an out parameter.
// Picks a free sound channel from a four entry list of sound objects: a valid
// one in the list is taken as is, otherwise the object with the highest
// priority is recycled, or list[0] is cloned when the list has a free slot.
// The chosen object is then set up and filed in the channel table at +0x38.
extern int DAT_0051ff48;
struct Chan_004cf570;
struct Unit_004cf570;
struct Info_004fcf68 {
    int dummy;
};
extern const Info_004fcf68 DAT_004fcf68;
struct Pos_004cf570 {
    int x, y, z;
};

struct Unit_004cf570 {
    int (__stdcall *f00)(Unit_004cf570*, const Info_004fcf68*, Chan_004cf570**);
    int (__stdcall *f10)(Unit_004cf570*, unsigned int*, unsigned int*);
    int (__stdcall *f24)(Unit_004cf570*, Unit_004cf570**);
    int (__stdcall *f30)(Unit_004cf570*, int, int, int);
    int (__stdcall *f34)(Unit_004cf570*, int);
    int (__stdcall *f3c)(Unit_004cf570*, Pos_004cf570*);
};

struct Chan_004cf570 {
    int (__stdcall *f08)(Chan_004cf570*);
    int (__stdcall *f40)(Chan_004cf570*, int, int);
    int (__stdcall *f44)(Chan_004cf570*, int, int);
    int (__stdcall *f48)(Chan_004cf570*, int, int);
    int (__stdcall *f4c)(Chan_004cf570*, float, float, float, float);
};

struct Factory_004cf570 {
    int pad[5];
    int (__stdcall *f14)(Factory_004cf570*, Unit_004cf570*, Unit_004cf570**);
};

class Class_004cf180 {
public:
    char unknown_0[0x30];
    int count;
    char unknown_34[4];
    void* buffers[0x20];
    int priority[0x20];
    int flags[0x20];

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
    int count;
    int field_34;
    Unit_004cf570* buffers[0x20];
    int priority[0x20];
    int flags[0x20];

    int FUN_004cf570(int arg1, Unit_004cf570** list, Pos_004cf570* pos);
};

// FUNCTION: 0x4cf570
int Class_004cf570::FUN_004cf570(int arg1, Unit_004cf570** list, Pos_004cf570* pos)
{
    Unit_004cf570* unit = 0;
    int slot = 0;
    if (DAT_0051ff48 != 0) {
        for (int i = 0; i < 0x20; i++) {
            if (buffers[i] != 0 && flags[i] == 1)
                return 0;
        }
    }
    while (count >= field_2c)
        ((Class_004cf180*)this)->FUN_004cf180();
    unsigned int best = 0;
    int bestidx = 0;
    if (list == 0)
        return 0;
    for (int i = 0; i < 4; i++) {
        if (list[i] != 0) {
            Unit_004cf570* c;
            if (list[i]->f24(list[i], &c) != 0)
                return 0;
            if (c == 0) {
                unit = list[i];
                break;
            }
            unsigned int a, b;
            list[i]->f10(list[i], &a, &b);
            if (a > best) {
                best = a;
                bestidx = i;
            }
        } else {
            slot = i;
        }
    }
    if (unit == 0) {
        if (slot > 0) {
            if (field_24->f14(field_24, list[0], &unit) != 0)
                return 0;
            list[slot] = unit;
        } else {
            unit = list[bestidx];
            unit->f34(unit, 0);
        }
    }
    Chan_004cf570* chan;
    if (unit->f00(unit, &DAT_004fcf68, &chan) == 0) {
        if (field_4 == 0 || pos == 0) {
            chan->f48(chan, 2, 0);
        } else {
            chan->f4c(chan, (float)pos->x, (float)pos->y, (float)pos->z, 0.0f);
            chan->f44(chan, field_8, 0);
            chan->f40(chan, field_c, 0);
            chan->f48(chan, 0, 0);
        }
        chan->f08(chan);
    }
    if (unit->f34(unit, 0) != 0)
        return 0;
    if (unit->f3c(unit, pos) != 0)
        return 0;
    if (unit->f30(unit, 0, 0, DAT_0051ff48 != 0) != 0)
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
