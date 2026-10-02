// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// PARTIAL 46.6%. Size 1891 vs original 1853 bytes (38 bytes long).
// Recovered from a permuter run whose best_ratio.cpp was never copied back (that
// file alone was 39.1% to 42.8%), then 42.8% to 43.6% by hand, then 44.5%, 45.6%
// and 46.6% from three more permuter batches. Every score below is measured
// against the body named, so none of it is comparable to the older 33.0% notes.
// A METHOD WARNING, because it cost a full batch on this file: comparing two
// permuter RUNS to attribute a gain to one line is CONFOUNDED. Runs 21 and 22
// differed by four edits at once, and reading the 45.43-against-45.63 gap as
// "the `char* tmp3 = g_game` temporary is noise" was WRONG: run 33 re-added the
// same temporary (as tmp9) and that single line is worth +0.94, isolated below.
// Always isolate a run's diff before believing which line did the work.
// TWO PERMUTER FINDINGS WORTH RECORDING, both measured here:
//  * `best.json`'s `best_ratio` field did not point at `best_ratio.cpp`. Run 5's
//    summary recorded best_ratio 0.4029 while the file it had written was
//    0.4279, which is why that run read as finding nothing. Always probe the
//    candidate files on disk; never trust the JSON summary.
//  * best_ratio.cpp beat best.cpp and best_raw.cpp in EVERY run measured (run 12:
//    44.48 vs 43.58/43.58; runs 21-24: 45.43, 45.63, 45.29, 45.29 as the ratio
//    files against 45.43, 44.75, 45.29, 45.29 as the best files; runs 31-34:
//    46.57, 46.57, 46.57, 46.43 against 45.89, 46.57, 45.89, 45.89). Same
//    pattern as 0x43f0e0 in the guide: the ratio variant is the one to copy back
//    here. Note run 32 is the one case where best.cpp tied its ratio file, so
//    the claim is "wins or ties, never loses", not "always wins outright".
// What gained 44.5% to 45.6% (run 22's best_ratio, four edits):
//  * The b0 arm's `p->field_4a < (unsigned int)gt` moved into a new helper
//    `inl0(p, gt)`, giving that one compare a boundary. This is the guide's
//    "give a single load a boundary" technique and it is the largest of the four.
//  * `int tmp30 = p->vel.z;` merged onto the line that uses it, and the tail's
//    last two `&&` operands folded onto one line. Cosmetic, but they are part of
//    the same candidate so their individual worth is not yet separated.
//  * `short tmp31 = p->counter; if (0 == tmp31)` instead of testing
//    `p->counter` directly: the compare result is parked in a local first,
//    the guide's technique 4.
//  * the frame-number load collapsed from a temporary to `g_game` inline. That
//    one was later REVERSED by run 33; see the 46.6% note below.
// What gained 45.6% to 46.6% (run 33's best_ratio, isolated one at a time):
//  * `char* tmp9 = g_game;` in front of the frame-number load: +0.94, and it is
//    the ONLY load-bearing edit of the three. The temporary exists to give that
//    one load a boundary, the guide's technique 2. Dropping it is -0.94.
//  * `int a; a = FUN_004b6c30(type->field_ee);` instead of `int a = ...`: inert.
//  * inl22 as a one-line `return` instead of a ret-locals pair: inert.
//    Both inert edits are kept as they stand because they are byte-identical and
//    the split form is what the rest of the permuter output uses, but neither is
//    worth points and neither should be credited with any.
// What gained 43.6% to 44.5%, three edits from run 12's best_ratio:
//  * `int* tmp18 = ..., offset;` as ONE multi-declarator instead of two
//    statements. This is declaration SHAPE, and it is load-bearing here even
//    though every permutation of the declaration ORDER measured inert.
//  * `idx = idx + 1;` instead of `idx++;` in the three-slot search loop.
//  * The b1 arm's guard inverted to `if (p->field_46 <= ...) { } else { ... }`,
//    worth +0.9 alone. The same flip on the b3 test two arms earlier is -2.32,
//    so these two empty arms are NOT the same shape and must be flipped
//    independently. An empty arm is not automatically noise.
// What gained 42.8% to 43.6%:
//  * THE INCLUDES WERE REMOVED. This file now has none. The 256-subset sweep in
//    build/scratch/0x49b720/hdrsweep.py found exactly two distinct scores across
//    all 256 subsets of {windows.h, ddraw.h, stdio.h, stdlib.h, math.h,
//    string.h, memory.h, assert.h}: 43.58% with no windows.h, 43.23% with it.
//    So <windows.h> was COSTING 0.35 points and the older note that including it
//    helps was wrong for this function.
//  * ApplyVel now reads pos.y, pos.x, pos.z in that order instead of
//    pos.x, pos.y, pos.z, matching the original's slot-access order. +0.37.
//  * The loop tail's two s tests plus the two v guards are one && chain, so all
//    four compares share the original's single failure label. +0.07, and 8 bytes
//    smaller (1880 to 1872) before the include change.
//  * 19 of the permuter's helper accessors were measured inert call by call and
//    collapsed back into plain expressions (build/scratch/0x49b720/collapse.py).
//    Byte-identical, but the file went from 31 helpers to 11.
// Helpers that ARE load-bearing, do not inline them: inl18 (-2.00 if inlined),
// inl12 (-0.40), inl9 (an int return is -2.51), inl15 (an int return is -0.32).
// Residuals, biggest first. The prologue, recoil and Drift figures are against
// 43.23%, the body before the include change; the rest is against this body:
//  1. Prologue. The original loads g_game into eax BEFORE `sub esp, 0x14`, then
//     re-reads it into ecx after the three pushes; ours loads once into ecx and
//     CSEs the second use. The original homes count to [esp+0x18] after the
//     pushes, ours spills to [esp+0xc] before the branch. This resisted every
//     spelling tried: count declared first with an inline initialiser, count and
//     the array read through separate helpers, g_game hoisted into its own
//     `char* gbase` local, the two reads spelled with opposite addend order, the
//     guard written `0 < count` or `!(count <= 0)`, and an early return instead
//     of the guard. All inert. Declaration order was swept too: all 210 adjacent
//     swaps of the function-scope declarations are byte-identical
//     (build/scratch/0x49b720/dsweep.py), so ORDER is inert and should not be
//     retried, even though the declaration SHAPE above was worth 0.9.
//  2. `s` is read by the original as `mov cx, word ptr [ebp+esi+0xa]` BEFORE
//     `add ebp, esi`, from base+offset rather than from p, then sign extended
//     into edx; ours reads `movsx ebx, word ptr [ebp+0xa]` after the add.
//     Reading s before p is -1.09. A separate base-pointer local for the array
//     is -15.47, as char* and as int, which is the strongest signal here that
//     the two-register SIB form is not reachable from this source shape.
//  3. The recoil block: the original does `sub cx, ax` in 16 bits then
//     `add edi, ecx`; ours sign extends ang to eax and rebuilds the sum with
//     `lea edi,[eax+ebx]`. Casting a, t or the sum to short gives -0.18 to
//     -0.36, routing the sum through a short local reused for both calls is
//     -2.36, and a short t or plain source is byte-identical. The block's widths
//     are already right, so this is a register-allocation difference.
//  4. The empty `if (!type->flags.bits.b3) { } else { ... }` in the b0 path is
//     load-bearing as written; the positive-test spelling is -2.32. Do not tidy
//     it, and see the b1 note above for why the other one flips.
//  5. Drift and Drift2 are each -0.73 and -0.91 when their stores are reordered
//     to match the original's slot order, the opposite of ApplyVel. Leave both.
// Harness: build/scratch/0x49b720/probe.py is a compile-only probe reusing
// check.py's own comparison, ~0.5 s per variant instead of ~60 s. A `d` of 0 in
// its output means "same as this file", NOT "matches the original".

