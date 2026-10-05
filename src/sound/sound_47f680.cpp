// Decompiled by space-bunny-free. Names are provisional.
// Ages the sound driver held in DAT_0051e68c: if the list is not empty, and
// the game frame has reached the driver's next expiry, it refreshes that
// expiry and plays the entry's sound. Either way it then drops the head entry
// of the nine-entry list and frees its data.

void __cdecl FUN_004d85a0(void* param_1);
void FUN_0049f620();

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    unsigned int frame;               // +0x38a47
};

struct Entry_0047f680 {               // 0x11 bytes
    int field_0;                      // +0x0
    int field_4;                      // +0x4
    int field_8;                      // +0x8
    int* data;                        // +0xc
    char field_10;                    // +0x10
};

class Class_0047f960 {
public:
    Entry_0047f680 entries[9];        // +0x0
    int count;                        // +0x99
    int field_9d;                     // +0x9d
    int field_a1;                     // +0xa1
    int field_a5;                     // +0xa5

    int FUN_0047fd70(int param_1, int param_2, int index);
};
#pragma pack(pop)

extern Game* g_game;
extern Class_0047f960* DAT_0051e68c;
extern int DAT_0051e694;

// The count is reached through a reference and the entries through the local
// pointer, the fields through the global: that mix is what keeps the driver's
// value in ecx for the two calls and copies it to ebx for the rest.
// FUNCTION: 0x47f680
void FUN_0047f680()
{
    Class_0047f960* driver = DAT_0051e68c;
    int& count = DAT_0051e68c->count;
    if (count) {
        if (g_game->frame >= DAT_0051e68c->field_9d + DAT_0051e68c->field_a1) {
            DAT_0051e68c->FUN_0047fd70(0, 1, 1);
            driver->field_9d = g_game->frame;
        } else {
            DAT_0051e68c->FUN_0047fd70(0, 0, 1);
        }
        if (driver->entries[0].data) {
            FUN_004d85a0(driver->entries[0].data);
            driver->entries[0].data = 0;
        }
        for (int i = 0; i < count; i++)
            driver->entries[i] = driver->entries[i + 1];
        count--;
    }
    if (DAT_0051e694)
        FUN_0049f620();
}
