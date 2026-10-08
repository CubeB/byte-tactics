// Decompiled by deepseek-v4.1, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// Kept its own file: it matches only under /Gi, which the other functions do
// not use.
// Draws the selected unit / feature info panel (and the PFSTATE debug overlay).
#include <windows.h>
#include <stdio.h>

extern char* g_game;

int GetScreenHeight();
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);
void __stdcall DrawFrame(void* dst, void* bmp, int x, int y);
void __stdcall SetTextColors(int param_1, int param_2);
int GetTextKeyColor();
void __stdcall SetFont(int param_1);
int GetFontHeight();
int __stdcall GetTextWidth(void* font, unsigned char* text);
void __stdcall DrawString(void* dst, unsigned char* text, int x, int y, int maxWidth);
char* __stdcall Translate(char* key);
int __stdcall FUN_00439d20(void* owner);
int __stdcall FUN_00439dd0(void* unit);
int __stdcall FUN_00439df0(void* obj);
int __stdcall FUN_00465ac0(void* map, void* u);
void __stdcall FUN_00467c00(void* surf, void* player, void* rect, int dy);
unsigned short __stdcall FindUnitTypeId(const char* name);
int __stdcall FillRectangle(void* surface, void* rect, int color);

class Mission {
  public:
    int GetGameType();
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

#pragma pack(push, 1)
struct Player_0046a860 { char pad[0x27]; char* info; char pad2[331 - 0x2b]; };
#pragma pack(pop)

struct Rect_0046a860 {
    int left, top, right, bottom;
};

static void DrawBar_0046a860(void* surface, Rect_0046a860* bounds, int value, int maximum, int dy,
                             unsigned char* palette) {
    Rect_0046a860 rect = *bounds;
    rect.top = rect.top + dy;
    rect.bottom += dy;
    if (value < 0)
        value = 0;
    if (value > maximum)
        value = maximum;
    rect.right = rect.left + (bounds->right - bounds->left) * value / maximum;
    FillRectangle(surface, &rect, palette[10]);
    if (rect.right != bounds->right) {
        rect.left = rect.right + 1;
        rect.right = bounds->right;
        FillRectangle(surface, &rect, palette[4]);
    }
}

static float Positive_0046a860(float value) {
    float result = value > 0.0f ? value : 0.0f;
    return result;
}

struct Flags46b_3923b { unsigned short b0:1; unsigned short b1:1; };

// FUNCTION: 0x46a860
void __stdcall DrawUnitInfoPanel(void* surface) {
    int yOffset = *(int*)(g_game + 0x37e23) - *(int*)(g_game + 0x147a7);
    Snapshot_0046a860 snapshot;
    memset(&snapshot, 0, sizeof(snapshot));
    char text[256];

    unsigned short flags = *(unsigned short*)(g_game + 0x3923b);
    if (flags & 1 && flags & 2) {
        int y = 0x81;
        unsigned char player = *(unsigned char*)(g_game + 0x2a43);
        char* table = *(char**)(g_game + player * 331 + 0x1b8a);
        int idx = *(unsigned char*)(table + 0x95);
        do {
            int dy = GetScreenHeight() - 0x20;
            unsigned short* icon = *(unsigned short**)(g_game + idx * 4 + 0x14833);
            int bmp = GetGafFrame(icon, 0);
            DrawFrame(surface, (void*)bmp, (short)*(unsigned short*)(bmp + 4) + y,
                         (short)*(unsigned short*)(bmp + 6) + dy);
            y += *(unsigned short*)bmp;
        } while (y < *(int*)(g_game + 0x37e1f));

        SetFont(*(int*)(g_game + 0x391f9));
        SetTextColors(0x53, GetTextKeyColor());
        // No pfable or field_c locals: the PFSTATE sprintf re-reads *(g_game + 0xc) per argument.
        int pfstate = *(int*)(g_game + 0x37e23) - GetFontHeight() - 1;
        sprintf(text, "PFSTATE %d, PFABLE %d\n", *(unsigned char*)(*(char**)(g_game + 0xc) + 0xf0) & 1,
                *(int*)(*(char**)(g_game + 0xc) + 0x9c));
        DrawString(surface, (unsigned char*)text, 0x82, pfstate, -1);

        if (*(unsigned short*)(g_game + 0x2cba) != 0) {
            int idx = *(unsigned short*)(g_game + 0x2cba) & 0xffff;
            char* unit = *(char**)(g_game + 0x14357) + idx * 0x118;
            sprintf(text, "MOVEORD: %d FIREORD: %d\n", (*(unsigned int*)(unit + 0x110) >> 0x12) & 3,
                    (*(unsigned int*)(unit + 0x110) >> 0x14) & 3);
            DrawString(surface, (unsigned char*)text, 0x108, pfstate, -1);
        }

        sprintf(text, "DELTATIME: %d\n", *(int*)(g_game + 0x38a3b));
        DrawString(surface, (unsigned char*)text, 0x190, pfstate, -1);

        sprintf(text, "GAMETIME: %d\n", *(int*)(g_game + 0x38a47));
        DrawString(surface, (unsigned char*)text, 0x208, pfstate, -1);

        pfstate -= 0x10;
        sprintf(text, "X: %d  Y: %d\n", *(int*)(g_game + 0x1431f), *(int*)(g_game + 0x14323));
        DrawString(surface, (unsigned char*)text, 0x82, pfstate, -1);

        sprintf(text, "UNITS %d\\%d\n", *(int*)(g_game + 0x14353), *(int*)(g_game + 0x14367));
        DrawString(surface, (unsigned char*)text, 0x108, pfstate, -1);

        sprintf(text, "PACKETS: %d %d %d\n", *(int*)(g_game + 0x1cbe), *(int*)(g_game + 0x1e09),
                *(int*)(g_game + 0x1f54));
        DrawString(surface, (unsigned char*)text, 0x190, pfstate, -1);

        int v = *(int*)(g_game + 0x14233);
        v *= (short)*(unsigned short*)(g_game + 0x2c90);
        v += (short)*(unsigned short*)(g_game + 0x2c8e);
        unsigned char c = *(unsigned char*)(*(int*)(g_game + 0x14287) + v * 13 + 4);
        sprintf(text, "XYH: %d %d %d\n", (short)*(unsigned short*)(g_game + 0x2c8e),
                (short)*(unsigned short*)(g_game + 0x2c90), c);
        DrawString(surface, (unsigned char*)text, 0x208, pfstate, -1);
        return;
    }

    snapshot.button = *(int*)(g_game + 0x581);
    snapshot.selected = *(unsigned short*)(g_game + 0x2cba);
    snapshot.feature = *(unsigned short*)(g_game + 0x2cbc);
    snapshot.height = *(int*)(g_game + 0x37e94);
    snapshot.width = *(int*)(g_game + 0x37e90);

    if (snapshot.selected != 0) {
        char* unit = *(char**)(g_game + 0x14357) + snapshot.selected * 0x118;
        snapshot.health = *(unsigned short*)(unit + 0x108);
        snapshot.build = *(unsigned short*)(unit + 0xb8);
        snapshot.orderName = FUN_00439df0(unit);
        // Copied through float temporaries; direct assignment gives integer moves.
        float f0 = *(float*)(unit + 0xd0);
        float f1 = *(float*)(unit + 0xcc);
        float f2 = *(float*)(unit + 0xe8);
        float f3 = *(float*)(unit + 0xe4);
        snapshot.values[0] = f0;
        snapshot.values[1] = f1;
        snapshot.values[2] = f2;
        snapshot.values[3] = f3;
        char* slot = unit + 0x1f;
        int* out = snapshot.weapons;
        // Keep both `*out = -1` stores (nested ifs): the extra use sets the register of out.
        for (int i = 0; i < 3; i++, out++, slot += 0x1c) {
            char* weapon = *(char**)(slot - 0xf);
            if (*(unsigned short*)(weapon + 0xe4) > 0x1e) {
                if (*(unsigned char*)slot & 2)
                    *out = *(unsigned short*)(slot - 7);
                else
                    *out = -1;
            } else
                *out = -1;
        }

        snapshot.targetType = 0;
        if (*(unsigned char*)(unit + 0xff) == *(unsigned char*)(g_game + 0x2a43)) {
            int result = FUN_00439dd0(unit);
            if (result != 0) {
                snapshot.targetType = *(unsigned short*)(result + 0xa8);
                snapshot.targetHealth = *(unsigned short*)(result + 0x108);
            }
        }
    }

    char* saved = g_game + 0x37e60;
    if (memcmp(saved, &snapshot, sizeof(snapshot)) == 0)
        return;
    memcpy(saved, &snapshot, sizeof(snapshot));

    char* palette = g_game + 0xdcb;
    SetTextColors(0x53, GetTextKeyColor());

    int player = *(unsigned char*)(g_game + 0x2a43);
    Player_0046a860* playerInfo = (Player_0046a860*)(g_game + 0x1b63) + player;
    char* info = playerInfo->info;
    int side = *(unsigned char*)(info + 0x95);
    char* panel = g_game + side * 562 + 0x37f3d;
    SetFont(*(int*)(panel + 0x22e));
    int y = 0x81;
    do {
        int dy = GetScreenHeight() - 0x20;
        char* loopInfo = playerInfo->info;
        int loopSide = *(unsigned char*)(loopInfo + 0x95);
        unsigned short* icon = *(unsigned short**)(g_game + loopSide * 4 + 0x14833);
        int bmp = GetGafFrame(icon, 0);
        DrawFrame(surface, (void*)bmp, (short)*(unsigned short*)(bmp + 4) + y,
                     (short)*(unsigned short*)(bmp + 6) + dy);
        y += *(unsigned short*)bmp;
    } while (y < *(int*)(g_game + 0x37e1f));

    if (snapshot.button != -1) {
        strncpy(text, (char*)(snapshot.button * 0x15b + *(int*)(*(int*)(g_game + 0x531) + 4) + 2), 0x10);
        text[0x10] = 0;
        unsigned short type = FindUnitTypeId(text);
        if (type != 0) {
            char* entry = (char*)(0x249 * type + *(int*)(g_game + 0x1439b));
            if (_strcmpi(text, "CORBUILD") != 0) {
                sprintf(text, "%s  M:%d E:%d", entry, (int)*(float*)(entry + 0x18a),
                        (int)*(float*)(entry + 0x186));
                DrawString(surface, (unsigned char*)text, *(int*)(panel + 0x1d2),
                             *(int*)(panel + 0x1d6) + yOffset, -1);
                DrawString(surface, (unsigned char*)(entry + 0x40), *(int*)(panel + 0x1e2),
                             *(int*)(panel + 0x1e6) + yOffset, -1);
                return;
            }
        }
    } else if (snapshot.selected != 0) {
        char* unit = *(char**)(g_game + 0x14357) + snapshot.selected * 0x118;
        if (*(unsigned short*)(unit + 0xa6) != 0) {
            char* playerMap = g_game + *(unsigned char*)(g_game + 0x2a43) * 331 + 0x1b63;
            if (FUN_00465ac0(playerMap, unit)) {
                char* definition = *(char**)(unit + 0x92);
                unsigned int unitFlags = *(unsigned int*)(definition + 0x245);
                int flagsOk = ((unitFlags & 0x20000) | ((unitFlags >> 1) & 0x20000)) >> 0x11;
                int gameMode = ((Mission*)*(void**)(g_game + 0x391e9))->GetGameType();
                if (gameMode == 3 && flagsOk)
                    strcpy(text, *(char**)(unit + 0x96) + 0x2b);
                else
                    strcpy(text, *(char**)(unit + 0x92));
                int titleX = *(int*)(panel + 0x142) -
                             GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)text) / 2;
                SetTextColors(0x53, GetTextKeyColor());
                DrawString(surface, (unsigned char*)text, titleX, yOffset + *(int*)(panel + 0x146), -1);
                if (*(unsigned char*)(*(char**)(unit + 0x96) + 0x146) == *(unsigned char*)(g_game + 0x2a43) ||
                    !(*(unsigned int*)(*(char**)(unit + 0x92) + 0x241) & 0x4000)) {
                    int maximum = *(int*)(*(char**)(unit + 0x92) + 0x1fa);
                    DrawBar_0046a860(surface, (Rect_0046a860*)(panel + 0x152), *(short*)(unit + 0x108),
                                     maximum, yOffset, (unsigned char*)palette);
                }
                FUN_00467c00(surface, *(void**)(unit + 0x96), panel + 0x132, yOffset);
                if (*(unsigned char*)(unit + 0xff) == *(unsigned char*)(g_game + 0x2a43) ||
                    (*(unsigned char*)(g_game + 0x3923b) & 2)) {
                    // Declared in this block, not at function scope: affects load scheduling.
                    char amount[100];
                    char killsText[100];
                    SetTextColors((unsigned char)palette[10], GetTextKeyColor());
                    sprintf(amount, "+%.1f", Positive_0046a860(snapshot.values[3]));
                    DrawString(surface, (unsigned char*)amount, *(int*)(panel + 0x182),
                                 *(int*)(panel + 0x186) + yOffset, -1);
                    sprintf(amount, "+%.0f", Positive_0046a860(snapshot.values[1]));
                    DrawString(surface, (unsigned char*)amount, *(int*)(panel + 0x162),
                                 *(int*)(panel + 0x166) + yOffset, -1);
                    SetTextColors((unsigned char)palette[12], GetTextKeyColor());
                    sprintf(amount, "-%.1f", Positive_0046a860(snapshot.values[2]));
                    DrawString(surface, (unsigned char*)amount, *(int*)(panel + 0x192),
                                 *(int*)(panel + 0x196) + yOffset, -1);
                    sprintf(amount, "-%.0f", Positive_0046a860(snapshot.values[0]));
                    DrawString(surface, (unsigned char*)amount, *(int*)(panel + 0x172),
                                 *(int*)(panel + 0x176) + yOffset, -1);
                    if ((*(unsigned int*)(unit + 0x110) & 0x80000000) && *(unsigned short*)(unit + 0xb8)) {
                        int killsY = *(int*)(panel + 0x15e) + yOffset + 2, killsX = *(int*)(panel + 0x152);
                        char* plural = Translate("kills");
                        char* singular = Translate("kill");
                        if (*(unsigned short*)(unit + 0xb8) > 4)
                            sprintf(killsText, "%d %s - %s", *(unsigned short*)(unit + 0xb8),
                                    *(unsigned short*)(unit + 0xb8) == 1 ? singular : plural,
                                    Translate("Veteran"));
                        else
                            sprintf(killsText, "%d %s", *(unsigned short*)(unit + 0xb8),
                                    *(unsigned short*)(unit + 0xb8) == 1 ? singular : plural);
                        SetTextColors(*(unsigned char*)(g_game + 0xdda), GetTextKeyColor());
                        DrawString(surface, (unsigned char*)killsText, killsX, killsY, -1);
                    }
                    if (snapshot.orderName) {
                        strcpy(amount, Translate((char*)snapshot.orderName));
                        int orderX = *(int*)(panel + 0x1a2) -
                                     GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)amount) / 2;
                        SetTextColors(0x53, GetTextKeyColor());
                        DrawString(surface, (unsigned char*)amount, orderX, *(int*)(panel + 0x1a6) + yOffset, -1);
                    }
                }
                int progress = FUN_00439d20(unit);
                if (progress) {
                    if (*(unsigned char*)(*(char**)(unit + 0x96) + 0x146) != *(unsigned char*)(g_game + 0x2a43))
                        return;
                    char* weaponText = Translate("Weapon");
                    int progressX = *(int*)(panel + 0x1b2) -
                                    GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)weaponText) / 2;
                    SetTextColors(0x53, GetTextKeyColor());
                    DrawString(surface, (unsigned char*)weaponText, progressX, *(int*)(panel + 0x1b6) + yOffset, -1);
                    DrawBar_0046a860(surface, (Rect_0046a860*)(panel + 0x1c2), progress, 100, yOffset,
                                     (unsigned char*)palette);
                    return;
                }
                if (snapshot.targetType) {
                    char* target = *(char**)(g_game + 0x14357) + snapshot.targetType * 0x118;
                    if (!*(unsigned short*)(target + 0xa6) || !FUN_00465ac0(playerMap, target))
                        return;
                    int targetX = *(int*)(panel + 0x1b2) -
                                  GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)*(char**)(target + 0x92)) / 2;
                    SetTextColors(0x53, GetTextKeyColor());
                    DrawString(surface, (unsigned char*)*(char**)(target + 0x92), targetX,
                                 *(int*)(panel + 0x1b6) + yOffset, -1);
                    if (*(unsigned char*)(*(char**)(target + 0x96) + 0x146) == *(unsigned char*)(g_game + 0x2a43) ||
                        !(*(unsigned int*)(*(char**)(target + 0x92) + 0x241) & 0x4000)) {
                        int maximum = *(int*)(*(char**)(target + 0x92) + 0x1fa);
                        DrawBar_0046a860(surface, (Rect_0046a860*)(panel + 0x1c2), *(short*)(target + 0x108),
                                         maximum, yOffset, (unsigned char*)palette);
                    }
                }
            } else {
                char* prefix = ((unsigned char)(*(unsigned int*)(unit + 0x110) >> 9) & 1) != 0 ? "S: " : "R: ";
                char* s = Translate("Unidentified object");
                sprintf(text, "%s%s", prefix, s);
                int w = GetTextWidth((void*)*(int*)(panel + 0x22e), (unsigned char*)text);
                int x = *(int*)(panel + 0x142) - w / 2;
                SetTextColors(0x53, GetTextKeyColor());
                DrawString(surface, (unsigned char*)text, x, *(int*)(panel + 0x146) + yOffset, -1);
                return;
            }
        }
    } else if (snapshot.feature != 0xffff) {
        char* feature = (char*)(*(int*)(g_game + 0x1426f) + snapshot.feature * 0x100);
        if ((*(unsigned char*)(feature + 0xff) & 4) == 0 || (*(unsigned char*)(g_game + 0x3923b) & 2) != 0) {
            char mText[16];
            if (*(float*)(feature + 0xf0) != 0.0f)
                sprintf(mText, " M:%d", (int)*(float*)(feature + 0xf0));
            else
                mText[0] = 0;
            char eText[16];
            if (*(float*)(feature + 0xec) != 0.0f)
                sprintf(eText, " E:%d", (int)*(float*)(feature + 0xec));
            else
                eText[0] = 0;
            char* name = ((Flags46b_3923b*)(g_game + 0x3923b))->b1 ? feature : feature + 0x80;
            if ((*(unsigned char*)(feature + 0xff) & 2) == 0)
                sprintf(text, "%s %s%s", Translate(name), mText, eText);
            else
                strcpy(text, Translate(name));
            SetTextColors(0x53, GetTextKeyColor());
            DrawString(surface, (unsigned char*)text, *(int*)(panel + 0x1d2), *(int*)(panel + 0x1d6) + yOffset, -1);
        }
    }
}
