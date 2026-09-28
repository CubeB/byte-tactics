// Decompiled by Space Bunny Free. Names are provisional.
// Builds a DirectPlay compound address for the service provider selected in
// g_game + 0x39201 and hands it to the lobby's CreateCompoundAddress.
// Returns 0 on success, with the address block and its size in the two out
// parameters; otherwise the HRESULT from the lobby.
#include <windows.h>
#include <string.h>

// One element of the compound address list: a GUID naming the address type,
// the byte size of the data and the data itself.
struct Guid_00441c30 {
    unsigned long data1;
    unsigned long data2;
    unsigned long data3;
    unsigned long data4;
};

struct Elem_00441c30 {         // 0x18 bytes
    Guid_00441c30 guid;        // +0x0
    unsigned long size;        // +0x10
    void* data;                // +0x14
};

// The address used for the "serial" service provider: a 0x14 byte block
// written in DAT_00512770.
struct Serial_00441c30 {        // 0x14 bytes
    char unknown_0[8];
    unsigned long unknown_8;
    unsigned long unknown_c;
    unsigned long unknown_10;
};

extern char* g_game;

extern Guid_00441c30 DAT_004fce88;   // address type of the service provider element
extern Guid_00441c30 DAT_004fcdc8;   // serial
extern Guid_00441c30 DAT_004fcda8;   // TCP/IP
extern Guid_00441c30 DAT_004fcd98;   // IPX
extern Guid_00441c30 DAT_004fcdb8;   // modem
extern Guid_00441c30 DAT_004fcec8;   // address type of the save list name
extern Guid_00441c30 DAT_004fcea8;   // address type of the save list number
extern Guid_00441c30 DAT_004fcee8;   // address type of a named host
extern Guid_00441c30 DAT_004fcf08;   // address type of the serial settings

extern char DAT_005119b8[];
extern char* DAT_00512980;
extern Serial_00441c30 DAT_00512770;

char* __stdcall FUN_004a0d00(void* obj, char* name, char* buf);
int __stdcall FUN_004ca880(void* net, void* elements, unsigned long count, void* address,
                           unsigned long* size);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

