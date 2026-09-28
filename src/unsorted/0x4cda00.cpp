// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// IMPORTANT: the repository's orig/TotalA.exe is the GOG / NoCD music patch
// build, not a pristine Total Annihilation 3.1 exe (its WINMM import is
// renamed to WIN32.dll, which is the Audiere shim). This function was hex
// patched by hand in that build and cannot be reproduced from C++:
//
//   file 0xCCE40 (VA 0x4cda44), pristine: 3B C3 0F 85 DE 00 00 00
//                                        = cmp eax,ebx / jne 0x4cdb2a
//   patched:                               E9 E3 DE 02 00 90 90 90
//                                        = jmp 0x4fb92c / 3x nop
//
// The trampoline the patch adds at VA 0x4fb92c (file 0xFAD20) does
// `cmp eax,ebx / jne 0x4cdb2a / mov dword ptr [esp+0x30],0x3731 /
// jmp 0x4cda4c`: it keeps the pristine "if the MCI call failed return 0"
// test, then forces the track-count buffer to "17" before atoi. That is a
// hand-written jump into a gap region, not compiler output, so check.py can
// never print MATCH for this address against this exe.
//
// The code below is the pristine source: `if (mciSendStringA(...) != 0)
// return 0;` followed by atoi. It still differs from the original in the
// dead store of the second strcmp result (the original keeps
// `mov [esp+0xc], eax`; MSVC 5 deletes it here) and in the resulting register
// allocation (the original keeps 0 in ebx, this build of the source uses ebp
// and `test eax,eax` instead of `cmp eax,ebx`).
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>

extern char __stdcall FUN_004bb190(char drive);
extern int __stdcall FUN_004bb260(char drive);

class Class_004cda00 {
public:
    char unknown_0[0x200];
    int field_200;                     // +0x200  number of CD audio tracks
    int field_204;                     // +0x204
    int field_208;                     // +0x208
    int field_20c;                     // +0x20c
    int field_210;                     // +0x210
    char unknown_214[0x280 - 0x214];
    int field_280;                     // +0x280  track 1 is not audio

    int FUN_004cda00();
};

// FUNCTION: 0x4cda00
int Class_004cda00::FUN_004cda00()
{
    char type[32];
    char buf[32];
    int other;

    field_208 = 0;
    field_20c = 0;
    int drive = FUN_004bb190(0);
    if (drive != 0)
        field_210 = FUN_004bb260(drive);
    if (mciSendStringA("status cdaudio number of tracks", buf, 0x20, 0) != 0)
        return 0;
    field_200 = atoi(buf);
    mciSendStringA("set cdaudio time format tmsf", 0, 0, 0);
    if (mciSendStringA("status cdaudio type track 1", type, 0x20, 0) == 0) {
        if (strcmp(type, "audio") == 0) {
            field_280 = 0;
        } else {
            other = strcmp(type, "other");
            if (other == 0)
                field_280 = 1;
            else
                field_280 = 1;
        }
    } else {
        field_280 = 1;
    }
    if (field_280 != 0) {
        if (--field_200 < 0)
            field_200 = 0;
    }
    if (field_200 != 0) {
        field_208 = 1;
        return field_200;
    }
    return 0;
}
