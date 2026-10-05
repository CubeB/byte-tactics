// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Obj_0049c980 {
    char unknown_0[0x3a];
    int value;                         // +0x3a
};
#pragma pack(pop)

struct Src_0049c980 {
    char unknown_0[0x68];
    int f_68;                          // +0x68
    int f_6c;                          // +0x6c
    int f_70;                          // +0x70
};

// FUNCTION: 0x49c980
void __stdcall FUN_0049c980(Obj_0049c980* obj, Src_0049c980* src)
{
    if (src->f_6c) {
        obj->value = src->f_6c;
        return;
    }
    if (src->f_70 == 0) {
        obj->value = src->f_68;
        return;
    }
    obj->value = 0;
}
