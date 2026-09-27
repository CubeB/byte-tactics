// Decompiled by space-bunny-free. Names are provisional.
// Not a match (check.py prints 59.0% on this version, 58.7% with the other
// Pop() wording, 2026-09-28). What still differs, all of it from one decision:
//  - the original keeps the literal 0 for the whole function in ebx (so every
//    `== 0`, `> 0` and `= 0` is `cmp reg, ebx` or `mov [mem], ebx`), and that
//    forces its head-frame local into a stack slot ([esp+0x14], two frame
//    dwords, `sub esp, 8`). MSVC gives ebx to my head-frame local instead, so
//    the zero is rematerialised at each use (`test reg, reg`, `mov [mem], 0`)
//    and the frame is one dword short (`push ecx`).
//  - because ecx is not free for the zero, the inlined Pop() stores readIdx
//    and reloads it, while the original compares the register it already had
//    (`inc edx; mov ecx, edx; cmp ecx, eax; mov [readIdx], edx`), and the
//    inlined Push() compares writeIdx+1 directly instead of copying it to ecx.
//  - everything after that shifts: dpid is reloaded into eax twice instead of
//    being kept in ebx, g_game+0x14 lands in eax instead of ecx, and the
//    `DAT_0051e2f8 = p ? 4 : 0` is computed in eax instead of edx.
// Tried and rejected: the 0x4623b0 Pop wording with a single count test (50%),
// a Pop that calls GetFirst() and then has its own count test (58.7%, this is
// the shape the original shows), all six declaration orders of the three
// locals, `int headFrame = 0` at declaration, comparing the two sides the
// other way round, a flat queue (no member struct, the helpers as methods of
// the class, 55.8%), an int-typed queue, a `goto` label instead of the outer
// `while (1)` (58.5%), moving the "assigning packets" print before the head
// frame read (53.6%), and the N-declarations sweep from 0 to 120, which stays
// flat at 58.5-59%, so this is a source-shape problem, not compiler state.
// Sends every queued packet whose frame field matches the frame of the entry
// at the head of the queue: pops it, appends its payload (type byte included)
// to the global outgoing buffer at 0x513000 and counts it. Entries of other
// frames are pushed back on the tail. When at least one packet went out, the
// 4-byte destination dword plus the buffer is handed to the net sender object
// at 0x5129d0 and the frame counter is dropped back to -2.

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
    int n = queue.count;
    if (n == 0)
        return 1;
    int i;
    int headFrame = 0;
    int sent;
    while (1) {
        headFrame = queue.GetFirst()->frame;
        sent = 0;
        FUN_00461170("assigning packets to frame number: %ld\n", frame);
        for (i = 0; i < n; i++) {
            // Two calls, not one: the original's inlined code has the diamond
            // of a two-return helper and then a second count test of its own.
            Packet_004624a0* entry = queue.GetFirst();
            queue.Pop();
            if (entry->frame == headFrame) {
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
            DAT_005129d0.FUN_004626e0(g_game + 0x14, headFrame, dpid, DAT_0051e2f4, nbytes);
            DAT_0051e2f8 = (DAT_0051e2f4 != 0) ? 4 : 0;
            frame = frame - 1;
            if (frame >= -1)
                frame = -2;
            if (queue.count == 0)
                return 1;
        }
        n = queue.count;
        if (n != 0)
            continue;
        return 1;
    }
}
