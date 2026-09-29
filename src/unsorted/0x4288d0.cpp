// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PALETTE CACHE. Best: 76.7%. Still differs: after the palette buffer is
// allocated the original keeps g_game in edx across the cache shift, so its
// strcpy saves the length in edx and the tail reloads g_game into eax and
// surface into ebp; ours caches g_game in edx (reloading it after the free
// calls), uses eax for the length and reloads surface from its slot. The
// prologue, the name search, the move-to-front and the entry-9 eviction all
// match byte for byte; only the load-block/tail register allocation differs.
#include <string.h>

struct Entry_004288d0 {
    void* surface;                     // +0x00
    int* data;                         // +0x04
    char name[0x20];                   // +0x08
};

extern Entry_004288d0 DAT_005120b8[10];
extern char* g_game;

void __stdcall FUN_004c69a0(int param_1);
void __stdcall FUN_004c6890(int param_1, int param_2);
void FUN_004c63a0();
void __stdcall FUN_004c6ac0(void* param_1);
void FUN_004d85a0(int* param_1);
void* FUN_004d83b0(const char* name, unsigned int size);
void* __stdcall FUN_00429290(const char* name, int param_2);
int __stdcall FUN_004ab290(int param_1, int param_2);
int __stdcall FUN_004ba200(unsigned char* palette, int first, int count);

// FUNCTION: 0x4288d0
int __stdcall FUN_004288d0(const char* name, int param_2, int param_3, int param_4)
{
    void* surface = 0;
    int* data = 0;
    Entry_004288d0 saved;

    if (param_2 != 0) {
        FUN_004c69a0(*(int*)(g_game + 0x37e1b));
        FUN_004c6890(0, 0);
        FUN_004c63a0();
    }

    if (name != 0) {
        for (int i = 0; i < 10; i++) {
            if (strcmp(DAT_005120b8[i].name, name) == 0) {
                surface = DAT_005120b8[i].surface;
                data = DAT_005120b8[i].data;
                saved = DAT_005120b8[i];
                for (int j = i - 1; j >= 0; j--)
                    DAT_005120b8[j + 1] = DAT_005120b8[j];
                DAT_005120b8[0] = saved;
                break;
            }
        }
        if (surface == 0) {
            void* buf = FUN_004d83b0("Palette", 0x400);
            surface = FUN_00429290(name, (int)buf);
            data = (int*)buf;
            if (*(int*)(g_game + 0x391f1) != 6) {
                if (DAT_005120b8[9].surface != 0) {
                    FUN_004c6ac0(DAT_005120b8[9].surface);
                    FUN_004d85a0(DAT_005120b8[9].data);
                }
                for (int j = 9; j > 0; j--)
                    DAT_005120b8[j] = DAT_005120b8[j - 1];
                DAT_005120b8[0].surface = surface;
                DAT_005120b8[0].data = data;
                strcpy(DAT_005120b8[0].name, name);
            }
        }
    }

    if (param_4 != 0)
        goto fail;

    if (*(int*)(g_game + 0x531) != 0) {
        FUN_004ab290((int)(g_game + 0x519), (int)surface);
        if (param_3 != 0)
            FUN_004ba200((unsigned char*)data, 0, 0x100);
    } else {
        *(void**)(g_game + 0x11eb) = surface;
        if (name != 0)
            strcpy(g_game + 0x11ef, name);
    }

    if (surface == 0 && name != 0)
        goto fail;
    if (name == 0) {
        *(g_game + 0x11ef) = 0;
        return 1;
    }
    strcpy(g_game + 0x11ef, name);
    return 1;
fail:
    return 0;
}
