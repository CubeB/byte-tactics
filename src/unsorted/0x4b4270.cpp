// Decompiled by space-bunny-free. Names are provisional.
// NOT MATCHING YET: 46.0% (753 bytes against 737). LEFT AS ONE ALLOCATION
// STATE, not four problems.
//
// Frame layout of the ORIGINAL, read straight off the disassembly (all offsets
// are from E0 = esp right after "sub esp,0x42c" plus the four register
// pushes, so a "[esp+X]" in the body is E0+X-0x10):
//   E0+0x00 buf        (0x4b4324, 0x4b436d store; 0x4b438d, 0x4b447f load)
//   E0+0x04 len        (0x4b42eb), REUSED by the blob loop counter i
//                      (0x4b4411 stores 0, 0x4b4510/0x4b451b read+write it)
//   E0+0x08 base       (0x4b4293 stores it, 0x4b4476 loads it)
//   E0+0x0c end        (0x4b42b2 stores it, 0x4b4525 loads it)
//   E0+0x10 p, the record walk pointer (0x4b442a/0x4b4433)
//   E0+0x14 the 0x2c-byte section header (0x4b428b lea; size +0, strOffset
//                      +4, nInts +8, nDoubles +0xc, nStrings +0x10, nBlobs
//                      +0x14, compressed +0x18, so four unused ints follow)
//   E0+0x40 reclen     (0x4b4446 stores it, 0x4b4465/0x4b44c4 load it)
//   E0+0x40 the 0x3ec-byte sprintf buffer (0x4b433e and 0x4b4350 both lea
//                      E0+0x40, and 0x40+0x3ec == 0x42c == the frame size)
// reclen and the sprintf buffer SHARE one slot, so the frame is exactly 0x42c
// only if the two scopes' slots are shared, and the header must be 0x2c bytes.
//
// What my version gets wrong: it spends ONE extra home slot at E0+0x0c, so
// end lands at +0x10, p at +0x14 and the header at +0x18, and every header
// field read is 4 bytes high from there on. It also keeps arg1 in edi where
// the original keeps it in ebx (and puts this in ebx), and reloads arg2 from
// the stack in four places where the original holds it in edi from 0x4b42a7
// to the end. The four scattered diffs in the loop tail are the same state:
// the original's ebx holds the walked entry pointer from 0x4b44a1 to 0x4b44e6
// (so e->size is read BEFORE the lea, and e->len is re-read after memcpy),
// while mine materialises `e` as a statement first, so the reads happen after.
//
// Two spellings of the last store inside the blob loop are still unmatched.
// The original: "test ecx,ecx / jge / mov edx,ecx / mov [eax+0xc],edx", i.e.
// a real branch reusing the edx that memcpy already zeroed. Mine compiles
// "e->size < 0 ? e->size : 0" to "setge dl / dec edx / and ecx,edx", which is
// the if-converted form; an if/else spelling was not tried.
//
// TRIED AND REJECTED (do not repeat):
//  * No local `e` at all: spell the whole
//    table->slots[table->index].entries[table->slots[...].current] walk out at
//    every use, the way the original's two re-walks (0x4b4462 and 0x4b44e9)
//    suggest. 36.0% and 860 bytes: MSVC 5 does not CSE the four uses, so the
//    body explodes. The repeated expression is what the compiler produces from
//    ONE local pointer, not what it produces from four copies.
//  * Keeping the local `e` but writing only the LAST store as the full
//    expression, and `++i < nBlobs` instead of `i++; ... i < nBlobs`: exactly
//    46.0%, same 753 bytes, instruction for instruction the same diff. The
//    repeated expression is free here, so the last store's spelling is not the
//    cause of the setge there.
//  * `Header_004b4270` grown from 0x28 to the 0x2c the original needs: the
//    frame went to 0x430 and no offset improved, because the extra slot at
//    +0x0c is charged ahead of the locals, not inside the header.
// Reads one 0x20-byte section header out of a HapiBank archive (FUN_004b4270
// is called in a loop by 0x4b3770) and, when the caller's name matches the
// section name, unpacks the section body and files its records away in the
// current section of the parsed bank: integers, doubles, strings and raw
// blobs, in that order.
#include <stdio.h>
#include <string.h>

