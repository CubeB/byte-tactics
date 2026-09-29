// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL, 10-minute timebox hit. check.py: 4.1% (not MATCH). Only the opening
// is reconstructed: the +0x2bbf/+0x2bc0 state-sync check and case 0 of the
// outer switch. Everything from case 1 to case 0x14 (including the nested
// switches, the DAT_004fcdb8/004fcdc8/004fcda8 16-byte compares, the
// FUN_00450d80/strncpy error path and the char buf[256] scratch use) is NOT
// written yet; this file is a head start, not a match.
//
// What still differs, exactly (from check.py):
//   - original's second instruction is `sub esp, 0x100` (the 64-dword local
//     buffer is live somewhere in the missing cases); ours has no buffer use,
//     so the allocation is gone and the prologue is `push ebx` instead.
//   - original loads `mov cl, [eax+0x2bbf]` before `push ebx` then
//     `mov bl, [eax+0x2bc0]`; ours loads bl first. Source that produces the
//     original order has not been found yet.
//   - the dispatch is not a jump table in ours (`cmp cl,bl; jne`) because the
//     remaining cases have no bodies; with them the compiler emits the
//     `cmp ecx,0x14; ja 0x42858f; jmp [ecx*4+0x42859c]` table at 0x42859c.
//   - case 0's bit 1 test: original is `mov dl,[esi+0xf0]; shr dl,1;
//     test dl,1`, ours is `test byte [esi+0xf0], 2`.
//
// Structure (from the disassembly and Ghidra pseudo-C in
// build/scratch/0x426e80/ctx.txt):
//   outer switch on +0x2bbe for the front-end state (0..0x14, jump table
//   0x42859c). Cases 0,1,3,5 report through FUN_004256d0 and jump to shared
//   tails (0x427603 clears +0x2bbf/+0x2bc0 and returns; 0x427f88/0x427f8f and
//   0x427762/0x4272c7 are other tails). Cases 2,7,8,9,0xb..0xf,0x10 dispatch
//   again on +0x2bbf (jump tables 0x4285f0 and 0x428618 with a byte index
//   table at 0x428638). FUN_004256d0(line, file) is the checksum reporter (see
//   0x4256d0.cpp); the file literal is always
//   "c:\\cavedog\\wargame\\frontend.cpp" (DAT_00503004).

struct Class_004b6220 {
    char unknown_0[0xf0];
    unsigned char field_f0;            // +0xf0
};

extern char* g_game;

Class_004b6220* FUN_004b6220(void);
void __stdcall FUN_004256d0(int line, char* file);
void __stdcall FUN_00426780(char* name);
void __stdcall FUN_004c22d0(int param);
void FUN_00430f00();

// FUNCTION: 0x426e80
void FUN_00426e80(void)
{
    char buf[256];

    char cur = g_game[0x2bbf];
    char next = g_game[0x2bc0];
    if (next != cur) {
        FUN_004256d0(0xa3, "c:\\cavedog\\wargame\\frontend.cpp");
        g_game[0x2bbf] = next;
        g_game[0x2bc0] = next;
    }

    switch ((unsigned char)g_game[0x2bbe]) {
    case 0: {
        Class_004b6220* d = FUN_004b6220();
        FUN_004c22d0(0);
        bool active = ((d->field_f0 >> 1) & 1) != 0;
        if (active) {
            if (*(int*)(g_game + 0x3923d) != 0) {
                FUN_00426780("1.zrb");
                FUN_004256d0(0x3dc, "c:\\cavedog\\wargame\\frontend.cpp");
                g_game[0x2bbe] = 1;
                FUN_004256d0(0x9b, "c:\\cavedog\\wargame\\frontend.cpp");
                g_game[0x2bbf] = 0;
                g_game[0x2bc0] = 0;
                *(int*)(g_game + 0x3923d) = 0;
                FUN_00430f00();
                return;
            }
            if (*(int*)(g_game + 0x39245) == 0) {
                FUN_00426780("1.zrb");
                FUN_004256d0(0x3e6, "c:\\cavedog\\wargame\\frontend.cpp");
            } else {
                FUN_004256d0(0x3e9, "c:\\cavedog\\wargame\\frontend.cpp");
            }
        } else {
            FUN_004256d0(0x3ed, "c:\\cavedog\\wargame\\frontend.cpp");
        }
        g_game[0x2bbe] = 2;
        FUN_004256d0(0x9b, "c:\\cavedog\\wargame\\frontend.cpp");
        g_game[0x2bbf] = 0;
        g_game[0x2bc0] = 0;
        return;
    }
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xa:
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    default:
        break;
    }
}
