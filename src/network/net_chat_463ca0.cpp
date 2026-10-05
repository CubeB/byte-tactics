// Decompiled by space-bunny-free. Names are provisional.
// Appends a line to the ring buffer of 0x48-byte entries at g_game+0x12ef,
// indexed by the short at g_game+0x2a3e (wrapped at 30). If the divisor at
// g_game+0x37f27 says the head is one behind the tail, the head is advanced so
// the oldest entry is dropped. Every access re-reads g_game and the tail from
// memory, so the expressions are written out in full, and the object at
// g_game+0x519 is recomputed for each of the last three calls.
#include <string.h>

#pragma pack(push, 1)
struct Entry_00463ca0 {                // 0x48 bytes
    char text[0x40];                   // +0x00
    unsigned int time;                 // +0x40
    unsigned short field_44;           // +0x44
    char field_46;                     // +0x46
    unsigned char field_47;            // +0x47
};

struct Game {
    char unknown_0[0x12ef];
    Entry_00463ca0 entries[30];         // +0x12ef
    char unknown_1b5f[0x2a3e - 0x1b5f];
    unsigned short tail;               // +0x2a3e
    unsigned short head;               // +0x2a40
    char unknown_2a42[0x37f27 - 0x2a42];
    int field_37f27;
    char unknown_37f2b[0x38a47 - 0x37f2b];
    unsigned int now;                  // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall FUN_004ab060(void* obj, const char* name);
void __stdcall FUN_0049fa90(void* obj);
void __stdcall FUN_0049fad0(void* obj);

// FUNCTION: 0x463ca0
void __stdcall AddMessage(char* text, unsigned char key, unsigned short value, char last)
{
    if (!*text)
        return;
    if (g_game->field_37f27 == 0)
        return;
    if ((g_game->tail + 1) % g_game->field_37f27 == g_game->head) {
        g_game->head++;
        if (g_game->head == 30)
            g_game->head = 0;
    }
    strncpy(g_game->entries[g_game->tail].text, text, 0x40);
    g_game->entries[g_game->tail].text[0x3f] = 0;
    g_game->entries[g_game->tail].time = g_game->now;
    unsigned char c = g_game->entries[g_game->tail].field_47;
    g_game->entries[g_game->tail].field_47 = (c ^ key) & 0xf ^ c;
    g_game->entries[g_game->tail].field_44 = value;
    g_game->entries[g_game->tail].field_46 = last;
    g_game->tail++;
    if (g_game->tail == 30)
        g_game->tail = 0;
    if (last != '\n')
        FUN_0047f1a0("MessageArrived", 0);
    if (FUN_004ab060((char*)g_game + 0x519, "TIMEOUT.GUI")) {
        FUN_0049fa90((char*)g_game + 0x519);
        FUN_0049fad0((char*)g_game + 0x519);
    }
}
