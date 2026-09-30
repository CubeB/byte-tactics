// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Partial: 94.2%, 1005 bytes versus 1013. Body and FAT date/time block match.
// Remaining differences: PE timestamp address folding, gmtime scratch LEA,
// library-date load timing, system-info pointer register and processor-count
// formatting. GPT-6 tried 768 header sets and 15 source variants without
// improvement. Direct sprintf(buf + strlen(buf), ...) restores the library
// pointer timing but changes processor and memory-status registers (94.1%).
// Typed NT/file headers and a separate date-prefix length do not improve it.
//
// Follow-up (deepseek-v4.1-flash): the four remaining hunks are one global
// register-allocation cascade, not four independent fixes. Writing the two
// processor arms as direct sprintf(buf + strlen(buf), ...) instead of via the
// p temporary gives 1012 bytes and fixes the library-date load timing and the
// GlobalMemoryStatus pointer register (both hunks disappear) but moves the
// processor count into eax and the GetSystemInfo pointer into edx. Keeping the
// p temporary in both arms gives 1005 bytes with the processor arms close (the
// count stays in edx) but leaves library and GlobalMemoryStatus wrong. Mixed
// forms (p temporary in the then arm, direct in the else arm) reproduce the
// then arm byte for byte, but the else arm gets lea eax where the original has
// lea edx, and the tail stays wrong. So the processor arms' source form
// decides the whole function's allocation; the original pulls the count into
// edx AND keeps the tail allocation that only the direct form gives.
// A DWORD/unsigned procCount local does not change either result. The
// linkTime fold (mov ecx,[eax+0x3c]; add ecx,eax; lea ebx,[ecx+8]) is
// unchanged by written-out two-step pointers and by IMAGE_NT_HEADERS; a
// separate asctime(&gmTimeCopy) statement is byte-identical to the nested call.
// Scratch: build/scratch/0x4ded60/v0..v7,e1..e7,f1..f4,g1..g9,k1..k5,m1.
// Original bug preserved: CreateFileA failure is tested against zero at
// 0x4deee1, so INVALID_HANDLE_VALUE reaches GetFileSize at 0x4deeec.
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern char DAT_005119b8;
extern char* DAT_0050d4d0;
extern "C" int __cdecl FUN_004d8df0(void);
extern "C" void* __cdecl FUN_004d8e20(void);

// FUNCTION: 0x4ded60
void __cdecl FUN_004ded60(char* dest, int destLen)
{
    WORD fatDate;
    time_t now;
    WORD fatTime;
    DWORD userNameSize;
    FILETIME ft;
    MEMORYSTATUS memStatus;
    SYSTEM_INFO sysInfo;
    struct tm gmTimeCopy;
    struct tm localTimeCopy;
    char userName[300];
    char buf[2000];
    char exeName[1000];
    char* p;

    buf[0] = DAT_005119b8;
    memset(buf + 1, 0, 1999);

    now = time(NULL);
    localTimeCopy = *localtime(&now);
    p = buf + strlen(buf);
    sprintf(p, "Time: %s", asctime(&localTimeCopy));

    userNameSize = 300;
    if (GetUserNameA(userName, &userNameSize) == 0) {
        strcpy(userName, "unknown user");
    }

    char* machine = getenv("computername");
    if (machine == NULL) {
        machine = "unknown machine";
    }

    if (GetModuleFileNameA(NULL, exeName, 1000) == 0) {
        strcpy(exeName, "Unknown");
    }

    sprintf(buf + strlen(buf), "%s, run by %s on %s\n", exeName, userName, machine);

    HANDLE hFile = CreateFileA(exeName, GENERIC_READ, FILE_SHARE_READ, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != 0) {
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (GetFileTime(hFile, NULL, NULL, &ft)) {
            if (FileTimeToLocalFileTime(&ft, &ft)) {
                if (FileTimeToDosDateTime(&ft, &fatDate, &fatTime)) {
                    p = buf + strlen(buf);
                    sprintf(p,
                        "Executable is %d bytes long and dated %d/%d/%d %02d:%02d:%02d\n",
                        fileSize, (fatDate >> 5) & 0xf, fatDate & 0x1f, (fatDate >> 9) + 1980,
                        fatTime >> 11, (fatTime >> 5) & 0x3f, (fatTime & 0x1f) * 2);
                }
            }
        }
        CloseHandle(hFile);
    }

    HANDLE hMod = GetModuleHandleA(NULL);
    DWORD* linkTime = (DWORD*)((char*)hMod + ((IMAGE_DOS_HEADER*)hMod)->e_lfanew + 8);
    gmTimeCopy = *gmtime((time_t*)linkTime);
    p = buf + strlen(buf);
    sprintf(p, "UTC link time: %08lx - %s", *linkTime, asctime(&gmTimeCopy));

    p = buf + strlen(buf);
    sprintf(p, "Library version %d. Library date %s\n",
            996, DAT_0050d4d0 + strlen("Library date stamp: "));

    GetSystemInfo(&sysInfo);
    if (sysInfo.dwNumberOfProcessors > 1) {
        p = buf + strlen(buf);
    sprintf(p, "%d processors\n", sysInfo.dwNumberOfProcessors);
    } else {
        p = buf + strlen(buf);
    sprintf(p, "1 processor\n");
    }

    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatus(&memStatus);
    p = buf + strlen(buf);
    sprintf(p, "%d MBytes physical memory\n",
            (memStatus.dwTotalPhys + 900000) >> 20);

    p = buf + strlen(buf);
    sprintf(p, "Stack goes from %08lX to %08lX\n",
            FUN_004d8df0(), FUN_004d8e20());

    strncpy(dest, buf, destLen);
    dest[destLen - 1] = '\0';
}
