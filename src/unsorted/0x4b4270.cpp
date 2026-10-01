// Decompiled by space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5. Names are provisional.
// Sonnet 5.5 retry: 74.8% -> 78.2%, now 737 bytes like the original. Reading
// the section-name table pointer into a local before the name test
// (`char* sect = *image;` then `_strcmpi(name, sect + h.strOffset)`) pulls the
// load of `image` into edi above the `test name` like the original and makes
// the size exact; the other `*image` reads stay uncached (the original reloads
// `[edi]` at every use, caching them all in `sect` costs 60.3%). What still
// differs: the original derefs `*image` only inside the branch (edi is loaded
// early, `[edi]` after the `je`), ours derefs before the test; the ints and
// strings loops load p[1] before `*image` where the original loads p[0],
// `*image`, p[1] (the loop forms listed below were re-measured on top of this
// change: direct p[0]/p[1] 75.5%, `r = p; p += 2` 78.0%, no gain).
// PARTIAL 74.8% (737 bytes original, ours 742). Prologue, frame, the early name
// test, the compressed branch and now the ints and strings loops match. The
// deepseek-v4.1-flash pass landed three fixes on top of the 62.8% attempt:
//  * blob store: an `int need = h.reclen + e->len;` local with `if (e->size < need)`
//    made the size/len loads and the compare match the original (62.8 -> 71.7).
//  * blob record: dropping the `int a/b` locals and using `rec[0]/rec[1]` inline
//    put the loads in better registers (71.7 -> 73.1).
//  * ints and strings loops: `int a = p[0]; int b = p[1]; p += k;` with the
//    call using a/b matched the original's load order (73.1 -> 74.8).
// What still differs (all allocator, no structural differences left):
//  * `image` is loaded into edi after the `je` at 0x4b42e0 where the original
//    loads it at 0x4b42a7, before the test. Everything after is shifted 3 bytes.
//  * the doubles loop wants the original's `rec = p; p += 3;` shape (0x4b43bf)
//    but rec form still measures 73.9%, below the direct form kept here.
//  * the blob loop keeps c in esi and builds src in esi; the original keeps c
//    in ebx and builds src in edi, so the original reloads base into edx and
//    ours into ebx/edi. One allocator state.
// Also tried and rejected this pass: inline rec[0]/rec[1] in the ints loop
// (61.2%), local `double d` (58.8%), moving p += k after the call (63.4%),
// swapping the reclen/c assignment order (74.8%, unchanged), adding `int b`
// unconditionally (74.4%), `char* sect = *image + h.strOffset` before the test
// (70.8%, it also hoists the strOffset load the original keeps lazy).
// Previously rejected (do not repeat): doubles/strings `rec = p; p += k;` forms
// (50-59%), a `double` local (50.7%), ternary `e->len =`, `char* buf`/`int* p`
// (breaks the prologue), direct ints `p[0]/p[1]` with a late `p += 2` (50.9%),
// `int rl = h.reclen;` between the idx calls and the slot store (59.7%).

// Frame of the ORIGINAL off the disassembly (B = esp right after sub esp,0x42c):
//   B+0x00 buf        B+0x04 len (reused by the blob loop counter i)
//   B+0x08 base       B+0x0c end
//   B+0x10 p          B+0x14 header (0x30 bytes modelled, 0x20 read)
//   B+0x40 reclen     B+0x44 message[1000]
// 0x44 + 0x3e8 == 0x42c. MSVC 5 lays 4-byte scalars below the aggregates by
// size, so a separate `int reclen` local lands at B+0x14 and pushes the header
// to B+0x18; folding reclen into the (already address-taken) header makes it
// land at B+0x40 as the original does.
//
// Reads one 0x20-byte section header out of a HapiBank archive (called in a loop
// by 0x4b3770) and, when the caller's name matches the section name, unpacks the
// section body and files its records away in the current section of the parsed
// bank: integers, doubles, strings and raw blobs, in that order.
#include <stdio.h>
#include <string.h>

extern int __cdecl _strcmpi(const char* s1, const char* s2);

struct File_004b4270 {            // the open archive
    void* field_0;                // +0x00
    void* field_4;                // +0x04
    void* field_8;                // +0x08
    int field_c;                  // +0x0c
    void* field_10;               // +0x10
    void* field_14;               // +0x14
    char name[0x100];             // +0x18
};

