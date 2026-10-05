// Decompiled by space-bunny-free. Names are provisional.
// Pushes the current CD/music state into the menu gadgets. The gadgets that
// follow the transport are set to the inverse of the "NOTRAK" flag, "TRACKMODE"
// gets the track mode index, and "TRACKTYPE" is 0 only while the CD is in
// state 4 and tracking is on.

class Class_004a1080;
class Class_004a1250;
class Object_004a1450;

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char gui[0x37f14 - 0x519];          // +0x519
    char notrak;                         // +0x37f14
    char unknown_37f15;
    unsigned char state;                 // +0x37f16
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall SetButtonStageByName(Class_004a1080* obj, char* name, int value);
void __stdcall FUN_004a1250(Class_004a1250* obj, char* name, int value);
void __stdcall FUN_004a1450(Object_004a1450* obj, char* name, int value);

// FUNCTION: 0x45d130
void FUN_0045d130()
{
    SetButtonStageByName((Class_004a1080*)g_game->gui, "NOTRAK", g_game->notrak & 1);
    SetButtonStageByName((Class_004a1080*)g_game->gui, "TRACKMODE", g_game->state - 1);
    FUN_004a1450((Object_004a1450*)g_game->gui, "MUSICVOL", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "CDPREV", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "CDSTOP", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "CDPLAY", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "CDNEXT", (char)(~g_game->notrak & 1));
    FUN_004a1250((Class_004a1250*)g_game->gui, "TRACKMODE", (char)(~g_game->notrak & 1));
    // Spelled as one negated test, not an if/else with a call in each arm:
    // MSVC then materialises the value in a register instead of pushing 0/1.
    FUN_004a1250((Class_004a1250*)g_game->gui, "TRACKTYPE", !((g_game->notrak & 1) && g_game->state == 4));
}
