// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Writes the meteor shower state to the "Meteor" section of a parsed text file.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

struct Point16_00438180 {
    short x;
    short y;
};

extern int g_meteorsEnabled;
extern int g_meteorActive;
extern int g_meteorNextStrikeTime;
extern int g_meteorStrikeEndTime;
extern int g_meteorNextHitTime;
extern Point16_00438180 g_meteorOrigin;
extern Point16_00438180 g_meteorTarget;

// FUNCTION: 0x438180
void __stdcall SaveMeteors(Class_004b4560* file)
{
    file->FUN_004b4560("Meteor");
    ((Class_004b4630*)file)->FUN_004b4630("Enabled", g_meteorsEnabled);
    ((Class_004b4630*)file)->FUN_004b4630("Active", g_meteorActive);
    ((Class_004b4630*)file)->FUN_004b4630("Next Strike Time", g_meteorNextStrikeTime);
    ((Class_004b4630*)file)->FUN_004b4630("Time Strike Ends", g_meteorStrikeEndTime);
    ((Class_004b4630*)file)->FUN_004b4630("Next Hit Time", g_meteorNextHitTime);
    ((Class_004b4630*)file)->FUN_004b4630("Origin X", g_meteorOrigin.x);
    ((Class_004b4630*)file)->FUN_004b4630("Origin Z", g_meteorOrigin.y);
    ((Class_004b4630*)file)->FUN_004b4630("Target X", g_meteorTarget.x);
    ((Class_004b4630*)file)->FUN_004b4630("Target Z", g_meteorTarget.y);
}
