// Decompiled by Space Bunny Free. Names are provisional.
// Queues a speech message for a unit: drops it if the message is not due yet or
// already queued, makes room by playing the last entry when the list already
// holds eight, then inserts the new entry in priority order.

#include <string.h>

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* data);

#pragma pack(push, 1)
struct Unit_0047fad0;

struct Entry_0047fad0 {                // 0x11 bytes
    int field_0;                       // +0x0 message kind
    int field_4;                       // +0x4 frame it was queued at
    Unit_0047fad0* unit;               // +0x8
    char* data;                        // +0xc
    char field_10;                     // +0x10 priority
};

struct Message_0047fad0 {              // 0x18 bytes
    int priority;                      // +0x0
    char unknown_4[8];
    char* text;                        // +0xc
    unsigned int minFrame;             // +0x10
    char unknown_14[4];
};

class Class_0047f960 {
public:
    char unknown_0[0x99];
    int count;                         // +0x99
    int field_9d;                      // +0x9d

    int FUN_0047fd70(int index, int param_2, int param_3);
};

class Class_0047fad0 {
public:
    Entry_0047fad0 entries[9];         // +0x0
    int count;                         // +0x99

    void FUN_0047fad0(Unit_0047fad0* unit, int kind, char* text);
};

struct Game_0047fad0 {
    char unknown_0[0x38a47];
    unsigned int frame;                // +0x38a47
};
#pragma pack(pop)

extern Game_0047fad0* g_game;
extern Message_0047fad0 DAT_005086dc[];

// FUNCTION: 0x47fad0
void Class_0047fad0::FUN_0047fad0(Unit_0047fad0* unit, int kind, char* text)
{
    if (g_game->frame < DAT_005086dc[kind].minFrame)
        return;

    for (int i = 0; i < count; i++)
        if (entries[i].field_0 == kind)
            return;

    if (count == 8) {
        ((Class_0047f960*)this)->FUN_0047fd70(7, 0, 1);
        if (entries[7].data) {
            FUN_004d85a0(entries[7].data);
            entries[7].data = 0;
        }
        for (int i = 7; i < count; i++)
            entries[i] = entries[i + 1];
        count--;
    }

    int j;
    if (count != 0) {
        for (j = 0; j < count; j++)
            if (DAT_005086dc[entries[j].field_0].priority < DAT_005086dc[kind].priority)
                break;

        for (int i = count; i > j; i--)
            entries[i] = entries[i - 1];
    } else {
        j = 0;
    }

    entries[j].field_0 = kind;
    entries[j].field_4 = g_game->frame;
    entries[j].unit = unit;
    entries[j].field_10 = DAT_005086dc[kind].priority;
    if (text) {
        entries[j].data = (char*)FUN_004d83b0("Speech Text", strlen(text) + 1);
        strcpy(entries[j].data, text);
    } else {
        entries[j].data = 0;
    }
    count++;
}
