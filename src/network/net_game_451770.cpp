// Decompiled by Opus. Names are provisional.
// Forwards six argument values to HAPINET_createorjoinlobbygame on the object at g_game+0x14
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

extern int __stdcall HAPINET_createorjoinlobbygame(void* obj, char* name, int a, int b, int c, int d, int e);

// FUNCTION: 0x451770
void __stdcall JoinLobbyGameThread(Args_00451770* args)
{
    args->result = HAPINET_createorjoinlobbygame(g_game + 0x14, args->name, args->arg_4, args->arg_8,
                                args->arg_c, args->arg_10, args->arg_14);
}
