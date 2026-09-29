// Decompiled by Space Bunny Free, finished by muse-spark-1.3-free, finished by
// space-bunny-free. Names are provisional.
//
// Not matching yet: 580 of 580 bytes, but ONE source construct is still wrong.
// Everything from 0x4bd038 to the end of the function is byte identical (the
// relocation-filled call targets and string addresses aside). The only diff is
// the block at 0x4bd173..0x4bd193, which the original writes as
//
//     je  ; push ebp; call eax; add esp,4
//     xor ecx,ecx ; mov eax,ecx ; mov [esp+0x18],ecx    <- sb.buf = 0
//     mov eax,0x14 ; push eax ; push <"Package Data"> ; push ecx
//     mov [esp+0x20],eax                               <- sb.size = 20, sunk
//     call FUN_004d84a0
//
// and this file writes (same length, 8 bytes of prologue difference either way):
//
//     mov [esp+0x14],ebp ; mov [esp+0x18],ebp ; je      <- two zero stores
//     push ebp ; call eax ; add esp,4
//     push 0x14 ; push <"Package Data"> ; push ebp
//     mov [esp+0x20],0x14 ; call FUN_004d84a0
//
// So three things are needed, and they must come from ONE construct: the
// sb.buf = 0 store has to happen AFTER the if (cb) cb(0) block, both constants
// have to be materialised into registers (eax for 20, ecx for 0) instead of
// immediates, and the ONE zero has to be a FRESH register (ecx), not the ebp
// zero that extra = 0 already holds, because the original reuses that same ecx
// for both the sb.buf store and the pushed NULL argument. This file CSEs the
// ctor's 0, the argument's 0 and extra = 0 into one ebp value.
//
// Measured this session (all via the free scratch scorer, all WORSE than the
// 96.7% here, so none was kept):
//   build/scratch/0x4bd160/k_D0_assign_temp.cpp and k_G0_local_ctor_after.cpp
//     93.6%: "sb = HB1(0, 20); sb.buf = FUN_004d84a0(0, "Package Data", 20);"
//     with a two-argument constructor gets 20 into eax and 0x14 shared between
//     the push and the store, but still pushes ebp, still sinks BOTH field
//     stores past the three pushes, and has no `mov eax,ecx`.
//   build/scratch/0x4bd160/m_L2.cpp  92.6%: declaring `struct HapiBuf sb = {0}`
//     AFTER the if (cb) cb(0) statement kills the dead size = 0 store and puts
//     the single remaining zero store after the call, which is the right block,
//     but MSVC still sinks it past the three argument pushes and still folds
//     the constants to immediates.
//   build/scratch/0x4bd160/g_A0_ctor_buf0.cpp  91.2%: a constructor that only
//     sets buf = 0 keeps the store in the right ORDER (it is not sunk past the
//     callback) but the store lands BEFORE the `je`, not after it.
//   Separate locals instead of a struct (unsigned size; char* buf, declared
//     between extra and year) drop the frame to `sub esp,0x50` and the whole
//     body shifts: 48.8%. The struct is load bearing.
//   Passing sb.buf as the first argument, a shared `char* nul = 0` variable, a
//     two-argument constructor, `sb.size = 20` written twice, and the
//     aggregate `{20, 0}`: all 71% to 93.6%, none better.
//
// Second session (space-bunny-free). Scored by BYTE difference against the
// original rather than by difflib, which is misleading here: the current file
// is 580 of 580 bytes with only 27 masked bytes different, and every byte from
// 0x4bd198 to the end is identical AT THE SAME OFFSETS. The whole remaining
// problem is the 8 bytes at 0x4bd173..0x4bd193 and their 8-byte replacement.
// `build/scratch/0x4bd160/probe.py <file>...` reports size + differing byte
// count for free; `idiff.py <file>` prints an LCS aligned instruction diff.
//
// The best alternative found, `build/scratch/0x4bd160/T4.cpp` (96.9% by
// difflib, 39 bytes different), is worth knowing about because it fixes the
// ORDER and gets a constant into a register: declaring `sb` AFTER the
// `if (cb) cb(0)` statement with a constructor `HapiBuf(char* b) : buf(b) {}`
// and calling `HapiBuf sb(0)` puts `mov [sb.buf], 0` in exactly the right
// place, and writing `sb.size = 20` a SECOND time after the FUN_004d84a0
// statement makes MSVC sink that store into the call sequence and share the
// register holding 20 with the pushed argument. What it still gets wrong:
//
//   original                       T4
//   xor  ecx, ecx                  mov  esi, 0x14
//   mov  eax, ecx                  mov  [esp+0x18], ebp     <- the ctor's store
//   mov  [esp+0x18], ecx           push esi / push name / push ebp
//   mov  eax, 0x14                 mov  [esp+0x20], esi
//   push eax / push name / push ecx
//   mov  [esp+0x20], eax
//                                   plus one extra `mov [esp+0x20], esi` at +0x4a
//
// So three things are still missing, and by the lesson in the brief they are
// probably ONE cause: (1) the 0 must be materialised in a FRESH register
// (ecx) instead of being CSE'd with the `xor ebp,ebp` zero that `extra = 0`
// already holds, and the dead `mov eax,ecx` is the fingerprint of a source
// construct that wanted 0 in eax; (2) the 20 temp must land in eax, not esi;
// (3) the second `sb.size = 20` must not produce a store at all. Adjacent
// duplicate `sb.size = 20` statements are merged by MSVC back into one store
// (T1, U1: 576 bytes, everything after the region shifted), so the duplicate
// that keeps the first store alive has to straddle the call statement, which
// is what forces the extra store at +0x4a.
// Tried and no better: `HapiBuf sb(0)` with `size` also in the ctor,
// `HapiBuf sb(0, 20)`, `sb = HapiBuf(0)`, explicit ctor re-call (MSVC 5
// rejects `sb.HapiBuf()`), passing `sb.buf` as the first argument, separate
// locals instead of the struct, and three `sb.size = 20` statements.
#include <stdio.h>
#include <string.h>
#include <time.h>

void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __cdecl FUN_004d85a0(int* p);
int __stdcall FUN_004bd3b0(char* path, unsigned int* size, char** extra);
int __stdcall FUN_004bd830(char* path, void* buf, int off, FILE* f,
                           void (__cdecl* cb)(int), char* extra, int key, int flags);

struct HapiBuf {
    unsigned int size;          // +0x00
    char* buf;                  // +0x04
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
    struct HapiBuf sb = {0};
    char copyright[0x40];
    int off;
    FILE* f;
    int i, n;
    unsigned char* p;

    if (cb)
        cb(0);

    sb.size = 20;
    sb.buf = (char*)FUN_004d84a0(0, "Package Data", sb.size);
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
