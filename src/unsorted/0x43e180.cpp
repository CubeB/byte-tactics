// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Builds a three-short offset from the object's position fields (+0x64,
// +0x66, +0x68) and the animation record at obj->recs[index] (+0x9e): the
// record's +0x32/+0x34 words pair with y/z and the next record's +0 word
// with x. The two static helpers keep the operand order (object field
// first, record second) that the original used.

struct Rec_0043e180 {
    short a;                       // +0x00
    char pad[0x30];                // +0x02
    short b;                       // +0x32
    short c;                       // +0x34
};

#pragma pack(push, 2)
struct Obj_0043e180 {
    char pad0[0x64];
    short f64;                     // +0x64
    short f66;                     // +0x66
    short f68;                     // +0x68
    char pad1[0x9e - 0x6a];
    Rec_0043e180* recs;            // +0x9e
};
#pragma pack(pop)

struct Out_0043e180 {
    short x;
    short y;
    short z;
};

static inline short SumY_0043e180(Obj_0043e180* obj, Rec_0043e180* q)
{
    return obj->f66 + q->c;
}

static inline short SumZ_0043e180(Obj_0043e180* obj, Rec_0043e180* q)
{
    return obj->f68 + q->b;
}

// FUNCTION: 0x43e180
Out_0043e180 __stdcall FUN_0043e180(Obj_0043e180* obj, int index)
{
    Rec_0043e180* r = obj->recs;
    Out_0043e180 p;
    p.z = SumZ_0043e180(obj, &r[index]);
    p.y = SumY_0043e180(obj, &r[index]);
    p.x = obj->f64 + r[index + 1].a;
    return p;
}
