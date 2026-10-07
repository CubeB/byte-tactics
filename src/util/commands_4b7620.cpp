// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by deepseek-v4.1-flash. Names are provisional.
// Registers one named handler in
// the file-local sorted handler table (created by 0x4b75a0, destroyed by
// 0x4b7ad0). It binary-searches the table case-insensitively with _strcmpi for
// the name, and if the exact-case name is not present (a plain strcmp of the
// element tells) inserts a new 12-byte element, then stores the handler pointer
// and its mask into the element. Called by 0x406f00 with "plan", "weight" and
// "limit". This is the out-of-line form of the inner body of 0x4b7760.
#include <string.h>
#include <vector>

class Class_004c91a0 {
public:
    char* data;                        // +0x0
    Class_004c91a0(const Class_004c91a0& other);
};

struct Pair_004b7620;

typedef int Fwd_004b7620;

class CommandEntry {
public:
    Class_004c91a0 handle;             // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    CommandEntry(const Class_004c91a0& h, Pair_004b7620 pp);
    ~CommandEntry();
};

static std::vector<CommandEntry> s_commandTable;

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

static inline bool operator==(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return strcmp(a.data, b.data) == 0;
}

class Class_004c9390 {
public:
    void ReleaseRef();
};

struct Pair_004b7620 {
    int fn;
    int mask;
    Pair_004b7620(int f, int m) : fn(f), mask(m) {}
};

CommandEntry::CommandEntry(const Class_004c91a0& h, Pair_004b7620 pp) : handle(h)
{
    *(Pair_004b7620*)&value1 = pp;
}
CommandEntry::~CommandEntry() { ((Class_004c9390*)&handle)->ReleaseRef(); }

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ((Class_004c9390*)this)->ReleaseRef(); }
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