#pragma pack(push, 1)

struct Vec_0049b720 {
    int x;
    int y;
    int z;
};

union UF_0049b720 {
    unsigned int raw;
    struct {
        unsigned int b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1;
        unsigned int b8:1,b9:1,b10:1,b11:1,b12:1,b13:1,b14:1,b15:1;
        unsigned int b16:1,b17:1,b18:1,b19:1,b20:1,b21:1,b22:1,b23:1;
        unsigned int b24:1,b25:1,b26:1,b27:1,b28:1,b29:1,b30:1,b31:1;
    } bits;
};

struct WType_0049b720 {
    char unknown_0[0x68];
    int field_68;                      // +0x68
    char unknown_6c[4];
    int field_70;                      // +0x70
    char unknown_74[0x7c - 0x74];
    void* field_7c;                    // +0x7c
    char unknown_80[0xe6 - 0x80];
    unsigned short field_e6;           // +0xe6
    char unknown_e8[0xec - 0xe8];
    unsigned short field_ec;           // +0xec
    unsigned short field_ee;           // +0xee
    unsigned short field_f0;           // +0xf0
    unsigned short field_f2;           // +0xf2
    unsigned short field_f4;           // +0xf4
    char unknown_f6[0xfa - 0xf6];
    unsigned short field_fa;           // +0xfa
    unsigned short field_fc;           // +0xfc
    unsigned short field_fe;           // +0xfe
    char unknown_100[0x111 - 0x100];
    UF_0049b720 flags;                 // +0x111
};

