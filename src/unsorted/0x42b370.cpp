// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Class_0042b370::operator= for the 0x249-byte UnitType element. Hand-written:
// char arrays are copied with byte loops, plain ints/shorts/structs member by
// member, and the two flag words at +0x241/+0x245 bit by bit.
#pragma pack(push, 1)

// Flags word at +0x241: 2 + 2 + 27 one-bit fields + 1.
struct Flags241_0042b370 {
    unsigned int b00 : 2;
    unsigned int b02 : 2;
    unsigned int b04 : 1;
    unsigned int b05 : 1;
    unsigned int b06 : 1;
    unsigned int b07 : 1;
    unsigned int b08 : 1;
    unsigned int b09 : 1;
    unsigned int b10 : 1;
    unsigned int b11 : 1;
    unsigned int b12 : 1;
    unsigned int b13 : 1;
    unsigned int b14 : 1;
    unsigned int b15 : 1;
    unsigned int b16 : 1;
    unsigned int b17 : 1;
    unsigned int b18 : 1;
    unsigned int b19 : 1;
    unsigned int b20 : 1;
    unsigned int b21 : 1;
    unsigned int b22 : 1;
    unsigned int b23 : 1;
    unsigned int b24 : 1;
    unsigned int b25 : 1;
    unsigned int b26 : 1;
    unsigned int b27 : 1;
    unsigned int b28 : 1;
    unsigned int b29 : 1;
    unsigned int b30 : 1;
    unsigned int b31 : 1;
};

// Flags word at +0x245: 20 one-bit fields + 3.
struct Flags245_0042b370 {
    unsigned int b00 : 1;
    unsigned int b01 : 1;
    unsigned int b02 : 1;
    unsigned int b03 : 1;
    unsigned int b04 : 1;
    unsigned int b05 : 1;
    unsigned int b06 : 1;
    unsigned int b07 : 1;
    unsigned int b08 : 1;
    unsigned int b09 : 1;
    unsigned int b10 : 1;
    unsigned int b11 : 1;
    unsigned int b12 : 1;
    unsigned int b13 : 1;
    unsigned int b14 : 1;
    unsigned int b15 : 1;
    unsigned int b16 : 1;
    unsigned int b17 : 1;
    unsigned int b18 : 1;
    unsigned int b19 : 1;
    unsigned int b20 : 3;
};

struct Vec3i_0042b370 {
    int x, y, z;
};

class Class_0042b370 {
public:
    char f000[0x20];                    // +0x000
    char f020[0x20];                    // +0x020
    char f040[0x40];                    // +0x040
    char f080[0x20];                    // +0x080
    char f0a0[0x1e];                    // +0x0a0
    char f0be[0x40];                    // +0x0be
    char f0fe[0x40];                    // +0x0fe
    int f13e, f142, f146, f14a;         // +0x13e
    int f14e, f152, f156, f15a;
    Vec3i_0042b370 f15e, f16a, f176;    // +0x15e
    int f182, f186, f18a, f18e;         // +0x182
    int f192, f196, f19a, f19e;
    int f1a2, f1a6, f1aa, f1ae;
    int f1b2, f1b6;
    short f1ba, f1bc, f1be, f1c0;       // +0x1ba
    int f1c2, f1c6, f1ca, f1ce;         // +0x1c2
    int f1d2, f1d6, f1da, f1de;
    int f1e2, f1e6, f1ea;
    int f1ee[3];                        // +0x1ee
    int f1fa;                           // +0x1fa
    short f1fe, f200, f202, f204;       // +0x1fe
    short f206, f208, f20a, f20c;
    short f20e, f210, f212, f214;
    short f216, f218, f21a, f21c;
    short f21e;
    int f220, f224;                     // +0x220
    char f228, f229, f22a, f22b;        // +0x228
    char f22c, f22d, f22e, f22f;
    char f230;
    int f231[3];                        // +0x231
    int f23d;                           // +0x23d
    Flags241_0042b370 f241;             // +0x241
    Flags245_0042b370 f245;             // +0x245

    Class_0042b370& operator=(const Class_0042b370& src);
};
#pragma pack(pop)

