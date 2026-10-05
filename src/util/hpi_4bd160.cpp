// Decompiled by Space Bunny Free, finished by muse-spark-1.3-free, finished by space-bunny-free, finished by GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by DeepSeek V4.1 Flash, checked by GPT-6, finished by Claude Opus 5.5. Names are provisional.
//
// MATCH (Claude Opus 5.5, #5252). Writes an HPI package: a 20-byte "HAPI"
// header in a growing buffer {size, buf}, the directory built by HAPI_BuildArchiveDirectory,
// the file data from HAPI_WriteArchiveData, the directory encrypted with the key, and
// a copyright line at the end.
//
// The buffer setup (`xor ecx,ecx / mov eax,ecx / mov [buf],ecx / mov eax,0x14`)
// that held this file at 99.3% for many passes is the inlined Grow helper of
// the matched HAPI_BuildArchiveDirectory on a memset-cleared buffer: the dead `mov eax,ecx`
// is Grow's unused `old = b->size`, the zero is memset's (so it is not the
// `extra = 0` constant in ebp), and `size += 20` folds to `mov eax,0x14`.

#include <stdio.h>
#include <string.h>
#include <time.h>

void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __cdecl FUN_004d85a0(int* p);
int __stdcall HAPI_WriteArchiveData(char* path, void* buf, int off, FILE* f,
                           void (__cdecl* cb)(int), char* extra, int key, int flags);

struct HapiBuf {
    unsigned int size;
    char* buf;
};

int __stdcall HAPI_BuildArchiveDirectory(char* path, HapiBuf* out, char** extra);

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

static inline unsigned int Grow(HapiBuf* b, unsigned int n)
{
    unsigned int old = b->size;
    b->size += n;
    b->buf = (char*)FUN_004d84a0(b->buf, "Package Data", b->size);
    return old;
}

// FUNCTION: 0x4bd160
int __stdcall HAPI_PackDirectory(char* srcname, char* dstname, void (__cdecl* cb)(int),
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
    struct HapiBuf sb;
    memset(&sb, 0, sizeof(sb));
    Grow(&sb, 20);
    off = HAPI_BuildArchiveDirectory(srcname, &sb, &extra);

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
    HAPI_WriteArchiveData(srcname, sb.buf, off, f, cb, extra, key, flags);
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

