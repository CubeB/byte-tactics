// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (58.3%). The body, loop rotation and the buf->count==0 early exit
// match. What still differs: the two spilled temporaries are swapped. The
// original keeps `this` in esi and the wrapped index in edi; this build chooses
// edi for `this` and esi for the index. All the loop registers agree
// (buf=ebp, now=ebx, p=esi, mustBeSentBefore=edi). Tried: unsigned/signed
// index, declaration order, ternary and if/else wrap forms (those flip the
// register but lose the branchy store/reload wrap), an inlined wrap helper,
// local `count`, `self` aliases, every standard header set, and defining the
// immediately preceding real functions; none moved the choice. The
// `not_eligible` block also ends up outlined after the return blocks instead
// of between grow and the shared reset, which costs a few more bytes.

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

// FUNCTION: 0x461b10
Class_004629b0* Class_00461b10::FUN_00461b10()
{
    Class_004629b0* buf;
    unsigned int ix;
    int minRetain;
    Packet_004629b0* p;
    unsigned int now;

    do {
        if (this->count > 0) {
            ix = this->head + 1;
            if (ix >= this->count)
                ix = 0;
            buf = this->array[ix];
            minRetain = this->minRetain;
            if (buf->count == 0) {
                buf->FUN_004629b0();
                this->head = ix;
                return buf;
            }
            p = buf->first;
            now = FUN_004b6340();
            int mustBeSentBefore = now - minRetain;
            FUN_00461170("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                         now, minRetain, mustBeSentBefore);
            if (p != 0) {
                do {
                    if (p->owner != buf)
                        goto not_eligible;
                    if (p->queued >= 0)
                        goto eligible;
                    if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
                        goto eligible;
                    p = p->next;
                } while (p != 0);
            }
            goto not_eligible;
        eligible:
            if (this->count >= 0x22) {
                buf->FUN_004629b0();
                this->head = ix;
                FUN_00461170("force-allocated a previously-used buffer, ix=%ld\n", ix);
                return buf;
            }
        }
    grow:
        if (((Class_00461fd0*)this)->FUN_00461fd0(0x10, 0x320) == 0)
            return 0;
    } while (1);
not_eligible:
    FUN_00461170("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                 minRetain);
    buf->FUN_004629b0();
    this->head = ix;
    return buf;
}
