// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry in #3224: seven checker invocations, best remains 76.0%; no MATCH. Capturing flags6 tied at 76.0%, splitting the case-9 OR/store also tied, and capturing the command kind byte scored 75.6%. Existing mask/register, Wait-push, and MoveToEnd differences remain.
// deepseek-v4.1-flash (#3039 retry): still 76.0% (772 of 780). The two-literal Wait
// form (one static helper called with 0xf and 0x1e) does tail-merge into one block
// with real `push 0xf`/`push 0x1e` entries (768 bytes, 71.7), so the goto-Wait is not
// needed for the merge. But switching the pending mask to `unsigned short` reproduces
// the original pending block byte for byte yet stops the two Wait copies merging
// (804 bytes, 68.2) and rotates the downstream allocator. The pending-mask register
// choice and the Wait tail-merge are coupled through one global MSVC 5 allocator
// state. headers.py 128 sets flat at 76.0. Residual: pending-block eax/ecx rotation,
// case-9 OR result (ours eax, original edx then mov eax,edx), the two Wait immediate
// pushes in the goto form, and the case-6/7 `mov ecx,[ebx]` preload.
// PARTIAL (71.0%, 772 bytes against the original's 780). Per-frame driver of
// the unit's command list (+0x5c), the "main list" twin of 0x43bad0 (the +0x60
// list, matched, and the source of the Wait_0043b7c0 shape below). The list
// head is re-read after every node, so a node the callback re-queues is seen
// again in the same pass. The switch cases are written in the order the
// original emitted the bodies (3, 1, 0, 2/4, 5/8, 9, 6, 7, default), which is
// also the order of the jump table at 0x43baa4.
//
// What matches byte for byte: the whole prologue (unit in edi, node in esi,
// the list-head address in ebx, spilled to [esp+0x10]), the due check, the
// 3-iteration FUN_0048a0f0 countdown loop (it only matches as a do/while,
// which is what frees ebx for the counter and pushes the head into the
// spill), the case layout, the tail-merged Wait block shared by cases 3 and
// 9, both unlink searches, the delete sequence, ClearAll and the two
// FUN_0043ac60 arms.
//
// What still differs: one allocator state, seen as an eax/ecx swap in every
// block. The original keeps the 16-bit mask in ecx and flags6 in eax
// (`mov cx,word [edi+0xba]` / `mov eax,[esi+6]`); ours keeps the mask in eax
// and flags6 in ecx. The same swap reappears downstream: the callback-table
// base is in edx here and ecx in the original, `unit` is reloaded into ecx
// for the 3-iteration loop instead of eax, `unit->def` into eax instead of
// edx, `*link` in ClearAll into ecx instead of edx, and the default case's
// `unit` into edx instead of eax. Tried and rejected: the mask as `int`
// (55.5%, it also gains a `xor eax,eax; mov ax`), the field read twice with
// no local at all (identical score, identical code), the `unit` reload for
// the field_ba store hoisted differently. The one shape that did help was
// Wait_0043b7c0: with `when = FUN_004b6c30(n) + 0x1e` computed first and
// `flags6 |= 1` second it tail-merges cases 3 and 9 the way the original does
// (0x43b951); the other order duplicated the block at both sites (792 bytes).
//
// Retried by deepseek-v4.1-flash: six re-spellings of the pending block
// (flags6 local first, OR operands swapped, no mask local, no cast, int
// pending, mask pre-masked) all score the same 59.7% byte for byte, so the
// eax/ecx choice is a global allocator rotation, not an expression lever. A
// normalised opcode diff shows the only real structural gap is case 9: the
// original keeps the just-stored `flags | 0x800000` in eax and reuses it for
// the later `test eax, 0x40000` (`mov eax, edx` before loading node->next),
// where ours re-reads node->flags inside the inlined RemoveAndDelete. That
// extra live value is the likely source of the rotation.
//
// Fixed by deepseek-v4.1-flash: writing case 9 as `if (node->next != 0)
// RemoveAndDelete(...); else { count = 0; Wait(...); }` (RemoveAndDelete
// fallthrough, wait out of line) makes the compiler keep the just-stored
// flags value (`mov eax, ecx`) instead of reloading it, 59.7 -> 62.8.
// Still differs (764 vs 780, 16 bytes short): case 9 now leaves the flags in
// ecx and reloads node->next in the shared unlink tail, where the original
// had flags in eax and kept node->next in edx from the condition
// (`mov eax,edx` then `mov edx,[esi+0x4a]`, reused as `mov [ecx],edx`).
// The pending block's eax/ecx rotation and the new-command tail's kind store
// ordering are also still off.
//
// Improved by space-bunny-free: 62.8 -> 67.8 (764 -> 800 bytes) by writing
// case 9 out in full instead of calling RemoveAndDelete, and by binding the two
// values it needs to locals *before* the store that makes them:
//
//     unsigned int f9 = node->flags | 0x800000;
//     Class_0043a1f0* next9 = node->next;
//     node->flags = f9;
//     if (next9 != 0) { ...inlined unlink using f9 and next9... }
//     else { node->count = 0; Wait_0043b7c0(node, 0x1e); }
//
// That is what makes the case-5/8 unlink and the case-9 unlink share one
// physical `or 0x10000; delete` tail (case 5's found branch now ends in a
// `jmp` into case 9's, as in the original), and it puts `next9` in the
// register the original keeps it in, so the `*link = next` needs no reload.
// Reading `node->next` before the store (and before the `je`) is required: it
// is what the original's `mov edx,[esi+0x4a]` before the compare is.
// The tail of the loop must be inside the `n9 == node` branch: keeping a copy
// after the loop costs 28 bytes of duplicated delete and drops the score to
// 64.0. Inverting the test (`if (next9 == 0) { Wait; break; }`) also drops it,
// 760 bytes but only 64.0, so the original really does branch on `next == 0`
// into the Wait block.
//
// Improved by space-bunny-free (retry): 67.8 -> 71.0 (800 -> 772 bytes) by
// finally getting the two Wait bodies to share ONE block. The way to do it is
// a `goto` INTO case 9's else, with the delay in a local set before the jump:
//
//     int waitn = 0;
//     case 3: waitn = 0xf; goto do_wait;
//     case 9: ... else { node->count = 0; waitn = 0x1e;
//     do_wait: Wait_0043b7c0(node, waitn); }
//
// The shared block then matches the original's 0x43b951 byte for byte
// (edi/edx->ecx), and case 3 becomes `mov eax,0xf; jmp 0x43b954` instead of a
// second copy of the whole body (build/scratch/0x43b7c0/variants/v0.cpp is the
// two-literal-call-sites form, which is 800 bytes: MSVC allocates the two
// inlined copies differently, so its block folding never joins them, even
// though the second copy is byte identical to the original).
//
// What still differs, 8 bytes short plus the known rotations: sharing the block
// costs the immediate pushes. The original keeps a literal at each site
// (`push 0xf` at case 3, `push 0x1e` at case 9) because its source called the
// wait helper twice with two constants and MSVC merged the blocks AFTER
// inlining, hoisting the common tail; a source that shares one block cannot
// push a literal, so it needs `mov eax,imm32; push eax` (6 bytes) at each of
// the two entries, +4 each. Since the function is 8 bytes short overall, that
// means roughly 16 bytes of the original's other code are missing as well: the
// original's case-9 `mov eax,edx` copy of the just-stored flags (we keep the
// OR result straight in eax and never copy it), and one `mov` in the ClearAll
// search that we do not emit. The pending block's eax/ecx rotation, the
// `unit->def` load into ecx instead of edx, the kind byte going to
// [esp+0x18] instead of the dead argument slot at [esp+0x1c], and the
// `mov ecx,[ebx]` we add at the top of case 6 all still stand.
//
// Retried by deepseek-v4.1 (10 more check runs, all 67.8% or worse): the
// pending block was re-spelled nine ways (flags6 read into a local first, the
// mask local hoisted out of the loop, the two stores swapped, the OR operands
// swapped, mask & ~pending instead of ~pending & mask, int pending, an extra
// field_4e local, the mask read at the top of the loop) and every one of them
// compiles to the same 800 bytes: the eax/ecx rotation is not reachable from
// this statement's source shape. The case-3 Wait block was also inlined
// textually instead of calling the helper (same 67.8%) and the helper was
// reordered to set the flag before the call (66.4%), so the missing tail-merge
// at 0x43b951 is a register-allocation consequence of the rotation, not a
// separate bug. Left as is: the rotation and the duplicated Wait are the two
// remaining defects, worth 20 bytes in total.
//
// Retried by deepseek-v4.1 (2 more check runs, both 67.8%, byte identical to
// the 800 already here): the original's case-9 `mov eax, edx` copy suggests
// the source stored the OR'd flags before reading node->next, so `node->next`
// was moved after the `node->flags = f9` store, and the store was then spelled
// as a chained assignment (`node->flags = f9 = node->flags | 0x800000`); MSVC
// emits the same sunk store both times, so the copy in the original is an
// allocator decision, not a statement order. The case-3 Wait copy is the real
// blocker for the tail merge: it picks edx for flags6 and ecx->edx for the
// g_game frame, while the case-9-else copy (which matches the original's
// 0x43b951 byte for byte, edi/edx->ecx) is right next to it, so the two
// blocks are not textually identical and MSVC's folding pass leaves both.
// Block folding in MSVC 5 is evidently post-allocation here, which is why the
// register rotation also decides the merge. Writing the Wait body out
// textually at both sites (no helper at all) also compiles to the same 800
// bytes, so the two expansions never become textually identical whatever the
// source shape.
//
// Retried by deepseek-v4.1-flash (7 check runs, 71.0 -> 71.4): re-checked the
// two-literal Wait form (800 bytes, 67.8, no merge) and confirmed the goto
// trick is still the best Wait shape. Two more attempts changed nothing
// (swapping the top-level & operands, splitting the pending expression across
// two statements). The one gain: reading unit->def->field_230 inline in the
// new expression instead of through a `kind` local (the local spilled to
// [esp+0x18] before the push, 772 bytes; the inline form re-reads the field
// for the ctor, 776 bytes, 71.4). Splitting case 9 into
// `f9 = node->flags; node->flags = f9 | 0x800000;` makes the function exactly
// 780 bytes and puts the OR result in ecx with the pre-OR value in eax, but
// the test then uses the pre-OR eax and the byte score drops to 64.4, so the
// combined `f9 = node->flags | 0x800000` form is kept.
//
// What still differs at 71.4 (776 vs 780): the pending block's eax/ecx mask
// vs flags6 rotation, case 9 leaving the OR result in eax/next in edx (the
// original keeps the OR result in edx and copies it to eax before reusing edx
// for next), the Wait immediate push (`mov eax,0xf; push eax` vs `push 0xf`,
// +4 at each of the two entries), and the case-7 ClearAll / case-6 MoveToEnd
// preload (`mov ecx,[ebx]` before `mov eax,ebx`).
//
// Improved by deepseek-v4.1-flash (retry): 71.4 -> 76.0 (776 -> 772 bytes) by
// declaring the pending mask as `int mask = unit->field_ba;` instead of
// `unsigned short`. The 16-bit type made MSVC keep the mask in a 16-bit
// register and zero-extend it with a separate `and edx,0xffff`, and that in
// turn put the whole allocator one slot off downstream: the callback-table
// base came out in edx (original: ecx) and the pending block's mask/flags6
// were swapped. The int mask emits `xor eax,eax; mov ax,[edi+0xba]` instead,
// and the callback table base, the 3-iteration loop, unit->def and the
// ClearAll link all land in the original's registers. Together with the
// goto-Wait shape already here this is the best found.
//
// Tried in this retry, all at or below 76.0: unsigned short and unsigned int
// casts of the mask, a flags6 local before and after the mask, the mask
// declared after pending, inline field_ba with no local, the two pending
// stores swapped, the kind local and the int kind local in the new-command
// tail, assignment-in-condition for the tail kind, and the two-literal Wait
// form (still no tail merge, 768 bytes, 71.7). Scoring scratch variants with
// `check.py <addr> <file>` does not count against the run limit.
//
// What still differs at 76.0 (772 vs 780): the pending block's registers are
// rotated (ours: field_4e edx, flags6 ecx, mask eax; original: mask ecx,
// field_4e edi, flags6 eax) and its two stores are scheduled differently;
// case 9 ORs into eax and loads node->next early where the original ORs into
// edx, copies to eax, then reuses edx for next; the Wait immediate is still
// `mov eax,imm; push eax` at both entries (original `push imm`, +4 each, the
// missing 8 bytes); and the new-command tail re-reads unit->def for the ctor
// where the original spills the already-tested byte into the dead unit
// argument slot ([esp+0x18] before the push vs [esp+0x1c] after it).

