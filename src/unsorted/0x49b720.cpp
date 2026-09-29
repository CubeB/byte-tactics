// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL. 0x49b720 (1853 bytes) is the per-projectile update pass. This is a
// first structural transcription straight from the disassembly: the whole
// control flow and every field offset used are believed right, but register
// allocation, the frame layout (original has 5 local dwords and keeps the loop
// offset/count in two of them) and the exact helper-argument order for the
// heading/velocity trig calls are NOT verified. Expect a low score; the value
// here is the annotated map of the branches and fields for the next attempt.
//
// Layout facts gathered so far:
//   Projectile stride 0x6b; +0x0 type, +0x4 pos(Vec3), +0x10 start(Vec3),
//   +0x1c velocity(Vec3), +0x34 short, +0x36 heading short, +0x38 pitch short,
//   +0x3a int, +0x3e int, +0x42 int age, +0x46 int altitude, +0x4a int,
//   +0x4e ptr, +0x52 int* array (stride 0x1c entries with type at +0x10),
//   +0x56 ptr, +0x60 short counter, +0x62 short, +0x64 short, +0x69 flags word
//   (bit1 = dead/killed, bits 4/5 turret-select related).
//   Type: +0x68 int, +0x70 int, +0x7c ptr, +0xe6/e.c/e.e/f0/f2/f4/fa/fc/fe
//   shorts, +0x111 flags dword.
//   Projectile array at g_game+0x141f7 (base) with count at +0x141f3, capped
//   at 300. Other globals: +0x14263 int, +0x1427f byte seaLevel,
//   +0x142f7 selected projectile ptr, +0x1433f..+0x1434b saved selection,
//   +0x38a47 game time, +0x37ecc Vec3 drift, +0x391e9 net ptr (+0xd48).

#pragma pack(push, 1)

