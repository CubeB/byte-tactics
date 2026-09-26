// Decompiled by Opus. Names are provisional.

struct Item_4afec0 {
    char used;                       // +0x0
    char unknown_1[0xa3];
};

#pragma pack(push, 1)
struct Class_004afec0 {
    char unknown_0[0xa6];
    Item_4afec0* items;              // +0xa6
    int itemCount;                   // +0xaa
};
#pragma pack(pop)

// FUNCTION: 0x4afec0
void __stdcall FUN_004afec0(Class_004afec0* obj)
{
    if (obj->items != 0) {
        for (int i = 0; i < obj->itemCount; i++) {
            obj->items[i].used = 0;
        }
    }
}
