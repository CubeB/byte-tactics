// Decompiled by Opus. Names are provisional.
// Makes `ctx` the current context (DAT_0051fba4) and resets its state.
#include <string.h>

#pragma pack(push, 1)
class Class_0051fba4 {
public:
    char unknown_0[0x4];
    int field_4;                       // +0x4
    int field_8[3];                    // +0x8
    int field_14;                      // +0x14
    int field_18;                      // +0x18
    char unknown_1c[0x68 - 0x1c];
    int field_68;                      // +0x68
    char unknown_6c[0x78 - 0x6c];
    int field_78;                      // +0x78
    char unknown_7c[0x96 - 0x7c];
    unsigned int time;                 // +0x96
    int field_9a;                      // +0x9a
    int field_9e;                      // +0x9e
    int field_a2;                      // +0xa2
    char unknown_a6[0x9b6 - 0xa6];
    char text_9b6[0x100];              // +0x9b6
    char text_ab6[0x100];              // +0xab6
    char text_bb6[0x110];              // +0xbb6
    int field_cc6;                     // +0xcc6
    char unknown_cca[0xcd2 - 0xcca];
    int field_cd2;                     // +0xcd2
    char field_cd6;                    // +0xcd6
};
#pragma pack(pop)

extern Class_0051fba4* DAT_0051fba4;

unsigned int __cdecl FUN_004b6340();

// FUNCTION: 0x4aa850
void __stdcall FUN_004aa850(Class_0051fba4* ctx)
{
    DAT_0051fba4 = ctx;
    ctx->field_18 = 0;
    ctx->text_9b6[0] = 0;
    ctx->text_ab6[0] = 0;
    ctx->text_bb6[0] = 0;
    ctx->field_cc6 = 1;
    ctx->time = FUN_004b6340();
    ctx->field_9a = 0;
    ctx->field_9e = 1;
    ctx->field_4 = 0;
    ctx->field_cd2 = 0;
    ctx->field_cd6 = 0;
    ctx->field_78 = 0;
    ctx->field_68 = -1;
    memset(ctx->field_8, 0, 12);
    ctx->field_14 = 0;
    ctx->field_a2 = 1;
}
