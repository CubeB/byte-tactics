// Decompiled by Opus, Sonnet, space-bunny-free, deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, Haiku and Claude Sonnet 5.5. Names are provisional.
// The SQSH compression module: the LZSS sliding window and its binary tree
// (the classic Okumura compressor's contract, replace, find-next, delete and
// insert operations), the compress and expand entry points with the preset
// window they share, the SQSH chunk packer and unpacker, and the header,
// size, checksum and encryption helpers around them (0x4d0ab0 to 0x4d1c60).
// The module's files gathered in address order; the tree is the flat 0x1001
// node array of the classic LZSS source, with node 0x1000 as its root.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Node_004d0b10;
extern Node_004d0b10* DAT_00526ff0;
extern char* DAT_00526ff4;

// Frees the window buffer allocated by 0x4d0a70.
// FUNCTION: 0x4d0ab0
int __cdecl LzssFreeWindow(void)
{
    if (DAT_00526ff4 == 0) {
        printf("Hey!  The window buffer ptr is not pointing to anything!\n");
        return -1;
    }
    free(DAT_00526ff4);
    DAT_00526ff4 = 0;
    return 0;
}

// Frees the tree allocated by 0x4d0a70.
// FUNCTION: 0x4d0ae0
int LzssFreeTree()
{
    if (DAT_00526ff0 == 0) {
        printf("Hey!  The tree ptr is not pointing to anything!\n");
        return -1;
    }
    free(DAT_00526ff0);
    DAT_00526ff0 = 0;
    return 0;
}

// The LZSS binary-tree node; the array holds 0x1000 nodes and the root.
struct Node_004d0b10 {
    unsigned short parent;           // +0x0
    unsigned short smaller;          // +0x2
    unsigned short larger;           // +0x4
};

// Looks like ContractNode() from the classic LZSS binary-tree compressor:
// the node's only child takes its place under the node's parent.
// FUNCTION: 0x4d0b10
void __stdcall LzssContractNode(int oldNode, int newNode)
{
    DAT_00526ff0[newNode].parent = DAT_00526ff0[oldNode].parent;
    if (DAT_00526ff0[DAT_00526ff0[oldNode].parent].larger == (unsigned short)oldNode)
        DAT_00526ff0[DAT_00526ff0[oldNode].parent].larger = newNode;
    else
        DAT_00526ff0[DAT_00526ff0[oldNode].parent].smaller = newNode;
    DAT_00526ff0[oldNode].parent = 0;
}

// The original calls these four out of line from LzssCompress; in one file
// /Ob2 would inline them, so the pragmas hold it back.
#pragma auto_inline(off)

// Looks like ReplaceNode() from the classic LZSS binary-tree compressor:
// newNode takes oldNode's place in the tree, and oldNode is unlinked.
// Keep the headers above: without any header MSVC swaps base and index in the
// two child loads.
// FUNCTION: 0x4d0b80
void __stdcall LzssReplaceNode(int oldNode, int newNode)
{
    int parent = DAT_00526ff0[oldNode].parent;
    if (DAT_00526ff0[parent].smaller == (unsigned short)oldNode)
        DAT_00526ff0[parent].smaller = newNode;
    else
        DAT_00526ff0[parent].larger = newNode;
    DAT_00526ff0[newNode] = DAT_00526ff0[oldNode];
    DAT_00526ff0[DAT_00526ff0[newNode].smaller].parent = newNode;
    DAT_00526ff0[DAT_00526ff0[newNode].larger].parent = newNode;
    DAT_00526ff0[oldNode].parent = 0;
}

// Follows the chain from entry index's smaller child to the last record.
// The caller at 0x4d12eb uses the result (mov edi, eax); without the return
// value MSVC is free to keep the index out of eax.
// FUNCTION: 0x4d0c10
int __stdcall LzssFindNextNode(int index)
{
    unsigned int cur;
    unsigned short nxt;
    for (cur = DAT_00526ff0[index].smaller; (nxt = DAT_00526ff0[cur].larger) != 0; cur = nxt) {
    }
    return cur;
}

