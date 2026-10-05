// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Makes the game's entry table writable, then fills each entry's short list
// (max 30) with the type ids whose element rows match the entry index, and
// finally restores the table to read-only.

#pragma pack(push, 1)
struct Elem_0042be30 {              // 0x25 bytes
    unsigned short index;           // +0x00
    char unknown_2[2];
    char name[0x25 - 4];            // +0x04
};
struct Rec_0042be30 {               // 0xbd bytes
    int count;                      // +0x00
    Elem_0042be30 elems[5];         // +0x04
};
struct Entry_0042be30 {             // 0x249 bytes
    char unknown_0[0x152];
    int count;                      // +0x152
    unsigned short* items;          // +0x156
    char unknown_15a[0x249 - 0x15a];
};
struct Game {
    char unknown_0[0x1438f];
    int count1;                     // +0x1438f
    char unknown_14393[8];
    Entry_0042be30* entries;        // +0x1439b
    char unknown_1439f[0x391c7 - 0x1439f];
    int count2;                     // +0x391c7
    Rec_0042be30* recs;             // +0x391cb
};
#pragma pack(pop)

extern Game* g_game;

unsigned short __stdcall FindUnitTypeId(char* name);
void __cdecl FUN_004d8710(void* p);
void __cdecl FUN_004d8780(void* p);

// FUNCTION: 0x42be30
void AddDownloadBuildOptions()
{
    FUN_004d8780(g_game->entries);
    Entry_0042be30* e = g_game->entries;
    for (int a = 0; a < g_game->count1; a++, e++) {
        if (e->items != 0) {
            for (int b = 0; b < g_game->count2; b++) {
                for (int c = 0; c < g_game->recs[b].count; c++) {
                    if (a == g_game->recs[b].elems[c].index && e->count <= 0x1e) {
                        unsigned short id = FindUnitTypeId(g_game->recs[b].elems[c].name);
                        if (id != 0) {
                            e->items[e->count] = id;
                            e->count++;
                        }
                    }
                }
            }
        }
    }
    FUN_004d8710(g_game->entries);
}
