// Decompiled by space-bunny-free, Haiku, nemotron-3.5-lightning-free. Names are provisional.
#include <string.h>
#include <windows.h>

// A DirectPlay service provider GUID, 16 bytes: the IPX, modem and serial
// providers, none of which can host a remote console.
struct Guid_0046bf30 {
    unsigned long data1;
    unsigned long data2;
    unsigned long data3;
    unsigned long data4;
};

// The game state LoadReporterDll and ShutdownScoreTables share: the network game
// at +0x4e5 and the service provider GUID it copies out at +0x39201.
#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14];
    char field_14[1];                  // +0x14
    char unknown_15[0x4e5 - 0x15];
    char* net;                         // +0x4e5
    char unknown_4e9[0x39201 - 0x4e9];
    char field_39201[16];              // +0x39201
};
#pragma pack(pop)

struct PlayerInfo_0046bce0 {     // 0x18 bytes
    char unknown_0[0x14];
    void* allies;                 // +0x14
};

struct ScoreBoard_0046bce0 {     // 0xc bytes
    char unknown_0[8];
    void* ppScores;               // +0x8
};

extern Game* g_game;             // 0x511de8
extern char DAT_00512c98[];      // 0x512c98
extern Guid_0046bf30 DAT_004fcd98;
extern Guid_0046bf30 DAT_004fcdb8;
extern Guid_0046bf30 DAT_004fcdc8;
extern PlayerInfo_0046bce0** DAT_0051e574;
extern ScoreBoard_0046bce0** DAT_0051e57c;
extern char** DAT_0051e550;
extern int DAT_0051e590;
extern HMODULE DAT_0051e58c;

typedef int (__stdcall *EnableFn_0046bf30)(int);
typedef int (__stdcall *VersionFn_0046bf30)(char*);
typedef int (__stdcall *InitFn_0046bf30)(int*, int);
typedef int (__stdcall *ReportFn_0046bf30)(int, int, int, int, int, int, int, int, int, int);
typedef int (__stdcall *ChatFn_0046bf30)(int, char*);
typedef void (__stdcall *SetCbFn_0046bf30)(void (__stdcall* cb1)(int), int (__stdcall* cb2)(char*, int));
typedef int (__stdcall *TimerFn_0046bf30)();
typedef void (__cdecl *TermFn_0046bf30)();

extern EnableFn_0046bf30 g_riEnable;           // _RIEnable@4
extern InitFn_0046bf30 g_riInitializeEx;       // _RIInitializeEx@8
extern VersionFn_0046bf30 g_riGetVersion;      // _RIGetVersion@4
extern TermFn_0046bf30 g_riTerminate;          // _RITerminate@0
extern SetCbFn_0046bf30 g_riSetCallbacks;     // _RISetCallbacks@8
extern TimerFn_0046bf30 DAT_0051e580;         // _RIIntervalTimer@0
extern ReportFn_0046bf30 DAT_0051e584;         // _RIReport@40
extern ChatFn_0046bf30 DAT_0051e548;          // _RIReportGameChat@8

void ShutdownScoreTables();
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
int AllocScoreTables();
void __stdcall ReportGameEventCallback(int param_1);
int __stdcall ShowGameMessage(char* text, int mode);
void __stdcall RISetCallbacks(int param_1, int param_2);
void __stdcall HAPINET_uninitmultiplay(void* p);
void __cdecl FUN_004d85a0(void* p);