// DeleteNode() of the classic LZSS binary-tree window: unlinks node t from
// the tree, promoting its only child, or its in-order successor when it has
// two children. The node fields are those of 0x4d0b80 (parent, smaller,
// larger).

// Keep <stdio.h> and <stdlib.h>, and read the parent from the array on each
// use (no local): the register allocation depends on both.
void __stdcall LzssDeleteString(int t);

// FUNCTION: 0x4d0c50
void __stdcall LzssDeleteString(int t)
{
    if (DAT_00526ff0[t].parent == 0) {
        return;
    }
    if (DAT_00526ff0[t].larger == 0) {
        int q = DAT_00526ff0[t].smaller;
        DAT_00526ff0[q].parent = DAT_00526ff0[t].parent;
        if (DAT_00526ff0[DAT_00526ff0[t].parent].larger == (unsigned short)t) {
            DAT_00526ff0[DAT_00526ff0[t].parent].larger = q;
        } else {
            DAT_00526ff0[DAT_00526ff0[t].parent].smaller = q;
        }
        DAT_00526ff0[t].parent = 0;
        return;
    }
    if (DAT_00526ff0[t].smaller == 0) {
        int q = DAT_00526ff0[t].larger;
        DAT_00526ff0[q].parent = DAT_00526ff0[t].parent;
        if (DAT_00526ff0[DAT_00526ff0[t].parent].larger == (unsigned short)t) {
            DAT_00526ff0[DAT_00526ff0[t].parent].larger = q;
        } else {
            DAT_00526ff0[DAT_00526ff0[t].parent].smaller = q;
        }
        DAT_00526ff0[t].parent = 0;
        return;
    }
    int q = DAT_00526ff0[t].smaller;
    while (DAT_00526ff0[q].larger != 0) {
        q = DAT_00526ff0[q].larger;
    }
    LzssDeleteString(q);
    if (DAT_00526ff0[DAT_00526ff0[t].parent].smaller == (unsigned short)t) {
        DAT_00526ff0[DAT_00526ff0[t].parent].smaller = q;
    } else {
        DAT_00526ff0[DAT_00526ff0[t].parent].larger = q;
    }
    DAT_00526ff0[q] = DAT_00526ff0[t];
    DAT_00526ff0[DAT_00526ff0[q].smaller].parent = q;
    DAT_00526ff0[DAT_00526ff0[q].larger].parent = q;
    DAT_00526ff0[t].parent = 0;
}

// LZSS tree walk (the same node layout as 0x4d0b80 and 0x4d0c50): walks the
// 6 byte node array of DAT_00526ff0 (parent, smaller, larger) from the root
// node at 0x1000, comparing 17 window bytes at cur + n against pos + n and
// keeping the longest run, then inserts or replaces a node. The window is the
// 0x1011 byte buffer DAT_00526ff4 and every index is masked with 0xfff.
// Keep <stdio.h> (or <windows.h>): it decides the tree addressing modes.
// FUNCTION: 0x4d0de0
int __stdcall LzssAddString(int pos, int* out)
{
    if (pos == 0)
        return 0;
    int best = 0;
    int cur = DAT_00526ff0[0x1000].larger;
    for (;;) {
        int n, j, diff;
        for (n = 0, j = pos; n < 0x11; n++, j++) {
            // Index is (cur + n), not (cur - pos) + j: it fixes the registers.
            diff = DAT_00526ff4[j & 0xfff] - DAT_00526ff4[(cur + n) & 0xfff];
            if (diff != 0)
                break;
        }
        if (n >= best) {
            best = n;
            *out = cur;
            if (n >= 0x11) {
                Node_004d0b10* par = &DAT_00526ff0[DAT_00526ff0[cur].parent];
                if (par->smaller == (unsigned short)cur)
                    par->smaller = pos;
                else
                    par->larger = pos;
                DAT_00526ff0[pos] = DAT_00526ff0[cur];
                DAT_00526ff0[DAT_00526ff0[pos].smaller].parent = pos;
                DAT_00526ff0[DAT_00526ff0[pos].larger].parent = pos;
                DAT_00526ff0[cur].parent = 0;
                return best;
            }
        }
        unsigned short* p;
        if (diff >= 0)
            p = &DAT_00526ff0[cur].larger;
        else
            p = &DAT_00526ff0[cur].smaller;
        if (*p == 0) {
            *p = pos;
            DAT_00526ff0[pos].parent = cur;
            DAT_00526ff0[pos].larger = 0;
            DAT_00526ff0[pos].smaller = 0;
            return best;
        }
        cur = *p;
    }
}

