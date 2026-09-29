// Decompiled by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
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
// FUNCTION: 0x407d40
Class_00407d40::Class_00407d40(Class_00408cb0* p, void* q)
    : Class_00407350(p, q), a(g_game), b(g_game), c(g_game), field_38(0)
{
}
