// Decompiled by Haiku. Names are provisional.

extern int g_defaultCommandHandler;
extern int g_defaultCommandMask;

// FUNCTION: 0x4b78e0
void __stdcall SetDefaultCommandHandler(int param_1, int param_2)
{
    g_defaultCommandHandler = param_1;
    g_defaultCommandMask = param_2;
}
