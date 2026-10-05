// Decompiled by Haiku. Names are provisional.

unsigned char __cdecl OpenGdperf(void);

extern unsigned char DAT_00529e6c;
extern unsigned char DAT_00529e70;

// FUNCTION: 0x4e1680
unsigned char __cdecl HasPerfCounters(void)
{
    if (DAT_00529e6c == 0) {
        DAT_00529e6c = 1;
        unsigned char result = OpenGdperf();
        if (result != 0) {
            DAT_00529e70 = 1;
        }
    }
    return DAT_00529e70;
}
