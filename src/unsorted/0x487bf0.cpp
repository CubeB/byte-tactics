// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
// PARTIAL 67.7% (ours 1811 bytes = the original's). Notes from the claude-sonnet-5-5 pass:
//  - Class_00438760 is trivially copyable (no copy constructor declared). The
//    string kinds are passed as implicit conversions (`FUN_0043adc0("WAIT", ...)`):
//    MSVC 5 builds them straight in the argument slot (`push ecx; mov ecx,esp;
//    push str; call ctor`), while a named `Class_00438760 out = FUN_0043f0e0(...)`
//    is read back as a raw dword. An explicit `Class_00438760("X")` temp, or a
//    declared copy constructor, gives the wrong sequence instead.
//  - P and W pass `(int)(f * 30.0f)` straight to FUN_0043adc0; there is no
//    shared `fire` result variable. W's "%d" is the `move` slot (frame 0x18).
//  - W-a: `int count = sscanf(...); int target = 0; if (count == 1) ...` puts
//    `target` in eax after the call like the original.
//  - The frame order f1 f2 n pos move | selected | wf pf fire | G A M U P outs
//    (all dwords from frame 0x00 to 0x3c, buf at 0x40) is only reached with the
//    locals grouped in small structs (MSVC 5 orders loose scalars by its own
//    ranking, not by declaration).
// STILL DIFFERENT: with `volatile unsigned int flags` in Unit_00487bf0 this
// scores 92.7% (see build/scratch/0x487bf0/t5.cpp): the original reads
// unit->flags twice back to back in 'O' (edx and eax, no CSE), reads it again
// after sscanf, does the final `&= ~0x20` as load/and/store, and the sink of
// the pos.z stores, the B arm's shared push and the prologue order all follow.
// Only the final `flags &= ~0x20` temp register (ecx in the original, edx here)
// then differs. Left non-volatile here per the brief; the lead should decide.

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
    char pad[3]; // makes each Outs_t member a dword slot like the original
    Class_00438760(const char* name);
    Class_00438760() {}
};

void __stdcall FUN_0043adc0(Class_00438760 kind, int remove, Unit_00487bf0* owner,
                            int id, Vec3_00487bf0* pos, int param_6, int param_7);
void __stdcall FUN_0043f0e0(Class_00438760* out, int mode, Unit_00487bf0* unit,
                            int target, Vec3_00487bf0* pos);
int __stdcall FUN_00487af0(char* name, Table_00487bf0* table, int value);
unsigned short __stdcall FUN_00488b10(char* name);
void __stdcall FUN_0048aac0(Unit_00487bf0* unit, int target, int a, int b);

struct Outs_t {
    Class_00438760 g, a, m, u, p;
};

