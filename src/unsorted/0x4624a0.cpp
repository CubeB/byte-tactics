// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Claude Sonnet 5.5 (#3250): 78.9% (was 77.9%), 555 of 570 bytes. Found by an
// automated source-mutation hill-climb scored with check.py --sym (about 900
// variants): `unsigned int sent` and `unsigned int nbytes` locals, `0 != x`
// operand order in the conditions, `q = base + 0x14 + offset`. The structural
// residual described below is unchanged.
// #3046 retry by GPT-6.1-sol: six checks retained 77.9%; goto and payload
// pointer variants did not fix the remaining register/liveness differences.
//
// deepseek-v4.1-flash retry 3 (issue 2865, timeboxed): 70.8 -> 77.9 percent
// (555 of 570 bytes). Two source levers:
//  (a) Set the packet counter with a separate `sent = 0;` statement at the top
//      of the while body (before the headFrame read) instead of in the for-init
//      comma expression. That spills `sent` to the argument home [esp+0x1c]
//      (`inc dword ptr [esp+0x1c]` in the success path) and frees ebp for `i`,
//      which is the register/space split the original has. (Placing the same
//      store after the headFrame read or after the log call scores 73.4.)
//  (b) Wrap the whole body in `if (now >= nextSend || force != 0) { ... }` with
//      one trailing `return 1;` after the while loop, instead of an early
//      `return 1;` inside the now/nextSend test. Now every early return-1 path
//      shares a single epilogue like the original (0x4626cb) instead of the
//      first one being emitted inline.
// Residual, all downstream of keeping the extracted-packet pointer `q` live
// across the logging call: q takes ebx (`lea ebx,[base+offset+0x14]`), so ebx
// is no longer the constant 0 and needs a `xor ebx,ebx` restore; that also
// turns the counter into load/inc/store rather than `inc dword ptr [esp+0x1c]`.
// The original recomputes base+offset+0x14 for FUN_004614e0 (q dead across the
// call), but writing that here collapses the allocation back to 51.8 percent
// (546 bytes), so q-live is kept. Also the loop-top count test is register-form
// (`mov eax,[esi+0x38]`) where the original tests memory (`cmp [esi+0x38],ebx`).
//
// deepseek-v4.1-flash retry 2 (issue 2865, timeboxed): lifted 67.8 -> 70.8
// percent (556 of 570 bytes). The lever was the twin 0x435a20's "what is live
// across a call": keep an ADDRESS live across the extracted-packet logging
// call. `char* q = (char*)entry->base + entry->offset + 0x14;` used for the
// `data="%s"` byte (as `*q`) and `q + 1` in the log call, then passed as-is to
// FUN_004614e0, keeps q (a heap address) live across the "extracted packet"
// call. MSVC then spills the packet counter to the argument slot and starts
// keeping the constant 0 in ebx (`cmp reg, ebx`, `mov [esi+0x20], ebx`), which
// is the whole old 15-byte gap. Recomputing the FUN_004614e0 argument from
// `entry->base + entry->offset + 0x14` instead of reusing q falls straight
// back to 67.8 (address dead across the call), which confirms the mechanism.
// `loc.headFrame = 0;` must now be DELETED (it added 5 bytes and dropped the
// score); the deleted store and the combined `for (i = 0, sent = 0; ...)`
// init were the other two changes. headers.py is flat at 70.8.
// Residual (all downstream of one register choice, `sent` still takes ebp
// where the original keeps `sent` in the argument slot and `i` in ebp):
//  - the inner for counter `i` lives at [esp+0x1c] here, `sent` in ebp; the
//    original is the exact opposite (i in ebp, `inc dword ptr [esp+0x1c]` for
//    sent). Swapping declaration order, decl scope, for-init comma order,
//    unsigned/long/reference spellings, and hoisting `i = 0` before the log
//    call all leave the allocation unchanged.
//  - the force==0 return is emitted inline here; the original branches to the
//    shared return-1 block at the bottom (`je 0x4626cb`).
//  - the send block colours dpid in eax instead of ebx/ebp.
//
// deepseek-v4.1-flash retry (2557, timeboxed): best remains 67.8 percent (555
// of 570 bytes). Measured with check.py --sym on scratch copies, none better:
//  - reordering the inlined Pop to load readIdx before count--: identical.
//  - loc.n = 0 instead of loc.headFrame = 0, or dropping that init store: 547
//    bytes, 66.5 percent, so the headFrame store is load-bearing for alignment.
//  - `for (;;)` instead of `while (1)`: 67.3 percent.
//  - `if (now < nextSend && force == 0) return 1;` instead of the nested if:
//    byte-identical.
//  - reusing `force` as the sent counter: byte-identical (sent still in ebx).
//  - caching GetFirst() in `entry` before reading ->frame: 67.3 percent.
//  - a local `int* p = DAT_0051e2f4;` used for the store and the send call, to
//    put a pointer live across the logging call: 547 bytes, 64.0 percent.
// Nothing moved the constant 0 into ebx, so the 15-byte gap is unchanged.
//
// space-bunny-free third pass: still 67.8 percent (555 of 570 bytes), unchanged.
// TWIN TEST: the matched 0x435a20 (100 percent) has exactly this shape (an
// `xor ebx,ebx` in its prologue, then `mov [mem],ebx` for a 0 store and
// `cmp reg,ebx` much later), so this is outcome type 3, a plateau rather than a
// wall: the wanted allocation exists. Its own notes name the lever: WHAT IS
// LIVE ACROSS A CALL. There a stack address live across a call makes C1 park
// the long-lived 0 in ebx; handed a non-address instead, the 0 is
// rematerialised and everything else moves. Here nothing in this source is
// live across a call except `this`, `entry`, `i` and the counter, which is why
// the counter takes the fourth callee-saved register.
// Measured this pass, all with check.py on a scratch copy:
//  - scratch/0x4624a0/vC.cpp respells the inlined Pop and Push in the shape
//    the original itself shows (`r = idx + 1; idx = r; if (r >= 0x400) idx = 0`,
//    giving `mov [m],idx / cmp / jl / mov [m],0 / jmp`): 552 bytes, 65.3
//    percent, so that source form is ruled out and the current diamond shape
//    stays.
//  - the 15-byte gap itemised: three one-use literal zeros (the original has
//    `mov [esi+0x3c],ebx`, `mov [esi+0x40],ebx`, `mov [esi+0x20],ebx`, 3 bytes
//    each, where we emit `mov [m],0`, 7 to 10 bytes each) plus nine `cmp reg,
//    ebx` against our `test reg,reg`. Two further differences are layout, not
//    allocation: the original's redundant `jmp 0x4624e3` after the force test
//    and its `mov ecx,edx; cmp ecx,eax` copies in the two index wraps.
// Making the zero non-rematerialisable did NOT work: escaping the counter
// (scratch v1, `force = (int)&force;`) does push the counter out of ebx into
// the argument home, but the zero still never enters a register and the escape
// costs 4 bytes.
//
// deepseek-v4.1 (2047) note: this is still the best version (67.8%, 555 vs 570
// bytes). The one thing still differing is the original's `int 0` held in ebx
// for the whole function (`cmp reg, ebx`, `cmp [esi+0x38], ebx`, `mov [esi+0x3c],
// ebx`, `mov [esi+0x20], ebx`), which also forces `sent` (the packet counter)
// out of registers into the argument home slot `[esp+0x1c]` (`inc dword ptr
// [esp+0x1c]`). Ours folds every 0 to `test reg,reg` / `mov [mem],0` per block
// and enregisters `sent` in ebx instead, which is the whole 15-byte gap.
// New experiment this pass: scratch/0x4624a0/v1.cpp reuses `force` itself as
// the counter and escapes its address (`force = (int)&force;` right after
// `nextSend = now + ticks;`, the same dead-store trick that already pins
// `Locals_004624a0` to memory). That does move the counter back to the argument
// slot, but the escape costs 4 bytes and the score fell to 65.2% (559 bytes),
// so it is not the answer; the constant 0 still never reaches ebx.

