// Decompiled by space-bunny-free. Names are provisional.
// Rebuilds every in-use entry of the 32-entry table at 0x512358 for the
// current map size and tracks the progress percentage in g_game+0x38d73.
//
// The size is computed from the entry fields just stored (q[-1], q[-2]), not
// from the width/height locals. That makes MSVC 5 keep the table cursor in esi
// and the size in edi; reading the locals instead gives edi/esi swapped.

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14233];
    unsigned int width;                // +0x14233
    unsigned int height;               // +0x14237
    char unknown_1423b[0x38d73 - 0x1423b];
    unsigned char progress;            // +0x38d73
};
#pragma pack(pop)

extern Game* g_game;

extern char DAT_00512370[];
extern char DAT_00512770[];

struct Class_00440320 {
    int* field_0;                      // +0x0
    short field_4;
    short field_6;
    short field_8;
    short field_a;
    unsigned char field_c;
    unsigned char field_d;
    unsigned char field_e;
    unsigned char field_f;
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    void* field_18;                    // +0x18
    int field_1c;                      // +0x1c
};

class Class_00440500 {
public:
    void FUN_00440500();
};

struct Class_00440290 {
    Class_00440320 entries[32];

    static Class_00440290 DAT_00512358;
};

// FUNCTION: 0x440940
void FUN_00440940(void)
{
    int count = 0;
    unsigned int n;
    int p = (int)&Class_00440290::DAT_00512358;
    do {
        if (*(int*)p != 0) {
            count++;
        }
        p += 0x20;
    } while (p < (int)&Class_00440290::DAT_00512358 + 0x400);

    int progress = 100;
    unsigned int* q = (unsigned int*)DAT_00512370;
    do {
        if (q[-6] != 0) {
            unsigned int h = g_game->height;
            unsigned int w = g_game->width;
            q[-2] = w;
            q[-1] = h;
            n = ((unsigned)(q[-1] + 0xf) >> 4) * q[-2];
            operator delete((void*)q[0]);
            if (n != 0) {
                q[0] = (unsigned int)operator new(n * 4);
            } else {
                q[0] = 0;
            }
            ((Class_00440500*)(q - 6))->FUN_00440500();
            progress += 100;
            g_game->progress = progress / count;
        }
        q += 8;
    } while ((int)q < (int)DAT_00512770);

    g_game->progress = 100;
}
