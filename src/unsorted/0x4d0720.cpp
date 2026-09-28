// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL: 97.5%, code size matches (205 bytes), every instruction matches
// except the scheduling of the pre-loop block at 0x4d076c-0x4d077d.
// The original loads the target parameter into ebx before pushing the strncmp
// arguments and puts "mov edi, 0x14" last, just before the call:
//     mov ebx,[esp+0x1c] / push 4 / lea eax,[esp+0x14] / push ebx / push eax
//     mov edi,0x14 / call strncmp
// and this source gives
//     push 4 / lea eax,[esp+0x14] / mov ebx,[esp+0x20] / mov edi,0x14
//     push ebx / push eax / call strncmp
// Note the displacement difference: 0x1c against 0x20.  That is not a second
// fault, it is the same one.  Loading ebx before the "push 4" leaves the
// parameter at [esp+0x1c]; loading it after leaves it at [esp+0x20].
// Everything else (the three locals at esp+0xc/0x10/0x18, the rotation of the
// loop so the name test sits in the preheader, the destructive "add ecx,edi"
// for the seek argument, and both epilogues) matches byte for byte, and so
// does the loop body's second strncmp at 0x4d07be, which reuses ebx without
// reloading it in both versions.  So the two loads really are one decision in
// one basic block, and it is a scheduling tie rather than a structural one.
//
// TRIED AND REJECTED (all free, scored with wcl + objdump, not check.py):
//   "for (pos = 0x14; ; )"      identical output to the line above it
//   "unsigned int pos = 0x14;"  sinks "mov edi,0x14" to the very top of the
//                               block (and the total += 8 becomes an in-place
//                               "add [mem],8"), which is worse
//   "const char* target"        no change
//   inverted test with else     no change to the block
//   "while (1)" instead of      MUCH worse, and informative: it rotates the
//   "for (;;)"                  whole loop, moves file to edi and the angle
//                               to esi, and reorders the back edge.  The
//                               original wants "for (;;)"; do not try "while".
// So the loop spelling is settled and the residual really is the scheduler.
// This is the same tie the guide records at 0x45b6570 and 0x426200, where a
// local reload drifts around a call's argument pushes.
// Walks a sound file's marker table: the header holds the table size (plus the
// 8 bytes of the two header fields) and the first marker, then the table is a
// run of [4 byte name][4 byte offset] pairs. Returns the offset of the named
// marker, or 0 when the table runs out.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

// FUNCTION: 0x4d0720
int __stdcall FUN_004d0720(void* file, char* target)
{
    char name[4];
    unsigned int total;
    unsigned int pos;
    unsigned int off;

    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &total, 4);
    total += 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, name, 4);
    FUN_004bb7c0(file, &off, 4);
    pos = 0x14;
    for (;;) {
        if (strncmp(name, target, 4) == 0)
            return off;
        FUN_004bb710(file, pos + off);
        pos += off;
        if (pos >= total)
            return 0;
        FUN_004bb7c0(file, name, 4);
        FUN_004bb7c0(file, &off, 4);
        pos += 8;
    }
}
