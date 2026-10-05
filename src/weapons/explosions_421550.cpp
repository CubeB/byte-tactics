// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

struct Vec3_004b6cc0 {
    int x;
    int y;
    int z;
};

struct Arr_00421550 {
    char unknown_0[4];
    int count;                    // +0x4
    char unknown_8[0x1c];
    Vec3_004b6cc0* items;         // +0x24
};

#pragma pack(push, 1)
struct Inner_00421550 {
    Arr_00421550* f0;             // +0x0
    char unknown_4[0xc];
    short f10;                    // +0x10
    short f12;                    // +0x12
    short f14;                    // +0x14
    int f16;                      // +0x16
    int f1a;                      // +0x1a
    int f1e;                      // +0x1e
    Vec3_004b6cc0* f22;           // +0x22
};
#pragma pack(pop)

struct Obj_00421170 {
    char unknown_0[0x28];
    unsigned int b0 : 1;          // +0x28
    unsigned int b1 : 1;
    Inner_00421550* inner;        // +0x2c
};

void __stdcall EmitWhiteSmoke(void* buf, int arg);
void __stdcall FUN_00472ab0(void* buf, int arg);
void __stdcall RotateByAngles(Vec3_004b6cc0* in, Vec3_004b6cc0* out, short* angles);
void __stdcall DrawExplodedPieceFaces(int param_1, Obj_00421170* obj, Inner_00421550* inner);

// FUNCTION: 0x421550
int __stdcall DrawExplodedPiece(int param_1, Obj_00421170* obj)
{
    Inner_00421550* inner = obj->inner;
    short angles[3];
    int buf[3];

    if (obj->b1) {
        buf[0] = inner->f16;
        buf[1] = inner->f1a;
        buf[2] = inner->f1e;
        EmitWhiteSmoke(buf, 9);
    }
    if (obj->b0) {
        buf[0] = inner->f16;
        buf[1] = inner->f1a;
        buf[2] = inner->f1e;
        FUN_00472ab0(buf, 9);
    }

    angles[0] = inner->f14;
    angles[2] = inner->f10;
    angles[1] = inner->f12;

    int n = inner->f0->count;
    for (int i = n - 1; i >= 0; i--) {
        RotateByAngles(&inner->f0->items[i], &inner->f22[i], angles);
    }

    DrawExplodedPieceFaces(param_1, obj, inner);
    return 1;
}
