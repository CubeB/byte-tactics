// Decompiled by GPT-5.6-Terra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// GPT-6.1-sol (#3172 retry): four scored checks, best 92.6%; loop-counter-before-while and reversed local order tied, while(1) with break fell to 65.4%. No MATCH.
// Partial: 92.6% (431 bytes vs 428), best of v20-v39 plus the v40-v88 retry.
// Prologue, branch A and
// the whole branch B loop body match; only two instructions differ, in the
// branch B preheader (both are register/base choices for the p reads):
//   original: mov edx,[eax+0x14363]; mov eax,[eax+0x1436b]; cmp eax,edi;
//             mov edi,[esi+4]; mov esi,[esi]; mov [esp+0x18],eax
//   ours:     mov ecx,[eax+0x1436b]; mov edx,[eax+0x14363]; cmp ecx,edi;
//             mov esi,[esi]; mov edi,[eax+0x2c7a]; mov [esp+0x18],ecx
// Reading p->x (first statement) makes p die on `mov esi,[esi]`, which is the
// original's dying-register load, but the second read g_game->view.y must then
// be rematerialised from EAX, so g_game stays live and count2 takes ECX.
// Reading both fields from p (v20 dy-first 71.2%, v21 dx-first 72.7%) moves p
// to EDI for the whole function and the zero to ESI, which wrecks branch A.
// Also tried: dy-first with the y read from p (90.4%, p dies on the wrong
// load), q = p copies and a Point& alias (fold into p, 71.2%), q = &g_game->view
// (rematerialised from g_game, 79.1%), cast-based address forms (identical).
// The remaining work is getting `mov edi,[esi+4]` plus `mov eax,[eax+0x1436b]`
// without re-triggering the whole-function p->EDI reallocation.
// Retry v40-v88 confirmed: ANY read of p->y (or a branch-local q alias, or a
// hoisted py/dyv local) in the branch B loop makes MSVC give p=EDI and the
// zero=ESI, moving branch A's u into p's register and adding a `jmp`; using
// only g_game->view fields (v42/v56) shifts the p spill slot and also breaks
// branch A. The exact original (list2 then count2 into EAX, then [esi+4] and
// [esi]) needs g_game dead before the p reads, but every source spelling that
// kills g_game there also perturbs the function-wide p/zero assignment.
// Retry by deepseek-v4.1-flash (v89+): the both-fields-from-p form is the
// exact structural mirror of the original with only p/zero swapped
// (p=EDI, zero=ESI; branch B emits mov esi,[edi+4]; mov edi,[edi] and count2
// still lands in EAX), confirming the target instruction shape is reachable.
// The nearest keeper keeps p=ESI: dy first with s->y - p->y and
// s->x - g_game->view.x scores 90.4% but reads p->y into ESI and leaves
// count2 in ECX. Nothing tried (const/reference p, int* p[i], inline
// Dist2(Slot*,Point*) helpers, q aliases, two separate ifs, explicit returns,
// reordered declarations) moved p back to ESI while reading both fields from
// p; the allocator's p/zero colour choice tracks the extra p dereference.

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
