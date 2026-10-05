// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FatalError(char* text);

extern char DAT_0050310c[];
extern char DAT_005119b8[];

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

class Class_004c4760 {
public:
    double FUN_004c4760(const char* name, double def);
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c2ea0 {
public:
    int field_0;
    Class_004c46c0* current;            // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_00438320 {
public:
    char name[0x20];                    // +0x0
    int radius;                         // +0x20
    float density;                      // +0x24
    float duration;                     // +0x28
    float interval;                     // +0x2c
    void LoadMeteorDefaults();
};

// FUNCTION: 0x438320
void Class_00438320::LoadMeteorDefaults()
{
    Class_004c2ea0 parser;
    char path[256];
    BuildDataPath(path, "gamedata", "meteor", DAT_0050310c);
    if (((Class_004c2f60*)&parser)->FUN_004c2f60(path)
        && ((Class_004c3410*)&parser)->FUN_004c3410("Default")) {
        if (((Class_004c48c0*)parser.current)->FUN_004c48c0((char*)this, "MeteorWeapon", 0x20, DAT_005119b8)) {
            radius = parser.current->FUN_004c46c0("MeteorRadius", 0);
            density = (float)((Class_004c4760*)parser.current)->FUN_004c4760("MeteorDensity", 0.0);
            duration = (float)((Class_004c4760*)parser.current)->FUN_004c4760("MeteorDuration", 0.0);
            float intervalTime = (float)((Class_004c4760*)parser.current)->FUN_004c4760("MeteorInterval", 0.0);
            interval = intervalTime;
            if (radius != 0 && density != 0.0f && duration != 0.0f && intervalTime != 0.0f)
                return;
        }
        FatalError("Hey, hoser!  The default meteor shower data was bogus!");
    }
}
