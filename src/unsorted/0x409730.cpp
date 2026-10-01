// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash 10-minute retry (this session): re-confirmed 1678 bytes
// and 99.6%, exactly the two SIB base/index bytes (0x4099f6 wants [esi + ecx],
// 0x409b53 wants [eax + edx]; ours [ecx + esi] and [edx + eax]). No new probes
// this pass; the earlier notes below close the store-side and read-side routes.
//
// deepseek-v4.1-flash 10-minute pass (build/scratch/0x409730/v1-v10.cpp), the
// two SIB base/index bytes (0x4099f6 `[esi + ecx]` vs ours `[ecx + esi]`,
// 0x409b53 `[eax + edx]` vs ours `[edx + eax]`) are untouched, 1678 bytes and
// 99.6% every time. New negatives: the address ADD probe, forcing the operand
// order through integer arithmetic, is neutral in all four spellings
// (`(unsigned char*)((int)i + (int)vec_8d.begin())` at the store, at the read,
// at both, and the control `(int)vec_8d.begin() + (int)i`), so the front end
// canonicalises the commutative ADD and the tree order is not reachable from
// the source; `i[vec_8d.begin()]` (store only, read only, both) is likewise
// byte-identical at 99.6%, confirming that the source operand order does not
// reach the SIB. Routing either access through a `static` inline helper on a
// raw pointer (`StoreByte(unsigned char*, int, unsigned char)`,
// `LoadByte(unsigned char*, int)`) costs the inline budget: 1683 bytes and
// 83.9% for the store alone, the read alone and both, exactly like the
// reference-form helpers before, so the parameter node does not survive the
// expansion into a real variable. Together with the earlier store-comma result
// this closes the last two routes to a variable pointer node.
// STILL 99.6% (deepseek-v4.1-flash decomp-worker pass): the same two SIB
// base/index bytes remain (0x4099f6 wants [esi + ecx], 0x409b53 wants
// [eax + edx]; ours [ecx + esi] and [edx + eax]), size 1678 exact. New
// negative results, all scored with check.py --sym (build/scratch/0x409730/):
// temp-first comma pointer at the store (`int v8 = max(...); p8 =
// vec_8d.begin(), p8[i] = (unsigned char)v8`) is propagated back and neutral,
// and so is the clamp-first single-statement comma
// (`v8 = max(...), p8 = vec_8d.begin(), p8[i] = v8`): the store flip needs the
// pointer load sequenced BEFORE the clamp, which is exactly the T9 hoist, so
// the flip and the hoist cannot be separated by reordering the statement.
// A second comma pointer at the read (`q8 = vec_8d.begin(), x += (char)q8[i] /
// 2`, and the value-position form `x += (char)(q8 = vec_8d.begin(), q8[i]) /
// 2`) does not cancel the T9 hoist (combined still 1675 bytes, 86.6%) and is
// neutral alone; the address-comma form `*(p8 = vec_8d.begin(), p8 + i) = ...`
// is neutral. Renumbering the inlined operator[] temps via the neighbouring
// accesses (`Elem_0040cfb0* e = vec_65.begin() + i`, `n = (short)*(vec_7d.begin()
// + i)`, both, and combined with the clamp-first comma) is byte-identical;
// splitting the loop declaration (`int i; for (i = 1; ...)`) and dead
// `int z8 = (char)vec_8d[i]` / `vec_8d.size()` expansions at the sites all tip
// the first resize's inlined copy loop (1683 bytes, 83.9%), so this function's
// inline budget sits exactly on a boundary and dead-code numbering probes are
// not free. No spelling tried flips the read SIB at all.
//
// STILL 99.6% (deepseek-v4.1-flash final timebox pass): the two SIB base/index
// bytes at 0x4099f6 (want [esi + ecx], ours [ecx + esi]) and 0x409b53 (want
// [eax + edx], ours [edx + eax]) are still the only diffs; size 1678 exact.
// New negative results this pass (all scored with check.py --sym,
// build/scratch/0x409730/run.py): store as `*(&vec_8d[i])` neutral; read
// through `unsigned char& r8 = vec_8d[i]` in a block neutral (propagated back,
// contradicting the earlier note that the reference form flips); `int idx8 = i`
// copies at either site neutral; comma pointer `p8 = vec_8d.begin(), x += (char)
// p8[i] / 2` at the READ site byte-identical (neutral), while the same trick at
// the STORE site (T9) DOES flip the store SIB to the original [esi + ecx] but
// hoists `mov edx,[esp+0x20]` / `mov esi,[edx+0x91]` above the clamp block,
// 1675 bytes 86.6%; hoisting `int i` to function top 1683 bytes 83.9% still
// swapped; std::vector<signed char> 98.0% still swapped. So the store flip
// always costs the hoist (pointer def floats early once it is a real variable
// node), and no read-side spelling tried flips 0x409b53 at all.
// deepseek-v4.1-flash 10-minute retry (this session): still exactly the two SIB
// bytes at 0x4099f6 (want [esi + ecx]) and 0x409b53 (want [eax + edx]), 1678
// bytes, 99.6%. One new negative: a value local at the store site
// (`char v8 = (char)max(-100, min(100, a)); vec_8d[i] = v8;`) is 1677 bytes and
// 85.1%, so the store SIB flip still cannot be bought without a real pointer
// node and its early hoist. No spelling tried this pass moved either byte.
//
// char>&,int,unsigned char)` helper used only at the store site stays 1678
// bytes and 99.6% with both bytes still swapped; a static `LoadByte(...)` at
// the read site and an `AtByte(...)` reference helper at both sites both blow
// the inline budget (1683 bytes, 83.9%); `std::vector<char> vec_8d` with the
// read as bare `vec_8d[i] / 2` is 98.0%. So neither a helper call boundary nor
// the element type flips the encoding, consistent with the compiler-state
// conclusion below.
//
// STILL 99.6% (deepseek-v4.1-flash timebox pass): nothing this pass moved the
// two SIB base/index bytes (0x4099f6 wants [esi + ecx], 0x409b53 wants
// [eax + edx]; ours [ecx + esi] and [edx + eax]). What this pass tried, all
// scored with check.py --sym (build/scratch/0x409730/), all 1678 bytes with
// the identical two SIB hunks: extended the unused-prototype decl-count sweep
// into the 3000-6000 range (the 0x4b6c30 note in docs/agent-guide.md says
// 2700-5400 unused prototypes can flip base/index): proto3200 98.0%,
// proto3600 99.6%, proto4000 97.6%, proto4400 97.6%, proto4800 99.6%,
// proto5200 98.0%, proto5600 99.2%, proto6000 97.6%, so decl count only moves
// the score bands and the two SIB bytes never flip in any band; and all six
// permutations of the <windows.h>/<math.h>/<vector> include order (99.6% each,
// so header order is neutral even though header sets are not).
//
// deepseek-v4.1 10-minute pass: eleven more variants, every one of them still
// exactly 99.6% with the identical two SIB diffs (the schedule and the size are
// untouched, 1678 bytes): reversing the source operand order of the address ADD
// at both sites (`i[vec_8d.begin()]`, the store as `*(i + vec_8d.begin())`,
// `(&vec_8d[0])[i]`), `this->vec_8d[i]`, the explicit call `vec_8d.operator[](i)`,
// a cast chain through `void*` at both sites, `(&vec_8d.front())[i]`, the read
// with a non-leaf index `vec_8d[(unsigned char)i]` (1685 bytes, 92.9%,
// adds the movzx, SIBs still swapped), reversing the read's outer add to
// `x = (char)vec_8d[i] / 2 + x;`, and a fresh single-use pointer temporary for
// each site (`unsigned char* p8 = vec_8d.begin(); p8[i] = t2;` in its own block,
// value in a temp first). That last one is the informative one: when the pointer
// temporary is initialized after the value is computed, MSVC propagates the load
// back to the use site and the SIB roles stay swapped, so the store-only flip the
// earlier pass saw came from the pointer load being hoisted above the whole
// block, not from the "variable" node as such. tools/headers.py re-run: all 128
// sets, closest 99.6% (`<windows.h> <math.h>`). The two SIB bytes remain the
// whole work list: 0x4099f6 wants `[esi + ecx]`, 0x409b53 wants `[eax + edx]`.
//
// Additional pass (deepseek-v4.1): the read site rewritten as a braced block with
// `unsigned char* p8 = vec_8d.begin(); x += (char)p8[i] / 2;` recompiles to the
// identical 1678 bytes with the identical two SIB diffs (the single-use local is
// propagated back into the subscript), an explicit `(unsigned char)` cast around
// the clamped store value is neutral at 99.6%, and an `unsigned int` loop index
// is much worse (1683 bytes, 83.9%, the loop guard turns into a 64-bit compare).
// The two SIB base/index bytes remain the whole work list.
//
// (deepseek-v4.1-flash): key negative finding. The two swapped accesses are NOT
// a subscript-form problem and NOT a vector-container problem. A named pointer
// local `unsigned char* q = vec_8d.begin(); q[i]` DOES produce the original's
// base order for both sites, but only when it is the ONLY use in the loop. Adding
// the other site back keeps the original base: store via exact-width pointer on
// plain form like `*(&vec_8d[i])`, and `unsigned char* q = vec_8d.begin(); p[i]`
// on the store form. At the read, `unsigned char& r = vec_8d[i]` (an element
// reference) also yields the original base order. HOWEVER every construct that
// fixes one of the two accesses makes the pointer local survive into the vast
// mid-function region (it spills to a stack slot and its reload shifts a dozen
// unrelated instructions, 1679-1690 bytes, 52-85%), whereas the original has no
// such live pointer there. The free scratch scoring (`check.py --sym`) makes
// this cheap to test: the variants are in build/scratch/0x409730/exp/. Remaining
// diff is still exactly the two SIB base/index bytes.
//
// Still 99.6% (space-bunny-free pass): the code is the same 1678 bytes and every
// instruction matches except these two hunks, both the SIB base/index order of a
// byte access to vec_8d (a different SIB byte, not a different instruction):
//   0x4099f6  orig `mov byte ptr [esi + ecx], al`   ours `mov byte ptr [ecx + esi], al`
//             (the store of the clamped rating: base should be the vector pointer)
//   0x409b53  orig `movsx eax, byte ptr [eax + edx]`  ours `movsx eax, byte ptr [edx + eax]`
//             (the `(char)vec_8d[i] / 2` read: base should be the vector pointer)
// What this pass added (all scored with check.py --sym, all still 99.6% or worse):
// - The trigger is NOT the access form but the surrounding function. A 6-line
//   minimal member reproduces both orders: `v[i]` encodes base=index var, index=
//   pointer, while a NAMED POINTER LOCAL (`unsigned char* q = v.begin(); q[i]`)
//   encodes base=pointer, index=index var, for both the byte store and the byte
//   read feeding a signed /2, with no headers involved. But inserting exactly
//   that local pointer into this function (v1-v3, 9 variants: local in a block,
//   value split into a temp first, `char*` cast, unsigned pointer with and
//   without the `(char)` cast, `&v[0]`, `v.begin()`, the read split into
//   `int c` first) leaves both SIB bytes swapped; only the reference form
//   `unsigned char& r = vec_8d[i]; r = ...` changes the code at all, and it is
//   much worse (1679 bytes, 84.6%). So MSVC5's swap decision here is made on the
//   full expression/register-pressure state, not on the subscript.
// - `unsigned char& r = vec_8d[i]` forces the address into a register first
//   (two extra movs, the pointer kept in ebp) and is never right for a
//   single-use subscript.
// - Second space-bunny-free pass, header set is already optimal: adding
//   <string>, <list>, <map>, <set>, <deque>, <algorithm>, <iostream> or
//   <xstring> (before or after <vector>) scores 99.2 / 98.0 / 97.6 / 99.6, never
//   100, so the missing header is not the cause here. `<memory>` and `<new>`
//   are neutral at 99.6%, so they are also free to add.
// - Ten more access forms, all with the identical two-line diff: a named
//   `unsigned char* p8 = vec_8d.begin()` at the top of the loop body is a
//   disaster (51.0%, and 48.8% with the stored value split into a temp), even
//   used at only ONE of the two sites (the read alone 99.6%, the store alone
//   86.6% because the block reshuffles a dozen unrelated instructions).
//   Neutral at 99.6% with the same diff: `vec_8d[(unsigned)i]`,
//   `*(vec_8d.begin() + i)`, an `int k = i` copy for the store, and reading
//   `(char)vec_8d[i]` into a local first. Worse: an `unsigned char` value temp
//   before the store (85.1%), a `unsigned char& Rating(int)` member used at
//   both sites (83.9%, the accessor changes the inline budget), and making
//   vec_8d a `std::vector<char>` so the read needs no cast (98.0%). Binding
//   the vector itself to a reference inside the loop body, `v8[i]`, is 73.7%.
// - So the swap is decided before the subscript is even formed: the register
//   roles are already identical (pointer and index in the same two registers),
//   the definition order of the two registers is inconsistent between the two
//   sites, and the register NUMBERS are inconsistent too, which rules out both
//   definition order and register number as the rule. It has to be the
//   operand-tree order or some per-function state the front end carries.
// - A named pointer local hoists `mov <ptr>, [this+0x91]` to the top of the block
//   when its initializer is written before the value expression (1675 bytes,
//   86.6%); writing the value into a temp first puts it back in place.
//
// deepseek-v4.1 near-miss pass (all scored with check.py --sym, every one of them
// 99.6% or worse, the two SIB bytes unchanged unless noted): the access FORM is
// irrelevant here. Byte-identical 1678-byte results, i.e. both SIBs still swapped,
// for `i * 1` / `1 * i` as the index, `*&vec_8d[i]`, an inline cast pointer
// `((unsigned char*)vec_8d.begin())[i]`, the pointer-to-array forms
// `(*(unsigned char (*)[1])vec_8d.begin())[i]` and
// `((unsigned char (*)[1])vec_8d.begin())[i][0]`, and a 1-byte-struct element
// (`vec_8d[i].value`, 98.0%, which behaves like the char-vector result). Moving the
// /2 inside the cast, `x += (char)(vec_8d[i] / 2)`, changes the read to
// `mov al, [edx+eax]; movsx edx, al` (1677 bytes, 94.1%): the value tree changes,
// the SIB order does not. A `char` cast on the clamped store value is neutral.
// New finding: a pointer whose tree node is a real VARIABLE (assigned by a comma
// expression in the same statement, `p8 = vec_8d.begin(), p8[i] = ...`) does flip
// the store SIB to the original `[esi + ecx]`, but the extra variable reshuffles
// the schedule (1675 bytes, 86.6%, the pointer load hoists and a register moves
// from eax to ebx), and the read site needs braces plus the same trick for the
// 86.6% copy; there is no variant that flips both bytes while keeping the 1678-byte
// schedule. Remaining work list is unchanged: 0x4099f6 `[esi + ecx]` vs
// `[ecx + esi]` and 0x409b53 `[eax + edx]` vs `[edx + eax]`.
//
// GPT-6 retry: rating access, clamp, half-rating and pointer getter helpers, plus
// all 768 header sets, did not improve 99.6%. Remaining differences are still the
// two SIB base/index encodings; accessor wrappers can disturb STL inline budgeting.
// Recomputes a player's per-unit-type tables (the object built by 0x409160):
// resizes the tables at +0x8d and +0x65 to the unit type count, then for each
// unit type rates it into vec_8d[i] and the three bytes of vec_65[i].
//
// Best so far 99.6%: the code is the same length and every instruction
// matches except the base/index order of two byte accesses to vec_8d:
// the original has `mov [esi + ecx], al` (store) and `movsx eax, byte ptr
// [eax + edx]` (the `(char)vec_8d[i] / 2` read), ours encodes [ecx + esi] and
// [edx + eax]. No header set (tools/headers.py plus <ddraw.h>, <string>,
// <map>, <list> and others), access form (begin()[i], *(begin() + i),
// unsigned index), full class layout, or defining 0x409160 and 0x409470
// above this function in the same file changed it. The weapon sum's
// division order also flips with the number of declarations in the file
// (it goes wrong with the full class layout), so both are probably compiler
// state from the rest of the original file.
//
// Retry (deepseek-v4.1-flash) left both bytes unchanged: headers.py --cpp
// (768 sets) all 99.6%, N unused externs and N prototypes swept wide (0-3000)
// produce only two score bands and the same two SIB lines, and rewriting the
// accesses as begin()[i], *(begin()+i), operator[](i), data(), element struct,
// (signed char) / (int) casts and reference bindings all leave the identical
// two-line diff. A minimal function with a member vector reproduces the
// swapped order only when a byte read feeds a signed /2, so the trigger is in
// the expression's value path, not the access itself.
//
// Things that were needed to get here:
// - MSVC 5's inline budget decides which STL calls stay out of line (the
//   first resize() inlines erase() and its _Destroy, the second calls insert
//   and erase out of line). It only matched with the inline Def methods
//   HasField1ce() and Bonus1ce() below, whose inlined calls use up the budget
//   the way the original's did.
// - The last byte is one windows.h min/max expression; the sum
//   `(float)(f18a * -0.02f) + (bonus ? 25 : 0)` is shared between the macro's
//   repeated evaluations (MSVC spills it to [esp+0x14]), while Bonus1ce()
//   is re-evaluated each time. Without the (float) cast MSVC folds the
//   -0.02 into a subtraction.
// - The two flag bits at +0x241 are read from one local copy of the bitfield
//   word (`mov ecx, ebx; shr ecx, 0xb; test cl, 1`).
#include <windows.h>
#include <math.h>
#include <vector>
struct Unit {
    int unknown_0;
};

