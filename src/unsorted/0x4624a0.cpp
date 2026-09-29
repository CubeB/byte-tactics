// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
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
    unsigned int now = FUN_004b6340();
    FUN_00461170("player: %ld, ticks betw sends=%lu, nextsend=%lu, gametimereal=%lu\n",
                 dpid, ticks, nextSend, now);
    if (now < nextSend) {
        if (force == 0)
            return 1;
    }
    nextSend = now + ticks;
    Locals_004624a0 loc;
    loc.headFrame = 0;
    loc.n = (int)&loc;
    loc.n = queue.count;
    if (loc.n == 0)
        return 1;
    int i;
    int sent;
    while (1) {
        loc.headFrame = queue.GetFirst()->frame;
        sent = 0;
        FUN_00461170("assigning packets to frame number: %ld\n", frame);
        for (i = 0; i < loc.n; i++) {
            // Two calls, not one: the original's inlined code has the diamond
            // of a two-return helper and then a second count test of its own.
            Packet_004624a0* entry = queue.GetFirst();
            queue.Pop();
            if (entry->frame == loc.headFrame) {
                char* p = (char*)entry->base + entry->offset;
                FUN_00461170("extracted packet (len=%ld, type=%d, data=\"%s\")\n",
                             entry->size, (unsigned char)p[0x14], p + 0x15);
                entry->queued = frame;
                entry->time = FUN_004b6340();
                if (DAT_00513000.FUN_004614e0(
                        (unsigned char*)((char*)entry->base + entry->offset + 0x14),
                        entry->size) == 0)
                    return 0;
                sent = sent + 1;
            } else {
                queue.Push(entry);
            }
        }
        queuedBytes = 0;
        if (sent > 0) {
            FUN_00461170("sending %ld packets in frame: %ld\n", sent, frame);
            *DAT_0051e2f4 = (dpid != 0) ? -1 : frame;
            int nbytes = DAT_0051e2f8;
            FUN_00461170("bytes to send to (DPID)(%ld): %ld\n", dpid, nbytes);
            DAT_005129d0.FUN_004626e0(g_game + 0x14, loc.headFrame, dpid, DAT_0051e2f4, nbytes);
            DAT_0051e2f8 = (DAT_0051e2f4 != 0) ? 4 : 0;
            frame = frame - 1;
            if (frame >= -1)
                frame = -2;
            if (queue.count == 0)
                return 1;
        }
        loc.n = queue.count;
        if (loc.n != 0)
            continue;
        return 1;
    }
}
