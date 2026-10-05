// Decompiled by Opus. Names are provisional.
// Console command: looks up the order type named by argument 1 and, when it
// exists, hands it to IssueOrderToSelection with arguments 1 and 2 as numbers.

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

class Class_004b73c0 {
public:
    char* GetArg(int index, char* fallback);
};

// Command arguments.
class CommandArgs {
public:
    int GetIntArg(int index, int fallback);
};

extern char DAT_005119b8[];
extern char* g_game;

void __stdcall IssueOrderToSelection(void* a, int b, Class_00438760 kind, int d, int e, int f);

// FUNCTION: 0x416310
void __stdcall CmdAssign(CommandArgs* args)
{
    Class_00438760 kind(((Class_004b73c0*)args)->GetArg(1, DAT_005119b8));
    if (kind.index) {
        int a = args->GetIntArg(1, 0);
        int b = args->GetIntArg(2, 0);
        IssueOrderToSelection(g_game + 0x2c76, 0, kind, 0, a, b);
    }
}
