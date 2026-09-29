// Decompiled by space-bunny-free. Names are provisional.
// Reads the installed DirectX version: first through dsetup.dll's
// DirectXSetupGetVersion, then, if that fails, through
// HKLM\Software\Microsoft\DirectX (the "InstalledVersion" DWORD on NT, the
// "Version" string on Win9x), and compares the result with the wanted version.
//
// NOT MATCHING YET (53.7%, 606 of 619 bytes). What still differs, largest
// cause first:
//
// 1. THE FRAME IS 0xD0, THE ORIGINAL'S IS 0xCC, so every [esp+X] in the body
//    is 4 bytes high. The original's local area is exactly full: six dwords at
//    0x10..0x24, the 30-byte version buffer at 0x28 and OSVERSIONINFOA at 0x48
//    (0x94 bytes, ending exactly at 0xdb). It shares slots three ways
//    (dwMaj/hKey, dwMin/size30, lib/size4). This source needs seven dwords:
//    the two size variables cannot be folded onto dwMaj, dwMin and lib even
//    when they are block scoped in the !status block and hKey is moved into
//    it, which is what variant f does. Declaring one `DWORD size` for both
//    arms, or two, makes no difference to the generated code.
// 2. REGISTER ROLES. The original holds ebx=majhi, ebp=majlo, edi=minhi,
//    esi=minlo, and keeps isNT, err, lib and status in memory; here lib is
//    register allocated and the four values land in esi=ebx=edi=ebp in a
//    different order. All four orderings of the four assignments were scored
//    on top of variant f: the one used here (majhi, majlo, minlo, minhi) is
//    the best at 53.7%; (majhi, minhi, majlo, minlo) gives 34.9% and
//    (majhi, majlo, minhi, minlo) 33.9%.
// 3. THE ZEROING OF THE version BUFFER ON THE TWO FAILURE EXITS. Both
//    failures (0x4b52ba) store 0 to [esp+0x28]..[esp+0x34] from one zeroed
//    register, interleaved with the pops, which is exactly how MSVC 5 expands
//    memset(buf, 0, 16) (one zeroed register, four dword stores). The same
//    block is also entered from the RegQueryValueExA failure, so the two exits
//    share it. Duplicating `memset(version, 0, 16); return 0;` into both arms
//    of the source scores 51.0%, so the spelling is close but not right, and
//    the 4-byte zero of [esp+0x2c] at 0x4b510a (version[4]) is unexplained.
// 4. Small shapes: the original tests the LoadLibrary result with `test eax,
//    eax` and only then stores it, and compares call results against a zero
//    register (`cmp eax, ebp`, `cmp eax, ecx`) where this source sometimes
//    emits `test eax, eax`.
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
    unsigned int majhi;
    unsigned int majlo;
    unsigned int minhi;
    unsigned int minlo;
    DWORD status = 0;
    int isNT;
    HMODULE lib;
    FARPROC proc;
    DWORD dwMaj;
    DWORD dwMin;
    OSVERSIONINFOA osvi;
    LONG err;
    DWORD type;
    char version[30];

    majhi = majlo = minhi = minlo = 0;
    lib = LoadLibraryA("dsetup.dll");
    if (lib) {
        proc = GetProcAddress(lib, "DirectXSetupGetVersion");
        if (proc) {
            dwMaj = 0;
            dwMin = 0;
            status = (DWORD)((FN_DIRECTXSETUPGETVERSION)proc)(&dwMaj, &dwMin);
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
        HKEY hKey;
        DWORD size4 = 4;
        DWORD size30 = 30;
        majhi = minhi = minlo = 0;
        isNT = 0;
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
        if (GetVersionExA(&osvi)) {
            isNT = osvi.dwPlatformId == 2;
        }
        hKey = 0;
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectX", 0, KEY_READ, &hKey) == 0) {
            status = 0;
            if (isNT) {
                err = RegQueryValueExA(hKey, "InstalledVersion", 0, &type, (LPBYTE)&status, &size4);
            } else {
                err = RegQueryValueExA(hKey, "Version", 0, &type, (LPBYTE)version, &size30);
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
