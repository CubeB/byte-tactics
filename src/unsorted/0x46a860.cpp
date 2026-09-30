// Decompiled by deepseek-v4.1, edited by deepseek-v4.1. Names are provisional.
// Started by longcat-2.5-preview-free, continued by deepseek-v4.1-flash and GPT-6.
// Partial: 80.3%, 4217 bytes versus 4247. The frame is the original 0x23c.
// deepseek-v4.1-flash retry stopped on the shared-board WATCHDOG; 80.3% is the
// best variant found and is unchanged by this round (4 experiments, all worse).
// What this round changed (each one verified by a check.py run):
// - PFSTATE/PFABLE is one expression `*(int*)(g_game+0x37e23) - pfable - 1` and
//   y3 = pfstate - 0x10, which reproduces the original sub/dec pair, +5.8.
// - the local_38 arm is `if (local_38 != 0) {selected} else if (local_5e != 0xffff)
//   {feature}`, so the selected path is the fall-through like the original, +5.9.
// - the visible-unit test is `if (visible != 0) {main} else {unidentified}`,
//   putting the unidentified-object arm at the end like the original, +1.7.
// - the unit flag pair is a materialised bool, `((unitFlags & 0x20000) |
//   ((unitFlags >> 1) & 0x20000)) >> 0x11`, computed before the FUN_00435100
//   call, giving the original shift/or/shift sequence, +2.2.
// - the " M:%d" and " E:%d" buffers are 16-byte locals inside the feature
//   branch, so they pack into the inlined rect slot (M lands at [esp+0x74],
//   the E buffer at [esp+0x18] versus the original [esp+0x14]).
// Still differs, from the diff hunks:
// - param_1 homes to esi (original: edi) and the first icon loop keeps its
//   bitmap in edi (original: esi), a straight swap of the two registers.
// - the selected-unit pointer takes [esp+0x14] in ours, [esp+0x24] in the
//   original, and the inlined DrawBar rect copy sits at [esp+0x18] versus
//   [esp+0x14], so that whole region is one slot off.
// - the PFSTATE block loads [g_game+0xc] before [g_game+0x37e23] and keeps
//   g_game in edx; the original loads [0x37e23] into esi first, then [0xc]
//   into eax, and keeps pfable in ecx.
// - `(*(int*)(unit+0x110) >> 9) & 1` folds to `test ch,2`; the original
//   shifts into dl and tests dl,1.
// Tried and did not help: declaring the buffers at the top of the function,
// reordering the PFSTATE statements, and `int` versus `unsigned int` for the
// MOVEORD shift pair (the latter did fix sar/shr in that one spot).
#include <windows.h>
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

#pragma pack(push, 1)
struct Snapshot_0046a860 {
    int orderName;
    unsigned short selected, health, build;
    int weapons[3];
    float values[4];
    unsigned short targetType, targetHealth, feature;
    int button, width, height, reserved;
};
#pragma pack(pop)

struct Rect_0046a860 {
    int left, top, right, bottom;
};
static void DrawBar_0046a860(void* surface, Rect_0046a860* bounds, int value, int maximum, int dy,
                             unsigned char* palette) {
    Rect_0046a860 rect = *bounds;
    rect.top += dy;
    rect.bottom += dy;
    if (value < 0)
        value = 0;
    if (value > maximum)
        value = maximum;
    rect.right = rect.left + (bounds->right - bounds->left) * value / maximum;
    FUN_004bf6f0(surface, &rect, palette[10]);
    if (rect.right != bounds->right) {
        rect.left = rect.right + 1;
        rect.right = bounds->right;
        FUN_004bf6f0(surface, &rect, palette[4]);
    }
}
static float Positive_0046a860(float value) { return value > 0.0f ? value : 0.0f; }

