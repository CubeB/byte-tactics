// Decompiled by space-bunny-free. Names are provisional.
// Walks the call stack of the calling thread with imagehelp's StackWalk,
// collecting one address per frame into an array, until the array is full, the
// walk fails, or a frame field comes back zero. DAT_00528ac0 is StackWalk,
// DAT_00528ac4 SymFunctionTableAccess, DAT_00528ac8 SymGetModuleBase, all three
// resolved by GetProcAddress in the imagehelp loader next door. 0x14c is
// IMAGE_FILE_MACHINE_I386.
#include <windows.h>
#include <string.h>

extern char DAT_00528aa8;                     // "process handle read" flag
extern HANDLE DAT_00528ab0;                   // cached process handle
extern void* DAT_00528ac4;                    // SymFunctionTableAccess
extern void* DAT_00528ac8;                    // SymGetModuleBase

typedef BOOL (__stdcall *StackWalk_004de700)(DWORD, HANDLE, HANDLE, void*, void*, void*, void*, void*, void*);
extern StackWalk_004de700 DAT_00528ac0;       // StackWalk

// The frame description StackWalk fills in.
struct Frame_004de700 {
    int addrFrame;                             // +0x00
    int unknown_04;
    int flags0;                                // +0x08
    char unknown_0c[0x0c];
    int addrStack;                             // +0x18
    char unknown_1c[4];
    int flags1;                                // +0x20
    int handler;                               // +0x24
    char unknown_28[4];
    int flags2;                                // +0x2c
    char unknown_30[0x40];
};

// FUNCTION: 0x4de700
void __cdecl FUN_004de700(int param1, int param2, int param3, int param4, int* param5, int param6, int* param7)
{
    Frame_004de700 frame;
    memset(&frame, 0, sizeof(frame));
    frame.addrFrame = param3;
    frame.flags0 = 3;
    frame.flags2 = 3;
    frame.flags1 = 3;
    frame.handler = param2;
    frame.addrStack = param1;
    if (!(DAT_00528aa8 & 1)) {
        DAT_00528aa8 |= 1;
        DAT_00528ab0 = GetCurrentProcess();
    }
    HANDLE thread = GetCurrentThread();
    while (*param7 < param6) {
        if (!DAT_00528ac0(0x14c, DAT_00528ab0, thread, &frame, 0, 0, DAT_00528ac4, DAT_00528ac8, 0)) {
            if (*param7 == 0) {
                *param7 = 1;
                param5[0] = param3;
            }
            return;
        }
        if (frame.addrStack == 0)
            return;
        if (frame.addrFrame == 0)
            return;
        if (param4 == 0) {
            param5[*param7] = frame.addrFrame;
            ++(*param7);
        } else {
            --param4;
        }
    }
}