struct Vec_0049b720 {
    int x;
    int y;
    int z;
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
    unsigned int flags;                // +0x111
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
    int field_46;                      // +0x46
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

// FUNCTION: 0x49b720
void FUN_0049b720()
{
    int count = *(int*)(g_game + 0x141f3);
    if (count <= 0) {
        FUN_0049ae20();
        return;
    }

    int offset = 0;

    do {
        Proj_0049b720* p = (Proj_0049b720*)(*(int*)(g_game + 0x141f7) + offset);
        short s = *(short*)((char*)p + 0xa);
        WType_0049b720* type = p->type;

        if (p->counter != 0) {
            unsigned short ec = type->field_ec;
            if (*(int*)(g_game + 0x38a47) < (int)(p->field_42 + (unsigned int)ec))
                goto Next;

            if (ec >= 5 || (p->counter & 1)) {
                int* arr = p->field_52;
                unsigned char idx = 0;
                while (*(int*)((char*)arr + idx * 0x1c + 0x10) != (int)type) {
                    idx++;
                    if (idx >= 3)
                        break;
                }
                FUN_0043e240(arr, &p->pos, idx, p->field_62);
            }

            p->counter = p->counter - 1;
            p->field_42 = p->field_42 + type->field_ec;

            Proj_0049b720* q = 0;
            if (*(int*)(g_game + 0x141f3) < 300) {
                q = (Proj_0049b720*)(*(int*)(g_game + 0x141f7)
                                     + *(int*)(g_game + 0x141f3) * 0x6b);
                *(int*)(g_game + 0x141f3) = *(int*)(g_game + 0x141f3) + 1;
                q->flags69 = q->flags69 & 0xfffd;
                q->field_4e = 0;
            }

            if (q != 0) {
                int i;
                char* d = (char*)q;
                char* src = (char*)p;
                for (i = 0; i < 0x6b; i++)
                    d[i] = src[i];
                q->field_42 = *(int*)(g_game + 0x38a47);
                if ((type->flags >> 0xb) & 1)
                    FUN_0047f300(type->field_f4, &p->pos, 0);
                if (type->field_e6 != 0)
                    q->field_46 = *(int*)(g_game + 0x38a47) + type->field_e6;
                else
                    q->field_46 = (p->field_3e + 0x100000) / (unsigned int)p->field_3a
                                  + *(int*)(g_game + 0x38a47);
                if (type->field_f2 != 0)
                    q->field_46 = q->field_46 + FUN_004b6c30(type->field_f2)
                                  - (type->field_f2 >> 1);
                q->counter = 0;
                if (type->field_ee != 0) {
                    int a = FUN_004b6c30(type->field_ee);
                    short ang = (short)(p->heading - (type->field_ee >> 1));
                    int t = FUN_004b7123(p->pitch, type->field_68);
                    p->vel.x = -FUN_004b70ef(ang + a, t);
                    p->vel.z = -FUN_004b7123(ang + a, t);
                }
            }

            if (p->counter == 0)
                goto Select;

            goto Next;
        }

        // counter == 0: live behaviour
        if ((type->flags >> 0x15) & 1)
            p->field_64 = p->field_64 + 0x400;

        {
            unsigned int fl = type->flags;
            if (fl & 0x100000) {
                if (p->field_46 > *(int*)(g_game + 0x38a47)) {
                    if (!(fl & 0x10000)
                        || s < (short)(unsigned char)*(g_game + 0x1427f)) {
                        int e = p->field_3a;
                        if (e < type->field_68) {
                            p->field_3a = e + type->field_70;
                            if (p->field_3a > type->field_68)
                                p->field_3a = type->field_68;
                        }
                        int flag = 0;
                        unsigned int f = type->flags;
                        if ((f >> 0x18) & 1) {
                            if (p->flags69 & 0x30)
                                flag = 1;
                        } else if ((f >> 0xc) & 1) {
                            flag = 1;
                        }
                        if (flag) {
                            Vec_0049b720* v = FUN_0049b3e0(p);
                            if (FUN_0049b520(p, v) == 0)
                                FUN_00499eb0(p, 0);
                        }
                        p->vel.y = FUN_004b70ef(p->pitch, p->field_3a);
                        {
                            int t = FUN_004b7123(p->pitch, p->field_3a);
                            p->vel.x = -FUN_004b70ef(p->heading, t);
                            p->vel.z = -FUN_004b7123(p->heading, t);
                        }
                    } else {
                        p->vel.y = p->vel.y - *(int*)(g_game + 0x14263);
                        p->pitch = 0;
                    }
                } else if (fl & 0x800000) {
                    FUN_00499eb0(p, 0);
                } else {
                    p->vel.y = p->vel.y - *(int*)(g_game + 0x14263);
                    if ((type->flags >> 0x18) & 1) {
                        if ((p->flags69 & 0x30) == 0) {
                            unsigned int e;
                            p->field_46 = *(int*)(g_game + 0x38a47) + type->field_fc;
                            e = type->flags >> 0x18;
                            p->flags69 = (unsigned short)(((((e & 0xfff0) + 0x10)
                                                            ^ (e & 0xff)) & 0x30) ^ e);
                            if (!(type->flags & 0x2000)) {
                                p->field_56 = 0;
                                p->field_4e = 0;
                            }
                        }
                    }
                }
                goto ApplyVel;
            }

            if (fl & 1) {
                if (p->field_46 > *(int*)(g_game + 0x38a47)) {
                    p->pos.x += p->vel.x;
                    p->pos.y += p->vel.y;
                    p->pos.z += p->vel.z;
                    if ((type->flags >> 3) & 1) {
                        if (p->flags69 & 1) {
                            p->start.x += p->vel.x;
                            p->start.y += p->vel.y;
                            p->start.z += p->vel.z;
                        } else if (p->field_42 + type->field_f0
                                   < (unsigned int)*(int*)(g_game + 0x38a47)) {
                            p->flags69 = p->flags69 | 1;
                        }
                    }
                    goto Call090;
                }
                goto Select;
            }

            if ((fl >> 1) & 1) {
                if (type->field_e6 == 0)
                    goto Drift;
                if (p->field_46 > *(int*)(g_game + 0x38a47)) {
                    p->pos.x += p->vel.x;
                    p->pos.y += p->vel.y;
                    p->pos.z += p->vel.z;
                    goto Drift2;
                }
                if (fl & 0x800000) {
                    FUN_00499eb0(p, 0);
                    goto TailOnly;
                }
                FUN_00472810(&p->pos, 9);
                goto Select;
            }

            if ((fl >> 8) & 1)
                goto Drift;
            if ((fl >> 5) & 1) {
                p->pos.x += p->vel.x;
                p->pos.y += p->vel.y;
                p->pos.z += p->vel.z;
                p->field_34 = (short)(p->field_34
                                      + (*(short*)((char*)p + 0x1e) << 8));
                p->pitch = (short)(p->pitch
                                   + (*(short*)((char*)p + 0x26) << 8));
                goto Call090;
            }
            goto TailOnly;
        }

    ApplyVel:
        p->pos.x += p->vel.x;
        p->pos.y += p->vel.y;
        p->pos.z += p->vel.z;
        goto Call090;

    Drift:
        p->pos.x += p->vel.x;
        p->pos.y += p->vel.y;
        p->pos.z += p->vel.z;

    Drift2:
        p->pos.x += *(int*)(g_game + 0x37ecc);
        p->pos.y += *(int*)(g_game + 0x37ed0);
        p->pos.z += *(int*)(g_game + 0x37ed4);
        p->vel.y = p->vel.y - *(int*)(g_game + 0x14263);

    Call090:
        FUN_0049b090(type, p);
        goto TailOnly;

    Select:
        {
            Proj_0049b720* sel = *(Proj_0049b720**)(g_game + 0x142f7);
            if (p == sel) {
                *(int*)(g_game + 0x1433f) = sel->pos.x;
                *(int*)(g_game + 0x14343) = sel->pos.y;
                *(int*)(g_game + 0x14347) = sel->pos.z;
                *(short*)(g_game + 0x1434b) = *(short*)((char*)type + 0xfe);
                *(Proj_0049b720**)(g_game + 0x142f7) = 0;
            }
            p->flags69 = p->flags69 | 2;
        }

    TailOnly:
        if (!(p->flags69 & 2)) {
            int gt = *(int*)(g_game + 0x38a47);
            if ((type->flags & 0x40000)
                && p->field_46 > gt
                && p->field_4a < (unsigned int)gt) {
                FUN_00472810(&p->pos, 9);
                p->field_4a = p->field_4a + type->field_fa;
            }
            {
                unsigned char sl = *(unsigned char*)(g_game + 0x1427f);
                if (s > sl && *(short*)((char*)p + 0xa) <= sl) {
                    void* v = FUN_004815a0(&p->pos);
                    if (v != 0
                        && *(unsigned char*)((char*)v + 5) < sl
                        && *(int*)(*(int*)(g_game + 0x391e9) + 0xd48) == 0)
                        FUN_00420a30(&p->pos, type->field_7c, 0, 1);
                }
            }
        }

    Next:
        offset += 0x6b;
        count--;
    } while (count != 0);

    FUN_0049ae20();
}
