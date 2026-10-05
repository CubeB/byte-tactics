// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free,
// matched by deepseek-v4.1-flash. Names are provisional.
// MATCH, 1013 bytes. Calling convention is __cdecl (original symbol
// ?FormatSystemInfo@@YAXPADH@Z; the original ends in plain `ret`), so the /Gz
// __stdcall lever does not apply here: __stdcall gives ?...@@YG... and `ret 8`.
// /Gz changes nothing, and dropping the explicit convention on the
// GetStackLow/GetStackHigh externs changes nothing (0-arg callees).
//
// Two source facts took it from 94.2% to MATCH. Both are about VALUE NUMBERS.
//
// 1. The link-time block. The old one-expression form
//        DWORD* linkTime = (DWORD*)((char*)hMod + e_lfanew + 8);
//    let MSVC fold the NT header pointer away:
//        mov edx,[eax+0x3c]; lea ebx,[edx+eax+8]
//    and choosing edx there cascaded into every later scratch register.
//    Naming the NT header and USING IT TWICE (once for &FileHeader.
//    TimeDateStamp, once for the value) forces it to live in a register:
//        mov ecx,[eax+0x3c]; add ecx,eax; lea ebx,[ecx+8]
//    which is the original, and the whole function falls into place.
//    A named pointer to TimeDateStamp (used once) still folds.
//
// 2. The processor arms are asymmetric in the original: the then arm keeps the
//    p temporary, the else arm calls sprintf(buf + strlen(buf), ...) directly.
//    then: p = buf + strlen(buf); sprintf(p, "%d processors\n", count);
//    else: sprintf(buf + strlen(buf), "1 processor\n");
//    With p in both arms MSVC CSEs buf+strlen(buf) and hoists `lea edi` above
//    the cmp (1005 bytes); with direct in both it moves the count to eax.
//
// Scratch: build/scratch/0x4ded60/v0..v7,e1..e7,f1..f4,g1..g9,k1..k5,m1;
// build/scratch/refine/L1,L1D..L1H.
// Original bug preserved: CreateFileA failure is tested against zero at
// 0x4deee1, so INVALID_HANDLE_VALUE reaches GetFileSize at 0x4deeec.
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern char DAT_005119b8;
extern char* DAT_0050d4d0;
extern "C" int __cdecl GetStackLow(void);
extern "C" void* __cdecl GetStackHigh(void);

// FUNCTION: 0x4ded60
void __cdecl FormatSystemInfo(char* dest, int destLen)
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
    IMAGE_NT_HEADERS* pNT = (IMAGE_NT_HEADERS*)((char*)hMod + ((IMAGE_DOS_HEADER*)hMod)->e_lfanew);
    gmTimeCopy = *gmtime((time_t*)&pNT->FileHeader.TimeDateStamp);
    p = buf + strlen(buf);
    sprintf(p, "UTC link time: %08lx - %s", pNT->FileHeader.TimeDateStamp, asctime(&gmTimeCopy));

    p = buf + strlen(buf);
    sprintf(p, "Library version %d. Library date %s\n",
            996, DAT_0050d4d0 + strlen("Library date stamp: "));

    GetSystemInfo(&sysInfo);
    if (sysInfo.dwNumberOfProcessors > 1) {
        p = buf + strlen(buf);
    sprintf(p, "%d processors\n", sysInfo.dwNumberOfProcessors);
    } else {
        sprintf(buf + strlen(buf), "1 processor\n");
    }

    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatus(&memStatus);
    p = buf + strlen(buf);
    sprintf(p, "%d MBytes physical memory\n",
            (memStatus.dwTotalPhys + 900000) >> 20);

    p = buf + strlen(buf);
    sprintf(p, "Stack goes from %08lX to %08lX\n",
            GetStackLow(), GetStackHigh());

    strncpy(dest, buf, destLen);
    dest[destLen - 1] = '\0';
}
