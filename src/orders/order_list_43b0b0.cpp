// Decompiled by space-bunny-free. Names are provisional.
// Adds `amount` to the object's last list node of the given kind and id when the
// amount is positive (asking FUN_0043adc0 to make a new node if there is none);
// when it is not positive it takes the amount off the matching node, deleting
// nodes (and asking again) until the amount is used up. The kind table's flag
// 0x40000 selects which of the object's two lists (+0x60 or +0x5c) is used.
// Header dependence: with <windows.h> (or most other headers) the kind table
// read inside the loop keeps the table pointer as the base of the address and
// the hoisted entry offset as its index; without a header MSVC swaps them.

#include <windows.h>

#pragma pack(push, 1)
struct KindEntry_0043b0b0 {
    char unknown_0[0x11];
    unsigned int flags;                // +0x11
    char unknown_15[0x19 - 0x15];
};

class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char kind;                // +0x4
    char unknown_5[0x36 - 0x5];
    int id;                            // +0x36
    int amount;                        // +0x3a
    char unknown_3e[0x42 - 0x3e];
    unsigned int flags;                // +0x42
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;              // +0x4a

    ~Class_0043a1f0();
};
#pragma pack(pop)

struct Owner_0043b0b0 {
    char unknown_0[0x5c];
    Class_0043a1f0* list;              // +0x5c
    Class_0043a1f0* list2;             // +0x60
};

extern KindEntry_0043b0b0* DAT_00512344;

void __stdcall FUN_0043adc0(unsigned char kind, int remove, Owner_0043b0b0* owner, int id, int* pos, int param_6, int param_7);

// FUNCTION: 0x43b0b0
void __stdcall FUN_0043b0b0(int kind, Owner_0043b0b0* owner, int id, int amount)
{
    Class_0043a1f0* node;
    if (amount > 0) {
        if (DAT_00512344[kind & 0xff].flags & 0x40000)
            node = owner->list2;
        else
            node = owner->list;
        while (node != 0 && node->next != 0) {
            node = node->next;
        }
        if (node != 0 && node->kind == (unsigned char)kind && node->id == id) {
            node->amount += amount;
            return;
        }
        FUN_0043adc0(kind, 1, owner, 0, 0, id, amount);
        return;
    }
    for (;;) {
        Class_0043a1f0* found = 0;
        if (DAT_00512344[kind & 0xff].flags & 0x40000)
            node = owner->list2;
        else
            node = owner->list;
        for (; node != 0; node = node->next) {
            if (node->kind == (unsigned char)kind && node->id == id)
                found = node;
        }
        if (found == 0)
            break;
        if (found->amount > -amount) {
            found->amount += amount;
            break;
        }
        Class_0043a1f0* first = owner->list;
        amount += found->amount;
        Class_0043a1f0** link = &owner->list;
        if (found->flags & 0x40000)
            link = &owner->list2;
        for (node = *link; node != 0; node = node->next) {
            if (node == found) {
                *link = found->next;
                if (found != first)
                    found->flags |= 0x10000;
                delete found;
                break;
            }
            link = &node->next;
        }
    }
}