// space-bunny-free second pass (1642): still 67.8 percent, 555 against 570
// bytes, still the single missing ebx = 0 register. New results, all measured
// with check.py --sym on scratch copies (no check.py runs spent):
//  - headers.py: all 128 header sets; the best are 67.8 percent for <none>,
//    <windows.h>, <stdio.h> and <stdlib.h>. Headers are not the lever.
//  - scratch/0x4624a0/vA.cpp: reuse the parameter `force` itself as the sent
//    counter (`force = 0; ... force = force + 1; if (force > 0) ...`) instead
//    of a separate `int sent;`. Byte for byte the same allocation as this file
//    (555 bytes, 67.8 percent, `sent` still in ebx, `inc ebx`, `test ebx,ebx`),
//    so the original's use of the dead argument slot for the counter is a
//    frame-slot allocation decision, not a source level parameter reuse.
//  - scratch/0x4624a0/vB.cpp: put the counter in the escaped local struct as a
//    third member. 551 bytes but only 58.9 percent: the third slot changes
//    `sub esp, 8` to `sub esp, 0xc` and moves every stack reference, which
//    confirms the two dword frame is exactly `n` plus `headFrame` and the
//    counter really does live in the argument slot.
//  - frame arithmetic, worth writing down: at entry ESP = S, so [esp+0x18] is
//    the return address, [esp+0x1c] is the single argument, and the two locals
//    are [esp+0x10] and [esp+0x14]. So `n` is the first local, `headFrame` the
//    second, and `sent` shares the dead argument slot: three memory variables,
//    no third local slot.
//  - the missing web: this build does enregister constants locally (the inlined
//    Pop region gets `xor ecx,ecx` for both `count <= 0` and `writeIdx = 0`),
//    so MSVC 5 can hold 0 in a register. What it will not do is keep one 0
//    live across the whole function, which is what the original does (one ebx
//    from 0x4624d5/0x4624e1 through 0x46262e, then reused for dpid at
//    0x46263c). `xor ebx,ebx` in both arms of the now<nextSend test, and once
//    more at 0x46269f, is that web being rematerialised per block, so the
//    source must name a 0 valued variable that C1 refuses to fold, and every
//    spelling tried so far (plain int, unsigned, bitfield, address taken,
//    two definitions) folds or spills.
//
// deepseek-v4.1-flash retry (1296): best is now 67.8% (was 67.3%). Moving the
// per-packet entry declaration out of the for loop to function scope with an
// = 0 initialiser lifts the byte score by 0.5 points; the missing 15 bytes are
// still the constant 0 that the original keeps in ebx for the whole function.
// A literal 0 is rematerialised here; int zero = 0 in many placements and a
// zero = 0 assignment in both arms of the now-nextSend test (which would
// explain the two xor ebx,ebx) all fold back to test reg,reg and mov [m],0.
// Not a match (67.3% with this version; 66.0% without the extra
// `loc.headFrame = 0;` line, 59.0% for the previous clean-locals version).
//
// space-bunny-free pass, re-confirmed the diagnosis and added these results:
//  - the whole remaining difference is still one register decision. The four
//    callee-saved registers hold, in the original, esi=this, edi=entry,
//    ebp=`i` and ebx=THE CONSTANT 0, with `sent` left in the parameter slot
//    at [esp+0x1c]. This file allocates esi, edi, ebx=`sent`, ebp=`i` and
//    rematerialises every 0 (`test reg,reg`, `mov [mem],0`), so it is 15
//    bytes short (555 against 570) purely from `cmp reg,ebx` (2 bytes) against
//    `test reg,reg` (2 bytes) plus the three `xor ebx,ebx` and the extra
//    one-byte reloads that follow from it.
//  - NEW, re-measured here: naming a local `int zero = 0;` at the top of the
//    function and writing every comparison and every zero store through it
//    (`force == zero`, `loc.n == zero`, `sent > zero`, `queuedBytes = zero`,
//    `dpid != zero`, `loc.n != zero`) changes NOTHING: build/scratch/0x4624a0/
//    e1.cpp (this file plus that `zero` variable) scores 67.3 percent with
//    the same 555 bytes, i.e. MSVC 5 folds
//    the variable's single definition back into a constant before the
//    allocator runs, exactly as the notes below say. Combined with the
//    `&loc` address escape already in place (so that n and headFrame stay in
//    memory) this is the one spelling still untried, and it does not help.
//  - the original's `xor ebx,ebx` appears at the top of BOTH arms of the
//    `now < nextSend` test and once more before the latch at 0x4626a1, but
//    `ebx` holds `dpid` in between (0x46263c), and the `queue.count == 0` test
//    at 0x46269b uses `test eax,eax`, not `cmp eax,ebx`. So ebx is an
//    enregistered constant that the allocator is free to reuse, not a source
//    variable: whatever produced it, it is not `x = 0` in the source.
//  - the two `for`-loop counters are correct: `i` is in ebp and matches, and
//    the Pop wrap test is the only other difference (`cmp edx,eax` here
//    against `mov ecx,edx / cmp ecx,eax` there), which is downstream of ebx.
// The whole remaining difference is still one decision by MSVC 5's register
// allocator: the original keeps the int constant 0 in ebx for the whole
// function (every `== 0`, `<= 0` and `= 0` is `cmp reg, ebx` or
// `mov [mem], ebx`), and spills BOTH `n` ([esp+0x10]) and `headFrame`
// ([esp+0x14]) to the stack, while also keeping `sent` in the parameter slot
// ([esp+0x1c], `inc dword ptr [esp+0x1c]`). Here MSVC rematerialises the 0 at
// every use (`test reg, reg`, `mov [mem], 0`), keeps `headFrame` in ebx, and
// `sent` lands in ebx too once headFrame is forced out.
//
// What is settled (do not undo):
//  - `n` and `headFrame` must both be stack locals: the original has
//    `sub esp, 8` and reads `n` from [esp+0x10] and `headFrame` from
//    [esp+0x14]. Separate plain locals (v0) give `push ecx` (one slot) with
//    `headFrame` in ebx; a two-member local struct (v4) gives `sub esp, 8`
//    but MSVC still promotes `headFrame` to ebx.
//  - A local struct `Locals_004624a0 { int n; int headFrame; }` whose address
//    escapes keeps both members in memory. `loc.n = (int)&loc;` is a stand-in
//    for whatever the original source did to make the struct addressable;
//    without it headFrame is promoted to ebx again.
//  - `int zero = 0;` does NOT survive here: the compiler constant-propagates
//    it and rematerialises (`test reg, reg`). A micro with no inlined helper
//    keeps the same variable in a register, so the inlined queue helpers'
//    own `return 0` (pointer nulls) are what let the int 0 fold.
//  - The extra `loc.headFrame = 0;` is an artifact of the address escape: the
//    original has no store to [esp+0x14] before the loop top, so it should be
//    removed once headFrame spills for the right reason. It is here only
//    because it lifts check.py's byte-alignment score from 66.0 to 67.3.
//
// Tried without effect (all still 58.5-59% with plain locals): `int zero`
// declared at several positions and used in every comparison/store; passing
// `zero` as a parameter to the inlined GetFirst/Pop/Push; `force & 0` and
// `force - force` spellings; `int headFrame` inside the loop; all declaration
// orders; `do/while` and `while(n)` loop forms; `unsigned` headFrame; a
// pointer to headFrame; all 128 header sets and 5 extra C++ headers
// (headers.py); `bool`/`short` types. The 0-120 unused-declaration sweep was
// already flat (see the previous notes below).
//
// The remaining register differences, all downstream of the ebx choice:
//  - the inlined Pop stores `readIdx` and reloads it for the wrap test; the
//    original keeps `readIdx+1` in edx and compares a copy in ecx.
//  - the extracted-packet block loads `[edi+4]`/`[edi+0xc]` in the opposite
//    order, and the send block puts dpid in eax/edx instead of ebx/ebp.
//
// deepseek-v4.1 fourth pass (2047): still 67.8 percent, 555 against 570 bytes.
// Seven check.py runs on scratch copies, every one byte-identical to this file
// (67.8 percent): `int sent = 0;` at function scope instead of `int sent;`
// (v20), `Packet_004624a0* entry;` without the `= 0` (v21), a Pop that caches
// the index in a temp (v22), an `int zero = 0;` local used at every
// zero comparison and store (v30), and the same zero threaded into the inlined
// GetFirst as a parameter (v31). One shape regressed: a Push that caches
// `writeIdx + 1` in a temp (v23) and the Pop+Push combination (v24) drop to
// 546 bytes and 52.4 percent, so the index must stay written as
// `writeIdx = writeIdx + 1;` with the wrap test on the member.
// Flag probe (free, scratch scoring): `/Oa` gives 550 bytes / 40.8 percent and
// `/Ow /Oa` 550 bytes / 59.9 percent, both worse than the default flags, so the
// constant-in-ebx choice is not a flag artifact of aliasing assumptions.
// Still missing: the whole 15-byte gap is the allocator keeping the literal 0
// in ebx (`cmp reg, ebx`, `mov [esi+0x3c], ebx`, `mov [esi+0x20], ebx`) with
// `sent` spilled to the dead argument home `[esp+0x1c]` (so `inc dword ptr
// [esp+0x1c]`), plus the original's `jmp` after the `force == 0` test (ours
// falls through) and the original's `mov ecx, edx; cmp ecx, eax` copies in the
// two index wraps. Naming or escaping `sent` does not bring 0 into a register.

