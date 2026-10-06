// Decompiled by deepseek-v4.1-flash, GPT-6, space-bunny-free, mimo-v2.6-pro, Claude Opus 5.5, Sonnet, Haiku and Opus. Names are provisional.

#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

void __cdecl FUN_004d85a0(void* p);

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

#pragma pack(pop)

class Class_004c4340 {
public:
    Class_004c91a0 MakeTrimmedString(char* start, char* end);
};

char* __cdecl GameStrdup(char* text);
char* ComputeChecksum(char* text, int len);
void FatalError(char* text);

static inline char* SkipSpace(char* p)
{
    while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n'))
        p++;
    return p;
}

#pragma pack(push, 1)
// A .TDF section: `name = value;` entries kept sorted by name
// (case-insensitively) and `[name] { ... }` sub-sections.
class TdfRecord {
public:
    char* name;                             // +0x00
    std::vector<TdfRecord*> children;       // +0x04
    Map_004c3e40 entries;                   // +0x14
    char* text;                             // +0x25

    TdfRecord(char* name, char* text, char** nextblock, char* filename);
    ~TdfRecord();
    void ComputeRecordChecksum(char* start, char* end);
    void CopyRecordName(char* dest, size_t count);
    TdfRecord* FindSubRecord(const char* name);
    int GetFieldCount();
    char* GetFieldName(int index);
    int FindFieldValue(char* name);
    int GetFieldInt(const char* name, int def);
    double GetFieldDouble(const char* name, double def);
    int* GetFieldFixed(int* dst, char* key, int def);
    int GetFieldString(char* dst, char* key, size_t size, char* def);
};
#pragma pack(pop)

// The constructor of a .TDF section: copies the name, then parses the text
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

