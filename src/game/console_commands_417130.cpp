// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

void SaveSettings();

// FUNCTION: 0x417130
void __stdcall CmdScreenChat(int unused)
{
    int eax = DAT_00511de8;
    *(int*)(eax + 0x37f02) ^= 1;
    SaveSettings();
}
