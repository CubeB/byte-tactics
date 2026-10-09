// Decompiled by space-bunny-free, deepseek-v4.1-flash, Haiku, Opus, LongCat 2.5 Preview Free, Space Bunny Free, Sonnet 5.5, Sonnet, GPT-6.1-sol, deepseek-v4.1, claude-sonnet-5-5, mimo-v2.6-pro, DeepSeek V4.1 Flash, GPT-6 and Claude Opus 5.5. Names are provisional.
//
// TDF files: a text file is loaded into a tree of sections (TdfRecord), each
// with its `name = value;` entries kept sorted by name, and the file object
// (TdfFile) keeps a root and a current section. The std::string, logic_error,
// out_of_range and vector members the compiler emitted for the unit follow
// the code that uses them. The translation tables built on these files
// (0x4c54f0 to 0x4c5d60) are a unit of their own: tdf_4c54f0.cpp. The paths
// that delete the root section (LoadFile, LoadBuffer, and the destructor with
// ??_GTdfField) and the field-vector insert stay in files of their own
// (tdf_4c2f60.cpp, tdf_4c3120.cpp, tdf_4c51b0.cpp and tdf_4c59d0.cpp): no one
// view of TdfRecord, and not the real <vector>, gives all of them.

#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// Included only for its symbol count: the std::string members below match at one window of it.
#include <float.h>

// The string handle: a reference-counted char* (0x4c9180 builds the empty
// one, 0x4c91a0 copies, 0x4c91b0 builds from text, 0x4c9390 releases,
// 0x4c93b0 assigns).
class Class_004c91a0;

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

class Class_004c93b0 {
public:
    char* ptr;

    Class_004c93b0* Assign(const Class_004c91a0* other);
};

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

// A handle built from text.
class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
};

// One entry of a section or of the translations: a key and a value. The
// compiler's destructor of the pair (0x4c5190) releases both handles.
// FUNCTION: 0x4c5190 ??1TdfField@@QAE@XZ
struct TdfField {
    Class_004c91a0 key;                  // +0x0
    Class_004c91a0 value;                // +0x4

    TdfField(const Class_004c91a0& a, const Class_004c91a0& b);
    TdfField(const TdfField& other);
    TdfField& operator=(const TdfField& other)
    {
        AssignPair((int*)&other);
        return *this;
    }
    void* AssignPair(int* param_1);
};

// The vector's destroy loop calls the pair's destructor directly.
namespace std {
inline void _Destroy(TdfField* p)
{
    p->~TdfField();
}
}

extern char DAT_005119b8[];
void __cdecl GameFreeThunk(void* p);
char* __cdecl GameStrdup(char* text);
char* ComputeChecksum(char* text, int len);
void FatalError(char* text);

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

static inline TdfField MakePair(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return TdfField(a, b);
}

#pragma pack(push, 1)
// The entries of a section, kept sorted by key.
class Map_004c3e40 {
public:
    char less;                             // +0x0
    std::vector<TdfField> v;         // +0x1

    TdfField* LowerBound(const char* key)
    {
        TdfField* first = v.begin();
        TdfField* last = v.end();
        while (first != last) {
            TdfField* mid = first + (last - first) / 2;
            if (Less(mid->key.ptr, key))
                first = mid + 1;
            else
                last = mid;
        }
        return first;
    }

    Class_004c91a0* InsertNew(TdfField* e, const Class_004c91a0& key)
    {
        return &v.insert(e, MakePair(key, Class_004c9180()))->value;
    }
};

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

    Class_004c91a0 MakeTrimmedString(char* start, char* end);

    // TdfRecord::FindSubRecord (0x4c4470), inlined.
    TdfRecord* FindChild(char* key)
    {
        for (TdfRecord** p = children.begin(); p < children.end(); p++) {
            if (_strcmpi((*p)->name, key) == 0)
                return *p;
        }
        return 0;
    }

    // The extra inline level keeps logic_error's constructor out of line.
    TdfRecord* GetChild(int index)
    {
        return index >= children.size() ? 0 : children.at(index);
    }

    void ComputeRecordChecksum(char* start, char* end);
    void CopyRecordName(char* dest, size_t count);
    int GetRecordName();
    TdfRecord* FindSubRecord(const char* name);
    int GetSubRecordCount();
    TdfRecord* GetSubRecord(int index);
    int GetFieldCount();
    char* GetFieldName(int index);
    int FindFieldValue(char* name);
    int GetFieldInt(const char* name, int def);
    double GetFieldDouble(const char* name, double def);
    int* GetFieldFixed(int* dst, char* key, int def);
    int GetFieldString(char* dst, char* key, size_t size, char* def);
};
#pragma pack(pop)

