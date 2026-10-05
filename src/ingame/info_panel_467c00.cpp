// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Blits a player's logo entry (indexed by data->field_96) from the logos32
// table onto a destination rectangle shifted down by dy. src is the full
// texture rectangle, dst the screen rectangle.

#pragma pack(push, 1)
struct PlayerData_467c00 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
};

struct Player_467c00 {
    char unknown_0[0x27];
    PlayerData_467c00* data;           // +0x27
};

struct Game {
    char unknown_0[0x148db];
    void* logos32;                     // +0x148db
};
#pragma pack(pop)

extern Game* g_game;

struct Rect_467c00 {
    int left;                          // +0
    int top;                           // +4
    int right;                         // +8
    int bottom;                        // +0xc
};

struct Point_467c00 {
    int x;
    int y;
};

struct Quad_467c00 {
    Point_467c00 p[4];
};

struct Entry_467c00 {
    unsigned short w;                  // +0
    unsigned short h;                  // +2
};

void __stdcall FUN_004c7580(void* surf, void* entry, Quad_467c00* dst, Quad_467c00* src);

// FUNCTION: 0x467c00
void __stdcall FUN_00467c00(void* surf, Player_467c00* player, Rect_467c00* rect, int dy)
{
    unsigned char idx = player->data->field_96;
    Entry_467c00* entry = (Entry_467c00*)*(void**)((char*)g_game->logos32 + idx * 8 + 0x28);

    Quad_467c00 src;
    src.p[0].x = 0;
    src.p[0].y = 0;
    src.p[1].x = entry->w;
    src.p[1].y = 0;
    src.p[2].x = entry->w;
    src.p[2].y = entry->h;
    src.p[3].x = 0;
    src.p[3].y = entry->h;

    Quad_467c00 dst;
    dst.p[0].x = rect->left;
    dst.p[0].y = rect->top + dy;
    dst.p[1].x = rect->right;
    dst.p[1].y = rect->top + dy;
    dst.p[2].x = rect->right;
    dst.p[2].y = rect->bottom + dy;
    dst.p[3].x = rect->left;
    dst.p[3].y = rect->bottom + dy;

    FUN_004c7580(surf, entry, &dst, &src);
}
