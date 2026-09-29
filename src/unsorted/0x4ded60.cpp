// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// PARTIAL 92.6% (1005 vs 1013 bytes). The body is correct; what remains is
// register allocation and one address-mode fold:
//  - the fat date/time fields: reading fatDate into an `unsigned int` local at
//    the top of the block is what puts the date in eax and fatTime in ecx, with
//    the strlen's not/dec ahead of the two loads: that block is now instruction
//    for instruction the original's. Reading the two WORDs straight in the
//    sprintf arguments gives the mirror image (fatTime in eax) and is 17 points
//    worse. What is left there is only that the local reverses the two WORD
//    slots, so fatDate sits at esp+0xc and fatTime at esp+0x14, the other way
//    round from the original.
//  - PE link-time pointer: original does `mov ecx,[eax+0x3c]; add ecx,eax;
//    lea ebx,[ecx+8]`; MSVC folds ours into `lea ebx,[ecx+eax+8]` for every
//    spelling tried (IMAGE_NT_HEADERS, an explicit ntBase local, a DWORD
//    lfanew local, an unsigned int base, `&nt[2]`, `nt + 2`, compound +=).
//  - the "%d processors" / "1 processor" if/else: the original repeats
//    `lea edi,[buf]` in both arms, ours hoists it above the `cmp`, and the
//    original's second arm puts the pointer in edx where ours uses eax.
//  - GlobalMemoryStatus arg: original loads `lea eax,[esp+0x24]`, ours edx
//    (a pointer local for &memStatus does not change it).
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
                    {
                        unsigned int d = fatDate;
                    sprintf(p,
                        "Executable is %d bytes long and dated %d/%d/%d %02d:%02d:%02d\n",
                        fileSize, d & 0xf, (d >> 5) & 0x1f, (d >> 9) + 1980,
                        fatTime >> 11, (fatTime >> 5) & 0x3f, (fatTime & 0x1f) * 2);
                    }
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
