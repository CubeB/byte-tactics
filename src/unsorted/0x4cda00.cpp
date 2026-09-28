// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// NOTE: this address cannot print MATCH against the repository's
// orig/TotalA.exe. That exe is the GOG / NoCD music patch build (its WINMM
// import is renamed to WIN32.dll, the Audiere shim); the 8 bytes at
// VA 0x4cda44 were hand-patched and are not compiler output:
//
//   pristine: 3B C3 0F 85 DE 00 00 00   cmp eax,ebx / jne 0x4cdb2a
//   patched:  E9 E3 DE 02 00 90 90 90   jmp 0x4fb92c / 3x nop
//
// The replacement block sits in zero padding at VA 0x4fb92a: it repeats the
// "MCI call failed -> return 0" test and forces the track-count buffer to
// "17" (`mov dword ptr [esp+0x30], 0x3731`) before atoi. The source below is
// the pristine code, so only that 8-byte range still differs.
//
// Two codegen details drove how it is written:
//  * the second mciSendStringA result is assigned to `hr` before it is tested;
//    that is the only form MSVC 5 compiles to `cmp eax,ebx` here;
//  * `other` is written to the stack and never read (the original's
//    `mov [esp+0xc], eax`). MSVC 5 gives `other` a stack slot only when it is
//    live across a loop, so the no-op loop below (it compiles to nothing)
//    reproduces that dead store and the 0x44-byte frame.
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
    int other;
    int i;
    char type[32];
    char buf[32];

    field_208 = 0;
    field_20c = 0;
    int drive = FUN_004bb190(0);
    if (drive != 0)
        field_210 = FUN_004bb260(drive);
    if (mciSendStringA("status cdaudio number of tracks", buf, 0x20, 0) != 0)
        return 0;
    field_200 = atoi(buf);
    mciSendStringA("set cdaudio time format tmsf", 0, 0, 0);
    int hr = mciSendStringA("status cdaudio type track 1", type, 0x20, 0);
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
    if (field_200 != 0) {
        field_208 = 1;
        return field_200;
    }
    return 0;
}
