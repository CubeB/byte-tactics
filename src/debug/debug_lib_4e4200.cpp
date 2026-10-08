// Decompiled by Haiku. Names are provisional.
extern void __cdecl EmptyAtexitHandler();
extern int __cdecl atexit(void (*func)());

// FUNCTION: 0x4e4200
void FUN_004e4200()
{
    atexit(EmptyAtexitHandler);
}
