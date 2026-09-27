// Decompiled by Haiku. Names are provisional.

extern void* DAT_00509edc;
extern int __stdcall MessageBoxA(void* hWnd, const char* lpText, const char* lpCaption, unsigned int uType);

// FUNCTION: 0x4b6b80
void __stdcall FUN_004b6b80(const char* param_1, int unused) {
    (void)unused;
    MessageBoxA(0, param_1, (const char*)DAT_00509edc, 0x40000);
}
