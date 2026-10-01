// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash retry 5 (timeboxed, unchanged at 84.1%): tested the two
// shapes the retry-4 note left open. `Entry* e = &(entries[button->index].type
// == 1 ? entries[button->index] : *button);` (lvalue ?: over struct objects
// addressed afterwards) is byte-identical at 84.1%, so that lead is refuted
// too. Also re-tested the copy/raw-parameter split in both directions (a local
// `b = button` used for the 11 index loads with the raw parameter only in the
// else arm, and the reverse with the selection on `b`): VC5 coalesces both and
// still emits `mov ebp, esi`. No shape reached the original's
// `mov ebp, [esp+0x34]`.
// deepseek-v4.1-flash retry 4 (timeboxed out at 84.1%, this file unchanged
// from the known best): no new variant was scored this session, only evidence
// gathered. objdump of the original confirms the census exactly: `button`
// (param 1) is loaded once into edi and read11 times as [edi+0x60], `entries`
// (param 2) is loaded once into esi and used 12 times (1 lea + 11 pushes),
// and the ONLY other read of a parameter slot is the false arm
// `mov ebp, [esp+0x34]` at 0x419c1c. So the single remaining diff is still
// that false arm (our `mov ebp, esi`, 2 bytes vs 4) and the esi/edi role swap
// it causes via the use-count tie. Still untested from the notes: declaring
// param 1 as a Menu-like struct (index at +0x60) with a cast in the false arm
// `(Entry_00419be0*)menu`, which gives the false arm a structurally different
// expression tree than the 11 `menu->index` loads; also untested is an
// lvalue ternary over struct OBJECTS addressed afterwards,
// `e = &(cond ? entries[i] : *button);`, a middle ground between the plain
// ternary (84.1%) and the lvalue-?: over references (66.6%, the only shape
// ever seen to read the home slot).
// deepseek-v4.1-flash retry 3 (still 84.1%, no improvement): the remaining
// `mov ebp,[esp+0x34]` vs `mov ebp,esi` is allocator state, confirmed again.
// New variants, every one byte-identical at 84.1% except the first: an
// `Entry*& button` reference parameter with all 11 index loads routed through
// a local copy `b` and only the false arm reading the reference (reaches the
// exact 1329 bytes but 64.1%, the reference forces two levels of addressing);
// a named element pointer `p = &entries[button->index]` then
// `p->type == 1 ? p : button`; `const` pointer parameters plus a
// `*(Entry**)&button` false arm; a local `Entry* b = button` for the index
// loads with the raw parameter left for the false arm; an inline
// `Pick(Entry* s, Entry*& b)` returning `s->type == 1 ? s : b`; and an
// out-parameter inline `Pick2(Entry* s, Entry*& b, Entry*& out)`. The
// reference forms reproduce the prologue read but add code, so the source
// shape alone cannot produce this allocation.
// deepseek-v4.1-flash retry (still 84.1%, no improvement): re-tested the
// remaining leads from the notes. `*(&button)` and
// `*(Entry_00419be0**)&button` as the false arm, a `button = button;`
// self-store before the selection, and `Entry_00419be0* const` parameters
// (the last also changes the mangled name to QAU...) all leave the body
// byte-identical to the current 1327-byte version. The inverted
// `type != 1 ? button : &entries[..]` scores 83.8%. Rewriting the whole chain
// with the sibling's inlined `Contains(entries, text, index)` helper (owning
// its own char name[32], no hoisted index) is also 84.1%. Still only
// `mov ebp, [esp+0x34]` vs `mov ebp, esi` (the button home-slot reload) and
// the esi/edi parameter roles remain.
// deepseek-v4.1 retry (84.1%, unchanged): the whole remaining diff is one
// instruction plus the register names it drags with it. Check says original
// 1329 bytes vs ours 1327, and the 2 bytes are exactly the encoding gap
// between `mov ebp, esi` (2) and the original's `mov ebp, [esp+0x34]` (4),
// so no other instruction is missing anywhere: the sole real difference is
// that the original's else arm of the selection reads `button` back from the
// parameter home slot while every other use is the cached edi, which in turn
// gives button edi and entries esi (ours: esi/edi). New evidence this round:
// making the first parameter a reference (`Entry*& button`) does reproduce the
// original prologue exactly (push edi / mov edi,[esp+0x34] / lea ebx /
// mov esi,[esp+0x38]) but it also dereferences through the reference, grows
// the frame and loses 12 points, which proves the source shape is a plain
// pointer whose *variable* is read from its home slot once. Shapes retried at
// 84.1% with byte-identical output (each one run through check.py): an index
// local (drops to 63.9%), a type local, `q = &entries[i]` element pointer,
// a `base = &entries[0]` pointer, comma-operator duplicate uses of button,
// `Pick(list, btn)` and `Pick(btn, list)` inline helpers, a local
// reference binding `Entry*& b = button;` (VC5 folds it away), void*/char*
// casts in the else arm, the `entries + button->index` pointer form, and an
// `int* pidx = &button->index;` local (61.3%, same 1329 byte size but the
// whole shape shifts). Every shape that keeps the code shape lands on exactly
// 84.1% with identical bytes, so the one memory read cannot be levered from
// the source expression; it needs the construct that made VC5 treat `button`
// as memory-resident for that single use (address-taken semantics), which no
// local reference, inline helper or cast reproduced.
// GPT-6 retry: a selection object holding the button parameter by reference
// or pointer, with ternary/early-return/assignment selection, did not improve
// 84.1%. The button stack reload and entries/button register swap remain.
// GPT-6.1-sol retry: Rechecked at 84.1%. A reference parameter for button and an explicit index/selected-pointer temporary both produced the same initial register assignment and score. The mismatch remains button/entries in esi/edi instead of edi/esi, affecting repeated calls and branch offsets.
// deepseek-v4.1 round 2 (84.1%, same 1327-byte body): 18 more shapes scored
// through a local harness, every one byte-identical to the current file:
// `:(&button)` forms, `(void)&button;`, `Entry** pb = &button;` with `*pb` and
// `pb[0]`, `button + 0`, a char* cast, inline `Id(Entry*&)`, `Id(Entry* const&)`
// and `Touch(Entry**)` helpers, a `Entry*& b = button;` alias, a plain local
// copy of the button (used for all 11 index loads, with the raw parameter left
// for the else arm, and vice versa), a coalesced `Entry* es = entries;` alias,
// and a cached `int idx`. VC5 coalesces every one of those copies and still
// folds the else arm to the parameter's register, so the shape really is
// allocator state. The only form that reads the home slot and gets the
// parameter roles right remains the lvalue `?:` (66.6%, see below).
// Two extra facts from the disassembly: the frame is 0x20 (locals only at
// [esp+0x10], the 32-byte name buffer), so a spilled local can never land in
// the parameter home slot; and the original never stores to [esp+0x34] or
// [esp+0x38] (only two loads plus the else arm read), so the else arm is a
// genuine read of the incoming parameter, not of a reused local slot.
// Handles a click on an order button: finds which order the button's name
// contains and selects that order mode (FUN_00419bc0 inlined), plays the
// "immediateorders" or "specialorders" sound and returns 1; returns 0 when the
// name is no order. STOP issues the stop order at once.
//
// PARTIAL. Everything matches except two things that come from one cause:
// the original reads `button` for the else arm of the selection from its
// stack slot (`mov ebp, [esp+0x34]`) although it also keeps `button` in edi,
// and because that use is not a register use, `entries` gets esi and `button`
// edi (ours: the reverse, `mov ebp, esi`). No header set and no number of
// dummy declarations changes it, so the source shape is still wrong.
// Tried without effect: if/else, `!=` forms, a pointer local, a copy of the
// button in a local, reassigning the parameter, inline helpers taking the
// button by value, by pointer or by (const) reference, struct-wrapped or
// `int` parameters, a member function with `this` on the stack, and reading
// the slot through casts. Only an lvalue `?:` over references
// (`Entry*& Pick(Entry*& button, Entry*& s) { return s->type == 1 ? s : button; }`)
// reads the slot and fixes esi/edi (66.6%), but it also stores `s` to the
// stack and selects addresses, so it is not the original either.
// What that variant shows: the original treats `button` as an address-taken
// variable whose address never escapes (its loads are shared in edi, loaded
// after the pushes, with no reloads after calls), so look for a construct
// that takes `&button` locally and leaves no code of its own. Passing
// `&button` to a real function in a branch that is later removed also makes
// it address-taken, but then it is reloaded after every call.
//
// Retry (deepseek-v4.1-flash): confirmed the register pair and the single
// reload are insensitive to the selection's source form. Roughly 45 shapes
// scored through compile_source/compare directly (sel local, if/else, `!=`,
// `*&button`, pointer casts, union/struct/array copies, inline helpers taking
// `Entry*&`, `Entry* const&`, `Entry**` or by value, moving the `orders`/`arr`
// declarations, swapping the parameter declaration order) and a 0..44
// dummy-`extern int` sweep all produced byte-identical output at 84.1%. So
// this is compiler state, not something the expression can lever.
//
// space-bunny-free round, still 84.1%. New evidence, from a minimal probe
// (build/scratch/0x419be0/probe*.cpp, compiled with /Fa so the register
// assignment is readable) that isolates the head:
// * The callee-saved queue is [param1, param2, orders, sel]: MSVC 5 ranks the
//   live-across-call values by parameter slot, so the first parameter always
//   takes the first register it offers. The original has the SECOND parameter
//   in the first register, so the queue there is [param2, param1, orders, sel].
// * The same probe compiled WITHOUT the `orders` local (3 registers instead of
//   4) produces exactly the original's order, param1 in edi and param2 in esi,
//   from the identical selection expression. So the fourth register-resident
//   value is what pushes param1 ahead of param2, not the selection form:
//   eight shapes of `orders` (declared before/after the selection, char* and
//   PE* types, assigned in a later statement, no local at all) leave the real
//   function at 84.1% or worse (46.9% and 60.2% for the two that reorder it).
// * No probe shape produced the original's `mov ebp, [esp+0x34]`: every form
//   of the selection (ternary, if/else, inverted if/else, address into a
//   named local first, comma operator, `PE*&` reference parameter, a local
//   copy of the parameter, `entries + button->index`) still substitutes the
//   register for the false arm. The reference parameter does reproduce the
//   two prologue copies but adds a third register for the dereference.
// * Swapping the two parameter NAMES, so the first stack slot is called
//   `entries`, scores 84.4% (kept at build/scratch/0x419be0/swapparams.cpp).
//   It is not the original: the caller pushes the button first, and the
//   original reads the index out of the first slot, so the declaration has to
//   be (button, entries). Recorded only, not used.
//
// space-bunny-free, second pass, still 84.1%. New result: a register-use
// census proves the two visible diffs are ONE cause, and names the exact
// quantity the allocator ranks on.
//   Register-use census of the original (a memory-operand use is not a
//   register use and does not count):
//     esi = entries, 12 uses (1 lea + 11 push esi)
//     edi = button,  11 uses (11 `[edi+0x60]`; the 12th use is the reload,
//                       which reads [esp+0x34] and so is not a register use)
//     ebp = sel,     13 uses (1 mov + 11 `[ebp+0x138]`)
//     ebx = orders,   1 use (1 push ebx)
//   Parameters are allocated before locals, and within each group by use
//   count. Ours has entries 12 and button 12, so the tie goes to parameter 1
//   and button takes esi. The original has entries 12 and button 11, so
//   entries takes esi. The single missing memory read is therefore both the
//   2 byte size difference and the esi/edi swap; nothing else is wrong.
//   Corollary for whoever retries: any byte-neutral way of REMOVING one
//   register use of `button` (or adding one of `entries`) flips the pair, but
//   then the size is 1327 and the instruction still differs, so it cannot
//   reach MATCH on its own.
// Shapes retried this pass, every one byte-identical at 84.1%: `sizeof
//   (&button);`, a dead `__inline void Dead(Entry_00419be0**)`, `: (button =
//   button);`, a local `Entry_00419be0* const& cref = button`, `: *(Entry_00419be0**)
//   &button;`, `: ((Entry_00419be0*(&)[1])&button)[0];`, the condition written
//   as `((char*)&entries[button->index])[0] == 1`, `Entry_00419be0* const e
//   = ...`, a comma operator around the whole ternary, `if (!(e->type == 1))`,
//   `e = button; if (...) e = &entries[...];` (83.8%, wrong order), an inner
//   pointer helper doing `p = &en[b->index]; if (p->type != 1) p = b;` (83.3%),
//   and helpers taking the whole selection by value in both argument orders.
//   Moving the `orders` declaration below the selection drops to 46.9%
//   because its initializer then becomes a later statement and the
//   `lea ebx, [eax+0x2c76]` moves into the middle of the first block, which
//   confirms declaration order is a real allocator input here.
//
// space-bunny-free, third pass, still 84.1% (code unchanged, 1 real run).
// New lead, from the MATCHED sibling handler 0x41a490 (src/unsorted/
// 0x41a490.cpp, 100%), which the caller passes the very same two arguments to:
// * That file proves the author's idiom for this family: a file-scope
//   `static inline int Contains(Entry* entries, char* text, int index)`
//   owning its own `char name[32]` and doing FUN_0049fed0 plus strstr, and a
//   local `int index = menu->index` hoisted before the chain. Its prologue
//   loads the Menu* parameter into a register (`mov edx, [esp+0x30]` then
//   `mov ebx, [edx+0x60]`), so a parameter read for `->index` is normally
//   register-allocated exactly as here. The hoisted index is what differs
//   between the two functions, not the parameter's handling.
// * So the shapes still worth a run are structural, not expression-level:
//   rewrite this function with the same `Contains` helper taking
//   `menu->index` as its third argument (the name buffer then lives in the
//   inlined helper rather than in the outer frame), and with a local
//   `int index` handed to it.
// * Second untried lead: the two parameters are DIFFERENT types in the
//   caller (0x41aa00 passes `Menu_0041aa00* menu, Entry_0041aa00* entries`,
//   and 0x41a490's mangled name carries both types), so the real source
//   probably declares the first parameter as a Menu-like struct and needs a
//   cast for the fallback `e = (Entry*)menu`. A cast in the false arm is a
//   structurally different expression tree from the 11 `menu->index` loads,
//   which is the one thing the brief says can break a load CSE. My run of
//   this was cut off by the time limit, so it is untested, not refuted.
// Shapes re-tested this pass from /Fa listings, all still `mov ebp, esi`:
// the statement-level `if (e->type != 1) e = button;` (it does change the
// compare to `mov cl, [edx+ecx*2]`, but not the fallback),
// `e = *(Entry_00419be0**)&button;`, and an `Entry* m = button;` alias with
// all 11 index loads routed through `m` (VC5 forwards the copy: the listing
// shows `mov esi, _button$` and every `m` use through esi).
// Build/scratch/0x419be0/try.sh compiles a selection variant and prints the
// prologue from the /Fa listing in about six seconds, which is the fast way
// to test this; a variant file is `//SUB old => new` lines then `//SEL` and
// the replacement selection statement.
// space-bunny-free, fourth pass, still 84.1%, code unchanged (2 real runs, ~30 free
// scratch scores). Every shape listed below produces BYTE-IDENTICAL output at
// 84.1%, so the head cannot be moved from the expression at all:
// * the sibling's own signature: first parameter declared `Menu_00419be0*`
//   (index at +0x60) with `(Entry_00419be0*)button` in the false arm. This was
//   the one lead the notes above left open; it is now REFUTED, the cast tree
//   makes no difference at all.
// * an inlined helper carrying the selection, with the fallback arm by value
//   (`Fall(button)`, `Fall2(button)` with a live body), by reference
//   (`Entry_00419be0*&`), by pointer-to-pointer (`Entry_00419be0**` fed
//   `&button`) and with the TRUE arm by reference. All of them are substituted
//   by the inliner: the listing still says `mov ebp, esi`, so a reference or a
//   `&button` does NOT survive as a home-slot read. That is the mechanism the
//   66.6% lvalue-`?:` variant uses, and it only works because its RETURN is a
//   reference, which forces the addresses out; a by-value return does not.
//   Note for anyone re-deriving that variant: it cannot be written without a
//   named temporary, because `&entries[button->index]` is a prvalue and VC5
//   refuses to bind it to `Entry_00419be0*&` (error C2664), and the temporary
//   is exactly the extra stack store that costs it 18 points.
// * a local copy of the parameter for the 11 index reads with the raw parameter
//   left for the false arm, in three spellings: `void* b = (void*)button`,
//   `Entry_00419be0* b = &button[0]`, `char* b = (char*)button` with the reads
//   cast back. The listing shows `_button$` in all three: VC5 coalesces every
//   copy to the parameter, so the two variables never exist.
// * `&entries[0]` in place of `entries` as the first argument of FUN_0049fed0,
//   in the first call and in all eleven, i.e. the cheapest way to add a live
//   graph node to `entries` and break the use-count tie. Folded away.
// * `register` on the local and on both parameters.
// * `char name[16]` scores 80.9%, so the 32-byte buffer is confirmed.
// * the 6 permutations of the three local declarations reproduce the 46.9% /
//   84.1% split already in the notes and add nothing.
// Two pieces of new evidence for whoever retries:
// * The inverted ternary `type != 1 ? button : &entries[i]` shows VC5 hoists the
//   FIRST arm's assignment above the address computation, so arm order is
//   observable. The original assigns `ebp` from the address first and from
//   `button` second, exactly as our form does, so the home-slot read is not an
//   "arm position" effect.
// * `mov ebp, [esp+0x34]` cannot be a spill reload. A spilled value would be
//   reloaded before its eleven later uses as well, and the frame is only 0x20
//   with the name buffer at [esp+0x10], so there is no local at [esp+0x34] and
//   the original never stores there: it is a materialisation of a value that
//   was NOT promoted to a register in that one leaf block, while the same value
//   is promoted everywhere else. That is a register-priority decision, which is
//   why the two diffs are one: `button` has 11 register uses in the original and
//   12 in ours, so the allocator's ranking puts `entries` first there and
//   parameter 1 first here. Nothing found so far can make `button` a second
//   variable or `entries` a thirteenth user, and that is what it would take.
#include <string.h>

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

