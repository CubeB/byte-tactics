// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Runs the command in obj: takes its first argument, lowercases it, looks the
// name up in the file-local sorted handler table (the global vector whose
// initialiser is 0x4b75a0 and destructor 0x4b75d0) and calls the handler whose
// mask matches param_2, or else the global default handler registered by
// 0x4b78e0. Returns the mask of the handler that ran, or 0.
//
// The element is declared with plain ints at +4/+8 exactly as 0x4b7ad0.cpp
// does, which is also what fixes the compiler's file-local name for the vector
// (s_commandTable$S4554): only the declarations above the vector affect it, so
// every other class is declared after it. The handler is called through a
// reinterpreted HandlerSlot view of those two fields.
#include <string.h>
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct CommandEntry {
    Class_004c9390 name;               // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    ~CommandEntry() { name.ReleaseRef(); }
};

static std::vector<CommandEntry> s_commandTable;

extern char DAT_005119b8[];
extern int g_defaultCommandHandler;
extern int g_defaultCommandMask;

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};
class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ReleaseRef(); }
};
class Class_004c9290 {
public:
    char* data;                        // +0x0
    Class_004c9290* MakeLower();
};
class CommandArgs {
public:
    char* args[0x34];                  // +0x0
    int count;                         // +0xd0
    char* GetArg(int index, const char* def);
};

typedef void (__stdcall *Handler_004b7900)(void*);

struct HandlerSlot_004b7900 {
    Handler_004b7900 fn;               // +0x4
    int mask;                          // +0x8
};

struct NameLess_004b7900 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

static inline HandlerSlot_004b7900* Find_004b7900(const char* key)
{
    NameLess_004b7900 less;
    CommandEntry* first = s_commandTable.begin();
    CommandEntry* last = s_commandTable.end();
    while (first != last) {
        CommandEntry* mid = first + (last - first) / 2;
        if (less(mid->name.data, key))
            first = mid + 1;
        else
            last = mid;
    }
    if (first == s_commandTable.end() || less(key, first->name.data))
        return 0;
    return (HandlerSlot_004b7900*)&first->value1;
}

// FUNCTION: 0x4b7900
int __stdcall ExecuteCommand(CommandArgs* obj, int param_2)
{
    int result = 0;
    if (obj->count >= 1) {
        Class_004c91b0 key(obj->GetArg(0, DAT_005119b8));
        ((Class_004c9290*)&key)->MakeLower();
        HandlerSlot_004b7900* h = Find_004b7900(key.data);
        if (h != 0 && (h->mask & param_2)) {
            result = h->mask;
            h->fn(obj);
        } else if (g_defaultCommandHandler != 0 && (g_defaultCommandMask & param_2)) {
            result = g_defaultCommandMask;
            ((void (__stdcall *)(void*))g_defaultCommandHandler)(obj);
        }
    }
    return result;
}
