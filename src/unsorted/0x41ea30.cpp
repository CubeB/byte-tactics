// Decompiled by Sonnet. Names are provisional.

struct Inner_41ea30 {
    char unknown_0[4];
    char* arrBase;   // +4
};

extern char* DAT_00511de8;   // pointer to game object

// FUNCTION: 0x41ea30
int FUN_0041ea30(void)
{
    Inner_41ea30* p531 = *(Inner_41ea30**)(DAT_00511de8 + 0x531);
    char* base = p531->arrBase;
    int i = 0;
    int count = *(short*)(base + 0xb6);
    base += 0xba;
    for (; i < count; i++, base += 0x15b) {
        if (*(unsigned char*)(base - 0xba) == 0xd) {
            if (*(unsigned char*)(base - 0x91) == 0) {
                return 0;
            }
            if (*(int*)base < *(int*)(base + 4)) {
                return 0;
            }
        }
    }
    return 1;
}