// PARTIAL, 94.0% (973 of 973 bytes, exact size). Everything matches except the
// order of the two blocks at the tail, about 30 bytes. Recorded here because
// the size matching exactly means this is purely a block-layout question, and
// the layout is the last thing left.
//
// The original's tail, with its two exit blocks:
//
//   0x441fa9: test edi,edi
//   0x441fab: jge  0x441fd9      ; forward, past the cleanup
//   0x441fad: CLEANUP  test ebx,ebx / je / GlobalUnlock / GlobalFree
//   0x441fcb: mov eax,edi / epilogue / ret 8
//   0x441fd9: SUCCESS  out-stores / xor eax,eax / epilogue / ret 8
//
// and earlier, `cmp edi,0x8877001e / jne 0x441fad` and, after the failed
// allocation, `mov edi,0x8007000e / jmp 0x441fad`. So the cleanup block has
// three predecessors, two of them forward jumps, and the success block is the
// LAST thing in the function with one predecessor. Note also that the success
// block does its `pop edi` at 0x441feb, before the two stores, while the
// cleanup does its `pop edi` at 0x441fcd, after the calls.
//
// This file instead shares the test between both arms: `test edi,edi / jl` to
// a backward join, with the success stores inline and the cleanup laid out
// after them. Same instructions, different placement.
//
// Measured negatives, about thirty shapes by the first pass and four more here,
// all by compiling a variant and scoring it with `check.py --sym`:
//  - the cleanup as a labelled block at the outer level with `goto success`
//    after the retry (93.4%), which is the layout above written out in C and
//    still lays the success block before the cleanup;
//  - the same with the failure written as `if (result < 0) goto cleanup`
//    (93.2%);
//  - the same with the retry wrapped in `do { ... } while (0)` to add a level
//    without emitting code (93.4%), which was the worker's suggested next axis
//    and does not move it;
//  - duplicating the cleanup into the failure arm of the retry so both exits
//    are self-contained (39.9%), far worse.
//
// The general finding, on a harness of about seven minimal shapes, is that
// every spelling which keeps the test *inside* the retry block makes MSVC 5
// lay the retry block out last, and only the shared-join spelling keeps it
// inline, which is the opposite of what the original does. So the one that
// scores best here is structurally wrong, and the one that matches the
// original's shape scores 0.6 points lower. If anyone picks this up, the thing
// to look for is an MSVC 5 block-ordering or level-numbering behaviour rather
// than another source spelling; roughly thirty spellings is enough to be
// confident the answer is not in the source.
//
// Everything before the tail is solved and worth keeping: the frame is
// `sub esp, 0x2b4` with `size` at +0xc, `elements[3]` at +0x10, `guid` at
// +0x58 and the three `char[200]` buffers at +0x68, +0x130 and +0x1f8, which
// needs all three declared at function top so MSVC does not merge two of them;
// `memset(buf1, 0, sizeof buf1)` for the plain `rep stosd` (a `= ""`
// initialiser produces MSVC's byte-then-stosd trick instead); `FUN_004d83b0`
// is `__cdecl` and the other two callees `__stdcall`; `HGLOBAL block = 0;`
// declared before `unsigned long size = 0;` gives the `xor ebx,ebx` and the
// `[esp+4],ebx` in the prologue; and moving the `result >= 0` test out of the
// else into the outer block is what produces the correct 973-byte size at all.
//
// FUNCTION: 0x441c30
int __stdcall FUN_00441c30(HGLOBAL* addressOut, unsigned long* sizeOut)
{
    HGLOBAL block = 0;
    unsigned long size = 0;
    Elem_00441c30 elements[3];
    Guid_00441c30 guid;
    char buf1[200];
    char buf2[200];
    char buf3[200];
    unsigned long count;
    int result;

    guid = *(Guid_00441c30*)(g_game + 0x39201);

    if (memcmp(&guid, &DAT_004fcdc8, sizeof(Guid_00441c30)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcdc8;
        memset(buf1, 0, sizeof(buf1));
        char* s = DAT_00512980;
        if (s == 0) {
            s = DAT_005119b8;
        }
        lstrcpyA(buf1, s);
        elements[1].guid = DAT_004fcec8;
        elements[1].size = lstrlenA(buf1) + 1;
        elements[1].data = buf1;
        lstrcpyA(buf2, FUN_004a0d00(g_game + 0x519, "NUMBER", 0));
        elements[2].guid = DAT_004fcea8;
        elements[2].size = lstrlenA(buf2) + 1;
        elements[2].data = buf2;
        count = 3;
    } else if (memcmp(&guid, &DAT_004fcda8, sizeof(Guid_00441c30)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcda8;
        char* t = FUN_004a0d00(g_game + 0x519, "ADDRESS", 0);
        if (t == 0) {
            t = DAT_005119b8;
        }
        lstrcpyA(buf3, t);
        elements[1].guid = DAT_004fcee8;
        elements[1].size = lstrlenA(buf3) + 1;
        elements[1].data = buf3;
        count = 2;
    } else if (memcmp(&guid, &DAT_004fcd98, sizeof(Guid_00441c30)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcd98;
        count = 1;
    } else if (memcmp(&guid, &DAT_004fcdb8, sizeof(Guid_00441c30)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcdb8;
        DAT_00512770.unknown_8 = 0;
        DAT_00512770.unknown_c = 0;
        DAT_00512770.unknown_10 = 3;
        elements[1].guid = DAT_004fcf08;
        elements[1].size = 0x14;
        elements[1].data = &DAT_00512770;
        count = 2;
    } else {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &guid;
        count = 1;
    }

    result = FUN_004ca880(g_game + 0x14, elements, count, 0, &size);
    if (result == 0x8877001e) {
        block = (HGLOBAL)FUN_004d83b0("COMPOUND ADDR", size);
        if (block == 0) {
            result = 0x8007000e;
        } else {
            result = FUN_004ca880(g_game + 0x14, elements, count, block, &size);
        }
        if (result >= 0) {
            *addressOut = block;
            *sizeOut = size;
            return 0;
        }
    }
    if (block != 0) {
        GlobalUnlock(GlobalHandle(block));
        GlobalFree(GlobalHandle(block));
    }
    return result;
}
