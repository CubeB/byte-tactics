// Decompiled by Space Bunny Free. Names are provisional.
// Opens (or creates) HKCU\Software\Cavedog Entertainment\<subKey> and then
// either reads or writes one REG_DWORD / string / binary value in it.
// The decorated name is ?FUN_004b6880@@YGHPAD0PAEPAKKK@Z, so six stack
// arguments, ret 0x18.
//
// Retry (deepseek-v4.1-flash, issue 2435): re-confirmed 88.2%, 299 vs 297
// bytes. Only the 2-byte prologue differs: the original loads the 6th param
// straight into ebp (`mov ebp,[esp+0x30]; mov esi,ebp`) for the mask/doRead,
// ours hoists the load into eax and adds `mov ebp,eax`. doRead-first shapes
// make the mask read ebp but still load via eax (298 bytes, 82.4). A dummy
// sweep 0..64, headers.py (128 C plus 768 --cpp sets) and all order/type/mask
// spellings are flat, so it is a compiler-state tie.
//
// What this file reproduces:
// - The goto structure. All the failure exits jump to one `close:` label, and
//   the two success stores are written out in their own arms, which is what
//   makes MSVC duplicate `mov edi, 1` and put a `jmp` after each. With one
//   shared tail the store is emitted once and the function loses 4 bytes.
// - The frame holds four dwords, zeroed in the order key1, key2, key3, result,
//   which needs the three HKEYs declared with `= 0` and `result` zeroed by a
//   statement after them.
// - The result lives in edi: it is assigned 1 on both success paths and read
//   back from its stack slot on the failure path, then returned with
//   `mov eax, edi`.
// - `samDesired` is read ? KEY_READ : KEY_WRITE (`neg/sbb/and/add` in esi),
//   the flag copy is in ebp, and ebx holds the constant 0, so every comparison
//   is `cmp eax, ebx` and every zero argument is `push ebx`.
// - Every call result is stored in a LONG before it is compared, which is what
//   gives `cmp eax, ebx` rather than `test eax, eax`.
// - The read path accepts ERROR_SUCCESS and ERROR_MORE_DATA (0xea).
// - The ninth argument of all three RegCreateKeyExA calls is the address of
//   the sixth parameter, not of a local. Passing 0 instead loses the frame slot
//   and 12 bytes, so keep it.
//
// Still different (86.0% to 88.2% left on the table):
// - The prologue. The original loads the flag straight into ebp between the
//   two pushes and copies it into esi (`mov ebp, [esp+0x30]; push esi; mov esi,
//   ebp`); here MSVC hoists the load above `push ebx` into a scratch register
//   and copies it into esi early and ebp late, which costs 2 bytes. The
//   address of that same parameter is taken (passed as lpdwDisposition), and
//   that address-taken use is what stops MSVC folding the copy into a load:
//   every shape tried (flag declared first, samDesired computed from the local,
//   BOOL/unsigned/long flag, a separate `LPDWORD pRead`, passing 0) either
//   keeps the hoist or loses the frame.
//
// A later pass (Claude Sonnet 5.5, #566) scored about 30 more shapes with
// check.py --sym, all worse or equal, so do not repeat them:
// - every combination of order (`doRead` before or after `samDesired`), source of
//   the mask (`doRead` or `read`), type of the flag (`int`, `DWORD`,
//   `unsigned char`) and declaration (initialised or assigned later): 18 files.
//   `doRead` first with the mask taken from it gives 298 bytes (one over, 82.4
//   percent) but the load still goes into eax above `push ebx` and is copied to
//   ebp; `unsigned char` gives 300 to 301 bytes;
// - probes meant to stop MSVC unifying the two reads of `read` (the 0x4bfd60
//   trick): `*(int*)&read`, `read + 0`, `(doRead != 0)`, `doRead = doRead`,
//   reading the mask through `*(int*)&read`; an inline helper
//   `REGSAM Access(DWORD)` called with `doRead` or `read`, before or after the
//   flag. All 298 or 299 bytes, none moved the load.
// The difference is one fact: the original's load of `read` goes straight into
// ebp after `push ebp` (the flag's own register, so the mask is computed from a
// copy, `mov esi,ebp; neg esi`), while ours loads a temporary into eax at the
// top and copies it. Everything else (frame, zeroing order, exits, the doubled
// success stores) matches, and the 2-byte size difference is only this.
//
// Retry (Sonnet 5.5, #1107), no gain, 1 official run: /Gr, /Gz and /Gd give the
// same 88.2% (the function is already __stdcall and the callees are imports).
// A 3360-file scripted sweep of every declaration/statement order of result,
// key1..3, sam, doRead, err and the zeroing (key order fixed) topped out at
// 88.2% (the current shape, sam before doRead); every order with doRead first
// gives 298 bytes and 82.4%, where the load goes to eax, is copied to ebp, and
// the mask is negated in eax then moved to esi. Also flat or worse: if/else and
// KEY_WRITE + 0x13 forms of the mask, sam re-assigned from itself, the mask
// spelled in every call, and inline helpers taking DWORD, DWORD& or DWORD*.
// The original loads read straight into ebp (its own register) and copies it to
// esi for the neg; MSVC here always folds the two into one temporary.
// Retry (GPT-6.1-sol, #1164): 11 check.py invocations across both passes,
// counting the first pre-rebase invocation whose output was not captured. No
// gain from no-op self-assignment to key1, assigning read to result before its
// zero initialization, `register` on doRead, `read = read`, or copying doRead
// back to read. A bool doRead drops to 32.3%. Reversing the ternary condition
// is byte-identical to the baseline. The residual remains the prologue arg6
// load: original loads directly into ebp after push ebp; MSVC hoists it into
// eax and copies it to esi and ebp.
#include <windows.h>

// FUNCTION: 0x4b6880
int __stdcall FUN_004b6880(char* subKey, char* valueName, LPBYTE data, LPDWORD size,
                           DWORD type, DWORD read)
{
    int result;
    HKEY key1 = 0;
    HKEY key2 = 0;
    HKEY key3 = 0;
    REGSAM samDesired = read ? KEY_READ : KEY_WRITE;
    int doRead = read;
    LONG err;
    result = 0;
    err = RegCreateKeyExA(HKEY_CURRENT_USER, "Software", 0, 0, 0, samDesired, 0,
                          &key1, &read);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key1, "Cavedog Entertainment", 0, 0, 0, samDesired, 0,
                          &key2, &read);
    if (err != 0)
        goto close;
    err = RegCreateKeyExA(key2, subKey, 0, 0, 0, samDesired, 0, &key3, &read);
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
