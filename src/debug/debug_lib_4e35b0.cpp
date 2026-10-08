// Decompiled by Claude Opus 5.5. Names are provisional.
#include <windows.h>
#include <string.h>

bool __cdecl ProgramPerfEvent(unsigned int effect, unsigned int level, char f1, char f2);

extern char g_cpuVendor[13];            // the CPU's vendor string (cpuid 0: ebx, edx, ecx)
extern HANDLE g_gdperfDevice;           // the GDPERF driver
extern char g_gdperfDriverReady;        // the driver is open
extern int g_cpuFamily;                 // the CPU family, 4 for anything but 5 or 6

// Opens Cavedog's GDPERF performance-counter driver (the device on NT, the
// VxD on Windows 95) on an Intel Pentium or Pentium Pro, and sets the Pentium
// Pro's counters up. Whether the counters can be read.
// FUNCTION: 0x4e35b0
char OpenGdperf(void)
{
    OSVERSIONINFOA info;
    if (g_gdperfDriverReady)
        return 1;
    memset(g_cpuVendor, 0, sizeof g_cpuVendor);
    __asm {
        mov eax, 0
        _emit 0x0f      // cpuid, which this compiler's assembler does not know
        _emit 0xa2
        mov dword ptr g_cpuVendor, ebx
        mov dword ptr g_cpuVendor[4], edx
        mov dword ptr g_cpuVendor[8], ecx
    }
    if (strcmp(g_cpuVendor, "GenuineIntel")) {
        g_gdperfDriverReady = 0;
        return 0;
    }
    info.dwOSVersionInfoSize = sizeof info;
    GetVersionExA(&info);
    if (info.dwPlatformId == VER_PLATFORM_WIN32_NT)
        g_gdperfDevice = CreateFileA("\\\\.\\GDPERF", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    else
        g_gdperfDevice = CreateFileA("\\\\.\\GDPERF.VXD", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (g_gdperfDevice != INVALID_HANDLE_VALUE) {
        g_gdperfDriverReady = 1;
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
            mov g_cpuFamily, ebx
        }
        if (g_cpuFamily == 6) {
            ProgramPerfEvent(0x60790300, 0, 1, 1);
            ProgramPerfEvent(0x60790300, 1, 1, 1);
        }
        if (g_cpuFamily >= 5)
            return 1;
    }
    return 0;
}
