// Decompiled by space-bunny-free, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// Reads the installed DirectX version: first through dsetup.dll's
// DirectXSetupGetVersion, then, if that fails, through
// HKLM\Software\Microsoft\DirectX (the "InstalledVersion" DWORD on NT, the
// "Version" string on Win9x), and compares the result with the wanted version.
//
// Found this pass, all measured with tools/scratch/0x4b5070 (see the notes
// there for the sweeps):
//  * The two failure exits share ONE memset tail, so they must be reached with
//    `goto fail`, not with two inline `memset(version, 0, 16); return 0;`.
//    Two inline tails cost 31 extra bytes (637) because the 12-instruction
//    epilogue is emitted twice; one shared tail is 613.
//  * The four halves are re-zeroed at the top of the registry block. Without
//    that, the earlier version's version was 18 bytes long; the source needs
//    `majhi = majlo = minhi = minlo = 0;` there (chained, one zero register).
//  * `status = 0;` must come BEFORE LoadLibraryA, or MSVC sinks the store and
//    the `mov [esp+0x14], esi` that precedes the call disappears.
//  * `HKEY hKey` must be declared at FUNCTION scope. In the block it shares
//    dwMaj's slot and the frame is 0xc8; at function scope it gets its own
//    slot, the frame becomes the original's 0xcc, version lands at 0x28 and
//    osvi at 0x48. That single declaration was worth 5 bytes and every
//    [esp+N] operand in the function.
//  * The extraction order majhi, majlo, minlo, minhi is the best of the 24
//    (others: 55-64%).
//
// Still open, in order of size:
//  * dwMin is at [esp+0x24] here and at [esp+0x1c] in the original; `type`
//    shares lib's slot [esp+0x20] here and has [esp+0x24] there. The original's
//    six own slots, read off MSVC's /FA listing, are dwMaj 0x10, status 0x14,
//    isNT 0x18, dwMin 0x1c, lib 0x20, type 0x24, then version 0x28 and osvi
//    0x48; here they are dwMaj 0x10, status 0x14, isNT 0x18, hKey 0x1c, lib
//    0x20, dwMin 0x24. Measured and all worse (see "measured and rejected"):
//    the only shape that gives hKey no slot of its own puts it at 0x0c and
//    steals dwMin's.
//  * The register map is rotated one step: here majhi/majlo/minhi/minlo land in
//    esi/ebx/ebp/edi, the original has them in ebx/ebp/edi/esi. Same rotation
//    as the slot order. It answers to the extraction order, but only ever
//    produces two mappings (see "measured and rejected"): the six orders that
//    keep all four halves in registers give esi/ebx/ebp/edi, and none of the
//    24 gives ebx/ebp/edi/esi.
//  * The top of the function zeroes with `xor ebp,ebp / xor ebx,ebx / xor
//    edi,edi`; the original uses `mov ebp,esi / mov ebx,esi / mov edi,esi` off
//    the zero already in esi. Same byte count, different opcode: MSVC propagates
//    esi's zero into the other three there but not here.
//  * Because ebp is not known zero here, the three `push 0` for lpReserved cost
//    2 bytes each where the original has `push ebp`.
//  * The extraction is 10 instructions here and 9 in the original, which copies
//    dwMaj and dwMin into ebx and edi first and then shifts; only the six
//    orders above reach that shape and they cost 625 bytes.
//  * The re-zeroing at the top of the registry block is emitted three times
//    here (edi, ebp, esi); the original emits six stores. MSVC folds the other
//    three because it still knows those registers hold the zero from the top of
//    the function; in the original it does not.
//  * Also unexplained: the original's `mov [esp+0x30], esi` at 0x4b510a, a
//    dword store into version[4] at the top of the registry block.
//
// Measured and rejected this pass, all worse than the 63.6% in the file
// (compile-only ranking in build/scratch/0x4b5070):
//  * 120 permutations of the declaration order of isNT/status/lib/version and
//    the four halves: all compile to byte-identical code. Declaration order is
//    not a lever here, which is worth recording.
//  * 24 permutations of the declaration order of the four halves x 3 re-zero
//    spellings x 4 spellings of how size/type are declared: 384 variants, the
//    frame is 0xcc only when hKey is at function scope, otherwise 0xc8.
//  * The 24 extraction orders, twice, on two different bases: best 613 bytes
//    / 64.6%, worst 624 bytes / 32.9%.
//  * Two inline `memset(version, 0, 16); return 0;` tails instead of the shared
//    one: 637 bytes / 55.4%, the version this file replaces.
//  * `type` at function scope with hKey in the registry block: 0xc8 frame,
//    dwMin and hKey share a slot at 0x0c, every offset from dwMin up is 4 low.
//  * dwMaj, dwMin and type all at function scope: 0xd0 frame, every offset
//    wrong.
//  * dwMaj at function scope as the key handle (`(HKEY *)&dwMaj`), with and
//    without `type` at function scope: 0xc8 frame, dwMin shares size's slot.
//  * Re-zeroing spelled `minlo = majhi = minhi = isNT = 0; majlo = 0;` or with
//    isNT in the chain, and one zero statement per variable: 617-622 bytes.
//  * proc, err, size or type moved one scope in or out: 610-618 bytes, frame
//    0xc8 unless hKey is at function scope.
//  * An explicit `isNT = 0;` at the top of the registry block: 621 bytes and
//    59.4% (checked, not just ranked). It does bring the instruction count to
//    the original's 197, but MSVC emits the store as an 8-byte immediate where
//    the original has a 4-byte register store, and that costs more than the
//    extra store gains. A good reminder that this checker's score is byte
//    similarity, so an instruction-sequence ratio is only a filter.
//
// Two things in the original look like Cavedog's own bugs, kept here as they
// are: the "installed version is older" arm at 0x4b5233 compares the major half
// against argument 2 (the minor half) instead of against argument 1, and the
// Win9x query passes a 30-byte size for a buffer the frame only has room for
// from 0x28 to 0x45.
#include <windows.h>
#include <string.h>
#include <stdlib.h>

typedef int (__stdcall *FN_DIRECTXSETUPGETVERSION)(DWORD* major, DWORD* minor);

// FUNCTION: 0x4b5070
int __stdcall FUN_004b5070(int want0, int want1, int want2, int want3, int want4)
{
    int isNT = 0;
    unsigned int minhi = 0, majlo = 0, minlo = 0, majhi = 0;
    DWORD status;
    HMODULE lib;
    HKEY hKey;
    char version[30];

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
                majhi = dwMaj >> 16;
                majlo = dwMaj & 0xffff;
                minlo = dwMin & 0xffff;
                minhi = dwMin >> 16;
            }
        }
        FreeLibrary(lib);
    }
    if (!status) {
        OSVERSIONINFOA osvi;
        DWORD type;
        DWORD size;
        LONG err;

        majhi = majlo = minhi = minlo = 0;
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        if (GetVersionExA(&osvi)) {
            isNT = osvi.dwPlatformId == 2;
        }
        hKey = 0;
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectX", 0, KEY_READ, &hKey) == 0) {
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
                goto fail;
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
            goto fail;
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
fail:
    memset(version, 0, 16);
    return 0;
}
