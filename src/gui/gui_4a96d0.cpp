// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Decrements the scroll offset (field_140) of GUI entry `index`, clamped to
// [0, field_136 - 1]. When the value actually changes it marks the object
// changed, refreshes the gadget and runs the entry's callback (if any).
// The int copy of the old value is what makes MSVC keep the sign-extended old
// in edx and compute the entry address before the first load; using only the
// short local lets it fold the base+index into the load.

#pragma pack(push, 1)
struct Entry_004a96d0 {                // 0x15b-byte entry
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

struct Data_004a96d0 {
    int unknown_0;
    Entry_004a96d0* entries;           // +0x4
};

struct Object_004a96d0 {
    char unknown_0[0x18];
    Data_004a96d0* data;               // +0x18
    char unknown_1c[0xcca - 0x1c];
    int changed;                       // +0xcca
};
#pragma pack(pop)

void __stdcall FUN_004a2580(Object_004a96d0* obj, int index);
void __stdcall FUN_004a2be0(Object_004a96d0* obj, int index);

// FUNCTION: 0x4a96d0
void __stdcall DecrementKnobPos(Object_004a96d0* obj, int index)
{
    Entry_004a96d0* e = &obj->data->entries[index];
    short raw = e->field_140;
    int old = raw;
    e->field_140 = raw - 1;
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
