// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6,
// edited by deepseek-v4.1, finished by space-bunny-free, finished by
// deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by
// deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny
// Free. Names are provisional.
// MATCH. Two changes took this from 76.9% (1815 bytes) to a byte-identical 1811,
// plus one reorder that was the last byte.
//  - 'O': one of the two reads of unit->flags goes through a local pointer,
//    `unsigned int* flags = &unit->flags;` then
//    `M2.fire = (*flags >> 0x14) & 3;`, with the other still
//    `unit->flags >> 0x12`. That is what defeats MSVC 5's load CSE. With two
//    plain reads it folds them into one load plus `mov edx,eax`, and the arm
//    comes out 103 bytes against the original's 106. Reading one of them
//    through the pointer makes it emit the original's two loads back to back,
//    and as a side effect it picks edx for the >>18 half and eax for the >>20
//    half, which turns `and edx,0xffc3ffff` (6 bytes) into
//    `and eax,0xffc3ffff` (5). The arm then matches instruction for
//    instruction. build/scratch/487bf0/t_loads.cpp and t_alias.cpp hold the
//    probes: MSVC 5 reloads a field only when an aliasing store or a call sits
//    between the two reads, and a local pointer to the field is the one way to
//    get that reload with no extra instructions. A store through a parameter
//    pointer does it too, but adds code; a store MSVC deletes does not, not
//    even inside a statically folded branch, so no dead statement substitutes
//    for this. Earlier passes used `volatile` for it; that is not needed.
//  - With the O arm right, the four arms P, A, B and B-else that needed the
//    Found() codegen crutch fall into place by themselves, so the G arm wants
//    `if (target != 0)` again and the crutch is gone. That is where the last
//    four bytes were: 1815 was 1811 - 3 + 7, the O arm three bytes short
//    against Found()'s seven extra in the G arm, and both halves of that had
//    to go together. Nothing less than the O arm fix recovers those four arms,
//    which is why the earlier passes could not find them: 14 unused inline
//    helpers, pointer locals for &pos and &out in all four arms, permuting the
//    three pos stores, and every Found() spelling (bool, int, !!, 0 != x,
//    x ? 1 : 0) are all flat or worse
//    (build/scratch/487bf0/{hsweep,ptrsweep,orderweep,gsweep}.py), and
//    uv run tools/headers.py 0x487bf0 --cpp is flat at 76.9% over all 1536
//    sets. msvc5-rtm emits the same bytes, so none of this was a header or a
//    compiler-version effect.
//  - 'D': `processed = 1;` goes after the FUN_0043adc0 call, not before it.
//    With the two fixes above in place that single reorder is the last byte: it
//    moves the D arm's loop-back `jmp` from 0x487e49 to 0x487e50, past the
//    `mov ebp,[esp+0x158]` that only the G arm needs, because the G arm
//    clobbers ebp (ebp holds text). permute.py found it, together with
//    `if (',' == *text)` and a `do { ... } while (0);` around the P arm's
//    sscanf; neither of those two is needed, and this file has neither.
// The 'O' arm reuses one variable for both of its jobs: frame 0x28 holds
// (flags>>18)&3, is the first %d and is read back for the combine, so nothing
// uninitialised is combined. Earlier passes called that a bug in Cavedog's
// code; it is not, our spelling was what read a stale word.
// Frame map read off the exe (L = esp at the loop head, buf at L+0x50): f1
// L+0x10, f2 L+0x14, the strcspn length and B's "%d" L+0x18, pos at L+0x1c,
// L+0x20 and L+0x24, W's "%d" and O's first "%d" L+0x28, the selected flag
// L+0x2c, wf L+0x30, pf L+0x34, O's second "%d" L+0x38, and out.g, out.a,
// out.m, out.u, out.p at L+0x3c, 0x40, 0x44, 0x48 and 0x4c.
// Class_00438760 is trivially copyable (no copy constructor declared). The
// string kinds are implicit conversions (`FUN_0043adc0("WAIT", ...)`): MSVC 5
// builds them straight in the argument slot (`push ecx; mov ecx,esp; push str;
// call ctor`), so a named temporary is wrong. P and W pass `(int)(f * 30.0f)`
// straight to FUN_0043adc0; there is no float local. W-a: `int count =
// sscanf(...); int target = 0; if (count == 1) ...` leaves target in eax.
// The frame order f1 f2 n pos move | selected | wf pf fire | G A M U P outs
// (all dwords from frame 0x00 to 0x3c) is only reached with the locals grouped
// in small structs; MSVC 5 orders loose scalars by its own ranking.

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
    int count;
    struct { float f1, f2; int n; Vec3_00487bf0 pos; int move; } L;
    Outs_t out;
    char buf[256];
    int processed = 0, selected = 0;
    struct { float wf; float pf; int fire; } M2;

    while (*text != 0) {
        if (isspace(*text)) {
            do
                text += 1;
            while (isspace(*text));
        }
        L.n = strcspn(text, ",");
        strncpy(buf, text, L.n);
        text += L.n;
        buf[L.n] = 0;
        if (*text == ',')
            text = text + 1;

        switch (buf[0]) {
        case 'O':
        case 'o': {
            unsigned int* flags = &unit->flags;
            L.move = (unit->flags >> 0x12) & 3;
            M2.fire = (int)((*flags >> 0x14) & 3);
            sscanf(buf + 1, " %d %d", &L.move, &M2.fire);
            unit->flags = (0xffc3ffff & unit->flags)
                          | ((((3 & M2.fire) << 2) | (3 & L.move)) << 0x12);
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
            L.pos.z = (int)(65536.0 * L.f2);
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
                selected = 1;
                processed = 1;
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
                L.pos.z = (int)(65536.0 * L.f2);
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
                count = sscanf(buf + 2, " %[a-zA-Z0-9.]", buf);
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