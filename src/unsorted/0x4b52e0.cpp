// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Class_004b52e0 {
    char unknown_0[0x1ea];
    int clipLeft;                   // +0x1ea
    int clipTop;                    // +0x1ee
    int clipRight;                  // +0x1f2
    int clipBottom;                 // +0x1f6
    int width;                      // +0x1fa
    int height;                     // +0x1fe
    unsigned short flags : 10;      // +0x202
    unsigned short rest : 6;
    char unknown_204[0x614 - 0x204];
    float scale;                    // +0x614
};
#pragma pack(pop)

// FUNCTION: 0x4b52e0
void __stdcall FUN_004b52e0(Class_004b52e0* p)
{
    p->flags = 0;
    p->width = 640;
    p->height = 480;
    p->clipLeft = 0;
    p->clipTop = 0;
    p->clipRight = 639;
    p->clipBottom = 479;
    p->scale = 1.0f;
}
