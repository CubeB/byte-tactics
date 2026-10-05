// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

extern int g_meteorsEnabled;
extern int g_meteorActive;
extern int g_meteorNextStrikeTime;
extern int g_meteorStrikeEndTime;
extern int g_meteorNextHitTime;
extern short g_meteorOrigin;
extern short DAT_00512322;
extern short g_meteorTarget;
extern short DAT_00512336;

// Reads the "Meteor" section into the meteor weapon globals.
// FUNCTION: 0x438250
void __stdcall LoadMeteors(Class_004b4560* file)
{
    file->FUN_004b4560("Meteor");
    g_meteorsEnabled = ((Class_004b4800*)file)->FUN_004b4800("Enabled", 0);
    g_meteorActive = ((Class_004b4800*)file)->FUN_004b4800("Active", 0);
    g_meteorNextStrikeTime = ((Class_004b4800*)file)->FUN_004b4800("Next Strike Time", 0);
    g_meteorStrikeEndTime = ((Class_004b4800*)file)->FUN_004b4800("Time Strike Ends", 0);
    g_meteorNextHitTime = ((Class_004b4800*)file)->FUN_004b4800("Next Hit Time", 0);
    g_meteorOrigin = ((Class_004b4800*)file)->FUN_004b4800("Origin X", 0);
    DAT_00512322 = ((Class_004b4800*)file)->FUN_004b4800("Origin Z", 0);
    g_meteorTarget = ((Class_004b4800*)file)->FUN_004b4800("Target X", 0);
    DAT_00512336 = ((Class_004b4800*)file)->FUN_004b4800("Target Z", 0);
}
