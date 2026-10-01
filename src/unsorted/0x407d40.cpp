// Decompiled by GPT-6, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Best retry: 97.8% (296 bytes). Difference: derived vtable store is before
// the third vector's game loads; pop edi is between its second and third stores.
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
    Vec3_00407d40(Game* game) {
        int ax = (int)(game->baseX / 2 * 65536.0);
        *this = Vec3_00407d40(ax, 0, (int)(game->baseY / 2 * 65536.0));
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

// Vtable 0x4fc9a0, constructor 0x407d40, ??_G 0x407e70.
class Class_00407d40 : public Class_00407350 {
public:
    Vec3_00407d40 a;                   // +0x14
    Vec3_00407d40 b;                   // +0x20
    Vec3_00407d40 c;                   // +0x2c
    int field_38;                      // +0x38

    Class_00407d40(Class_00408cb0* p, void* q);
    virtual void FUN_00407380();                    // slot 0, 0x407e90
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
// FUNCTION: 0x407d40
Class_00407d40::Class_00407d40(Class_00408cb0* p, void* q)
    : Class_00407350(p, q), a(g_game), b(g_game)
{
    Vec3_00407d40 temp(g_game);
    field_38 = 0;
    c = temp;
}
