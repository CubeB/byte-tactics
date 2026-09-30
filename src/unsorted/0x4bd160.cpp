// Decompiled by Space Bunny Free, finished by muse-spark-1.3-free, finished by space-bunny-free, finished by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
//
// deepseek-v4.1-flash session: still 98.3%, same single difference
// (0x4bd17b..0x4bd198). New shapes tried this session, all scored with
// check.py: plain separate assignments `sb.size = 20; sb.buf = 0;` and the
// reverse (92.6%, MSVC folds both to immediates); `struct HapiBuf sb = {0};`
// placed AFTER the if (92.4% to 97.1%) so the dead size-zero store is not
// hoisted past the je; aggregate plus a computed first argument
// `(char*)(key & 0)`, `(key ^ key)`, `(key - key)` (all 93.8%, early buf
// store via ebp but no dead `mov eax, ecx`); passing `*(char**)&sb.buf`,
// `*pp` or `pb->buf` (94.1%); a local `int`/`unsigned`/pointer zero (98.3%,
// CSEs into ebp); `memset(&sb.buf, 0, 4)` (93.8%); `#include <windows.h>`
// (98.3%, no change); and a `static HapiBuf MakeB()` helper returned by value.
// The helper result is the first shape that gives the original's FRESH
// `xor ecx, ecx` zero (no ebp CSE) but both member stores still sink below
// the three argument pushes and the dead `mov eax, ecx` is absent. The dead
// copy plus the early buf store look like a front-end value copy for a store
// that the next `mov eax, 0x14` makes dead, and no plain assignment, computed
// zero or by-value helper body tested reproduces it.
// Best remains 98.3% after an additional source variant; still differs only in
// the buffer initialization sequence at 0x4bd17b..0x4bd198, as detailed below.
// GPT-6.1-sol refinement (issue 2310): the existing 98.3% source remains best.
// Returning HapiBuf from an inline initializer scored 93.4%; copying the
// initialized sb.buf through a local scored 94.1%. The remaining mismatch is
// still the buffer setup at 0x4bd17b..0x4bd198.
//
// 98.3%: 580 of 580 bytes, and every byte from 0x4bd198 to the end is identical
// to the original. The whole remaining difference is the eight instructions of
// the buffer setup at 0x4bd17b..0x4bd198:
//
//   original                        ours
//   xor  ecx, ecx                   mov  eax, 0x14
//   mov  eax, ecx                   mov  [sb.buf], ebp      (early)
//   mov  [sb.buf], ecx              push eax / push str / push ebp
//   mov  eax, 0x14                  mov  [sb.size], eax
//   push eax / push str / push ecx  mov  [sb.buf], ebp      (extra, dead)
//   mov  [sb.size], eax             call
//   call
//
// Three linked things remain, and by the "one upstream cause" lesson they are
// one problem: the original rematerialises the buffer's 0 into a FRESH ecx
// after the `if (cb) cb(0)` merge, where ours keeps reusing the ebp zero that
// `extra = 0` already holds; the original carries a dead `mov eax, ecx`, the
// fingerprint of that 0 being materialised in eax for a store; and its buffer
// store is scheduled before the size is materialised, where ours lands after.
// Fixing the fresh-zero register should fix all three.
//
// The source as it stands is `struct HapiBuf sb = {20}; sb.buf = 0;`. The
// aggregate gives the register-materialised size (`mov eax,0x14`, shared by the
// push and the store, which the original has), and the explicit `sb.buf = 0`
// is the only early buffer store; the aggregate's own zero-fill of `sb.buf` is
// the extra dead store at the end.
//
// Everything tried this session, scored free with `check.py --sym`:
//   98.3%  this file, and byte-identical variants using a local `char* b = 0`,
//          an inline `char* Zero()`, `(char*)0`, or a literal 0 argument.
//   96.7%  the previous file (kept below in history): `struct HapiBuf sb = {0}`
//          before the if, then `sb.size = 20; sb.buf = FUN(sb.buf, ...)`; that
//          keeps a dead `sb.size = 0` store before the je.
//   94.1%  `sb = {20}` alone: the buffer store sinks past the pushes.
//   92.6%  `struct HapiBuf sb; sb.buf = 0; sb.size = 20;`, `sb = {0}` after the
//          if, `sb = {20,0}`, every constructor shape (1-arg size, 2-arg, both
//          member-init orders), an inline `Zero()`, an inline `sb.Zero()`,
//          `sb.buf = sb.buf`, and separate `size`/`buf` locals: all compile to
//          576 bytes with both stores grouped after the argument pushes.
//   48.8%  separate `unsigned size; char* buf;` locals (frame drops to 0x50).
// Not tried: any construct that gives the buffer's zero a node distinct from
// `extra = 0` (an inlined helper returning it, a by-value struct copy, ...).
//
// deepseek-v4.1 added: `struct HapiBuf sb = {20};` ALONE is the construct that
// produces the original's fresh `xor ecx,ecx` (the explicit `sb.buf = 0;`
// statement always CSEs the zero into ebp, the register that already holds
// `extra = 0`, which is what leaves the extra store in this file). Its codegen,
// verified with objdump on the object, is
//     mov eax,0x14 / xor ecx,ecx / push eax / push str / push ecx
//     mov [esp+0x20],eax / mov [esp+0x24],ecx / call        (94.1%)
// i.e. the right values in the right registers but BOTH member stores sunk
// below the three argument pushes, where the original stores buf=0 (0x18)
// before the pushes and sinks only the size=20 store. So the remaining problem
// is store scheduling, not the constant or its register.
// Also measured this session (each compiled and objdumped):
//   98.3%  `struct HapiBuf sb = {20}; sb.buf = (char*)0;` and
//          `{20}; char* z = 0; sb.buf = z;` (same as the file: ebp, two stores)
//   97.1%  `{0}; sb.buf = 0; sb.size = 20;` (three zero stores, one early)
//   94.1%  `{20}; struct HapiBuf t = {20}; struct HapiBuf sb = t;` (copy)
//   92.6%  `{20, 0}`, `{20, (char*)0}`, `{0}; sb.size = 20;`, and
//          plain assignments (`sb.size = 0; sb.buf = 0; sb.size = 20;`), which
//          all fold the constants into immediates (mov [mem],0x14) instead of
//          the original's `mov eax,0x14` shared by the push and the store.
// A micro file (same shapes, stores kept alive by an escape after the call)
// shows `{20}` always emits eax=20 first and both stores last, and that an
// array initialiser `unsigned sb[2] = {20};` gives the identical shape, so the
// dead `mov eax,ecx` is not explained by the array form either.
//
// deepseek-v4.1, second session (each variant compiled with tools/wcl /O2 /Ob2
// /MT and objdumped, scoring 576 bytes / under 98.3% unless noted):
//   98.3%  unchanged file (the best): `{20}` + explicit `sb.buf = 0;` gives the
//          early buf store but through ebp, and keeps the aggregate's second
//          buf store after the pushes.
//   576    `{20}; sb.buf = FUN(sb.buf, ...)` (no explicit zero): arg forwarding
//          uses the aggregate's fresh ecx zero for `push ecx`, but BOTH stores
//          stay after the pushes (mov [esp+0x20],eax / mov [esp+0x24],ecx).
//   576    `{20}; sb.buf = FUN((char*)(sb.size - sb.size), ...)`: the member
//          reference in the argument HOISTS the buf store before the pushes and
//          drops the second buf store (exactly the original's schedule), but the
//          zero folds into ebp and the dead `mov eax,ecx` pair is absent. Same
//          for `(char*)(unsigned int)(sb.size - sb.size)` and `(char*)q` copies.
//   576    `{20, 0}`, `{20, (char*)0}`, ctor `HapiBuf() { size = 20; buf = 0; }`
//          and `HapiBuf() : size(20), buf(0) {}`: all fold to immediate stores.
//   580    `{20}; sb.buf = (char*)(sb.size - 20);` inserts a `lea ecx,[eax-0x14]`
//          and a late pointer store, so a computed zero in the statement is not
//          the original's shape either.
// Conclusion: the original's early `mov [esp+0x18],ecx` plus dead `mov eax,ecx`
// is a front end value copy (a zero node distinct from the `extra = 0` ebp
// zero) that no plain assignment, aggregate, ctor or computed-zero spelling
// tested reproduces. Everything from 0x4bd198 to the end is already identical.
//
// deepseek-v4.1 (second session, 20 more shapes dumped instruction by
// instruction) confirms the diagnosis and narrows it: the 94.1% `{20}` shape is
// the ONLY construct that materialises the buffer's zero fresh (`xor ecx,ecx`)
// instead of folding it into the live ebp zero, and it always sinks BOTH member
// stores below the argument pushes. Every attempt to flush that zero early
// (an explicit `sb.buf = 0`, a zeroed local declared before or after the if,
// inlined helpers that store-and-return, `SetBuf(&sb,0)`, `memset`, reading the
// aggregate member into a temporary, assignment-expression arguments such as
// `FUN(sb.buf = 0, ..., sb.size = 20)`, constructor and struct-returning
// initialisers, computed-zero right-hand sides like `sb.size - sb.size`,
// `key ^ key`, `i = 0`) either re-CSEs the zero into ebp (the 98.3% here, two
// stores) or folds the size into an immediate store (576 bytes). Writing the
// aggregate before the `if` makes the front end emit both stores before the je
// with an immediate size, so the original's early flush is not a statement
// order effect either. The `mov eax,ecx` plus early flush look like a front end
// value-copy (an inlined call result materialised for a store) that no plain
// assignment spelling reproduces.
//
// deepseek-v4.1 tried (all scored with check.py against scratch copies):
//   92.6%  `char* buf = 0; unsigned size = 0; size = 20;` then an
//          uninitialised `struct HapiBuf sb;` assigned from both: the frame
//          drops to 0x50 (576 bytes) and everything shifts, so separate
//          locals cannot carry the original frame; MSVC also folds both
//          stores to immediates and keeps the ebp zero.
//   97.1%  `buf`/`size` locals plus `struct HapiBuf sb = {20};` and
//          `sb.size = size;`: frame is right but the size push becomes
//          `push 0x14` and both member stores land after the pushes.
//   97.4%  zero produced through a union member (`union { unsigned s;
//          char* p; } u; u.s = 0; sb.buf = u.p;`): same 98.3% layout but
//          the union gets its own slot, so one more instruction.
//   98.3%  `*(char**)&sb.buf = 0;`, identical to the explicit assignment.
// Conclusion: the fresh `xor ecx,ecx` is a property of the aggregate's own
// zero-fill (the `{20}` alone produces it), while any explicit `= 0`
// statement CSEs into ebp; the dead `mov eax,ecx` looks like a value copy
// from a second target that the optimizer dropped, which no plain
// assignment spelling reproduces.
//
// Frame layout (21 dwords, do not disturb): X+0x00 extra, X+0x04 sb.size,
// X+0x08 sb.buf, X+0x0c year[8], X+0x14 copyright[0x40].
#include <stdio.h>
#include <string.h>
#include <time.h>

