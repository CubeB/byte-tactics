// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free. Names are provisional.
// PARTIAL 60.4% (ours 1847 bytes vs 1811). Frame, command buffer and the five
// per-case Class_00438760 temporaries match the original exactly.
// THE ONE REMAINING CAUSE, restated from the disassembly by space-bunny-free:
// the original holds the walk pointer `text` in ebp for the whole outer loop, so
// its latch at 0x487e50 is just `mov al,[ebp]; cmp al,bl; jne 0x487c1b`, and it
// writes the parameter home [esp+0x158] at each update (0x487c68, 0x487c76) with
// the single reload at 0x487e49 after the G case clobbers ebp. Ours instead gives
// ebp to `count` (strcspn's result), keeps `text` in eax across the switch and so
// spills it to [esp+0x158]; the latch therefore becomes `mov edx,[home]; mov
// al,[edx]; cmp al,bl; jne $L1038` plus a separate reload block. Because of the
// callee-saved preference ESI(unit), EDI(processed), EBX(zero), EBP, `text` and
// `count` are competing for the same last slot and the loser is memory-resident.
// Flipping that single choice is worth the remaining ~40%.
// Tried again by space-bunny-free, all byte-identical to the 60.4% baseline:
//   - `char* text2 = text;` used for the whole loop instead of the parameter
//     (1827 bytes but 22.9%: MSVC then never folds the two walks together);
//   - `processed = 1;` before the MAKESELECTABLE call in the S case, which is
//     what the original's `mov edi,1` at 0x48820a between the argument pushes
//     looks like. It makes things WORSE (60.2%), so the assignment really does
//     come after the call in the source, like the D case at 0x4881e0;
//   - swapping the O-case extraction order (fire before move), and hoisting the
//     flags load into a named `unsigned int fl`, both exactly 60.4%, so the
//     fire-first emission is not decided by the order of the two statements.
// Other diffs: the local slots after pos (original count 0x18 / move 0x28 /
// selected 0x2c / f3 0x34 / fire 0x38, with pos at 0x1c..0x24; ours leaves 0x18
// empty because count is in ebp, then selected 0x28 / fire 0x30 / move 0x34),
// and the switch's block placement (the original's loop latch sits between the G
// and P blocks at 0x487e50; ours puts it at the end, shared with the default arm).

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Vec3_00487bf0 {
    int x, y, z;
};

struct Unit_00487bf0 {
    int field_0;
    char unknown_4[0x110 - 4];
    unsigned int flags;
};

struct Table_00487bf0 {
    char unknown_0[4];
    int* ids;
};
#pragma pack(pop)

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
    Class_00438760() {}
    Class_00438760(const Class_00438760& other);
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
    char buf[256];
    float f1, f2;
    Vec3_00487bf0 pos;
    int move, fire;
    int selected = 0;
    int processed = 0;
    int count;

    while (*text != 0) {
        while (isspace(*text))
            text++;
        count = strcspn(text, ",");
        strncpy(buf, text, count);
        text += count;
        buf[count] = 0;
        if (*text == ',')
            text++;

        switch (buf[0]) {
        case 'O':
        case 'o': {
            move = (unit->flags >> 0x12) & 3;
            fire = (unit->flags >> 0x14) & 3;
            sscanf(buf + 1, " %d %d", &move, &fire);
            unit->flags = (unit->flags & 0xffc3ffff)
                          | ((((fire & 3) << 2) | (move & 3)) << 0x12);
            break;
        }
        case 'M':
        case 'm': {
            Class_00438760 out;
            sscanf(buf + 1, " %f %f", &f1, &f2);
            pos.x = (int)(f1 * 65536.0);
            pos.y = 0;
            pos.z = (int)(f2 * 65536.0);
            FUN_0043f0e0(&out, 2, unit, 0, &pos);
            FUN_0043adc0(out, 1, unit, 0, &pos, 0, 0);
            processed = 1;
            break;
        }
        case 'U':
        case 'u': {
            Class_00438760 out;
            sscanf(buf + 1, " %f %f", &f1, &f2);
            pos.x = (int)(f1 * 65536.0);
            pos.y = 0;
            pos.z = (int)(f2 * 65536.0);
            FUN_0043f0e0(&out, 5, unit, 0, &pos);
            FUN_0043adc0(out, 1, unit, 0, &pos, 0, 0);
            processed = 1;
            break;
        }
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
        case 'P':
        case 'p': {
            Class_00438760 out;
            float f3 = 0.0f;
            sscanf(buf + 1, " %f %f %f", &f1, &f2, &f3);
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
        case 'A':
        case 'a': {
            if (sscanf(buf + 1, " %f %f", &f1, &f2) == 2) {
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
        case 'b': {
            int n = 1;
            if (buf[1] == 'w' || buf[1] == 'W') {
                sscanf(buf + 2, " %d", &n);
                FUN_0043adc0(Class_00438760("BUILDWEAPON"), 1, unit, 0, 0, 0, n);
            } else {
                sscanf(buf + 1, " %[a-zA-Z0-9_.] %d %f %f", buf, &n, &f1, &f2);
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
        case 'D':
        case 'd':
            FUN_0043adc0(Class_00438760("SELFDESTRUCTFG"), 1, unit, 0, 0, 1, 0);
            processed = 1;
            selected = 1;
            break;
        case 'S':
        case 's':
            FUN_0043adc0(Class_00438760("MAKESELECTABLE"), 1, unit, 0, 0, 0, 0);
            processed = 1;
            selected = 1;
            break;
        case 'I':
        case 'i': {
            sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
            int target = FUN_00487af0(buf, table, 0);
            if (target != 0)
                FUN_0048aac0(unit, target, -1, 0);
            break;
        }
        }
    }
    if (processed) {
        unit->flags &= ~0x20u;
        if (selected == 0)
            FUN_0043adc0(Class_00438760("MAKESELECTABLE"), 1, unit, 0, 0, 0, 0);
    }
}
