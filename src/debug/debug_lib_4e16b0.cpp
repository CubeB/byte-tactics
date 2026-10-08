// Decompiled by Claude Opus 5.5. Names are provisional.

char IsPentiumOrBetter(void);

extern char g_cpuFeaturesRead;
extern char g_cpuHasTsc;

// FUNCTION: 0x4e16b0
char FUN_004e16b0(void)
{
    unsigned int features;
    if (!g_cpuFeaturesRead) {
        g_cpuFeaturesRead = 1;
        if (IsPentiumOrBetter()) {
            __asm {
                push eax
                push ebx
                push ecx
                push edx
                mov eax, 1
                _emit 0x0f      // cpuid, which this compiler's assembler does not know
                _emit 0xa2
                mov features, edx
                pop edx
                pop ecx
                pop ebx
                pop eax
            }
            if (features & 0x800000)
                g_cpuHasTsc = 1;
        }
    }
    return g_cpuHasTsc;
}
