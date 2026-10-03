// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.

// #5018 (Claude Opus 5.5): still 90.2%, gave up after about an hour. The file
// now uses the real DirectSound interfaces (the toolchain's <dsound.h> for
// IDirectSound/IDirectSoundBuffer; IDirectSound3DBuffer is declared by hand
// because that header is DirectX 3, and DAT_004fcf68 is its IID). The calls
// are GetStatus, GetCurrentPosition, DuplicateSoundBuffer, SetCurrentPosition,
// QueryInterface, SetPosition, SetMinDistance, SetMaxDistance, SetMode,
// Release, SetVolume and Play. The bytes are the same as the hand-written
// vtables (score 366 either way).
// New measurements, all with the plain for loop (80.2%, 642 bytes) unless
// stated:
// * The do-while only works because its test reads bestidx: the same loop
//   tested on slot, volume or `unit == 0 && unit != 0` (folded) spills `this`
//   but leaves bestidx in memory and the shared zero in edi (70.7 to 78.6%).
//   Constant-false wrappers (`int retry = 0; do .. while (retry)`,
//   `while (1) { .. break; }`) fold before the allocator sees a loop.
// * bestidx stays in memory even with no `this` competition: removing the
//   DAT loop, the room loop or the table loop, an extra in-loop read of
//   bestidx, up to four extra reads after the loop, or slot's in-loop store
//   all leave it at [esp+0x1c]. A `char` scan index is the only change that
//   gave the original's prologue (`xor ebp,ebp / mov esi,ecx`, this spilled
//   at entry), and then best took ebp instead of bestidx (77.9%).
// * Also flat: every order of the unit/slot/bestidx initialisers (with best
//   before or after the set test), bestidx/best initialised in the for
//   header or after the set test, a function-scope index shared by all three
//   loops, DWORD status/play/write at the top, HRESULT temporaries, an
//   explicit pointer plus index for the scan, inline member helpers for the
//   DAT check, the room loop, the table insert, the 3D setup or the whole
//   tail (the big ones are not inlined), a goto or a break-and-test form of
//   the table loop, and /Gi.
// * The permuter from the plain-loop dsound source: 12,489 candidates in 18
//   minutes, 80.2% throughout (best score 992, cosmetic only).
// * 50 MATCHED functions spill `this` at entry and reload it after a loop
//   (0x4629b0, 0x438760 and 0x461b10 are small ones to study); in all of them
//   the loop has its own high-pressure locals.
// Lead: C2.EXE decompiles cleanly with Ghidra (the frame layout code was
// found that way for 0x4cac40, see that file and the #5018 pull request), so
// the global allocator's priority and live-range splitting can be read
// directly instead of guessed.

// Space Bunny Free, #4337: 82.2% -> 90.2% (653 bytes against 646). The lever is
// not the declaration order but MSVC's weighting of register priority by loop
// nesting: the set scan has to sit one loop level deeper, and then the
// allocator gives `this` esi (spilled to [esp+0x18], reloaded at 0x4cf650 and
// 0x4cf699), the shared zero ebp (which later becomes bestidx) and `best` ebx,
// exactly as the original does. The `do { ... } while (bestidx >= 4);` around
// the scan is that extra level; it is a semantic no-op (bestidx is a set index,
// so it stays below 4) and it is what buys the rotation. `best` must be declared
// inside the do block so its `xor ebx,ebx` lands in a register of its own
// instead of being coalesced with the shared zero, and the priority update has
// to read `best = a;` before `bestidx = i;` to match the original's two moves.
// What still differs (653 against 646 bytes): (1) the do-while test
// `cmp ebp,4 / jge` after the scan, 6 bytes the original does not have; (2) the
// original has `mov edi,[esp+0x30]; xor ebx,ebx; cmp edi,ebp` before the
// `if (set == 0)` branch while ours folds the null test into
// `cmp DWORD PTR [esp+0x30],ebp` and loads set afterwards, 1 byte. Both follow
// from the extra loop level: with the plain for loop the second difference goes
// away but the whole this/zero/bestidx rotation reverts (80.2%).
// Tried and no better: an outer `for (int k = 0; k < 1; k++)` around the scan
// (adds a stack slot); `do { ... } while (0)` and `for(;;){...break;}` (MSVC
// folds them away, no rotation); a do-while test on best, arg2, unit, pos, DAT
// or set (rotation happens but scores 908 to 1074); a do-while test on a member
// of `this` (count, field_2c, field_34) which puts `this` back in ebp, so the
// lever is a live range crossing the back edge rather than nesting as such;
// giving bestidx one more read inside the scan with a dead
// `else if (bestidx < 0) bestidx = i;` (right rotation, 654 to 655 bytes,
// 87.2%); the scan as a pointer walk over buffers (this=esi but the loop becomes
// lea/sar and scores 76.8%); hoisting the scan index i out of the for (63.8%);
// a self-assignment `bestidx = bestidx` (emits no code and changes nothing);
// Class_004cf570 deriving from Class_004cf180 (no change); helpers for the
// priority fetch, the channel setup and the whole tail (no change); every
// declaration order of unit/slot/best/bestidx, self assignments, an explicit
// self pointer, and about 1900 permuter candidates, none better. Best permuter
// score for this source is 366 (0 would be a MATCH).
// Not MATCH (90.2% as left here, 82.2% for the version before this one): the
// notes below record the earlier rounds, all of which failed to move the
// this/zero/bestidx rotation.

