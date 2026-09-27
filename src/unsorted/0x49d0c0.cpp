// Decompiled by Opus. Names are provisional.

struct Flags_0049d0c0 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int unknown_2 : 18;
    unsigned int bit20 : 1;
};

#pragma pack(push, 1)
struct Info_0049d0c0 {
    char unknown_0[0x111];
    Flags_0049d0c0 flags;              // +0x111
};
#pragma pack(pop)

struct Object_0049d0c0 {
    char unknown_0[0xc];
    Info_0049d0c0* info;               // +0xc
};

int __stdcall FUN_0049c9c0(Object_0049d0c0* obj, int a, int b, int c, int d);
int __stdcall FUN_0049cde0(Object_0049d0c0* obj, int a, int b, int c, int d);

// The last two arguments are passed to both callees in swapped order.
// FUNCTION: 0x49d0c0
int __stdcall FUN_0049d0c0(Object_0049d0c0* obj, int a, int b, int d, int c)
{
    int result = 0;
    if (obj->info->flags.bit0 || obj->info->flags.bit20)
        result = FUN_0049c9c0(obj, a, b, c, d);
    else if (obj->info->flags.bit1)
        result = FUN_0049cde0(obj, a, b, c, d);
    return result;
}
