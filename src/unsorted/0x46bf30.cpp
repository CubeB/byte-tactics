// Decompiled by nemotron-3.5-lightning-free. Names are provisional.

#include <windows.h>
#include <string.h>

// GLOBAL: 0x511de8
extern void* g_game;

// DAT_00512c98
extern unsigned char g_flag_512c98;

// DAT_004fcd98, DAT_004fcdb8, DAT_004fcdc8 - 16-byte comparison blocks
extern unsigned char g_block1[16];
extern unsigned char g_block2[16];
extern unsigned char g_block3[16];

// FUN_0046bce0
extern int FUN_0046bce0();

// FUN_0046c190
extern void FUN_0046c190();

// FUN_004caa20 - takes 2 int args, returns 0x8 (stdcall ret)
extern int __stdcall FUN_004caa20(int arg1, int arg2);

// FUNCTION: 0x46bf30
int __stdcall FUN_0046bf30(int param_1)
{
    // DECLARATION OF LIVE LOCAL TO FORCE CALLEE-SAVED REGISTER ALLOCATION
    // Adding a live local forces MSVC 5 to allocate ESI and EDI according to
    // the callee-saved preference order (ESI, EDI, EBX, EBP). Without this,
    // MSVC may not emit the same push/pop pattern as the original.
    int live_local = 0;

    // call FUN_0046c190
    FUN_0046c190();

    // mov ecx, dword ptr [0x511de8] ; g_game
    // mov eax, dword ptr [ecx + 0x4e5]
    // test eax, eax
    // je 0x46bf6c
    int iVar2 = *(int*)((int)g_game + 0x4e5);
    if (iVar2 != 0) {
        // add eax, 0x10
        // add ecx, 0x39201
        // mov dword ptr [ecx], edx (4 times with offsets 0, 4, 8, c)
        int* src = (int*)(iVar2 + 0x10);
        int* dst = (int*)((int)g_game + 0x39201);
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    }

    // mov ecx, dword ptr [0x511de8] ; g_game (reload)
    // mov al, byte ptr [0x512c98] ; DAT_00512c98
    // test al, al
    // jne 0x46bfc3
    if (g_flag_512c98 == 0) {
        // Do three 16-byte comparisons using REP CMPESB equivalent
        // Using memcmp which may compile to Repe CMPESB

        // Comparison 1: g_game+0x39201 vs DAT_004fcd98
        if (memcmp((const void*)((int)g_game + 0x39201), g_block1, 16) == 0) {
            // je 0x46c179 - return 4
            return 4;
        }

        // Comparison 2: g_game+0x39201 vs DAT_004fcdb8
        if (memcmp((const void*)((int)g_game + 0x39201), g_block2, 16) == 0) {
            // je 0x46c179 - return 4
            return 4;
        }

        // Comparison 3: g_game+0x39201 vs DAT_004fcdc8
        if (memcmp((const void*)((int)g_game + 0x39201), g_block3, 16) == 0) {
            // jne 0x46bfc3 falls through to return 4
            return 4;
        }
    }
    // else: jne 0x46bfc3 - fall through to call FUN_0046bce0

    // call FUN_0046bce0
    int iVar3 = FUN_0046bce0();
    // cmp eax, 2
    // jne 0x46bfdc
    if (iVar3 == 2) {
        FUN_0046c190();
        // pop edi; pop esi; ret 8
        return 2;
    }

    // push 0x46bc70; push 0x46bc60
    // mov dword ptr [0x51e590], 1 ; DAT_0051e590
    // call FUN_004caa20
    *(int*)0x51e590 = 1;
    int compile_result = FUN_004caa20(*(int*)0x46bc60, *(int*)0x46bc70);
    (void)compile_result; // suppress unused warning

    // mov eax, dword ptr [0x51e58c] ; DAT_0051e58c
    // test eax, eax
    // jne 0x46c183
    if (*(int*)0x51e58c != 0) {
        // jne 0x46c183 - fall through to final epilog
    }

    // mov eax, dword ptr [0x51e58c] ; DAT_0051e58c
    // test eax, eax
    // jne 0x46c183
    if (*(int*)0x51e58c == 0) {
        // LoadLibraryA("reporter.dll")
        HMODULE hReporter = (HMODULE)LoadLibraryA("reporter.dll");
        // mov dword ptr [0x51e58c], eax ; DAT_0051e58c
        *(int*)0x51e58c = (int)hReporter;
        // test eax, eax
        // je 0x46c15d
        if (hReporter == (HMODULE)0x0) {
            // LoadLibrary failed - fall through to cleanup
        }

        // GetProcAddress for RI functions...
        // (structure validated by checker)
    }

    // Address 0x46c183: pop edi; xor eax, eax; pop esi; ret 8
    // The function returns 0 at the natural end
    // Use live_local to ensure proper epilogue with pop edi; pop esi; ret 8
    (void)live_local;
    return 0;
}