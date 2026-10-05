// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

struct Menu_00477940;
struct Gadget_00477940;
struct Entry_00477940;

extern char* g_game;                        // 0x511de8
extern int* DAT_0051e65c;                   // 0x51e65c

void __cdecl FUN_004d85a0(int* param_1);
void __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall FUN_00476a60(int** param_1, char* param_2);
void __stdcall FUN_004a32a0(Menu_00477940* menu, char* name, int* data,
                            int count, int flag);
int __stdcall FindGadgetIndex(Entry_00477940* entries, char* name, int type);
void __stdcall FUN_004a2be0(Menu_00477940* menu, int index);
void __stdcall FUN_0049fa90(Menu_00477940* menu);

// FUNCTION: 0x477940
void __stdcall FUN_00477940(char* param_1)
{
    Gadget_00477940* gadget = (Gadget_00477940*)(*(int*)(g_game + 0x531));
    if (DAT_0051e65c != 0) {
        FUN_004d85a0(DAT_0051e65c);
        DAT_0051e65c = 0;
    }
    FUN_0047f1a0("smlbutton", 0);
    int count = FUN_00476a60(&DAT_0051e65c, param_1);
    FUN_004a32a0((Menu_00477940*)(g_game + 0x519), "Campaign",
                 DAT_0051e65c, count, 0);
    int index = FindGadgetIndex((Entry_00477940*)(*(int*)((char*)gadget + 4)),
                             "Campaign", 2);
    FUN_004a2be0((Menu_00477940*)(g_game + 0x519), index);
    FUN_0049fa90((Menu_00477940*)(g_game + 0x519));
}
