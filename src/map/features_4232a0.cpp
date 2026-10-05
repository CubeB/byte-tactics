// Decompiled by Opus. Names are provisional.

struct Entry_004232a0 {
    char unknown_0[0x2f];
    unsigned char flag0 : 1;         // +0x2f bit 0
};

struct Pool_004232a0 {
    Entry_004232a0* entries;         // +0x0
    char unknown_4[4];
    int usedHead;                    // +0x8
    char unknown_c[4];
    int freeHead;                    // +0x10
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1420b];
    Pool_004232a0 pool;              // +0x1420b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall MoveFeatureSpot(int index, int* head);

// FUNCTION: 0x4232a0
int AllocFeatureSpot()
{
    Pool_004232a0* p = &g_game->pool;
    int i = p->freeHead;
    if (i == -1) {
        return 0x800;
    }
    MoveFeatureSpot(i, &p->usedHead);
    p->entries[i].flag0 = 0;
    return i;
}