#pragma pack(push, 1)
struct Entry_00419be0 {
    unsigned char type;                // +0x0
    char unknown_1[0x60 - 0x1];
    int index;                         // +0x60
    char unknown_64[0x138 - 0x64];
    short state;                       // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Game_00419be0 {
    char unknown_0[0x2c76];
    char orders[0x2cc3 - 0x2c76];      // +0x2c76
    unsigned char orderMode;           // +0x2cc3
    char unknown_2cc4[2];
    unsigned char orderFlags;          // +0x2cc6
};
#pragma pack(pop)

extern Game_00419be0* g_game;

void __stdcall FUN_0049fed0(Entry_00419be0* entries, char* name, int index);
void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_0048cf30(void* a, int b, Class_00438760 kind, int d, int e, int f);

static inline void SetOrderMode(unsigned char mode)
{
    g_game->orderMode = mode;
    g_game->orderFlags = g_game->orderFlags & 0xf7;
}

// FUNCTION: 0x419be0
int __stdcall FUN_00419be0(Entry_00419be0* button, Entry_00419be0* entries)
{
    char name[32];
    void* orders = g_game->orders;
    Entry_00419be0* e = entries[button->index].type == 1 ? &entries[button->index] : button;

    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "MOVE")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(2);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "STOP")) {
        SetOrderMode(1);
        FUN_0048cf30(orders, 0, "STOP", 0, 0, 0);
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "ATTACK")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(3);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "BLAST")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(4);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "DEFEND")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(7);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "REPAIR")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(8);
        }
        FUN_0047f1a0("specialorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "PATROL")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(9);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "RECLAIM")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(0xc);
        }
        FUN_0047f1a0("specialorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "CAPTURE")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(0xd);
        }
        FUN_0047f1a0("specialorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "UNLOAD")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(5);
        }
        FUN_0047f1a0("specialorders", 0);
        return 1;
    }
    FUN_0049fed0(entries, name, button->index);
    if (strstr(name, "LOAD")) {
        if (e->state == 0) {
            SetOrderMode(1);
        } else {
            SetOrderMode(6);
        }
        FUN_0047f1a0("immediateorders", 0);
        return 1;
    }
    return 0;
}
