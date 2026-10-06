// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class HapiBank {
public:
    void OpenAccount(const char* name);
    int GetIntegerItem(char* name, int def);
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
void __stdcall LoadMeteors(HapiBank* file)
{
    file->OpenAccount("Meteor");
    g_meteorsEnabled = file->GetIntegerItem("Enabled", 0);
    g_meteorActive = file->GetIntegerItem("Active", 0);
    g_meteorNextStrikeTime = file->GetIntegerItem("Next Strike Time", 0);
    g_meteorStrikeEndTime = file->GetIntegerItem("Time Strike Ends", 0);
    g_meteorNextHitTime = file->GetIntegerItem("Next Hit Time", 0);
    g_meteorOrigin = file->GetIntegerItem("Origin X", 0);
    DAT_00512322 = file->GetIntegerItem("Origin Z", 0);
    g_meteorTarget = file->GetIntegerItem("Target X", 0);
    DAT_00512336 = file->GetIntegerItem("Target Z", 0);
}
