// Decompiled by space-bunny-free. Names are provisional.
// Walks the six player slots twice: for every slot that looks like a target
// (active, type 1 or 2, field_140 set, field_22 clear) it looks for a source
// slot (active, type 3) whose data->field_94 is 1 and whose team (field_146)
// is not already listed in the target's three per-team byte tables, and hands
// the pair to FUN_004573d0. Returns 1 when no pair was found.

struct Struct_004572a0 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

#pragma pack(push, 1)
struct Player_004572a0 {
    int active;                        // +0x00
    char unknown_4[0x22 - 0x4];
    unsigned char field_22;            // +0x22
    char unknown_23[0x27 - 0x23];
    Struct_004572a0* data;             // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x11e - 0x74];
    unsigned char field_11e[0x129 - 0x11e];
    unsigned char field_129[0x140 - 0x129];
    int field_140;                     // +0x140
    short field_144;                   // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game_004572a0 {
    char unknown_0[0x1b63];
    Player_004572a0 players[6];        // +0x1b63
};
#pragma pack(pop)

extern Game_004572a0* g_game;

void __stdcall FUN_004b6b50(unsigned int param_1);
void __stdcall FUN_004573d0(void* from, void* to, unsigned char param_3);

// FUNCTION: 0x4572a0
int FUN_004572a0()
{
    int result = 1;
    for (int i = 0; i < 6; i++) {
        Player_004572a0* pi = &g_game->players[i];
        if (pi->active != 0 && (pi->type == 1 || pi->type == 2)
            && pi->field_140 != 0 && pi->field_22 == 0) {
            for (int j = 0; j < 6; j++) {
                Player_004572a0* pj = &g_game->players[j];
                unsigned char b = pj->field_146;
                if (((pj->active != 0 && pj->type == 3)
                        || (pj->field_140 == 0 || pj->field_22 != 0))
                    && pj->active != 0 && pj->type == 3
                    && ((pj->data->field_94 == 1
                            && (pi->field_11e[b] == 0 || pi->field_129[b] == 0
                                || pi->field_129[b] == 0))
                        || (pj->active != 0 && pj->type == 3 && pi->field_129[b] == 0))) {
                    FUN_004573d0(pi, pj, 1);
                    result = 0;
                }
            }
        }
    }
    FUN_004b6b50(0xfa);
    return result;
}
