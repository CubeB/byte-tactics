// Decompiled by Opus. Names are provisional.
// Frees the `count` packets assigned from index `start` (wrapping at the
// pool size), removing each from its owning queue, then resets the range.
void FUN_00461170(const char* fmt, ...);

struct Packet_004629b0;

class Class_00462ae0 {
public:
    void FUN_00462ae0(void* param);
};

struct Packet_004629b0 {
    char unknown_0[0xc];
    Class_00462ae0* owner;             // +0xc
    char unknown_10[0x20 - 0x10];
};

struct Pool_004629b0 {
    char unknown_0[0x28];
    Packet_004629b0* packets;          // +0x28
    unsigned int size;                 // +0x2c
};

class Class_004629b0 {
public:
    Pool_004629b0* pool;               // +0x0
    int start;                         // +0x4
    int count;                         // +0x8
    int field_c;                       // +0xc
    int field_10;                      // +0x10

    void FUN_004629b0();
};

// FUNCTION: 0x4629b0
void Class_004629b0::FUN_004629b0()
{
    int n = count;
    if (n > 0) {
        unsigned int size = pool->size;
        FUN_00461170("freeing packets assigned. range: %ld for %ld\n", start, n);
        unsigned int i = start;
        Packet_004629b0* packets = pool->packets;
        while (n--) {
            FUN_00461170("initialize freeing packet %ld\n", i);
            Packet_004629b0* p = &packets[i];
            i++;
            p->owner->FUN_00462ae0(p);
            p->owner = 0;
            if (i >= size)
                i = 0;
        }
    }
    field_c = 0;
    count = 0;
    start = -1;
    field_10 = 0;
}