extern int _strcmpi(const char* s1, const char* s2);

struct File_004b4270 {            // the open archive
    void* handle;                 // +0x00
    int field_4;                  // +0x04
    char unknown_8[0x10];
    char* name;                   // +0x18
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

struct Entry_004b4270 {           // 0x14 bytes, one key of a section
    int used;                     // +0x00
    char* name;                   // +0x04
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

struct Header_004b4270 {          // 0x20 bytes
    int size;                     // +0x00, of the whole section
    int strOffset;                // +0x04, of the section name
    int nInts;                    // +0x08
    int nDoubles;                 // +0x0c
    int nStrings;                 // +0x10
    int nBlobs;                   // +0x14
    int compressed;               // +0x18
    int unknown_1c[3];            // +0x1c
};

// The local frame the original builds (offsets from the bottom of the 0x42c
// byte frame): buf +0x10, len +0x14, base +0x18, end +0x1c, walk pointer
// +0x20, the 0x20-byte section header +0x24 (0x2c bytes as declared, so
// reclen lands at +0x50), reclen +0x50 and the sprintf buffer +0x54.

class Class_004b4270 {
public:
    Table_004b4270* file;

    void FUN_004b4270(File_004b4270* file, char** image, char* name);
};

// FUNCTION: 0x4b4270
void Class_004b4270::FUN_004b4270(File_004b4270* fh, char** image, char* name)
{
    Header_004b4270 h;
    int buf;
    int len;
    int base;
    int end;
    int* p;
    int reclen;
    char message[0x3ec];
    int i;

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
            FUN_004bb7c0(fh, (void*)base, len);
            buf = (int)FUN_004d8450(FUN_004d1b40((unsigned char*)tmp));
            if (FUN_004d1970((void*)buf, tmp) != 0) {
                sprintf(message, "[HapiBank::LoadAccount] Decompression Error: %s\nFile: %s",
                        FUN_004d1c60(0), fh->name);
                FUN_004b6290(message);
            }
            FUN_004d85a0(tmp);
        } else {
            buf = (int)FUN_004d8450(len);
            FUN_004bb7c0(fh, (void*)buf, len);
        }
        p = (int*)buf;
        ((Class_004b4560*)this)->FUN_004b4560(*image + h.strOffset);
        for (i = 0; i < h.nInts; i++) {
            ((Class_004b4630*)this)->FUN_004b4630(*image + p[0], p[1]);
            p += 2;
        }
        for (i = 0; i < h.nDoubles; i++) {
            ((Class_004b46c0*)this)->FUN_004b46c0(*image + p[0], *(double*)(p + 1));
            p += 3;
        }
        for (i = 0; i < h.nStrings; i++) {
            ((Class_004b4750*)this)->FUN_004b4750(*image + p[0], *image + p[1]);
            p += 2;
        }
        i = 0;
        if (h.nBlobs > 0) {
            do {
                int* rec = p;
                int a, b, c, idx;
                char* src;
                Entry_004b4270* e;
                p += 4;
                a = rec[0];
                b = rec[1];
                c = rec[2];
                reclen = rec[3];
                if (a < 0)
                    idx = ((Class_004b49d0*)this)->FUN_004b49d0(b, 1);
                else
                    idx = ((Class_004b4a80*)this)->FUN_004b4a80(*image + a, 1);
                ((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current = idx;
                src = (char*)(buf + (c - base) - 0x20);
                e = &((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index]
                        .entries[((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current];
                if (reclen + e->len > e->size) {
                    e->buffer = (char*)FUN_004d8580(e->buffer, reclen + e->len);
                    e->size = reclen + e->len;
                }
                memcpy(e->buffer + e->len, src, reclen);
                e->len += reclen;
                e = &((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index]
                        .entries[((Class_004b49d0*)this)->table->slots[((Class_004b49d0*)this)->table->index].current];
                e->len = e->size < 0 ? e->size : 0;
                i++;
            } while (i < h.nBlobs);
        }
        FUN_004bb710(fh, end);
        FUN_004d85a0((void*)buf);
    }
}
