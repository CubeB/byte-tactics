// Decompiled by Claude Opus 5.5. Names are provisional.
// Registers the game's data archives, only when DAT_0050289c is set (the
// flag that makes 0x41d6a0 use the current directory instead of a CD):
// rev31.GP3, then every *.CCX, *.UFO and *.HPI file in the current directory
// (stopping after 10 HPI files that HAPI_AddArchive accepts), then the *.hpi
// files in the root of every CD-ROM drive.
//
// The drive loop needs `continue` (as in 0x41d6a0), not `break`: with
// `break` MSVC passes a constant 0 to the first FindNextCdDrive call and rotates
// the loop, where the original stores 0 to the drive slot and reloads it.
#include <stdio.h>

struct FindData_0041d4c0 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

extern int DAT_0050289c;

void HAPI_DropMissingArchives();
void ChdirToExeDirectory();
void HAPI_ResolveShadowedFiles();
int __stdcall HAPI_FindFirst(const char* path, FindData_0041d4c0* fd, int a, int b);
int __stdcall HAPI_FindNext(int handle, FindData_0041d4c0* fd);
void __stdcall HAPI_FindClose(int handle);
int __stdcall HAPI_AddArchive(char* path, int flag);
char __stdcall FindNextCdDrive(char drive);

// FUNCTION: 0x41d4c0
void RegisterDataArchives()
{
    FindData_0041d4c0 fd;
    char pattern[64];
    char path[256];
    if (DAT_0050289c != 0) {
        HAPI_DropMissingArchives();
        ChdirToExeDirectory();
        sprintf(pattern, "rev%s.GP3", "31");
        int h = HAPI_FindFirst(pattern, &fd, -1, 1);
        if (h >= 0) {
            do {
                HAPI_AddArchive(fd.name, 1);
            } while (HAPI_FindNext(h, &fd) == 0);
            HAPI_FindClose(h);
        }
        h = HAPI_FindFirst("*.CCX", &fd, -1, 1);
        if (h >= 0) {
            do {
                HAPI_AddArchive(fd.name, 1);
            } while (HAPI_FindNext(h, &fd) == 0);
            HAPI_FindClose(h);
        }
        h = HAPI_FindFirst("*.UFO", &fd, -1, 1);
        if (h >= 0) {
            do {
                HAPI_AddArchive(fd.name, 0);
            } while (HAPI_FindNext(h, &fd) == 0);
            HAPI_FindClose(h);
        }
        int hpi = HAPI_FindFirst("*.HPI", &fd, -1, 1);
        int left = 10;
        if (hpi >= 0) {
            do {
                if (HAPI_AddArchive(fd.name, 0))
                    left--;
            } while (HAPI_FindNext(hpi, &fd) == 0 && left != 0);
            HAPI_FindClose(hpi);
        }
        char drive = 0;
        do {
            drive = FindNextCdDrive(drive);
            if (drive == 0)
                continue;
            sprintf(path, "%c:\\*.hpi", drive);
            h = HAPI_FindFirst(path, &fd, -1, 1);
            if (h >= 0) {
                do {
                    sprintf(path, "%c:\\%s", drive, fd.name);
                    HAPI_AddArchive(path, 0);
                } while (HAPI_FindNext(h, &fd) == 0);
                HAPI_FindClose(h);
            }
        } while (drive);
        HAPI_ResolveShadowedFiles();
    }
}