// State after the deepseek-v4.1-flash retry: 80.3%, 4217 versus 4247 bytes.
// The frame is 0x23c in both and the local slots already match the original
// (/Fa listing: iVar5 0x10, snapshot 0x34, text 0x84, amount 0x184, kills
// 0x1e8), so this is no longer a frame-layout problem. What is left is a long
// tail of register and scheduling choices, from the check.py diff:
// - Prologue homes param_1 in edi (mov edi,[esp+0x250]); ours picks esi, and
//   the first do/while icon loop keeps the bitmap in edi where the original
//   uses esi. A straight swap of the two registers.
// - After FUN_004c1450 the original loads [g_game+0x37e23] into esi first, then
//   [g_game+0xc] into eax, and keeps pfable in ecx; ours loads 0xc first, keeps
//   g_game in edx and field_c in ecx. Reordering the source so pfstate is
//   computed before field_c drops the whole function to 73.3%, so the current
//   (field_c first) order is the better one.
// - In the MOVEORD block ours folds `unit + 0x110` into one mov with an
//   addressing mode; the original materialises the unit pointer with a
//   separate lea first.
// - `int y3 = pfstate - 0x10` emits `add esi,-0x10` here, `sub esi,0x10` in the
//   original.
// - The four snapshot.values copies become integer movs here, but the original
//   uses four fld/fstp pairs.
// - `(*(int*)(unit+0x110) >> 9) & 1` folds to `test ah,2`; the original does
//   `shr edx,9` then `test dl,1`.
// - The type*0x249+base table pointer and the two sprintf call setups pick
//   different registers; one early return duplicates the full epilogue in the
//   original (about 30 bytes, our size gap) where ours jumps to the shared one.
// Tried this round and reverted (all scored lower or neutral): pfstate before
// field_c (73.3), splitting the strncpy base pointer into a named local (80.3,
// no change), moving the strncpy name into a dedicated char[17] (75.3, 0x240
// frame), and hoisting the type table pointer before _strcmpi (79.9).

