// Decompiled by Opus. Names are provisional.
#include <windows.h>
#include <stdio.h>

// FUNCTION: 0x4bb200
DWORD __stdcall GetVolumeNameAndSerial(char drive, LPSTR volumeName, DWORD volumeNameSize)
{
    char root[4];
    DWORD serial;
    DWORD flags;
    DWORD maxComponentLength;
    char fileSystemName[64];

    serial = 0;
    sprintf(root, "%c:\\", drive);
    GetVolumeInformationA(root, volumeName, volumeNameSize, &serial,
                          &maxComponentLength, &flags, fileSystemName, 64);
    return serial;
}