struct Proj_0049b720 {
    WType_0049b720* type;              // +0x0
    Vec_0049b720 pos;                  // +0x4
    Vec_0049b720 start;                // +0x10
    Vec_0049b720 vel;                  // +0x1c
    char unknown_28[0x34 - 0x28];
    short field_34;                    // +0x34
    short heading;                     // +0x36
    short pitch;                       // +0x38
    int field_3a;                      // +0x3a
    int field_3e;                      // +0x3e
    int field_42;                      // +0x42
    unsigned int field_46;                      // +0x46
    int field_4a;                      // +0x4a
    void* field_4e;                    // +0x4e
    int* field_52;                     // +0x52
    void* field_56;                    // +0x56
    char unknown_5a[0x60 - 0x5a];
    short counter;                     // +0x60
    short field_62;                    // +0x62
    short field_64;                    // +0x64
    char unknown_66[0x69 - 0x66];
    unsigned short flags69;            // +0x69
};

#pragma pack(pop)

extern char* g_game;

void __stdcall FUN_0049ae20();
int __stdcall FUN_0049b090(WType_0049b720* type, Proj_0049b720* p);
Vec_0049b720* __stdcall FUN_0049b3e0(Proj_0049b720* p);
int __stdcall FUN_0049b520(Proj_0049b720* p, Vec_0049b720* target);
void __stdcall FUN_00499eb0(Proj_0049b720* p, void* unit);
void __stdcall FUN_0043e240(int* arr, Vec_0049b720* pos, unsigned int idx, unsigned int value);
void __stdcall FUN_0047f300(unsigned int sound, Vec_0049b720* pos, int value);
void __stdcall FUN_00472810(Vec_0049b720* pos, int value);
void* __stdcall FUN_004815a0(Vec_0049b720* pos);
void __stdcall FUN_00420a30(Vec_0049b720* pos, void* value, int a, int b);
int __stdcall FUN_004b6c30(int range);
int __cdecl FUN_004b70ef(int angle, int distance);
int __cdecl FUN_004b7123(int angle, int distance);

static inline bool inl9(Proj_0049b720*p) { bool ret2 = (p->flags69 & 1) != 0;
return ret2; }

static inline char* inl12() { char* ret0;
ret0 = 0x141f7 + g_game;
return ret0; }

static inline int inl18() { char* tmp24 = g_game;
int tmp27 = *(int*)(tmp24 + 0x38a47), ret3;
ret3 = ((int)tmp27);
return ret3; }

static inline bool inl20(unsigned int tmp17, int s) { bool ret1;
ret1 = (tmp17) == 0
                        || s < (short)(unsigned char)*(g_game + 0x1427f);
return ret1; }

static inline int inl6(Proj_0049b720*p, WType_0049b720*type) { return p->heading - (type->field_ee >> 1); }

static inline int inl5(Proj_0049b720*p) { return p->vel.y; }

