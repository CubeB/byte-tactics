// Decompiled by space-bunny-free. Names are provisional.
// Pushes the current CD/music state into the menu gadgets that display it:
// the gadgets that follow the transport state are set to the inverse of the
// "NOTRAK" flag, "TRACKMODE" gets the track mode index, and "TRACKTYPE" is
// set to 0 only while the CD is in state 4 and tracking is on.
//
// Still differs in the last block only: the original materialises the
// "TRACKTYPE" value in a register in each arm (xor ecx,ecx / push ecx and
// mov ecx,1 / push ecx, both before the "add eax,0x519"), where a literal
// 0 or 1 gives "push 0" / "push 1" after the add. So the value there is some
// expression or temporary, not a literal: it is 0 exactly when the test
// holds, yet no instruction computes it. Every plain form I tried (local
// int/char/bool/long/short/register variable, assignment expression,
// static local, ternary, inlined helper with one return per path, and the
// boolean/negation expressions) folds to an immediate or re-tests the
// condition in the merged block. All eight earlier calls match exactly.

class Class_004a1080;
class Class_004a1250;
class Object_004a1450;

#pragma pack(push, 1)
struct Game_0045d130 {
    char unknown_0[0x519];
    char gui[0x37f14 - 0x519];          // +0x519
    char notrak;                         // +0x37f14
    char unknown_37f15;
    unsigned char state;                 // +0x37f16
};
#pragma pack(pop)

extern Game_0045d130* g_game;

int __stdcall FUN_004a1080(Class_004a1080* obj, char* name, int value);
void __stdcall FUN_004a1250(Class_004a1250* obj, char* name, int value);
void __stdcall FUN_004a1450(Object_004a1450* obj, char* name, int value);

// FUNCTION: 0x45d130
void FUN_0045d130()
{
    FUN_004a1080((Class_004a1080*)g_game->gui, "NOTRAK", g_game->notrak & 1);
    FUN_004a1080((Class_004a1080*)g_game->gui, "TRACKMODE", g_game->state - 1);
    FUN_004a1450((Object_004a1450*)g_game->gui, "MUSICVOL", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "CDPREV", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "CDSTOP", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "CDPLAY", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "CDNEXT", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "TRACKMODE", (char)(~g_game->notrak & 1));
    if ((g_game->notrak & 1) && g_game->state == 4) {
        FUN_004a1250((Class_004a1250*)g_game->gui, "TRACKTYPE", 0);
        return;
    }
    FUN_004a1250((Class_004a1250*)g_game->gui, "TRACKTYPE", 1);
}