// FUNCTION: 0x487bf0
void __stdcall FUN_00487bf0(Unit_00487bf0* unit, char* text, Table_00487bf0* table)
{
    char buf[256];
    struct { float f1, f2; int n; Vec3_00487bf0 pos; int move; } L;
    int selected = 0;
    struct { float wf; float pf; int fire; } M2;
    Outs_t out;
    int processed = 0;

    while (*text != 0) {
        while (isspace(*text))
            text++;
        L.n = strcspn(text, ",");
        strncpy(buf, text, L.n);
        text += L.n;
        buf[L.n] = 0;
        if (*text == ',')
            text++;

        switch (buf[0]) {
        case 'O':
        case 'o': {
            L.move = (unit->flags >> 0x12) & 3;
            M2.fire = (unit->flags >> 0x14) & 3;
            sscanf(buf + 1, " %d %d", &L.move, &M2.fire);
            unit->flags = (unit->flags & 0xffc3ffff)
                          | ((((M2.fire & 3) << 2) | (L.move & 3)) << 0x12);
            break;
        }
        case 'M':
        case 'm': {
            sscanf(buf + 1, " %f %f", &L.f1, &L.f2);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(L.f2 * 65536.0);
            FUN_0043f0e0(&out.m, 2, unit, 0, &L.pos);
            FUN_0043adc0(out.m, 1, unit, 0, &L.pos, 0, 0);
            processed = 1;
            break;
        }
        case 'U':
        case 'u': {
            sscanf(buf + 1, " %f %f", &L.f1, &L.f2);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(L.f2 * 65536.0);
            FUN_0043f0e0(&out.u, 5, unit, 0, &L.pos);
            FUN_0043adc0(out.u, 1, unit, 0, &L.pos, 0, 0);
            processed = 1;
            break;
        }
        case 'G':
        case 'g': {
            sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
            int target = FUN_00487af0(buf, table, 0);
            if (target != 0) {
                    FUN_0043f0e0(&out.g, 7, unit, target, 0);
                FUN_0043adc0(out.g, 1, unit, target, 0, 0, 0);
                processed = 1;
            }
            break;
        }
        case 'P':
        case 'p': {
            M2.pf = 0.0f;
            sscanf(buf + 1, " %f %f %f", &L.f1, &L.f2, &M2.pf);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(L.f2 * 65536.0);
            FUN_0043f0e0(&out.p, 9, unit, 0, &L.pos);
            FUN_0043adc0(out.p, 1, unit, 0, &L.pos, (int)(M2.pf * 30.0f), 0);
            processed = 1;
            selected = 1;
            break;
        }
        case 'A':
        case 'a': {
            if (sscanf(buf + 1, " %f %f", &L.f1, &L.f2) == 2) {
                    L.pos.x = (int)(L.f1 * 65536.0);
                L.pos.y = 0;
                L.pos.z = (int)(L.f2 * 65536.0);
                FUN_0043f0e0(&out.a, 3, unit, 0, &L.pos);
                FUN_0043adc0(out.a, 1, unit, 0, &L.pos, 0, 0);
                processed = 1;
                selected = 1;
            } else {
                sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
                unsigned short id = FUN_00488b10(buf);
                if (id != 0) {
                    FUN_0043adc0("ATTACKUTYPE", 1, unit, 0, 0, id, 0);
                    processed = 1;
                }
            }
            break;
        }
        case 'B':
        case 'b': {
            L.n = 1;
            if (buf[1] == 'w' || buf[1] == 'W') {
                sscanf(buf + 2, " %d", &L.n);
                FUN_0043adc0("BUILDWEAPON", 1, unit, 0, 0, 0, L.n);
            } else {
                sscanf(buf + 1, " %[a-zA-Z0-9_.] %d %f %f", buf, &L.n, &L.f1, &L.f2);
                L.pos.x = (int)(L.f1 * 65536.0);
                L.pos.y = 0;
                L.pos.z = (int)(L.f2 * 65536.0);
                unsigned short id = FUN_00488b10(buf);
                if (id != 0) {
                    if (unit->field_0 != 0)
                        FUN_0043adc0("MOBILEBUILD", 1, unit, 0,
                                     &L.pos, id, L.n);
                    else
                        FUN_0043adc0("BUILDINGBUILD", 1, unit, 0,
                                     0, id, L.n);
                    processed = 1;
                }
            }
            break;
        }
        case 'W':
        case 'w':
            if (buf[1] == 'a' || buf[1] == 'A') {
                int count = sscanf(buf + 2, " %[a-zA-Z0-9.]", buf);
                int target = 0;
                if (count == 1)
                    target = FUN_00487af0(buf, table, 0);
                if (target == 0)
                    target = (int)unit;
                FUN_0043adc0("WAITFORATTACK", 1, unit, target,
                             0, 0, 0);
                processed = 1;
            } else {
                M2.wf = 0.0f;
                L.move = 0;
                sscanf(buf + 1, " %f %d", &M2.wf, &L.move);
                FUN_0043adc0("WAIT", 1, unit, 0, 0, (int)(M2.wf * 30.0f), L.move);
                processed = 1;
            }
            break;
        case 'D':
        case 'd':
            FUN_0043adc0("SELFDESTRUCTFG", 1, unit, 0, 0, 1, 0);
            processed = 1;
            selected = 1;
            break;
        case 'S':
        case 's':
            FUN_0043adc0("MAKESELECTABLE", 1, unit, 0, 0, 0, 0);
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
            FUN_0043adc0("MAKESELECTABLE", 1, unit, 0, 0, 0, 0);
    }
}