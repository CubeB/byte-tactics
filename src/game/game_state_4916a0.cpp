// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Saves the per-track bytes from the CD object into the CD-list settings
// block and writes it to the registry under "CDLISTS", then releases the
// CD-list buffers, the track buffer and the rest of the game state.

extern char* g_game;

class Class_004ce450 {
public:
    int GetTrackCount();
};

class Class_004ce7e0 {
public:
    unsigned char GetCategoryOfTrack(int param_1);
};

// The CD-list settings block: 0x24 bytes of header, then 0xaa0-0x24 bytes
// of per-track data (0x51e84c onwards).
struct CdLists_4916a0 {
    char unknown_0[0x24];
    unsigned char tracks[0xaa0 - 0x24]; // +0x24
};

extern CdLists_4916a0 DAT_0051e828;

struct Obj_004aeda0;
struct Obj_004aef80;
struct Class_00452370;

void __stdcall WriteGameRegistryValue(void* key, void* buf, int value);
void FreePictureCache();
void __stdcall FUN_004aeda0(Obj_004aeda0* obj, int i);
void __stdcall FUN_004aef80(Obj_004aef80* obj);
void FUN_00431920();
void FUN_00431a20();
void FreeSounds();
void FUN_0042a3b0();
void ShutdownSound();
void FreeAnimFiles();
void __cdecl FUN_004d85a0(int* param_1);
void __stdcall SetRestoreSurface(int param_1);
void RestoreScreen();
void FUN_0043c350();
void FreeUnitInfo();
void __stdcall ReleasePacketData(Class_00452370* obj);
void FUN_00434b90();

// FUNCTION: 0x4916a0
void ShutdownGame(void)
{
    for (int i = 0; i < ((Class_004ce450*)*(void**)(g_game + 0x10))->GetTrackCount(); i++) {
        DAT_0051e828.tracks[i] = ((Class_004ce7e0*)*(void**)(g_game + 0x10))->GetCategoryOfTrack(i + 1);
    }
    WriteGameRegistryValue("CDLISTS", &DAT_0051e828, 0xaa0);
    FreePictureCache();
    FUN_004aeda0((Obj_004aeda0*)(g_game + 0x519), 1);
    FUN_004aeda0((Obj_004aeda0*)(g_game + 0x519), 0);
    FUN_004aef80((Obj_004aef80*)(g_game + 0x519));
    FUN_00431920();
    FUN_00431a20();
    FreeSounds();
    FUN_0042a3b0();
    ShutdownSound();
    FreeAnimFiles();
    FUN_004d85a0(*(int**)(g_game + 0x37e1b));
    *(int*)(g_game + 0x37e1b) = 0;
    SetRestoreSurface(0);
    RestoreScreen();
    FUN_0043c350();
    FUN_004d85a0(*(int**)(g_game + 0x29a0));
    *(int*)(g_game + 0x29a0) = 0;
    FreeUnitInfo();
    ReleasePacketData((Class_00452370*)(g_game + 0x12ef));
    FUN_00434b90();
}