// FUNCTION: 0x46a860
void __stdcall FUN_0046a860(void* param_1) {
    int iVar5 = *(int*)(g_game + 0x37e23) - *(int*)(g_game + 0x147a7);
    Snapshot_0046a860 snapshot;
    memset(&snapshot, 0, sizeof(snapshot));
    char text[256];
    char amount[100];
    char killsText[100];

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
            FUN_004b7f90(param_1, (void*)bmp, (short)*(unsigned short*)(bmp + 4) + y,
                         (short)*(unsigned short*)(bmp + 6) + dy);
            y += *(unsigned short*)bmp;
        } while (y < *(int*)(g_game + 0x37e1f));

        FUN_004c1420(*(int*)(g_game + 0x391f9));
        FUN_004c13a0(0x53, FUN_004c13f0());
        int pfable = FUN_004c1450();
        void* field_c = *(void**)(g_game + 0xc);
        int pfstate = *(int*)(g_game + 0x37e23) - pfable - 1;

        char* buf2 = text;
        sprintf(buf2, "PFSTATE %d, PFABLE %d\n", *(unsigned char*)((char*)field_c + 0xf0) & 1,
                *(int*)((char*)field_c + 0x9c));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x82, pfstate, -1);

        if (*(unsigned short*)(g_game + 0x2cba) != 0) {
            int idx = *(unsigned short*)(g_game + 0x2cba) & 0xffff;
            char* unit = *(char**)(g_game + 0x14357) + idx * 0x118;
            unsigned int v = *(int*)(unit + 0x110);
            sprintf(buf2, "MOVEORD: %d FIREORD: %d\n", (v >> 0x12) & 3, (v >> 0x14) & 3);
            FUN_004c14f0(param_1, (unsigned char*)buf2, 0x108, pfstate, -1);
        }

        sprintf(buf2, "DELTATIME: %d\n", *(int*)(g_game + 0x38a3b));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x190, pfstate, -1);

        sprintf(buf2, "GAMETIME: %d\n", *(int*)(g_game + 0x38a47));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x208, pfstate, -1);

        int y3 = pfstate - 0x10;
        sprintf(buf2, "X: %d  Y: %d\n", *(int*)(g_game + 0x1431f), *(int*)(g_game + 0x14323));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x82, y3, -1);

        sprintf(buf2, "UNITS %d\\%d\n", *(int*)(g_game + 0x14353), *(int*)(g_game + 0x14367));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x108, y3, -1);

        sprintf(buf2, "PACKETS: %d %d %d\n", *(int*)(g_game + 0x1cbe), *(int*)(g_game + 0x1e09),
                *(int*)(g_game + 0x1f54));
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x190, y3, -1);

        int v = *(int*)(g_game + 0x14233) * (short)*(unsigned short*)(g_game + 0x2c90) +
                (short)*(unsigned short*)(g_game + 0x2c8e);
        unsigned char c = *(unsigned char*)(*(int*)(g_game + 0x14287) + v * 15 + 4);
        sprintf(buf2, "XYH: %d %d %d\n", (short)*(unsigned short*)(g_game + 0x2c8e),
                (short)*(unsigned short*)(g_game + 0x2c90), c);
        FUN_004c14f0(param_1, (unsigned char*)buf2, 0x208, y3, -1);
        return;
    }

    snapshot.button = *(int*)(g_game + 0x581);
    int& local_60 = snapshot.button;
    snapshot.selected = *(unsigned short*)(g_game + 0x2cba);
    unsigned short& local_38 = snapshot.selected;
    snapshot.feature = *(unsigned short*)(g_game + 0x2cbc);
    unsigned short& local_5e = snapshot.feature;
    snapshot.height = *(int*)(g_game + 0x37e94);
    snapshot.width = *(int*)(g_game + 0x37e90);

    if (local_38 != 0) {
        int idx = local_38 & 0xffff;
        char* unit = *(char**)(g_game + 0x14357) + idx * 0x118;
        snapshot.health = *(unsigned short*)(unit + 0x108);
        snapshot.build = *(unsigned short*)(unit + 0xb8);
        snapshot.orderName = FUN_00439df0(unit);
        snapshot.values[0] = *(float*)(unit + 0xd0);
        snapshot.values[1] = *(float*)(unit + 0xcc);
        snapshot.values[2] = *(float*)(unit + 0xe8);
        snapshot.values[3] = *(float*)(unit + 0xe4);

        int* local_3e_int = snapshot.weapons;
        char* p = (char*)(unit + 0x1f);
        for (int i = 0; i < 3; i++) {
            char* weapon = *(char**)(p - 0xf);
            if (*(unsigned short*)(weapon + 0xe4) <= 0x1e || !(*(unsigned char*)p & 2)) {
                local_3e_int[i] = -1;
            } else {
                local_3e_int[i] = *(unsigned short*)(p - 7);
            }
            p += 0x1c;
        }

        snapshot.targetType = 0;
        if (*(unsigned char*)(unit + 0xff) == *(unsigned char*)(g_game + 0x2a43)) {
            int result = FUN_00439dd0((int)unit);
            if (result != 0) {
                snapshot.targetType = *(unsigned short*)(result + 0xa8);
                snapshot.targetHealth = *(unsigned short*)(result + 0x108);
            }
        }
    }

    char* saved = (char*)(g_game + 0x37e60);
    if (memcmp(saved, &snapshot, sizeof(snapshot)) == 0) {
        return;
    }
    memcpy(saved, &snapshot, sizeof(snapshot));

    char* local_28 = g_game + 0xdcb;
    FUN_004c13a0(0x53, FUN_004c13f0());

    int player = *(unsigned char*)(g_game + 0x2a43);
    char* local_24 = g_game + player * 331 + 0x1b63;
    char* ptr = *(char**)(g_game + player * 331 + 0x1b8a);
    int idx2 = *(unsigned char*)(ptr + 0x95);
    char* ebp = g_game + idx2 * 562 + 0x37f3d;
    FUN_004c1420(*(int*)(ebp + 0x22e));
    int y = 0x81;
    do {
        int dy = FUN_004b6710() - 0x20;
        char* ptr2 = *(char**)(local_24 + 0x27);
        int idx3 = *(unsigned char*)(ptr2 + 0x95);
        unsigned short* ptr3 = *(unsigned short**)(g_game + idx3 * 4 + 0x14833);
        int bmp = FUN_004b7f30(ptr3, 0);
        FUN_004b7f90(param_1, (void*)bmp, (short)*(unsigned short*)(bmp + 4) + y,
                     (short)*(unsigned short*)(bmp + 6) + dy);
        y += *(unsigned short*)bmp;
    } while (y < *(int*)(g_game + 0x37e1f));

    if (snapshot.button != -1) {
        // strncpy path
        char* buf7 = text;
        strncpy(buf7, (char*)(*(int*)(*(int*)(g_game + 0x531) + 4) + local_60 * 0x15b + 2), 0x10);
        buf7[0x10] = 0;
        unsigned short type = FUN_00488b10(buf7);
        if (type != 0) {
            int base = *(int*)(g_game + 0x1439b);
            if (_strcmpi(buf7, "CORBUILD") != 0) {
                char* buf8 = text;
                sprintf(buf8, "%s  M:%d E:%d", (char*)(type * 0x249 + base),
                        (int)*(float*)(type * 0x249 + base + 0x186),
                        (int)*(float*)(type * 0x249 + base + 0x18a));
                FUN_004c14f0(param_1, (unsigned char*)buf8, *(int*)(ebp + 0x1d2),
                             *(int*)(ebp + 0x1d6) + iVar5, -1);
                FUN_004c14f0(param_1, (unsigned char*)(type * 0x249 + base + 0x40),
                             *(int*)(ebp + 0x1e2), *(int*)(ebp + 0x1e6) + iVar5, -1);
                return;
            }
        }
    } else {
        if (local_38 != 0) {
            int idx = local_38 & 0xffff;
            char* unit = *(char**)(g_game + 0x14357) + idx * 0x118;
            if (*(unsigned short*)(unit + 0xa6) != 0) {
                char* local_220 = g_game + *(unsigned char*)(g_game + 0x2a43) * 331 + 0x1b63;
                int visible = FUN_00465ac0(local_220, unit);
                if (visible != 0) {
                    char* definition = *(char**)(unit + 0x92);
                    unsigned int unitFlags = *(unsigned int*)(definition + 0x245);
                    int flagsOk = ((unitFlags & 0x20000) | ((unitFlags >> 1) & 0x20000)) >> 0x11;
                    int gameMode = ((Class_00435100*)*(void**)(g_game + 0x391e9))->FUN_00435100();
                    if (gameMode == 3 && flagsOk)
                        strcpy(text, *(char**)(unit + 0x96) + 0x2b);
                    else
                        strcpy(text, *(char**)(unit + 0x92));
                    int titleX = *(int*)(ebp + 0x142) -
                                 FUN_004c1480((void*)*(int*)(ebp + 0x22e), (unsigned char*)text) / 2;
                    FUN_004c13a0(0x53, FUN_004c13f0());
                    FUN_004c14f0(param_1, (unsigned char*)text, titleX, *(int*)(ebp + 0x146) + iVar5,
                                 -1);
                    if (*(unsigned char*)(*(char**)(unit + 0x96) + 0x146) ==
                            *(unsigned char*)(g_game + 0x2a43) ||
                        !(*(unsigned int*)(*(char**)(unit + 0x92) + 0x241) & 0x4000)) {
                        int maximum = *(int*)(*(char**)(unit + 0x92) + 0x1fa);
                        DrawBar_0046a860(param_1, (Rect_0046a860*)(ebp + 0x152),
                                         *(short*)(unit + 0x108), maximum, iVar5,
                                         (unsigned char*)local_28);
                    }
                    FUN_00467c00(param_1, *(void**)(unit + 0x96), ebp + 0x132, iVar5);
                    if (*(unsigned char*)(unit + 0xff) == *(unsigned char*)(g_game + 0x2a43) ||
                        (*(unsigned char*)(g_game + 0x3923b) & 2)) {
                        FUN_004c13a0((unsigned char)local_28[10], FUN_004c13f0());
                        sprintf(amount, "+%.1f", Positive_0046a860(snapshot.values[3]));
                        FUN_004c14f0(param_1, (unsigned char*)amount, *(int*)(ebp + 0x182),
                                     *(int*)(ebp + 0x186) + iVar5, -1);
                        sprintf(amount, "+%.0f", Positive_0046a860(snapshot.values[1]));
                        FUN_004c14f0(param_1, (unsigned char*)amount, *(int*)(ebp + 0x162),
                                     *(int*)(ebp + 0x166) + iVar5, -1);
                        FUN_004c13a0((unsigned char)local_28[12], FUN_004c13f0());
                        sprintf(amount, "-%.1f", Positive_0046a860(snapshot.values[2]));
                        FUN_004c14f0(param_1, (unsigned char*)amount, *(int*)(ebp + 0x192),
                                     *(int*)(ebp + 0x196) + iVar5, -1);
                        sprintf(amount, "-%.0f", Positive_0046a860(snapshot.values[0]));
                        FUN_004c14f0(param_1, (unsigned char*)amount, *(int*)(ebp + 0x172),
                                     *(int*)(ebp + 0x176) + iVar5, -1);
                        if ((*(unsigned int*)(unit + 0x110) & 0x80000000) &&
                            *(unsigned short*)(unit + 0xb8)) {
                            int killsX = *(int*)(ebp + 0x152);
                            int killsY = *(int*)(ebp + 0x15e) + iVar5 + 2;
                            char* plural = FUN_004c5740("kills");
                            char* singular = FUN_004c5740("kill");
                            unsigned short kills = *(unsigned short*)(unit + 0xb8);
                            if (kills > 4)
                                sprintf(killsText, "%d %s - %s", kills, kills == 1 ? singular : plural,
                                        FUN_004c5740("Veteran"));
                            else
                                sprintf(killsText, "%d %s", kills, kills == 1 ? singular : plural);
                            FUN_004c13a0(*(unsigned char*)(g_game + 0xdda), FUN_004c13f0());
                            FUN_004c14f0(param_1, (unsigned char*)killsText, killsX, killsY, -1);
                        }
                        if (snapshot.orderName) {
                            strcpy(amount, FUN_004c5740((char*)snapshot.orderName));
                            int orderX =
                                *(int*)(ebp + 0x1a2) -
                                FUN_004c1480((void*)*(int*)(ebp + 0x22e), (unsigned char*)amount) / 2;
                            FUN_004c13a0(0x53, FUN_004c13f0());
                            FUN_004c14f0(param_1, (unsigned char*)amount, orderX,
                                         *(int*)(ebp + 0x1a6) + iVar5, -1);
                        }
                    }
                    int progress = FUN_00439d20(unit);
                    if (progress) {
                        if (*(unsigned char*)(*(char**)(unit + 0x96) + 0x146) !=
                            *(unsigned char*)(g_game + 0x2a43))
                            return;
                        char* weaponText = FUN_004c5740("Weapon");
                        int progressX =
                            *(int*)(ebp + 0x1b2) -
                            FUN_004c1480((void*)*(int*)(ebp + 0x22e), (unsigned char*)weaponText) / 2;
                        FUN_004c13a0(0x53, FUN_004c13f0());
                        FUN_004c14f0(param_1, (unsigned char*)weaponText, progressX,
                                     *(int*)(ebp + 0x1b6) + iVar5, -1);
                        DrawBar_0046a860(param_1, (Rect_0046a860*)(ebp + 0x1c2), progress, 100, iVar5,
                                         (unsigned char*)local_28);
                        return;
                    }
                    if (snapshot.targetType) {
                        char* target = *(char**)(g_game + 0x14357) + snapshot.targetType * 0x118;
                        if (!*(unsigned short*)(target + 0xa6) || !FUN_00465ac0(local_220, target))
                            return;
                        int targetX = *(int*)(ebp + 0x1b2) -
                                      FUN_004c1480((void*)*(int*)(ebp + 0x22e),
                                                   (unsigned char*)*(char**)(target + 0x92)) /
                                          2;
                        FUN_004c13a0(0x53, FUN_004c13f0());
                        FUN_004c14f0(param_1, (unsigned char*)*(char**)(target + 0x92), targetX,
                                     *(int*)(ebp + 0x1b6) + iVar5, -1);
                        if (*(unsigned char*)(*(char**)(target + 0x96) + 0x146) ==
                                *(unsigned char*)(g_game + 0x2a43) ||
                            !(*(unsigned int*)(*(char**)(target + 0x92) + 0x241) & 0x4000)) {
                            int maximum = *(int*)(*(char**)(target + 0x92) + 0x1fa);
                            DrawBar_0046a860(param_1, (Rect_0046a860*)(ebp + 0x1c2),
                                             *(short*)(target + 0x108), maximum, iVar5,
                                             (unsigned char*)local_28);
                        }
                    }
                } else {
                    char* prefix = ((*(int*)(unit + 0x110) >> 9) & 1) != 0 ? "S: " : "R: ";
                    char* s = FUN_004c5740("Unidentified object");
                    char* buf6 = text;
                    sprintf(buf6, "%s%s", prefix, s);
                    int w = FUN_004c1480((void*)*(int*)(ebp + 0x22e), (unsigned char*)buf6);
                    int yy = *(int*)(ebp + 0x142) - w / 2;
                    FUN_004c13a0(0x53, FUN_004c13f0());
                    FUN_004c14f0(param_1, (unsigned char*)buf6, yy, *(int*)(ebp + 0x146) + iVar5,
                                 -1);
                    return;
                }
            }
        } else if (local_5e != 0xffff) {
                    int idx = local_5e & 0xffff;
                    char* unit2 = (char*)(*(int*)(g_game + 0x1426f) + idx * 0x100);
                    if ((*(unsigned char*)(unit2 + 0xff) & 4) == 0 ||
                        (*(unsigned char*)(g_game + 0x3923b) & 2) != 0) {
                        char mText[16];
                        char* buf3 = mText;
                        if (*(float*)(unit2 + 0xf0) != 0.0f) {
                            sprintf(buf3, " M:%d", (int)*(float*)(unit2 + 0xf0));
                        } else {
                            buf3[0] = 0;
                        }
                        char eText[16];
                        char* buf4 = eText;
                        if (*(float*)(unit2 + 0xec) != 0.0f) {
                            sprintf(buf4, " E:%d", (int)*(float*)(unit2 + 0xec));
                        } else {
                            buf4[0] = 0;
                        }
                        char* name = unit2;
                        if ((*(unsigned char*)(g_game + 0x3923b) >> 1 & 1) == 0) {
                            name = unit2 + 0x80;
                        }
                        char* buf5 = text;
                        if ((*(unsigned char*)(unit2 + 0xff) & 2) == 0) {
                            char* s = FUN_004c5740(name);
                            sprintf(buf5, "%s %s%s", s, buf3, buf4);
                        } else {
                            char* s = FUN_004c5740(name);
                            strcpy(buf5, s);
                        }
                        FUN_004c13a0(0x53, FUN_004c13f0());
                        FUN_004c14f0(param_1, (unsigned char*)buf5, *(int*)(ebp + 0x1d2),
                                     *(int*)(ebp + 0x1d6) + iVar5, -1);
                    }
        }
    }
}