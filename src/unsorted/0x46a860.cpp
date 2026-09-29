// Decompiled by longcat-2.5-preview-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// Still differs (24.3%): the frame is 0x1a0 vs the original 0x23c. The original
// keeps a 60-byte snapshot struct at frame+0x24 (not a char[]; fields at
// +0x00 FUN_00439df0 result, +0x04 g_game+0x2cba ushort, +0x06/+0x0a..+0x15
// three weapon ids, +0x16/+0x1a/+0x1e/+0x22 four floats from unit+0xd0/0xcc/
// 0xe8/0xe4, +0x26 result+0xa8, +0x28 result+0x108, +0x2a g_game+0x2cbc), and
// two 100-byte buffers at frame+0x174 and frame+0x1d8 fill the frame out.
// The sprintf buffer is frame+0x74 (reused), the +0x12c declarations here let
// the allocator pick different slots. Register roles also differ from the
// first loop on: original holds param_1 in edi, the FUN_004b7f30 bitmap in esi,
// dy in ebp and the running right-edge in ebx; ours holds param_1 in esi and
// the bitmap in edi. The blit offset convention is FUN_004b7f90(dst,bmp,
// [bmp+4]+running, [bmp+6]+dy) (running is the horizontal edge, not y), fixed
// here. The main per-unit HUD walk 0x46aba3..0x46b8e9 is still an approximation.
#include <string.h>
#include <stdio.h>

extern char* g_game;

int FUN_004b6710();
int __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
void __stdcall FUN_004b7f90(void* dst, void* bmp, int x, int y);
void __stdcall FUN_004c13a0(int param_1, int param_2);
int FUN_004c13f0();
void __stdcall FUN_004c1420(int param_1);
int FUN_004c1450();
int __stdcall FUN_004c1480(void* font, unsigned char* text);
void __stdcall FUN_004c14f0(void* dst, unsigned char* text, int x, int y, int maxWidth);
char* __stdcall FUN_004c5740(char* key);
int __stdcall FUN_00439d20(void* owner);
int __stdcall FUN_00439dd0(int param_1);
int __stdcall FUN_00439df0(void* obj);
int __stdcall FUN_00465ac0(void* map, void* u);
void __stdcall FUN_00467c00(void* surf, void* player, void* rect, int dy);
unsigned short __stdcall FUN_00488b10(const char* name);
int __stdcall FUN_004bf6f0(void* surface, void* rect, int color);

class Class_00435100 {
public:
    int FUN_00435100();
};

