// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
#include <string.h>
#include <vector>

class Class_004c91a0 {
public:
    char* data;                        // +0x0
    Class_004c91a0(const Class_004c91a0& other);
};

struct Pair_004b7760;

// Standalone typedef: pads the declaration counter so the vector below
// mangles as s_commandTable$S4554.
typedef int Fwd_004b7760;

class CommandEntry {
public:
    Class_004c91a0 handle;             // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    CommandEntry(const Class_004c91a0& h, Pair_004b7760 pp);
    ~CommandEntry();
};

static std::vector<CommandEntry> s_commandTable;

// Declared after the vector: a declaration before it moves the $S number.
extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

static inline bool operator==(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return strcmp(a.data, b.data) == 0;
}

class Class_004c9390 {
public:
    void ReleaseRef();
};

struct Pair_004b7760 {
    int fn;
    int mask;
    Pair_004b7760(int f, int m) : fn(f), mask(m) {}
};

CommandEntry::CommandEntry(const Class_004c91a0& h, Pair_004b7760 pp) : handle(h)
{
    *(Pair_004b7760*)&value1 = pp;
}
CommandEntry::~CommandEntry() { ((Class_004c9390*)&handle)->ReleaseRef(); }

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

struct NameLess_004b7760 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

struct NameNe_004b7760 {
    bool operator()(const Class_004c91a0& a, const Class_004c91a0& b) const
    {
        return !(a == b);
    }
};

typedef void (__stdcall *Handler_004b7760)(void*);

struct ConsoleCommand {
    const char* name;                  // +0x0
    Handler_004b7760 fn;               // +0x4
    int mask;                          // +0x8
};

// FUNCTION: 0x4b7760
void __stdcall RegisterCommands(ConsoleCommand* rec)
{
    // for, not while: puts the record increment after the key destructor.
    for (; rec->name; rec++) {
        int mask = rec->mask;
        Handler_004b7760 fn = rec->fn;
        Class_004c91b0 key(rec->name);
        CommandEntry* first = s_commandTable.begin();
        CommandEntry* last = s_commandTable.end();
        NameLess_004b7760 less;
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
        if (first == s_commandTable.end() || NameNe_004b7760()(first->handle, key)) {
            CommandEntry e(key, Pair_004b7760(0, 0));
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