// deepseek-v4.1 third pass (2047): still 67.8 percent, 555 against 570 bytes;
// the file is unchanged because every variant scored lower or equal. Measured
// with check.py this pass:
//  - removing the `loc.headFrame = 0;` store (the original has no store of 0 to
//    [esp+0x14] before the loop, so the source's init was dead there): 547
//    bytes but 66.5 percent, so the store must stay to keep the byte alignment.
//  - forcing `sent` into memory by taking its address (`sent = (int)&sent;`):
//    559 bytes, 61.6 percent. The escape does push it out of ebx, but the
//    constant 0 still does not appear in a register and the extra code costs
//    more than it buys.
//  - `unsigned int sent;` instead of `int sent;`: byte-identical, 67.8 percent.
//  - `int sent = 0;` declared inside the while body (shorter live range):
//    byte-identical, 67.8 percent.
//  - `int sent; int i;` swapped declaration order: byte-identical, 67.8 percent.
// Conclusion for the next pass: the missing 15 bytes are one allocator choice
// (constant 0 kept in ebx, `sent` spilled to the argument slot) that no source
// spelling of `sent` reached. The next lever to try is a source shape that
// makes the zero uses heavier than the counter: e.g. more zero comparisons in
// the same block, or the counter living in a member instead of a local.
// Previous worker's notes (space-bunny-free, 59.0%): the inlined Pop wording
// was tried in both shapes (0x4623b0 and 0x4623e0); a flat 0-120
// unused-declaration sweep stayed flat, so the remaining gap is source shape,
// not compiler state.
unsigned int __cdecl FUN_004b6340();
void __cdecl FUN_00461170(const char* fmt, ...);

