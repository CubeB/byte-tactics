// Decompiled by space-bunny-free. Names are provisional.
//
// The GUI layer's command handler, called by 0x4a9fd0 with a decoded key/command
// in `cmd`. It looks up the layer's currently selected gadget (layer->current,
// layer->entries) and switches on the command:
//   9     toggle a checkbox-ish gadget (IsKeyDown(0xf9))
//   0x1b  make the gadget whose stored name matches entries[0].choice2 current
//   0xd   same for entries[0].choice, then fall through into 0x20
//   0x20  activate the selected gadget (types 1, 2, 6); type 1 also cycles its
//         field_137 sub-index and re-selects it
//   0xf4/0xf6  scroll the list one line up / down (type 4 gadgets)
//   0xf5/0xf7  page the list up / down (type 2 gadgets)
// A handled command is returned as 0, an unhandled one unchanged. The common
// tail refreshes the holder when nothing consumed the command and records the
// newly selected gadget in obj->field_60.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a9b90;

struct Data_004a9b90 {
    int current;                       // +0x00
    Entry_004a9b90* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    void* list;                        // +0x14
    int field_18;                      // +0x18
    char unknown_1c[0x20 - 0x1c];
    int field_20;                      // +0x20
};

struct Object_004a9b90 {
    char unknown_0[0x18];
    Data_004a9b90* data;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60
    int field_64;                      // +0x64
    char unknown_68[0xcca - 0x68];
    int changed;                       // +0xcca
};

struct Entry_004a9b90 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char name[0x10];                   // +0x02
    char unknown_12[0x17 - 0x12];
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x28 - 0x1f];
    char group;                        // +0x28
    signed char field_29;              // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                       // +0xb6 (entry 0 only)
    char unknown_b8[0xcc - 0xb8];
    union {
        char choice[0x10];             // +0xcc (entry 0 stores names here)
        struct {
            char pad_ce[2];
            void (__stdcall* callback)(void*, Entry_004a9b90*); // +0xce
        } cb;
    };
    char choice2[0x10];                // +0xdc (entry 0 stores names here)
    char unknown_ec[0x136 - 0xec];
    unsigned char field_136;           // +0x136
    unsigned char field_137;           // +0x137
    short field_138;                   // +0x138
    unsigned char field_13a;           // +0x13a
    unsigned char field_13b;           // +0x13b
    unsigned char field_13c;           // +0x13c
    char unknown_13d[0x15b - 0x13d];
};
#pragma pack(pop)

void __stdcall FUN_004a0340(Object_004a9b90* obj, int index);
void __stdcall DrawButton(Object_004a9b90* obj, int index);
void __stdcall FUN_004a7960(Object_004a9b90* obj, int value);
void __stdcall DecrementKnobPos(Object_004a9b90* obj, int index);
void __stdcall IncrementKnobPos(Object_004a9b90* obj, int index);
void __stdcall FUN_004a9830(Object_004a9b90* obj, int index);
void __stdcall FUN_004a99c0(Object_004a9b90* obj, int index);
void PopKey();
int __stdcall IsKeyDown(int id);

static inline int FindEntry_004a9b90(Entry_004a9b90* entries, char* name)
{
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a9b90
int __stdcall HandleGuiCommand(Object_004a9b90* obj, int cmd)
{
    int newsel = -1;
    int index = obj->data->field_20;
    Entry_004a9b90* entries = obj->data->entries;
    Entry_004a9b90* e = &entries[index];
    int type = e->type;

    // Case order follows the original emit order, not ascending.
    switch (cmd) {
    case 9:
        if (IsKeyDown(0xf9))
            FUN_004a7960(obj, 0);
        else
            FUN_004a7960(obj, 1);
        obj->changed = 1;
        cmd = 0;
        break;
    case 0x1b:
        {
            int found = FindEntry_004a9b90(entries, entries[0].choice2);
            if (found == -1 || entries[found].field_29 == 0)
                break;
            newsel = found;
        }
        cmd = 0;
        break;
    case 0xd:
        if (obj->field_64 != -1 && entries[obj->field_64].type == 3)
            break;
        {
            int found = FindEntry_004a9b90(entries, entries[0].choice);
            if (found != -1 && entries[found].field_29 != 0
                && !(entries[found].type == 1 && (entries[found].field_13c & 1))) {
                newsel = found;
                cmd = 0;
                break;
            }
        }
        // fall through to case 0x20
    case 0x20:
        if (type == 3)
            break;
        if (type != 1 && type != 2 && type != 6)
            break;
        if (e->field_29 == 0)
            break;
        if (type == 1 && (e->field_13c & 1))
            break;
        newsel = index;
        if (type == 1) {
            if (e->flags & 0x10) {
                e->field_138 = 1;
                FUN_004a0340(obj, index);
                DrawButton(obj, index);
            }
        }
        if (type == 1 && e->field_136 != 0) {
            // Wrap through a pointer: a local copy makes the field_136 reload differ.
            unsigned char* p = &e->field_137;
            if (++*p >= e->field_136)
                *p = 0;
        }
        cmd = 0;
        break;
    case 0xf5:
        if (type == 2) {
            FUN_004a9830(obj, index);
            if (e->cb.callback)
                e->cb.callback(obj, e);
        } else {
            FUN_004a7960(obj, 2);
        }
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf4:
        if (type == 3)
            break;
        if (type == 4 && e->field_17 > e->field_19)
            DecrementKnobPos(obj, index);
        else
            FUN_004a7960(obj, 0);
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf7:
        if (type == 2) {
            FUN_004a99c0(obj, index);
            if (e->cb.callback)
                e->cb.callback(obj, e);
        } else {
            FUN_004a7960(obj, 3);
        }
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf6:
        if (type == 3)
            break;
        if (type == 4 && e->field_17 > e->field_19)
            IncrementKnobPos(obj, index);
        else
            FUN_004a7960(obj, 1);
        obj->changed = 1;
        cmd = 0;
        break;
    }
    if (cmd == 0 && obj->data->field_18 == 0)
        PopKey();
    if (newsel != -1) {
        obj->field_60 = newsel;
        obj->changed = 1;
    }
    return cmd;
}
