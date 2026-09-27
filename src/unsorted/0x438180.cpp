// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Writes the meteor shower state to the "Meteor" section of a parsed text file.

class Class_004b4560 {
public:
    void FUN_004b4560(const char* name);
};

class Class_004b4630 {
public:
    void FUN_004b4630(const char* name, int value);
};

struct Point16_00438180 {
    short x;
    short y;
};

extern int DAT_0051232c;
extern int DAT_00512318;
extern int DAT_005122e8;
extern int DAT_0051231c;
extern int DAT_00512330;
extern Point16_00438180 DAT_00512320;
extern Point16_00438180 DAT_00512334;

// FUNCTION: 0x438180
void __stdcall FUN_00438180(Class_004b4560* file)
{
    file->FUN_004b4560("Meteor");
    ((Class_004b4630*)file)->FUN_004b4630("Enabled", DAT_0051232c);
    ((Class_004b4630*)file)->FUN_004b4630("Active", DAT_00512318);
    ((Class_004b4630*)file)->FUN_004b4630("Next Strike Time", DAT_005122e8);
    ((Class_004b4630*)file)->FUN_004b4630("Time Strike Ends", DAT_0051231c);
    ((Class_004b4630*)file)->FUN_004b4630("Next Hit Time", DAT_00512330);
    ((Class_004b4630*)file)->FUN_004b4630("Origin X", DAT_00512320.x);
    ((Class_004b4630*)file)->FUN_004b4630("Origin Z", DAT_00512320.y);
    ((Class_004b4630*)file)->FUN_004b4630("Target X", DAT_00512334.x);
    ((Class_004b4630*)file)->FUN_004b4630("Target Z", DAT_00512334.y);
}
