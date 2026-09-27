// Decompiled by space-bunny-free. Names are provisional.
// Sibling of 0x4736e0 and 0x4742c0: the same base call with the third
// argument, the same two 24-byte copies and the same trailing virtual call.
// This one keeps each 24-byte block as a {point, far point} pair, moves the
// point 4/11 of the way along it, and re-aims the far point 3/11 further on,
// so the block ends up holding {point at 4/11, that point's own delta}.
//
// Both divisions are the signed /11 magic 0x2e8ba2e9, the 7/11 one written as
// (d << 3) - d, so they have to be `d * 4 / 11` and `d * 7 / 11` on the same
// difference d. The old start point is kept in a local: the compiler holds it
// in a register (esi) across both stores, as the original does, but the two
// results have to be temporaries assigned to the fields, not written straight
// into them, or the 4/11 point is kept in a register for the subtraction
// below instead of being stored and re-read.
//
// The z component is the odd one out in the original too: its delta is the
// difference of the two lerps computed in registers, with the 4/11 point
// living in edi from before the x and y deltas are re-read from memory until
// the subtraction. `e.z = bz - az` with az and bz locals is what produces that;
// writing the subtraction against the field, or leaving one of the two out of
// a temporary, does not.
//
// Still differs (87.5%, 497 of 499 bytes), all of it scheduling inside the two
// copies of the helper:
//   - the 7/11 point is stored just before the next component's loads, where
//     the original sinks the store past them (x and y, in both blocks);
//   - the z 4/11 point is formed as `add edx, esi; mov edi, edx` instead of
//     `lea edi, [edx + esi]`, and its store lands before the x delta instead of
//     between the x delta's subtract and its store;
//   - the z delta is emitted as `sub edx, edi; add edx, esi` where the
//     original adds sz first and subtracts afterwards: MSVC reassociates
//     `bz - az` no matter how the two lerps are spelled;
//   - in the second block the base register ebp (= this + 0x1c) is also used
//     for start.y and start.z, where the original drops back to [ebx+0x20]
//     and [ebx+0x24], and the x delta is subtracted into eax where the
//     original reuses ebp (freeing the base register).
// I could not move any of those from the source: every spelling of the two
// divisions, of the two temporaries and of the three deltas lands on the same
// code, and no header set changes it either.

class Class_00471d70 {
public:
    void FUN_00471d70(int param_1);
};

struct Vec3_00473b50 {
    int x;
    int y;
    int z;
};

struct Seg_00473b50 {
    Vec3_00473b50 start;
    Vec3_00473b50 end;
};

static inline void Split_00473b50(Vec3_00473b50& s, Vec3_00473b50& e)
{
    int sx = s.x;
    int dx = e.x - sx;
    int ax = sx + dx * 4 / 11;
    int bx = sx + dx * 7 / 11;
    s.x = ax;
    e.x = bx;
    int sy = s.y;
    int dy = e.y - sy;
    int ay = sy + dy * 4 / 11;
    int by = sy + dy * 7 / 11;
    s.y = ay;
    e.y = by;
    int sz = s.z;
    int dz = e.z - sz;
    int az = sz + dz * 4 / 11;
    int bz = sz + dz * 7 / 11;
    s.z = az;
    e.x = e.x - s.x;
    e.y = e.y - s.y;
    e.z = bz - az;
}

class Class_00473b50 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    char unknown_4[0x1c - 4];
    Seg_00473b50 seg_1c;
    Seg_00473b50 seg_34;
    void FUN_00473b50(Seg_00473b50* a, Seg_00473b50* b, int c);
};

// FUNCTION: 0x473b50
void Class_00473b50::FUN_00473b50(Seg_00473b50* a, Seg_00473b50* b, int c)
{
    ((Class_00471d70*)this)->FUN_00471d70(c);
    seg_1c = *a;
    seg_34 = *b;
    Split_00473b50(seg_34.start, seg_34.end);
    Split_00473b50(seg_1c.start, seg_1c.end);
    v4();
}