struct Elem_0040cfb0 {
    char a;
    char b;
    char c;
};

struct Elem_0040d4f0 {
    char value;
};

struct Elem_0040d550 {
    int unknown_0;
};

struct Point16 {
    short x;
    short y;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

#pragma pack(push, 1)
struct Weapon_00409730 {
    char unknown_0[0xd4];
    unsigned short field_d4;           // +0xd4
    char unknown_d6[0xdc - 0xd6];
    int field_dc;                      // +0xdc
    char unknown_e0[0x10a - 0xe0];
    char field_10a;                    // +0x10a
};

struct Flags241_00409730 {
    unsigned int bits_0 : 6;
    unsigned int flag_6 : 1;           // bit 6
    unsigned int bits_7 : 4;
    unsigned int flag_11 : 1;          // bit 11
    unsigned int bits_12 : 12;
    unsigned int flag_24 : 1;          // bit 24
    unsigned int bits_25 : 7;
};

struct Def_00409730 {
    int HasField1ce() { return field_1ce != 0.0f; }
    int Bonus1ce() { if (field_1ce != 0.0f) return 100; return 0; }
    char unknown_0[0x186];
    float field_186;                   // +0x186
    float field_18a;                   // +0x18a
    char unknown_18e[0x1c0 - 0x18e];
    short field_1c0;                   // +0x1c0
    float field_1c2;                   // +0x1c2
    char unknown_1c6[0x1ce - 0x1c6];
    float field_1ce;                   // +0x1ce
    float field_1d2;                   // +0x1d2
    char unknown_1d6[0x1ee - 0x1d6];
    Weapon_00409730* weapons[3];       // +0x1ee
    char unknown_1fa[0x204 - 0x1fa];
    short field_204;                   // +0x204
    short field_206;                   // +0x206
    char unknown_208[0x22d - 0x208];
    char field_22d;                    // +0x22d
    char unknown_22e[0x241 - 0x22e];
    Flags241_00409730 flags_241;       // +0x241
    unsigned int bits_245_0 : 4;
    unsigned int flag_245_4 : 1;       // bit 4
    unsigned int bits_245_5 : 3;
    unsigned int flag_245_8 : 1;       // bit 8
    unsigned int bits_245_9 : 23;
};

struct Game_00409730 {
    char unknown_0[0x1425f];
    int field_1425f;                   // +0x1425f
    char unknown_14263[0x1434f - 0x14263];
    unsigned short field_1434f;        // +0x1434f
    char unknown_14351[0x1438f - 0x14351];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_00409730* defs;                // +0x1439b
    char unknown_1439f[0x37ec8 - 0x1439f];
    int field_37ec8;                   // +0x37ec8
};

struct Player_00409730 {
    char unknown_0[0x144];
    unsigned short field_144;          // +0x144
};

struct UnitList_00409730 {
    std::vector<Unit*> units;
};

class Class_00409730 {
public:
    Player_00409730* player;           // +0x00
    unsigned char index;               // +0x04
    UnitList_00409730 list_5;          // +0x05
    UnitList_00409730 list_15;         // +0x15
    UnitList_00409730 list_25;         // +0x25
    int pos_35[3];                     // +0x35
    int pos_41[3];                     // +0x41
    std::vector<Elem_0040cc40> vec_4d; // +0x4d
    short centerX;                     // +0x5d
    short centerY;                     // +0x5f
    int field_61;                      // +0x61
    std::vector<Elem_0040cfb0> vec_65; // +0x65
    int field_75;                      // +0x75
    int field_79;                      // +0x79
    std::vector<short> vec_7d;  // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<unsigned char> vec_9d; // +0x9d

