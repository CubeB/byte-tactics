// Decompiled by Haiku. Names are provisional.

extern long InterlockedExchange(long* target, long value);
extern void DeleteCriticalSection(void* section);

extern long DAT_00529dc0;
extern void* DAT_00529da8;

// FUNCTION: 0x4e1510
void FUN_004e1510()
{
    long result = InterlockedExchange(&DAT_00529dc0, 3);
    if (result == 2) {
        DeleteCriticalSection(&DAT_00529da8);
    }
}
