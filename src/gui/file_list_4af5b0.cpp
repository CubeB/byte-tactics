// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

struct Sub_004af5b0 {
    char unknown_0[0x140];
    short field_140;                 // +0x140
};

struct FileRequester {
    void* gui;                       // +0x00
    Sub_004af5b0* field_4;           // +0x04
    char unknown_8[4];               // +0x08
    char* field_c;                   // +0x0c
    char* field_10;                  // +0x10
    char drive[0x10];                // +0x14
    char cwd[0x100];                 // +0x24
    char unknown_124[0x110];         // +0x124
    int field_234;                   // +0x234
    int field_238;                   // +0x238
};

void __stdcall GetCurrentDriveLetter(char* buf);
char* __stdcall GetDriveDirectory(char* drive, char* buf, int size);
int __stdcall ScanDirectory(char* path, int a, int b, int c, int d, int e);
void __stdcall SortFileList(int a, int b, int c, int d);
void __stdcall FUN_004a32a0(void* gui, const char* name, int x, int y, int z);

// FUNCTION: 0x4af5b0
void __stdcall FUN_004af5b0(FileRequester* obj)
{
    GetCurrentDriveLetter(obj->drive);
    GetDriveDirectory(obj->drive, obj->cwd, 0x100);
    int n = ScanDirectory(obj->field_c + 0xb6, obj->field_234, obj->field_238, 1, 0, 0);
    SortFileList(obj->field_234, obj->field_238, 0, n);
    FUN_004a32a0(obj->gui, "SWIN", obj->field_234, n, 0);
    FUN_004a32a0(obj->gui, "SIZE", obj->field_238, n, 0);
    obj->field_4->field_140 = 0;
    strcpy(obj->field_10 + 0xb6, obj->cwd);
}
