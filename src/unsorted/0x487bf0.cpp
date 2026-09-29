// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (28.9%, 1911 bytes vs 1811). Comma-separated per-unit "extra"
// command list parser (0x488310 fills the string). Command dispatch is exact:
// the switch case order is A B D G I M O P S U W (jump table at 0x488270,
// byte index table at 0x4882cc), the token is strcspn'd to the next comma and
// sscanf'd into buf from buf+1 (or buf+2 for the W and Bw sub-forms).
// Still differs: (1) the frame is 0x190/0x140 and buf lands at +0xb0 not
// +0x50, so the pre-buffer locals are not packed into the original's
// overlapping slots; (2) the original keeps text in ebp across the loop and
// spills it to the arg2 home slot, ours reloads it at the loop top and emits
// an extra jmp; (3) register roles inside the cases (count in ebp vs eax).
// Call facts: 0x43f0e0 takes FIVE args (out Class_00438760*, mode, unit,
// target, pos), and 0x43adc0's first arg is a Class_00438760 temp built on
// the stack by 0x438760.
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Vec3_00487bf0 {
    int x, y, z;
};

struct Unit_00487bf0 {
    int field_0;                       // +0x0
    char unknown_4[0x110 - 4];
    unsigned int flags;                // +0x110
};

struct Table_00487bf0 {                // the std::vector<Unit*> from 0x488310
    char unknown_0[4];
    int* ids;                          // +0x4 (_Myfirst)
};
#pragma pack(pop)

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() { index = 0; }
};

void __stdcall FUN_0043adc0(Class_00438760 kind, int remove, Unit_00487bf0* owner,
                            int id, Vec3_00487bf0* pos, int param_6, int param_7);
void __stdcall FUN_0043f0e0(Class_00438760* out, int mode, Unit_00487bf0* unit,
                            int target, Vec3_00487bf0* pos);
int __stdcall FUN_00487af0(char* name, Table_00487bf0* table, int value);
unsigned short __stdcall FUN_00488b10(char* name);
void __stdcall FUN_0048aac0(Unit_00487bf0* unit, int target, int a, int b);

