// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// Pass 11 (deepseek-v4.1-flash, #4016): re-measured 75.0% (1025 vs 1040 bytes). Two new
// pop spellings, both byte-identical to the recorded best: `Entry e = *Pop(rows);` with
// no intermediate `r`, and the pop as a Buffer member `rows->Pop()` (MSVC canonicalises
// both back to the free-helper form, so the r=ECX/index=EAX rotation is not call shape).
// Pass 10 (deepseek-v4.1-flash, #3870): re-measured 75.0% (1025 vs 1040 bytes) and left it
// unchanged. Tried this pass: swapping the f14/f18 source store order (74.2, worse, so the
// compiler's own store order is the one that scores) and a named `unsigned int cap =
// size + 0x100` local reused for the new and the f4 assignment (byte-neutral at 75.0).
// The two open rotations below are unchanged.
// Pass 9 (deepseek-v4.1-flash, #3819): 74.2 -> 75.0% (1025 vs 1040 bytes). The finding: the
// a4/a5 tail loops each get their own count local computed as `n - 0x200` (`left1` in the
// a6!=0 path, `left2` in the a6==0 path), which finally puts the a5-path counter in EDI and
// the packet pointer in EBP exactly as the original (`test edi,edi` / `dec edi` /
// `mov ebp,[esp+0x2c]` / the entry store of ebp), replacing the old shared `lineLeft` that
// the allocator kept in memory ([esp+0x30]). `left2` declared before `char* q = text + 4;` is
// the 75.0 variant; taking it from `lineLeft` instead scores 74.9, and declaring it after
// `int rem2 = (int)size;` is flat at 74.9, so the count first is worth 0.1. Reordering the
// f14/f18 stores above memcpy regresses hard to 67.8% (1025 bytes), keep them after the copy.
// Still differing: the 0x46379e Pop block (ours rows=ECX / n=EAX, original rows=EAX / n=ECX)
// and the a4-path memcpy tail (ours loads [esp+0x34] early, stores f18 before f14, and uses
// EDI for the zero counter where the original uses EBP).
// Pass 8 (deepseek-v4.1-flash, #3767): re-measured and restored this 74.2% / 1029-byte
// build after testing more shapes. Declaring `remaining` first (either in place of, or
// in the loop header's for-init, of `n`) is byte-neutral while `char* p = text + 4;`
// stays last; moving `p` earlier (before `n`/`remaining`, any order) collapses the whole
// scan loop to 1018 bytes / 71.9%; `unsigned n` regresses to 73.8 (1028); the pop index
// re-spelled as a named `int index = r->n;` local (test/decrement/store on it) and
// `n = 0;` assigned after a bare `int n;` are byte-identical. So the missing 11 bytes
// (the 4-byte `mov [esp+0x2c], esi` x-spill plus the EDI/EBP spill pair) are one web
// assignment with the scan-loop counter, not an independent store.
//
// Pass 7 (deepseek-v4.1-flash, #3729): re-measured 74.2% (1029 vs 1040 bytes).
// New spellings this pass, all no better: `if ((remaining -= w) < 0) break;` in the
// scan loop is byte-identical at 74.2% / 1029 bytes, so the store placement of
// `remaining` is not the EDI/EBP swap; declaring n/remaining after `char* p = text + 4;`
// regresses to 71.9% / 1018 bytes; `for (int i = rows->n; i > 0; i--)` in the copy loop
// regresses to 69.4% / 1025 bytes. Still open, unchanged: pop block r=ECX where the
// original uses r=EAX, scan loop n=EDI/remaining=EBP where the original uses EBP/EDI,
// and the cascade into the 0x463a30 and 0x463ad0 loops plus the a4/a5 memcpy pairing.

// Pass (#3673, deepseek-v4.1-flash): re-measured 74.2% (1029 vs 1040 bytes). Byte-neutral
// this pass: swapping the scan-loop n/remaining declarations, and declaring rem2 before q
// in the a6==0 tail loop. Putting p between n and remaining regresses to 71.9 (1018 bytes).
// The remaining diff is still the two register rotations named below.

