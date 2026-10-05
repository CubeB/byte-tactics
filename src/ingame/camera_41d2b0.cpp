// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Reads the camera X and Z positions from the "Camera" section of a parsed
// text file (defaults to the current position), then clamps the view.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14281];
    unsigned short flags_14281;      // +0x14281
    char unknown_14283[0x142f1 - 0x14283];
    unsigned char flags_142f1;       // +0x142f1
    char unknown_142f2[0x1431f - 0x142f2];
    int x;                           // +0x1431f
    int y;                           // +0x14323
    int x2;                          // +0x14327
    int y2;                          // +0x1432b
};
#pragma pack(pop)

extern Game* g_game;

void FUN_0041c3c0(void);

extern char DAT_00502890[]; // "Camera"
extern char DAT_00502884[]; // "Z Position"
extern char DAT_00502878[]; // "X Position"

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

// FUNCTION: 0x41d2b0
void __stdcall FUN_0041d2b0(Class_004b4560* file)
{
    file->FUN_004b4560(DAT_00502890);
    int z = ((Class_004b4800*)file)->FUN_004b4800(DAT_00502884, g_game->y);
    int x = ((Class_004b4800*)file)->FUN_004b4800(DAT_00502878, g_game->x);
    g_game->x = x;
    g_game->y = z;
    g_game->flags_142f1 |= 2;
    FUN_0041c3c0();
    g_game->x2 = g_game->x;
    g_game->y2 = g_game->y;
    g_game->flags_14281 &= 0xfff7;
}