#pragma auto_inline(on)

// <io.h> and <malloc.h> are here only for their declaration count: without
// them the operand order of 0x4d0f60's bit pack and 0x4d1480's flag test
// flips. <process.h> <time.h>, <io.h> <time.h>, <mbstring.h> <time.h>,
// <process.h> <malloc.h>, <process.h> <float.h> and <io.h> <direct.h> also
// land in the window.
#include <io.h>
#include <malloc.h>

extern HANDLE DAT_0052a4f8;
extern long DAT_0052a4fc;
extern int g_lzssLockOwner;
extern int g_lzssPresetReady;
extern int g_lzssUsePreset;
extern char g_lzssPresetWindow[];
extern char g_lzssPresetTree[];

// FUNCTION: 0x4d0f60
int __stdcall LzssCompress(unsigned char *dest, unsigned char *src, int len) {
    unsigned char flags;
    unsigned int mask;
    int accum;
    unsigned char *end;
    unsigned char *base;
    unsigned char lit[0x81];
    unsigned short lenstack[0x81];
    struct {
        int cur;
        int pos;
        int count;
        int own;
    } state;
    int tid = (int)GetCurrentThreadId();

    while (1) {
        int r = InterlockedExchange(&DAT_0052a4fc, tid);
        if (r == 0) {
            g_lzssLockOwner = tid;
            state.own = 0;
            break;
        }
        if (g_lzssLockOwner == tid) {
            state.own = r;
            break;
        }
        WaitForSingleObject(DAT_0052a4f8, -1);
    }
    base = dest;
    end = src + len;
    DAT_00526ff4 = (char *)calloc(1, 0x1011);
    if (DAT_00526ff4 == 0) {
        printf("Could not alloc decompression window.\n");
        if (state.own == 0) {
            g_lzssLockOwner = 0;
            InterlockedExchange(&DAT_0052a4fc, 0);
            SetEvent(DAT_0052a4f8);
        }
        return -1;
    }
    DAT_00526ff0 = (Node_004d0b10 *)calloc(0x1001, 6);
    if (DAT_00526ff0 == 0) {
        printf("Could not alloc compression tree.\n");
        if (state.own == 0) {
            g_lzssLockOwner = 0;
            InterlockedExchange(&DAT_0052a4fc, 0);
            SetEvent(DAT_0052a4f8);
        }
        return -1;
    }
    state.pos = 1;
    if (g_lzssPresetReady != 0 && g_lzssUsePreset != 0) {
        memcpy(DAT_00526ff4, g_lzssPresetWindow, 0x1011);
        memcpy(DAT_00526ff0, g_lzssPresetTree, 0x6006);
    } else {
        DAT_00526ff0[0x1000].parent = 0;
        DAT_00526ff0[0x1000].larger = 1;
        DAT_00526ff0[0x1000].smaller = 0;
        DAT_00526ff0[1].parent = 0x1000;
        DAT_00526ff0[1].larger = 0;
        DAT_00526ff0[1].smaller = 0;
    }
    {
        int n0 = 0;
        // Plain while with the src check inside: a do-while gets rotated.
        while (n0 < 0x11) {
            if (src >= end)
                break;
            { int k = n0 + 1; DAT_00526ff4[k] = *src++; }
            n0++;
        }
        state.count = n0;
    }
    state.cur = 0;
    accum = 0;
    mask = 1;
    flags = 0;
    while (state.count > 0) {
        int j;
        // n is assigned inside each branch below, not before the if.
        int n;
        if (state.cur > state.count)
            state.cur = state.count;
        if (state.cur <= 1) {
            n = 1;
            lit[mask] = DAT_00526ff4[state.pos];
        } else {
            lenstack[mask] = (unsigned short)(((state.cur - 2) & 0xf) | (accum << 4));
            flags |= (unsigned char)mask;
            n = state.cur;
        }
        mask <<= 1;
        if (mask & 0x100) {
            *dest++ = flags;
            for (j = 0; j < 8; j++) {
                int f = flags & 0xff;
                if (!(f & (1 << j))) {
                    *dest++ = lit[1 << j];
                } else {
                    *dest++ = (unsigned char)lenstack[1 << j];
                    *dest++ = (unsigned char)(lenstack[1 << j] >> 8);
                }
            }
            mask = 1;
            flags = 0;
        }
        while (n > 0) {
                int p17 = (state.pos + 0x11) & 0xfff;
                if (DAT_00526ff0[p17].parent != 0) {
                    if (DAT_00526ff0[p17].larger == 0) {
                        LzssContractNode(p17, DAT_00526ff0[p17].smaller);
                    } else if (DAT_00526ff0[p17].smaller == 0) {
                        LzssContractNode(p17, DAT_00526ff0[p17].larger);
                    } else {
                        int q = LzssFindNextNode(p17);
                        LzssDeleteString(q);
                        LzssReplaceNode(p17, q);
                    }
                }
                if (src >= end)
                    state.count--;
                else
                    DAT_00526ff4[(state.pos + 0x11) & 0xfff] = *src++;
                state.pos = (state.pos + 1) & 0xfff;
                if (state.count != 0)
                    state.cur = LzssAddString(state.pos, &accum);
            n--;
        }
    }
    lenstack[mask] = 0;
    flags |= (unsigned char)mask;
    {
        int j;
        int cnt = 1;
        if (mask != 0) {
            do {
                cnt++;
                mask >>= 1;
            } while (mask != 0);
        }
        *dest++ = flags;
        for (j = 0; j < cnt; j++) {
            if (!((1 << j) & (flags & 0xff))) {
                *dest++ = lit[1 << j];
            } else {
                *dest++ = (unsigned char)lenstack[1 << j];
                *dest++ = (unsigned char)(lenstack[1 << j] >> 8);
            }
        }
    }
    if (DAT_00526ff0 == 0) {
        printf("Hey!  The tree ptr is not pointing to anything!\n");
    } else {
        free(DAT_00526ff0);
        DAT_00526ff0 = 0;
    }
    if (DAT_00526ff4 == 0) {
        printf("Hey!  The window buffer ptr is not pointing to anything!\n");
    } else {
        free(DAT_00526ff4);
        DAT_00526ff4 = 0;
    }
    int written = (int)(dest - base);
    if (state.own == 0) {
        g_lzssLockOwner = 0;
        InterlockedExchange(&DAT_0052a4fc, 0);
        SetEvent(DAT_0052a4f8);
    }
    return written;
}

