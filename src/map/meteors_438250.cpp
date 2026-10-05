// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4800 {
public:
    int FUN_004b4800(char* name, int def);
};

extern int DAT_0051232c;
extern int DAT_00512318;
extern int DAT_005122e8;
extern int DAT_0051231c;
extern int DAT_00512330;
extern short DAT_00512320;
extern short DAT_00512322;
extern short DAT_00512334;
extern short DAT_00512336;

// Reads the "Meteor" section into the meteor weapon globals.
// FUNCTION: 0x438250
void __stdcall FUN_00438250(Class_004b4560* file)
{
    file->FUN_004b4560("Meteor");
    DAT_0051232c = ((Class_004b4800*)file)->FUN_004b4800("Enabled", 0);
    DAT_00512318 = ((Class_004b4800*)file)->FUN_004b4800("Active", 0);
    DAT_005122e8 = ((Class_004b4800*)file)->FUN_004b4800("Next Strike Time", 0);
    DAT_0051231c = ((Class_004b4800*)file)->FUN_004b4800("Time Strike Ends", 0);
    DAT_00512330 = ((Class_004b4800*)file)->FUN_004b4800("Next Hit Time", 0);
    DAT_00512320 = ((Class_004b4800*)file)->FUN_004b4800("Origin X", 0);
    DAT_00512322 = ((Class_004b4800*)file)->FUN_004b4800("Origin Z", 0);
    DAT_00512334 = ((Class_004b4800*)file)->FUN_004b4800("Target X", 0);
    DAT_00512336 = ((Class_004b4800*)file)->FUN_004b4800("Target Z", 0);
}
