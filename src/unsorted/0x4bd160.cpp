// Decompiled by Space Bunny Free, finished by muse-spark-1.3-free. Names are provisional.
// Still differs, 96.7 percent (580 = 580 bytes), one block after the first
// `if (cb) cb(0);`:
//   original: xor ecx,ecx / mov eax,ecx / mov [esp+0x18],ecx / mov eax,0x14 /
//             push eax / push "Package Data" / push ecx / mov [esp+0x20],eax
//   ours:     push 0x14 / push "Package Data" / push ebp / mov [esp+0x20],0x14
//             (and the two zero stores of `= {0}` sit before the `je`).
// So in the original the header struct is zeroed AFTER the cb call (the only
// store before it is extra = 0), the zero comes from a fresh `xor ecx,ecx` and
// not from the ebp zero, and the 0x14 travels in eax (pushed and stored) rather
// than as an immediate. The dead `mov eax,ecx` looks like a size = 0 store that
// was removed as dead. Declaring `HapiBuf sb` after the cb call gets the zero
// into a fresh register (`xor eax,eax`) but the 0x14 stays an immediate; about
// 150 forms (ctor, Alloc method, inline wrappers of the alloc call and of cb,
// chained assignments, separate locals, int/pointer field types, zero sources)
// all land at 92.6 to 96.7.
// The key encoding was fixed in this pass: an `unsigned char t` local assigned
// 0 on one path and `~v` (v an unsigned int rotate) on the other gives the
// original's `xor cl,cl` and `not cl`; a ternary or casts never narrowed.
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
        strncpy(sb.buf, "HAPI", 4);
        h->a4 = 0;
        h->a5 = 0;
        h->a6 = 1;
        h->a7 = 0;
        h->size = sb.size;
        m = key & 0xff;
        unsigned char t; if ((unsigned char)key == 0) t = 0; else { unsigned int v = (m >> 2) | (m << 6); t = ~v; } h->key = t;
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
