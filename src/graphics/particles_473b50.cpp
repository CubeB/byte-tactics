// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, verified by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
// MATCH (Claude Opus 5.5, #4321). What every earlier pass was missing: the
// original reads and writes the segment fields as direct members of `this`
// (macro-expanded code, see SPLIT_SEG), not through Vec3& or int& parameters.
// With direct members MSVC knows the fields do not alias, so it schedules each
// e.x/e.y store after the next component's loads and runs the first segment's
// last store into the second segment's loads, which no reference spelling
// does. Each component has to read its start into v first and then subtract
// the field again (`d = e - s`, a CSE use of v: `mov edx, esi; sub ecx, edx`
// in the z part); `d = e - v` drops 4 bytes and the CSE copy.
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

// Moves a segment's start 4/11 of the way along it and turns its end into the
// step from there to the 7/11 point. A macro, not an inline function: the
// original addresses every field as this + offset (start.y of seg_1c is
// [ebx+0x20], not [ebp+4]), and only direct member expressions give that.
#define SPLIT_SEG(g)                         \
    {                                        \
        int v, d;                            \
        v = (g).start.x;                     \
        d = (g).end.x - (g).start.x;         \
        (g).start.x = v + d * 4 / 11;        \
        (g).end.x = v + d * 7 / 11;          \
        v = (g).start.y;                     \
        d = (g).end.y - (g).start.y;         \
        (g).start.y = v + d * 4 / 11;        \
        (g).end.y = v + d * 7 / 11;          \
        v = (g).start.z;                     \
        d = (g).end.z - (g).start.z;         \
        (g).start.z = v + d * 4 / 11;        \
        (g).end.z = v + d * 7 / 11;          \
        (g).end.x -= (g).start.x;            \
        (g).end.y -= (g).start.y;            \
        (g).end.z -= (g).start.z;            \
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
    SPLIT_SEG(seg_34);
    SPLIT_SEG(seg_1c);
    v4();
}
