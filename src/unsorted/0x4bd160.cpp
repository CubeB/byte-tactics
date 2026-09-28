// Decompiled by Space Bunny Free. Names are provisional.
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
    struct HapiBuf sb;
    char copyright[0x40];
    int off;
    FILE* f;
    int i, n;
    unsigned char* p;
    unsigned char k;
    time_t now;
    struct tm* t;

    if (cb)
        cb(0);

    sb.buf = 0;
    sb.size = 20;
    sb.buf = (char*)FUN_004d84a0(0, "Package Data", sb.size);
    off = FUN_004bd3b0(srcname, &sb.size, &extra);

    {
        struct Hapi_004bd160* h = (struct Hapi_004bd160*)sb.buf;
        strncpy(sb.buf, "HAPI", 4);
        h->a4 = 0;
        h->a5 = 0;
        h->a6 = 1;
        h->a7 = 0;
        h->size = sb.size;
        k = (unsigned char)key;
        if (!k)
            h->key = 0;
        else
            h->key = (char)~((key & 0xff) >> 2 | (key & 0xff) << 6);
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
    if (k) {
        for (i = 0; i < n; i++)
            p[i] = (char)~((unsigned char)(i + 20) ^ k ^ p[i]);
    }
    rewind(f);
    fwrite(sb.buf, sb.size, 1, f);
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
    return 1;
}
