// Decompiled by Opus. Names are provisional.
// Writes the camera position to a section ("Camera", "X Position", "Z Position").

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

extern char DAT_00502890[]; // "Camera"
extern char DAT_00502878[]; // "X Position"
extern char DAT_00502884[]; // "Z Position"

extern char* g_game;

// FUNCTION: 0x41d360
void __stdcall FUN_0041d360(Class_004b4560* file)
{
    file->FUN_004b4560(DAT_00502890);
    ((Class_004b4630*)file)->FUN_004b4630(DAT_00502878, *(int*)(g_game + 0x1431f));
    ((Class_004b4630*)file)->FUN_004b4630(DAT_00502884, *(int*)(g_game + 0x14323));
}
