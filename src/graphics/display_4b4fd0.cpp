// Decompiled by Opus. Names are provisional.
// Stores a callback and the argument it will be called with.

extern void (__cdecl *g_closeHandler)(int);
extern int g_closeHandlerArg;

// FUNCTION: 0x4b4fd0
void __stdcall SetCloseHandler(void (__cdecl *callback)(int), int param)
{
    g_closeHandler = callback;
    g_closeHandlerArg = param;
}