long __stdcall FUN_004bb7a0(File_004b4270* file);
void __stdcall FUN_004bb7c0(File_004b4270* file, void* buf, int size);
void __stdcall FUN_004bb710(File_004b4270* file, long pos);
void* __cdecl FUN_004d8450(unsigned int size);
int __stdcall FUN_004d1b40(unsigned char* src);
int __stdcall FUN_004d1970(void* dest, void* src);
char* __stdcall FUN_004d1c60(int code);
void* __cdecl FUN_004d8580(void* ptr, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void __stdcall FUN_004b6290(char* message);

struct Entry_004b4270 {           // 0x14 bytes, one entry of a section
    int used;                     // +0x00
    int value;                    // +0x04, string offset or integer
    int size;                     // +0x08
    int len;                      // +0x0c
    char* buffer;                 // +0x10
};

struct Slot_004b4270 {            // 0x18 bytes, one section
    char unknown_0[8];
    int count;                    // +0x08
    int current;                  // +0x0c
    char unknown_10[4];
    Entry_004b4270* entries;      // +0x14
};

struct Table_004b4270 {
    char unknown_0[4];
    Slot_004b4270* slots;         // +0x04
    int index;                    // +0x08
};

class Class_004b4560 {
public:
    Table_004b4270* file;
    int FUN_004b4560(char* name);
};

class Class_004b4630 {
public:
    Table_004b4270* file;
    int FUN_004b4630(char* name, int value);
};

class Class_004b46c0 {
public:
    Table_004b4270* file;
    int FUN_004b46c0(char* name, double value);
};

class Class_004b4750 {
public:
    Table_004b4270* file;
    int FUN_004b4750(char* name, char* value);
};

class Class_004b49d0 {
public:
    Table_004b4270* table;
    int FUN_004b49d0(int value, int flag);
};

class Class_004b4a80 {
public:
    Table_004b4270* table;
    int FUN_004b4a80(char* name, int flag);
};

struct Header_004b4270 {          // 0x30 bytes modelled, first 0x20 read from file
    int size;                     // +0x00, of the whole section
    int strOffset;                // +0x04, of the section name
    int nInts;                    // +0x08
    int nDoubles;                 // +0x0c
    int nStrings;                 // +0x10
    int nBlobs;                   // +0x14
    int compressed;               // +0x18
    int unknown_1c[4];            // +0x1c
    int reclen;                   // +0x2c
};

class Class_004b4270 {
public:
    Table_004b4270* file;

    void FUN_004b4270(File_004b4270* fh, char** image, char* name);
};

// FUNCTION: 0x4b4270
void Class_004b4270::FUN_004b4270(File_004b4270* fh, char** image, char* name)
{
    int buf;
    int len;
    int base;
    int end;
    int* p;
    Header_004b4270 h;

    char message[1000];

    base = (int)FUN_004bb7a0(fh);
    FUN_004bb7c0(fh, &h, 0x20);
    end = base + h.size;
    char* sect = *image;
    if (name != 0 && _strcmpi(name, sect + h.strOffset) != 0) {
        FUN_004bb710(fh, end);
        return;
    }
    len = h.size - 0x20;
    if (len > 0) {
        if (h.compressed == 1) {
            char* tmp = (char*)FUN_004d8450(len);
            FUN_004bb7c0(fh, tmp, len);
            buf = (int)FUN_004d8450(FUN_004d1b40((unsigned char*)tmp));
            int err = FUN_004d1970((void*)buf, tmp);
            if (err != 0) {
                sprintf(message, "[HapiBank::LoadAccount] Decompression Error: %s\nFile: %s",
                        FUN_004d1c60(err), fh->name);
                FUN_004b6290(message);
            }
            FUN_004d85a0(tmp);
        } else {
            buf = (int)FUN_004d8450(len);
            FUN_004bb7c0(fh, (void*)buf, len);
        }
        ((Class_004b4560*)this)->FUN_004b4560(*image + h.strOffset);
        p = (int*)buf;
        {
            for (int i = 0; i < h.nInts; i++) {
                int a = p[0];
                int b = p[1];
                p += 2;
                ((Class_004b4630*)this)->FUN_004b4630(*image + a, b);
            }
        }
        {
            for (int i = 0; i < h.nDoubles; i++) {
                ((Class_004b46c0*)this)->FUN_004b46c0(*image + p[0], *(double*)(p + 1));
                p += 3;
            }
        }
        {
            for (int i = 0; i < h.nStrings; i++) {
                int a = p[0];
                int b = p[1];
                p += 2;
                ((Class_004b4750*)this)->FUN_004b4750(*image + a, *image + b);
            }
        }
        int i = 0;
        if (h.nBlobs > 0) {
            do {
                int* rec = p;
                p += 4;
                int c = rec[2];
                int idx;
                char* src;
                Entry_004b4270* e;
                h.reclen = rec[3];
                if (rec[0] < 0)
                    idx = ((Class_004b49d0*)this)->FUN_004b49d0(rec[1], 1);
                else
                    idx = ((Class_004b4a80*)this)->FUN_004b4a80(*image + rec[0], 1);
                ((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current = idx;
                src = (char*)(buf + (c - base) - 0x20);
                e = &((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index]
                        .entries[((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current];
                int need = h.reclen + e->len;
                if (e->size < need) {
                    e->buffer = (char*)FUN_004d8580(e->buffer, need);
                    e->size = need;
                }
                memcpy(e->buffer + e->len, src, h.reclen);
                e->len += h.reclen;
                e = &((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index]
                        .entries[((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current];
                int newLen = 0;
                if (e->size < 0)
                    newLen = e->size;
                e->len = newLen;
                i++;
            } while (i < h.nBlobs);
        }
        FUN_004bb710(fh, end);
        FUN_004d85a0((void*)buf);
    }
}