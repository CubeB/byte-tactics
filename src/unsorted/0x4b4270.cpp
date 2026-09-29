// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 61.1% (737 bytes original, ours 755). Frame, prologue, the early
// name test, the compressed branch, the ints loop and the tail of the blob loop
// all match. What still differs is ONE allocator state in the loop bodies:
//  * `image` is loaded into edi at its first use where the original loads it at
//    0x4b42a7, before the test.
//  * the doubles loop wants `int* rec = p; p += 3;` (0x4b43bf) but any such
//    spelling drops the whole function to ~50%, so the plain p form is kept.
//  * in the blob loop the original keeps c in ebx and reclen in esi and builds
//    src in edi; ours puts c in esi and builds src in esi.
// TRIED AND REJECTED (do not repeat): doubles/strings `rec = p; p += k;` forms
// (50-59%), a `double` local for the double argument (50.7%), ternary `e->len =`.
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

extern int _strcmpi(const char* s1, const char* s2);

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
    if (name != 0 && _strcmpi(name, *image + h.strOffset) != 0) {
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
                int* rec = p;
                p += 2;
                ((Class_004b4630*)this)->FUN_004b4630(*image + rec[0], rec[1]);
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
                ((Class_004b4750*)this)->FUN_004b4750(*image + p[0], *image + p[1]);
                p += 2;
            }
        }
        if (h.nBlobs > 0) {
            int i = 0;
            do {
                int* rec = p;
                int a, b, c, idx;
                char* src;
                Entry_004b4270* e;
                p += 4;
                a = rec[0];
                b = rec[1];
                c = rec[2];
                h.reclen = rec[3];
                if (a < 0)
                    idx = ((Class_004b49d0*)this)->FUN_004b49d0(rec[1], 1);
                else
                    idx = ((Class_004b4a80*)this)->FUN_004b4a80(*image + rec[0], 1);
                ((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current = idx;
                src = (char*)(buf + (c - base) - 0x20);
                e = &((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index]
                        .entries[((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current];
                if (h.reclen + e->len > e->size) {
                    e->buffer = (char*)FUN_004d8580(e->buffer, h.reclen + e->len);
                    e->size = h.reclen + e->len;
                }
                memcpy(e->buffer + e->len, src, h.reclen);
                e->len += h.reclen;
                e = &((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index]
                        .entries[((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current];
                e->len = 0;
                if (e->size < 0)
                    e->len = e->size;
                i++;
            } while (i < h.nBlobs);
        }
        FUN_004bb710(fh, end);
        FUN_004d85a0((void*)buf);
    }
}
