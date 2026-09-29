// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 89.1% (1013 vs 1004 bytes). The whole body is correct; what remains
// is register allocation only:
//  - the "%d/%d/%d %02d:%02d:%02d" sprintf keeps fatTime in eax and fatDate in
//    ecx; the original has them the other way round (fatDate in eax, loaded
//    right after the strlen, fatTime in ecx).
//  - PE link-time pointer: original does `mov ecx,[eax+0x3c]; add ecx,eax;
//    lea ebx,[ecx+8]`; MSVC folds ours into `lea ebx,[ecx+eax+8]` whatever the
//    source split (tried IMAGE_NT_HEADERS and a char* ntBase local).
//  - GlobalMemoryStatus arg: original loads `lea eax,[esp+0x24]`, ours edx.
//  - `%d MBytes physical memory` / `Stack goes from` args: eax vs edx roles
//    swapped around the strlen scan.
// The `char* p; p = buf + strlen(buf); sprintf(p, ...)` form is needed for
// every append EXCEPT "%s, run by %s on %s\n", which must stay inline.
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
    WORD fatTime;
    time_t now;
    WORD fatDate;
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
                        fileSize, fatDate & 0xf, (fatDate >> 5) & 0x1f, (fatDate >> 9) + 1980,
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
