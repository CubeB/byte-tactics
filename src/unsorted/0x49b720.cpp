// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// PARTIAL 42.8%. Size 1880 vs original 1853 bytes (27 bytes long).
// Recovered from a permuter run whose best_ratio.cpp was never copied back; that
// single file moved 39.1% to 42.8% and is now the body here. Everything below
// records what was measured AFTER that recovery, so these numbers are against
// 42.79% and not against the 33.0% the older notes were written at.
// Key fixes already in the file: the Select block is emitted twice (the
// counter!=0 path ends at Next, the live path at TailOnly) via goto labels, the
// type flags tests use a 1-bit bitfield union so MSVC emits the original's
// mov/shr/test sequence instead of folding (x>>N)&1 into test reg,imm, and the
// global accesses are routed through static inline helpers.
// Residuals, biggest first:
//  1. Prologue. The original loads g_game into eax BEFORE `sub esp, 0x14` and
//     then re-reads it into ecx after the pushes; ours loads once into ecx and
//     CSEs the second use. The original also homes count to [esp+0x18] AFTER
//     the three pushes, ours spills it to [esp+0xc] before the branch. Neither
//     spelling tried moves it (see group.py P1..P7 below).
//  2. `s` is read by the original as `mov cx, word ptr [ebp+esi+0xa]` BEFORE
//     `add ebp, esi`, i.e. from base+offset rather than from p, then sign
//     extended into edx. Ours reads `movsx ebx, word ptr [ebp+0xa]` after the
//     add. Reading s before p scores 41.70% (-1.09), so the two-register SIB
//     form is not reachable by spelling s from the same base+offset expression.
//  3. The recoil block signs cx with `sub cx, ax` in 16 bits and adds it to a
//     32-bit `a` with `add edi, ecx`. Ours sign extends ang to eax and rebuilds
//     the sum with `lea edi,[eax+ebx]`. Casting a, t or the sum to short all
//     score between -0.18 and -0.36; routing the sum through a short local and
//     reusing it for both calls (H7) is -2.36. The block is byte-identical with
//     a short t or with plain source (H5/H6, both +0.00), so the fix is not in
//     this block's widths.
//  4. The empty `if (!type->flags.bits.b3) { } else { ... }` in the b0 path
//     IS load-bearing: making it a positive test scores 40.47% (-2.32).
// Measured and rejected (each applied alone onto this body):
//   * a separate `char* qbase` local for the 0x141f7 array, with p and s both
//     formed from qbase+offset: 27.32% (-15.47). Also -15.47 as an int qbase.
//   * `p` formed as `(Proj*)((char*)(*(int*)(0x141f7+g_game)) + offset)`: +0.00,
//     inert, kept because it reads better.
//   * `count` read through a static inline helper: +0.00, inert.
//   * the `count > 0` guard written `0 < count`: +0.00, inert.
//   * `s` declared `short` instead of `int`: -0.18.
//   * `offset` declared with its initialiser: -0.18.
// Harness: build/scratch/0x49b720/probe.py is a compile-only probe that reuses
// check.py's comparison, ~0.5 s per variant instead of ~60 s. A `d` of 0 in its
// output means "same as this file", NOT "matches the original".

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


// FUNCTION: 0x49b720
void __cdecl FUN_0049b720()
{
    unsigned int tmp19;
    int tmp6;
    int tmp23;
    short* tmp4;
    char* tmp13;
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
    int* tmp18 = (int*)(g_game + 0x141f3);
    int offset;
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
                idx++;
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
                    int t;
                    t = FUN_004b7123((p->pitch), type->field_68);
                    p->vel.x = -FUN_004b70ef(a + ang, (((int)t)));
                    int tmp34 = (a) + ang;
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
                                    tmp23 = (*(int*)(0x38a47 + g_game));
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
                    if (tmp32) { p->start.y += p->vel.y; int tmp30;
                    tmp30 = p->vel.z;
                    p->start.z += tmp30; inl7(p); } else if (*(unsigned int*)(0x38a47 + g_game) > p->field_42 + (type->field_f0)) p->flags69 = (p->flags69) | 1; }
                    goto Call090;
                }
                goto Select;
            }

            if (type->flags.bits.b1) {
                if (!(type->field_e6 != 0))
                    goto Drift;
                if (p->field_46 > *((int*)((int*)(g_game + 0x38a47)))) {
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
                        if (p->field_4a < (unsigned int)gt) {
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
                        && (*(unsigned char*)(g_game + 0x1427f)) > *(unsigned char*)((char*)v + 5)
                        && *(int*)(0xd48 + *(int*)(0x391e9 + g_game)) == 0)
                        FUN_00420a30(&p->pos, type->field_7c, 0, 1);
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
