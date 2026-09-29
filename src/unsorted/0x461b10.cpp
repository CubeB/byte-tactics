// Decompiled by DeepSeek V4.1 Flash and Claude Opus 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// Partial (86.0%, 270 bytes). The original is 272 bytes and keeps `this` in esi
// and the wrapped index ix in edi; this version has that pair right (so the
// whole register swap the earlier attempt was stuck on is fixed), but it pays
// for the flip with one extra `mov edi, [esi]` (a second read of head) and is
// therefore 2 bytes short, not identical.
//
// Mechanism found: the allocation is decided by whether some value lives across
// the loop body. Hoisting a loop-spanning copy of head (`int h = head;` before
// the loop and `h = head;` before the grow call) makes MSVC give edi to that
// value and esi to `this`; without it MSVC gives edi to `this` and esi to ix
// (the earlier 84.5% shape, kept in build/scratch/0x461b10/b_base.cpp). The
// original has esi=this with only one head read per iteration, so its extra
// live use of `this` must be one that folds away; nothing tried so far folds:
// helper as a free static or a member of the buffer or of the pool, helper
// taking (self, buf) or (buf, minRetain), inverted early return, `int ix`,
// `unsigned long` casts, accessor/getter spellings, a self local, loop-carried
// ix or buf, buf declared at function scope, helper result in a local,
// short-circuit `count == 0 || IsReusable(...)` (274 bytes), N-declarations and
// headers.py. The only lever that moved the pair was a second live head read.

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
    int h = head;
    for (;;) {
        if (count > 0) {
            unsigned int ix = h + 1;
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
        h = head;
        if (((Class_00461fd0*)this)->FUN_00461fd0(0x10, 0x320) == 0)
            return 0;
    }
}
