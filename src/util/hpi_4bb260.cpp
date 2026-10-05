// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <stdio.h>

// FUNCTION: 0x4bb260
DWORD __stdcall GetVolumeSerial(char drive)
{
    char root[4];
    char volumeName[64];
    DWORD serial;
    DWORD maxComponentLength;
    DWORD flags;
    char fileSystemName[64];

    serial = 0;
    sprintf(root, "%c:\\", drive);
    GetVolumeInformationA(root, volumeName, 64, &serial, &maxComponentLength,
                          &flags, fileSystemName, 64);
    return serial;
}
