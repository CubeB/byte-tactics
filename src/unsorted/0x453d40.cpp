// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL. This is the giant incoming-network-message interpreter that runs on
// the game object (g_game). It is the largest function in the batch, 8944 bytes
// as the checker sizes it (0x453d40..0x456030), of which the last 0xae bytes
// (0x455f81..0x45602f) are the switch jump tables the compiler placed in .text
// right after the code; the real code ends at 0x455f7f.
//
// What is confidently known and what still differs:
//  - The file below reproduces the entry gate, the ten-slot reset loop and the
//    outer `while (FUN_004534e0())` frame, but NOT the stack frame size: the
//    original allocates 0x51c bytes of locals (327 dwords), so it opens with
//    `sub esp, 0x51c`. A faithful reproduction needs the full body below to
//    drive the same local allocation; a partial version cannot match
//    instruction 2 onwards and therefore scores near 0%.
//  - Every remaining byte is body: a two-level dispatch over the received
//    message. First on the byte at packet[0] (cases 3, 5, 0x102, 0x103, 0x104),
//    then, for many cases, a 43-entry switch on another field (0x454864
//    `cmp eax, 0x2a`; `jmp [eax*4+0x455f84]`) whose table is at 0x455f84.
//  - The overwhelmingly repeated inlined helper is "find the player slot whose
//    id field (game+0x1b67+i*0x14b) equals X, searching i = 0..9, else 10".
//    It appears dozens of times, sometimes on the id at +0x1b67, sometimes on
//    the live flag (info+0x97 bit 0) selecting game+0x1b8a, sometimes both.
//    Recognising it is the key to reading the function: each long
//    `xor bl,bl / cmp bl,0xa / ... / inc bl` chain near 0x1bd6 is one copy,
//    and local stack slots 0x38,0x68,0x78,0x84,0x8c,0x94,0xa4,0xb4,0xec,0x104,
//    0x110,0x114 hold the resulting slot index.
//  - Player record stride is 0x14b. Per-player flag bytes tested:
//    +0x73 (state: 1/2/3 alive-ish, 0xa dead), +0x97 (in-game), +0x9b bits
//    0x80/0x40 (packed ushort), +0x146 (0xa), +0x2b and +0x49 are 0x1e-byte
//    strings copied with strncpy from the packet.
//  - Cases: 3 -> FUN_00450a10(id) then FUN_00456030(slot) checks and
//    FUN_00453010(slot, 3/4/8) side messages; 5 with packet[1]==1 -> target
//    lookup and FUN_00453010(...,1)/FUN_00463c60(slot,0); 0x102 -> unpack a
//    0x2e-dword (plus 1 byte) structure into player info and possibly
//    FUN_00453010(...,9); 0x103 -> test bit at player+0x9b (0x80) then
//    strncpy two names; 0x104 -> copy 0x14 dwords to game+0x471 and check
//    DAT_00512bc0 command-bit tables. Also DAT_005119b8 + FUN_004c9890 for
//    the chat/say path at 0x454135.
//  - Tail calls FUN_00450980 and FUN_00453c20 run once after the loop, then
//    the function returns the number of messages processed (local at esp+0x70).
//
// The skeleton below is deliberately only the head; the body has to be added
// case by case (see the notes above). It compiles and is the starting point for
// whoever takes this function next.

#pragma pack(push, 1)
struct Player_00453d40 {
    char unknown_0[0x10];
    int field_10;                     // +0x10
    char unknown_14[0x27 - 0x14];
    int field_27;                     // +0x27, pointer to player info
    char unknown_2b[0x73 - 0x2b];
    unsigned char state;              // +0x73: 1/2/3 alive, 0x0a dead
    char unknown_74[0x97 - 0x74];
    unsigned char flags_97;           // +0x97
    char unknown_98[0x146 - 0x98];
    unsigned char field_146;          // +0x146
};

struct Game_00453d40 {
    char unknown_0[0x14];
    char unknown_14[0x2a34 - 0x14];
    int field_2a34;                   // +0x2a34
    void* packet;                     // +0x2a38, pointer to the message

    char unknown_2a3c[0x2a42 - 0x2a3c];
    unsigned char localPlayer;        // +0x2a42
    char unknown_2a43[0x2a44 - 0x2a43];
    unsigned char flags_2a44;         // +0x2a44
};

// The player arrays live at +0x1a28 (parallel field, stride 0x14b),
// +0x1b63 (records, stride 0x14b) and +0x1b8a (info pointers, stride 0x14b).
struct GameTail_00453d40 {
    char unknown_0[0x471];
    int field_471[0x14];              // +0x471
    char unknown_4c1[0x2a34 - 0x4c1];
    int field_2a34;                   // +0x2a34
};
#pragma pack(pop)

extern Game_00453d40* g_game;
extern unsigned char DAT_00512bc0[];
extern char DAT_005119b8[];

// ret (no stack args): plain cdecl.
int __cdecl FUN_004534e0(void);
void __cdecl FUN_00450980(void);
void __cdecl FUN_00453c20(void);

// FUNCTION: 0x453d40
int FUN_00453d40(void)
{
    if (!(g_game->flags_2a44 & 1))
        return 0;

    // Ten players: zero the dword at +0x1a28 + i*0x14b (stride 0x14b).
    for (int i = 0; i < 10; i++)
        *(int*)((char*)g_game + 0x1a28 + (i + 1) * 0x14b) = 0;

    int processed = 0;
    while (FUN_004534e0() != 0) {
        processed++;
        // Body not yet reconstructed: dispatch on the message at g_game->packet
        // (see the notes at the top of this file).
    }

    FUN_00450980();
    FUN_00453c20();
    return processed;
}