// #3912 (deepseek-v4.1-flash): uninitialized bestidx (74.4%/642B) and swapping the DAT guard with the refresh loop (73.6%/645B) both regress, so 82.2% stands.
// Issue 2304 retry: baseline 80.2% confirmed; delaying best initialization scored 79.5%.
// Issue 2486 retry by deepseek-v4.1-flash: still 80.2%. The original's zero
// constant in ebp is really bestidx's initial 0 coalesced with the constant;
// ebp is reused as bestidx after the set loop. this keeps esi except inside
// the set loop, where the index i reuses esi and this is spilled to [esp+0x18]
// and reloaded after. This version instead keeps this in ebp forever and
// bestidx in memory ([esp+0x1c]) with the constant in esi, so it is 4 bytes
// short. Tried this retry: the count loop as a label/goto (68.4%, +17 bytes),
// a static inline wrapper around FUN_004cf180 taking this as an argument (no
// change), bestidx declared before unit/slot (no change), and initialising
// unit/slot from bestidx to force coalescing (no change).
// Issue 2774 retry by deepseek-v4.1-flash: still 80.2%. Declaration order of
// the initialized locals, an explicit self pointer (self = this), register
// int bestidx, swapping the best/bestidx assignment order, and naming bestidx
// in the null test (folded back into the shared zero) all left the
// this/zero/bestidx register rotation unchanged.
// Remaining mismatch is the this/zero/bestidx register allocation rotation described below.
// GPT-6.1-sol retry in #1928: 6 direct checks kept 80.2%. Delayed
// initialization, explicit self guard, and reusing bestidx for the zero guard
// did not improve the allocator rotation described below.
// Refinement in issue-1928-r1: nine more direct checks and the combined
// checker run kept 80.2%. Reordering variable declarations scored 77.2%; the
// best source was restored. No MATCH was reached.
// Issue 2854 retry by deepseek-v4.1-flash: 11 more scratch checks, still no
// MATCH. Best checker score is now 82.2% at exactly 646 bytes, but it is an
// alignment artifact from a wrong loop shape (see the note below), so read the
// 80.2% for-loop notes first. New facts: the this/zero/bestidx rotation is
// completely insensitive to the declaration order and initialisation order of
// unit, slot and bestidx (six permutations all compiled byte-identical at 642
// bytes), to chained/separate zero assignments, to "if (!set)" versus
// "if (set == 0)", to a local p = set[i], to unsigned or register loop indexes,
// to a pointer-walking set pointer, and to the type of best. Rewriting the set
// loop as "while (1) { if (i >= 4) break; ... }" scores 82.2% at 646 bytes and
// is the version left in this file, but it TOP-tests the loop ("cmp esi,4;
// jge"), while the original is bottom-tested ("xor esi,esi ... inc esi;
// cmp esi,4; jl 0x4cf5e5"); the gain is only the size match, not real progress.
// The structurally correct source (the for loop) is the 80.2% version; every
// count-loop rewrite (for(;;)+break, while(1)+break, do/while) and the DAT loop
// as a while were tried and all scored the same or worse. The one remaining
// difference stays the register assignment: this must be esi with a spill to
// [esp+0x18], the shared zero must be ebp, and bestidx must reuse ebp. Ours
// keeps this in ebp, the zero in esi, and bestidx in memory.
// Picks a free sound channel from a four entry set of sound objects: a valid
// one in the set is taken as is, otherwise the one with the highest priority is
// recycled, or set[0] is cloned when the set has a free slot. The chosen object
// is then set up and filed in the channel table at +0x38.
// Layout: argument 1 is the four entry set, argument 2 is passed to the +0x3c
// method, argument 3 is a position (int x/y/z). Table fields: count +0x30,
// counter +0x34, buffers +0x38, priorities +0xb8, flags +0x138, factory +0x24.
//
// Historical (superseded, the version of the file before #4337): not MATCH
// (80.2 for loop, 82.2 as left then): the code was byte-identical to the
// original except for one allocator state. The original keeps this in esi
// and the shared zero constant in ebp, spilling this to [esp+0x18], so ebp later
// becomes bestidx and esi is reused as the loop index. This version keeps this
// in ebp and the zero in esi, so bestidx gets a stack slot at [esp+0x1c] and
// slot moves to [esp+0x18]. Every other difference (the [esp+0x18] this reload,
// mov edi/esi choices, the set[bestidx] index) follows from that one rotation.
// The file as left uses the wrong top-tested while(1) set loop purely because
// check.py scores it 82.2 instead of 80.2; restoring the for loop gives the
// 80.2 baseline with the correct bottom-tested loop.
// GPT-6.1-sol verification: direct check.py scored 80.2% before and after the
// 128 common-header sweep; no header set matched. The optional C++ header sweep
// was stopped after 271 of 768 combinations to stay within the worker timebox.
// Tried and did NOT change the rotation: the interface calls as virtual
// __stdcall methods (that fixed 24 points on its own; function-pointer members
// were wrong) versus data members; an explicit self pointer or reference alias
// for this; an inline helper for the DAT scan taking this as an argument; the
// declarations of unit/slot/best/bestidx in every order and at every scope;
// bestidx declared after the null check; an early-continue loop form; swapping
// the first loop's condition order; set aliased to a local; and Windows/stdio/
// stdlib/string/math/dsound headers.
// mimo-v2.6-pro retry: still 82.2 (while form) / 80.2 (for form). Tried and
// confirmed no change: self=this used for all field accesses (copy-propagated),
// bestidx as scan-loop index with reset (compiler uses ecx for the scan index),
// bestidx uninitialized (w5: bestidx stays in memory), i declared at top before
// unit (zero+i moves to edi, this stays ebp), i at top after bestidx (same),
// inline helpers ScanBusy(this)/MakeRoom(this) for the pre-loop code (no change),
// do-while set loop (same as for). The this/zero/bestidx rotation is confirmed
// insensitive to all source-level shuffling tried across many sessions. The
// original keeps this in esi with a spill to [esp+0x18] and bestidx in ebp;
// ours keeps this in ebp and bestidx in memory. No construct found to flip this.
// deepseek-v4.1-flash #3399 retry: still 82.2 (while form) / 80.2 (for form).
// Confirmed the this/zero/bestidx rotation is insensitive to: an N-declaration
// sweep of 101 values (0..400 step 4, all byte-identical); defining the real
// preceding function 0x4cf540 above ours in the same file; explicit self and
// self_180 pointers; a named zero variable used for all three compares;
// naming bestidx in the DAT check and the set==0 check; a live dead-sum across
// the set-loop calls; a dead this-field store pair at the top; the register
// keyword; windows.h/dsound.h/string.h; loop-counter type and scope; and every
// declaration order. All left `xor esi,esi` (zero) / `mov ebp,ecx` (this).
#include <windows.h>
#include <dsound.h>

