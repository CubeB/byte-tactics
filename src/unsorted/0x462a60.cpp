// Decompiled by Opus. Names are provisional.
// Returns 1 when the buffer can be reused: none of its packets is still
// queued, and none was sent within the last `minRetain` game ticks.

void __cdecl FUN_00461170(const char* fmt, ...);
unsigned int FUN_004b6340();

class Class_00462a60;

struct Packet_00462a60 {
    char unknown_0[0xc];
    Class_00462a60* owner;             // +0xc
    int queued;                        // +0x10
    int sentTime;                      // +0x14
    char unknown_18[0x1c - 0x18];
    Packet_00462a60* next;             // +0x1c
};

class Class_00462a60 {
public:
    char unknown_0[0x8];
    int field_8;                       // +0x8
    char unknown_c[0x10 - 0xc];
    Packet_00462a60* first;            // +0x10

    int FUN_00462a60(int minRetain);
};

// FUNCTION: 0x462a60
int Class_00462a60::FUN_00462a60(int minRetain)
{
    if (field_8) {
        Packet_00462a60* p = first;
        unsigned int now = FUN_004b6340();
        int mustBeSentBefore = now - minRetain;
        FUN_00461170("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                     now, minRetain, mustBeSentBefore);
        for (; p && p->owner == this; p = p->next) {
            if (p->queued >= 0)
                return 0;
            if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
                return 0;
        }
        FUN_00461170("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                     minRetain);
    }
    return 1;
}