// LZ decompressor. It first takes the window lock: a spinlock on DAT_0052a4fc
// holding the owning thread id, with g_lzssLockOwner naming the owner and
// DAT_0052a4f8 the event that releases the waiters. Then it allocates the
// 0x1011 byte sliding window, and reloads it from the template at g_lzssPresetWindow
// when the template was set up (g_lzssPresetReady) and the caller switched it on
// (g_lzssUsePreset, set around each call by the net condenser through
// LzssEnablePreset / LzssDisablePreset). The bit stream that follows is read
// through a mask that counts 1, 2, 4 ... 0x80 and then reloads a fresh flag
// byte: a clear bit is a literal byte, a set bit is a 16 bit word whose high
// 12 bits are the distance back into the window and whose low four bits are
// the length minus two, and a distance of zero ends the stream. Every output
// byte also goes into the window at the current position, which starts at 1
// and wraps at 0xfff. The window is freed at the end, the lock released if
// this thread took it, and the number of bytes written returned.
// Unused but kept: the declaration count sets the loop test's operand order.
// FUNCTION: 0x4d1480
int __stdcall LzssExpand(unsigned char* dest, unsigned char* src)
{
    unsigned char* base;
    int own;
    int tid = (int)GetCurrentThreadId();
    int i;
    int pos;
    unsigned mask;
    unsigned char flags;
    while (1) {
        int r = InterlockedExchange(&DAT_0052a4fc, tid);
        if (r == 0) {
            g_lzssLockOwner = tid;
            own = 0;
            break;
        }
        if (g_lzssLockOwner == tid) {
            own = r;
            break;
        }
        WaitForSingleObject(DAT_0052a4f8, -1);
    }
    base = dest;
    DAT_00526ff4 = (char*)calloc(1, 0x1011);
    if (DAT_00526ff4 == 0) {
        printf("Could not alloc decompression window.\n");
        if (own == 0) {
            g_lzssLockOwner = 0;
            InterlockedExchange(&DAT_0052a4fc, 0);
            SetEvent(DAT_0052a4f8);
        }
        return -1;
    }
    if (g_lzssPresetReady && g_lzssUsePreset)
        memcpy(DAT_00526ff4, g_lzssPresetWindow, 0x1011);
    pos = 1;
    flags = *src++;
    mask = 1;
    for (;;) {
        if (!(flags & mask)) {
            unsigned char c = *src;
            *dest++ = c;
            src++;
            DAT_00526ff4[pos] = c;
            pos = (pos + 1) & 0xfff;
        } else {
            int w = *(unsigned short*)src;
            src += 2;
            unsigned dist = (unsigned)w >> 4;
            int len = (w & 0xf) + 2;
            if (dist == 0)
                break;
            for (i = 0; i < len; i++) {
                unsigned char c = DAT_00526ff4[(dist + i) & 0xfff];
                *dest++ = c;
                DAT_00526ff4[pos] = c;
                pos = (pos + 1) & 0xfff;
            }
        }
        mask <<= 1;
        if (mask & 0x100) {
            mask = 1;
            flags = *src++;
        }
    }
    if (DAT_00526ff4 == 0) {
        printf("Hey!  The window buffer ptr is not pointing to anything!\n");
    } else {
        free(DAT_00526ff4);
        DAT_00526ff4 = 0;
    }
    int n = (int)(dest - base);
    if (own == 0) {
        g_lzssLockOwner = 0;
        InterlockedExchange(&DAT_0052a4fc, 0);
        SetEvent(DAT_0052a4f8);
    }
    return n;
}

