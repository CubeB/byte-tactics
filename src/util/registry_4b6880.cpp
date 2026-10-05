// Decompiled by Space Bunny Free, finished by GPT-6.1-sol and mimo-v2.6-pro,
// finished by Space Bunny Free. Names are provisional.
//
// Space Bunny Free pass: MATCH, 297 of 297 bytes, all six linker references ok,
// from 88.2% (299 bytes). One construct closes the whole residual, and it is
// the thing eight earlier passes all treated as fixed: THE NINTH ARGUMENT OF THE
// THREE RegCreateKeyExA CALLS IS NOT `&read`, IT IS THE ADDRESS OF A LOCAL.
//
// The residual was the prologue: the original loads the sixth parameter straight
// into ebp between `push ebp` and `push esi` and copies it on to esi, while we
// hoisted the load into eax and copied out twice, once to esi and once to ebp.
// Taking the address of the sixth parameter is what stops MSVC 5 promoting it:
// an address-taken parameter has an indirect home and is reloaded after every
// call, so `int doRead = read;` has to be a register copy, and the allocator
// then splits the parameter's web into an early one (eax) and a late one (ebp).
//
// The sibling that decided it is 0x4b4cf0 (HapiBank::WriteBox, MATCH,
// 115 bytes), found by scanning the exe for `mov ebp,[esp+d]` followed by
// `push esi ; mov esi,ebp`: there `len` is a parameter whose address is never
// taken, so MSVC promotes it into ebp and copies it to esi for `len + c->size`,
// which is exactly our prologue shape. The only difference was that our
// parameter's address was taken.
//
// So the ninth argument is a local, `DWORD disp`, and the reason it still emits
// `lea eax,[esp+0x34]` is the lever docs/agent-guide.md records under "locals in
// parameter slots": `read`'s stack slot is dead after the two reads at the top,
// so MSVC 5 reuses it for `disp`, and E+0x18 is both `&read` and `&disp`. The
// generated code is unchanged, the frame is unchanged (still `sub esp,0x10` with
// key1, key2, key3 and result), and the prologue is byte for byte the original's.
//
// Measured while finding it, all compile-only, so none of it cost a check run:
//   * `&read` in one call only (0 in the other two) is still 291 bytes and still
//     hoists into eax: one address-taking call is enough to block the promotion,
//     so it is the address, not the number of calls, that decides;
//   * `&disp` with `disp` declared first, last or between the flag and `err` all
//     give 297 bytes and MATCH, so the slot reuse is not order sensitive;
//   * `DWORD disp = 0;` is 301 bytes: the zeroing store is a real instruction,
//     so `disp` must be left uninitialised (its value is never read here, only
//     written by RegCreateKeyExA);
//   * with `&read` restored and the dead `disp` kept (301 -> 299 bytes, eax),
//     which confirms the ninth argument is the whole lever.
//
// Harness for this pass, in build/scratch/0x4b6880/ (all scratch only):
//   * asmlist.py compiles a variant with tools/wcl and reads the generated code
//     straight out of the object with objdump, printing the exact size and the
//     prologue, about 4 s a variant, which is what made a 200-variant sweep
//     affordable. The size comes from the last instruction's address, which is
//     why the nop padding after the function does not count.
//   * variants.py .. variants6.py write the sweep grids (flag and mask spelling,
//     declaration position, err and result types, helper forms, union and array
//     flags, and the address-taking variants above).
//   * sweep.sh prints the distinct prologues of a whole grid at once and counts
//     how many hit `mov 0x30(%esp),%ebp`, the original's shape.
//   * scan.py and va.py look for other functions in the exe with this prologue
//     family; the full nine-byte pattern occurs exactly once, here, but the
//     six-byte `mov ebp,[esp+d] ; push esi ; mov esi,ebp` occurs twice and
//     0x4b4cf0 is the other one.
//   * sw.py scores a set of variants with check.py in parallel.
//
// What earlier passes measured, now all explained by the ninth argument: every
// shape of the flag copy, the mask, the declaration order and the file-scope
// noise kept `&read`, so they all kept the parameter unpromotable. doRead first
// (298 bytes), the mask from doRead (298), any type for the flag, `volatile`-free
// self assignments, inline helpers taking DWORD, DWORD& or DWORD*, a pointer to
// the flag local, `(read | doRead)` and `(read & doRead)`, arithmetic masks,
// a dummy-declaration sweep to 2200, headers.py and /Gr /Gz /Gd were all flat,
// and a 15 minute permuter run over 2197 candidates found nothing. None of them
// could have worked: with `&read` the load has to go through eax.
//
// Historical notes from the passes that got here, kept because the negatives are
// still true for a body that passes `&read`:
//
// The four frame dwords are zeroed in the order key1, key2, key3, result, which
// needs the three HKEYs declared with `= 0` and result zeroed by a statement
// after them. The result lives in edi: assigned 1 on both success paths and read
// back from its stack slot on the failure path, then returned with `mov eax,edi`.
// All the failure exits jump to one `close:` label and the two success stores are
// written out in their own arms, which is what makes MSVC duplicate `mov edi,1`
// and put a `jmp` after each; with one shared tail the store is emitted once and
// the function loses 4 bytes. `samDesired` is read ? KEY_READ : KEY_WRITE (the
// neg/sbb/and/add chain in esi), the flag copy is in ebp, and ebx holds the
// constant 0, so every comparison is `cmp eax,ebx` and every zero argument is
// `push ebx`. Every call result is stored in a LONG before it is compared, which
// is what gives `cmp eax,ebx` rather than `test eax,eax`. The read path accepts
// ERROR_SUCCESS and ERROR_MORE_DATA (0xea).
#include <windows.h>

// FUNCTION: 0x4b6880
int __stdcall AccessRegistryValue(char* subKey, char* valueName, LPBYTE data, LPDWORD size,
                           DWORD type, DWORD read)
{
    int result;
    HKEY key1 = 0;
    HKEY key2 = 0;
    HKEY key3 = 0;
    REGSAM samDesired = read ? KEY_READ : KEY_WRITE;
    int doRead = read;
    // lpdwDisposition for the three calls. Left uninitialised on purpose: the
    // key is never read back, and initialising it costs a store (301 bytes).
    DWORD disp;
    LONG err;
    result = 0;
    err = RegCreateKeyExA(HKEY_CURRENT_USER, "Software", 0, 0, 0, samDesired, 0,
                          &key1, &disp);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key1, "Cavedog Entertainment", 0, 0, 0, samDesired, 0,
                          &key2, &disp);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key2, subKey, 0, 0, 0, samDesired, 0, &key3, &disp);
    if (err != 0)
        goto close;
    if (doRead) {
        err = RegQueryValueExA(key3, valueName, 0, 0, data, size);
        if (err != 0 && err != ERROR_MORE_DATA)
            goto close;
    } else {
        err = RegSetValueExA(key3, valueName, 0, type, data, *size);
        if (err != 0)
            goto close;
    }
    result = 1;
close:
    if (key3 != 0) {
        RegCloseKey(key3);
    }
    if (key2 != 0) {
        RegCloseKey(key2);
    }
    if (key1 != 0) {
        RegCloseKey(key1);
    }
    return result;
}