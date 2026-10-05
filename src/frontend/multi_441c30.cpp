// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash. Names are provisional.
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

char* __stdcall GetGadgetText(void* obj, char* name, char* buf);
int __stdcall HAPINET_createcompoundaddress(void* net, void* elements, unsigned long count, void* address,
                           unsigned long* size);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

// MATCH. The tail is a block-ordering question, and the ordering rule is that
// MSVC 5 lays out a conditional's TRUE successor first, in a depth-first walk
// from the entry block. The original wants the retry test's true arm to be the
// cleanup (so the test emits `jge success` and falls into the cleanup), and the
// cleanup to stay the single block shared by the first call's failure, the
// failed allocation and the retry's failure. Putting the cleanup label inside
// the `if (result < 0)` body, with both earlier failures reaching it by `goto`,
// gives exactly that: the label is the true arm, the shared block keeps three
// predecessors, and the success stores move after it. Writing the same control
// flow as a structured if/else, or with the label after the retry, makes the
// success block the fallthrough instead and scores 94.0%.
//
// The frame is `sub esp, 0x2b4` with `size` at +0xc, `elements[3]` at +0x10,
// `guid` at +0x58 and the three `char[200]` buffers at +0x68, +0x130 and +0x1f8,
// which needs all three declared at function top so MSVC does not merge two of
// them; `memset(buf1, 0, sizeof buf1)` for the plain `rep stosd` (a `= ""`
// initialiser produces MSVC's byte-then-stosd trick instead); `FUN_004d83b0`
// is `__cdecl` and the other two callees `__stdcall`; and `HGLOBAL block = 0;`
// declared before `unsigned long size = 0;` gives the `xor ebx,ebx` and the
// `[esp+4],ebx` in the prologue.
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
        lstrcpyA(buf2, GetGadgetText(g_game + 0x519, "NUMBER", 0));
        elements[2].guid = DAT_004fcea8;
        elements[2].size = lstrlenA(buf2) + 1;
        elements[2].data = buf2;
        count = 3;
    } else if (memcmp(&guid, &DAT_004fcda8, sizeof(Guid_00441c30)) == 0) {
        elements[0].guid = DAT_004fce88;
        elements[0].size = 0x10;
        elements[0].data = &DAT_004fcda8;
        char* t = GetGadgetText(g_game + 0x519, "ADDRESS", 0);
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

    result = HAPINET_createcompoundaddress(g_game + 0x14, elements, count, 0, &size);
    if (result != 0x8877001e) goto cleanup;
    block = (HGLOBAL)FUN_004d83b0("COMPOUND ADDR", size);
    if (block == 0) {
        result = 0x8007000e;
        goto cleanup;
    }
    result = HAPINET_createcompoundaddress(g_game + 0x14, elements, count, block, &size);
    if (result < 0) {
cleanup:
        if (block != 0) {
            GlobalUnlock(GlobalHandle(block));
            GlobalFree(GlobalHandle(block));
        }
        return result;
    }
    *addressOut = block;
    *sizeOut = size;
    return 0;
}