static inline bool inl15(unsigned int fl) { return 0 != ((fl >> 0x17) & 1); }

static inline int inl21(char*tmp15) { return *(int*)tmp15; }

static inline unsigned short inl22(WType_0049b720*type) { return type->field_f2; }

static inline unsigned short inl29(unsigned int e) { return (unsigned short)((0x30 & ((0xff & e) ^ ((e & 0xfff0) + 0x10))) ^ e); }

static inline void inl7(Proj_0049b720*p) { p->start.x += p->vel.x; }


static inline bool inl0(Proj_0049b720*p, int gt) { return p->field_4a < (unsigned int)gt; }

// FUNCTION: 0x49b720
void __cdecl FUN_0049b720()
{
    unsigned int tmp19;
    int tmp6;
    int tmp23;
    short* tmp4;
    WType_0049b720* type;
    unsigned short sl;
    char* tmp15;
    unsigned int tmp1, flag;
    unsigned short tmp0;
    char* tmp10;
    Proj_0049b720* q;
    unsigned short tmp8;
    int tmp2;
    short ang;
    unsigned int fl;
    unsigned short ec;
    unsigned char idx;
    int * arr, s;
    int* tmp18 = (int*)(g_game + 0x141f3), offset;
    int count;
    count = *tmp18;

    Proj_0049b720* p;
    if (count > 0) {
        offset = 0;
        offset = (unsigned int)offset;
        while (1) {
        p = (Proj_0049b720*)(offset + *(int*)(0x141f7 + g_game));
        type = p->type;
        s = *(short*)(((char*)p) + 0xa);

        if (p->counter != 0) {
            ec = type->field_ec;
            if (*(int*)(g_game + 0x38a47) < ((unsigned int)ec + p->field_42))
                goto Next;

            if (!(5 <= ((unsigned short)ec) || (p->counter & 1))) goto skip1;
            idx = 0;
            arr = p->field_52;
            while (1) {
                unsigned char tmp11 = ((unsigned char)idx);
                tmp6 = (tmp11) * 0x1c;
                char* tmp28 = (char*)arr;
                tmp10 = ((int)tmp6) + ((char*)tmp28);
                if (((unsigned int)(*(int*)(tmp10 + 0x10) == (int)type)))
                    break;
                idx = idx + 1;
                if (((int)(idx >= 3))) break;
            }
            FUN_0043e240(arr, &p->pos, idx, p->field_62);
skip1:;

            p->counter--;
            p->field_42 += type->field_ec;

            q = 0;
            if (*((int*)(0x141f3 + g_game)) < 300) {
                q = (Proj_0049b720*)(*(int*)(g_game + 0x141f3) * 0x6b
                                     + *(int*)inl12());
                *(int*)(g_game + 0x141f3) = ((*(int*)(0x141f3 + g_game)) + 1);
                q->flags69 &= 0xfffd;
                q->field_4e = 0;
            }

            if (0 != q) {
                *((Proj_0049b720*)q) = *(p);
                q->field_42 = ((*(int*)((g_game + 0x38a47))));
                if (type->flags.bits.b11) FUN_0047f300(type->field_f4, &p->pos, 0);
                tmp8 = type->field_e6;
                if (tmp8 != 0) q->field_46 = (inl18() + type->field_e6); else q->field_46 = ((0x100000 + p->field_3e) / (unsigned int)p->field_3a
                                  + (*(int*)(0x38a47 + g_game)));
                tmp0 = (type->field_f2);
                if (0 != tmp0)
                        q->field_46 = q->field_46 + FUN_004b6c30(inl22(((WType_0049b720*)type)))
                                      - (type->field_f2 >> 1);
                q->counter = 0;
                if (type->field_ee != 0) {
                    unsigned int tmp5;
                    int a;
                    a = FUN_004b6c30(type->field_ee);
                    ang = (short)(inl6(p, type));
                    int tmp34 = (a) + ang;
                    int t;
                    t = FUN_004b7123((p->pitch), type->field_68);
                    p->vel.x = -FUN_004b70ef(a + ang, (((int)t)));
                    unsigned int tmp20;
                    tmp20 = -FUN_004b7123(tmp34, t);
                    tmp5 = tmp20;
                    p->vel.z = tmp5;
                }
            }

            short tmp31 = p->counter;
            if (0 == tmp31) {
                Proj_0049b720* sel = *(Proj_0049b720**)(g_game + 0x142f7);
                {
                        int tmp37 = ((Proj_0049b720*)p) == ((Proj_0049b720*)sel);
                        if (tmp37) {
                            *((int*)(0x1433f + g_game)) = sel->pos.x;
                            *(int*)(g_game + 0x14343) = sel->pos.y;
                            *(int*)(0x14347 + g_game) = sel->pos.z;
                            *(short*)(0x1434b + g_game) = p->type->field_fe;
                            *(Proj_0049b720**)(0x142f7 + g_game) = 0;
                        }
                        p->flags69 = p->flags69 | 2;
                }
            }
            goto Next;
        }

        // counter == 0: live behaviour
        if (type->flags.bits.b21)
            p->field_64 += 0x400;

        {
            fl = type->flags.raw;
            if (1 & ((((unsigned int)fl)) >> 0x14)) {
                tmp2 = ((int)((p->field_46) > *(int*)(g_game + 0x38a47)));
                if (tmp2 != 0) {
                    if (!inl20((0x10000 & fl), s)) {
                        int tmp16 = (int)p->vel.y, tmp26 = (*(int*)(g_game + 0x14263));
                        p->pitch = 0; p->vel.y = (((tmp16) - tmp26));
                    } else {
                        int e = p->field_3a;
                        flag = 0;
                        if ((unsigned int)e < (unsigned int)type->field_68) {
                            tmp1 = ((unsigned int)e);
                            p->field_3a = type->field_70 + ((unsigned int)tmp1);
                                if ((unsigned int)(p->field_3a) > (unsigned int)type->field_68) p->field_3a = type->field_68;
                        }
                        UF_0049b720 f = type->flags;
                        if (f.bits.b24) {
                            if (0x30 & p->flags69) flag = 1;
                        } else if (f.bits.b12) flag = 1;
                        if (((unsigned int)flag) != 0) {
                            Vec_0049b720* v = FUN_0049b3e0(p);
                            int tmp21 = FUN_0049b520((p), v);
                            if (tmp21 == 0)
                                FUN_00499eb0(p, 0);
                        }
                        p->vel.y = FUN_004b70ef(p->pitch, p->field_3a);
                        {
                            {
                                int t = FUN_004b7123(p->pitch, p->field_3a);
                                unsigned int tmp7;
                                p->vel.x = -FUN_004b70ef(p->heading, t);
                                tmp7 = -FUN_004b7123(p->heading, t);
                                p->vel.z = tmp7;
                            }
                        }
                    }
                } else if (inl15(fl)) FUN_00499eb0(p, 0); else {
                    p->vel.y -= (*(int*)(g_game + 0x14263));
                    if (type->flags.bits.b24) {
                        int tmp22;
                        tmp22 = !(p->flags69 & 0x30);
                        if (tmp22) {
                                    char* tmp9 = g_game;
                                    tmp23 = (*(int*)(0x38a47 + tmp9));
                                    p->field_46 = p->type->field_fc + tmp23;
                                    unsigned int e;
                                    int tmp33;
                                    tmp33 = (p->flags69);
                                    tmp19 = (((unsigned int)tmp33));
                                    e = tmp19;
                                    p->flags69 = inl29(e);
                                    if ((0x2000 & p->type->flags.raw) != 0) { goto skip0; }
                                    p->field_56 = 0; p->field_4e = 0;
skip0:;
                                }
                    }
                }
                goto ApplyVel;
            }

            if (1 & type->flags.raw) {
                tmp15 = g_game + 0x38a47;
                if (p->field_46 > (inl21((tmp15)))) {
                    p->pos.y = ((p->vel.y + p->pos.y));
                    p->pos.x += p->vel.x; p->pos.z = p->pos.z + p->vel.z; if (!type->flags.bits.b3) {
                    } else { int tmp32 = inl9(((Proj_0049b720*)p));
                    if (tmp32) { p->start.y += p->vel.y; int tmp30 = p->vel.z;
                    p->start.z += tmp30; inl7(p); } else if (*(unsigned int*)(0x38a47 + g_game) > p->field_42 + (type->field_f0)) p->flags69 = (p->flags69) | 1; }
                    goto Call090;
                }
                goto Select;
            }

            if (type->flags.bits.b1) {
                if (!(type->field_e6 != 0))
                    goto Drift;
                if (p->field_46 <= *((int*)((int*)(g_game + 0x38a47)))) {
                } else {
                    p->pos.y += p->vel.y;
                    p->pos.z += p->vel.z;
                    p->pos.x += p->vel.x;
                        goto Drift2;
                }
                if (type->flags.bits.b23) {
                    FUN_00499eb0(((Proj_0049b720*)p), 0);
                    goto TailOnly;
                }
                FUN_00472810(&p->pos, 9);
                goto Select;
            }

            if (type->flags.bits.b8)
                goto Drift;
            if (type->flags.bits.b5) {
                p->pos.x = p->vel.x + p->pos.x;
                p->pos.z += p->vel.z;
                tmp4 = (short*)((char*)p + 0x26);
                    p->pos.y = p->pos.y + p->vel.y; 
                    p->field_34 = (short)((p->field_34) + (*((short*)(0x1e + (char*)p)) << 8));
                    p->pitch += (*tmp4 << 8);
                goto Call090;
            }
            goto TailOnly;
        }

    ApplyVel:
        p->pos.y = p->vel.y + p->pos.y;
        p->pos.x = p->vel.x + p->pos.x;
        p->pos.z = p->pos.z + p->vel.z;
        goto Call090;

    Drift:
        p->pos.x += p->vel.x;
        p->pos.y = p->pos.y + inl5(((Proj_0049b720*)p));
        p->pos.z += p->vel.z;

    Drift2:
        p->pos.x += (*(int*)(g_game + 0x37ecc));
        p->pos.y = ((*(int*)(g_game + 0x37ed0))) + p->pos.y;
        p->pos.z = p->pos.z + (*(int*)(0x37ed4 + g_game));
        p->vel.y = p->vel.y - *(int*)(0x14263 + g_game);

    Call090:
        FUN_0049b090(((WType_0049b720*)type), p);
        goto TailOnly;

    Select:
        {
            Proj_0049b720* sel;
            sel = (*(Proj_0049b720**)(0x142f7 + g_game));
                if (p == ((Proj_0049b720*)sel)) { *(int*)(g_game + 0x1433f) = sel->pos.x; *(int*)(g_game + 0x14343) = sel->pos.y; *(int*)(g_game + 0x14347) = sel->pos.z; *((short*)((g_game + 0x1434b))) = p->type->field_fe; *(Proj_0049b720**)(g_game + 0x142f7) = 0; } 
                p->flags69 = p->flags69 | 2;
        }

    TailOnly:
        if ((p->flags69 & 2) == 0) {
            int gt;
            gt = *((int*)(g_game + 0x38a47));
            if ((type->flags.raw & 0x40000)) {
                if (p->field_46 > gt) {
                        if (inl0(p, gt)) {
                                                        FUN_00472810(&p->pos, 9);
                                                        p->field_4a = p->field_4a + type->field_fa;
                                                    }
                    }
            }
            {
                sl = *(unsigned char*)(0x1427f + g_game);
                if ((s) > (unsigned short)sl
                    && *(short*)(0xa + (char*)p) <= (unsigned short)sl) {
                    void* v = FUN_004815a0(&p->pos);
                    if (v != 0
                        && (*(unsigned char*)(g_game + 0x1427f)) > *(unsigned char*)((char*)v + 5) && *(int*)(0xd48 + *(int*)(0x391e9 + g_game)) == 0) FUN_00420a30(&p->pos, type->field_7c, 0, 1);
                }
            }
        }

    Next:
            offset += 0x6b;
            if ((--count) == 0) break;
        }
    }

    FUN_0049ae20();
}
