// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Partial (84.5%). Everything but one register pair matches: the original
// keeps `this` in esi and the wrapped index ix in edi before the scan (and
// reloads them into those registers after the "eligible" print), while this
// version swaps them. The stack slots, the block order and the scan registers
// all agree.
//
// What fixed the block order (65.7% to 84.5%): the scan is an inline helper,
// IsReusable, with one `return` per outcome, that also does the empty-buffer
// test and both debug prints, exactly like the sibling 0x461c20 (which
// matches). Tried without moving the swap: declaration orders, ix declared in
// or outside the loop, `++ix`, int ix, a `self` local, a count local, a
// NextIndex helper, a Grow() wrapper, `break` to a shared reuse tail, printing
// `head` in the force path, the helper as a member of the buffer class, helper
// parameter order. An N-declarations sweep (0 to 1200) and headers.py leave it
// at 84.5%, so the difference is in the source, not the compiler state.

void FUN_00461170(const char* fmt, ...);
unsigned int FUN_004b6340();

struct Packet_004629b0;

class Class_004629b0 {
public:
    char unknown_0[8];
    int count;                          // +0x8
    char unknown_c[4];
    Packet_004629b0* first;             // +0x10

    void FUN_004629b0();
};

struct Packet_004629b0 {
    char unknown_0[0xc];
    Class_004629b0* owner;              // +0xc
    int queued;                         // +0x10
    int sentTime;                       // +0x14
    char unknown_18[4];
    Packet_004629b0* next;              // +0x1c
};

class Class_00461fd0 {
public:
    int FUN_00461fd0(int unused, int size);
};

class Class_00461b10 {
public:
    int head;                           // +0x0
    char unknown_4[4];
    Class_004629b0** array;             // +0x8
    unsigned int count;                 // +0xc
    char unknown_10[8];
    int minRetain;                      // +0x18

    Class_004629b0* FUN_00461b10();
};

static inline int IsReusable(Class_004629b0* buf, int minRetain)
{
    if (buf->count == 0)
        return 1;
    Packet_004629b0* p = buf->first;
    unsigned int now = FUN_004b6340();
    int mustBeSentBefore = now - minRetain;
    FUN_00461170("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                 now, minRetain, mustBeSentBefore);
    for (; p != 0; p = p->next) {
        if (p->owner != buf)
            break;
        if (p->queued >= 0)
            return 0;
        if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
            return 0;
    }
    FUN_00461170("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                 minRetain);
    return 1;
}

// FUNCTION: 0x461b10
Class_004629b0* Class_00461b10::FUN_00461b10()
{
    for (;;) {
        if (count > 0) {
            unsigned int ix = head + 1;
            if (ix >= count)
                ix = 0;
            Class_004629b0* buf = array[ix];
            if (IsReusable(buf, minRetain)) {
                buf->FUN_004629b0();
                head = ix;
                return buf;
            }
            if (count >= 0x22) {
                buf->FUN_004629b0();
                head = ix;
                FUN_00461170("force-allocated a previously-used buffer, ix=%ld\n", ix);
                return buf;
            }
        }
        if (((Class_00461fd0*)this)->FUN_00461fd0(0x10, 0x320) == 0)
            return 0;
    }
}
