// Decompiled by Sonnet. Names are provisional.

struct Entry_004b7f70 {
    unsigned short value;
    char unknown_2[6];
};

struct Obj_004b7f70 {
    unsigned short index;
    char unknown_2[6];
    void* table;
};

static inline Entry_004b7f70* GetEntries(Obj_004b7f70* obj)
{
    return (Entry_004b7f70*)((char*)obj->table + 0x2c);
}

// FUNCTION: 0x4b7f70
unsigned short __stdcall FUN_004b7f70(Obj_004b7f70* param1)
{
    unsigned short result;
    if (param1->table != 0) {
        result = GetEntries(param1)[param1->index].value;
    } else {
        result = 0xffff;
    }
    return result;
}
