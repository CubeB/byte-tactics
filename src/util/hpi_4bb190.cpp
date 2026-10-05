// Decompiled by Opus. Names are provisional.
// Returns the letter of the first CD-ROM drive after the given drive letter
// (from 'A' when the letter is not a valid drive), or 0 when there is none.
#include <windows.h>
#include <stdio.h>
#include <ctype.h>

// FUNCTION: 0x4bb190
char __stdcall FUN_004bb190(char drive)
{
    char letter = toupper(drive);
    if (letter >= 'A' && letter <= 'Z')
        letter++;
    else
        letter = 'A';
    for (; letter <= 'Z'; letter++) {
        char root[4];
        sprintf(root, "%c:\\", letter);
        if (GetDriveTypeA(root) == DRIVE_CDROM)
            return letter;
    }
    return 0;
}