// LZSS compressor start-up: allocate the window and tree, seed the root and one
// node, copy the first len bytes into the window, insert each of them in the
// tree, then publish the state in the static buffers. Sibling of the already
// matched 0x4d0a10 (same root/node store order) and 0x4d0de0 (InsertNode).
// FUNCTION: 0x4d1670
int __stdcall LzssSetPreset(unsigned char* src, int len)
{
    int out;
    int n;
    int i;

    DAT_00526ff4 = (char*)calloc(1, 0x1011);
    if (DAT_00526ff4 == 0) {
        printf("Could not alloc decompression window.\n");
        return -1;
    }
    DAT_00526ff0 = (Node_004d0b10*)calloc(0x1001, 6);
    if (DAT_00526ff0 == 0) {
        printf("Could not alloc compression tree.\n");
        return -1;
    }
    n = len;
    if (n > 4000)
        n = 4000;
    DAT_00526ff0[0x1000].parent = 0;
    DAT_00526ff0[0x1000].larger = 0x12;
    DAT_00526ff0[0x1000].smaller = 0;
    DAT_00526ff0[0x12].parent = 0x1000;
    DAT_00526ff0[0x12].larger = 0;
    DAT_00526ff0[0x12].smaller = 0;
    memcpy(DAT_00526ff4 + 0x13, src, n);
    memcpy(g_lzssPresetWindow, DAT_00526ff4, 0x1011);
    for (i = 0; i < n; i++)
        LzssAddString(i + 0x13, &out);
    memcpy(g_lzssPresetTree, DAT_00526ff0, 0x6006);
    g_lzssPresetReady = 1;
    if (DAT_00526ff4 == 0) {
        printf("Hey!  The window buffer ptr is not pointing to anything!\n");
    } else {
        free(DAT_00526ff4);
        DAT_00526ff4 = 0;
    }
    if (DAT_00526ff0 == 0) {
        printf("Hey!  The tree ptr is not pointing to anything!\n");
        return 0;
    }
    free(DAT_00526ff0);
    DAT_00526ff0 = 0;
    return 0;
}

