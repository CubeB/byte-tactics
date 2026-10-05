// Decompiled by space-bunny-free, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by Claude Fable 5.1, finished by DeepSeek V4.1 Flash. Names are provisional.
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
// The permuter then found that zeroing isNT before LoadLibraryA and status after
// it makes the two top stores byte-identical (the aliased status store stays
// after the call, the isNT store sinks to the same place), 93.9% to 94.9%; the
// twelve remaining lines were the other references to the two swapped slots.
//
// DeepSeek V4.1 Flash (issue retry): the permuter with --stack status,isNT ran
// 3 min (2457 candidates) and a second 3 min run seeded from the flipped
// 625-byte shape both came back to this same 94.9/619 optimum. headers.py over
// 256 header sets is flat at 94.9. The flip itself is easy to reach (a
// declaration initialiser on status, or any second status store, puts status
// at -0xc8 and isNT at -0xc4 with everything after the prologue matching) but
// it always costs 6 bytes, and every source route to it without the extra
// store is inert: inlined helpers adding a use of status or isNT, name swaps,
// declaration reordering, type changes, folded extra tests, and self
// assignments all compile to the same 619 bytes with the same wrong slots. So
// the residual looked like the allocator tie-break between two six-reference
// locals.
//
// Space Bunny Free: MATCH, 94.9% -> 100%. The tie-break was never the problem:
// there are two locals, not one. The original does not read the registry's
// InstalledVersion DWORD back into the variable that holds DirectXSetupGetVersion's
// return code; it reads it into a separate block-scoped local, and that local
// takes the dead `status` slot. Each of the two then has three references, so
// the pair shares [esp+0x14] (L-0xc8) exactly as the original does, isNT keeps
// [esp+0x18] (L-0xc4), and every slot in the frame lands where the original has
// it. Reading the value into `status` instead is what made one six-reference
// local compete with the six-reference isNT and put them in each other's slots.
// With the split, the two top zero stores also order the original's way round:
// `status = 0` before LoadLibraryA (store to [esp+0x14]) and `isNT = 0` after it
// (store to [esp+0x18]).
// What did not help, in case it is tried again: swapping those two statements on
// their own (93.9%, the slots stay wrong), the dummy-extern sweep over 0 to 400,
// unused locals in the function body, `register`, an extra scope, folded extra
// mentions of either name, and every declaration order and type permutation.
// General lesson for a swapped pair of equal-count locals: before hunting the
// tie-break, check whether the original merged or split the values. A local that
// is dead where another begins is free to take its slot, and that changes both
// the counts and the order.
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
    DXVersion() { memset(this, 0, sizeof(*this)); }
};

// FUNCTION: 0x4b5070
int __stdcall FUN_004b5070(int want0, int want1, int want2, int want3, int want4)
{
    int isNT;
    DXVersion v;
    DWORD status;
    HMODULE lib;

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