extern int DAT_0051ff48;

extern const GUID DAT_004fcf68;

struct Pos_004cf570 {
    int x, y, z;
};

struct IDirectSound3DBuffer : public IUnknown {
    virtual HRESULT __stdcall GetAllParameters(void* p) = 0;
    virtual HRESULT __stdcall GetConeAngles(LPDWORD a, LPDWORD b) = 0;
    virtual HRESULT __stdcall GetConeOrientation(void* p) = 0;
    virtual HRESULT __stdcall GetConeOutsideVolume(LPLONG p) = 0;
    virtual HRESULT __stdcall GetMaxDistance(float* p) = 0;
    virtual HRESULT __stdcall GetMinDistance(float* p) = 0;
    virtual HRESULT __stdcall GetMode(LPDWORD p) = 0;
    virtual HRESULT __stdcall GetPosition(void* p) = 0;
    virtual HRESULT __stdcall GetVelocity(void* p) = 0;
    virtual HRESULT __stdcall SetAllParameters(void* p, DWORD apply) = 0;
    virtual HRESULT __stdcall SetConeAngles(DWORD a, DWORD b, DWORD apply) = 0;
    virtual HRESULT __stdcall SetConeOrientation(float x, float y, float z, DWORD apply) = 0;
    virtual HRESULT __stdcall SetConeOutsideVolume(LONG v, DWORD apply) = 0;
    virtual HRESULT __stdcall SetMaxDistance(float d, DWORD apply) = 0;
    virtual HRESULT __stdcall SetMinDistance(float d, DWORD apply) = 0;
    virtual HRESULT __stdcall SetMode(DWORD mode, DWORD apply) = 0;
    virtual HRESULT __stdcall SetPosition(float x, float y, float z, DWORD apply) = 0;
};