// Defined ahead of its place in the address order so that the deleting
// destructor in Unload can inline it.
// Unused here: the symbol ids these declarations take keep the allocation,
// standing in for the TdfRecord view merged into TdfRecord above
// (docs/c2-regalloc.md).
void EnableAICommands(void);

// Unused here: the symbol ids these declarations take keep the allocation
// after the TdfField join (docs/c2-regalloc.md).
void CountMessage(unsigned char, int, int);
void CountPacket(int, int, int);
void SetCameraPosition(int, int, int);
void StartScreenShake(int, int, int);
void AccumulateScreenShake(int, int, int);
void CenterCameraOnPoint(int, int, int);
void SetMissionStatus(int, int, int);
void StartFeatureBurning(int, int, int);
void SetGameSpeed(int, int);
// FUNCTION: 0x4c42a0
TdfRecord::~TdfRecord()
{
    if (name)
        GameFreeThunk(name);
    for (std::vector<TdfRecord*>::iterator p = children.begin(); p < children.end(); p++)
        delete *p;
}

// Handles the text at p (a // comment, a /* */ comment, or one ordinary
// character) and returns where to continue.
static inline char* SkipComment(char* p)
{
    if (p[0] == '/' && p[1] == '/') {
        while (1) {
            if (*p == '\n')
                break;
            *p = ' ';
            p++;
            if (*p == 0)
                break;
        }
    } else if (p[0] == '/' && p[1] == '*') {
        p[1] = ' ';
        p[0] = ' ';
        p += 2;
        while (*p) {
            if (p[-1] == '*' && p[0] == '/') {
                p[0] = ' ';
                p[-1] = ' ';
                p++;
                break;
            }
            p[-1] = ' ';
            p++;
        }
    } else {
        p++;
    }
    return p;
}

class TdfFile {
public:
    TdfRecord* root;                   // +0x0
    TdfRecord* current;                // +0x4
    int field_8;                       // +0x8

    // The functions that delete the root section each destroy its entries a
    // different way (inline, through ReleasePair, ~TdfField or
    // ??_GTdfField, as the inline budget runs out), and no one view of the
    // section gives all of them: those stay in files of their own with the
    // view each needs. They are the destructor (0x4c2eb0, with ??_GTdfField
    // in tdf_4c51b0.cpp), LoadFile (tdf_4c2f60.cpp), LoadBuffer
    // (tdf_4c3120.cpp) and ??_ETdfFile (data_files_42a870.cpp).
    TdfFile();
    ~TdfFile();
    int LoadFile(char* path);
    void StripComments(char* p);
    void LoadBuffer(char* data, int size, int flag, char* path);
    void Unload();
    int SelectRecord(char* name);
    int SelectRecordAt(int index);
    void ResetCurrentRecord();
    int GetCurrentRecord();
    void SetCurrentRecord(int val);
};

// Default constructor of a 12-byte class; its destructor is 0x4c2eb0.
// FUNCTION: 0x4c2ea0
TdfFile::TdfFile()
{
    root = 0;
    current = 0;
    field_8 = 0;
}

// Frees the TDF section tree hanging off root and zeroes the 12-byte object,
// like TdfFile's destructor (0x4c2eb0, in tdf_4c51b0.cpp), except that the
// entries are destroyed by the element destructor (0x4c5190). The delete
// emits the scalar deleting destructor of TdfRecord (0x4c32f0), which
// inlines the destructor again with the entries' handles released inline.
// FUNCTION: 0x4c32f0 ??_GTdfRecord@@QAEPAXI@Z
// FUNCTION: 0x4c3240
void TdfFile::Unload()
{
    // Explicit null test kept: it gives root the original's register.
    if (root)
        delete root;
    root = 0;
    current = 0;
    field_8 = 0;
}

// Blanks out C and C++ style comments in a text buffer, in place. A method
// that ignores `this`: its callers (0x4c2f60, 0x4c3120) pass their own
// `this` through in ecx.
// FUNCTION: 0x4c33a0
void TdfFile::StripComments(char* p)
{
    while (*p)
        p = SkipComment(p);
}

