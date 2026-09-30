// Decompiled by deepseek-v4.1-flash, finished by GPT-6. Names are provisional.
// Partial, 14.3%. Structural census from deepseek-v4.1-flash (best kept version):
//   - frame: original `sub esp,0x51c` (327 dword locals); ours `sub esp,0x318`.
//     The 0x204 (516 byte) deficit is missing arms plus missing large locals.
//   - original is 8944 bytes: code ends near 0x455f80, then the switch jump
//     table at 0x455f84 (43 entries, command 2..44).
//   - call instructions: original 106, ours 88. The 18 missing calls are all in
//     the three absent command arms: cmd 28 (0x454ec4, ~670B), cmd 33 (0x4559f5,
//     ~1094B, holds 2 of the original's 4 FUN_00451df0 sites) and cmd 39
//     (0x45525a, ~250B, sprintf + FUN_004c5740 + a 12x FUN_00463ca0 loop).
//     Per-callee deficits: FUN_0044ffd0 19 vs 13, FUN_00451df0 4 vs 1,
//     FUN_00453010 14 vs 13, FUN_00463ca0 3 vs 1.
//   - switch dispatch: `add eax,-2; cmp eax,0x2a; ja 0x455f50;
//     jmp dword ptr [eax*4 + 0x455f84]`, so table index i is command i+2.
//     Commands with no arm (jump to 0x455f50): 3, 4, 37, 43. Present here:
//     2, 5..36, 38..42, 44. Adding cmd 39 fixed the call census but dropped the
//     text ratio to 10.6%, because its new local moved every other frame slot.
//     Fix the frame and loop shape first, then add the arms.
//   - cmd 39 body: q = NetworkPlayer_453d40(*(int*)(bytes+1)); if (q)
//     sprintf(text, DAT_00506290, q+0x2b, FUN_004c5740(DAT_0050658c));
//     then `for (i=12;i;--i) FUN_00463ca0(text, 8, 0, q[0x146]);`.
//   - receive loop shape: top is 0x453d94 (packet = *(g_game+0x2a38);
//     r = FUN_004534e0(); [esp+0xf0] = r; if (r == 0) goto cond; ...);
//     cond at 0x455f50 (test r; jne top), so it is a do/while(r) with the test
//     at the bottom, not the current `while (receiving && FUN_004534e0())`.
//   - the zero-init loop reloads g_game every iteration in the original; our
//     source shape lets MSVC hoist g_game into a register instead.
// Frame, switch layout and register allocation still differ.
#include <string.h>

extern char* g_game;
extern char DAT_005119b8[];
extern unsigned char DAT_00512bc0[];
int FUN_004b6340();
void __stdcall FUN_004565a0(void*);
void __stdcall FUN_00463ca0(void*, int, int, unsigned char);
void __stdcall FUN_00451bc0(int, int, void*, int);
void __stdcall FUN_004861d0(unsigned char, void*);
void __stdcall FUN_0048ab70(void*);
void __stdcall FUN_00489ce0(void*);
void __stdcall FUN_004866d0(void*, int);
void __stdcall FUN_0049d270(void*, void*);
void __stdcall FUN_0049af90(void*, void*);
void __stdcall FUN_004233a0(int, int, int);
void __stdcall FUN_00423550(int, int, int);
int __stdcall FUN_00481550(int, int);
void __stdcall FUN_004244b0(int, int, int, void*);
void __stdcall FUN_0041b8d0(void*, void*);
void __stdcall FUN_0047f300(int, void*, int);
void __stdcall FUN_0047f0c0(int, int);
void __stdcall FUN_00464b30(unsigned char, unsigned char, float, int);
void __stdcall FUN_00464c60(unsigned char, unsigned char, float, int);
void __stdcall FUN_00485420(unsigned char, unsigned char);
void __stdcall FUN_00490df0(unsigned int, int);
void __stdcall FUN_00457540(void*, void*);
void __stdcall FUN_0048b920(void*, void*);
class Class_0048b090 {
  public:
    void FUN_0048b090(int, int);
};
class Class_004b0b00 {
  public:
    int FUN_004b0b00(int, void*, int, int, int, int, int, int);
};
class Class_0046d500 {
  public:
    void FUN_0046d500(void*, unsigned char);
};
int __stdcall FUN_00452570(int, int);
void __stdcall FUN_004523e0(int, int, int);
void __stdcall FUN_00451df0(int, void*, int);
void __stdcall FUN_00452bd0(void*);
void __stdcall FUN_00488570(void*, void*, void*);
void FUN_00450530();
extern int DAT_00506dbc;
class Class_004618a0 {
  public:
    void FUN_004618a0(int);
};
class Class_00461620 {
  public:
    void FUN_00461620(int, int, int);
};
extern Class_004618a0 DAT_00513000;
extern char DAT_00505dc4[];
void __stdcall FUN_0047f1a0(char*, int);
void __stdcall FUN_00452960(int, int, unsigned char, int);
void FUN_00446fb0();
int FUN_004534e0();
void FUN_00450980();
void FUN_00453c20();
int __stdcall FUN_0044ffd0(unsigned char);
unsigned char __stdcall FUN_0044fe40(int);
int __stdcall FUN_00450a10(int);
void __stdcall FUN_00453010(int, int);
void __stdcall FUN_00451090(char*, int*, int*, int*, int*);
void __stdcall FUN_004c9890(void*, char*, char*, int, int, int, int);
class Class_00456030 {
  public:
    int FUN_00456030();
};
class Class_00463c60 {
  public:
    void FUN_00463c60(int);
};
class Class_00463c40 {
  public:
    void FUN_00463c40();
};
class Class_00463be0 {
  public:
    char data[0x14b];
    Class_00463be0();
};

