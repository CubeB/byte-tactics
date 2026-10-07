// Decompiled by Space Bunny Free. Names are provisional.
// Increments the scroll offset (field_140) of GUI entry `index`, clamped to
// [0, field_136 - 1]. When the value actually changes it marks the object
// changed, refreshes the gadget and runs the entry's callback (if any).
// Note: the upper clamp still uses field_136 - 1, as the copy-paste source of
// this function did, even though this side scrolls the other way.

#pragma pack(push, 1)
struct Entry_004a9780 {                // 0x15b-byte entry
    char unknown_0[0x136];
    short field_136;                   // +0x136
    char unknown_138[0x140 - 0x138];
    short field_140;                   // +0x140
    char unknown_142[2];
    void (__stdcall* handler)(void* obj, int arg); // +0x144
    char unknown_148[2];
    int field_14a;                     // +0x14a
    char unknown_14e[0x15b - 0x14e];
};

struct Data_004a9780 {
    int unknown_0;
    Entry_004a9780* entries;           // +0x4
};

struct Object_004a9780 {
    char unknown_0[0x18];
    Data_004a9780* data;               // +0x18
    char unknown_1c[0xcca - 0x1c];
    int changed;                       // +0xcca
};
#pragma pack(pop)

void __stdcall FUN_004a2580(Object_004a9780* obj, int index);
void __stdcall FUN_004a2be0(Object_004a9780* obj, int index);

// Must stay a static inline helper: written inline it changes the load order.
static inline Entry_004a9780* entry_at(Object_004a9780* obj, int index)
{
    return &obj->data->entries[index];
}

// FUNCTION: 0x4a9780
void __stdcall IncrementKnobPos(Object_004a9780* obj, int index)
{
    Entry_004a9780* e = entry_at(obj, index);
    short raw = e->field_140;
    // Keep the int copy of the old value: it fixes the register used for it.
    int old = raw;
    e->field_140 = raw + 1;
    if (e->field_140 > e->field_136 - 1) {
        e->field_140 = e->field_136 - 1;
    }
    if (e->field_140 < 0) {
        e->field_140 = 0;
    }
    if (e->field_140 != old) {
        obj->changed = 1;
        FUN_004a2580(obj, index);
        FUN_004a2be0(obj, index);
    }
    if (e->handler) {
        e->handler(obj, e->field_14a);
    }
}
