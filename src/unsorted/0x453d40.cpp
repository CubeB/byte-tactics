// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL, 3.8%. No MATCH, and it cannot MATCH without the whole body: this is
// the giant incoming-network-message interpreter on g_game, 8944 bytes
// (0x453d40..0x456030). The last 0xae bytes are the switch jump tables the
// compiler placed in .text after the real code, which ends at 0x455f7f.
//
// Only the head (0x453d40..0x453ed4) is transcribed. Everything after it is
// collapsed to the message-counter increment so the file stays honest and
// compiles; the body is the two-level dispatch and every case.
//
// What is transcribed and confirmed against the original:
//  - entry gate: `if (!(g_game->flags_2a44 & 1)) return 0;`
//  - the ten-slot reset loop storing 0 at g_game + 0x1a28 + (i+1)*0x14b
//  - `packet = *(int*)(g_game + 0x2a38)` once before the loop
//  - the outer `while (FUN_004534e0() != 0)` frame (the result is spilled at
//    esp+0xf0 and re-tested at the shared tail 0x455f50)
//  - the first two inlined player-slot searches: find the slot whose live flag
//    (g_game + 0x1bd6 + slot*0x14b) is set and whose id
//    (g_game + 0x1b67 + slot*0x14b) equals g_game+0x4c9 / g_game+0x4cd,
//    searching slots 0..9 else 10. Results land in esp+0x14 and esp+0x38.
//  - the message counter at esp+0x70, incremented once per message.
//  - the tail: FUN_00450980(), FUN_00453c20(), return the counter.
//
// Frame finding (this pass): the original opens `sub esp, 0x51c`. Modelling the
// local area as `unsigned char L[0x51c]` plus two writes through a
// non-constant index is enough to stop scalar replacement and materialize the
// array, but the frame lands at 0x524, not 0x51c, and the fields shift. The
// bulk of the frame is a local Class_00463be0 object at esp+0x2e0 (see
// 0x454436 `lea ecx,[esp+0x2e0]; call Class_00463be0::Class_00463be0`, whose
// size is about 0x23c, ending at 0x51c). Until that region is transcribed the
// frame and therefore every esp-relative offset stays wrong, so a partial head
// cannot match.
//
// Remaining diff hunks, in address order (all of the body is missing):
//  0x453d4b  sub esp,0x51c; ours 0x524 (frame, see above)
//  0x453d5d  prologue register order: original `push edi; push esi; xor eax,eax;
//            push ebp; push ebx; mov [esp+0x70],eax; mov ecx,0xa; mov edx,[g_game]`
//  0x453d6c  reset loop reloads g_game into edx each iteration and stores an
//            immediate 0 via edx+eax; ours hoists the pointer into esi and
//            stores edx.
//  0x453d94  FUN_004534e0(); result also spilled to esp+0xf0 (shared tail test)
//  0x453ed4  entity validity checks (entity+0, +0x73 state 1/2/3, +0x146 != 0xa)
//  0x453ef7  packet[0] dispatch: cases 3 (0x45413f), 5 (0x454425),
//            0x102 (0x454425), 0x103 (0x45469e), 0x104 (0x45473f)
//  0x45413f  case 3: FUN_00450a10(packet[2]), FUN_0044ffd0/FUN_0044fe40 slot
//            search, Class_00456030::FUN_00456030, FUN_00453010(3/4/8/9)
//  0x454425  case 0x102: Class_00463be0 temporary at esp+0x2e0 (frame bulk),
//            rep movsd copy of 0xb9 bytes from packet+0xc to player+0x1b8a
//  0x454611  case 0x103: copies 0x14 dwords from packet+4 to g_game+0x471
//  0x45469e  case 0x104: strncpy(entity+0x2b, packet+0x18, 0x1e) and
//            strncpy(entity+0x49, packet+0x14, 0x1e)
//  0x45473f  non-player slot branch: reads g_game+0x391f1 (message type),
//            DAT_00512bc0 command-flag table, then the 43-entry switch at
//            0x454864 (`cmp eax,0x2a; jmp [eax*4+0x455f84]`) and all its cases
//  0x455f50  shared tail: FUN_00450980, FUN_00453c20, return counter
//  0x455f78  early return path for the second search hit
//
// Field notes for a later pass: DAT_005512bc0 is a byte-per-command flag table;
// g_game+0x2bee bit 1 is a dirty flag; g_game+0x471 holds 0x14 copied dwords;
// player +0x73 is state (1/2/3 alive, 0xa dead), +0x97 in-game, +0x9b packed
// flags, +0x146 the slot-in-use char; entity+0x27 is a pointer checked at +0x97.

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
    L[*(int*)(g_game + 0x4c9) & 0xf] = 0;
    L[*(int*)(g_game + 0x4cd) & 0xf] = 0;

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
