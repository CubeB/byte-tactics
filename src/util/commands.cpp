// Decompiled by Haiku, space-bunny-free, Opus, deepseek-v4.1-flash, GPT-6.1-sol, mimo-v2.6-pro and Sonnet 5.5. Names are provisional.
//
// Command arguments (%N), the named command handler table and the command
// dispatch. The static vector's symbol id (the $S suffix of
// s_commandTable$S4554, the name data/symbols.csv records for it) counts the
// declarations before it: <vector>, the forward declaration and the three
// declarations below put it at 4554, and the other headers go after it so
// they do not move that id.

#include <vector>

class CommandEntry;

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct Pair_004b7620 {
    int fn;
    int mask;
    Pair_004b7620(int f, int m) : fn(f), mask(m) {}
};

int __cdecl atoi(const char* str);

// The file-local sorted handler table. An element is 12 bytes: a
// reference-counted string handle and two ints; its destructor releases the
// handle through ReleaseRef. The compiler generates the initialiser
// (0x4b75a0) and the destructor it registers with atexit (0x4b75d0).
// FUNCTION: 0x4b75a0 _$E5
// FUNCTION: 0x4b75d0 _$E3
static std::vector<CommandEntry> s_commandTable;

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

extern int g_defaultCommandHandler;
extern int g_defaultCommandMask;
extern char DAT_005119b8[];

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

static inline bool operator==(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return strcmp(a.data, b.data) == 0;
}

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

struct NameLess_004b7620 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

struct NameNe_004b7620 {
    bool operator()(const Class_004c91a0& a, const Class_004c91a0& b) const
    {
        return !(a == b);
    }
};

typedef void (__stdcall *Handler_004b7620)(void*);

struct ConsoleCommand {
    const char* name;                  // +0x0
    Handler_004b7620 fn;               // +0x4
    int mask;                          // +0x8
};

struct Class_004c93b0 {
    void Assign(int);
};

class CommandEntry {
public:
    Class_004c91a0 handle;             // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    CommandEntry(const Class_004c91a0& h, Pair_004b7620 pp);
    CommandEntry(const CommandEntry& other);
    CommandEntry& operator=(const CommandEntry& other)
    {
        AssignEntry((int*)&other);
        return *this;
    }
    ~CommandEntry() { handle.ReleaseRef(); }
    void* AssignEntry(int* param_1);
};

CommandEntry::CommandEntry(const Class_004c91a0& h, Pair_004b7620 pp) : handle(h)
{
    *(Pair_004b7620*)&value1 = pp;
}

class CommandArgs {
public:
    char* args[0x14];                  // +0x00 token pointers
    char buffer[0x7e];                 // +0x50 the tokens' text
    char unknown_ce[2];
    int count;                         // +0xd0 token count

    void Tokenize(char* text, char* end);
    CommandArgs* InitArgs();
    int GetArg(int index, int default_val);
    int GetIntArg(int index, int default_val);
    float GetFloatArg(int index, float default_val);
    void ShiftArgs(int n);
};

// Command arguments: replaces each "%N" argument with argument N of another
// list. The same 0xd4 bytes seen with a flat token array.
class Class_004b74f0 {
public:
    char* args[0x34];                  // +0x00
    int count;                         // +0xd0

    // Inline copy of the range-checked getter at 0x4b73c0; its redundant
    // range check folds away but the extra use of `other` decides registers.
    char* GetArg(int index, char* def)
    {
        if (index < 0 || index >= count) return def;
        return args[index];
    }

    void SubstituteArgs(Class_004b74f0* other);
};

// The original calls these out of line from ExecuteCommand (GetArg) and
// ExecuteCommandText (InitArgs, Tokenize); in this file /Ob2 would inline
// them into the callers.
#pragma auto_inline(off)

// FUNCTION: 0x4b73b0
CommandArgs* CommandArgs::InitArgs()
{
    count = 0;
    return this;
}

// FUNCTION: 0x4b73c0
int CommandArgs::GetArg(int index, int default_val)
{
    if (index < 0 || index >= count) {
        return default_val;
    }
    return (int)args[index];
}

// FUNCTION: 0x4b73e0
int CommandArgs::GetIntArg(int index, int default_val)
{
    if (index < 0 || index >= count) {
        return default_val;
    }
    const char* str = args[index];
    return atoi(str);
}

// FUNCTION: 0x4b7410
float CommandArgs::GetFloatArg(int index, float default_val)
{
    if (index < 0 || index >= count) {
        return default_val;
    }
    char* str = args[index];
    return (float)atof(str);
}