// FUNCTION: 0x4d1800
int LzssDisablePreset()
{
    g_lzssUsePreset = 0;
    return 0;
}

// FUNCTION: 0x4d1810
int LzssEnablePreset()
{
    g_lzssUsePreset = 1;
    return 0;
}

// BUG: `length` is uninitialized for method 0 and 3 (the switch only sets it for
// cases 1 and 2), yet `total = length + 0x13` uses it, so a valid method < 4 can
// compute a bogus compressed size from stack garbage.
// Builds a "SQSH" compressed chunk: header, then the (optionally
// compressed and encrypted) data.
#pragma pack(push, 1)
struct Chunk_4d1820 {
    int marker;                      // +0x00 "SQSH"
    unsigned char version;           // +0x04
    unsigned char method;            // +0x05
    unsigned char encrypt;           // +0x06
    int compressedSize;              // +0x07
    int size;                        // +0x0b
    int checksum;                    // +0x0f
};
#pragma pack(pop)

void __stdcall _compress(char* out, int* outSize, char* in, int size);

static inline unsigned int encryptByte(unsigned int i, char* out) { return (i ^ out[i]) + i; }

// FUNCTION: 0x4d1820
int __stdcall SquashPack(Chunk_4d1820* chunk, int* chunkSize, char* data, int size, int method, int encrypt)
{
    Chunk_4d1820 header, * payload = chunk + 1;
    char* out = (char*)payload;
    if ((char*)payload == 0) {
        return 6;
    }
    if (data == 0) {
        return 6;
    }
    if (size == 0) {
        return 6;
    }
    if (method >= 4) {
        return 4;
    }
    // Left uninitialised: the original has no zeroing store here.
    int length;
    switch (method) {
    case 1:
        length = LzssCompress((unsigned char*)out, (unsigned char*)data, size);
        break;
    case 2:
        length = *chunkSize;
        _compress(out, &length, data, size);
        break;
    }
    // total unsigned, test spelled (length + 0x13): sets the XOR operand order.
    unsigned int total = length + 0x13;
    if ((length + 0x13) > *chunkSize) {
        return 5;
    }
    if (encrypt) {
        // Helper call, index copies and do-while wrappers all stay: XOR operand order.
        do {
            do {
                for (unsigned int i = 0; i < length; i++) {
                    do {
                        unsigned int index = i, idx = index;
                        out[i] = encryptByte(idx, out);
                    } while (0);
                }
            } while (0);
        } while (0);
    }
    memcpy(&header.marker, "SQSH", 4);
    header.version = 2;
    header.method = method;
    header.encrypt = encrypt;
    header.compressedSize = length;
    header.size = size;
    int sum = 0;
    for (unsigned char* p = (unsigned char*)out; p < (unsigned char*)out + length; p++) {
        sum += *p;
    }
    header.checksum = sum;
    *chunk = header;
    *chunkSize = total;
    return 0;
}

// Unpacks a "SQSH" chunk (see SquashPack for the packer).
int __stdcall LzssExpand(unsigned char* dest, unsigned char* src);
int __stdcall _uncompress(unsigned char* dest, unsigned long* destLen, unsigned char* source, unsigned long sourceLen);