extern char* g_game;
extern int* DAT_0051e2f4;
extern int DAT_0051e2f8;

class Class_004614e0 {
public:
    int FUN_004614e0(unsigned char* data, unsigned int len);
};

class Class_004626e0 {
public:
    void FUN_004626e0(void* session, int from, int value, void* data, int size);
};

extern Class_004614e0 DAT_00513000;
extern Class_004626e0 DAT_005129d0;

struct Packet_004624a0 {
    int frame;                       // +0x0
    int base;                        // +0x4
    int size;                        // +0x8
    int offset;                      // +0xc
    int queued;                      // +0x10
    unsigned int time;               // +0x14
};

// The ring buffer of 0x462370 (push) and 0x4623b0 (pop), inlined here.
class Queue_004624a0 {
public:
    int count;                            // +0x0
    int readIdx;                          // +0x4
    int writeIdx;                         // +0x8
    Packet_004624a0* buf[0x400];          // +0xc

    Packet_004624a0* GetFirst()
    {
        if (count > 0)
            return buf[readIdx];
        return 0;
    }

    Packet_004624a0* Pop()
    {
        if (count > 0) {
            count--;
            Packet_004624a0* value = buf[readIdx];
            readIdx++;
            if (readIdx < 0x400)
                return value;
            readIdx = 0;
            return value;
        }
        return 0;
    }

