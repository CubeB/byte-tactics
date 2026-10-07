// Decompiled by space-bunny-free, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Claude Fable 5.1, finished by DeepSeek V4.1 Flash. Names are provisional.
// Reads the installed DirectX version: first through dsetup.dll's
// DirectXSetupGetVersion, then, if that fails, through
// HKLM\Software\Microsoft\DirectX (the "InstalledVersion" DWORD on NT, the
// "Version" string on Win9x), and compares the result with the wanted version.
// The caller (0x4263b0) asks for 4.05.00.0155 (DirectX 5), or 3 on NT.
//
// One thing in the original still looks like Cavedog's own bug, kept as it is:
// the "installed major version differs" arm at 0x4b5233, reached when the major
// half is not the wanted one, compares the major half against argument 2 (the
// minor half) instead of against argument 1.
#include <windows.h>
#include <string.h>
#include <stdlib.h>

typedef int (__stdcall *FN_DIRECTXSETUPGETVERSION)(DWORD* major, DWORD* minor);

struct DXVersion {
    unsigned int majhi;
    unsigned int majlo;
    unsigned int minhi;
    unsigned int minlo;
    // memset zero: the four halves come out as copies of one zero register.
    DXVersion() { memset(this, 0, sizeof(*this)); }
};

// FUNCTION: 0x4b5070
int __stdcall CheckDirectXVersion(int want0, int want1, int want2, int want3, int want4)
{
    int isNT;
    DXVersion v;
    DWORD status;
    HMODULE lib;

    // status zeroed before LoadLibraryA and isNT after it: orders the two top stores.
    status = 0;
    lib = LoadLibraryA("dsetup.dll");
    isNT = 0;
    if (lib) {
        FARPROC proc = GetProcAddress(lib, "DirectXSetupGetVersion");
        if (proc) {
            DWORD dwMaj = 0;
            DWORD dwMin = 0;

            status = ((FN_DIRECTXSETUPGETVERSION)proc)(&dwMaj, &dwMin);
            if (status) {
                // This order fixes the register assignment of the four halves.
                v.majhi = HIWORD(dwMaj);
                v.minhi = HIWORD(dwMin);
                v.majlo = LOWORD(dwMaj);
                v.minlo = LOWORD(dwMin);
            }
        }
        FreeLibrary(lib);
    }
    if (!status) {
        OSVERSIONINFOA osvi;
        DWORD type;
        DWORD size;
        LONG err;
        char version[30];
        HKEY hKey;

        memset(&v, 0, sizeof(v));
        isNT = 0;
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        if (GetVersionExA(&osvi)) {
            isNT = osvi.dwPlatformId == 2;
        }
        hKey = 0;
        err = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectX", 0, KEY_READ, &hKey);
        if (err == 0) {
            // Separate from status: it takes status's dead frame slot.
            DWORD installed = 0;

            if (isNT) {
                size = 4;
                err = RegQueryValueExA(hKey, "InstalledVersion", 0, &type, (LPBYTE)&installed, &size);
            } else {
                DWORD len = sizeof(version);
                err = RegQueryValueExA(hKey, "Version", 0, &type, (LPBYTE)version, &len);
            }
            RegCloseKey(hKey);
            if (err) {
                goto fail;
            }
            if (isNT) {
                v.majlo = installed & 0xff;
            } else {
                v.majhi = atoi(strtok(version, "."));
                v.majlo = atoi(strtok(0, "."));
                v.minhi = atoi(strtok(0, "."));
                v.minlo = atoi(strtok(0, "."));
            }
        } else {
            goto fail;
        }
    }
    if (isNT) {
        return v.majlo >= (unsigned int)want4;
    }
    if (v.majhi == (unsigned int)want0) {
        if (v.majlo == (unsigned int)want1) {
            if (v.minhi == (unsigned int)want2) {
                return v.minlo >= (unsigned int)want3;
            }
            return v.minhi >= (unsigned int)want2;
        }
        return v.majlo >= (unsigned int)want1;
    }
    return v.majhi >= (unsigned int)want1;
fail:
    memset(&v, 0, sizeof(v));
    return 0;
}
