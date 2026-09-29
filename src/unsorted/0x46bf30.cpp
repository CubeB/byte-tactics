// Decompiled by nemotron-3.5-lightning-free, finished by space-bunny-free. Names are provisional.
// Brings up the network session and the remote console. Copies the service
// provider GUID out of the joined network game, refuses the three providers
// that cannot host one, then loads reporter.dll and resolves the Reporting
// Interface entry points into the DAT_0051e5xx globals.
// Returns 0 once the console is up, 2 when reporter.dll could not be loaded
// complete, and 4 when there is still nothing to report to.
#include <windows.h>
#include <string.h>

// A DirectPlay service provider GUID, 16 bytes: the IPX, modem and serial
// providers, none of which can host a remote console.
struct Guid_0046bf30 {
    unsigned long data1;
    unsigned long data2;
    unsigned long data3;
    unsigned long data4;
};

extern char* g_game;             // 0x511de8
extern char DAT_00512c98[];      // 0x512c98
extern Guid_0046bf30 DAT_004fcd98;
extern Guid_0046bf30 DAT_004fcdb8;
extern Guid_0046bf30 DAT_004fcdc8;

void __stdcall FUN_0046bc60(int param_1);
int __stdcall FUN_0046bc70(char* text, int mode);

typedef int (__stdcall *EnableFn_0046bf30)(int);
typedef int (__stdcall *VersionFn_0046bf30)(char*);
typedef int (__stdcall *InitFn_0046bf30)(int*, int);
typedef int (__stdcall *ReportFn_0046bf30)(int, int, int, int, int, int, int, int, int, int);
typedef int (__stdcall *ChatFn_0046bf30)(int, char*);
typedef void (__stdcall *SetCbFn_0046bf30)(void (__stdcall* cb1)(int), int (__stdcall* cb2)(char*, int));
typedef int (__stdcall *TimerFn_0046bf30)();
typedef void (__cdecl *TermFn_0046bf30)();

extern EnableFn_0046bf30 DAT_0051e588;         // _RIEnable@4
extern InitFn_0046bf30 DAT_0051e54c;           // _RIInitializeEx@8
extern VersionFn_0046bf30 DAT_0051e554;        // _RIGetVersion@4
extern TermFn_0046bf30 DAT_0051e558;           // _RITerminate@0
extern SetCbFn_0046bf30 DAT_0051e578;         // _RISetCallbacks@8
extern TimerFn_0046bf30 DAT_0051e580;         // _RIIntervalTimer@0
extern ReportFn_0046bf30 DAT_0051e584;         // _RIReport@40
extern ChatFn_0046bf30 DAT_0051e548;          // _RIReportGameChat@8
extern HMODULE DAT_0051e58c;
extern int DAT_0051e590;

int FUN_0046bce0();
void FUN_0046c190();
void __stdcall FUN_004caa20(int param_1, int param_2);

// The eight GetProcAddress calls are nested one inside the next so that the
// cleanup is the fall-through out of all eight, not a goto target: MSVC 5 lays
// a goto target out right after the last branch that reaches it, which would
// put it before the block below instead of between it and the tail.
// The first two GUID tests jump to the shared "return 4" at the end while the
// third returns inline; the original duplicates that epilogue only for the
// third, and writing all three the same way does not match.

// FUNCTION: 0x46bf30
int __stdcall FUN_0046bf30(int* param_1, int param_2)
{
    char* net;

    FUN_0046c190();

    net = *(char**)(g_game + 0x4e5);
    if (net != 0) {
        memcpy(g_game + 0x39201, net + 0x10, 16);
    }

    if (DAT_00512c98[0] == 0) {
        if (memcmp(g_game + 0x39201, &DAT_004fcd98, 16) == 0) goto four;
        if (memcmp(g_game + 0x39201, &DAT_004fcdb8, 16) == 0) goto four;
        if (memcmp(g_game + 0x39201, &DAT_004fcdc8, 16) == 0) {
            return 4;
        }
    }

    if (FUN_0046bce0() == 2) {
        FUN_0046c190();
        return 2;
    }

    DAT_0051e590 = 1;
    FUN_004caa20((int)FUN_0046bc60, (int)FUN_0046bc70);

    if (DAT_0051e58c == 0) {
        DAT_0051e58c = LoadLibraryA("reporter.dll");
        if (DAT_0051e58c != 0) {
            DAT_0051e588 = (EnableFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIEnable@4");
            if (DAT_0051e588 != 0) {
                DAT_0051e554 = (VersionFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIGetVersion@4");
                if (DAT_0051e554 != 0) {
                    DAT_0051e54c = (InitFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIInitializeEx@8");
                    if (DAT_0051e54c != 0) {
                        DAT_0051e584 = (ReportFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIReport@40");
                        if (DAT_0051e584 != 0) {
                            DAT_0051e548 = (ChatFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIReportGameChat@8");
                            if (DAT_0051e548 != 0) {
                                DAT_0051e578 = (SetCbFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RISetCallbacks@8");
                                if (DAT_0051e578 != 0) {
                                    DAT_0051e580 = (TimerFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RIIntervalTimer@0");
                                    if (DAT_0051e580 != 0) {
                                        DAT_0051e558 = (TermFn_0046bf30)GetProcAddress(DAT_0051e58c, "_RITerminate@0");
                                        if (DAT_0051e558 != 0) {
                                            if (DAT_0051e554("Total Annihilation") != 0) {
                                                DAT_0051e558();
                                                FreeLibrary(DAT_0051e58c);
                                                DAT_0051e58c = 0;
                                            } else if (DAT_0051e54c(param_1, param_2) == 4) {
                                                DAT_0051e558();
                                                FreeLibrary(DAT_0051e58c);
                                                DAT_0051e58c = 0;
                                            } else {
                                                DAT_0051e578(FUN_0046bc60, FUN_0046bc70);
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