// Moves the cursor to the child of the current node (or of the root when
// there is none) whose name matches case-insensitively; returns whether
// one was found.
// FUNCTION: 0x4c3410
int TdfFile::SelectRecord(char* name)
{
    TdfRecord* node = current;
    if (node == 0)
        node = root;
    current = node->FindChild(name);
    return current != 0;
}

// Moves the cursor to the child of the current TDF node (or of the root when
// there is none) at the given index. The node's child vector is a
// std::vector<TdfRecord*> at +4 (_First at +8, _Last at +0xc), and the
// out-of-range case throws out_of_range through vector::at() (the literal
// "invalid vector<T> subscript"), calling the logic_error constructor
// (0x4c35c0).
// FUNCTION: 0x4c3490
int TdfFile::SelectRecordAt(int index)
{
    TdfRecord* n = current ? current : root;
    TdfRecord* node = n->GetChild(index);
    current = node;
    return node != 0;
}

// The std::logic_error and std::out_of_range members the compiler emits out
// of line from <stdexcept>, for the std::out_of_range that vector::at() throws.
// FUNCTION: 0x4c35c0 ??0logic_error@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z
// FUNCTION: 0x4c3730 ?what@logic_error@std@@UBEPBDXZ
// FUNCTION: 0x4c3740 ?_Doraise@logic_error@std@@MBEXXZ
// FUNCTION: 0x4c38f0 ??_Glogic_error@std@@UAEPAXI@Z
// FUNCTION: 0x4c3950 ??0logic_error@std@@QAE@ABV01@@Z
// FUNCTION: 0x4c3af0 ?_Doraise@out_of_range@std@@MBEXXZ
// FUNCTION: 0x4c3c60 ??_Gout_of_range@std@@UAEPAXI@Z
// FUNCTION: 0x4c3cc0 ??0out_of_range@std@@QAE@ABV01@@Z

// FUNCTION: 0x4c3e10
void TdfFile::ResetCurrentRecord()
{
    current = 0;
}

// FUNCTION: 0x4c3e20
int TdfFile::GetCurrentRecord()
{
    return (int)current;
}

// FUNCTION: 0x4c3e30
void TdfFile::SetCurrentRecord(int val)
{
    current = (TdfRecord*)val;
}

static inline char* SkipSpace(char* p)
{
    while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n'))
        p++;
    return p;
}

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
//    `if (Less(mid->key.ptr, key))`: a plain `_strcmpi(...) < 0` loses the
//    `setl dl` (90.6%), and a named bool local puts lo, hi and mid in the
//    wrong registers (91.3%).
//  * The lookup is written in the body with InsertNew as a member, as in
//    0x4c54f0: `e == end || Ne(e->key, key)` puts the found arm first, and
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
            Class_004c91a0 subname = MakeTrimmedString(current + 1, close);
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
            Class_004c91a0 key = MakeTrimmedString(current, eq);
            current = eq + 1;
            char* semi = strchr(eq + 1, ';');
            if (!semi) {
                strcat(error, "Data field - ';' not found");
                goto fail;
            }
            Class_004c91a0 value = MakeTrimmedString(current, semi);
            current = semi + 1;
            TdfField* e = entries.LowerBound(key.ptr);
            Class_004c91a0* r;
            if (e == entries.v.end() || Ne(e->key, key))
                r = entries.InsertNew(e, key);
            else
                r = &e->value;
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

// Builds a reference-counted string handle (see 0x4c91b0 for the constructor
// from a C string, 0x4c91a0 for the copy constructor, 0x4c9390 for the
// destructor) out of the text between start and end, with leading and trailing
// whitespace trimmed.

