// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Registers one named handler in the file-local sorted handler table (created
// by 0x4b75a0, destroyed by 0x4b7ad0). It binary-searches the table
// case-insensitively with _strcmpi for the name, and if the exact-case name is
// not present (a plain strcmp of the element tells) inserts a new 12-byte
// element, then stores the handler pointer and its mask into the element.
// Called by 0x406f00 with "plan", "weight" and "limit".
#include <string.h>
#include <vector>

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { FUN_004c9390(); }
};

class Class_004b7e30 {
public:
    Class_004c91a0 handle;             // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004b7e30(const Class_004c91a0& h) : handle(h), field_4(0), field_8(0) {}
    ~Class_004b7e30() { handle.FUN_004c9390(); }
};

static std::vector<Class_004b7e30> DAT_0051fc99;

typedef void (__stdcall *Command_004b7620)(void*);

struct NameLess_004b7620 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

static inline bool NameEq_004b7620(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

// FUNCTION: 0x4b7620
void __stdcall FUN_004b7620(const char* name, Command_004b7620 fn, int flags)
{
    Class_004c91b0 key(name);
    Class_004b7e30* first = DAT_0051fc99.begin();
    Class_004b7e30* last = DAT_0051fc99.end();
    NameLess_004b7620 less;
    const char* k = key.data;
    while (first != last) {
        Class_004b7e30* mid = first + (last - first) / 2;
        if (less(mid->handle.data, k))
            first = mid + 1;
        else
            last = mid;
    }
    if (first == DAT_0051fc99.end() || !NameEq_004b7620(first->handle.data, key.data)) {
        Class_004b7e30 e(key);
        int index = first - DAT_0051fc99.begin();
        DAT_0051fc99.insert(first, e);
        first = DAT_0051fc99.begin() + index;
    }
    int* slot = &first->field_4;
    *slot = (int)fn;
    slot[1] = flags;
}
