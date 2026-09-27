// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Walks the ten player slots: for every player whose controller is set
// (1, 2 or 3) and whose +0x146 byte is not 10, it folds that slot's +0xc and
// +0x10 values into running maxima, sets the player's +0x149 flag and stores
// max(value, 200) as a float at +0xdc (from the +0x10 value) and +0xe0 (from
// the +0xc value). The +0x149 flag and the 200 floor match FUN_00496e90.
//
// PARTIAL: this file does not match. The original keeps the loop index in
// ebx, tests it at the top (`cmp bl,0xa; jae`) and uses esi (the slot offset)
// for the back-edge test (`cmp esi,0xf0; jl`), with the run of stores reached
// through a walked player offset in edi and both accumulators in stack slots.
// Every source form tried here either rotates the byte counter into a
// `dec`/`jne` countdown or spills the index to the stack, so the guard and the
// walked player offset do not both appear. Best similarity reached: 44%.
// The semantics and the data layout below are right; only the register
// allocation and loop shape differ.

#pragma pack(push, 1)
struct Player_00497080 {               // 0x14b bytes
    int field_0;                       // +0x0
    char unknown_4[0x73 - 0x4];
    char controller;                   // +0x73
    char unknown_74[0xdc - 0x74];
    float width;                       // +0xdc
    float height;                      // +0xe0
    char unknown_e4[0x146 - 0xe4];
    char field_146;                    // +0x146
    char unknown_147[0x149 - 0x147];
    unsigned short flag_149 : 1;       // +0x149
};

struct Slot_00497080 {
    char unknown_0[0xc];
    int field_c;                       // +0xc
    int field_10;                      // +0x10
    char unknown_14[4];
};

struct Game_00497080 {
    char unknown_0[0x1b63];
    Player_00497080 players[10];       // +0x1b63
    char unknown_2851[0x29a0 - 0x2851];
    Slot_00497080* slots;              // +0x29a0
};
#pragma pack(pop)

extern Game_00497080* g_game;

static inline int AtLeast200(int v)
{
    if (v < 200)
        v = 200;
    return v;
}

// FUNCTION: 0x497080
void FUN_00497080()
{
    unsigned char i = 0;
    int h = 0;
    int w = 0;
    Player_00497080* p = g_game->players;
    for (int off = 0; off < 0xf0; off += 0x18, i++, p++) {
        if (i >= 10)
            continue;
        if (g_game->players[i].field_0 == 0)
            continue;
        char c = g_game->players[i].controller;
        if (c != 1 && c != 2 && c != 3)
            continue;
        if (g_game->players[i].field_146 == 10)
            continue;
        Slot_00497080* s = (Slot_00497080*)((char*)g_game->slots + off);
        if (s->field_c > h)
            h = s->field_c;
        if (s->field_10 > w)
            w = s->field_10;
        p->flag_149 = 1;
        p->width = (float)AtLeast200(w);
        p->height = (float)AtLeast200(h);
    }
}
