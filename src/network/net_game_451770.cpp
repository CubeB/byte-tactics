// Decompiled by Opus. Names are provisional.
// Forwards six argument values to FUN_004c9a70 on the object at g_game+0x14
// and stores its result after them.

struct Args_00451770 {
    char* name;                        // +0x00
    int arg_4;                         // +0x04
    int arg_8;                         // +0x08
    int arg_c;                         // +0x0c
    int arg_10;                        // +0x10
    int arg_14;                        // +0x14
    int result;                        // +0x18
};

extern char* g_game;

extern int __stdcall FUN_004c9a70(void* obj, char* name, int a, int b, int c, int d, int e);

// FUNCTION: 0x451770
void __stdcall FUN_00451770(Args_00451770* args)
{
    args->result = FUN_004c9a70(g_game + 0x14, args->name, args->arg_4, args->arg_8,
                                args->arg_c, args->arg_10, args->arg_14);
}