#pragma pack(push, 1)

struct Unit_0043b7c0;
struct UnitDef_0043b7c0;
struct Player_0043b7c0;

class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char kind;            // +0x4, index into DAT_00512344
    unsigned char count;           // +0x5
    unsigned int flags6;           // +0x6, bit 0 set while the node waits
    unsigned int wakeFrame;        // +0xa
    Unit_0043b7c0* unit;           // +0xe, handed to the callback
    char unknown_12[0x42 - 0x12];
    unsigned int flags;            // +0x42, bit 0x40000 picks the second list
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;          // +0x4a
    unsigned int field_4e;         // +0x4e
    char unknown_52[0x56 - 0x52];

    Class_0043a1f0(unsigned char k, void* o, void* p, int a, int b, int c);
    ~Class_0043a1f0();
};

struct Player_0043b7c0 {
    int active;                    // +0x0
    char unknown_4[0x73 - 0x4];
    char state;                    // +0x73
};

struct UnitDef_0043b7c0 {
    char unknown_0[0x230];
    unsigned char field_230;       // +0x230, the kind of a fresh command
};

struct Unit_0043b7c0 {
    char unknown_0[0x5c];
    Class_0043a1f0* list;          // +0x5c
    Class_0043a1f0* list2;         // +0x60
    char unknown_64[0x92 - 0x64];
    UnitDef_0043b7c0* def;         // +0x92
    Player_0043b7c0* player;       // +0x96
    char unknown_9a[0xba - 0x9a];
    unsigned short field_ba;       // +0xba, the unit's pending mask
};

