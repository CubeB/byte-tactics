// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (98.5%), best found. Everything matches except how the player
// index is reloaded after FUN_0048ca20: the original reloads the parameter
// (`mov eax,[esp+0x24]; and eax,0xff`), while this source reads the list's
// allocator byte instead (`mov eax,[esp+0x10]`). The whole function is 3 bytes
// off. The original does not cache `player` in a register (ebx holds the
// players[player] pointer, esi/edi hold the list/set, ebp is pushed later);
// ~60 source variants (aggregate/member/partial/constructor init, separate
// byte locals, address-taking, references, unions, helper inlines, int/char
// params, all header sets and several /O flags) all cache it or change the
// address. Reading `list.allocator` was the only phrasing that reproduced the
// original's byte-load-first codegen.
#pragma pack(push, 1)
struct Unit_004933e0 {
    char unknown_0[0x86];
    int field_86;
    int field_8a;
    char unknown_8e[0xa6 - 0x8e];
    unsigned short type;
    char unknown_a8[0x110 - 0xa8];
    unsigned int flags;
};
#pragma pack(pop)

struct Player_004933e0 {
    char unknown_0[0x14b];
};

struct Game_004933e0 {
    char unknown_0[0x1b63];
    Player_004933e0 players[10];
};

struct List_004933e0 {
    unsigned char allocator;
    Unit_004933e0** _First;
    Unit_004933e0** _Last;
    Unit_004933e0** _End;
};

extern Game_004933e0* g_game;
void __stdcall FUN_0048ca20(void* list);
unsigned int* __stdcall FUN_00488c50(char* name);
void __stdcall FUN_00488570(Unit_004933e0* unit, void* player, int arg);
void operator delete(void* p);

static inline int TestBit_004933e0(unsigned int* set, unsigned short n)
{
    return set[n >> 5] & (1 << (n & 0x1f));
}
static inline Player_004933e0* At_004933e0(unsigned char p) { return &g_game->players[p]; }

// FUNCTION: 0x4933e0
void __stdcall FUN_004933e0(unsigned char player)
{
    List_004933e0 list = { player, 0, 0, 0 };
    FUN_0048ca20(&list);
    Player_004933e0* p = &g_game->players[list.allocator];
    unsigned int* set = FUN_00488c50("Commander");
    Unit_004933e0** it = list._First;
    if (it != list._Last) {
        do {
            Unit_004933e0* unit = *it;
            if ((unit->flags & 3) != 2 && unit->field_8a == 0 && unit->field_86 == 0
                && !TestBit_004933e0(set, unit->type)) {
                FUN_00488570(unit, p, 0);
            }
            it++;
        } while (it != list._Last);
        it = list._First;
    }
    operator delete(it);
}
