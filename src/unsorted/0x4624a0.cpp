// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro. Names are provisional.
//
// mimo-v2.6-pro retry (issue 3458): best 81.7 percent, 569 of 570 bytes (was
// 78.9). Three levers lifted it:
//  (a) Wrap the whole if-body in a flat `do { ... } while (0);` region (the
//      0x461020 / 0x461750 pattern) and make the outer loop a
//      `do { ... } while ((loc.n = queue.count) != 0);` with NO `return 1`
//      after it inside the region. C2 then tail-duplicates the return-1
//      epilogue exactly as the original has it: the latch fallthrough keeps
//      its own `mov eax, 1; pops` copy at 0x4626b0, the return 0 sits at
//      0x4626bf, and the three je edges merge into the end block at 0x4626cb
//      with the interleaved `pop edi; pop esi; pop ebp; mov eax, 1; pop ebx`
//      form. Without the region every return 1 merges into one block and the
//      latch comes out `je <merged>; jmp <top>`.
//  (b) `int sent` (signed) gives the `jle` of the `0 < sent` test (`unsigned`
//      gives `jbe`).
//  (c) `#include <windows.h>` (any of windows.h/stdio.h/stdlib.h/string.h)
//      flips the entry->base/entry->offset load order to the original's
//      `mov edx,[edi+4]; mov eax,[edi+0xc]`.
//
// What still differs (all one allocation cluster):
//  - q is kept live across the "extracted packet" log call in ebx
//    (`lea ebx,[edx+eax+0x14]`), where the original computes
//    `add eax,edx` (base+offset) and reads [eax+0x14]/lea [eax+0x15], then
//    RECOMPUTES base+offset+0x14 for FUN_004614e0 (`lea edx,[eax+ecx+0x14]`
//    from fresh entry field loads). So ebx holds the constant 0 in the
//    original through the whole inner loop (q dead there) and dpid borrows
//    ebx only in the send block, with `xor ebx,ebx` rematerialised at
//    0x46269f; ours has q borrow ebx and restores the zero after the sent
//    increment (`xor ebx,ebx` before the `mov [esp+0x1c],eax`).
//  - sent increments with load/inc/store instead of `inc dword ptr [esp+0x1c]`
//    (downstream of the same ebx split).
//  - the loop-top `sent = 0` store is emitted early (between the count load
//    and the GetFirst test, which forces the register-form `mov eax,[count];
//    cmp eax,ebx`); the original materialises `xor edi,edi` and stores
//    `mov [esp+0x24],edi` in the "assigning packets" call group, leaving the
//    count test as `cmp dword ptr [esi+0x38],ebx`. Placing `sent = 0` after
//    the headFrame read sinks the store PAST the call (76.8 percent), not to
//    the call group.
//  - `cmp ecx, edx` where the original has `cmp edx, ecx` (frame vs headFrame
//    operand order, identical loads and registers). Flipping the source also
//    flips the load order, so it must be a scheduler artifact of a shape not
//    found.
//  - the send block does not hoist `mov ebp,[DAT_0051e2f4]` before the
//    "bytes to send" call (dpid in ebx is reproduced by a local
//    `int id = dpid`, which fixes the tail `mov eax,[esi+0x38]; test eax,eax;
//    je; xor ebx,ebx` but costs the DATptr hoist and the `neg edx` register,
//    net 80.5 percent).
//
// Tried without gain (all measured with check.py on scratch copies): q-dead
// recomputation (66-68 percent, the zero leaves ebx and sent takes a callee-
// saved register); q as base+offset kept live (78.0); a PktData struct
// overlay for [eax+0x14]/[eax+0x15] (67.0 dead / 78.5 live); force reused as
// the counter (byte-identical); sent address escapes and ClearSent/IncSent
// inline helpers writing through int* (80.7 at best, no flush at the call);
// sent = 0 in the for-init comma, in the log call argument, as a declaration
// with initialiser, spelled 0L/'\0'/false/0u/(int)0 and as count&0,
// count*0, count-count (all fold or sink past the call); dead-copy register
// moves (entry->frame = f) which do not compile away; headFrame/frame locals
// in both orders and inline FrameOf/SameFrame helpers for the compare (loads
// always follow the cmp operand order here, the original has them reversed);
// declaration-order swaps of sent/i/entry; a hoisted int* dp local for
// DAT_0051e2f4 (grows the frame one slot and shifts every reference);
// extract-block inline helpers (62.1); headers.py and headers.py --cpp (768
// sets, 81.7 is the ceiling with the current source).
//
// Older notes (deepseek-v4.1-flash, space-bunny-free, deepseek-v4.1, Claude
// Sonnet 5.5): the file reached 78.9 percent by keeping q live across the
// extracted-packet logging call so the counter spills to the argument home
// [esp+0x1c]; the full experiment log is in git history.
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

#include <windows.h>

// FUNCTION: 0x4624a0
int Class_004624a0::FUN_004624a0(int force)
{
    int now;
    now = FUN_004b6340();
    FUN_00461170("player: %ld, ticks betw sends=%lu, nextsend=%lu, gametimereal=%lu\n",
                 dpid, ticks, nextSend, now);
    if (now >= nextSend || force != 0) {
    do {
    nextSend = ticks + now;
    Locals_004624a0 loc;
    loc.n = (int)&loc;
    loc.n = queue.count;
    if (0 == loc.n)
        return 1;
    int sent;
    int i;
    Packet_004624a0* entry;
    entry = 0;
    do {
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
    } while (loc.n != 0);
    } while (0);
    }
    return 1;
}