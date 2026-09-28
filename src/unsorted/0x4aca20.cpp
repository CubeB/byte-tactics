// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes the red, green and blue bytes of one entry of a palette (4 bytes per
// entry) into the field_140 word of the "RED", "GREN" and "BLUE" GUI entries,
// then refreshes the object's gadget state.
// The entry pointer is held in a local `e`; with the entry addressed directly
// (`entries[i].field_140`) the compiler hoists the colour byte load above the
// FUN_0049fdf0 call and keeps it in bp, which pushes the object pointer out of
// ebp and changes the whole allocation.
#pragma pack(push, 1)
struct Entry_004aca20 {                // 0x15b-byte entry
    char unknown_0[0x140];
    unsigned short field_140;          // +0x140
    char unknown_142[0x15b - 0x142];
};

struct Data_004aca20 {
    int unknown_0;
    Entry_004aca20* entries;           // +0x4
};

struct Object_004aca20 {
    char unknown_0[0x18];
    Data_004aca20* data;               // +0x18
};
#pragma pack(pop)

int __stdcall FUN_0049fdf0(Entry_004aca20* entries, const char* name, int flag);
void __stdcall FUN_004a81e0(Object_004aca20* obj, int value);

// FUNCTION: 0x4aca20
void __stdcall FUN_004aca20(Object_004aca20* obj, unsigned char* colors, int index)
{
    Entry_004aca20* entries = obj->data->entries;
    Entry_004aca20* e;
    e = &entries[FUN_0049fdf0(entries, "RED", 4)];
    e->field_140 = colors[index * 4];
    e = &entries[FUN_0049fdf0(entries, "GREN", 4)];
    e->field_140 = colors[index * 4 + 1];
    e = &entries[FUN_0049fdf0(entries, "BLUE", 4)];
    e->field_140 = colors[index * 4 + 2];
    FUN_004a81e0(obj, 4);
}
