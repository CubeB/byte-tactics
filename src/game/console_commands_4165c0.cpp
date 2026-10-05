// Decompiled by Opus. Names are provisional.

struct Flags_004165c0
{
    unsigned short low : 8;
    unsigned short flag : 1;
    unsigned short rest : 7;
};

extern void* g_game;

// Command arguments.
class Class_004b73e0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0
    unsigned char FUN_004b73e0(int index, int fallback);
};

void SaveSettings(void);

// FUNCTION: 0x4165c0
void __stdcall CmdSwitchAlt(Class_004b73e0* args)
{
    if (args->count < 2) {
        Flags_004165c0* f = (Flags_004165c0*)((char*)g_game + 0x37f06);
        f->flag = !f->flag;
        SaveSettings();
        return;
    }
    ((Flags_004165c0*)((char*)g_game + 0x37f06))->flag = args->FUN_004b73e0(1, 0);
}
