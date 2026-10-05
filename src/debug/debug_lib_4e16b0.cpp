// Decompiled by Claude Opus 5.5. Names are provisional.

char FUN_004e1700(void);

extern char DAT_00529e74;
extern char DAT_00529e78;

// FUNCTION: 0x4e16b0
char FUN_004e16b0(void)
{
    unsigned int features;
    if (!DAT_00529e74) {
        DAT_00529e74 = 1;
        if (FUN_004e1700()) {
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
                DAT_00529e78 = 1;
        }
    }
    return DAT_00529e78;
}