struct Game_0043b7c0 {
    char unknown_0[0x38a47];
    unsigned int frame;            // +0x38a47
};

// One entry of the callback table, 0x19 bytes: the index arithmetic is
// kind*5*4 + kind*5 + 4.
struct Callback_0043b7c0 {
    char unknown_0[4];
    int (__stdcall* notify)(Unit_0043b7c0* unit, Class_0043a1f0* node, unsigned int pending);  // +0x4
    char unknown_8[0x19 - 0x8];
};

#pragma pack(pop)

extern Game_0043b7c0* g_game;
extern Callback_0043b7c0* DAT_00512344;

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
int __stdcall FUN_0048a0f0(Unit_0043b7c0* unit, int index);
int __stdcall FUN_004b6c30(int n);
void __stdcall FUN_00439eb0(Unit_0043b7c0* unit, int all);
void __stdcall FUN_00439f80(Unit_0043b7c0* unit, Class_0043a1f0* node);
void __stdcall FUN_0043ac60(Unit_0043b7c0* unit, Class_0043a1f0* node, Class_0043a1f0* before);

// Unlinks `node` from the list `pp` starts (or the unit's second list when the
// node has flag 0x40000) and deletes it, marking it 0x10000 unless it is the
// head. Returns false when the node is not in the list at all.
static bool RemoveAndDelete(Unit_0043b7c0* unit, Class_0043a1f0** pp, Class_0043a1f0* node)
{
    Class_0043a1f0* first = *pp;
    Class_0043a1f0** link = (node->flags & 0x40000) ? &unit->list2 : pp;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
        if (n == node) {
            *link = node->next;
            if (node != first)
                node->flags |= 0x10000;
            delete node;
            return true;
        }
        link = &n->next;
    }
    return false;
}