// PARTIAL: 74.2% (1029 vs 1040 bytes). Best found this session.
//
// What now matches: the `mov edi,0x200` preheader hoist, `sub ebx,4` then
// `mov ebp,ebx` for the scan init, and block 1 (0x4637e2) now has the original
// instruction order (inc head, cmp against the local h BEFORE the store, lea
// ep from the pre-wrap h). The wrap is a static inline helper taking the
// buffer and `int& h`, comparing h (not a reload of r->head). Passing h by
// value into a helper that reloads r->head instead reintroduces the wrong
// order. The a6==0 tail loop now uses its own signed `rem2 = (int)size` local:
// the original restarts that pass from the full size-4 (ebx) rather than the
// scan-updated remaining, so a distinct variable is required.
//
// What still differs, one register-allocator rotation that cascades:
//  * block 1 (0x4637e2) r/index swap: ours r=ECX / index=EAX, original
//    r=EAX / index=ECX. Any wrap that compares the local h (instead of
//    reloading r->head) flips r to ECX. Keeping Wrap_00463790(r->head,0x200)
//    (a reload) gives r=EAX but puts the cmp after the store.
//  * the 0x4638db scan loop: ours n=EDI / remaining=EBP, the original
//    n=EBP / remaining=EDI. Declaration order, `++n`, `n = n + 1`,
//    `remaining -= w`, `(int)size`, and passing `rows` directly to Pop do not
//    change it.
//  * these cascade into the 0x463a30 and 0x463ad0 loops and the memcpy
//    a4/a5 register pairing (ours loads a5 early / a4 late).
//
// Ruled out again in the #3556 pass (pop r=ECX would not move to EAX):
// fully inlining the pop without a helper (68.6), `if (rows->n > 0)` with the
// local r loaded inside (70.5, adds `mov esi,eax`), a reference-form Pop
// (74.2, unchanged), calling `Pop_00463790(rows)` directly (74.2), dropping
// the helper's count test (69.0), both wrap forms (local h vs r->head) and
// both WrapHeadRef parameter orders (74.2), `r->n >= 1` (72.5), function-scope
// `i` and `e` (74.2), `delete text` instead of operator delete (74.2),
// `#include <memory.h>` (74.2), swapping the n/remaining/p declarations or
// putting them on one line (74.2), and one shared `r` for pop+push (67.0,
// loses the push reload). Odd detail: the push block gets r=EAX in ours too,
// so only the pop block's r vreg rotates.
//
// Ruled out (all worse): int-returning Wrap that also increments (loses the
// EDI hoist), fully inlining the pop without a helper (entry copy moves to
// EDX), `unsigned int remaining` (66.6%), a do-while restructure of the scan
// loop (73.1 with rem2, 72.2 without), and reordering the f14/f18 stores
// (67.1 / 73.4).
#include <string.h>

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

// Bit reader, see src/unsorted/0x415dc0.cpp.
class Class_00415dc0 {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int FUN_00415dc0(int bits);
};

// Character widths, one word per entry with a 4-byte stride.
extern unsigned short DAT_00512ad8[][2];

struct Entry_00463790 {
    int a;                             // +0x00
    int b;                             // +0x04
    int c;                             // +0x08
};

// 0x180c-byte ring of 0x200 12-byte entries.
struct Buffer_00463730 {
    int n;                             // +0x00
    int head;                          // +0x04
    int tail;                          // +0x08
    Entry_00463790 entry[0x200];       // +0x0c
};


// Bounded wrap of a ring index, see the note at the top of the file.
__inline void Wrap_00463790(int& i, int cap)
{
    if (i >= cap)
        i = 0;
}

static __inline void WrapHeadRef_00463790(Buffer_00463730* r, int& h)
{
    if (h >= 0x200)
        r->head = 0;
}

static __inline Entry_00463790* Pop_00463790(Buffer_00463730* r)
{
    if (r->n > 0) {
        r->n--;
        int h = r->head + 1;
        r->head = h;
        Entry_00463790* ep = (Entry_00463790*)((char*)r + h * 12);
        WrapHeadRef_00463790(r, h);
        return ep;
    }
    return 0;
}

class Class_00463730 {
public:
    int f0;                            // +0x00
    int f4;                            // +0x04
    int f8;                            // +0x08
    char* text;                        // +0x0c
    Buffer_00463730* rows;             // +0x10
    int f14;                           // +0x14
    int f18;                           // +0x18

    int FUN_00463790(char* src, unsigned int size, int x, int a4, int a5, int a6);
};