// FUNCTION: 0x4c4340
Class_004c91a0 TdfRecord::MakeTrimmedString(char* start, char* end)
{
    char* p = start;
    while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n'))
        p++;
    start = p;
    if (start >= end) {
        Class_004c91b0 empty(DAT_005119b8);
        return empty;
    }
    char* last = end - 1;
    while (*last == ' ' || *last == '\t' || *last == '\r' || *last == '\n')
        last--;
    char saved = last[1];
    last[1] = 0;
    Class_004c91b0 tmp(start);
    last[1] = saved;
    return tmp;
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

// FUNCTION: 0x4c4440
int TdfRecord::GetRecordName()
{
    return (int)name;
}

// Unused here: classes of the neighbouring files (the string handle's other
// methods, 0x4c90b0 to 0x4c9490, and the translation table), declared for the
// symbol ids that GetRecordName's own class took before it joined TdfRecord.
class Class_004c90b0;
class Class_004c9230;
class Class_004c9290;
class Class_004c9310;
class Class_004c93f0;
class Class_004c9490;
class TranslationTable;

// size() of the std::vector of pointers held at +4 (_First at +8), TdfRecord's
// children.
// FUNCTION: 0x4c4450
int TdfRecord::GetSubRecordCount()
{
    return children.size();
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

// Bounds-checked accessor into the vector of pointers held at +4 (_First at
// +8). The index is checked against size() and 0 is returned when it is out
// of range; otherwise std::vector::at returns the element.
// FUNCTION: 0x4c44c0
TdfRecord* TdfRecord::GetSubRecord(int index)
{
    if ((unsigned int)index >= children.size())
        return 0;
    return children.at(index);
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
    for (std::vector<TdfField>::iterator p = entries.v.begin(); p < entries.v.end(); p++, i++) {
        if (i == (unsigned int)index)
            return p->key.ptr;
    }
    return 0;
}

struct NameLess {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

static inline TdfField* LowerBound(TdfField* first, TdfField* last, const char* name)
{
    NameLess less;
    while (first != last) {
        TdfField* mid = first + (last - first) / 2;
        if (less(mid->key.ptr, name))
            first = mid + 1;
        else
            last = mid;
    }
    return first;
}

static inline char** Find(TdfRecord* table, const char* name)
{
    NameLess less;
    TdfField* it = LowerBound(table->entries.v.begin(), table->entries.v.end(), name);
    if (it == table->entries.v.end() || less(name, it->key.ptr))
        return 0;
    return &it->value.ptr;
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
    TdfField* lo = entries.v.begin();
    TdfField* hi = entries.v.end();
    while (lo != hi) {
        TdfField* mid = lo + (hi - lo) / 2;
        if (Less(mid->key.ptr, key)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    char** p;
    if (lo == entries.v.end() || Less(key, lo->key.ptr)) {
        p = 0;
    } else {
        p = &lo->value.ptr;
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
    TdfField* lo = entries.v.begin();
    TdfField* hi = entries.v.end();
    while (lo != hi) {
        TdfField* mid = lo + (hi - lo) / 2;
        if (Less(mid->key.ptr, key)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    char** p;
    if (lo == entries.v.end() || Less(key, lo->key.ptr)) {
        p = 0;
    } else {
        p = &lo->value.ptr;
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

// The std::basic_string<char> members the compiler's own <xstring> emits for
// this unit. The copy constructor is a game row; the other members
// (0x4c4ac0 to 0x4c50a0) are library rows annotated in
// src/runtime/basic_string.cpp, and _Copy (0x4c4fa0) is the gap region
// src/runtime/basic_string_4c4fa0.cpp.
// FUNCTION: 0x4c49a0 ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z
template class std::basic_string<char, std::char_traits<char>, std::allocator<char> >;

// The out-of-line vector::insert(iterator, size_type, const T&) that the
// section parser's push_back (children, 0x4c4d70) and entry insert (entries,
// 0x4c51e0) call.
// FUNCTION: 0x4c4d70 ?insert@?$vector@PAVTdfRecord@@V?$allocator@PAVTdfRecord@@@std@@@std@@QAEXPAPAVTdfRecord@@IABQAV3@@Z
// FUNCTION: 0x4c51e0 ?insert@?$vector@UTdfField@@V?$allocator@UTdfField@@@std@@@std@@QAEXPAUTdfField@@IABU3@@Z

// Releases the key and the value of an entry.
// FUNCTION: 0x4c5170
void __stdcall ReleasePair(char* param_1)
{
    ((Class_004c9390*)(param_1 + 4))->ReleaseRef();
    ((Class_004c9390*)(param_1))->ReleaseRef();
}

// FUNCTION: 0x4c5470
void* TdfField::AssignPair(int* param_1)
{
    ((Class_004c93b0*)this)->Assign((Class_004c91a0*)param_1);
    ((Class_004c93b0*)((char*)this + 4))->Assign((Class_004c91a0*)(param_1 + 1));
    return this;
}

// Copy constructor of a pair of reference-counted string handles.
// FUNCTION: 0x4c54a0 ??0TdfField@@QAE@ABU0@@Z
TdfField::TdfField(const TdfField& other)
    : key(other.key), value(other.value)
{
}

// FUNCTION: 0x4c54d0 ??0TdfField@@QAE@ABVClass_004c91a0@@0@Z
TdfField::TdfField(const Class_004c91a0& a, const Class_004c91a0& b)
    : key(a), value(b)
{
}
