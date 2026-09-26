// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Object_0047e750 {
    char unknown_0[0x6a];
    int x;                                   // +0x6a
    char unknown_6e[0x72 - 0x6e];
    int y;                                   // +0x72
    char unknown_76[0x8e - 0x76];
    Object_0047e750* next;                   // +0x8e
};

struct Cell_0047e750 {                       // 10 bytes
    char unknown_0[6];
    Object_0047e750* head;                   // +0x6
};

struct Game_0047e750 {
    char unknown_0[0x1429f];
    Cell_0047e750* cells;                    // +0x1429f
    unsigned int width;                      // +0x142a3
    unsigned int height;                     // +0x142a7
};
#pragma pack(pop)

class Visitor_0047e750 {
public:
    virtual void Visit(Object_0047e750* obj) = 0;
};

extern Game_0047e750* g_game;

static inline int Clamp_0047e750(int v, unsigned int size)
{
    if (v < size) {
        return v;
    }
    if (v < 0) {
        return 0;
    }
    return size - 1;
}

// FUNCTION: 0x47e750
void __stdcall FUN_0047e750(int x1, int y1, int x2, int y2, Visitor_0047e750* visitor)
{
    int cx1 = Clamp_0047e750(x1 >> 23, g_game->width);
    int cy1 = Clamp_0047e750(y1 >> 23, g_game->height);
    int cx2 = Clamp_0047e750(x2 >> 23, g_game->width);
    int cy2 = Clamp_0047e750(y2 >> 23, g_game->height);
    for (int y = cy1; y <= cy2; y++) {
        for (int x = cx1; x <= cx2; x++) {
            for (Object_0047e750* o = g_game->cells[g_game->width * y + x].head; o != 0; o = o->next) {
                if (o->x >= x1 && o->x <= x2 && o->y >= y1 && o->y <= y2) {
                    visitor->Visit(o);
                }
            }
        }
    }
}
