// Decompiled by deepseek-v4.1-flash, finished by GPT-6, space-bunny-free, mimo-v2.6-pro, Space Bunny Free, DeepSeek V4.1 Flash and Claude Opus 5.5. Names are provisional.
// MATCH. The constructor of a .TDF section (its destructor is 0x4c42a0, its
// scalar deleting destructor 0x4c32f0): copies the name, then parses the text
// into `name = value;` entries and `[name] { ... }` sub-sections until the
// closing '}' (nextblock gets the position after it) or the end of the text.
//
// What took it from 70.6% to MATCH (Claude Opus 5.5, #4991): the file was
// rewritten on the real <vector> instead of a hand-made one.
//  * Both vectors are constructed by the header's own constructor, which is
//    the hoisted `mov al, [esp+7]` copy of the allocator temporary, and the
//    sub-section is added with children.push_back(), whose inlined
//    insert(end(), x) keeps &children in ebx for the whole function.
//  * The entries are a sorted vector of key/value handle pairs behind a byte
//    at +0x14 (the same shape as the global map in 0x4c54f0, whose lookup is
//    0x4c5c60). The binary search takes the key's char* by value (so it sits
//    in ebx), and its comparison must be a bool-returning helper,
//    `if (Less(mid->first.ptr, key))`: a plain `_strcmpi(...) < 0` loses the
//    `setl dl` (90.6%), and a named bool local puts lo, hi and mid in the
//    wrong registers (91.3%).
//  * The lookup is written in the body with InsertNew as a member, as in
//    0x4c54f0: `e == end || Ne(e->first, key)` puts the found arm first, and
//    InsertNew's vector::insert(it, x) inlines down to the out-of-line
//    insert(it, 1, x) with the index arithmetic around it. An inline
//    operator[] doing the same is one inline level too deep (91.8%).
//  * `strchr(eq + 1, ';')` rather than `strchr(current, ';')` after
//    `current = eq + 1` is the original's `lea eax, [esi+1]` (82.0% to 91.3%).
//
// The name: data/symbols.csv calls 0x4c3e40 Class_004c42a0::FUN_004c3e40 (from
// the callers 0x4c2f60 and 0x4c3120, which call it on operator new's result),
// but it is a constructor, and 0x4c4d70's symbol names the children's element
// type Class_004c3e40, so the class has to be called that here for the
// push_back call to resolve. Class_004c42a0 and Class_004c3e40 are one class.
#include <vector>
#include <string.h>
#include <stdio.h>

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

class Class_004c91a0;

class Class_004c93b0 {
public:
    char* ptr;

    Class_004c93b0* Assign(const Class_004c91a0* other);
};

// A reference-counted string handle (0x4c91a0 copies, 0x4c9390 releases,
// 0x4c93b0 assigns).
class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0() {}
    Class_004c91a0(const Class_004c91a0& other);
    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
    Class_004c91a0& operator=(const Class_004c91a0& other)
    {
        ((Class_004c93b0*)this)->Assign(&other);
        return *this;
    }
};

// The empty handle.
class Class_004c9180 : public Class_004c91a0 {
public:
    Class_004c9180();
};

// One entry of a section: a key and a value.
class Class_004c54a0 {
public:
    Class_004c91a0 first;                  // +0x0
    Class_004c91a0 second;                 // +0x4

    Class_004c54a0(const Class_004c54a0& other);
};

class Class_004c54d0 : public Class_004c54a0 {
public:
    Class_004c54d0(const Class_004c91a0& a, const Class_004c91a0& b);
};

static inline Class_004c54d0 MakePair(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return Class_004c54d0(a, b);
}

static inline bool operator==(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return strcmp(a.ptr, b.ptr) == 0;
}

static inline bool Ne(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return !(a == b);
}

static inline bool Less(const char* a, const char* b)
{
    return _strcmpi(a, b) < 0;
}

#pragma pack(push, 1)
// The entries of a section, kept sorted by key.
class Map_004c3e40 {
public:
    char less;                             // +0x0
    std::vector<Class_004c54a0> v;         // +0x1

    Class_004c54a0* LowerBound(const char* key)
    {
        Class_004c54a0* first = v.begin();
        Class_004c54a0* last = v.end();
        while (first != last) {
            Class_004c54a0* mid = first + (last - first) / 2;
            if (Less(mid->first.ptr, key))
                first = mid + 1;
            else
                last = mid;
        }
        return first;
    }

    Class_004c91a0* InsertNew(Class_004c54a0* e, const Class_004c91a0& key)
    {
        return &v.insert(e, MakePair(key, Class_004c9180()))->second;
    }
};

class Class_004c4340 {
public:
    Class_004c91a0 FUN_004c4340(char* start, char* end);
};

// A section of a .TDF file: its name, its sub-sections and its entries.
class Class_004c3e40 {
public:
    char* name;                            // +0x0
    std::vector<Class_004c3e40*> children; // +0x4
    Map_004c3e40 entries;                  // +0x14
    char* text;                            // +0x25

    Class_004c3e40(char* name, char* text, char** nextblock, char* filename);
};
#pragma pack(pop)

char* __cdecl GameStrdup(char* text);
char* FUN_004b6ba0(char* text, int len);
void FatalError(char* text);

static inline char* SkipSpace(char* p)
{
    while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n'))
        p++;
    return p;
}

// FUNCTION: 0x4c3e40
Class_004c3e40::Class_004c3e40(char* name, char* text, char** nextblock, char* filename)
{
    char error[2000] = "Parse error in .TDF File! ";
    this->name = GameStrdup(name);
    char* current = text;
    for (;;) {
        current = SkipSpace(current);
        switch (*current) {
        case '[': {
            char* close = strchr(current, ']');
            if (!close) {
                strcat(error, "Sub-record - closing ']' not found");
                goto fail;
            }
            Class_004c91a0 subname = ((Class_004c4340*)this)->FUN_004c4340(current + 1, close);
            current = SkipSpace(close + 1);
            if (*current != '{') {
                strcat(error, "Sub-record - opening '{' not found");
                goto fail;
            }
            children.push_back(new Class_004c3e40(subname.ptr, current + 1, &current, filename));
            break;
        }
        case '}': {
            if (nextblock)
                *nextblock = current + 1;
            char* end = current - 1;
            if (text <= end)
                this->text = FUN_004b6ba0(text, end - text - 1);
            else
                this->text = 0;
            return;
        }
        case 0: {
            if (nextblock) {
                strcat(error, "End of file - nextblock not zero");
                goto fail;
            }
            char* end = current - 1;
            if (text <= end)
                this->text = FUN_004b6ba0(text, end - text - 1);
            else
                this->text = 0;
            return;
        }
        default: {
            char* eq = strchr(current, '=');
            if (!eq) {
                strcat(error, "Data field - '=' not found");
                goto fail;
            }
            Class_004c91a0 key = ((Class_004c4340*)this)->FUN_004c4340(current, eq);
            current = eq + 1;
            char* semi = strchr(eq + 1, ';');
            if (!semi) {
                strcat(error, "Data field - ';' not found");
                goto fail;
            }
            Class_004c91a0 value = ((Class_004c4340*)this)->FUN_004c4340(current, semi);
            current = semi + 1;
            Class_004c54a0* e = entries.LowerBound(key.ptr);
            Class_004c91a0* r;
            if (e == entries.v.end() || Ne(e->first, key))
                r = entries.InsertNew(e, key);
            else
                r = &e->second;
            *r = value;
            break;
        }
        }
    }
fail:
    if (name)
        sprintf(error + strlen(error), " - name = '%s' from file %s", name, filename);
    FatalError(error);
}
