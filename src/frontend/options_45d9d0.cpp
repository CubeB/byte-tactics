// Decompiled by space-bunny-free. Names are provisional.
// Pushes the low three bits of the game's sound flag byte into the sound
// settings menu, with the on/off sense of each setting inverted as needed.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char gui[0x37f19 - 0x519];        // +0x519
    unsigned char soundFlags;         // +0x37f19
};
#pragma pack(pop)

class Class_004a1080;
class Object_004a0570;
class Object_004a1450;

extern Game* g_game;
extern char DAT_005069d0[];           // "MODE"
extern char DAT_005069c8[];           // "VOLTEXT"
extern char DAT_00506884[];           // "FXVOL"
extern char DAT_005069c0[];           // "TEST"
extern char DAT_005069b8[];           // "SPEECH"

int __stdcall FUN_004a1080(Class_004a1080* obj, char* name, int value);
void __stdcall FUN_004a0570(Object_004a0570* obj, char* name, int value);
void __stdcall FUN_004a1450(Object_004a1450* obj, char* name, int value);

// FUNCTION: 0x45d9d0
void FUN_0045d9d0()
{
    FUN_004a1080((Class_004a1080*)g_game->gui, DAT_005069d0, g_game->soundFlags & 7);
    FUN_004a0570((Object_004a0570*)g_game->gui, DAT_005069c8, (g_game->soundFlags & 7) != 0);
    FUN_004a1450((Object_004a1450*)g_game->gui, DAT_00506884, (g_game->soundFlags & 7) == 0);
    FUN_004a1450((Object_004a1450*)g_game->gui, DAT_005069c0, (g_game->soundFlags & 7) == 0);
    FUN_004a1450((Object_004a1450*)g_game->gui, DAT_005069b8, (g_game->soundFlags & 7) == 0);
}
