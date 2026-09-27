// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// NOT a match (74.9%). The ebx/ebp assignment is correct here, but this loop
// shape is semantically wrong and must be fixed before it can match:
//
//   The inner loop is written with the not-eligible handling as the loop's
//   fall-through block (the queued/sent checks `break`), so `p == 0` exits the
//   loop into the NOT-eligible branch. In the original, both the entry test and
//   the bottom test jump to the ELIGIBLE block (0x461cec): `0x461ca8 je
//   0x461cec`, `0x461cc6 je 0x461cec`. So the source loop was
//   `for (; p != 0; p = p->next) { if (p->owner != owner) break; if (p->queued
//   >= 0) goto notEligible; ... }` (or the equivalent compound-condition form
//   `for (; p != 0 && p->owner == owner; ...)`), which is semantically correct
//   but compiles to 51.5% because MSVC then allocates idx to ebp and owner to
//   ebx, and does not rotate the loop.
//
//   Register allocation is driven by the block layout: only when the
//   not-eligible block is the loop's fall-through does MSVC give idx=ebx and
//   owner=ebp (matching the original). Making the correct-semantics loop
//   rotate while keeping that layout is what is still missing. Also note the
//   original lays its blocks out as [loop][notEligible][eligible][force], and
//   its bottom test is `test esi,esi; je <eligible>; jmp <body>` while ours is
//   `test esi,esi; jne <body>`.

void FUN_00461170(const char* fmt, ...);
unsigned int FUN_004b6340();

class Class_004629b0 {
public:
    void FUN_004629b0();
};

class Class_00461fd0 {
public:
    void FUN_00461fd0(int a, int b);
};

struct Packet_00461c20;

struct Buffer_00461c20 {
    char unknown_0[8];
    int inUse;                          // +0x8
    char unknown_c[4];
    Packet_00461c20* first;             // +0x10
};

struct Packet_00461c20 {
    int field_0;                        // +0x0
    char unknown_4[8];
    Buffer_00461c20* owner;             // +0xc
    int queued;                         // +0x10
    int sentTime;                       // +0x14
    char unknown_18[4];
    Packet_00461c20* next;              // +0x1c
};

class Class_00462710 {
public:
    char unknown_0[0x18];
    unsigned int minRetain;             // +0x18
    int index;                          // +0x1c
    char unknown_20[8];
    Packet_00461c20* packets;           // +0x28
    unsigned int poolSize;              // +0x2c

    Packet_00461c20* FUN_00461c20(int param_1);
};

// FUNCTION: 0x461c20
Packet_00461c20* Class_00462710::FUN_00461c20(int param_1)
{
    unsigned int idx;
    Packet_00461c20* packet;
    Buffer_00461c20* owner;

    for (;;) {
        FUN_00461170("current packet pool index: %ld\n", index);
        idx = index + 1;
        if (idx >= poolSize)
            idx = 0;
        packet = &packets[idx];
        owner = packet->owner;
        if (owner == 0)
            goto done;
        int minRetain = this->minRetain;
        if (owner->inUse == 0)
            goto freeBuffer;
        {
            Packet_00461c20* p = owner->first;
            unsigned int now = FUN_004b6340();
            int mustBeSentBefore = now - minRetain;
            FUN_00461170("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                         now, minRetain, mustBeSentBefore);
            for (; p != 0; p = p->next) {
                if (p->owner != owner)
                    goto eligible;
                if (p->queued >= 0)
                    break;
                if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
                    break;
            }
        }
        // NOTE: original sends p == 0 here to `eligible`, not to `notEligible`.
    notEligible:
        if (poolSize >= 0x6a4) {
            FUN_00461170("force-initializing a in-use buffer which was not eligible for reuse!\n");
            ((Class_004629b0*)owner)->FUN_004629b0();
            goto done;
        }
        ((Class_00461fd0*)this)->FUN_00461fd0(0, 0x320);
        continue;
    eligible:
        FUN_00461170("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                     minRetain);
    freeBuffer:
        ((Class_004629b0*)owner)->FUN_004629b0();
        goto done;
    }
done:
    index = idx;
    FUN_00461170("current packet pool index set to: %ld\n", idx);
    packet->field_0 = param_1;
    return packet;
}
