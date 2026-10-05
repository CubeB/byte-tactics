// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
//
// MATCH (check.py, 4 real runs; every candidate variant was first scored free
// with `check.py --sym`).
//
// WHAT THE LAST 1.8% WAS, AND WHAT FIXED IT
// Six earlier passes got 98.2% and the one difference left was a single
// basic-block BOUNDARY in the exit code: the original has the mci-failure
// `xor eax, eax` in its own two-byte block that only the failure `jne` enters,
// with the done-side `je` jumping PAST it into the shared epilogue, while ours
// had one shared block with the `xor` sunk between `pop esi` and `pop ebx`.
//
// The fix is the SHAPE OF THE FUNCTION, not the spelling of the return. Nestle
// the whole body, including the done tail, inside `if (hr == 0) { ... }` and let
// the mci failure be a trailing `return 0;` AFTER the closing brace:
//
//     int hr = mciSendStringA("status cdaudio number of tracks", buf, 0x20, 0);
//     if (hr == 0) {
//         field_200 = atoi(buf);
//         ...
//     done:
//         if (field_200 != 0)
//             field_208 = 1;
//         return field_200;
//     }
//     return 0;
//
// That gives the original's three return blocks: the `return field_200` keeps its
// own epilogue copy at the end of the body, the trailing `return 0` is the
// out-of-line `xor eax, eax` block the failure `jne` reaches, and the done-side
// `je` lands on the epilogue both of them share. Written the other way round
// (`if (hr != 0) return 0;` first, with the zero return inside the body) MSVC 5
// sees two identical `return 0` sites, folds them into one block and the
// scheduler is free to sink the `xor` into the middle of the epilogue.
//
// Note that the tail is now a SINGLE return statement (`if (field_200 != 0)
// field_208 = 1; return field_200;`). A two-statement tail with a trailing
// `return 0;` inside the body merges the zero returns again, and the single
// return is what makes the value on the zero path already sit in eax.
//
// WHY THE EARLIER PASSES COULD NOT SEE IT: every one of them tried the return
// VALUE (constant 0, the field itself, a local, a goto, a helper, a label, an
// if/else, a loop region). The lever was the position of the `if` around the
// body, which none of them varied. The same idea is what the guide's 0x461db0
// and 0x461750 entries describe from the other side ("`if (!p) return 0;` does
// not", "an inline helper tested with `if (!Helper()) return 0;`"): when a
// failure exit cannot be written as an early `return`, write the SUCCESS as the
// `if` body and let the failure fall out of the end of the function.
//
// Two codegen details that are load bearing and should be kept:
//   * both mciSendStringA results are assigned to a local `int` before they are
//     tested. `mciSendStringA(...) != 0` compiles to `test eax, eax`, and only
//     the assigned form gives the original's `cmp eax, ebx`.
//   * `other` is written to the stack and never read (`mov [esp+0xc], eax`).
//     MSVC 5 only gives `other` a stack slot when it is live across a loop, so
//     the no-op loop below (which compiles to nothing) reproduces that dead store
//     and the 0x44-byte frame.
//
// TRIED THIS PASS AND STILL 98.2% or worse (all scored free with `check.py --sym`):
//   * `return field_200;` on the done-side zero path, so the two zero return
//     sites are different expressions in the source: 98.2%, byte-identical diff.
//     MSVC 5's value numbering sees through `cmp eax, ebx / je` and folds the
//     field back to the constant 0 at that site, so the sites merge again. This
//     is why the return VALUE is the wrong lever and the block GRAPH is the
//     right one.
//   * `goto fail` with a trailing `fail: return 0;`: 98.2% (constant tail) and
//     79.8% / 303 bytes (single register tail), the two known families.
//   * an exit test that does not prove the returned dword is zero
//     (`> 0`, `< 0`, `>= 0`) does keep the two exits apart and does reach the
//     original's block graph, but the branch becomes `jle` and MSVC 5 inlines
//     the failure epilogue after the inverted branch: the 79.8% family.
//   * `(char)field_200 != 0` as the exit test: MSVC 5 reloads the field from
//     memory (`cmp byte ptr [edi + 0x200], bl`) and the shared
//     `mov eax, [edi + 0x200]` leaves the done block. Much worse.
//
// Suspected original bugs: none found. The reader/writer sides of
// field_200/field_208/field_280 in this file and its callers agree.
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>

extern char __stdcall FindNextCdDrive(char drive);
extern int __stdcall GetVolumeSerial(char drive);

class Sound {
public:
    char unknown_0[0x200];
    int field_200;                     // +0x200  number of CD audio tracks
    int field_204;                     // +0x204
    int field_208;                     // +0x208
    int field_20c;                     // +0x20c
    int field_210;                     // +0x210
    char unknown_214[0x280 - 0x214];
    int field_280;                     // +0x280  track 1 is not audio

    int QueryDisc();
};

// FUNCTION: 0x4cda00
int Sound::QueryDisc()
{
    int other;
    int i;
    char type[32];
    char buf[32];

    field_208 = 0;
    field_20c = 0;
    int drive = FindNextCdDrive(0);
    if (drive != 0)
        field_210 = GetVolumeSerial(drive);
    int hr = mciSendStringA("status cdaudio number of tracks", buf, 0x20, 0);
    if (hr == 0) {
        field_200 = atoi(buf);
        mciSendStringA("set cdaudio time format tmsf", 0, 0, 0);
        hr = mciSendStringA("status cdaudio type track 1", type, 0x20, 0);
        if (hr != 0)
            goto notAudio;
        if (strcmp(type, "audio") != 0) {
            other = strcmp(type, "other");
            for (i = 0; i < other; i++) {
            }
            goto notAudio;
        } else {
            field_280 = 0;
            goto done;
        }
notAudio:
        field_280 = 1;
        if (--field_200 < 0)
            field_200 = 0;
done:
        if (field_200 != 0)
            field_208 = 1;
        return field_200;
    }
    return 0;
}
