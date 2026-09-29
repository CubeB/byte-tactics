// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL, 3.7% (baseline for this retry was 1.1%). No MATCH.
//
// This is the giant incoming-network-message interpreter on g_game;
// 8944 bytes as the checker sizes it (0x453d40..0x456030). The last 0xae bytes
// of that range (0x455f81..0x45602f) are the switch jump tables the compiler
// placed in .text after the code; real code ends at 0x455f7f.
//
// What is transcribed and confirmed against the original here:
//  - entry gate: `if (!(g_game->flags_2a44 & 1)) return 0;`
//  - the ten-slot reset loop storing 0 at g_game + 0x1a28 + (i+1)*0x14b
//  - `packet = *(int*)(g_game + 0x2a38)` once before the loop
//  - the outer `while (FUN_004534e0() != 0)` frame (the result is spilled at
//    esp+0xf0 and re-tested at the shared tail 0x455f50)
//  - the first two inlined player-slot searches: find the slot whose
//    live flag (g_game + 0x1bd6 + slot*0x14b) is set and whose id
//    (g_game + 0x1b67 + slot*0x14b) equals g_game+0x4c9 / g_game+0x4cd,
//    searching slots 0..9 else 10. Results land in esp+0x14 and esp+0x38.
//  - the message counter at esp+0x70, incremented once per message.
//  - the tail: FUN_00450980(), FUN_00453c20(), return the counter.
//
// What still differs (all of it is why this is partial):
//  - STACK FRAME. The original opens with `sub esp, 0x51c` (327 dword locals)
//    and closes with `add esp, 0x51c`; ours allocates only 8 bytes. MSVC
//    scalar-replaces constant-index array accesses, so modelling the local
//    area with `unsigned char L[0x51c]` does NOT pin the frame or the offsets.
//    Reproducing the frame requires reproducing every local the body uses
//    (the body passes pointers into the local area, e.g. lea [esp+0x12c] at
//    0x454093 feeding FUN_00451090), which pins the array.
//  - REGISTER CHOICES. The original reloads g_game inside the reset loop
//    (`mov edx,[g_game]; mov [edx+eax+0x1a28],0`) and stores an immediate 0;
//    ours hoists the pointer into esi and stores edx. Same for the loop
//    counters/registers throughout the two searches.
//  - THE WHOLE DISPATCH BODY is still missing: the two-level dispatch on
//    packet[0] (cases 3, 5, 0x102, 0x103, 0x104), the 43-entry switch at
//    0x454864 (`cmp eax,0x2a` / `jmp [eax*4+0x455f84]`) and every case body.
//    DAT_005512bc0 is a byte-per-command flag table; g_game+0x2bee bit 1 is a
//    dirty flag; +0x471 holds 0x14 copied dwords; player +0x73 is state
//    (1/2/3 alive, 0xa dead), +0x97 in-game, +0x9b packed flags, +0x146.
//
// Below is the transcribed head only; everything after the second search
// collapsed to the counter increment so the file stays honest and compiles.

extern char* g_game;

// ret (no stack args): plain cdecl.
int __cdecl FUN_004534e0(void);
void __cdecl FUN_00450980(void);
void __cdecl FUN_00453c20(void);

// FUNCTION: 0x453d40
int FUN_00453d40(void)
{
    unsigned char L[0x51c];

    if (!(*(unsigned char*)(g_game + 0x2a44) & 1))
        return 0;

    *(int*)(L + 0x70) = 0;

    int n = 10;
    int off = 0;
    do {
        off += 0x14b;
        *(int*)(g_game + 0x1a28 + off) = 0;
    } while (--n);

    *(int*)(L + 0x10) = *(int*)(g_game + 0x2a38);

    while (FUN_004534e0() != 0) {
        int target = *(int*)(g_game + 0x4c9);
        *(int*)(L + 0x1c) = target;
        if (target != -1) {
            unsigned char idx = 0;
            while (idx < 10) {
                char* rec = g_game + 0x14b * idx;
                *(unsigned char*)(L + 0xb4) = idx;
                if (rec[0x1bd6] && *(int*)(rec + 0x1b67) == target)
                    break;
                idx++;
            }
            if (idx == 10)
                *(unsigned char*)(L + 0x14) = 10;
            else
                *(unsigned char*)(L + 0x14) = idx;
        } else {
            *(unsigned char*)(L + 0x14) = 10;
        }

        int target2 = *(int*)(g_game + 0x4cd);
        if (target2 != -1) {
            unsigned char idx2 = 0;
            while (idx2 < 10) {
                char* rec = g_game + 0x14b * idx2;
                *(unsigned char*)(L + 0x110) = idx2;
                if (rec[0x1bd6] && *(int*)(rec + 0x1b67) == target2)
                    break;
                idx2++;
            }
            if (idx2 == 10)
                *(unsigned char*)(L + 0x38) = 10;
            else
                *(unsigned char*)(L + 0x38) = idx2;
        } else {
            *(unsigned char*)(L + 0x38) = 10;
        }

        *(int*)(L + 0x70) += 1;
    }

    FUN_00450980();
    FUN_00453c20();
    return *(int*)(L + 0x70);
}
