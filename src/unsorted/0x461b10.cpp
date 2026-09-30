// Decompiled by DeepSeek V4.1 Flash and Claude Opus 5.5, finished by deepseek-v4-flash. Names are provisional.
// Partial (99.0%, 272 bytes). The original keeps `this` in esi and the wrapped
// index ix in edi; earlier attempts had that pair swapped (esi=ix, edi=this).
// The swap is fixed here without any extra instruction: reading head into ix
// and then assigning it straight back to head (`head = ix;`, a no-op that the
// compiler folds, but whose use of head is enough to move it off edi) makes
// MSVC give edi to ix and esi to `this`.
//
// The one remaining difference is the order of the two reloads on the
// reuse-success path, after the inlined IsReusable's final FUN_00461170 call:
//   original: mov esi,[esp+0x1c]  then  mov edi,[esp+0x18]
//   ours:     mov edi,[esp+0x18]  then  mov esi,[esp+0x1c]
// Same two instructions (reload this and ix for `head = ix;`), swapped. The
// force-alloc path already reloads ix then this like the original; only this
// scheduler pick differs. Rewriting the store as this->head = ix, a duplicated
// store, a temporary value, an unsigned long/int ix, a pointer store, and a
// separate return local all keep the same 99.0%.

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
            unsigned int ix = head;
            head = ix;
            ++ix;
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
