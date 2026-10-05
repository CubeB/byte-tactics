// Decompiled by Opus. Names are provisional.

struct Player_00437cd0 {
    char unknown_0[0x111];
    unsigned char flags;            // +0x111
};

extern int DAT_00512314;
extern int g_meteorActive;
extern int g_meteorNextStrikeTime;
extern char DAT_005122f0[];
extern Player_00437cd0* DAT_00512328;
extern char* g_game;

Player_00437cd0* __stdcall FindWeaponByName(char* name);

// FUNCTION: 0x437cd0
void InitMeteors()
{
    g_meteorActive = 0;
    g_meteorNextStrikeTime = DAT_00512314;
    DAT_00512328 = FindWeaponByName(DAT_005122f0);
    if (DAT_00512328 == 0) {
        DAT_00512328 = (Player_00437cd0*)(g_game + 0x2cf3);
        return;
    }
    if (!(DAT_00512328->flags & 0x20))
        DAT_00512328 = (Player_00437cd0*)(g_game + 0x2cf3);
}