    void FUN_00409730();
};
#pragma pack(pop)

extern Game_00409730* g_game;

float __stdcall FUN_00488f30(Def_00409730* def);

// deepseek-v4.1-flash timebox pass: two new shapes scored at the two remaining SIB
// bytes, neither helps. A function-scope `std::vector<unsigned char>& v8d = vec_8d;`
// reference local used at both sites keeps the pointer live across the whole function
// and drops to 1679 bytes, 73.7%. An explicit `(signed char)` cast at the read
// (`x += (signed char)vec_8d[i] / 2;`) is byte-identical at 99.6% (1678 bytes), so the
// read SIB is not a cast-spelling artefact either.
// FUNCTION: 0x409730
void Class_00409730::FUN_00409730()
{
    vec_8d.resize(g_game->count, 0);
    {
        Elem_0040cfb0 e;
        e.a = 0;
        e.b = 0;
        e.c = 0;
        vec_65.resize(g_game->count, e);
    }
    for (int i = 1; i < g_game->count; i++) {
        Def_00409730* def = &g_game->defs[(unsigned short)i];
        int a = 1;
        if (def->HasField1ce())
            a = 11;
        if (def->field_22d)
            a += 10;
        if (FUN_00488f30(def) < 0.0f)
            a += 10;
        int t = (int)(a - def->field_18a * -0.01f);
        a = (int)(t - def->field_186 * -0.002f);
        int b = 1;
        if (def->flag_245_4)
            b = 11;
        for (int w = 0; w < 3; w++) {
            Weapon_00409730* wp = def->weapons[w];
            if (wp->field_10a)
                b += wp->field_dc / 100 + wp->field_d4 / 40 + 5;
        }
        a += (char)max(-100, min(100, b));
        vec_8d[i] = max(-100, min(100, a));

        a = 1;
        Elem_0040cfb0* e = &vec_65[i];
        int n = (short)vec_7d[i];
        if (def->flag_245_4)
            a = 21;
        if (def->flags_241.flag_6 && n < 3)
            a += 30;
        if (FUN_00488f30(def) < 0.0f)
            a += 50;
        if (def->HasField1ce())
            a += 50;
        if (def->field_22d)
            a += 25;
        Flags241_00409730 flags = def->flags_241;
        if (flags.flag_11)
            a += 40;
        if (def->field_206)
            a += 15;
        if (def->field_204)
            a += 5;
        int x = (int)(a + min(max(def->field_1c2, 0.0f), 30.0f));
        if (n == 0)
            x *= 4;
        if (n == 1)
            x *= 2;
        if (def->field_1c0 >= 0)
            x *= 3;
        if (player->field_144 > (unsigned short)(g_game->field_1434f / 2))
            x += (char)vec_8d[i] / 2;
        if (def->flag_245_8)
            x = 0;
        if (flags.flag_24)
            x = 0;
        if (def->field_1d2 != 0.0f && g_game->field_1425f < g_game->field_37ec8 / 2)
            x = 0;
        x = min(x, 100);
        e->a = x;
        e->c = (char)max(0.0f, min(100.0f, def->field_186 * -0.0025f - FUN_00488f30(def) * 5.0f));
        e->b = (char)max(0.0f, min(100.0f, (float)(def->field_18a * -0.02f) + (def->field_22d ? 25 : 0) + def->Bonus1ce()));
    }
}
