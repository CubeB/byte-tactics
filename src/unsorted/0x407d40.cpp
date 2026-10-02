// Decompiled by GPT-6, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// MATCH (296 bytes). It took tools/permute.py 0x407d40 --jobs 4 --minutes 10
// --seed 11 (1066 candidates, 3.4 min) after an hour of hand work that stayed
// at 98.9%; the winning chain ends in two temp_intro and two swap_commutative
// steps inside Vec3_00407d40's Game constructor, and the hand-tidied version
// below is what check.py reports MATCH on. What the last percent needed:
// `pop edi` has to land before c's three stores instead of between c.y and
// c.z, which takes three to five more IL nodes before the tail than the
// byte-exact prologue has, and the node that supplies them is in the vector's
// constructor, not in the constructor below: `int ax;` declared on its own and
// then assigned in a separate statement from a `double` local. Both halves as
// one expression, `int ax = (int)(game->baseX / 2 * 65536.0);`, puts the pop
// back between c.y and c.z, and so does every other spelling tried, which is
// why the wall below held for a day.
// The class still declares no virtual functions (its vtable slot is a plain
// field at +0) and the constructor still stores both vtables by hand: the
// matched 0x407a90, a real derived class of the same base, keeps only its own
// vtable store because the inlined base constructor's is dead, while the
// original here stores 0x4fc980 early and 0x4fc9a0 late, so both stores come
// from source. Every shape that declares the real virtual classes scores at
// most 97.8% (see the notes below for why).
//
// deepseek-v4.1-flash retry (2026-09-30): confirmed 97.8%. The remaining diff
// is purely the schedule of the derived vptr store, which lands after b's
// stores here and after the sixth _ftol in the original. Tried: field_38 before
// temp (95.5%), c before field_38 (95.5%), comma forms `c = temp, field_38 = 0`
// (95.5%) and `field_38 = 0, c = temp` (97.8%, same as below), explicit pc
// pointer (97.8%), and a 16-byte Vec4 third member (91.6%, emits lea edx plus
// xor ecx for the zero word). Nothing moved the vptr store later; left at 97.8%.
//
// The original's last instructions are: lea ecx,[esi+0x2c]; mov [esi+0x38],ebp;
// mov [esi],0x4fc9a0; pop edi; mov [ecx],ebx; mov [ecx+4],ebp; mov [ecx+8],eax;
// mov eax,esi. So field_38's store and the derived vtable store are in front of
// the third vector's struct copy, and that copy's first store is NOT folded to
// [esi+0x2c] (2 bytes, so the whole function is 296). The two shapes that reach
// each half of that were measured here:
// - Full initialiser list (a(g_game), b(g_game), c(g_game), field_38(0)) puts
//   both field_38's store and the vtable store exactly where the original has
//   them, but the copy is left unfolded next to its lea: 95.5%, 297 bytes.
// - All three vectors assigned in the body scores 98.3% and is NOT a legal
//   match: with no member initialiser the derived vtable store lands on top of
//   the base's, the base store is then dead and disappears, and the function
//   comes out 291 bytes / 88 instructions with one vtable store missing.
// So the source needs field_38 in the initialiser list (only then does MSVC put
// the vtable store after it) and the third vector's struct copy after that store,
// which no initialiser list produces. All 128 header sets from tools/headers.py
// score 97.8% on the version below.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
};
#pragma pack(pop)

extern Game* g_game;

struct Vec3_00407d40 {
    int x, y, z;

