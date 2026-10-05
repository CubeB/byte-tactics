// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct Bitmap_004cb170 {
    int width;                         // +0x0
    int height;                        // +0x4
    char unknown_8[4];
    unsigned char* data;               // +0xc
};

#pragma pack(push, 1)
struct Game_004cb170 {
    char unknown_0[0xbc];
    Bitmap_004cb170* bitmap;           // +0xbc
    char unknown_c0[0x1c];
    int field_dc;                      // +0xdc
    char unknown_e0[0x214 - 0xe0];
    PALETTEENTRY palette[256];         // +0x214
};
#pragma pack(pop)

struct FindData_004cb170 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

extern char DAT_00503374[];
extern char DAT_005119b8[];

Game_004cb170* FUN_004b6220();
int __stdcall FUN_004bc4b0(const char* path, FindData_004cb170* fd, int a, int b);
int __stdcall FUN_004bc640(int handle, FindData_004cb170* fd);
void __stdcall FUN_004bc8d0(int handle);
int __stdcall FUN_004cac40(char* name, unsigned char* data, int width, int height, unsigned char* palette);

// FUNCTION: 0x4cb170
int __stdcall FUN_004cb170(char* param_1, char* param_2)
{
    Game_004cb170* game = FUN_004b6220();
    int flag = 0;
    char filename[260];
    FindData_004cb170 fd;
    unsigned char pal[768];
    int best = 0;

    char c = param_1[0];
    if (c != '\0') {
        int len = strlen(param_1);
        if (param_1[len - 1] != '\\')
            flag = 1;
    }
    if (game->field_dc == 0)
        return 0;

    const char* sep = flag ? DAT_00503374 : DAT_005119b8;
    sprintf(filename, "%s%s%s*.pcx", param_1, sep, param_2);

    int handle = FUN_004bc4b0(filename, &fd, -1, 1);
    if (handle >= 0) {
        do {
            int n = atoi(fd.name + strlen(param_2));
            if (n > best)
                best = n;
        } while (FUN_004bc640(handle, &fd) == 0);
        FUN_004bc8d0(handle);
    }

    sep = flag ? DAT_00503374 : DAT_005119b8;
    sprintf(filename, "%s%s%s%04i.pcx", param_1, sep, param_2, best + 1);

    Bitmap_004cb170* bitmap = game->bitmap;
    Game_004cb170* g = FUN_004b6220();
    for (int i = 0; i < 256; i++) {
        pal[i * 3] = g->palette[i].peRed;
        pal[i * 3 + 1] = g->palette[i].peGreen;
        pal[i * 3 + 2] = g->palette[i].peBlue;
    }
    return FUN_004cac40(filename, bitmap->data, bitmap->width, bitmap->height, pal);
}
