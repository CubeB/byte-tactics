// Decompiled by Opus. Names are provisional.

#pragma pack(push, 2)
struct Object_0049b680 {
    char unknown_0[0x1c];
    int x;                             // +0x1c
    int y;                             // +0x20
    int z;                             // +0x24
    char unknown_28[0x36 - 0x28];
    short angle1;                      // +0x36
    short angle2;                      // +0x38
    int length;                        // +0x3a
};
#pragma pack(pop)

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// FUNCTION: 0x49b680
void __stdcall FUN_0049b680(Object_0049b680* obj)
{
    obj->y = FUN_004b70ef(obj->angle2, obj->length);
    int r = FUN_004b7123(obj->angle2, obj->length);
    obj->x = -FUN_004b70ef(obj->angle1, r);
    obj->z = -FUN_004b7123(obj->angle1, r);
}
