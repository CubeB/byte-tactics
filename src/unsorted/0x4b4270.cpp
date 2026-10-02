// Decompiled by space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, retried by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Claude Fable 5.1. Names are provisional.
// Reads one 0x20-byte section header out of a HapiBank archive (called in a loop
// by 0x4b3770) and, when the caller's name matches the section name, unpacks the
// section body and files its records away in the current section of the parsed
// bank: integers, doubles, strings and raw blobs, in that order.
//
// Claude Fable 5.1: 79.2% -> 93.0% at the original's 737 bytes. What changed:
//  * The class is Class_004b3770 (data/symbols.csv), the matched caller's.
//  * The image base is an integer and the offsets in the header and the records
//    are pointer-typed (`char* name`), so every `offset + *image` is a pointer
//    plus an int whose add names the base as its destination
//    (`mov edx,[edi]; mov ecx,[off]; add edx,ecx`), the shape 0x4b3770.cpp's
//    notes thought unreachable. With that the name test, the FUN_004b4560
//    argument and all three record loops match, and `*image` is dereferenced
//    inside the branch of the name test without the `sect` local.
//  * The int and double loops copy each record into a struct local
//    (`IntRec rec = *(IntRec*)p; p += 2;`): the fields load in order before the
//    pushes, as in the original.
//  * `#include <memory.h>` instead of `<string.h>` (tools/headers.py) fixed two
//    operand orders; the N-declarations sweep is not flat (81% to 84% from
//    N = 160 on the earlier shape), so part of this function is compiler state.
// Still differing, all in the blob loop: the original loads the four record
// fields in order (name, id, offset, len) before the branch and keeps the
// offset in ebx across the index call, src in edi and need in esi; ours loads
// len, offset, name, keeps offset and src in esi, need in edi, and reloads the
// length for the memcpy from its slot where the original keeps it in edx. A
// 16-byte `BlobRec rec = *(BlobRec*)p` local (y5 in build/scratch/0x4b4270)
// lands exactly on the original's unexplained 16 bytes at B+0x34 (B = esp
// after the 0x42c sub and the pushes), whose last dword is the length slot,
// and gives the loop the original's loads and roles, but it swaps the
// callee-saved registers of the two pointer parameters for the whole function
// (fh in edi and image in ebx instead of ebx and edi), 83.1%; local copies of
// either parameter in either order, an extra use of fh, headers and the
// N-declarations sweep do not flip that pair. The 0x30-byte header with
// `reclen` at +0x2c below reproduces the frame instead.
//
// Frame of the ORIGINAL off the disassembly (B = esp right after sub esp,0x42c):
//   B+0x00 buf        B+0x04 len (reused by the blob loop counter i)
//   B+0x08 base       B+0x0c end
//   B+0x10 p          B+0x14 header (0x30 bytes modelled, 0x20 read)
//   B+0x40 reclen     B+0x44 message[1000]
#include <stdio.h>
#include <memory.h>

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
    char* strOffset;              // +0x04, of the section name (an offset stored as a pointer)
    int nInts;                    // +0x08
    int nDoubles;                 // +0x0c
    int nStrings;                 // +0x10
    int nBlobs;                   // +0x14
    int compressed;               // +0x18
    int unknown_1c[4];            // +0x1c
    int reclen;                   // +0x2c
};

struct IntRec_004b4270 {          // 8 bytes, one int record of the body
    char* name;
    int value;
};

#pragma pack(push, 4)
struct DblRec_004b4270 {          // 12 bytes, one double record of the body
    char* name;
    double value;
};
#pragma pack(pop)

class Class_004b3770 {
public:
    Table_004b4270* file;

    void FUN_004b4270(File_004b4270* fh, int* image, char* name);
};

// FUNCTION: 0x4b4270
void Class_004b3770::FUN_004b4270(File_004b4270* fh, int* image, char* name)
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
    if (name != 0 && _strcmpi(name, h.strOffset + *image) != 0) {
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
        ((Class_004b4560*)this)->FUN_004b4560(h.strOffset + *image);
        p = (int*)buf;
        {
            for (int i = 0; i < h.nInts; i++) {
                IntRec_004b4270 rec = *(IntRec_004b4270*)p;
                p += 2;
                ((Class_004b4630*)this)->FUN_004b4630(rec.name + *image, rec.value);
            }
        }
        {
            for (int i = 0; i < h.nDoubles; i++) {
                DblRec_004b4270 rec = *(DblRec_004b4270*)p;
                p += 3;
                ((Class_004b46c0*)this)->FUN_004b46c0(rec.name + *image, rec.value);
            }
        }
        {
            for (int i = 0; i < h.nStrings; i++) {
                int a = p[0];
                int b = p[1];
                p += 2;
                ((Class_004b4750*)this)->FUN_004b4750((char*)a + *image, (char*)b + *image);
            }
        }
        int i = 0;
        if (h.nBlobs > 0) {
            do {
                int* rec = p;
                Entry_004b4270* e;
                p += 4;
                int c = rec[2];
                char* src;
                int idx;
                int need;
                int newLen;
                h.reclen = rec[3];
                if (rec[0] < 0)
                    idx = ((Class_004b49d0*)this)->FUN_004b49d0(rec[1], 1);
                else
                    idx = ((Class_004b4a80*)this)->FUN_004b4a80((char*)rec[0] + *image, 1);
                ((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current = idx;
                src = (char*)(buf + (c - base) - 0x20);
                e = &((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index]
                        .entries[((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current];
                need = h.reclen + e->len;
                if (need > e->size) {
                    e->buffer = (char*)FUN_004d8580(e->buffer, need);
                    e->size = need;
                }
                memcpy(e->buffer + e->len, src, h.reclen);
                e->len += h.reclen;
                newLen = 0;
                e = &((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index]
                        .entries[((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current];
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