class Class_004cf180 {
public:
    void FUN_004cf180();
};

class Class_004cf570 {
public:
    int field_0;
    int field_4;
    float field_8;
    float field_c;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int field_20;
    IDirectSound* field_24;
    int field_28;
    int field_2c;
    int count;                              // +0x30
    int field_34;
    IDirectSoundBuffer* buffers[0x20];      // +0x38
    int priority[0x20];                     // +0xb8
    int flags[0x20];                        // +0x138

    int FUN_004cf570(IDirectSoundBuffer** set, LONG volume, Pos_004cf570* pos);
};

// FUNCTION: 0x4cf570
int Class_004cf570::FUN_004cf570(IDirectSoundBuffer** set, LONG volume, Pos_004cf570* pos)
{
    IDirectSoundBuffer* unit = 0;
    int slot = 0;
    int bestidx = 0;
    if (DAT_0051ff48 != 0) {
        for (int i = 0; i < 0x20; i++) {
            if (buffers[i] != 0 && flags[i] == 1)
                return 0;
        }
    }
    while (count >= field_2c)
        ((Class_004cf180*)this)->FUN_004cf180();
    if (set == 0)
        return 0;
    // The scan sits one loop level deeper than the code needs, which is how this
    // reproduces the original's register allocation: MSVC weights register
    // priority by loop nesting, and only with the extra level does it keep `this`
    // in esi (spilled to [esp+0x18] and reloaded after the scan) with the shared
    // zero in ebp. The test never fires, since bestidx is a set index and stays
    // below 4, so it is semantically a no-op, but it does cost the six byte
    // `cmp ebp,4 / jge` after the scan that the original does not have. Without
    // it the whole rotation reverts and the file drops back to 80.2%.
    do {
        DWORD best = 0;
        for (int i = 0; i < 4; i++) {
            if (set[i] != 0) {
                DWORD status;
                if (set[i]->GetStatus(&status) != 0)
                    return 0;
                if (status == 0) {
                    unit = set[i];
                    break;
                }
                DWORD play, write;
                set[i]->GetCurrentPosition(&play, &write);
                if (play > best) {
                    best = play;
                    bestidx = i;
                }
            } else {
                slot = i;
            }
        }
    } while (bestidx >= 4);
    if (unit == 0) {
        if (slot > 0) {
            if (field_24->DuplicateSoundBuffer(set[0], &unit) != 0)
                return 0;
            set[slot] = unit;
        } else {
            unit = set[bestidx];
            unit->SetCurrentPosition(0);
        }
    }
    IDirectSound3DBuffer* chan;
    if (unit->QueryInterface(DAT_004fcf68, (void**)&chan) == 0) {
        if (field_4 == 0 || pos == 0) {
            chan->SetMode(2, 0);
        } else {
            chan->SetPosition((float)pos->x, (float)pos->y, (float)pos->z, 0);
            chan->SetMinDistance(field_8, 0);
            chan->SetMaxDistance(field_c, 0);
            chan->SetMode(0, 0);
        }
        chan->Release();
    }
    if (unit->SetCurrentPosition(0) != 0)
        return 0;
    if (unit->SetVolume(volume) != 0)
        return 0;
    if (unit->Play(0, 0, DAT_0051ff48 != 0) != 0)
        return 0;
    for (int j = 0; j < 0x20; j++) {
        if (buffers[j] == 0) {
            buffers[j] = unit;
            priority[j] = ++field_34;
            flags[j] = DAT_0051ff48 != 0;
            count++;
            return 1;
        }
    }
    return 1;
}