// Builds the player tables: three arrays of ten, then for each slot its player
// block, that player's allies block, a scoreboard, the scoreboard's ppScores
// block and a zeroed scores block, every block allocated through FUN_004d83b0
// with a name wsprintfA formats from the slot number. Any allocation that
// comes back null tears the lot down again through ShutdownScoreTables and reports 1,
// so the slot loop only records the failure in a flag and breaks; the
// epilogue tests that flag.
// The inner loop writes 0x48 bytes of pointers into the 0x24 byte ppScores
// block, the original's bug: 18 pointers of 4 bytes into a 36 byte block, so
// 36 bytes land past the end of the allocation.
// FUNCTION: 0x46bce0
int AllocScoreTables()
{
    char name[64];
    int failed;
    int i;

    DAT_0051e574 = (PlayerInfo_0046bce0**)FUN_004d83b0("PlayersArray", 0x28);
    DAT_0051e57c = (ScoreBoard_0046bce0**)FUN_004d83b0("ScoreBoardsArray", 0x28);
    DAT_0051e550 = (char**)FUN_004d83b0("ScoresArray", 0x28);
    if (DAT_0051e574 == 0)
        goto failed;
    if (DAT_0051e57c == 0)
        goto failed;
    if (DAT_0051e550 == 0)
        goto failed;
    memset(DAT_0051e574, 0, 0x28);
    memset(DAT_0051e57c, 0, 0x28);
    memset(DAT_0051e550, 0, 0x28);
    failed = 0;
    i = 0;
    // Both loops are `while (1)` with an early `break`: a `for` is rotated to a
    // bottom test.
    while (1) {
        if (i >= 10)
            break;
        wsprintfA(name, "PlayerInfo%d", i);
        DAT_0051e574[i] = (PlayerInfo_0046bce0*)FUN_004d83b0(name, 0x18);
        if (DAT_0051e574[i] == 0) {
            failed = 1;
            break;
        }
        wsprintfA(name, "Allies%d", i);
        DAT_0051e574[i]->allies = FUN_004d83b0(name, 0x28);
        if (DAT_0051e574[i]->allies == 0) {
            failed = 1;
            break;
        }
        wsprintfA(name, "ScoreBoard%d", i);
        DAT_0051e57c[i] = (ScoreBoard_0046bce0*)FUN_004d83b0(name, 0xc);
        if (DAT_0051e57c[i] == 0) {
            failed = 1;
            break;
        }
        wsprintfA(name, "ppScores%d", i);
        DAT_0051e57c[i]->ppScores = FUN_004d83b0(name, 0x24);
        if (DAT_0051e57c[i]->ppScores == 0) {
            failed = 1;
            break;
        }
        memset(DAT_0051e57c[i]->ppScores, 0, 0x24);
        wsprintfA(name, "Scores%d", i);
        DAT_0051e550[i] = (char*)FUN_004d83b0(name, 0x48);
        if (DAT_0051e550[i] == 0) {
            failed = 1;
            break;
        }
        memset(DAT_0051e550[i], 0, 0x48);
        int** slot = (int**)DAT_0051e57c[i]->ppScores;
        int j = 0;
        while (1) {
            if (j >= 0x48)
                break;
            // Three separate statements: otherwise the pointer becomes an induction
            // variable.
            *slot = (int*)(DAT_0051e550[i] + j);
            j += 8;
            slot++;
        }
        i++;
    }
    if (!failed)
        return 0;
failed:
    ShutdownScoreTables();
    return 1;
}

// FUNCTION: 0x46bf00
int __stdcall EnableReporter(int param_1)
{
    if (g_riEnable != 0) {
        return g_riEnable(param_1);
    }
    return 1;
}

// FUNCTION: 0x46bf20
bool IsReporterDllLoaded()
{
    return DAT_0051e58c != 0;
}

