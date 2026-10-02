// Decompiled by space-bunny-free, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Claude Fable 5.1. Names are provisional.
// Reads the installed DirectX version: first through dsetup.dll's
// DirectXSetupGetVersion, then, if that fails, through
// HKLM\Software\Microsoft\DirectX (the "InstalledVersion" DWORD on NT, the
// "Version" string on Win9x), and compares the result with the wanted version.
// The caller (0x4263b0) asks for 4.05.00.0155 (DirectX 5), or 3 on NT.
//
// Claude Fable 5.1: 63.6% -> 94.9% at the original's 619 bytes. The shape that
// did it, in the order it was found:
//  * The four version halves are the fields of one small struct that is zeroed
//    with memset (here through its constructor) and promoted to registers. A
//    memset's zero is stored from a register and the field reads are forwarded
//    from those stores, so the halves come out as copies of one zero register
//    (`xor esi,esi; mov ebx,esi; mov ebp,esi; mov edi,esi`). Any chain of
//    `= 0` assignments, a reference helper, the C front-end, or an aggregate
//    initialiser materialises each register with its own xor instead.
//  * Zeroing the struct again at the top of the registry block with a plain
//    memset (not a temporary `v = DXVersion()`, which adds a `mov eax,esi`
//    detour) gives the original's `mov [esp+0x30],esi`: the dead store of the
//    majlo field, which both arms reassign, lands at the struct's home +4. The
//    struct's home shares its slot with the registry block's `version` buffer,
//    exactly as in the original (both at L0x18, frame 0xcc).
//  * `isNT = 0` after that memset then stores from the same zero register
//    without turning isNT into a register variable (it did when the zeroing was
//    a chain or a temporary).
//  * HIWORD/LOWORD in the order majhi, minhi, majlo, minlo fixes the dsetup
//    extraction and the register assignment ebx/ebp/edi/esi.
//  * Frame slots: MSVC 5 shares slots by liveness and orders them by reference
//    count (most referenced lowest). hKey block-scoped shares dwMaj's slot;
//    one `size` at registry-block level (used by the NT arm) shares lib's; a
//    second length local in the Win9x arm shares dwMin's; `type` gets its own.
//  * `err = RegOpenKeyExA(...); if (err == 0)` gives the `cmp eax,ebp` test.
// Still differing: status and isNT have their slots swapped (ours isNT at
// [esp+0x14] and status at [esp+0x18], the original the reverse). Both have six
// references, so the tie-break is the open question; declaration order, types,
// names, the position of the top-level `isNT = 0`, a dead initialiser, and a
// folded extra test of status all leave it unchanged, and a real extra store
// to status flips it but costs 6 bytes (t5 in the scratch notes). The permuter
// then found that zeroing isNT before LoadLibraryA and status after it makes
// the two top stores byte-identical (the aliased status store stays after the
// call, the isNT store sinks to the same place), 93.9% to 94.9%; the twelve
// remaining lines are the other references to the two swapped slots. The
// original most likely has `status = 0` before the call with the slots the
// other way round, so whoever flips the slots should restore that order.
//
// Two things in the original look like Cavedog's own bugs, kept here as they
// are: the "installed major version differs" arm at 0x4b5233 compares the major
// half against argument 2 (the minor half) instead of against argument 1, and
// the NT path reads the InstalledVersion value into the status variable.
#include <windows.h>
#include <string.h>
#include <stdlib.h>

typedef int (__stdcall *FN_DIRECTXSETUPGETVERSION)(DWORD* major, DWORD* minor);

struct DXVersion {
    unsigned int majhi;
    unsigned int majlo;
    unsigned int minhi;
    unsigned int minlo;
    DXVersion() { memset(this, 0, sizeof(*this)); }
};

// FUNCTION: 0x4b5070
int __stdcall FUN_004b5070(int want0, int want1, int want2, int want3, int want4)
{
    int isNT = 0;
    DXVersion v;
    DWORD status;
    HMODULE lib;

    isNT = 0;
    lib = LoadLibraryA("dsetup.dll");
    status = 0;
    if (lib) {
        FARPROC proc = GetProcAddress(lib, "DirectXSetupGetVersion");
        if (proc) {
            DWORD dwMaj = 0;
            DWORD dwMin = 0;

            status = ((FN_DIRECTXSETUPGETVERSION)proc)(&dwMaj, &dwMin);
            if (status) {
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
            status = 0;
            if (isNT) {
                size = 4;
                err = RegQueryValueExA(hKey, "InstalledVersion", 0, &type, (LPBYTE)&status, &size);
            } else {
                DWORD len = sizeof(version);
                err = RegQueryValueExA(hKey, "Version", 0, &type, (LPBYTE)version, &len);
            }
            RegCloseKey(hKey);
            if (err) {
                goto fail;
            }
            if (isNT) {
                v.majlo = status & 0xff;
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
