// Decompiled by Opus. Names are provisional.

struct Obj_004b7f00 {
    unsigned short index;              // +0x0
    char unknown_2[6];
    unsigned short* table;             // +0x8, starts with the entry count
};

// FUNCTION: 0x4b7f00
int __stdcall SetGafSequenceFrame(Obj_004b7f00* obj, int index)
{
    int result = 0;
    if (obj->table != 0 && *obj->table > index) {
        obj->index = index;
        result = 1;
    }
    return result;
}
