// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Shutdown of the map/game resources: frees the eyeball array, runs the
// feature pool cleanup (0x422170), then releases the radar frame, the map
// tables, the two map grids and several unnamed buffers, nulling each slot.
// Every slot is freed without a null test except the radar frame, the
// 0x1421f pair and the four word/height fields of the grids; the grid
// destroys keep `&grid` in edi across the operator delete call.
#pragma pack(push, 1)
struct Grid_00483dd0 {
    void* cells;                       // +0x0
    int width;                         // +0x4
    int height;                        // +0x8
    int field_c;                       // +0xc
};

struct Game_00483dd0 {
    char unknown_0[0x141fb];
    void* field_141fb;                 // +0x141fb
    void* field_141ff;                 // +0x141ff
    void* field_14203;                 // +0x14203
    char unknown_14207[0x1421f - 0x14207];
    void** field_1421f;                // +0x1421f
    char unknown_14223[0x1426b - 0x14223];
    void* radarFrame;                  // +0x1426b
    char unknown_1426f[0x14273 - 0x1426f];
    void* field_14273;                 // +0x14273
    char unknown_14277[0x1427b - 0x14277];
    void* eyeball;                     // +0x1427b
    char unknown_1427f[0x14283 - 0x1427f];
    void* iconSet;                     // +0x14283
    void* field_14287;                 // +0x14287
    void* mapValues;                   // +0x1428b
    Grid_00483dd0 grid1;               // +0x1428f
    Grid_00483dd0 grid2;               // +0x1429f
    char unknown_142af[0x142b7 - 0x142af];
    void* field_142b7;                 // +0x142b7
};
#pragma pack(pop)

extern Game_00483dd0* g_game;

void FUN_004d85a0(void* p);
void FUN_00422170();
void operator delete(void* p);

// FUNCTION: 0x483dd0
void FUN_00483dd0()
{
    FUN_004d85a0(g_game->eyeball);
    FUN_00422170();
    if (g_game->radarFrame) {
        FUN_004d85a0(g_game->radarFrame);
        g_game->radarFrame = 0;
    }
    FUN_004d85a0(g_game->field_14203);
    FUN_004d85a0(g_game->field_141ff);
    FUN_004d85a0(g_game->field_141fb);
    FUN_004d85a0(g_game->iconSet);
    FUN_004d85a0(g_game->field_14273);
    FUN_004d85a0(g_game->field_14287);
    FUN_004d85a0(g_game->mapValues);
    g_game->field_14203 = 0;
    g_game->field_141ff = 0;
    g_game->field_141fb = 0;
    g_game->iconSet = 0;
    g_game->field_14273 = 0;
    g_game->field_14287 = 0;
    g_game->mapValues = 0;

    void** p = g_game->field_1421f;
    if (p) {
        operator delete(p[0]);
        operator delete(p);
    }
    g_game->field_1421f = 0;

    Grid_00483dd0* g1 = &g_game->grid1;
    g1->width = 0;
    g1->height = 0;
    operator delete(g1->cells);
    g1->field_c = 0;
    g1->cells = 0;

    Grid_00483dd0* g2 = &g_game->grid2;
    g2->width = 0;
    g2->height = 0;
    operator delete(g2->cells);
    g2->field_c = 0;
    g2->cells = 0;

    operator delete(g_game->field_142b7);
    g_game->field_142b7 = 0;
}
