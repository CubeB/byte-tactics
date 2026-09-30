// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol. Names are provisional.
// Reads the installed DirectX version: first through dsetup.dll's
// DirectXSetupGetVersion, then, if that fails, through
// HKLM\Software\Microsoft\DirectX (the "InstalledVersion" DWORD on NT, the
// "Version" string on Win9x), and compares the result with the wanted version.
//
// PARTIAL (48.6%, 599 of 619 bytes). The frame is now the original 0xcc
// (one DWORD size variable shared by both RegQueryValueExA arms, rather than
// separate size4/size30 which grew the frame to 0xd0). What still differs:
//
// 1. REGISTER ROLES. The original keeps all four version halves in
//    ebx=majhi, ebp=majlo, edi=minhi, esi=minlo, so lib has nowhere to live
//    and spills to [esp+0x20]; here lib takes ebp and the allocator spills
//    two of the four halves to [esp+0x1c]/[esp+0x20] instead. Reordering the
//    four shift/and assignments (all permutations tried) compiles
//    byte-identically and does not change this.
// Reversing the declaration order of the version halves raises the score to
// 48.6%; using unsigned short locals instead drops it to 47.2%.
// 2. status is held in a register in places where the original loads
//    [esp+0x14] (for example `if (!status)` becomes `cmp eax,ebp` here versus
//    `test eax,eax` on a reloaded value), and lib's load/store slots land one
//    dword off in spots.
// 3. The original's two failure exits share a tail that zeroes version[0..15]
//    with one zeroed register (four stores interleaved with the pops);
//    spelling that as memset(version,0,16); return 0; in both arms does not
//    reproduce it.
// 4. The 4-byte zero store at 0x4b510a (original [esp+0x2c], version[4]) is
//    unexplained.
// The original also reads the status slot at [esp+0x14] after FreeLibrary.
// That slot is written only when DirectXSetupGetVersion is called, so failed
// LoadLibraryA/GetProcAddress paths test an uninitialized value. Keep status
// uninitialized here to preserve the observed source behavior.
//
// Two things in the original look like Cavedog's own bugs, kept here as they
// are: the "installed version is older" arm at 0x4b5233 compares the major
// half against argument 2 (the minor half) instead of against argument 1, and
// the Win9x query passes a 30-byte size for a buffer the frame only has room
// for from 0x28 to 0x45.
#include <windows.h>
#include <string.h>
#include <stdlib.h>

typedef int (__stdcall *FN_DIRECTXSETUPGETVERSION)(DWORD* major, DWORD* minor);

// FUNCTION: 0x4b5070
int __stdcall FUN_004b5070(int want0, int want1, int want2, int want3, int want4)
{
    int isNT = 0;
    unsigned int minlo = 0, minhi = 0, majlo = 0, majhi = 0;
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
                majhi = dwMaj >> 16;
                majlo = dwMaj & 0xffff;
                minlo = dwMin & 0xffff;
                minhi = dwMin >> 16;
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
            return 0;
        }
    }
    if (isNT) {
        return majlo >= (unsigned int)want4;
    }
    if (majhi != (unsigned int)want0) {
        return majhi >= (unsigned int)want1;
    }
    if (majlo != (unsigned int)want1) {
        return majlo >= (unsigned int)want1;
    }
    if (minhi != (unsigned int)want2) {
        return minhi >= (unsigned int)want2;
    }
    return minlo >= (unsigned int)want3;
}
