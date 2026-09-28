// Decompiled by space-bunny-free. Names are provisional.
// Walks a chunked file. The 4 byte total size is read at offset 4 and grown by
// 8, then every record is an 8 byte header: a 4 byte tag at 0xc and a 4 byte
// record length behind it. A record tagged "data" gives its length back, any
// other tag moves the walk on by that length plus the 8 header bytes, and the
// walk gives up with 0 once it passes the total size. The file handle goes
// into esi, the loop offset into edi, the record length into ecx, and the dead
// parameter slot is reused for that length (0x4d094e, 0x4d09a0) and the tag
// buffer sits in the first local dword (0x4d0941, 0x4d0993).
//
// NOT MATCHED yet (206 bytes original, ours 213, 76.1%). Two differences left,
// both in the loop body's register use:
//
// 1. The original computes the seek offset as
//        mov ecx,[len]; add ecx,edi; push ecx
//    reusing the register that already held the length (so the length has to be
//    reloaded after the call, which the original does at 0x4d0985), while this
//    source makes MSVC pick a fresh register and fold the sum into a lea
//        mov ecx,[len]; lea edx,[edi+ecx]; push edx
//    Swapping the operands (`pos + len`) and both signednesses of the locals
//    changed nothing, so the choice must come from something else in the
//    original's source shape that I could not recover.
//
// 2. The original stores the total size as
//        mov edx,[size]; add edx,8; mov [size],edx
//    (0x4d092e to 0x4d0938) while this source gets the in place
//    `add dword ptr [size], 8`. MSVC only refuses the in place form when the
//    load and the store are different stack objects, so the original probably
//    assigned the total to a second local that shares the dead first local's
//    slot. Two spellings of that were tried and MSVC 5 promoted the second
//    local into ebx instead (a `push ebx` appears and the frame goes wrong), so
//    it seems this compiler has no way to be pushed into it. `size = size + 8`
//    and `unsigned int size` were both tried for the one variable spelling and
//    are byte for byte identical to `size += 8` with `int`.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

// FUNCTION: 0x4d0910
int __stdcall FUN_004d0910(void* file)
{
    int size;
    char tag[4];
    int len;
    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &size, 4);
    size += 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, tag, 4);
    FUN_004bb7c0(file, &len, 4);
    unsigned int pos = 0x14;
    while (strncmp(tag, "data", 4) != 0) {
        FUN_004bb710(file, len + pos);
        pos += len;
        if (pos >= size)
            return 0;
        FUN_004bb7c0(file, tag, 4);
        FUN_004bb7c0(file, &len, 4);
        pos += 8;
    }
    return len;
}
