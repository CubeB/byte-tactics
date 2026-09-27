// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <windows.h>
#include <process.h>
#include <string.h>

struct PlayerInfo_00451640 {
    char unknown_0[0x80];
};

#pragma pack(push, 1)
struct Player_00451640 {
    char unknown_0[0x27];
    PlayerInfo_00451640* info;         // +0x27
};
#pragma pack(pop)

struct Obj_00451640 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;
    unsigned short flag : 1;           // +0xf0, mask 2
};

struct Args_00451640 {
    char* name;                        // +0x00
    int arg_4;                         // +0x04
    int arg_8;                         // +0x08
    int arg_c;                         // +0x0c
    int arg_10;                        // +0x10
    int arg_14;                        // +0x14
    int result;                        // +0x18
};

extern char* g_game;
extern int DAT_00512c8c;

int __stdcall FUN_004ca450(void* net);
int FUN_0045b660(void);
void* FUN_004d83b0(char* tag, int size);
void __stdcall FUN_00491c80(int n);
void FUN_004d85a0(void* p);
unsigned __stdcall FUN_00451770(void* args);
Obj_00451640* FUN_004b6220();
void FUN_004b5910();
char* __stdcall FUN_004c5740(char* s);
void __stdcall FUN_004b6290(char* msg);

// FUNCTION: 0x451640
int __stdcall FUN_00451640(Player_00451640* p)
{
    int result = 0;
    char* name = 0;

    if (FUN_004ca450(g_game + 0x14) != 0)
        name = (char*)p->info + 0x80;

    int count = 10;
    if (FUN_0045b660() != 0 && DAT_00512c8c > 1) {
        unsigned int n = DAT_00512c8c;
        count = n < 10 ? n : 10;
        *(int*)(g_game + 0x4f1) = count;
    }

    Args_00451640* args = (Args_00451640*)FUN_004d83b0("LOBBY JOIN INFO", 0x1c);
    if (args != 0) {
        memset(args, 0, 0x1c);
        args->arg_4 = count;
        args->name = name;
        args->arg_8 = 0;
        args->arg_c = 0;
        args->arg_10 = 0;
        args->arg_14 = 0;
        args->result = 0;

        unsigned int tid = 0;
        FUN_00491c80(0x14);
        unsigned long h = _beginthreadex(0, 0x8000, FUN_00451770, args, 0, &tid);
        if (h != 0) {
            if (WaitForSingleObject((HANDLE)h, 40000) == WAIT_TIMEOUT) {
                FUN_00491c80(0x13);
                if (FUN_004b6220()->flag) {
                    FUN_004b5910();
                    Sleep(500);
                }
                FUN_004b6290(FUN_004c5740("Timed out while connecting to DirectPlay lobby!"));
            } else {
                result = args->result;
            }
        }

        FUN_00491c80(0x13);
        FUN_004d85a0(args);
    }

    return result;
}