// FUNCTION: 0x487bf0
void __stdcall FUN_00487bf0(Unit_00487bf0* unit, char* text, Table_00487bf0* table)
{
    char buf[240];
    int processed = 0;
    int selected = 0;

    while (*text != 0) {
        while (isspace(*text))
            text++;
        int count = strcspn(text, ",");
        strncpy(buf, text, count);
        text += count;
        buf[count] = 0;
        if (*text == ',')
            text++;

        switch (buf[0]) {
        case 'A':
        case 'a': {
            float f1, f2;
            if (sscanf(buf + 1, " %f %f", &f1, &f2) == 2) {
                Vec3_00487bf0 pos;
                Class_00438760 out;
                pos.x = (int)(f1 * 65536.0);
                pos.y = 0;
                pos.z = (int)(f2 * 65536.0);
                FUN_0043f0e0(&out, 3, unit, 0, &pos);
                FUN_0043adc0(out, 1, unit, 0, &pos, 0, 0);
                processed = 1;
                selected = 1;
            } else {
                sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
                unsigned short id = FUN_00488b10(buf);
                if (id != 0) {
                    FUN_0043adc0(Class_00438760("ATTACKUTYPE"), 1, unit, 0, 0, id, 0);
                    processed = 1;
                }
            }
            break;
        }
        case 'B':
        case 'b':
            if (buf[1] == 'w' || buf[1] == 'W') {
                int n;
                sscanf(buf + 2, " %d", &n);
                FUN_0043adc0(Class_00438760("BUILDWEAPON"), 1, unit, 0, 0, 0, n);
            } else {
                int n;
                float f1, f2;
                sscanf(buf + 1, " %[a-zA-Z0-9_.] %d %f %f", buf, &n, &f1, &f2);
                Vec3_00487bf0 pos;
                Class_00438760 out;
                pos.x = (int)(f1 * 65536.0);
                pos.y = 0;
                pos.z = (int)(f2 * 65536.0);
                unsigned short id = FUN_00488b10(buf);
                if (id != 0) {
                    if (unit->field_0 != 0)
                        FUN_0043adc0(Class_00438760("MOBILEBUILD"), 1, unit, 0,
                                     &pos, id, n);
                    else
                        FUN_0043adc0(Class_00438760("BUILDINGBUILD"), 1, unit, 0,
                                     0, id, n);
                    processed = 1;
                }
            }
            break;
        case 'D':
        case 'd':
            FUN_0043adc0(Class_00438760("SELFDESTRUCTFG"), 1, unit, 0, 0, 1, 0);
            processed = 1;
            selected = 1;
            break;
        case 'G':
        case 'g': {
            sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
            int target = FUN_00487af0(buf, table, 0);
            if (target != 0) {
                Class_00438760 out;
                FUN_0043f0e0(&out, 7, unit, target, 0);
                FUN_0043adc0(out, 1, unit, target, 0, 0, 0);
                processed = 1;
            }
            break;
        }
        case 'I':
        case 'i': {
            sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
            int target = FUN_00487af0(buf, table, 0);
            if (target != 0)
                FUN_0048aac0(unit, target, -1, 0);
            break;
        }
        case 'M':
        case 'm': {
            float f1, f2;
            sscanf(buf + 1, " %f %f", &f1, &f2);
            Vec3_00487bf0 pos;
            Class_00438760 out;
            pos.x = (int)(f1 * 65536.0);
            pos.y = 0;
            pos.z = (int)(f2 * 65536.0);
            FUN_0043f0e0(&out, 2, unit, 0, &pos);
            FUN_0043adc0(out, 1, unit, 0, &pos, 0, 0);
            processed = 1;
            break;
        }
        case 'O':
        case 'o': {
            int move = (unit->flags >> 0x12) & 3;
            int fire = (unit->flags >> 0x14) & 3;
            sscanf(buf + 1, " %d %d", &move, &fire);
            unit->flags = (unit->flags & 0xffc3ffff)
                          | ((((fire & 3) << 2) | (move & 3)) << 0x12);
            break;
        }
        case 'P':
        case 'p': {
            float f1, f2, f3;
            sscanf(buf + 1, " %f %f %f", &f1, &f2, &f3);
            Vec3_00487bf0 pos;
            Class_00438760 out;
            pos.x = (int)(f1 * 65536.0);
            pos.y = 0;
            pos.z = (int)(f2 * 65536.0);
            int z = (int)(f3 * 30.0f);
            FUN_0043f0e0(&out, 9, unit, 0, &pos);
            FUN_0043adc0(out, 1, unit, 0, &pos, z, 0);
            processed = 1;
            selected = 1;
            break;
        }
        case 'S':
        case 's':
            FUN_0043adc0(Class_00438760("MAKESELECTABLE"), 1, unit, 0, 0, 0, 0);
            processed = 1;
            selected = 1;
            break;
        case 'U':
        case 'u': {
            float f1, f2;
            sscanf(buf + 1, " %f %f", &f1, &f2);
            Vec3_00487bf0 pos;
            Class_00438760 out;
            pos.x = (int)(f1 * 65536.0);
            pos.y = 0;
            pos.z = (int)(f2 * 65536.0);
            FUN_0043f0e0(&out, 5, unit, 0, &pos);
            FUN_0043adc0(out, 1, unit, 0, &pos, 0, 0);
            processed = 1;
            break;
        }
        case 'W':
        case 'w':
            if (buf[1] == 'a' || buf[1] == 'A') {
                int target = 0;
                if (sscanf(buf + 2, " %[a-zA-Z0-9.]", buf) == 1)
                    target = FUN_00487af0(buf, table, 0);
                if (target == 0)
                    target = (int)unit;
                FUN_0043adc0(Class_00438760("WAITFORATTACK"), 1, unit, target,
                             0, 0, 0);
                processed = 1;
            } else {
                float f = 0.0f;
                int n = 0;
                sscanf(buf + 1, " %f %d", &f, &n);
                int z = (int)(f * 30.0f);
                FUN_0043adc0(Class_00438760("WAIT"), 1, unit, 0, 0, z, n);
                processed = 1;
            }
            break;
        }
    }

    if (processed) {
        unit->flags &= ~0x20u;
        if (selected == 0)
            FUN_0043adc0(Class_00438760("MAKESELECTABLE"), 1, unit, 0, 0, 0, 0);
    }
}
