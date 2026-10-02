// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// PARTIAL 76.9% (ours 1815 bytes, the original's 1811). Notes from the
// claude-sonnet-5-5 pass:
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
// SUSPECTED ORIGINAL BUG: 'O' builds bits 18-19 of unit->flags from the
// frame-0x28 word, which that arm never writes. That word is the one 'W'
// parses its %d into, so 'O' combines an uninitialised local into the flags,
// and the second number it parses is thrown away (it lands in the frame-0x18
// word, which nothing reads afterwards). Kept as the original has it.
// Space Bunny Free pass. build/scratch/0x487bf0/cmp.py diffs the .dis of a
// scratch variant against the exe's arm by arm (447 of 534 instructions now
// agree); that plus permute.py took this from 67.7% to 76.9%. What changed:
//  - 'O': the arm passes &(frame 0x18) as the first %d and &(frame 0x38) as the
//    second, and combines the frame-0x38 and frame-0x28 words, so the
//    (flags >> 18) initialiser belongs to the frame-0x18 word (the one 'B'
//    counts into) and the two initialisers must be written high-pair first for
//    MSVC to shift >>18 first.
//  - the isspace skip has to be `if (isspace(*text)) { do text += 1; while
//    (isspace(*text)); }`, and `count` has to be a function-scope local.
//  - `Found()` below is a codegen crutch, not Cavedog's spelling. Putting the
//    G-arm test through one small inlined predicate is what fixes the P, A and
//    B arms, which share the register allocation the G arm sets up: a `bool`
//    local in the same place does not (65.2%), an int-returning helper does not
//    (68.4%), and moving the whole if into a helper does not (68.4%). It costs
//    three instructions in 'G' (xor eax,eax; setne al; test al,al), so ours is
//    1815 bytes against the original's 1811.
// STILL DIFFERENT, register and scheduling noise in four arms:
//  - 'O': the original loads unit->flags twice with nothing between them
//    (mov edx,[esi+0x110]; mov eax,[esi+0x110]) and computes the address of
//    the second sscanf argument before the first; ours folds the pair into one
//    load plus `mov edx,eax` and takes the addresses the other way round. No
//    non-volatile spelling reproduces the double load: rewriting the shifts
//    (build/scratch/0x487bf0/v4.cpp, v5.cpp), reading the fields as bitfields
//    (v16.cpp), giving each read its own static inline helper (v6.cpp) or one
//    helper taking the shift (v26.cpp) all still fold. Only `volatile` does
//    it, and an earlier pass measured that at 92.7% from this file (its
//    build/scratch/0x487bf0/t5.cpp is gone; it is this code without Found(),
//    with `volatile unsigned int flags`). Worth a decision from the lead: this
//    is the only field in the function that re-reads without a barrier.
//  - 'M' and 'U': same source shape as the (now matching) P and A arms, but the
//    original computes &pos and &out before the argument pushes and sinks the
//    pos.z store below them, where ours does the opposite in both, and ours
//    starts the register rotation one step earlier (edx,eax,ecx against the
//    original's ecx,edx,eax).
//  - 'G': register rotation only (edx,eax,ecx against the original's
//    eax,ecx,edx), plus the three instructions Found() costs.

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

// Codegen crutch, see the note at the top: the test the G arm makes after
// looking a name up in the script's table.
static inline bool Found(int target) { return target != 0; }

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
            M2.fire = (unit->flags >> 0x14) & 3;
            L.n = (unit->flags >> 0x12) & 3;
            sscanf(buf + 1, " %d %d", &L.n, &M2.fire);
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
            if (Found(target)) {
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
            processed = 1;
            FUN_0043adc0("SELFDESTRUCTFG", 1, unit, 0, 0, 1, 0);
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