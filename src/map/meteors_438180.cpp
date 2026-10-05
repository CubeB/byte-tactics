// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Writes the meteor shower state to the "Meteor" section of a parsed text file.

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
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
void __stdcall SaveMeteors(HapiBank* file)
{
    file->OpenAccount("Meteor");
    ((HapiBank*)file)->SetIntegerItem("Enabled", g_meteorsEnabled);
    ((HapiBank*)file)->SetIntegerItem("Active", g_meteorActive);
    ((HapiBank*)file)->SetIntegerItem("Next Strike Time", g_meteorNextStrikeTime);
    ((HapiBank*)file)->SetIntegerItem("Time Strike Ends", g_meteorStrikeEndTime);
    ((HapiBank*)file)->SetIntegerItem("Next Hit Time", g_meteorNextHitTime);
    ((HapiBank*)file)->SetIntegerItem("Origin X", g_meteorOrigin.x);
    ((HapiBank*)file)->SetIntegerItem("Origin Z", g_meteorOrigin.y);
    ((HapiBank*)file)->SetIntegerItem("Target X", g_meteorTarget.x);
    ((HapiBank*)file)->SetIntegerItem("Target Z", g_meteorTarget.y);
}
