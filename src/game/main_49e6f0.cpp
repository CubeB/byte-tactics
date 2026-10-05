// Decompiled by Haiku. Names are provisional.

void OutOfMemoryHandler();
void __cdecl SetOutOfMemoryHandler(void* func);

// FUNCTION: 0x49e6f0
void FUN_0049e6f0()
{
    SetOutOfMemoryHandler((void*)OutOfMemoryHandler);
}
