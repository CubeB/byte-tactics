// Decompiled by Opus. Names are provisional.
// Copies the per-track bytes from the CD object into the CD-list settings
// block and saves it to the registry under "CDLISTS".

class Class_004ce450 {
public:
    int GetTrackCount();
};

class Class_004ce7e0 {
public:
    unsigned char GetCategoryOfTrack(int param_1);
};

struct Game {
    char unknown_0[0x10];
    Class_004ce450* cd;                 // +0x10
};

// GLOBAL: 0x511de8
extern Game* g_game;

struct CdLists_490f80 {
    char unknown_0[0x24];
    unsigned char tracks[0xaa0 - 0x24]; // +0x24
};

extern CdLists_490f80 DAT_0051e828;

void __stdcall WriteGameRegistryValue(const char* key, void* data, int size);

// FUNCTION: 0x490f80
void SaveCdLists()
{
    for (int i = 0; i < g_game->cd->GetTrackCount(); i++) {
        DAT_0051e828.tracks[i] = ((Class_004ce7e0*)g_game->cd)->GetCategoryOfTrack(i + 1);
    }
    WriteGameRegistryValue("CDLISTS", &DAT_0051e828, sizeof(DAT_0051e828));
}
