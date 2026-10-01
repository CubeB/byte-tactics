// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 65.9% (ours 1859 bytes vs 1811), improved from 62.1% by deepseek-v4.1-flash.
// IMPROVEMENT (deepseek-v4.1-flash): the P arm's own `int z` and the W arm's
// shadowed `int n` were the one extra stack dword. The original reuses the
// function-scope `fire` for both `_ftol` results (P at frame 0x28, 0x487f12;
// W at frame 0x28, 0x488166) and the strcspn `n` for the W "%d" (frame 0x8).
// Writing `fire = (int)(x * 30.0f)` in both arms and `n = 0; sscanf(buf + 1,
// " %f %d", &f1, &n)` with no shadow brings the frame to exactly 0x140, the
// original's, and 62.1% -> 65.9%.
// STILL DIFFERENT: the copy-ctor declaration is the last big defect; it emits
// `lea eax,[out]/mov ecx,esp/push eax/call copy-ctor` at the five FUN_0043f0e0
// -> FUN_0043adc0 handoffs where the original reloads the raw dword
// (`mov ecx,[esp+0x48]` etc). Measured on top of the frame fix: trivial class
// alone 54.1% (1835 bytes); trivial class plus five function-scope objects in
// the original's slot order 54.1% (1835 bytes); reusing function-scope
// variables for the case locals (move for target, fire for the A/B ids, f1 for
// the W float) 20.2%. So the copy-ctor declaration stays, and the case locals
// really are separate slots. Frame sizes of the losers: 0x14c and 0x150.
// FINDINGS (deepseek-v4.1-flash, retry): measured the ORIGINAL's exact frame via
// per-case esp-tracking (reset esp=WESP=-0x10 at each switch entry). Original
// frame is 0x140 = EXACTLY 16 dwords at esp0+0x00..0x3f then buf at esp0+0x40
// (256 bytes, ends 0x140). Slot map (correcting the older notes):
//   0x0 f1, 0x4 f2, 0x8 n, 0x0c..0x14 pos.x/y/z, 0x18 move, 0x1c selected,
//   0x20 wfloat, 0x24 pfloat, 0x28 fire,
//   0x2c G-out, 0x30 A-out, 0x34 M-out, 0x38 U-out, 0x3c P-out.
// KEY: there are FIVE distinct FUN_0043f0e0 out slots (0x2c,0x30,0x34,0x38,0x3c),
// not four (older note missed G at 0x2c). And the P arm has only ONE float f3 at
// 0x24: 0x487eb5 `mov [esp+0x48],0` zero-inits the SAME 0x24 slot sscanf's3rd
// "%f" writes (both resolve to esp0+0x24 with 5 pushes pending), so older item 3
// ("two distinct floats") is wrong. Also confirmed the string temporaries
// (D/S/W/B/A-else/tail) ARE elided into the arg slot in BOTH the copy-ctor and
// trivial versions via the string ctor (`mov ecx,esp; push str; call 0x438760`),
// so the copy-ctor declaration is NOT needed for those. The ONLY thing the copy
// ctor breaks is the named `out` pass: instead of the original's raw
// `mov ecx,[esp+..]; push ecx` (M 0x487d65, U 0x487ddd, G 0x487e35, P 0x487f12,
// A 0x487f96) it emits `lea eax,[out]; mov ecx,esp; push eax; call copy-ctor`
// at the five f0e0->adc0 handoffs. Verified with objdump of our two builds:
// copy-ctor build frame=0x144 (one extra dword, likely a copy temp), trivial
// build frame=0x150 (four extra dwords) and buf shifts up by 0x10, so the block
// ints (target/z/id/f) fail to share the 0x0..0x28 scalar slots the way the
// original merges them. CORE GOAL restated: trivially copyable class (raw
// out-pass) AND frame exactly 0x140 with the 16-dword map above AND the 5 out
// objects landing at 0x2c..0x3c. Ideas NOT yet tried (out of timebox): declare
// exactly five function-scope trivially-copyable Class_00438760 in slot order
// (g_out,a_out,m_out,u_out,p_out) after the11 scalars, and force the block ints
// to alias scalar slots via a union or shared declarations; and per-case type
// split using an operator= based handoff. 2 check.py runs this session (62.1%
// main, 49.7% trivial scratch); best stays 62.1%.
// RETRY NOTES (deepseek-v4.1-flash): measured three variants against the original
// with check.py --sym. (a) Deleting the Class_00438760 copy-constructor
// declaration emits no copy calls and drops the code to 1807 bytes (the original
// is 1811, the closest of anything tried) but the frame grows from 0x144 to
// 0x150 and the score falls to 49.7%: with a trivially copyable class MSVC 5
// spreads the five case-local `out` objects over 7 stack slots (0x40..0x5c)
// instead of the original's 4 (0x40..0x4c). (b) Same but with one shared
// function-scope `out`: frame is then exactly 0x140 (the original's) and the code
// is still 1807 bytes, but the score is 57.8% because the scalar slots below it
// no longer line up. (c) Making FUN_0043f0e0 return Class_00438760 by value and
// nesting it as FUN_0043adc0's first argument: 1795 bytes, 42.9%, much worse (no
// RVO into the argument slot; the original really does build into a named local
// and then reload it). P scanning from buf+5 rather than buf+1 is exactly
// neutral in every variant, confirming the earlier note. The remaining gap is
// the copy constructor and the 5-slot class-temporary layout; the copy ctor
// declaration is what keeps the 0x144 frame AND the 62.1% layout, so removing it
// trades one defect for a worse one.
// WHAT CHANGED (space-bunny-free): the strcspn length and the B case's "%d" are
// ONE variable, not two. The original stores the strcspn result at esp0+8 and the
// B case's initial "n = 1" and its sscanf "%d" also land on esp0+8 (0x487c4f,
// 0x488009, 0x488088, 0x4880e1/0x4880f5), so a single function-scope `int n`
// does both. That drops one live variable, `text` wins the last callee-saved
// slot (ebp) instead of losing it to `count`, and the loop latch moves back to
// its right place: the emitted block order is now
//   prologue, isspace, strcspn/strncpy, dispatch, O, M, U, G,
//   latch, after-loop, P, A, B, W, D, S, I
// which is exactly the original's. Every per-case body matched before only by
// luck of the diff; now the frame, the four Class_00438760 slots and the
// strncpy/strcspn sequence all line up.
// STILL WRONG, in rough order of size:
//   1. The five cases that build a Class_00438760 with FUN_0043f0e0 (M, U, G,
//      P and the A-with-floats arm) still emit an extra `lea eax,[...]/mov
//      ecx,esp/push eax/call copy-ctor` pair before FUN_0043adc0, because the
//      copy constructor is declared. The original never calls it: it reads the
//      4-byte object straight out of the FUN_0043f0e0 result slot (0x487d65
//      `mov ecx,[esp+0x48]`, 0x487e35 `mov edx,[esp+0x44]`, 0x487f12
//      `mov ecx,[esp+0x50]`, 0x487f96 `mov edx,[esp+0x44]`), so the class is
//      trivially copyable and the declaration is simply wrong.
//      Deleting the declaration is NOT a net win, though, and the reason is
//      measured: the frame grows from 0x144 to 0x150 (pos moves 4 bytes down to
//      esp0+8 and the Class slots move from esp0+0x48.. to esp0+0x48... all the
//      way past the original's esp0+0x30..0x3c), which drops the score to
//      49.7% even though the total size drops to 1807, the closest of anything
//      tried. Declaring the copy ctor INLINE with a foldable body
//      (`: index(other.index) {}`) still emits the same out-of-line call pair,
//      exactly 62.1% and 1847 bytes, so that is not the way out either.
//      Re-ordering the function-scope declarations to the original's slot order
//      (f1, f2, n, pos, move, selected, the two block floats, fire, all hoisted
//      to function scope) is also exactly neutral at 62.1% and 1847 bytes, so
//      MSVC 5's local slot assignment here is not driven by declaration order
//      and item 2 below has to be attacked some other way.
//   2. The frame is 0x144, the original is 0x140, and the local slots run in a
//      different order. Original: f1 esp0+0, f2 esp0+4, n esp0+8, pos
//      esp0+0x0c..0x14, move esp0+0x18, selected esp0+0x1c, the W case's
//      float esp0+0x20, the P case's third float esp0+0x24, fire esp0+0x28,
//      the four Class slots esp0+0x30/0x34/0x38/0x3c, buf esp0+0x40..0x13f.
//      Ours: selected esp0+0x18, fire esp0+0x20, move esp0+0x28, Class slots
//      from esp0+0x40, buf esp0+0x44. So `move` and `fire` are allocated in
//      the opposite order; MSVC 5 assigns local slots by declaration order,
//      and the original's order (move, selected, two floats, fire) is a
//      declaration order our source does not have.
//   3. Slot +0x20 (ours) / +0x28 (original) is `fire`, yet the P case
//      zero-initialises a float there (0x487eb5 `mov dword ptr [esp+0x48],0`)
//      while sscanf writes its third "%f" to esp0+0x24 (0x487e9c `lea eax,
//      [esp+0x34]`). So the original really does keep two distinct floats in
//      the P arm, not one `f3 = 0.0f`.
//   4. The original's P arm scans from `buf + 5` (0x487eab `lea eax,[esp+0x5d]`)
//      where we scan from `buf + 1`. Changing ours to buf + 5 scored exactly
//      the same 62.1%, so it is free but not yet load bearing.
//   5. The original's P arm is also missing one push we emit (it has 5 pushes,
//      we have 6), so one of its "%f" arguments is computed but not passed.
//   6. In the original the M arm passes `esp0+0x10` (= &pos.y) as FUN_0043adc0's
//      5th argument while U, P and A pass `esp0+0x0c` (= &pos), which looks
//      like an off-by-one-struct in Cavedog's own code rather than a construct
//      we can write.
//   7. Our A, B and W arms tail-merge their if/else sub-arms into the parent;
//      the original keeps them as separate blocks (A's else at 0x487fb3, B's
//      "w" arm at 0x4880e1, W's "a" arm at 0x488184).

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
    int n;
    int selected = 0;
    int processed = 0;

    while (*text != 0) {
        while (isspace(*text))
            text++;
        n = strcspn(text, ",");
        strncpy(buf, text, n);
        text += n;
        buf[n] = 0;
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
            fire = (int)(f3 * 30.0f);
            FUN_0043f0e0(&out, 9, unit, 0, &pos);
            FUN_0043adc0(out, 1, unit, 0, &pos, fire, 0);
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
            n = 1;
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
                n = 0;
                sscanf(buf + 1, " %f %d", &f, &n);
                fire = (int)(f * 30.0f);
                FUN_0043adc0(Class_00438760("WAIT"), 1, unit, 0, 0, fire, n);
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