// Takes `node` out of the list and puts it back at the end.
static void MoveToEnd(Class_0043a1f0** pp, Class_0043a1f0* node)
{
    Class_0043a1f0** link = pp;
    while (*link != node)
        link = &(*link)->next;
    *link = node->next;
    while (*link != 0)
        link = &(*link)->next;
    *link = node;
    node->next = 0;
}

// Deletes every node of the first list, then every node of the second one.
static void ClearAll(Unit_0043b7c0* unit, Class_0043a1f0** pp)
{
    Class_0043a1f0* first = *pp;
    Class_0043a1f0* node;
    while ((node = *pp) != 0) {
        *pp = node->next;
        if (node != first)
            node->flags |= 0x10000;
        delete node;
    }
    while ((node = unit->list2) != 0)
        FUN_00439f80(unit, node);
}

// Puts the node to sleep for `n` milliseconds. The `when` temporary is what
// keeps the sum from folding into one lea, and the flag is set after the call
// so that both call sites tail-merge (as in 0x43bad0).
static void Wait_0043b7c0(Class_0043a1f0* node, int n)
{
    unsigned int when = FUN_004b6c30(n) + 0x1e;
    node->flags6 |= 1;
    node->wakeFrame = g_game->frame + when;
}

// FUNCTION: 0x43b7c0
void __stdcall FUN_0043b7c0(Unit_0043b7c0* unit)
{
    Class_0043a1f0** pp = &unit->list;
    Class_0043a1f0* node = *pp;

    while (node != 0) {
        unsigned int f9;
        Class_0043a1f0* next9;
        if (g_game->frame >= node->wakeFrame) {
            node->wakeFrame = 0xffffffff;
            node->field_4e |= 1;
        }
        int mask = unit->field_ba;
        unsigned int pending = (node->field_4e | mask) & node->flags6;
        if (node->flags6 != 0 && pending == 0)
            return;
        unit->field_ba = (unsigned short)(~pending) & mask;
        node->field_4e &= ~pending;
        node->flags6 = 0;

        if (pending & 0x10000) {
            int i = 0;
            int n = 3;
            do {
                FUN_0048a0f0(unit, i);
                i++;
            } while (--n);
        }

        int waitn = 0;
        switch (DAT_00512344[node->kind].notify(node->unit, node, pending)) {
        case 3:
            waitn = 0xf;
            goto do_wait;
        case 1:
            node->count++;
            break;
        case 0:
            node->count = 0;
            break;
        case 2:
        case 4:
            break;
        case 5:
        case 8:
            RemoveAndDelete(unit, pp, node);
            break;
        case 9: {
            f9 = node->flags | 0x800000;
            node->flags = f9;
            next9 = node->next;
            if (next9 != 0) {
                Class_0043a1f0* first9 = *pp;
                Class_0043a1f0** link9 = (f9 & 0x40000) ? &unit->list2 : pp;
                for (Class_0043a1f0* n9 = *link9; n9 != 0; n9 = n9->next) {
                    if (n9 == node) {
                        *link9 = next9;
                        if (node != first9)
                            node->flags |= 0x10000;
                        delete node;
                        break;
                    }
                    link9 = &n9->next;
                }
            } else {
                node->count = 0;
                waitn = 0x1e;
            do_wait:
                Wait_0043b7c0(node, waitn);
            }
            break;
        }
        case 6:
            MoveToEnd(pp, node);
            break;
        case 7:
            ClearAll(unit, pp);
            return;
        default:
            FUN_00439eb0(unit, 1);
            return;
        }
        node = *pp;
    }

    if (unit->player->active == 0)
        return;
    char state = unit->player->state;
    if (state != 1 && state != 2)
        return;
    if (unit->def->field_230 == 0)
        return;
    Class_0043a1f0* cmd = new Class_0043a1f0(unit->def->field_230, 0, 0, 0, 0, 0);
    cmd->flags |= 0x4000;
    FUN_0043ac60(unit, cmd, (cmd->flags & 0x40000) ? unit->list2 : *pp);
}
