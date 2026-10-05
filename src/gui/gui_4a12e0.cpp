// Decompiled by space-bunny-free. Names are provisional.
// Sets a flag of the layout entry `index` (0x15b-byte entries, entry 0 holds
// the count at +0xb6). Type 4 sets field 0x157 and then copies the flag to
// every type-1 entry with the same byte at +0x01 whose flags word has both
// bits 0x1800. The other types write one flag bit. The `or`/`and` in type 2
// and the `& 1` merges come from the source's explicit masks.
// <windows.h> is needed: without it MSVC 5 swaps which operand of the
// commutative `xor` it loads first, and the value lands in ebx instead of edx.

#include <windows.h>

#pragma pack(push, 1)
struct Entry_004a12e0 {                // 0x15b bytes
    unsigned char type;               // +0x00, the switch value
    unsigned char group;              // +0x01
    char name[0x10];                  // +0x02
    char unknown_12[0x1b - 0x12];
    int flags;                        // +0x1b, bits 11 and 12 select a group
    char unknown_1f[0xb6 - 0x1f];
    short count;                      // +0xb6 (entry 0)
    char unknown_b8[0xbc - 0xb8];
    unsigned short flag_bc;           // +0xbc, bit 0 set by type 12
    char unknown_be[0x13c - 0xbe];
    unsigned short flag;              // +0x13c, bit 0 set by type 1
    char unknown_13e[0x148 - 0x13e];
    unsigned int flag_148;            // +0x148, bit 0 set by type 5
    char unknown_14c[0x157 - 0x14c];
    int field_157;                    // +0x157
};
#pragma pack(pop)

struct Holder_004a12e0 {
    char unknown_0[4];
    Entry_004a12e0* entries;           // +0x04
};

struct Class_004a12e0 {
    char unknown_0[0x18];
    Holder_004a12e0* holder;           // +0x18
};

// FUNCTION: 0x4a12e0
void __stdcall FUN_004a12e0(Class_004a12e0* obj, int index, int value)
{
    Entry_004a12e0* entries = obj->holder->entries;
    if (index == -1) {
        return;
    }
    Entry_004a12e0* e = &entries[index];
    switch (e->type) {
    case 4: {
        e->field_157 = value;
        unsigned char group = e->group;
        for (int i = 1; i < entries->count + 1; i++) {
            if (entries[i].type == 1 && entries[i].group == group && (entries[i].flags & 0x1800)) {
                entries[i].flag = (entries[i].flag & 0xfffe) | (value & 1);
            }
        }
        break;
    }
    case 1:
        e->flag = (e->flag & 0xfffe) | (value & 1);
        break;
    case 2:
        if (value) {
            e->flags |= 0x100;
        } else {
            e->flags &= 0xfffffeff;
        }
        break;
    case 5:
        e->flag_148 = (e->flag_148 & 0xfffffffe) | (value & 1);
        break;
    case 12:
        e->flag_bc = (e->flag_bc & 0xfffe) | (value & 1);
        break;
    default:
        break;
    }
}
