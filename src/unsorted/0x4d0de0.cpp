// Decompiled by space-bunny-free. Names are provisional.
// LZSS tree walk: walks the 6 byte node array of DAT_00526ff0 (parent, smaller,
// larger) starting at the root's larger link, comparing the character the
// candidate node's window position holds against the one at pos, keeping the
// longest run, and inserting or replacing a node. pos is a window position, so
// every index is masked with 0xfff into the 0x1011 byte window DAT_00526ff4.
// The inner loop counts matching bytes up to 0x11: MSVC keeps the running index
// in the parameter's register and adds it to the loop invariant (cur - pos), so
// the source needs the subtraction in a local of its own (writing the sum
// inline lets MSVC fold it back to cur + i and the frame changes).
// NOT MATCHED yet: 373 bytes original, ours 362, 57.3%. The frame (push ecx as
// the single local slot, arg1 in ecx, both epilogues), the loop shapes and the
// instruction order all match, but the register assignment is permuted: the
// original has cur in esi, delta in edi, diff in edx (and reuses edi for best
// once the inner loop is done), while this source gives diff in esi, cur in
// edi, delta in edx. That permutation also moves the tree walk, where the
// original reloads DAT_00526ff0 in each arm of the larger/smaller choice and
// this source hoists that load and the index before the test. Swapping the
// declaration order of cur/best, of delta/diff and inlining (cur - pos + j)
// were all tried and changed nothing (inlining also loses the frame).
// The par->smaller == cur test needs its (unsigned short) cast: without it the
// compare is 32 bit, as in 0x4d0b80.
struct Node_004d0de0 {
    unsigned short parent;   // +0x0
    unsigned short smaller;  // +0x2
    unsigned short larger;   // +0x4
};

struct Tree_004d0de0 {
    Node_004d0de0 nodes[0x1000];
    unsigned short root_parent;   // +0x6000
    unsigned short root_smaller;  // +0x6002
    unsigned short root_larger;   // +0x6004
};

extern Tree_004d0de0* DAT_00526ff0;
extern char* DAT_00526ff4;

// FUNCTION: 0x4d0de0
int __stdcall FUN_004d0de0(int pos, int* out)
{
    if (pos == 0)
        return 0;
    int best = 0;
    int cur = DAT_00526ff0->root_larger;
    for (;;) {
        int n, j, delta, diff;
        delta = cur - pos;
        for (n = 0, j = pos; n < 0x11; n++, j++) {
            diff = DAT_00526ff4[j & 0xfff] - DAT_00526ff4[(delta + j) & 0xfff];
            if (diff != 0)
                break;
        }
        if (n >= best) {
            best = n;
            *out = cur;
            if (n >= 0x11) {
                Node_004d0de0* par = &DAT_00526ff0->nodes[DAT_00526ff0->nodes[cur].parent];
                if (par->smaller == (unsigned short)cur)
                    par->smaller = pos;
                else
                    par->larger = pos;
                DAT_00526ff0->nodes[pos] = DAT_00526ff0->nodes[cur];
                DAT_00526ff0->nodes[DAT_00526ff0->nodes[pos].smaller].parent = pos;
                DAT_00526ff0->nodes[DAT_00526ff0->nodes[pos].larger].parent = pos;
                DAT_00526ff0->nodes[cur].parent = 0;
                return best;
            }
        }
        unsigned short* p;
        if (diff >= 0)
            p = &DAT_00526ff0->nodes[cur].larger;
        else
            p = &DAT_00526ff0->nodes[cur].smaller;
        if (*p == 0) {
            *p = pos;
            DAT_00526ff0->nodes[pos].parent = cur;
            DAT_00526ff0->nodes[pos].larger = 0;
            DAT_00526ff0->nodes[pos].smaller = 0;
            return best;
        }
        cur = *p;
    }
}
