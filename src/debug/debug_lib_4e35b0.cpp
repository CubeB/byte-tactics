// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>
#include <string.h>

bool __cdecl ProgramPerfEvent(unsigned int effect, unsigned int level, char f1, char f2);

extern char DAT_00529e88[13];           // the CPU's vendor string (cpuid 0: ebx, edx, ecx)
extern HANDLE DAT_00529e98;             // the GDPERF driver
extern char DAT_00529e9c;               // the driver is open
extern int DAT_00529ea0;                // the CPU family, 4 for anything but 5 or 6

// Opens Cavedog's GDPERF performance-counter driver (the device on NT, the
// VxD on Windows 95) on an Intel Pentium or Pentium Pro, and sets the Pentium
// Pro's counters up. Whether the counters can be read.
// FUNCTION: 0x4e35b0
char OpenGdperf(void)
{
    OSVERSIONINFOA info;
    if (DAT_00529e9c)
        return 1;
    memset(DAT_00529e88, 0, sizeof DAT_00529e88);
    __asm {
        mov eax, 0
        _emit 0x0f      // cpuid, which this compiler's assembler does not know
        _emit 0xa2
        mov dword ptr DAT_00529e88, ebx
        mov dword ptr DAT_00529e88[4], edx
        mov dword ptr DAT_00529e88[8], ecx
    }
    if (strcmp(DAT_00529e88, "GenuineIntel")) {
        DAT_00529e9c = 0;
        return 0;
    }
    info.dwOSVersionInfoSize = sizeof info;
    GetVersionExA(&info);
    if (info.dwPlatformId == VER_PLATFORM_WIN32_NT)
        DAT_00529e98 = CreateFileA("\\\\.\\GDPERF", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    else
        DAT_00529e98 = CreateFileA("\\\\.\\GDPERF.VXD", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (DAT_00529e98 != INVALID_HANDLE_VALUE) {
        DAT_00529e9c = 1;
        __asm {
            mov eax, 1
            _emit 0x0f  // cpuid
            _emit 0xa2
            mov ebx, eax
            shr ebx, 8
            and ebx, 0xf
            cmp ebx, 4
            je family4
            cmp ebx, 5
            je known
            cmp ebx, 6
            je known
        family4:
            mov ebx, 4
        known:
            mov DAT_00529ea0, ebx
        }
        if (DAT_00529ea0 == 6) {
            ProgramPerfEvent(0x60790300, 0, 1, 1);
            ProgramPerfEvent(0x60790300, 1, 1, 1);
        }
        if (DAT_00529ea0 >= 5)
            return 1;
    }
    return 0;
}
