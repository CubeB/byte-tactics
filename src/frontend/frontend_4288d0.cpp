// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PALETTE CACHE.
// Suspected original bug: the name search matches a cached entry whose surface
// is already 0, moves it to the front, then the alloc path shifts and inserts a
// second entry with the same name at index 0, leaving a duplicate at index 1.
#include <string.h>

struct Entry_004288d0 {
    void* surface;                     // +0x00
    int* data;                         // +0x04
    char name[0x20];                   // +0x08
};

extern Entry_004288d0 DAT_005120b8[10];
extern char* g_game;

void __stdcall SetOffscreenSurface(int param_1);
void __stdcall FillSurface(int param_1, int param_2);
void FlipScreen();
void __stdcall FreeSurface(void* param_1);
void __cdecl FUN_004d85a0(int* param_1);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void* __stdcall LoadBitmapByName(const char* name, int param_2);
int __stdcall FUN_004ab290(int param_1, int param_2);
int __stdcall SetPaletteColors(unsigned char* palette, int first, int count);

// FUNCTION: 0x4288d0
int __stdcall LoadPictureCached(const char* name, int param_2, int param_3, int param_4)
{
    void* surface = 0;
    int* data = 0;
    Entry_004288d0 saved;

    if (param_2 != 0) {
        SetOffscreenSurface(*(int*)(g_game + 0x37e1b));
        FillSurface(0, 0);
        FlipScreen();
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
            surface = LoadBitmapByName(name, (int)buf);
            data = (int*)buf;
            if (*(int*)(g_game + 0x391f1) != 6) {
                if (DAT_005120b8[9].surface != 0) {
                    FreeSurface(DAT_005120b8[9].surface);
                    FUN_004d85a0(DAT_005120b8[9].data);
                }
                for (int j = 9; j > 0; j--)
                    DAT_005120b8[j] = DAT_005120b8[j - 1];
                DAT_005120b8[0].surface = surface;
                DAT_005120b8[0].data = data;
                strcpy(DAT_005120b8[0].name, name);
            }
        }
    } else {
        surface = 0;
        // Shared tail via goto: places that block after the return.
        goto after;
    }

after:
    // Nested if, one trailing return 0: both failure exits share one return block.
    if (param_4 == 0) {
        if (*(int*)(g_game + 0x531) != 0) {
            FUN_004ab290((int)(g_game + 0x519), (int)surface);
            if (param_3 != 0)
                SetPaletteColors((unsigned char*)data, 0, 0x100);
        } else {
            *(void**)(g_game + 0x11eb) = surface;
            if (name != 0)
                strcpy(g_game + 0x11ef, name);
        }
        if (surface != 0 || name == 0) {
            if (name != 0) {
                strcpy(g_game + 0x11ef, name);
                return 1;
            }
            *(g_game + 0x11ef) = 0;
            return 1;
        }
    }
    return 0;
}
