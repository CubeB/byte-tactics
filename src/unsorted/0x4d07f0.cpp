// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 69.8% (269 of 275 bytes; the diff is 12 instructions).
//
// What still differs, all of it register choice or one missing store, not
// semantics (every instruction and every stack offset that the checker compares
// matches except these):
//
// 1. 0x4d0814: the original stores the grown total size as
//        mov edx,[esp+8] / push 0xc / add edx,8 / push esi / mov [esp+0x10],edx
//    while `size += 8` gives the in place `add dword ptr [esp+0x10], 8`.
//    `size = size + 8`, `int` and `unsigned int` spellings are all identical.
//    Sibling 0x4d0910 hit exactly this and had the same result, while
//    0x4d0720 (same idiom, 2 parameters, no tail) does get the register form,
//    so the trigger is somewhere in the tail that I did not isolate. Defining
//    the real preceding function 0x4d0720 above this one in the same file (the
//    trick from the guide) changed nothing here either.
//
// 2. 0x4d085c: the original computes the seek offset destructively,
//        mov ecx,[pos] / add ecx,edi / push ecx,
//    and this source makes MSVC allocate a fresh temporary and fold the sum
//    into a lea, `mov ecx,[pos] / lea edx,[edi+ecx] / push edx`. Swapping the
//    operands (`off + pos`, `pos + off`), both signednesses, and moving every
//    declaration changed nothing. 0x4d0910 reports the same.
//
// 3. The 0x10 byte tail that copies the record out: the original keeps the
//    three output pointers in eax and the three values in ecx and edx and
//    sinks `pop edi` between the first and second store and `pop esi` to the
//    end, while this source allocates the pointers in ecx, the values in edx
//    and eax, and pops both registers before the first store. The instruction
//    sequence, the order and every esp offset agree; only the register
//    numbering differs. Three spellings of the stores (a cast, `& 0xffff` and
//    named temporaries) all compile identically.
//
// The loop shape itself is exact: a `for(;;)` with the tag test in the
// preheader, the two exits (`n = pos` on the match, `n = 0` when the walk runs
// past the total) placed as a jump over the zero block, and the running offset
// in edi with the record length in the dead first parameter slot.
//
// Reads a sound file's marker table: the dword at offset 4 is the table size
// and grows by the 8 header bytes, then the table is a run of [4 byte tag]
// [4 byte length] records. The record tagged "fmt " must claim at least 0x10
// bytes, and its 16 byte body is reported as the dword at +4, the word at +0xa
// and the word at +2. Returns 0 when the tag is not found.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

// FUNCTION: 0x4d07f0
int __stdcall FUN_004d07f0(void* file, int* out1, int* out2, int* out3)
{
    unsigned int size;
    char name[4];
    unsigned int pos;
    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &size, 4);
    size += 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, name, 4);
    FUN_004bb7c0(file, &pos, 4);
    unsigned int off = 0x14;
    unsigned int n;
    for (;;) {
        if (strncmp(name, "fmt ", 4) == 0) {
            n = pos;
            break;
        }
        FUN_004bb710(file, pos + off);
        off += pos;
        if (off >= size) {
            n = 0;
            break;
        }
        FUN_004bb7c0(file, name, 4);
        FUN_004bb7c0(file, &pos, 4);
        off += 8;
    }
    if (n < 0x10)
        return 0;
    char data[0x14];
    FUN_004bb7c0(file, data, 0x10);
    *out1 = *(int*)(data + 4);
    *out2 = (unsigned short)*(int*)(data + 10);
    *out3 = (unsigned short)*(int*)(data + 2);
    return 1;
}