// FUNCTION: 0x4b7440
void CommandArgs::Tokenize(char* text, char* end)
{
    char* p = buffer;

    if (end == 0)
        end = text + strlen(text);

    count = 0;

    while (1) {
        while (text != end && isspace(*text))
            text++;

        if (text == end)
            return;

        if (*text == '#')
            return;

        if (count < 0x14) {
            args[count] = p;
            count++;
        }

        while (text != end) {
            if (isspace(*text))
                break;
            if (*text == '#')
                break;
            if (p >= &buffer[0x7e])
                break;
            *p = *text;
            p++;
            text++;
        }

        *p = 0;
        p++;
    }
}

// FUNCTION: 0x4b7540
void CommandArgs::ShiftArgs(int n)
{
    if (count >= n) {
        count = 0;
        return;
    }
    char** end = &args[count];
    char** p = &args[n];
    char** dst = args;
    while (p != end)
        *dst++ = *p++;
    count -= n;
}

// The original calls this out of line from ExecuteCommandText.
#pragma auto_inline(off)
// FUNCTION: 0x4b74f0
void Class_004b74f0::SubstituteArgs(Class_004b74f0* other)
{
    for (int i = 0; i < count; i++) {
        if (*args[i] == '%') {
            int n = atoi(args[i] + 1);
            if (n >= 0 && n < other->count) {
                args[i] = other->GetArg(n, 0);
            }
        }
    }
}

#pragma auto_inline(on)

// FUNCTION: 0x4b7590
void ClearDefaultCommandHandler()
{
    g_defaultCommandHandler = 0;
    g_defaultCommandMask = 0;
}

// Registers one named handler in the file-local sorted handler table. It
// binary-searches the table case-insensitively with _strcmpi for the name, and
// if the exact-case name is not present (a plain strcmp of the element tells)
// inserts a new 12-byte element, then stores the handler pointer and its mask
// into the element. Called by 0x406f00 with "plan", "weight" and "limit". This
// is the out-of-line form of the inner body of 0x4b7760.
// FUNCTION: 0x4b7620
void __stdcall RegisterCommand(const char* name, Handler_004b7620 fn, int mask)
{
    // Built before key: the tail copies the pair from this local.
    Pair_004b7620 p((int)fn, mask);
    Class_004c91b0 key(name);
    CommandEntry* first = s_commandTable.begin();
    CommandEntry* last = s_commandTable.end();
    NameLess_004b7620 less;
    const char* k = key.data;
    while (first != last) {
        CommandEntry* mid = first + (last - first) / 2;
        if (less(mid->handle.data, k))
            first = mid + 1;
        else
            last = mid;
    }
    // int* slot: the original stores through [esi]/[esi+4].
    int* slot;
    if (first == s_commandTable.end() || NameNe_004b7620()(first->handle, key)) {
        CommandEntry e(key, Pair_004b7620(0, 0));
        int index = first - s_commandTable.begin();
        s_commandTable.insert(first, e);
        slot = &(s_commandTable.begin() + index)->value1;
    } else {
        slot = &first->value1;
    }
    *(Pair_004b7620*)slot = p;
}

// FUNCTION: 0x4b7760
void __stdcall RegisterCommands(ConsoleCommand* rec)
{
    // for, not while: puts the record increment after the key destructor.
    for (; rec->name; rec++) {
        int mask = rec->mask;
        Handler_004b7620 fn = rec->fn;
        Class_004c91b0 key(rec->name);
        CommandEntry* first = s_commandTable.begin();
        CommandEntry* last = s_commandTable.end();
        NameLess_004b7620 less;
        const char* k = key.data;
        while (first != last) {
            CommandEntry* mid = first + (last - first) / 2;
            if (less(mid->handle.data, k))
                first = mid + 1;
            else
                last = mid;
        }
        // int* slot: the original stores through [esi]/[esi+4].
        int* slot;
        if (first == s_commandTable.end() || NameNe_004b7620()(first->handle, key)) {
            CommandEntry e(key, Pair_004b7620(0, 0));
            int index = first - s_commandTable.begin();
            s_commandTable.insert(first, e);
            slot = &(s_commandTable.begin() + index)->value1;
        } else {
            slot = &first->value1;
        }
        slot[0] = (int)fn;
        slot[1] = mask;
    }
}

// FUNCTION: 0x4b78e0
void __stdcall SetDefaultCommandHandler(int param_1, int param_2)
{
    g_defaultCommandHandler = param_1;
    g_defaultCommandMask = param_2;
}