// FUNCTION: 0x463790
int Class_00463730::FUN_00463790(char* src, unsigned int size, int x, int a4, int a5, int a6)
{
    if (size <= 0) {
        return 1;
    }
    if ((rows ? rows->n : 0) != 0) {
        f8++;
        int i = rows->n;
        while (i--) {
            Entry_00463790 e = *Pop_00463790(rows);
            Buffer_00463730* r = rows;
            if (r->n < 0x200) {
                int t = r->tail + 1;
                r->tail = t;
                Wrap_00463790(r->tail, 0x200);
                r->entry[r->tail].b = e.b;
                r->entry[r->tail].a = x;
                r->entry[r->tail].c = e.c;
                r->n++;
            }
        }
        return 0;
    }

    f8 = 0;
    if (size > f4) {
        operator delete(text);
        char* nb = (char*)operator new(size + 0x100);
        text = nb;
        if (nb == 0) {
            f4 = 0;
            return 1;
        }
        f4 = size + 0x100;
    }
    memcpy(text, src, size);
    f14 = a4;
    f18 = a5;
    size -= 4;
    int n = 0;
    int remaining = size;
    char* p = text + 4;
    while (remaining > 0) {
        unsigned char c = *p;
        unsigned char cc = c;
        if (cc <= 1) {
            break;
        }
        if (cc >= 0x2d) {
            break;
        }
        unsigned short w;
        if (cc == 0x2c) {
            Class_00415dc0 reader;
            reader.data = (unsigned int*)p;
            reader.index = 0;
            reader.bit = 0;
            reader.FUN_00415dc0(8);
            w = (unsigned short)reader.FUN_00415dc0(0x10);
        } else {
            w = DAT_00512ad8[c][0];
        }
        remaining -= w;
        if (remaining < 0) {
            break;
        }
        p += w;
        n++;
    }

    if (n > 0) {
        int lineLeft = n - 0x200;
        if (a6 != 0) {
            int left1 = n - 0x200;
            int span = x - f0;
            if (span > 0x1e) {
                span = 0x1e;
            } else if (span <= 0) {
                span = 1;
            }
            int spacing = 0x10;
            if (n > span) {
                spacing = (n << 4) / span;
            }
            int progress = 0;
            int i = 0;
            char* q = text + 4;
            do {
                unsigned char c = *q;
                unsigned char cc = c;
                unsigned short w;
                if (cc == 0x2c) {
                    Class_00415dc0 reader;
                    reader.data = (unsigned int*)q;
                    reader.index = 0;
                    reader.bit = 0;
                    reader.FUN_00415dc0(8);
                    w = (unsigned short)reader.FUN_00415dc0(0x10);
                    if (left1 > 0) {
                        left1--;
                        remaining -= w;
                        q += w;
                        goto next2;
                    }
                } else {
                    w = DAT_00512ad8[c][0];
                }
                {
                    Buffer_00463730* r = rows;
                    if (r->n >= 0x200) {
                        return 1;
                    }
                    int t = r->tail + 1;
                    r->tail = t;
                    if (t >= 0x200) {
                        r->tail = 0;
                    }
                    r->entry[r->tail].b = (int)q;
                    r->entry[r->tail].a = x;
                    r->entry[r->tail].c = w;
                    r->n++;
                }
                i++;
                {
                    int scaled = i << 4;
                    if ((unsigned int)scaled >= (unsigned int)progress) {
                        progress += spacing;
                        x++;
                    }
                }
                remaining -= w;
                q += w;
            next2:
                ;
            } while (--n > 0);
            return 1;
        }

        int left2 = n - 0x200;
        char* q = text + 4;
        int rem2 = (int)size;
        while (rem2 > 0) {
            unsigned char c = *q;
            unsigned char cc = c;
            if (cc <= 1) {
                return 1;
            }
            if (cc >= 0x2d) {
                return 1;
            }
            unsigned short w;
            if (cc == 0x2c) {
                Class_00415dc0 reader;
                reader.data = (unsigned int*)q;
                reader.index = 0;
                reader.bit = 0;
                reader.FUN_00415dc0(8);
                w = (unsigned short)reader.FUN_00415dc0(0x10);
                if (left2 > 0) {
                    left2--;
                    rem2 -= w;
                    q += w;
                    goto next3;
                }
            } else {
                w = DAT_00512ad8[c][0];
            }
            rem2 -= w;
            if (rem2 < 0) {
                return 1;
            }
            {
                Buffer_00463730* r = rows;
                if (r->n >= 0x200) {
                    return 1;
                }
                int t = r->tail + 1;
                r->tail = t;
                if (t >= 0x200) {
                    r->tail = 0;
                }
                r->entry[r->tail].b = (int)q;
                r->entry[r->tail].a = x;
                r->entry[r->tail].c = w;
                r->n++;
            }
            q += w;
        next3:
            ;
        }
    }
    return 1;
}
