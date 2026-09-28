// Decompiled by space-bunny-free. Names are provisional.
// Writes one HAPI cache file. 0x4bd3b0 loads a directory's contents and returns
// the total size through its second argument and a second pointer through its
// third; a 20 byte "HAPI" header is laid out in front of that data, the whole
// thing is written, 0x4bd830 rewrites the body in place, everything past the
// header is then encrypted with a key byte derived from the fourth argument,
// and the header is rewritten from offset 0. The copyright line, with the
// current year patched into its "0000" placeholder, is appended at the end.
// Returns 0 when the file cannot be opened, 1 otherwise.
//
// 56.6% of the original's 580 bytes. What is established and what is not:
// MATCHING SO FAR: the frame size 0x54, `extra` at [esp+0x10], the copyright
// buffer at [esp+0x24], the whole tail sequence (time, localtime, sprintf,
// the strcpy of the copyright literal, the strstr + strncpy year patch, fseek,
// fprintf, fclose, free, return 1) instruction for instruction including the
// dead-argument-slot reuse that puts the time_t in the third argument's slot
// at [esp+0x70], and every name the linker fills in.
//
// 1. The original gives the buffer pointer a stack slot of its own at
//    [esp+0x18]: it is stored as 0 just before the allocator call, stored
//    again with the allocator's result, and reloaded at every use (the strncpy
//    destination, both fwrite buffers, both free calls, the loop base). Here
//    the same pointer is promoted to a register, so MSVC gives it no home at
//    all. Declaring the three pointers at function scope with their
//    initialisers before the progress callback does produce all three slots,
//    but it also moves the allocator call ahead of the callback call, which
//    contradicts the original's order, and no arrangement tried keeps the
//    callback first and the slot. The frame is padded back to 0x54 with a
//    twelve byte year buffer, so the frame size and two of the five slots are
//    right while the year string lands at [esp+0x18] instead of [esp+0x1c]
//    and the buffer slot is missing. The twelve is a reconstruction: the
//    original's year buffer is 8 bytes at [esp+0x1c] with the buffer pointer
//    at [esp+0x18] and 4 + 4 + 4 + 8 + 0x40 = 0x54. The one arrangement that
//    does give all three pointer slots is the one that puts the allocator call
//    first, and its layout is right except that MSVC puts the unsigned int in
//    the lower of the two 4-byte slots and the char* above it, the reverse of
//    the original.
// 2. MSVC puts `size` in the dead third argument slot and `now` in the frame;
//    the original does the opposite (size at [esp+0x14], now in the argument
//    slot). Every declaration order and scope tried gives the same choice.
//    Wrapping the two out-parameters in a local struct pins the layout to
//    extra at [esp+0x10] and size at [esp+0x14] and puts `now` back in the
//    argument slot, which is right, but that version scored 54.6% rather than
//    this one's 56.6% because the struct access costs an extra instruction
//    around the allocator call. So the slot order and the score pull apart.
// 3. The original keeps the fourth argument (the key) in ebx for its whole
//    live range and reloads the progress callback from the argument slot
//    instead of keeping it live, which leaves esi for the buffer. Here the
//    callback is live in esi from the prologue and the buffer sits in ebp, so
//    the middle of the function uses different registers throughout.
// 4. The key transform: the original computes `(key & 0xff) >> 2` and
//    `(key & 0xff) << 6` in 32 bit registers and complements only the low
//    byte. Every spelling tried lets MSVC narrow the shifts to a byte
//    register and add an `and 0x3f`.
// 5. The encrypt loop: the original indexes off a hoisted base pointer with a
//    separate index, hoists the bound into edi with `lea edi, [size-20]` and
//    loads the element into a register before xoring. A pointer-increment
//    loop scored best here but still folds the element load into the xor.
#include <stdio.h>
#include <string.h>
#include <time.h>

void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __cdecl FUN_004d85a0(int* p);
int __stdcall FUN_004bd3b0(char* path, unsigned int* size, char** extra);
int __stdcall FUN_004bd830(char* path, void* buf, int offset, FILE* f,
                          void (__cdecl *cb)(int), char* extra, int key, int flags);

// The 20 byte header the file starts with: offset 8 is the total size of the
// file, 0xc the key byte derived from the fourth argument and 0x10 whatever
// 0x4bd3b0 returns.
struct Hapi_004bd160 {
    char magic[4];          // +0x00 "HAPI"
    char a4;                // +0x04
    char a5;                // +0x05
    char a6;                // +0x06, always 1
    char a7;                // +0x07
    unsigned int size;      // +0x08
    unsigned char key;      // +0x0c
    char ad;                // +0x0d
    unsigned short ae;      // +0x0e
    int extra;              // +0x10
};

// FUNCTION: 0x4bd160
int __stdcall FUN_004bd160(char* srcname, char* dstname, void (__cdecl *cb)(int),
                           unsigned int key, int flags)
{
    char* extra = 0;

    if (cb)
        cb(0);
    {
        char* buf = 0;
        unsigned int size = 20;
        char year[12];
        char copyright[0x40];
        int off;
        FILE* f;
        unsigned char* p;
        unsigned char k;
        int i, n;
        time_t now;
        struct tm* t;

        buf = (char*)FUN_004d84a0(buf, "Package Data", size);
        off = FUN_004bd3b0(srcname, &size, &extra);

        strncpy(buf, "HAPI", 4);
        buf[4] = 0;
        buf[5] = 0;
        buf[6] = 1;
        buf[7] = 0;
        *(unsigned int*)(buf + 8) = size;
        k = (unsigned char)key;
        if (k)
            buf[0xc] = (char)~(k >> 2 | k << 6);
        else
            buf[0xc] = 0;
        buf[0xd] = 0;
        *(unsigned short*)(buf + 0xe) = 0;
        *(int*)(buf + 0x10) = off;

        f = fopen(dstname, "wb");
        if (!f) {
            if (buf)
                FUN_004d85a0((int*)buf);
            return 0;
        }
        fwrite(buf, size, 1, f);
        if (cb)
            cb(5);
        FUN_004bd830(srcname, buf, off, f, cb, extra, key, flags);
        if (cb)
            cb(0x5f);
        if (key) {
            n = (int)size - 20;
            for (p = (unsigned char*)buf + 20, i = 0; i < n; i++, p++)
                *p = (char)~((unsigned char)(i + 0x14) ^ key ^ *p);
        }
        rewind(f);
        fwrite(buf, size, 1, f);
        now = time(0);
        t = localtime(&now);
        sprintf(year, "%i", t->tm_year + 1900);
        strcpy(copyright, "Copyright 0000 Cavedog Entertainment");
        strncpy(strstr(copyright, "0000"), year, 4);
        fseek(f, 0, SEEK_END);
        fprintf(f, copyright);
        fclose(f);
        if (buf)
            FUN_004d85a0((int*)buf);
        return 1;
    }
}
