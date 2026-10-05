// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads (or saves) the PerformanceSettings block of the Cavedog registry key.

class Class_004e2be0 {
public:
    int key;                           // +0x00
    unsigned char readOnly;            // +0x04
    Class_004e2be0(int readOnly, char* app, char* section);
    ~Class_004e2be0();
};

class Class_004e2fe0 {
public:
    void FUN_004e2fe0(char* name, unsigned char* value, unsigned char def);
};

class Class_004e2e20 {
public:
    void FUN_004e2e20(char* name, unsigned int* value, int minValue, int maxValue, int defaultValue);
};

extern unsigned char DAT_00529dd8;
extern unsigned char DAT_00529dd4;
extern unsigned char DAT_00529ddc;
extern unsigned char DAT_00529e64;
extern unsigned char DAT_00529dc8;
extern unsigned int DAT_00529e00;
extern unsigned int DAT_00529e10;

// FUNCTION: 0x4e1b10
void __cdecl FUN_004e1b10(int readOnly)
{
    Class_004e2be0 key(readOnly, "PerformanceSettings", "CavedogLibrary");
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("EnabledInRelease", &DAT_00529dd8, 0);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("RaisePriority", &DAT_00529dd4, 1);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("DisplayInDebugger", &DAT_00529ddc, 0);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("DisplayInWindow", &DAT_00529e64, 1);
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("AutoPairing", &DAT_00529dc8, 1);
    ((Class_004e2e20*)&key)->FUN_004e2e20("Event0", &DAT_00529e00, 0, -1, 0);
    ((Class_004e2e20*)&key)->FUN_004e2e20("Event1", &DAT_00529e10, 0, -1, 0);
}
