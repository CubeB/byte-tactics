// Decompiled by Opus. Names are provisional.
// Debug "crash" chat command (cheats enabled only): argument 1 exhausts
// memory with operator new, 2 through FUN_004d83b0, 3 exits with a division
// by zero; no argument or 0 breaks into the debugger.
#include <windows.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Game_00417a60 {
    char unknown_0[0x37f2f];
    unsigned char flags_37f2f;         // +0x37f2f
    char unknown_37f30[0x3923b - 0x37f30];
    unsigned char flags_3923b;         // +0x3923b
};

struct Display_00417a60 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;           // +0xf0
    unsigned short windowed : 1;
    unsigned short bit2_15 : 14;
};
#pragma pack(pop)

extern Game_00417a60* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    char unknown_0[0xd0];
    int count;                         // +0xd0
    int FUN_004b73e0(int index, int fallback);
};

extern Display_00417a60* FUN_004b6220();
void FUN_004b5910();
void __cdecl FUN_004d83b0(char* name, unsigned int size);

// FUNCTION: 0x417a60
void __stdcall FUN_00417a60(Class_004b73e0* args)
{
    if ((g_game->flags_37f2f & 2) && (g_game->flags_3923b & 2)) {
        if (args->count > 0) {
            if (args->FUN_004b73e0(1, 0) == 1) {
                for (;;)
                    operator new(0x2000000);
            }
            if (args->FUN_004b73e0(1, 0) == 2) {
                for (;;)
                    FUN_004d83b0("FORCE OUT-OF-MEMORY", 0x2000000);
            }
            if (args->FUN_004b73e0(1, 0) == 3) {
                volatile int one = 1;   // keeps the division by zero out of the constant folder
                exit(one / (one >> 1));
                return;
            }
        }
        if (args->count == 0 || args->FUN_004b73e0(1, 0) == 0) {
            if (FUN_004b6220()->windowed) {
                FUN_004b5910();
                Sleep(500);
            }
            DebugBreak();
        }
    }
}
