// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Entry_004232f0 {
    short prev;                          // +0x0
    short next;                          // +0x2
    char unknown_4[0x30 - 4];
};

struct Pool_004232f0 {
    Entry_004232f0* entries;             // +0x0
    char unknown_4[4];
    int usedHead;                        // +0x8
    int unknown_c;                       // +0xc
    int freeHead;                        // +0x10
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1420b];
    Pool_004232f0 pool;                  // +0x1420b
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x4232f0
void __stdcall MoveFeatureSpot(int index, int* head)
{
    Pool_004232f0* p = &g_game->pool;
    Entry_004232f0* e = &p->entries[index];
    if (e->next == -1) {
        if (p->usedHead == index)
            p->usedHead = e->prev;
        else if (p->unknown_c == index)
            p->unknown_c = e->prev;
        else if (p->freeHead == index)
            p->freeHead = e->prev;
    } else {
        p->entries[e->next].prev = e->prev;
    }
    if (e->prev != -1)
        p->entries[e->prev].next = e->next;
    short old = *head;
    e->next = -1;
    e->prev = old;
    *head = index;
    if (e->prev != -1)
        p->entries[e->prev].next = index;
}