// FUNCTION: 0x4d1970
int __stdcall SquashUnpack(char* dest, char* data)
{
    Chunk_4d1820 header;
    // Separate from data, which is advanced past the header: keeps the address.
    Chunk_4d1820* chunk = (Chunk_4d1820*)data;
    memcpy(&header, data, 0x13);
    if (memcmp(&header, "SQSH", 4) != 0) {
        return 1;
    }
    if (header.method >= 4) {
        return 4;
    }
    data += 0x13;
    unsigned char* end = (unsigned char*)data + header.compressedSize;
    int sum = 0;
    for (unsigned char* p = (unsigned char*)data; p < end; p++) {
        sum += *p;
    }
    if (header.checksum != sum) {
        return 2;
    }
    if (header.encrypt) {
        for (unsigned int i = 0; i < header.compressedSize; i++) {
            data[i] = (data[i] - i) ^ i;
        }
    }
    int length;
    switch (header.method) {
    case 1:
        length = LzssExpand((unsigned char*)dest, (unsigned char*)data);
        break;
    case 2:
        // Through a temporary: the `== 0 ? size : 0` form adds a jmp.
        { int t = memcmp(chunk, "SQSH", 4) ? 0 : chunk->size;
        length = t; }
        _uncompress((unsigned char*)dest, (unsigned long*)&length, (unsigned char*)data, header.compressedSize);
        break;
    }
    return (header.size - length) ? 3 : 0;
}

// Scales a value by 120% (mode 1) or 110% (mode 2) and adds 119; any other
// mode gives 0.
// FUNCTION: 0x4d1aa0
unsigned int __stdcall SquashMaxPackedSize(unsigned int value, int mode)
{
    unsigned int result;
    switch (mode) {
    case 1:
        result = value * 120 / 100 + 119;
        break;
    case 2:
        result = value * 110 / 100 + 119;
        break;
    default:
        result = 0;
    }
    return result;
}

// Checks a "SQSH" header: 1 if the magic is wrong, otherwise the byte at +5
// clamped to at most 4.
// FUNCTION: 0x4d1b00
int __stdcall SquashGetPackType(unsigned char* header)
{
    if (memcmp(header, "SQSH", 4) != 0) {
        return 1;
    }
    if (header[5] >= 4) {
        return 4;
    }
    return header[5];
}

// Returns the dword at +0xb of a "SQSH" header (see SquashGetPackType), or 0 when
// the magic is wrong.
// FUNCTION: 0x4d1b40
int __stdcall SquashUnpackedSize(unsigned char* header)
{
    if (memcmp(header, "SQSH", 4) != 0) {
        return 0;
    }
    return *(int*)(header + 0xb);
}

// FUNCTION: 0x4d1b70
int __stdcall SquashEncrypt(char* param1, unsigned int param2)
{
    unsigned int i = 0;
    while (i < param2) {
        unsigned char dl = *param1;
        dl ^= i;
        dl += i;
        *param1 = dl;
        param1++;
        i++;
    }
    return 0;
}

// FUNCTION: 0x4d1ba0
int __stdcall SquashDecrypt(unsigned char* param_1, unsigned int param_2)
{
    unsigned int i = 0;
    if (param_2 > 0) {
        do {
            unsigned char dl = param_1[0];
            dl = dl - (unsigned char)i;
            dl = dl ^ (unsigned char)i;
            param_1[0] = dl;
            param_1++;
            i++;
        } while (i < param_2);
    }
    return 0;
}

// FUNCTION: 0x4d1bd0
int __stdcall SquashChecksum(unsigned char* param_1, int param_2)
{
    int sum = 0;
    unsigned char* end = param_1 + param_2;
    for (; param_1 < end; param_1++) {
        sum += *param_1;
    }
    return sum;
}

// FUNCTION: 0x4d1c00
void __stdcall SquashDumpHeader(Chunk_4d1820* header)
{
    printf("\nheader.packType     = %d\n", header->method);
    printf("header.encryptType  = %d\n", header->encrypt);
    printf("header.packedSize   = %ld\n", header->compressedSize);
    printf("header.unpackedSize = %ld\n", header->size);
}

extern void* g_squashErrorNames[];

// FUNCTION: 0x4d1c60
void* __stdcall SquashErrorString(int param_1)
{
    if (param_1 < 7) {
        return g_squashErrorNames[param_1];
    }
    return 0;
}
