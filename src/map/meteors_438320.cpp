// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FatalError(char* text);

extern char DAT_0050310c[];
extern char DAT_005119b8[];

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, size_t size, char* def);
    int GetFieldInt(const char* name, int def);
    double GetFieldDouble(const char* name, double def);
};

class TdfFile {
public:
    int field_0;
    TdfRecord* current;            // +0x4
    int field_8;
    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    int SelectRecord(char* name);
};

class MeteorParams {
public:
    char name[0x20];                    // +0x0
    int radius;                         // +0x20
    float density;                      // +0x24
    float duration;                     // +0x28
    float interval;                     // +0x2c
    void LoadMeteorDefaults();
};

// FUNCTION: 0x438320
void MeteorParams::LoadMeteorDefaults()
{
    TdfFile parser;
    char path[256];
    BuildDataPath(path, "gamedata", "meteor", DAT_0050310c);
    if (((TdfFile*)&parser)->LoadFile(path)
        && ((TdfFile*)&parser)->SelectRecord("Default")) {
        if (((TdfRecord*)parser.current)->GetFieldString((char*)this, "MeteorWeapon", 0x20, DAT_005119b8)) {
            radius = parser.current->GetFieldInt("MeteorRadius", 0);
            density = (float)((TdfRecord*)parser.current)->GetFieldDouble("MeteorDensity", 0.0);
            duration = (float)((TdfRecord*)parser.current)->GetFieldDouble("MeteorDuration", 0.0);
            float intervalTime = (float)((TdfRecord*)parser.current)->GetFieldDouble("MeteorInterval", 0.0);
            interval = intervalTime;
            if (radius != 0 && density != 0.0f && duration != 0.0f && intervalTime != 0.0f)
                return;
        }
        FatalError("Hey, hoser!  The default meteor shower data was bogus!");
    }
}
