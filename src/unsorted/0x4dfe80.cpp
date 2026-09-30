// Decompiled by Haiku. Names are provisional.

extern void FUN_004dfd10();
extern void FUN_004dfd00();
extern const char* __cdecl FUN_004d9f60(const char*);

// FUNCTION: 0x4dfe80
void FUN_004dfe80() {
    FUN_004dfd10();
    const char* result = FUN_004d9f60("-performancestatus");
    if (result != 0) {
        FUN_004dfd00();
    }
}