void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __cdecl FUN_004d85a0(int* p);
int __stdcall FUN_004bd3b0(char* path, unsigned int* size, char** extra);
int __stdcall FUN_004bd830(char* path, void* buf, int off, FILE* f,
                           void (__cdecl* cb)(int), char* extra, int key, int flags);

struct HapiBuf {
    unsigned int size;
    char* buf;
};

struct Hapi_004bd160 {
    char magic[4];
    char a4;
    char a5;
    char a6;
    char a7;
    unsigned int size;
    unsigned char key;
    char ad;
    unsigned short ae;
    int extra;
};

// FUNCTION: 0x4bd160
int __stdcall FUN_004bd160(char* srcname, char* dstname, void (__cdecl* cb)(int),
                           unsigned int key, int flags)
{
    char* extra = 0;
    char year[8];
    char copyright[0x40];
    int off;
    FILE* f;
    int i, n;
    unsigned char* p;

    if (cb)
        cb(0);
    struct HapiBuf sb = {20};
    sb.buf = 0;
    sb.buf = (char*)FUN_004d84a0(sb.buf, "Package Data", sb.size);
    off = FUN_004bd3b0(srcname, &sb.size, &extra);

    {
        struct Hapi_004bd160* h = (struct Hapi_004bd160*)sb.buf;
        unsigned int m;
        unsigned int t;
        unsigned char k;
        strncpy(sb.buf, "HAPI", 4);
        h->a4 = 0;
        h->a5 = 0;
        h->a6 = 1;
        h->a7 = 0;
        h->size = sb.size;
        m = key & 0xff;
        if ((unsigned char)key == 0)
            k = 0;
        else {
            t = (m >> 2) | (m << 6);
            k = ~t;
        }
        h->key = k;
        h->ad = 0;
        h->ae = 0;
        h->extra = off;
    }

    f = fopen(dstname, "wb");
    if (!f) {
        if (sb.buf)
            FUN_004d85a0((int*)sb.buf);
        return 0;
    }
    fwrite(sb.buf, sb.size, 1, f);
    if (cb)
        cb(5);
    FUN_004bd830(srcname, sb.buf, off, f, cb, extra, key, flags);
    if (cb)
        cb(0x5f);
    n = (int)sb.size - 20;
    p = (unsigned char*)sb.buf + 20;
    if ((unsigned char)key) {
        for (i = 0; i < n; i++)
            p[i] = (char)~((unsigned char)(i + 20) ^ (unsigned char)key ^ p[i]);
    }
    rewind(f);
    fwrite(sb.buf, sb.size, 1, f);
    {
    time_t now;
    struct tm* t;
    now = time(0);
    t = localtime(&now);
    sprintf(year, "%i", t->tm_year + 1900);
    strcpy(copyright, "Copyright 0000 Cavedog Entertainment");
    strncpy(strstr(copyright, "0000"), year, 4);
    fseek(f, 0, SEEK_END);
    fprintf(f, copyright);
    fclose(f);
    if (sb.buf)
        FUN_004d85a0((int*)sb.buf);
    }
    return 1;
}
