// Decompiled by Claude Opus 5.5. Names are provisional.
// Registers the game's data archives, only when DAT_0050289c is set (the
// flag that makes 0x41d6a0 use the current directory instead of a CD):
// rev31.GP3, then every *.CCX, *.UFO and *.HPI file in the current directory
// (stopping after 10 HPI files that FUN_004be0b0 accepts), then the *.hpi
// files in the root of every CD-ROM drive.
//
// The drive loop needs `continue` (as in 0x41d6a0), not `break`: with
// `break` MSVC passes a constant 0 to the first FUN_004bb190 call and rotates
// the loop, where the original stores 0 to the drive slot and reloads it.
#include <stdio.h>

struct FindData_0041d4c0 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

extern int DAT_0050289c;

void FUN_004be180();
void FUN_0049f540();
void FUN_004be320();
int __stdcall FUN_004bc4b0(const char* path, FindData_0041d4c0* fd, int a, int b);
int __stdcall FUN_004bc640(int handle, FindData_0041d4c0* fd);
void __stdcall FUN_004bc8d0(int handle);
int __stdcall FUN_004be0b0(char* path, int flag);
char __stdcall FUN_004bb190(char drive);

// FUNCTION: 0x41d4c0
void FUN_0041d4c0()
{
    FindData_0041d4c0 fd;
    char pattern[64];
    char path[256];
    if (DAT_0050289c != 0) {
        FUN_004be180();
        FUN_0049f540();
        sprintf(pattern, "rev%s.GP3", "31");
        int h = FUN_004bc4b0(pattern, &fd, -1, 1);
        if (h >= 0) {
            do {
                FUN_004be0b0(fd.name, 1);
            } while (FUN_004bc640(h, &fd) == 0);
            FUN_004bc8d0(h);
        }
        h = FUN_004bc4b0("*.CCX", &fd, -1, 1);
        if (h >= 0) {
            do {
                FUN_004be0b0(fd.name, 1);
            } while (FUN_004bc640(h, &fd) == 0);
            FUN_004bc8d0(h);
        }
        h = FUN_004bc4b0("*.UFO", &fd, -1, 1);
        if (h >= 0) {
            do {
                FUN_004be0b0(fd.name, 0);
            } while (FUN_004bc640(h, &fd) == 0);
            FUN_004bc8d0(h);
        }
        int hpi = FUN_004bc4b0("*.HPI", &fd, -1, 1);
        int left = 10;
        if (hpi >= 0) {
            do {
                if (FUN_004be0b0(fd.name, 0))
                    left--;
            } while (FUN_004bc640(hpi, &fd) == 0 && left != 0);
            FUN_004bc8d0(hpi);
        }
        char drive = 0;
        do {
            drive = FUN_004bb190(drive);
            if (drive == 0)
                continue;
            sprintf(path, "%c:\\*.hpi", drive);
            h = FUN_004bc4b0(path, &fd, -1, 1);
            if (h >= 0) {
                do {
                    sprintf(path, "%c:\\%s", drive, fd.name);
                    FUN_004be0b0(path, 0);
                } while (FUN_004bc640(h, &fd) == 0);
                FUN_004bc8d0(h);
            }
        } while (drive);
        FUN_004be320();
    }
}