static inline unsigned char FindPlayer_453d40(int id) {
    if (id != -1) {
        for (unsigned char i = 0; i < 10; ++i) {
            char* p = g_game + 0x14b * i;
            int found = p[0x1bd6] ? *(int*)(p + 0x1b67) : -1;
            if (found == id)
                return i;
        }
    }
    return 10;
}
static inline unsigned char FindHost_453d40() {
    for (unsigned char i = 0; i < 10; ++i) {
        char* p = g_game + 0x14b * i;
        if (p[0x1bd6] && (*(unsigned char*)(*(char**)(p + 0x1b8a) + 0x97) & 1))
            return i;
    }
    return 10;
}
static inline char* ResolvePlayer_453d40(int id) {
    if (id != -1) {
        for (unsigned char i = 0; i < 10; ++i) {
            if (FUN_0044ffd0(i) == id) {
                unsigned char slot = FUN_0044fe40(id);
                return g_game + 0x14b * slot + 0x1b63;
            }
        }
    }
    return 0;
}
static inline int PlayerId_453d40(unsigned char i) {
    if (i != 10 && g_game[0x1bd6 + 0x14b * i])
        return *(int*)(g_game + 0x1b67 + 0x14b * i);
    return -1;
}

static inline unsigned char NetworkSlot_453d40(int id) {
    if (id != -1) {
        for (unsigned char i = 0; i < 10; ++i)
            if (FUN_0044ffd0(i) == id)
                return i;
    }
    return 10;
}
static inline char* NetworkPlayer_453d40(int id) {
    if (NetworkSlot_453d40(id) == 10)
        return 0;
    unsigned char i = NetworkSlot_453d40(id);
    return g_game + 0x1b63 + 0x14b * i;
}
static inline char* LogicalPlayer_453d40(int id) {
    if (FindPlayer_453d40(id) == 10)
        return 0;
    unsigned char i = FindPlayer_453d40(id);
    return g_game + 0x1b63 + 0x14b * i;
}

