// Decompiled by Opus. Names are provisional.

struct Rect_00440ca0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    int d;                             // +0xc
};

#pragma pack(push, 1)
struct Dst_00440ca0 {
    char unknown_0[0x99];
    Rect_00440ca0 rect;                // +0x99
};
#pragma pack(pop)

struct Src_00440ca0 {
    int unknown_0;
    Rect_00440ca0 rect;                // +0x4
};

// FUNCTION: 0x440ca0
void __stdcall FUN_00440ca0(Dst_00440ca0* dst, Src_00440ca0* src)
{
    dst->rect = src->rect;
}
