// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Appends `size` bytes at `data` to the buffer's inline storage (at +0x14),
// fills in the output packet `p`, and bumps the buffer's packet count.
//
// PARTIAL: 81.9%. Semantically identical to the original. The instruction
// sequence, registers and store order all match except for two bytes:
//   1. the original reloads `length` (mov eax,[ebx+0xc]) before
//      `mov [esi+4],eax`; this version keeps the value in eax from the copy.
//   2. the copy destination is encoded as lea edi,[eax+ebx+0x14] here versus
//      lea edi,[ebx+eax+0x14] in the original (base/index swapped).
// Both come from reading the member `length` directly: that emits the reload
// and fixes the lea order, but then MSVC hoists the `value` load into ecx
// before the offset store (and uses edx for count+1, eax for the last print).
// Keeping `len` in a local reproduces the original value/count register
// allocation but drops the reload. A local `unsigned int len = length;` is
// used here because it gets 81.9% versus 77.3% for the direct member read.
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
        memcpy(buffer + len, data, size);
        p->offset = len;
        p->owner = this;
        p->size = size;
        p->value = value;
        p->next = 0;
        int old = count;
        length += size;
        count = old + 1;
        if (old == 0) {
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
