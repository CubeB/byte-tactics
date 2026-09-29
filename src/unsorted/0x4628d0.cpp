// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5, retried by deepseek-v4.1-flash. Names are provisional.
// Appends `size` bytes at `data` to the buffer's inline storage (at +0x14),
// fills in the output packet `p`, and bumps the buffer's packet count.
// `value` is the previously queued packet (0x462710 passes its tail), stored
// in the packet's prev field at +0x18.
//
// Partial (96.6%). One instruction is missing: after the copy the original
// reloads `length` (mov eax, [ebx+0xc]) for `p->offset`; this version reuses
// the value loaded for the size check, kept in eax through the copy by `len`.
// Writing `p->offset = length` gives the reload, but then MSVC loads `value`
// into ecx above the offset store and rotates the registers of the count and
// length updates (90.7%). No store order (all 120 tried), inline helper for
// the copy or the packet setup, reference or pointer to `length`, or rewritten
// size check gives both at once. Taking the copy destination from the member
// (`buffer + length`, not `buffer + len`) is what fixes the lea operand order.
// An N-declarations sweep (0 to 600) and headers.py change nothing, so the
// difference is in the source, not the compiler state.
//
// Retry (deepseek-v4.1-flash) confirmed it: every source form that produces the
// post-copy reload (`p->offset = length`, `= (int)length`, `= this->length`,
// local read after the copy, `&buffer[length]` destination, offset via a
// reference or pointer, `length = length + size` update, an inline helper, and
// the early-return shape) lands at exactly 90.7%. The reload alone perturbs the
// global register allocation: `value` goes to ecx and is loaded above the
// offset store, and the count/length update rotates ecx to edx. Dummy function
// definitions placed before the target (compiler state) and an early-return
// guard do not change this (early return drops to 77.3%). All scratch variants
// that score above this one's 96.6%: none.
#include <string.h>

void FUN_00461170(const char* fmt, ...);

class Class_004628d0;

struct Packet_004628d0 {
    char unknown_0[4];
    int offset;                        // +4
    int size;                          // +8
    Class_004628d0* owner;             // +0xc
    char unknown_10[8];                // +0x10
    int value;                         // +0x18
    Packet_004628d0* next;             // +0x1c
};

class Class_004628d0 {
public:
    char unknown_0[4];                 // +0
    int firstIndex;                    // +4
    int count;                         // +8
    int length;                        // +0xc
    Packet_004628d0* first;            // +0x10
    char buffer[0x416];                // +0x14

    int FUN_004628d0(Packet_004628d0* p, int index, const void* data,
                     unsigned int size, int value);
};

// FUNCTION: 0x4628d0
int Class_004628d0::FUN_004628d0(Packet_004628d0* p, int index, const void* data,
                                 unsigned int size, int value)
{
    FUN_00461170("adding packet %ld (data=\"%s\")\n", index,
                 (const char*)data + 1);
    FUN_00461170("current buffer length: %ld, toadd=%ld, max=%ld\n", length,
                 size, 0x42a);
    if (length + size <= 0x42a) {
        unsigned int len = length;
        memcpy(buffer + length, data, size);
        p->offset = len;
        p->owner = this;
        p->size = size;
        p->value = value;
        p->next = 0;
        length += size;
        if (count++ == 0) {
            firstIndex = index;
            FUN_00461170("set first packet ix to: %ld\n", index);
            first = p;
        }
        FUN_00461170("assigned packet count this buf: %ld\n", count);
        return 1;
    }
    FUN_00461170("out of space in buffer!\n");
    return 0;
}