struct HandlerSlot_004b7900 {
    Handler_004b7620 fn;               // +0x4
    int mask;                          // +0x8
};

static inline HandlerSlot_004b7900* Find_004b7900(const char* key)
{
    NameLess_004b7620 less;
    CommandEntry* first = s_commandTable.begin();
    CommandEntry* last = s_commandTable.end();
    while (first != last) {
        CommandEntry* mid = first + (last - first) / 2;
        if (less(mid->handle.data, key))
            first = mid + 1;
        else
            last = mid;
    }
    if (first == s_commandTable.end() || less(key, first->handle.data))
        return 0;
    return (HandlerSlot_004b7900*)&first->value1;
}

// Runs the command in obj: takes its first argument, lowercases it, looks the
// name up in the file-local sorted handler table and calls the handler whose
// mask matches param_2, or else the global default handler registered by
// 0x4b78e0. Returns the mask of the handler that ran, or 0.
//
// The handler is called through a reinterpreted HandlerSlot view of the two
// ints at +4/+8 of the element.
// The original calls this out of line from ExecuteCommandText.
#pragma auto_inline(off)
// FUNCTION: 0x4b7900
int __stdcall ExecuteCommand(CommandArgs* obj, int param_2)
{
    int result = 0;
    if (obj->count >= 1) {
        Class_004c91b0 key((char*)obj->GetArg(0, (int)DAT_005119b8));
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

#pragma auto_inline(on)

// The newline search at 0x4b7a10 (the function just before this one in the
// original file), defined here so /Ob2 inlines it as the original did.
// FUNCTION: 0x4b7a10
int __stdcall FindLineEnd(char* param_1, int param_2)
{
    for (int i = 0; i < param_2; i++) {
        if (param_1[i] == '\n') {
            return i;
        }
    }
    return param_2;
}

// Runs a block of command lines: splits the text at newlines, parses each line
// into a command object (Tokenize), substitutes "%N" arguments from vars
// (SubstituteArgs) and executes it (ExecuteCommand); returns the OR of the results.
// FUNCTION: 0x4b7a30
int __stdcall ExecuteCommandText(char* text, int len, Class_004b74f0* vars, int param_4)
{
    int result = 0;
    Class_004b74f0 cmd;
    ((CommandArgs*)&cmd)->InitArgs();
    while (len > 0) {
        int n = FindLineEnd(text, len);
        if (n > len)
            break;
        char* end = text + n;
        ((CommandArgs*)&cmd)->Tokenize(text, end);
        cmd.SubstituteArgs(vars);
        result |= ExecuteCommand((CommandArgs*)&cmd, param_4);
        text = end + 1;
        len -= n + 1;
    }
    return result;
}

// FUNCTION: 0x4b7ac0
void NopRet(void)
{
}

// Empties the file-local global vector created by 0x4b75a0 (destroyed by
// 0x4b75d0): the inlined vector::clear() runs each element's destructor.
// FUNCTION: 0x4b7ad0
void ClearCommandTable()
{
    s_commandTable.clear();
}

// std::vector<CommandEntry>::insert(iterator, size_type, const T&), out of
// line: 12-byte elements holding a reference-counted string handle and two
// ints. The copy constructor is 0x4b7e30 and the assignment 0x4b7e00; the
// destructor releases the handle through 0x4c9390.
typedef std::vector<CommandEntry> Vec_004b7b00;
typedef void (Vec_004b7b00::*InsertFn_004b7b00)(
    Vec_004b7b00::iterator, Vec_004b7b00::size_type, const CommandEntry&);

// Taking insert's address is what emits the template instantiation.
// FUNCTION: 0x4b7b00 ?insert@?$vector@VCommandEntry@@V?$allocator@VCommandEntry@@@std@@@std@@QAEXPAVCommandEntry@@IABV3@@Z
InsertFn_004b7b00 g_insert_004b7b00 = &Vec_004b7b00::insert;

// FUNCTION: 0x4b7e00
void* CommandEntry::AssignEntry(int* param_1)
{
    ((Class_004c93b0*)&handle)->Assign((int)param_1);
    value1 = param_1[1];
    value2 = param_1[2];
    return this;
}

// Copy constructor of a {string handle, int, int} record.
// FUNCTION: 0x4b7e30 ??0CommandEntry@@QAE@ABV0@@Z
CommandEntry::CommandEntry(const CommandEntry& other)
    : handle(other.handle), value1(other.value1), value2(other.value2)
{
}