    int Push(Packet_004624a0* value)
    {
        if (count < 0x400) {
            writeIdx = writeIdx + 1;
            if (writeIdx >= 0x400)
                writeIdx = 0;
            buf[writeIdx] = value;
            count = count + 1;
            return 1;
        }
        return 0;
    }
};

class Class_004624a0 {
public:
    int field_00;                     // +0x0
    int ticks;                        // +0x4
    void** field_08;                  // +0x8
    char unknown_c[4];
    int frame;                        // +0x10
    int dpid;                         // +0x14
    char unknown_18[8];
    unsigned int queuedBytes;         // +0x20
    unsigned int nextSend;            // +0x24
    char unknown_28[0x10];
    Queue_004624a0 queue;             // +0x38

    int FUN_004624a0(int force);
};

struct Locals_004624a0 {
    int n;
    int headFrame;
};

// FUNCTION: 0x4624a0
int Class_004624a0::FUN_004624a0(int force)
{
    int now;
    now = FUN_004b6340();
    FUN_00461170("player: %ld, ticks betw sends=%lu, nextsend=%lu, gametimereal=%lu\n",
                 dpid, ticks, nextSend, now);
    if (now >= nextSend || force != 0) {
    nextSend = ticks + now;
    Locals_004624a0 loc;
    loc.n = (int)&loc;
    loc.n = queue.count;
    if (0 == loc.n)
        return 1;
    unsigned int sent;
    int i;
    Packet_004624a0* entry;
    entry = 0;
    while (1) {
        sent = 0;
        loc.headFrame = queue.GetFirst()->frame;
        FUN_00461170("assigning packets to frame number: %ld\n", frame);
        for (i = 0; i < loc.n; ++i) {
            // Two calls, not one: the original's inlined code has the diamond
            // of a two-return helper and then a second count test of its own.
            entry = queue.GetFirst();
            queue.Pop();
            if (loc.headFrame == entry->frame) {
                char* q;
                q = (char*)entry->base + 0x14 + entry->offset;
                FUN_00461170("extracted packet (len=%ld, type=%d, data=\"%s\")\n",
                             entry->size, (unsigned char)*q, q + 1);
                entry->queued = frame;
                entry->time = FUN_004b6340();
                if (DAT_00513000.FUN_004614e0(
                        (unsigned char*)q,
                        entry->size) == 0)
                    return 0;
                sent += 1;
            } else {
                queue.Push(entry);
            }
        }
        queuedBytes = 0;
        if (0 < sent) {
            FUN_00461170("sending %ld packets in frame: %ld\n", sent, frame);
            *DAT_0051e2f4 = (0 != dpid) ? -1 : frame;
            unsigned int nbytes;
            nbytes = DAT_0051e2f8;
            FUN_00461170("bytes to send to (DPID)(%ld): %ld\n", dpid, nbytes);
            DAT_005129d0.FUN_004626e0(g_game + 0x14, loc.headFrame, dpid, DAT_0051e2f4, nbytes);
            DAT_0051e2f8 = (0 != DAT_0051e2f4) ? 4 : 0;
            frame -= 1;
            if (frame >= -1)
                frame = -2;
            if (0 == queue.count)
                return 1;
        }
        loc.n = queue.count;
        if (loc.n)
            continue;
        return 1;
    }
    }
    return 1;
}