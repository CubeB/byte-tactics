// Decompiled by space-bunny-free, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Reads the installed DirectX version: first through dsetup.dll's
// DirectXSetupGetVersion, then, if that fails, through
// HKLM\Software\Microsoft\DirectX (the "InstalledVersion" DWORD on NT, the
// "Version" string on Win9x), and compares the result with the wanted version.
//
// deepseek-v4.1-flash retry: best is 55.4% (637 bytes) with the failure exits
// spelled memset(version, 0, 16); return 0;. The RegOpenKeyExA failure path
// then compiles to the original's shared tail at 0x4b52ba exactly (xor edx,edx
// / pop edi / mov [esp+0x24],edx / ... / ret 0x14); the RegQueryValueExA error
// path still emits its own inline four-dword zero block, so the function is
// 18 bytes long. A single `goto fail` with one memset at the end (610 bytes,
// 53.3%) makes both paths share one block but loses more elsewhere.
//
// The remaining, unsolved difference is register allocation. The original
// keeps ebx=majhi, ebp=majlo, edi=minhi, esi=minlo and spills the library
// handle to [esp+0x20]; here lib takes ebx and minlo spills to [esp+0x20].
// This is independent of the tail: it is present at 54.6% too. Every halves
// declaration order (24 permutations) and every extraction order tried leaves
// lib in ebx; status also lands at [esp+0x1c] instead of the original's
// [esp+0x14]. Per the brief, these two are probably one shared allocator
// state caused by a source shape not yet found.
//
// Also unexplained: the 4-byte zero at version[4] (0x4b510a).
//
// deepseek-v4.1-flash re-verified every knob that could steer the allocator and
// all of them compile to the identical 55.4%/637 bytes: all 24 orders of the
// four halves as separate initialized declarations (55.4 or 54.4), int/DWORD/
// unsigned/DWORD-array types, a combined struct (worse), declaration of lib
// before or after the halves, status declared or initialized first, a chained
// `minlo = minhi = majhi = majlo = 0;` (54.4), `unsigned short` halves (52.0,
// 636 bytes), a function-scope FARPROC, `if (lib != 0)`/`if (proc != 0)`,
// `DWORD size` in each branch scope (54.9), the version buffer declared before
// type (54.9), an early-return RegOpenKeyExA form (44.9) and two `goto fail`
// forms with one shared memset (53.3, 610 bytes). The 24-perm sweep changing
// only the init order moves the score between two fixed points, so the four
// registers, the lib spill and the status slot are one allocator decision that
// no source shape tried here can flip.
//
// Two things in the original look like Cavedog's own bugs, kept here as they
// are: the "installed version is older" arm at 0x4b5233 compares the major
// half against argument 2 (the minor half) instead of against argument 1, and
// the Win9x query passes a 30-byte size for a buffer the frame only has room
// for from 0x28 to 0x45. The status slot at [esp+0x14] is read after
// FreeLibrary although it is only written when DirectXSetupGetVersion is
// called, so the failed LoadLibraryA/GetProcAddress paths test an
// uninitialized value; status is left uninitialized here to preserve that.
#include <windows.h>
#include <string.h>
#include <stdlib.h>

typedef int (__stdcall *FN_DIRECTXSETUPGETVERSION)(DWORD* major, DWORD* minor);

// FUNCTION: 0x4b5070
int __stdcall FUN_004b5070(int want0, int want1, int want2, int want3, int want4)
{
    int isNT = 0;
    unsigned int minlo = 0, minhi = 0, majhi = 0, majlo = 0;
    DWORD status;
    HMODULE lib;

    lib = LoadLibraryA("dsetup.dll");
    isNT = 0;
    if (lib) {
        FARPROC proc = GetProcAddress(lib, "DirectXSetupGetVersion");
        if (proc) {
            DWORD dwMaj = 0;
            DWORD dwMin = 0;

            status = ((FN_DIRECTXSETUPGETVERSION)proc)(&dwMaj, &dwMin);
            if (status) {
                minhi = dwMin >> 16;
                majlo = dwMaj & 0xffff;
                minlo = dwMin & 0xffff;
                majhi = dwMaj >> 16;
            }
        }
        FreeLibrary(lib);
    }
    if (!status) {
        HKEY hKey = 0;
        OSVERSIONINFOA osvi;
        DWORD type;
        DWORD size;
        char version[30];

        isNT = 0;
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        if (GetVersionExA(&osvi)) {
            isNT = osvi.dwPlatformId == 2;
        }
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectX", 0, KEY_READ, &hKey) == 0) {
            LONG err;

            status = 0;
            if (isNT) {
                size = 4;
                err = RegQueryValueExA(hKey, "InstalledVersion", 0, &type, (LPBYTE)&status, &size);
            } else {
                size = 30;
                err = RegQueryValueExA(hKey, "Version", 0, &type, (LPBYTE)version, &size);
            }
            RegCloseKey(hKey);
            if (err) {
                memset(version, 0, 16);
                return 0;
            }
            if (isNT) {
                majlo = status & 0xff;
            } else {
                majhi = atoi(strtok(version, "."));
                majlo = atoi(strtok(0, "."));
                minhi = atoi(strtok(0, "."));
                minlo = atoi(strtok(0, "."));
            }
        } else {
            memset(version, 0, 16);
            return 0;
        }
    }
    if (isNT) {
        return majlo >= (unsigned int)want4;
    }
    if (majhi == (unsigned int)want0) {
        if (majlo == (unsigned int)want1) {
            if (minhi == (unsigned int)want2) {
                return minlo >= (unsigned int)want3;
            }
            return minhi >= (unsigned int)want2;
        }
        return majlo >= (unsigned int)want1;
    }
    return majhi >= (unsigned int)want1;
}
