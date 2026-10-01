// Decompiled by GPT-5.6-Terra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro. Names are provisional.
// Partial: 92.6% (431 bytes vs 428). Prologue, branch A and the whole branch B
// loop body match; only the branch B preheader register/base choices differ:
//   original: mov edx,[eax+0x14363]; mov eax,[eax+0x1436b]; cmp eax,edi;
//             mov edi,[esi+4]; mov esi,[esi]; mov [esp+0x18],eax
//   ours:     mov ecx,[eax+0x1436b]; mov edx,[eax+0x14363]; cmp ecx,edi;
//             mov esi,[esi]; mov edi,[eax+0x2c7a]; mov [esp+0x18],ecx
// The original reads BOTH loop fields through p (esi): y first (mov edi,[esi+4])
// then x into the dying base (mov esi,[esi]), with count2 in EAX, which needs
// g_game dead at its count2 load. Ours reads x from p (correct dying load) but
// y as g_game->view.y, which keeps g_game live, forces count2 into ECX and the
// y load to [eax+0x2c7a].
// Structural result (confirmed again this retry, mimo-v2.6-pro): the both-from-p
// form (dy = s->y - p->y; dx = s->x - p->x) compiles to the exact mirror of the
// target preheader (s load first, count2 in EAX, mov esi,[edi+4]; mov edi,[edi])
// but the whole function then swaps p to EDI and the zero constant to ESI (also
// swapping the p and counter spill slots and reloading p through the stack in
// branch A), scoring 71.2 to 72.7%. The flip tracks g_game dying at the count2
// load, not the p->y read itself: any spelling that keeps g_game alive in the
// loop keeps p=ESI (92.6% here, 90.4% for dy-first with y from p and x from
// g_game->view.x, where p dies on the wrong load).
// mimo-v2.6-pro retry (v91 to v108) tried on the both-from-p base, all still
// 71 to 73%: DistSq(Slot*,Point*) and DistSq2(int,int,int,int) inline helpers,
// inline (s->x-p->x)*(s->x-p->x)+... in both operand orders, while loop with
// the counter declared outside, s/best declaration swap, one declaration group
// (int dy = ..., dx = ...;), Point* const p, const Point* p, uninitialised dx/dy
// before the for, p/result declaration swap at the top, pre-read int n =
// g_game->count2, early-return control flow. tools/permute.py on the 92.6% base
// (seed 3, 27538 candidates) found no gain. Earlier retries also tried q = p
// copies, a Point& alias, q = &g_game->view (79.1%), cast-based address forms,
// int* p[i], two separate ifs and reordered declarations (notes preserved in
// git history). What remains: a spelling where g_game dies at the count2 load
// but p keeps ESI, or any source perturbation that flips the p/zero colour
// choice back on the both-from-p form.

#pragma pack(push, 1)
struct Point_0048cd80 {
    int x;                             // +0x0
    int y;                             // +0x4
};

struct Rect_0048cd80 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct UnitDef_0048cd80 {
    char unknown_0[0x176];
    int field_176;                     // +0x176
    int field_17a;                     // +0x17a
    int field_17e;                     // +0x17e
};

struct Unit_0048cd80 {
    char unknown_0[0x92];
    UnitDef_0048cd80* def;             // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    unsigned short field_a8;           // +0xa8
    char unknown_aa[0x118 - 0xaa];
};

struct Slot_0048cd80 {
    unsigned short field_0;            // +0x0
    int x;                             // +0x2
    int y;                             // +0x6
};

struct Game_0048cd80 {
    char unknown_0[0x2c76];
    Point_0048cd80 view;               // +0x2c76
    char unknown_2c7e[0x142bb - 0x2c7e];
    Rect_0048cd80 rect_142bb;          // +0x142bb
    char unknown_142cb[0x14357 - 0x142cb];
    Unit_0048cd80* units;              // +0x14357
    Unit_0048cd80* units_end;          // +0x1435b
    unsigned short* list;              // +0x1435f
    Slot_0048cd80* list2;              // +0x14363
    int count;                         // +0x14367
    int count2;                        // +0x1436b
    char unknown_1436f[0x37e27 - 0x1436f];
    Rect_0048cd80 rect_37e27;          // +0x37e27
};
#pragma pack(pop)

extern Game_0048cd80* g_game;

int __stdcall FUN_004b6720(Rect_0048cd80* rect, int x, int y);
int __stdcall FUN_0048c6a0(Unit_0048cd80* unit, Point_0048cd80* p);

static inline int FixMul(int a, int b)
{
    return (int)(((__int64)a * b) >> 16);
}

// Parent retry note: This worker kept the inherited 92.6% version. Rewriting
// the branch-B counter as a declaration-ordered while loop or as a top-tested
// while(1)/break loop did not improve it (attempts 1 and 3 stayed at 92.6%;
// attempt 2 dropped to 65.4%). The remaining mismatch is the branch-B
// preheader register allocation described above.
// FUNCTION: 0x48cd80
unsigned short __stdcall FUN_0048cd80(void)
{
    Point_0048cd80* p = &g_game->view;
    unsigned short result = 0;
    if (FUN_004b6720(&g_game->rect_37e27, p->x, p->y)) {
        int best = 0x7fff0000;
        unsigned short* ids = g_game->list;
        if (ids == 0)
            return 0;
        for (int i = 0; i < g_game->count; i++) {
            Unit_0048cd80* u = &g_game->units[ids[i]];
            if (u->field_a6 != 0) {
                if (FUN_0048c6a0(u, p)) {
                    UnitDef_0048cd80* def = u->def;
                    int v = FixMul(def->field_17a, 0x8000) + def->field_17e;
                    v = FixMul(v, def->field_176);
                    if (v < best) {
                        result = u->field_a8;
                        best = v;
                    }
                }
            }
        }
    } else if (FUN_004b6720(&g_game->rect_142bb, p->x, p->y)) {
        int best = 99999;
        Slot_0048cd80* s = g_game->list2;
        for (int i = g_game->count2; i > 0; i--) {
            int dx = s->x - p->x;
            int dy = s->y - g_game->view.y;
            int d = dx * dx + dy * dy;
            if (d < 4 && d < best) {
                best = d;
                result = s->field_0;
            }
            s++;
        }
    }
    return result;
}