// FUNCTION: 0x4c3e40
TdfRecord::TdfRecord(char* name, char* text, char** nextblock, char* filename)
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
            Class_004c91a0 subname = ((Class_004c4340*)this)->MakeTrimmedString(current + 1, close);
            current = SkipSpace(close + 1);
            if (*current != '{') {
                strcat(error, "Sub-record - opening '{' not found");
                goto fail;
            }
            children.push_back(new TdfRecord(subname.ptr, current + 1, &current, filename));
            break;
        }
        case '}': {
            if (nextblock)
                *nextblock = current + 1;
            char* end = current - 1;
            if (text <= end)
                this->text = ComputeChecksum(text, end - text - 1);
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
                this->text = ComputeChecksum(text, end - text - 1);
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
            Class_004c91a0 key = ((Class_004c4340*)this)->MakeTrimmedString(current, eq);
            current = eq + 1;
            char* semi = strchr(eq + 1, ';');
            if (!semi) {
                strcat(error, "Data field - ';' not found");
                goto fail;
            }
            Class_004c91a0 value = ((Class_004c4340*)this)->MakeTrimmedString(current, semi);
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

// Frees the name, deletes the sub-sections, and lets the entries vector (pairs
// of reference-counted string handles) destroy its elements itself.
// FUNCTION: 0x4c42a0
TdfRecord::~TdfRecord()
{
    if (name)
        FUN_004d85a0(name);
    for (std::vector<TdfRecord*>::iterator p = children.begin(); p < children.end(); p++)
        delete *p;
}

// FUNCTION: 0x4c43f0
void TdfRecord::ComputeRecordChecksum(char* start, char* end)
{
    if (start <= end) {
        text = ComputeChecksum(start, end - start - 1);
    } else {
        text = 0;
    }
}

// FUNCTION: 0x4c4420
void TdfRecord::CopyRecordName(char* dest, size_t count)
{
    strncpy(dest, name, count);
}

// FUNCTION: 0x4c4470
TdfRecord* TdfRecord::FindSubRecord(const char* name)
{
    for (std::vector<TdfRecord*>::iterator p = children.begin(); p < children.end(); p++) {
        if (_strcmpi((*p)->name, name) == 0)
            return *p;
    }
    return 0;
}

// FUNCTION: 0x4c45c0
int TdfRecord::GetFieldCount()
{
    return entries.v.size();
}

// FUNCTION: 0x4c45e0
char* TdfRecord::GetFieldName(int index)
{
    if (index < 0 || (unsigned int)index >= entries.v.size())
        return 0;
    unsigned int i = 0;
    for (std::vector<Class_004c54a0>::iterator p = entries.v.begin(); p < entries.v.end(); p++, i++) {
        if (i == (unsigned int)index)
            return p->first.ptr;
    }
    return 0;
}

struct NameLess {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

static inline Class_004c54a0* LowerBound(Class_004c54a0* first, Class_004c54a0* last, const char* name)
{
    NameLess less;
    while (first != last) {
        Class_004c54a0* mid = first + (last - first) / 2;
        if (less(mid->first.ptr, name))
            first = mid + 1;
        else
            last = mid;
    }
    return first;
}

static inline char** Find(TdfRecord* table, const char* name)
{
    NameLess less;
    Class_004c54a0* it = LowerBound(table->entries.v.begin(), table->entries.v.end(), name);
    if (it == table->entries.v.end() || less(name, it->first.ptr))
        return 0;
    return &it->second.ptr;
}

static inline char* GetString(TdfRecord* table, const char* name)
{
    char** p = Find(table, name);
    if (p)
        return *p;
    return 0;
}

// FUNCTION: 0x4c4630
int TdfRecord::FindFieldValue(char* name)
{
    char** p = Find(this, name);
    if (p)
        return (int)*p;
    return 0;
}

// Returns the named field as an int, or a default.
// FUNCTION: 0x4c46c0
int TdfRecord::GetFieldInt(const char* name, int def)
{
    char* s = GetString(this, name);
    if (s)
        return atoi(s);
    return def;
}

// Returns the named field as a double, or a default.
// FUNCTION: 0x4c4760
double TdfRecord::GetFieldDouble(const char* name, double def)
{
    char* s = GetString(this, name);
    if (s)
        return atof(s);
    return def;
}

// FUNCTION: 0x4c4800
int* TdfRecord::GetFieldFixed(int* dst, char* key, int def)
{
    Class_004c54a0* lo = entries.v.begin();
    Class_004c54a0* hi = entries.v.end();
    while (lo != hi) {
        Class_004c54a0* mid = lo + (hi - lo) / 2;
        if (Less(mid->first.ptr, key)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    char** p;
    if (lo == entries.v.end() || Less(key, lo->first.ptr)) {
        p = 0;
    } else {
        p = &lo->second.ptr;
    }
    char* value = p ? *p : 0;
    if (value) {
        *dst = (int)(atof(value) * 65536.0);
        return dst;
    }
    *dst = def;
    return dst;
}

// FUNCTION: 0x4c48c0
int TdfRecord::GetFieldString(char* dst, char* key, size_t size, char* def)
{
    Class_004c54a0* lo = entries.v.begin();
    Class_004c54a0* hi = entries.v.end();
    while (lo != hi) {
        Class_004c54a0* mid = lo + (hi - lo) / 2;
        if (Less(mid->first.ptr, key)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    char** p;
    if (lo == entries.v.end() || Less(key, lo->first.ptr)) {
        p = 0;
    } else {
        p = &lo->second.ptr;
    }
    char* value = p ? *p : 0;
    if (value) {
        strncpy(dst, value, size);
        dst[size - 1] = 0;
        return 1;
    }
    if (def) {
        strcpy(dst, def);
    }
    return 0;
}

// The scalar deleting destructor (0x4c32f0) inlines the destructor (each
// delete inside it goes back through itself, the inline budget being spent).
// The holder's destructor exists only to make the compiler emit it.
class Holder_004c32f0 {
public:
    TdfRecord* root;
    int field_4;
    int field_8;

    ~Holder_004c32f0();
};

// FUNCTION: 0x4c32f0 ??_GTdfRecord@@QAEPAXI@Z
Holder_004c32f0::~Holder_004c32f0()
{
    delete root;
    root = 0;
    field_4 = 0;
    field_8 = 0;
}