// FUNCTION: 0x46a860
void __stdcall FUN_0046a860(void* param_1)
{
    int iVar5 = *(int*)(g_game + 0x37e23) - *(int*)(g_game + 0x147a7);
    char buf[60];
    memset(buf, 0, 60);

    unsigned short flags = *(unsigned short*)(g_game + 0x3923b);
    if ((flags & 1) && (flags & 2)) {
        int y = 0x81;
        unsigned char player = *(unsigned char*)(g_game + 0x2a43);
        char* table = *(char**)(g_game + player * 331 + 0x1b8a);
        int idx = *(unsigned char*)(table + 0x95);
        do {
            int dy = FUN_004b6710() - 0x20;
            unsigned short* ptr = *(unsigned short**)(g_game + idx * 4 + 0x14833);
            int bmp = FUN_004b7f30(ptr, 0);
            FUN_004b7f90(param_1, (void*)bmp, (short)*(unsigned short*)(bmp + 4) + y, (short)*(unsigned short*)(bmp + 6) + dy);
            y += (short)*(unsigned short*)bmp;
        } while (y < *(int*)(g_game + 0x37e1f));

        FUN_004c1420(*(int*)(g_game + 0x391f9));
        FUN_004c13a0(0x53, FUN_004c13f0());
        int pfable = FUN_004c1450();
        int pfstate = *(int*)(g_game + 0x37e23) - pfable;
        int y2 = pfstate - 1;

        char buf2[0x12c];
        void* field_c = *(void**)(g_game + 0xc);
        sprintf(buf2, "PFSTATE %d, PFABLE %d\n", *(unsigned char*)((char*)field_c + 0xf0) & 1, *(int*)((char*)field_c + 0x9c));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x82, y2, -1);

        if (*(unsigned short*)(g_game + 0x2cba) != 0) {
            int idx = *(unsigned short*)(g_game + 0x2cba) & 0xffff;
            char* unit = *(char**)(g_game + 0x14357) + idx * 0x118;
            int v = *(int*)(unit + 0x110);
            sprintf(buf2, "MOVEORD: %d FIREORD: %d\n", (v >> 0x12) & 3, (v >> 0x14) & 3);
            FUN_004c14f0(param_1, (unsigned char*)buf2, 0x108, y2, -1);
        }

        sprintf(buf2, "DELTATIME: %d\n", *(int*)(g_game + 0x38a3b));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x190, y2, -1);

        sprintf(buf2, "GAMETIME: %d\n", *(int*)(g_game + 0x38a47));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x208, y2, -1);

        int y3 = pfstate - 0x11;
        sprintf(buf2, "X: %d  Y: %d\n", *(int*)(g_game + 0x1431f), *(int*)(g_game + 0x14323));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x82, y3, -1);

        sprintf(buf2, "UNITS %d\\%d\n", *(int*)(g_game + 0x14353), *(int*)(g_game + 0x14367));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x108, y3, -1);

        sprintf(buf2, "PACKETS: %d %d %d\n", *(int*)(g_game + 0x1cbe), *(int*)(g_game + 0x1e09), *(int*)(g_game + 0x1f54));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x190, y3, -1);

        int v = *(int*)(g_game + 0x14233) * (short)*(unsigned short*)(g_game + 0x2c90) + (short)*(unsigned short*)(g_game + 0x2c8e);
        unsigned char c = *(unsigned char*)(*(int*)(g_game + 0x14287) + v * 15 + 4);
        sprintf(buf2, "XYH: %d %d %d\n", (short)*(unsigned short*)(g_game + 0x2c8e), (short)*(unsigned short*)(g_game + 0x2c90), c);
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x208, y3, -1);
        return;
    }

    int local_60 = *(int*)(g_game + 0x581);
    unsigned short local_38 = *(unsigned short*)(g_game + 0x2cba);
    unsigned short local_5e = *(unsigned short*)(g_game + 0x2cbc);
    int local_68 = *(int*)(g_game + 0x37e94);
    int local_64 = *(int*)(g_game + 0x37e90);

    if (local_38 != 0) {
        int idx = local_38 & 0xffff;
        char* unit = *(char**)(g_game + 0x14357) + idx * 0x118;
        unsigned short local_3e = *(unsigned short*)(unit + 0x108);
        unsigned short local_40 = *(unsigned short*)(unit + 0xb8);
        char* local_34 = (char*)FUN_00439df0(unit);
        float local_4a = *(float*)(unit + 0xd0);
        float local_4e = *(float*)(unit + 0xcc);
        float local_52 = *(float*)(unit + 0xe8);
        float local_56 = *(float*)(unit + 0xe4);

        int* local_3e_int = (int*)(buf + 0xa);
        char* p = (char*)(unit + 0x1f);
        for (int i = 0; i < 3; i++) {
            int* weapon = (int*)(*(int*)(p - 0xf));
            if (*(unsigned short*)(weapon + 0xe4) <= 0x1e || !(*(unsigned char*)p & 2)) {
                local_3e_int[i] = -1;
            } else {
                local_3e_int[i] = *(unsigned short*)(p - 7);
            }
            p += 0x1c;
        }

        unsigned short local_5a = 0;
        if (*(unsigned char*)(unit + 0xff) == *(unsigned char*)(g_game + 0x2a43)) {
            int result = FUN_00439dd0((int)unit);
            if (result != 0) {
                local_5a = *(unsigned short*)(result + 0xa8);
                unsigned short local_5c = *(unsigned short*)(result + 0x108);
            }
        }
    }

    char* saved = (char*)(g_game + 0x37e60);
    if (memcmp(buf, saved, 60) == 0) {
        return;
    }
    memcpy(saved, buf, 60);

    char* local_28 = g_game + 0xdcb;
    FUN_004c13a0(0x53, FUN_004c13f0());

    int player = *(unsigned char*)(g_game + 0x2a43);
    char* local_24 = g_game + player * 331 + 0x1b63;
    char* ptr = *(char**)(g_game + player * 331 + 0x1b8a);
    int idx2 = *(unsigned char*)(ptr + 0x95);
    char* ebp = g_game + idx2 * 562 + 0x37f3d;
    int y = 0x81;
    do {
        int dy = FUN_004b6710() - 0x20;
        char* ptr2 = *(char**)(local_24 + 0x27);
        int idx3 = *(unsigned char*)(ptr2 + 0x95);
        unsigned short* ptr3 = *(unsigned short**)(g_game + idx3 * 4 + 0x14833);
        int bmp = FUN_004b7f30(ptr3, 0);
        FUN_004b7f90(param_1, (void*)bmp, (short)*(unsigned short*)(bmp + 4) + y, (short)*(unsigned short*)(bmp + 6) + dy);
        y += (short)*(unsigned short*)bmp;
    } while (y < *(int*)(g_game + 0x37e1f));

    if (local_60 == -1) {
        if (local_38 == 0) {
            if (local_5e != 0xffff) {
                int idx = local_5e & 0xffff;
                char* unit2 = (char*)(*(int*)(g_game + 0x1426f) + idx * 0x100);
                if ((*(unsigned char*)(unit2 + 0xff) & 4) == 0 || (*(unsigned char*)(g_game + 0x3923b) & 2) != 0) {
                    char buf3[16];
                    if (*(float*)(unit2 + 0xf0) != 0.0f) {
                        sprintf(buf3, " M:%d", (int)*(float*)(unit2 + 0xf0));
                    } else {
                        buf3[0] = 0;
                    }
                    char buf4[16];
                    if (*(float*)(unit2 + 0xec) != 0.0f) {
                        sprintf(buf4, " E:%d", (int)*(float*)(unit2 + 0xec));
                    } else {
                        buf4[0] = 0;
                    }
                    char* name = unit2;
                    if ((*(unsigned char*)(g_game + 0x3923b) >> 1 & 1) == 0) {
                        name = unit2 + 0x80;
                    }
                    char buf5[0x12c];
                    if ((*(unsigned char*)(unit2 + 0xff) & 2) == 0) {
                        char* s = FUN_004c5740(name);
                        sprintf(buf5, "%s %s%s", s, buf3, buf4);
                    } else {
                        char* s = FUN_004c5740(name);
                        strcpy(buf5, s);
                    }
                    FUN_004c13a0(0x53, FUN_004c13f0());
                    FUN_004c14f0(param_1, (unsigned char*)buf5, *(int*)(ebp + 0x1d2), *(int*)(ebp + 0x1d6) + iVar5, -1);
                }
            }
        } else {
            int idx = local_38 & 0xffff;
            char* unit = *(char**)(g_game + 0x14357) + idx * 0x118;
            if (*(unsigned short*)(unit + 0xa6) != 0) {
                char* local_220 = g_game + player * 331 + 0x1b63;
                int visible = FUN_00465ac0(local_220, unit);
                if (visible == 0) {
                    char* s = FUN_004c5740("Unidentified object");
                    char buf6[0x12c];
                    sprintf(buf6, "%s%s", s, (*(int*)(unit + 0x110) >> 9) & 1 ? "S: " : "R: ");
                    int w = FUN_004c1480((void*)*(int*)(ebp + 0x22e), (unsigned char*)buf6);
                    int yy = *(int*)(ebp + 0x142) - w / 2;
                    FUN_004c13a0(0x53, FUN_004c13f0());
                    FUN_004c14f0(param_1, (unsigned char*)buf6, yy, *(int*)(ebp + 0x146) + iVar5, -1);
                    return;
                }
                // ... more code
            }
        }
    } else {
        // strncpy path
        char buf7[0x10];
        strncpy(buf7, (char*)(*(int*)(*(int*)(g_game + 0x531) + 4) + local_60 * 0x15b + 2), 0x10);
        buf7[0xf] = 0;
        unsigned short type = FUN_00488b10(buf7);
        if (type != 0) {
            int base = *(int*)(g_game + 0x1439b);
            if (_strcmpi(buf7, "CORBUILD") != 0) {
                char buf8[0x12c];
                sprintf(buf8, "%s  M:%d E:%d", buf7, (int)*(float*)(type * 0x249 + base + 0x186), (int)*(float*)(type * 0x249 + base + 0x18a));
                FUN_004c14f0(param_1, (unsigned char*)buf8, *(int*)(ebp + 0x1d2), *(int*)(ebp + 0x1d6) + iVar5, -1);
                FUN_004c14f0(param_1, (unsigned char*)(type * 0x249 + base + 0x40), *(int*)(ebp + 0x1e2), *(int*)(ebp + 0x1e6) + iVar5, -1);
                return;
            }
        }
    }
}