// FUNCTION: 0x453d40
int FUN_00453d40() {
    if (!(g_game[0x2a44] & 1))
        return 0;
    int messages = 0;
    int receiving = 1;
    int n = 10, off = 0;
    do {
        off += 0x14b;
        *(int*)(g_game + 0x1a28 + off) = 0;
    } while (--n);
    int* packet = *(int**)(g_game + 0x2a38);
    while (receiving && FUN_004534e0() != 0) {
        int sender = *(int*)(g_game + 0x4c9);
        unsigned char from = FindPlayer_453d40(sender);
        unsigned char to = FindPlayer_453d40(*(int*)(g_game + 0x4cd));
        char* recipient = g_game + 0x1b63 + 0x14b * to;
        ++messages;
        if (sender != 0) {
            char* player = g_game + 0x1b63 + 0x14b * from;
            unsigned char* bytes = (unsigned char*)packet;
            unsigned char cmd = bytes[0];
            int mode = *(int*)(g_game + 0x391f1);
            int mask = mode == 5 ? 2 : (mode == 6 ? 4 : 1);
            if (!(DAT_00512bc0[cmd * 4] & mask))
                continue;
            if (*(int*)player && (player[0x73] == 1 || player[0x73] == 2))
                continue;
            if (!*(int*)player || (player[0x73] != 1 && player[0x73] != 2 && player[0x73] != 3) ||
                player[0x146] == 10) {
                FUN_00453010(sender, 6);
                continue;
            }
            if (cmd < 2 && cmd > 0x2c) {
                FUN_00453010(sender, 6);
                continue;
            }
            if (!*(int*)recipient ||
                (recipient[0x73] != 1 && recipient[0x73] != 2 && recipient[0x73] != 3) ||
                recipient[0x146] == 10)
                continue;
            *(int*)(player + 0x1c) = FUN_004b6340();
            ++*(int*)(player + 0x10);
            switch (cmd) {
            case 2:
                FUN_004565a0(packet);
                break;
            case 5:
                if (*(int*)recipient && recipient[0x73] == 1)
                    FUN_00463ca0(bytes + 1, 8, 0, from);
                break;
            case 6: {
                unsigned char response = 7;
                int id = -1;
                for (int j = 0; j < 10; ++j) {
                    char* q = g_game + 0x1b63 + 0x14b * j;
                    if (*(int*)q && (q[0x73] == 1 || q[0x73] == 2)) {
                        id = *(int*)(q + 4);
                        break;
                    }
                }
                FUN_00451bc0(id, *(int*)(g_game + 0x4c9), &response, 1);
                break;
            }
            case 7:
                player[0x21] |= 1;
                break;
            case 8:
                receiving = 0;
                g_game[0x2a44] |= 4;
                break;
            case 9:
                FUN_004861d0(from, packet);
                break;
            case 10:
                FUN_0048ab70(packet);
                break;
            case 11:
                FUN_00489ce0(packet);
                break;
            case 12:
                FUN_004866d0(packet, 0);
                break;
            case 13:
                FUN_0049d270(player, packet);
                break;
            case 14:
                FUN_0049af90(player, packet);
                break;
            case 15: {
                unsigned int kind = bytes[1];
                unsigned int x = *(unsigned short*)(bytes + 2);
                unsigned int y = *(unsigned short*)(bytes + 4);
                if (kind == 0xfd)
                    FUN_00423550(x, y, 0);
                else if (kind == 0xfe)
                    FUN_004233a0(x, y, 1);
                else if (kind == 0xff)
                    FUN_00423550(x, y, 1);
                else {
                    char* feature = g_game + 0x2cf3 + kind * 0x115;
                    int tile = FUN_00481550(x, y);
                    FUN_004244b0(tile, x, y, feature);
                }
                break;
            }
            case 16: {
                unsigned short index = *(unsigned short*)(bytes + 1);
                char* unit = index ? *(char**)(g_game + 0x14357) + index * 0x118 : 0;
                if (*(unsigned int*)(unit + 0x110) & 0x10000000)
                    (*(Class_004b0b00**)(unit + 0x9a))
                        ->FUN_004b0b00(*(short*)(bytes + 3), 0, 0, bytes[5], *(int*)(bytes + 6),
                                       *(int*)(bytes + 10), *(int*)(bytes + 14),
                                       *(int*)(bytes + 18));
                break;
            }
            case 17: {
                unsigned short index = *(unsigned short*)(bytes + 1);
                char* unit = index ? *(char**)(g_game + 0x14357) + index * 0x118 : 0;
                if (*(unsigned int*)(unit + 0x110) & 0x10000000) {
                    ((Class_0048b090*)unit)->FUN_0048b090(bytes[3], 1);
                    ((Class_0048b090*)unit)->FUN_0048b090((unsigned char)~bytes[3], 0);
                }
                break;
            }
            case 18: {
                unsigned short first = *(unsigned short*)(bytes + 1);
                unsigned short second = *(unsigned short*)(bytes + 3);
                char* units = *(char**)(g_game + 0x14357);
                void* a = first ? units + first * 0x118 : 0;
                FUN_0041b8d0(second ? *(char**)(g_game + 0x14357) + second * 0x118 : 0, a);
                break;
            }
            case 19:
                if (!bytes[1])
                    FUN_0047f300(*(int*)(bytes + 2), bytes + 6, 0);
                else
                    FUN_0047f0c0(*(int*)(bytes + 2), 0);
                break;
            case 20: {
                unsigned short index = *(unsigned short*)(bytes + 1);
                char* unit = index ? *(char**)(g_game + 0x14357) + index * 0x118 : 0;
                if (unit && (*(unsigned int*)(unit + 0x110) & 0x10000000)) {
                    int id = *(int*)(bytes + 3);
                    char* q = 0;
                    if (FindPlayer_453d40(id) != 10) {
                        unsigned char slot = NetworkSlot_453d40(id);
                        q = g_game + 0x1b63 + 0x14b * slot;
                    }
                    if (*(int*)q && (q[0x73] == 1 || q[0x73] == 2))
                        FUN_00488570(unit, q, packet);
                }
                break;
            }
            case 21:
                if (g_game[0x38d75] & 4)
                    *(int*)(g_game + 0x29a4 + 4 * from) = 1;
                break;
            case 22: {
                unsigned char a = FindPlayer_453d40(*(int*)(bytes + 5));
                unsigned char b = FindPlayer_453d40(*(int*)(bytes + 9));
                if (a != 10 && b != 10) {
                    int kind = *(int*)(bytes + 1);
                    if (kind == 1)
                        FUN_00464b30(a, b, *(float*)(bytes + 13), 0);
                    else if (kind == 2)
                        FUN_00464c60(a, b, *(float*)(bytes + 13), 0);
                    else if (kind == 3)
                        FUN_00485420(a, b);
                }
                break;
            }
            case 23:
                if (*(unsigned char*)(*(char**)(g_game + 0x1b8a +
                                                0x14b * (unsigned char)g_game[0x2a42]) +
                                      0x97) &
                    1) {
                    int result = FUN_00452570(*(int*)(g_game + 0x4c9), (signed char)bytes[1]);
                    int id = -1;
                    for (int j = 0; j < 10; ++j) {
                        if (g_game[0x1bd6 + j * 0x14b] == 1) {
                            id = *(int*)(g_game + 0x1b67 + j * 0x14b);
                            break;
                        }
                    }
                    if (!result) {
                        FUN_004523e0(id, *(int*)(g_game + 0x4c9), (signed char)bytes[1]);
                    } else {
                        unsigned char response[2];
                        response[0] = 0x18;
                        response[1] = bytes[1];
                        FUN_00451bc0(id, *(int*)(g_game + 0x4c9), response, 2);
                        if (DAT_00506dbc)
                            DAT_00513000.FUN_004618a0(1);
                    }
                }
                break;
            case 24:
                (*(char**)(g_game + 0x1b8a + 0x14b * to))[0x96] = bytes[1];
                if (to == (unsigned char)g_game[0x2a42] && (g_game[0x2a44] & 1)) {
                    for (int j = 0; j < 10; ++j) {
                        char* q = g_game + 0x1b63 + 0x14b * j;
                        if (*(int*)q && (q[0x73] == 1 || q[0x73] == 2)) {
                            unsigned char response[0xba];
                            memcpy(response + 1, *(void**)(q + 0x27), 0xb9);
                            *(int*)(response + 0x91) = *(int*)(q + 4);
                            response[0] = 0x20;
                            FUN_00451df0(*(int*)(q + 4), response, 0xba);
                            FUN_00452bd0(q);
                        }
                    }
                    FUN_00450530();
                    DAT_00513000.FUN_004618a0(1);
                }
                g_game[0x2bee] |= 1;
                break;
            case 25:
                if (!bytes[1])
                    *(unsigned short*)(g_game + 0x38a51) =
                        (*(unsigned short*)(g_game + 0x38a51) & 0xfffe) | (bytes[2] & 1);
                else
                    FUN_00490df0(bytes[2], 0);
                break;
            case 26:
                if (*(void**)(g_game + 0x2a30) && *(int*)recipient && recipient[0x73] == 1)
                    (*(Class_0046d500**)(g_game + 0x2a30))->FUN_0046d500(packet, from);
                break;
            case 27: {
                char* q = NetworkPlayer_453d40(*(int*)(bytes + 1));
                if (q)
                    FUN_00453010(*(int*)(q + 4), bytes[5]);
                break;
            }
            case 29:
                if (DAT_00506dbc)
                    ((Class_00461620*)&DAT_00513000)
                        ->FUN_00461620(*(int*)(g_game + 0x4c9), *(int*)(bytes + 1),
                                       *(int*)(bytes + 5));
                break;
            case 30: {
                unsigned char response[5];
                response[0] = 0x1f;
                recipient[0x147] = bytes[1];
                *(int*)(response + 1) = *(int*)(recipient + 4);
                FUN_00451bc0(*(int*)(response + 1), *(int*)(player + 4), response, 5);
                break;
            }
            case 31: {
                unsigned char target = FindPlayer_453d40(*(int*)(bytes + 1));
                if (target != 10)
                    *(int*)(g_game + 0x29d0 + target * 4) = 1;
                break;
            }
            case 34: {
                char* q = LogicalPlayer_453d40(*(int*)(bytes + 1));
                if (q)
                    *(int*)(q + 0xc) = bytes[5];
                break;
            }
            case 35: {
                char* a = NetworkPlayer_453d40(*(int*)(bytes + 1));
                char* b = NetworkPlayer_453d40(*(int*)(bytes + 5));
                if (a && b) {
                    if (bytes[9])
                        FUN_0047f1a0(DAT_00505dc4, 0);
                    if (*(int*)b && (b[0x73] == 1 || b[0x73] == 2)) {
                        FUN_00452960(*(int*)(bytes + 1), *(int*)(bytes + 5), bytes[9],
                                     *(int*)(bytes + 10));
                        if (!(g_game[0x2a44] & 4))
                            g_game[0x2bee] |= 1;
                        else
                            FUN_00446fb0();
                    }
                    g_game[0x1c6b + 0x14b * (unsigned char)a[0x146] + (unsigned char)b[0x146]] =
                        bytes[9];
                }
                break;
            }
            case 36: {
                char* q = NetworkPlayer_453d40(*(int*)(bytes + 1));
                if (q)
                    q[0x13f] = bytes[5];
                if (!(g_game[0x2a44] & 4))
                    g_game[0x2bee] |= 1;
                break;
            }
            case 38:
                memcpy(g_game + 0x2c28, bytes + 1, 40);
                g_game[0x2bee] |= 1;
                break;
            case 40:
                FUN_00457540(packet, player);
                break;
            case 41:
                if (bytes[1]) {
                    recipient[0x11e + from] = 1;
                    if (bytes[2])
                        recipient[0x134 + from] = 1;
                }
                break;
            case 42:
                player[0x20] = bytes[1];
                break;
            case 44:
                FUN_0048b920(player, packet);
                break;
            case 32: {
                unsigned char target = FindPlayer_453d40(*(int*)(bytes + 0x91));
                if (target != 10) {
                    char* q = g_game + 0x1b63 + target * 0x14b;
                    if (*(int*)q && q[0x73] == 3) {
                        memcpy(*(void**)(q + 0x27), bytes + 1, 0xb9);
                        FUN_00450980();
                    }
                }
                break;
            }
            default:
                break; // Remaining command cases are not yet transcribed.
            }
            continue;
        }
        if (*(int*)recipient == 0 || recipient[0x73] != 1)
            continue;
        switch (packet[0]) {
        case 5:
            if (packet[1] == 1) {
                char* p = ResolvePlayer_453d40(packet[2]);
                if (p && *(int*)p && (p[0x73] == 1 || p[0x73] == 2 || p[0x73] == 3) &&
                    p[0x146] != 10) {
                    if (!(g_game[0x2a44] & 4) &&
                        (*(unsigned char*)(*(char**)(p + 0x27) + 0x97) & 1) && p[0x73] == 3) {
                        FUN_00453010(*(int*)(p + 4), 1);
                        FUN_00453010(
                            *(int*)(g_game + 0x1b67 + 0x14b * (unsigned char)g_game[0x2a42]), 10);
                        ((Class_00463c60*)p)->FUN_00463c60(0);
                        p = g_game + 0x1b63 + 0x14b * (unsigned char)g_game[0x2a42];
                    } else {
                        FUN_00453010(*(int*)(p + 4), 1);
                    }
                    ((Class_00463c60*)p)->FUN_00463c60(0);
                    g_game[0x2bee] |= 1;
                    if (*(unsigned char*)(*(char**)(g_game + 0x1b8a +
                                                    0x14b * (unsigned char)g_game[0x2a42]) +
                                          0x97) &
                        1) {
                        char name[32];
                        int d, c, b, a;
                        FUN_00451090(name, &d, &c, &b, &a);
                        if (*(unsigned char*)(*(char**)(g_game + 0x1b8a +
                                                        0x14b * (unsigned char)g_game[0x2a42]) +
                                              0x9b) &
                            0x10)
                            *(int*)(g_game + 0x475) |= 0x20;
                        FUN_004c9890(g_game + 0x14, name, DAT_005119b8, d, c, b, a);
                    }
                }
            }
            break;
        case 3:
            if (FUN_00450a10(packet[2])) {
                ResolvePlayer_453d40(packet[2]);
                unsigned char target = FindPlayer_453d40(packet[2]);
                unsigned char host = FindHost_453d40();
                if (((Class_00456030*)(g_game + 0x1b63 + 0x14b * host))->FUN_00456030()) {
                    char* payload = (char*)packet[4];
                    char* info = *(char**)(g_game + 0x1b8a + 0x14b * (unsigned char)g_game[0x2a42]);
                    if (*(unsigned short*)(info + 0x9b) & 0x8000) {
                        FUN_00453010(PlayerId_453d40(target), 3);
                    } else if (packet[5] != 0x15) {
                        FUN_00453010(PlayerId_453d40(target), 8);
                    } else if (*(short*)(payload + 0x11) != 0 ||
                               *(short*)(payload + 0x13) != 0x50) {
                        FUN_00453010(PlayerId_453d40(target), 8);
                    } else if ((info[0x9d] & 1) &&
                               (!payload || _strcmpi(g_game + 0x2be3, payload))) {
                        FUN_00453010(PlayerId_453d40(target), 4);
                    }
                }
            }
            break;
        case 0x102:
            if (packet[1] == 1) {
                Class_00463be0 temporary;
                char* p = ResolvePlayer_453d40(packet[2]);
                if (p) {
                    unsigned char target = FindPlayer_453d40(packet[2]);
                    if (target != 10) {
                        char* payload = (char*)packet[3];
                        memcpy(*(void**)(g_game + 0x1b8a + 0x14b * target), payload, 0xb9);
                        unsigned char host = FindHost_453d40();
                        if (host == (unsigned char)g_game[0x2a42] &&
                            !(*(unsigned char*)(*(char**)(g_game + 0x1b8a +
                                                          0x14b * (unsigned char)g_game[0x2a42]) +
                                                0x9b) &
                              0x80) &&
                            (payload[0x9b] & 0x40))
                            FUN_00453010(*(int*)(p + 4), 9);
                    }
                }
                ((Class_00463c40*)&temporary)->FUN_00463c40();
            }
            break;
        case 0x103:
            if (packet[1] == 1) {
                char* p = ResolvePlayer_453d40(packet[2]);
                if (p) {
                    strncpy(p + 0x2b, (char*)packet[6], 0x1e);
                    strncpy(p + 0x49, (char*)packet[5], 0x1e);
                }
            }
            break;
        case 0x104:
            if (FindHost_453d40() != (unsigned char)g_game[0x2a42])
                memcpy(g_game + 0x471, packet + 1, 0x50);
            break;
        }
    }
    FUN_00450980();
    FUN_00453c20();
    return messages;
}