// Brings up the network session and the remote console. Copies the service
// provider GUID out of the joined network game, refuses the three providers
// that cannot host one, then loads reporter.dll and resolves the Reporting
// Interface entry points into the DAT_0051e5xx globals.
// Returns 0 once the console is up, 2 when reporter.dll could not be loaded
// complete, and 4 when there is still nothing to report to.
// FUNCTION: 0x46bf30
int __stdcall LoadReporterDll(int* param_1, int param_2)
{
    char* net;

    ShutdownScoreTables();

    net = g_game->net;
    if (net != 0) {
        memcpy(g_game->field_39201, net + 0x10, 16);
    }

    if (DAT_00512c98[0] == 0) {
        // First two jump to the shared return 4, the third returns inline.
        if (memcmp(g_game->field_39201, &DAT_004fcd98, 16) == 0) goto four;
        if (memcmp(g_game->field_39201, &DAT_004fcdb8, 16) == 0) goto four;
        if (memcmp(g_game->field_39201, &DAT_004fcdc8, 16) == 0) {
            return 4;
        }
    }

    if (AllocScoreTables() == 2) {
        ShutdownScoreTables();
        return 2;
    }

    DAT_0051e590 = 1;
    RISetCallbacks((int)ReportGameEventCallback, (int)ShowGameMessage);

    if (DAT_0051e58c == 0) {
        DAT_0051e58c = LoadLibraryA("reporter.dll");
        if (DAT_0051e58c != 0) {
            // Nested, so the cleanup is the fall-through out of all eight: a goto
            // target would be laid out before the block below.
            g_riEnable = (EnableFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIEnable@4");
            if (g_riEnable != 0) {
                g_riGetVersion = (VersionFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIGetVersion@4");
                if (g_riGetVersion != 0) {
                    g_riInitializeEx = (InitFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIInitializeEx@8");
                    if (g_riInitializeEx != 0) {
                        DAT_0051e584 = (ReportFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIReport@40");
                        if (DAT_0051e584 != 0) {
                            DAT_0051e548 = (ChatFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIReportGameChat@8");
                            if (DAT_0051e548 != 0) {
                                g_riSetCallbacks = (SetCbFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RISetCallbacks@8");
                                if (g_riSetCallbacks != 0) {
                                    DAT_0051e580 = (TimerFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIIntervalTimer@0");
                                    if (DAT_0051e580 != 0) {
                                        g_riTerminate = (TermFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RITerminate@0");
                                        if (g_riTerminate != 0) {
                                            if (g_riGetVersion("Total Annihilation") != 0) {
                                                g_riTerminate();
                                                FreeLibrary(DAT_0051e58c);
                                                DAT_0051e58c = 0;
                                            } else if (g_riInitializeEx(param_1, param_2) == 4) {
                                                g_riTerminate();
                                                FreeLibrary(DAT_0051e58c);
                                                DAT_0051e58c = 0;
                                            } else {
                                                g_riSetCallbacks(ReportGameEventCallback, ShowGameMessage);
                                            }
                                            goto tail;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            FreeLibrary(DAT_0051e58c);
            DAT_0051e58c = 0;
            return 2;
        } else {
            *param_1 = 0;
        }
    }
tail:
    if (DAT_0051e58c != 0 || DAT_0051e590 != 0) {
        return 0;
    }
four:
    return 4;
}

// FUNCTION: 0x46c190
void ShutdownScoreTables()
{
    DAT_0051e590 = 0;
    RISetCallbacks(0, 0);
    if (DAT_0051e58c != 0) {
        HAPINET_uninitmultiplay(g_game->field_14);
        if (g_riTerminate != 0)
            g_riTerminate();
        FreeLibrary(DAT_0051e58c);
        DAT_0051e58c = 0;
    }
    for (int i = 0; i < 10; i++) {
        if (DAT_0051e574 != 0) {
            FUN_004d85a0(DAT_0051e574[i]->allies);
            FUN_004d85a0(DAT_0051e574[i]);
        }
        if (DAT_0051e57c != 0) {
            FUN_004d85a0(DAT_0051e57c[i]->ppScores);
            FUN_004d85a0(DAT_0051e57c[i]);
        }
        if (DAT_0051e550 != 0) {
            FUN_004d85a0(DAT_0051e550[i]);
        }
    }
    if (DAT_0051e574 != 0) {
        FUN_004d85a0(DAT_0051e574);
        DAT_0051e574 = 0;
    }
    if (DAT_0051e57c != 0) {
        FUN_004d85a0(DAT_0051e57c);
        DAT_0051e57c = 0;
    }
    if (DAT_0051e550 != 0) {
        FUN_004d85a0(DAT_0051e550);
        DAT_0051e550 = 0;
    }
}