// FUNCTION: 0x42b370 ??4Class_0042b370@@QAEAAV0@ABV0@@Z
Class_0042b370& Class_0042b370::operator=(const Class_0042b370& src)
{
    unsigned int i;

    for (i = 0; i < 0x20; i++) f000[i] = src.f000[i];
    for (i = 0; i < 0x20; i++) f020[i] = src.f020[i];
    for (i = 0; i < 0x40; i++) f040[i] = src.f040[i];
    for (i = 0; i < 0x20; i++) f080[i] = src.f080[i];
    for (i = 0; i < 0x1e; i++) f0a0[i] = src.f0a0[i];
    for (i = 0; i < 0x40; i++) f0be[i] = src.f0be[i];
    for (i = 0; i < 0x40; i++) f0fe[i] = src.f0fe[i];

    f13e = src.f13e; f142 = src.f142; f146 = src.f146; f14a = src.f14a;
    f14e = src.f14e; f152 = src.f152; f156 = src.f156; f15a = src.f15a;
    f15e = src.f15e; f16a = src.f16a; f176 = src.f176;
    f182 = src.f182; f186 = src.f186; f18a = src.f18a; f18e = src.f18e;
    f192 = src.f192; f196 = src.f196; f19a = src.f19a; f19e = src.f19e;
    f1a2 = src.f1a2; f1a6 = src.f1a6; f1aa = src.f1aa; f1ae = src.f1ae;
    f1b2 = src.f1b2; f1b6 = src.f1b6;
    f1ba = src.f1ba; f1bc = src.f1bc; f1be = src.f1be; f1c0 = src.f1c0;
    f1c2 = src.f1c2; f1c6 = src.f1c6; f1ca = src.f1ca; f1ce = src.f1ce;
    f1d2 = src.f1d2; f1d6 = src.f1d6; f1da = src.f1da; f1de = src.f1de;
    f1e2 = src.f1e2; f1e6 = src.f1e6; f1ea = src.f1ea;
    for (i = 0; i < 3; i++) f1ee[i] = src.f1ee[i];
    f1fa = src.f1fa;
    f1fe = src.f1fe; f200 = src.f200; f202 = src.f202; f204 = src.f204;
    f206 = src.f206; f208 = src.f208; f20a = src.f20a; f20c = src.f20c;
    f20e = src.f20e; f210 = src.f210; f212 = src.f212; f214 = src.f214;
    f216 = src.f216; f218 = src.f218; f21a = src.f21a; f21c = src.f21c;
    f21e = src.f21e;
    f220 = src.f220; f224 = src.f224;
    f228 = src.f228; f229 = src.f229; f22a = src.f22a; f22b = src.f22b;
    f22c = src.f22c; f22d = src.f22d; f22e = src.f22e; f22f = src.f22f;
    f230 = src.f230;
    for (i = 0; i < 3; i++) f231[i] = src.f231[i];
    f23d = src.f23d;

    f241.b00 = src.f241.b00; f241.b02 = src.f241.b02;
    f241.b04 = src.f241.b04; f241.b05 = src.f241.b05;
    f241.b06 = src.f241.b06; f241.b07 = src.f241.b07;
    f241.b08 = src.f241.b08; f241.b09 = src.f241.b09;
    f241.b10 = src.f241.b10; f241.b11 = src.f241.b11;
    f241.b12 = src.f241.b12; f241.b13 = src.f241.b13;
    f241.b14 = src.f241.b14; f241.b15 = src.f241.b15;
    f241.b16 = src.f241.b16; f241.b17 = src.f241.b17;
    f241.b18 = src.f241.b18; f241.b19 = src.f241.b19;
    f241.b20 = src.f241.b20; f241.b21 = src.f241.b21;
    f241.b22 = src.f241.b22; f241.b23 = src.f241.b23;
    f241.b24 = src.f241.b24; f241.b25 = src.f241.b25;
    f241.b26 = src.f241.b26; f241.b27 = src.f241.b27;
    f241.b28 = src.f241.b28; f241.b29 = src.f241.b29;
    f241.b30 = src.f241.b30; f241.b31 = src.f241.b31;

    f245.b00 = src.f245.b00; f245.b01 = src.f245.b01;
    f245.b02 = src.f245.b02; f245.b03 = src.f245.b03;
    f245.b04 = src.f245.b04; f245.b05 = src.f245.b05;
    f245.b06 = src.f245.b06; f245.b07 = src.f245.b07;
    f245.b08 = src.f245.b08; f245.b09 = src.f245.b09;
    f245.b10 = src.f245.b10; f245.b11 = src.f245.b11;
    f245.b12 = src.f245.b12; f245.b13 = src.f245.b13;
    f245.b14 = src.f245.b14; f245.b15 = src.f245.b15;
    f245.b16 = src.f245.b16; f245.b17 = src.f245.b17;
    f245.b18 = src.f245.b18; f245.b19 = src.f245.b19;
    f245.b20 = src.f245.b20;

    return *this;
}
