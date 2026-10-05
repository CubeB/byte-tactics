// Decompiled by Opus. Names are provisional.
// Writes the camera position to a section ("Camera", "X Position", "Z Position").

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
};

extern char DAT_00502890[]; // "Camera"
extern char DAT_00502878[]; // "X Position"
extern char DAT_00502884[]; // "Z Position"

extern char* g_game;

// FUNCTION: 0x41d360
void __stdcall WriteCameraPosition(HapiBank* file)
{
    file->OpenAccount(DAT_00502890);
    ((HapiBank*)file)->SetIntegerItem(DAT_00502878, *(int*)(g_game + 0x1431f));
    ((HapiBank*)file)->SetIntegerItem(DAT_00502884, *(int*)(g_game + 0x14323));
}
