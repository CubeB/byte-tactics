// Decompiled by Claude Opus 5.5, deepseek-v4.1-flash, Haiku and Opus. Names are provisional.

#include <stdio.h>
#include <string.h>
#include <windows.h>

class TdfRecord;

#include "../util/tdf.h"


struct FindData_0041d4c0 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

extern int DAT_0050289c;
extern int g_cdBypassDriveScan;
extern int g_cdPathMismatch;

void HAPI_DropMissingArchives();
void ChdirToExeDirectory();
void HAPI_ResolveShadowedFiles();
int __stdcall HAPI_FindFirst(const char* path, FindData_0041d4c0* fd, int a, int b);
int __stdcall HAPI_FindNext(int handle, FindData_0041d4c0* fd);
void __stdcall HAPI_FindClose(int handle);
int __stdcall HAPI_AddArchive(char* path, int flag);
char __stdcall FindNextCdDrive(char drive);
char __stdcall FindGameCdDrive(int side);
char* __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
char* __stdcall StripExtension(char* name);
void __cdecl FUN_004d83a0(int);

// Registers the game's data archives, only when DAT_0050289c is set (the
// flag that makes FindGameCdDrive use the current directory instead of a CD):
// rev31.GP3, then every *.CCX, *.UFO and *.HPI file in the current directory
// (stopping after 10 HPI files that HAPI_AddArchive accepts), then the *.hpi
// files in the root of every CD-ROM drive.
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
                // continue, not break: break changes the first FindNextCdDrive argument.
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

// FUNCTION: 0x41d6a0
char __stdcall FindGameCdDrive(int side)
{
    if (DAT_0050289c != 0)
        return '.';
    char* name;
    switch (side) {
    case 0:
        name = "Campaign";
        break;
    case 1:
        name = "Multiplayer";
        break;
    default:
        return 0;
    }
    char drive = 0;
    do {
        if (g_cdBypassDriveScan != 0)
            drive = drive ? '\0' : 'h';
        else
            drive = FindNextCdDrive(drive);
        if (drive == 0)
            continue;
        char path[256];
        sprintf(path, "%c:\\TOTALA.ID", drive);
        if (path[0] != drive)
            g_cdPathMismatch = 1;
        TdfFile parser;
        if (parser.LoadFile(path)
            && parser.SelectRecord("Contents")
            && parser.current->GetFieldInt(name, 0))
            return drive;
    } while (drive);
    return 0;
}

// FUNCTION: 0x41d7b0
char* __stdcall BuildCdFilePath(char* out, const char* dir, const char* name, const char* ext)
{
    if (DAT_0050289c != 0) {
        char c = 0;
        int i;
        for (i = 0; i < 2; i++) {
            c = FindGameCdDrive(i);
            if (c != 0) {
                break;
            }
        }
        if (c == 0) {
            *out = 0;
            return 0;
        }
        sprintf(out, "%c\\\\%s\\%s", c, dir, name);
        StripExtension(out);
        if (strlen(ext) != 0) {
            strcat(out, ".");
            strcat(out, ext);
        }
        return out;
    }
    return BuildDataPath(out, dir, name, ext);
}

// FUNCTION: 0x41d8a0
int FUN_0041d8a0(void)
{
    return DAT_0050289c;
}

// FUNCTION: 0x41d8b0
int GetCdPathMismatch(void)
{
    return g_cdPathMismatch;
}

// FUNCTION: 0x41d8c0
void* __stdcall AllocZeroedWithTickOffset(unsigned int size)
{
    unsigned int pad = GetTickCount() % 1000 * 7;
    unsigned int total = pad + size;
    char* p = (char*)operator new(total);
    memset(p, 0, total);
    FUN_004d83a0((int)p);
    return p + pad;
}