    Vec3_00407d40() {}
    // The declaration of ax, the double and the assignment are three separate
    // statements on purpose: folding them into `int ax = (int)(game->baseX / 2
    // * 65536.0);` moves the constructor's `pop edi` back between c.y and c.z
    // and the function stops matching (see the notes at the top).
    Vec3_00407d40(Game* game) {
        int ax;
        double halfX = ((double)(((game->baseX / 2) * 65536.0)));
        ax = (int)halfX;
        *this = Vec3_00407d40(ax, 0, (int)((game->baseY / 2) * 65536.0));
    }
    Vec3_00407d40(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

struct Class_00408cb0 {                // the owner (constructor 0x408cb0)
    char unknown_0[4];
    unsigned char field_4;             // +0x4
};

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class Class_00407350 {
public:
    Class_00408cb0* owner;             // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10

    Class_00407350(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0
    virtual ~Class_00407350() {}                    // slot 1
};

extern void* DAT_004fc980[];
extern void* DAT_004fc9a0[];

class Class_00407d40 {
public:
    void* vptr_slot;                   // +0x0
    Class_00408cb0* owner;             // +0x4
    void* field_8;                     // +0x8
    int field_c;                       // +0xc
    unsigned int field_10;             // +0x10
    Vec3_00407d40 a;                   // +0x14
    Vec3_00407d40 b;                   // +0x20
    Vec3_00407d40 c;                   // +0x2c
    int field_38;                      // +0x38

    Class_00407d40(Class_00408cb0* p, void* q);
};

Class_00407350::Class_00407350(Class_00408cb0* p, void* q)
    : owner(p), field_8(q), field_c(0), field_10(p->field_4) {}

// Partial (77.5%): everything up to the last _ftol matches. The original then
// does `lea ecx, [esi+0x2c]`, stores field_38 and the derived vtable, pops edi
// and only then stores c through ecx (`mov [ecx], ebx` unfolded). Here c's
// stores come first and the scheduler folds the first one to [esi+0x2c], so
// this version emits 297 bytes instead of 296.
//
// Notes from a second attempt (Claude Opus 5.5):
// - The `lea reg, [esi+K]` store pattern only appears for an implicit struct
//   copy into an inlined constructor's `this` (`*this = Vec3(...)`). Field
//   initialisers, a user operator=, a Set() helper or a by-value helper
//   returning Vec3 give direct [esi+K] stores and no lea.
// - The scheduler folds `[ecx]` into `[esi+0x2c]` only when nothing can fill
//   the slot after the lea; in the original the field_38 and vtable stores
//   fill it, so they must come before c's copy in the compiler's input, while
//   c's two _ftol values are computed before field_38 is stored.
// - No variant reproduced that order: c assigned in the body (the vtable store
//   then precedes c's g_game loads), c(Vec3(g_game)), an explicit copy
//   constructor, a trivial destructor, a named temporary, member order in the
//   list, c plus field_38 as one member struct (stores fold, no lea), self
//   assignments (removed entirely), every header set (tools/headers.py) and
//   the RTM compiler all keep c's copy before field_38 or lose the lea.
//
// Notes from deepseek-v4.1-flash (2026-09-29): all remaining candidates were
// compiled with tools/wcl and inspected. c assigned in the body (with or
// without a named temporary), through `Vec3* pc = &c`, through a
// `(char*)this + offset` pointer, and through an inline helper taking
// `Vec3*` all still emit the first c store as `mov [esi+0x2c], ebx`, and the
// field_38/vtable stores move to before the g_game loads instead of between
// the lea and the stores. Written-order init lists (`field_38(0), c(g_game)`)
// are normalised back to declaration order. All 128 header sets from
// tools/headers.py score 95.5%. Not reproducible from source in the timebox.
//
// Notes from deepseek-v4.1 (2026-09-30): the `mov [ecx], ebx` versus
// `mov [esi+0x2c], ebx` choice is an ADJACENCY peephole, proven by experiment:
// make the third member a 16-byte struct built with `*this = C4(ax,0,az,0)`
// and the emitted tail is `lea edx,[esi+0x2c]; xor ecx,ecx; mov [edx],ebx`
// (store NOT folded, because xor ecx,ecx sits between the lea and the store).
// So an intervening node kills the fold, and the original's IR must have the
// [esi+0x38] store between the lea and the first c store. Nothing that emits
// it there was found: the full init list (a,b,c,field_38) puts the stores and
// pop edi exactly right but the store adjacent to the lea folds (297 bytes),
// field_38 in the list with c in the body unfolds nothing (flag/vptr stores
// land before the c compute), and comma/parenthesised single statements
// (`field_38 = 0, c = Vec3(g_game)`, `c = Vec3(g_game), field_38 = 0`,
// `field_38 = (c = Vec3(g_game), 0)`) all score 95.5%.
//
// deepseek-v4.1-flash second retry (2026-10-01): still 97.8%. Confirmed the
// derived vptr store is emitted at the end of the member init list, so it only
// lands after field_38 when field_38 is an init-list member; but then field_38
// itself is emitted right after b, before the temp, never after the sixth
// _ftol. Tried init lists a,b,field_38 with body c=Vec3(g_game) and with a
// named temp; a,b,c(Vec3(g_game)) with field_38 in the list or body; a user
// copy constructor on Vec3 with the full list; `c = Vec3_00407d40(temp)`;
// pointers/references to c before and after field_38; field_38 before the
// temp. Every 296-byte shape keeps the vptr after b; every shape that moves
// the vptr later is 297 bytes. Kept the best (97.8%).
//
// Notes from space-bunny-free: also tried, all worse than the version below.
// The vector's own constructor rewritten to assign x, y and z separately drops
// the `lea` altogether (64.7%); a two-int constructor
// `Vec3(int ax, int az) { *this = Vec3(ax, 0, az); }` called with inlined
// `HalfX(g_game), HalfZ(g_game)` helpers (so the two __ftol calls are the
// constructor's arguments, not its body) still scores 95.5%, as do
// `c(Vec3_00407d40(g_game))` through the copy constructor, a single
// `Vec3 a, b, c;` declaration, `short field_38`, a `short`/`void*` g_game, a
// self assignment `c = c;` in the body, a dead `field_c = 0;` in the body, and
// `c = Vec3_00407d40(g_game)` with field_38 in the list. The copy is emitted
// unfolded (through ecx) only when the compiler's input has something between
// the `lea` and the stores, and the only IR node that can go there is the
// derived vtable store, which MSVC emits before the body.
// deepseek-v4.1-flash third retry (2026-10-01): still 97.8%, 296 bytes. New
// negatives: explicit `this->field_38 = 0;` and a raw-layout twin struct copied
// through `c = *(Vec3_00407d40*)&raw` are byte-identical to the file (the twin's
// stores through the cast pointer do not break the fold either); making the
// store depend on the temp (`field_38 = temp.y;`) is byte-identical, and on an
// already-computed member (`field_38 = a.y;`) is 92.7% / 299 bytes. The vptr
// store still lands after b's stores; the late placement stays out of reach.
//
// deepseek-v4.1-flash fourth retry (2026-10-01): confirmed 97.8% with the old
// file; the notes above still hold. Proved with `/Op` listings that the wanted
// codegen order (compute, lea, field_38, vptr, stores) is reachable, but the
// /O2 scheduler moves the lea down to its first use whenever the gap between
// the lea and the first c store holds two or more instructions; with a gap of
// one it keeps the lea in place. The vptr store is emitted at the end of the
// member init list, so a vptr store after field_38 needs field_38 in the init
// list, which pushes the c compute into the body, and the scheduler never
// moves that compute (it contains a call) above the vptr store.
//
// Fifth retry (2026-10-01), reduced test cases in build/scratch/0x407d40/
// mini*.cpp (compiled with tools/wcl, read from the /Fa listing):
// - mini9: a class with no base, `: a(g), b(g)` plus body `V temp(g); f = 0;
//   *(void**)this = 0x4fc9a0; c = temp;` produces EXACTLY the wanted tail
//   (lea, f, vptr, pop edi, c.x, c.y, c.z), so the wanted schedule exists.
// - mini12: mini9 plus a hand store of 0x4fc980 to [this] right before the
//   temp also matches.
// - mini11: mini9 with a base class that has NO virtuals (no vptr store)
//   keeps the lea but puts pop edi between c.y and c.z.
// - mini10/mini14/mini15 and the real-base hand-store variant sink the lea
//   and put pop edi after c.x: any base whose ctor stores a vtable in front
//   of the a/b computes moves the lea, and the original has exactly that
//   store at 0x407d61. The two halves of the wanted tail are reachable
//   separately but not together with a real virtual base class here.
// - A 16-byte Vec4 third member always emits `lea edx` plus `xor ecx, ecx`
//   for the zero word, never the ebp the original uses (91.6%).
// The file below is the all-body form (no initialiser list) with the two
// vtables stored by hand and the vptr slot declared as a plain field at +0.
// It gets the lea, field_38 and vptr store positions right; only pop edi is
// two instructions late (between c.y and c.z). Perturbations that did NOT
// move pop edi: c copy via `c = Vec3(temp)`, `c = *(Vec3*)&temp`, a pointer
// or reference to temp, writing field_38/vptr through `(char*)this + K`,
// comma expressions for the pair, a scope around the pair, `const` temp,
// temp assignment instead of construction, one temp per vector, and an extra
// statement after the copy. Adding any extra instruction after the vptr
// store does move pop edi, but it also changes the bytes.
//
// space-bunny-free (2026-10-01, 98.9% kept): permute.py 0x407d40 --jobs 4
// --minutes 15 found no improvement at all (build/permute/0x407d40/best.cpp
// is byte-identical to the file below, best.json unchanged), so this version
// stays. New measurements, all compiled with tools/wcl and read from /Fa:
// - `pop edi` sits between c.y and c.z in EVERY variant of the all-body form
//   with the full five-statement prologue (which is 89 instructions, the
//   original's count). Dropping any single prologue statement moves it much
//   further: without the base vtable store or without `field_c = 0` it lands
//   after `mov eax, esi`; with only `owner`/`field_8` it lands after the
//   derived vtable store (one step too early, which is the p_intro-like
//   result). So its position is a step function of the total instruction
//   count of the ctor, not of the tail's own shape, and the all-body form is
//   already at the count the original has.
// - Introducing the prologue values through locals before the stores
//   (`void* t = q; unsigned char f4 = p->field_4; owner = p; field_8 = t;
//   ...`) is the one perturbation that puts `pop edi` exactly where the
//   original has it (right after the derived vtable store, before c.x), but
//   it also adds `push ecx`, an `and eax, 255` and stack slots, and moves the
//   g_game loads ahead of the prologue stores: far too many bytes. The wanted
//   schedule is reachable, but only with a longer prologue than the original.
// - `c` assigned field by field from the temporary (`c.x = temp.x; c.y =
//   temp.y; c.z = temp.z;`) drops the `lea` entirely and emits three direct
//   [esi+44]/[esi+48]/[esi+52] stores, 88 instructions, confirming again that
//   only a whole-struct copy produces the `lea ecx` / [ecx+4] / [ecx+8] shape.
// - a, b and c all in the initialiser list, or just a and b, or the
//   initialiser list plus a body tail, all score the same 98.9% body or worse;
//   the file below is still the best.
// space-bunny-free (2026-10-02, the 98.9% state, kept for a further hour
// before the permuter matched it). New
// measurements, all compiled with tools/wcl and read from the /Fa listing in
// build/scratch/0x407d40/ (the shape is reported as L=lea ecx, P=pop edi,
// S=one of c's three stores):
// - The tail is INVARIANT. Thirty-odd body-level rewrites all produce a
//   listing identical to the one below, pop edi still between c.y and c.z:
//   all six orderings of the last three statements (only the file's order
//   keeps field_38 before the vtable store, and it scores 98.9%), a scope,
//   `if (1)`, `do {} while (0)`, a comma statement, `this->` everywhere,
//   c via a pointer, a reference, a cast, a `Vec3 temp2(temp)` double copy,
//   a conditional expression, Vec3 with a user-declared destructor, Vec3's
//   ctor as a member initialiser list, and the whole body wrapped in
//   `static inline` helpers (prologue, a and b, the tail, the copy, an empty
//   Nop) at six different points. The Identity(v) { return v; } trick is a
//   no-op here: /O2 deletes it and the listing is unchanged.
// - The 31 prologue subsets (s*.cpp: owner/field_8/field_c/field_10/vtable
//   store) fall into three shapes, A = L P S S S (the wanted one, pop right
//   after the derived vtable store), B = L S S S P (pop after all three c
//   stores) and C = L S S P S (the file's). A is 83 to 86 instructions, B is
//   87 to 89 and C is 90, and every subset of a given size gives the same
//   shape (all five 87-instruction subsets are B), so inside this family the
//   earlier "step function of the instruction count" measurement holds: the
//   full five-statement prologue is the only one in shape C, at 90.
// - The wanted shape is reachable, and only with a longer function: a real
//   (non-virtual) base class whose ctor stores the four scalars and 0x4fc980,
//   called from the initialiser list, with the derived body storing the same
//   four scalars again. That is x1b.cpp (97 instructions, 319 bytes, 65.9%):
//   its tail is exactly the original's, lea, [esi+0x38], [esi], pop edi,
//   [ecx], [ecx+4], [ecx+8] (with edx for the lea). The store the base
//   constructor repeats is not removed, because it goes through the base's own
//   `this` and DSE cannot match it with the derived body's, so the extra
//   instructions are real stores, and the original's 89 instructions have
//   exactly one set of them.
// - The extra-instruction probes agree with the earlier notes: one extra
//   surviving store early (a duplicate `vptr_slot = DAT_004fc9a0;` before the
//   vectors, u1.cpp, 90 instructions) sinks the lea and puts pop edi after
//   c.x, which is the 97.8% shape; an inlined virtual base (t2.cpp) does the
//   same; an extra store at the end of the body (u5.cpp) pushes pop edi past
//   the epilogue's `mov eax, esi`; duplicate scalar or vtable stores in the
//   prologue are removed by DSE and change nothing (t5.cpp is the file).
// - The family files support the plain `vptr_slot` field at +0: 0x407a90
//   (matched) is a real derived class and keeps only its own vtable store,
//   0x4fc998, because the inlined base constructor's store of 0x4fc980 is
//   dead. The original here stores 0x4fc980 early and 0x4fc9a0 late, so both
//   stores come from source and the class is not a real derived class, which
//   is why every version that declares it one caps below this one.
// - How many instructions the wanted shape costs (L0..L4.cpp): the same
//   non-virtual base class, with the derived body storing k = 0, 1, 2, 3, 4 of
//   the four scalars again after the base constructor has stored them. The pop
//   moves one slot per added store and the lea stops sinking at the same
//   point: k=0 (91 instructions) `mov [esi],vtable, lea, [ecx], pop, [ecx+4],
//   [ecx+8]`; k=1 (92) the pop is already before all three stores; k=2 (93),
//   k=3 (94) and k=4 (97, with edx for the lea) all give the original's tail
//   exactly, `lea, [esi+0x38], [esi], pop edi, [ecx], [ecx+4], [ecx+8]`. So
//   the wanted schedule needs three to five more IL nodes than the byte-exact
//   prologue has, and every node that buys it is a store, so it costs bytes.
//   That is the wall.
// - Ten more ways of adding an IL node without an instruction also compile to
//   a listing identical to the file: a redundant `goto` and a label, a dead
//   address computation on `this`, `&c`, an empty `for` and `switch`, a
//   `while` that peels once, a nested block around the tail, and calls to
//   empty `static inline` helpers between the two vtable stores (g1..g10).
//   /O2 erases every trace of them before the scheduler runs.
// - One warning about the method used here: the /Fa listing is not always a
//   faithful guide. r2.cpp (this file with `int ax; double halfX = ...; ax =
//   (int)halfX;` left as it is) shows the wanted tail in its listing, compiled
//   with the same flags check.py uses, and still scores 98.9% in check.py, so
//   every candidate was confirmed with check.py and not with the listing.
// FUNCTION: 0x407d40
Class_00407d40::Class_00407d40(Class_00408cb0* p, void* q)
{
    owner = p;
    field_8 = q;
    field_c = 0;
    field_10 = p->field_4;
    vptr_slot = DAT_004fc980;
    a = Vec3_00407d40(g_game);
    b = Vec3_00407d40(g_game);
    Vec3_00407d40 temp(g_game);
    field_38 = 0;
    vptr_slot = DAT_004fc9a0;
    c = temp;
}
