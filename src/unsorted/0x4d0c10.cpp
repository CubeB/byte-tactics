// Decompiled by Opus. Names are provisional.

struct Rec_004d0c10 {
    unsigned short unknown_0;  // +0x0
    unsigned short head;       // +0x2
    unsigned short next;       // +0x4
};

extern Rec_004d0c10* DAT_00526ff0;

// Follows the next links from entry index's head and returns the last record
// of the chain. The caller at 0x4d12eb uses the result (mov edi, eax); without
// the return value MSVC is free to keep the index out of eax.
// FUNCTION: 0x4d0c10
int __stdcall FUN_004d0c10(int index)
{
    unsigned int cur;
    unsigned short nxt;
    for (cur = DAT_00526ff0[index].head; (nxt = DAT_00526ff0[cur].next) != 0; cur = nxt) {
    }
    return cur;
}
