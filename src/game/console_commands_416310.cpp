// Decompiled by Opus. Names are provisional.
// Console command: looks up the order type named by argument 1 and, when it
// exists, hands it to FUN_0048cf30 with arguments 1 and 2 as numbers.

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

class Class_004b73c0 {
public:
    char* FUN_004b73c0(int index, char* fallback);
};

// Command arguments.
class Class_004b73e0 {
public:
    int FUN_004b73e0(int index, int fallback);
};

extern char DAT_005119b8[];
extern char* g_game;

void __stdcall FUN_0048cf30(void* a, int b, Class_00438760 kind, int d, int e, int f);

// FUNCTION: 0x416310
void __stdcall CmdAssign(Class_004b73e0* args)
{
    Class_00438760 kind(((Class_004b73c0*)args)->FUN_004b73c0(1, DAT_005119b8));
    if (kind.index) {
        int a = args->FUN_004b73e0(1, 0);
        int b = args->FUN_004b73e0(2, 0);
        FUN_0048cf30(g_game + 0x2c76, 0, kind, 0, a, b);
    }
}
