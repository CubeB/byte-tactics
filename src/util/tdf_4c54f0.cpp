// Decompiled by space-bunny-free, deepseek-v4.1-flash, GPT-6.1-sol, deepseek-v4.1, Space Bunny Free, claude-sonnet-5-5, Opus and DeepSeek V4.1 Flash. Names are provisional.
//
// The translation tables: the language file is read into a sorted vector of
// (key, value) string handles (g_translations), looked up by Translate and
// FindTranslation, and the section parser's GetLocalizedString reads a TDF
// key with the language prefix. This is a unit of its own, after the TDF
// code in tdf_4c2ea0.cpp: it calls that code's methods (they are not inlined
// here) and its vector<TdfField> instantiations follow it, so the two files
// each see TdfField (and the element destructor, 0x4c5190) their own way.

#include <vector>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <new>

extern char g_language[256];
extern char DAT_005119b8[];

void __cdecl FUN_004d83a0(int);

// The string handle (see tdf_4c2ea0.cpp): 0x4c9180 builds the empty one,
// 0x4c91a0 copies, 0x4c91b0 builds from text, 0x4c9390 releases, 0x4c93b0 and
// 0x4c93f0 assign.
class Class_004c91a0;

class Class_004c9390 {
public:
    char* ptr;

    void ReleaseRef();
};

class Class_004c93b0 {
public:
    char* ptr;

    Class_004c93b0* Assign(const Class_004c91a0* other);
};

class Class_004c93f0 {
public:
    char* ptr;

    Class_004c93f0* AssignText(const char* text);
};

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
    Class_004c91a0& operator=(const Class_004c91a0& other)
    {
        ((Class_004c93b0*)this)->Assign(&other);
        return *this;
    }
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
};

class Class_004c9180 : public Class_004c91a0 {
public:
    Class_004c9180();
};

static inline bool operator==(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return strcmp(a.ptr, b.ptr) == 0;
}

static inline bool Ne(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return !(a == b);
}

// One entry of the translations: a key and a value, both string handles. Its
// destructor (0x4c5190) is in tdf_4c2ea0.cpp.
struct TdfField {
    Class_004c91a0 key;                  // +0x0
    Class_004c91a0 value;                // +0x4

    ~TdfField();
};

// The destroy loop of the entry vector releases the two handles inline.
namespace std {
inline void _Destroy(TdfField* p)
{
    ((Class_004c9390*)&p->value)->ReleaseRef();
    ((Class_004c9390*)&p->key)->ReleaseRef();
}
}

class Class_004c54d0 : public TdfField {
public:
    Class_004c54d0(const Class_004c91a0& a, const Class_004c91a0& b);
};

// Builds the new entry as a by-value result (the exe passes the address of a
// hidden result slot, not the constructor's return value).
static inline Class_004c54d0 MakeElem(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return Class_004c54d0(a, b);
}

// The vector of entries.
class Class_004c5ba0 : public std::vector<TdfField> {
public:
    iterator Insert(iterator p, const TdfField& x);

    // Destroys the entries and frees the storage.
    void DestroyAll()
    {
        TdfField* e = _Last;
        TdfField* p = _First;
        while (p != e) {
            p->~TdfField();
            p++;
        }
        ::operator delete(_First);
        _First = 0;
        _Last = 0;
        _End = 0;
    }
};

#pragma pack(push, 1)
// The map itself, keyed by the string the handle points at.
class TranslationTable {
public:
    char unknown_0;                      // +0x0
    std::vector<TdfField> v;             // +0x1

    TranslationTable(const std::allocator<TdfField>& a) : v(a) {}
};

class Class_004c5c60 {
public:
    char unknown_0;                      // +0x0
    std::vector<TdfField> v;             // +0x1

    TdfField* FindLowerBound(const char* key);
};
#pragma pack(pop)

extern TranslationTable* g_translations;

// Inserts a new entry for the key at e, with an empty value, and returns its
// value handle. The temporaries (the empty handle and the entry) live until
// the end of the return statement.
static inline Class_004c93f0* InsertNew(TranslationTable* s, TdfField* e, const Class_004c91a0& key)
{
    return (Class_004c93f0*)&((Class_004c5ba0*)&s->v)->Insert(e, MakeElem(key, Class_004c9180()))->value;
}

// A TDF section: its name and the entries under it.
class TdfRecord {
public:
    const char* name;                    // +0x0

    void CopyRecordName(char* dest, size_t count);
    int GetFieldString(char* dst, const char* key, size_t size, const char* def);
};

class TdfFile {
public:
    int root;                            // +0x0
    TdfRecord* current;                  // +0x4
    int file;                            // +0x8

    TdfFile();
    ~TdfFile();
    int LoadFile(char* filename);
    TdfRecord* SelectRecordAt(int index);
    void ResetCurrentRecord();
    void Unload();
};

// Reloads one TDF file into the global map (g_translations):
// frees the old map when the section name changed, builds a new one and inserts
// every (name, value) pair of the file's sections.
// FUNCTION: 0x4c54f0
void __stdcall LoadTranslations(char* filename, char* section)
{
    // Deliberately uninitialised: the exe copies this stack byte into the new map.
    std::allocator<TdfField> alloc;
    TranslationTable* s;

    if (_strcmpi(section, g_language) == 0)
        return;
    // Global read once for the vector and once into old: separate registers.
    if (g_translations) {
        TranslationTable* old = g_translations;
        ((Class_004c5ba0*)&g_translations->v)->DestroyAll();
        ::operator delete(old);
    }
    g_translations = new TranslationTable(alloc);
    FUN_004d83a0((int)g_translations);
    strcpy(g_language, section);
    {
        TdfFile f;
        char value[256];
        char name[256];
        if (f.LoadFile(filename)) {
            int index;
            index = 0;
            while (f.SelectRecordAt(index)) {
                f.current->CopyRecordName(name, 0xff);
                f.current->GetFieldString(value, g_language, 0xff, DAT_005119b8);
                if (strlen(value) != 0) {
                    Class_004c91b0 key(name);
                    TdfField* e;
                    s = g_translations;
                    e = ((Class_004c5c60*)g_translations)->FindLowerBound(key.ptr);
                    Class_004c93f0* r;
                    // Nested inline == and Ne helpers give the exe's bool sequence.
                    if (e == s->v.end() || Ne(e->key, key)) {
                        r = InsertNew(s, e, key);
                    } else {
                        r = (Class_004c93f0*)&e->value;
                    }
                    r->AssignText(value);
                }
                f.ResetCurrentRecord();
                index++;
            }
            f.Unload();
        }
    }
}

// Binary search (lower_bound-style) over the sorted vector held by the global
// at 0x51fdb8, then a final key check.
// FUNCTION: 0x4c5740
char* __stdcall Translate(char* key)
{
    if (key == 0)
        return 0;
    TranslationTable* c = g_translations;
    if (c == 0)
        return key;

    TdfField* first = c->v.begin();
    TdfField* last = c->v.end();
    TdfField* end = c->v.end();

    while (first != last) {
        TdfField* mid = first + (last - first) / 2;
        bool less = strcmp(mid->key.ptr, key) < 0;
        if (less)
            first = mid + 1;
        else
            last = mid;
    }

    // char** intermediate: gives the lea/mov pair for the value field.
    char** found;
    if (first != end) {
        bool before = strcmp(key, first->key.ptr) < 0;
        found = before ? 0 : &first->value.ptr;
    } else {
        found = 0;
    }
    if (found != 0)
        return *found;
    return key;
}

// FUNCTION: 0x4c5840
int __stdcall FindTranslation(char* name)
{
    if (name == 0 || g_translations == 0)
        return 0;
    for (std::vector<TdfField>::iterator p = g_translations->v.begin(); p < g_translations->v.end(); p++) {
        if (_strcmpi(p->value.ptr, name) == 0)
            return (int)p->key.ptr;
    }
    return 0;
}

// Looks a key up in a TDF section: builds the key by prepending the current
// section name (g_language), tries that first and falls back to the plain
// key. def (or the empty default DAT_005119b8) is the TDF default value.
// file is the open file object, its section parser at +4.
// FUNCTION: 0x4c58a0
int __stdcall GetLocalizedString(TdfFile* file, char* dst, char* key, size_t size,
                           char* def)
{
    char full[256];
    strcpy(full, g_language);
    strcat(full, key);
    // Intermediate int keeps each argument in the original's register.
    int r;
    if (def) {
        r = file->current->GetFieldString(dst, full, size, def);
        if (r) return 1;
        return file->current->GetFieldString(dst, key, size, def);
    }
    r = file->current->GetFieldString(dst, full, size, DAT_005119b8);
    if (r) return 1;
    return file->current->GetFieldString(dst, key, size, DAT_005119b8);
}

// The vector<TdfField> helpers are protected: a derived class takes their
// addresses to make the compiler emit them out of line.
typedef std::vector<TdfField> Vec_004c5b70;
typedef void (Vec_004c5b70::*DestroyFn_004c5b70)(Vec_004c5b70::iterator, Vec_004c5b70::iterator);
typedef Vec_004c5b70::iterator (Vec_004c5b70::*UcopyFn_004c5b70)(
    Vec_004c5b70::const_iterator, Vec_004c5b70::const_iterator, Vec_004c5b70::iterator);
typedef void (Vec_004c5b70::*UfillFn_004c5b70)(
    Vec_004c5b70::iterator, Vec_004c5b70::size_type, const TdfField&);

struct Access_004c5b70 : Vec_004c5b70 {
    static DestroyFn_004c5b70 destroy;
    static UcopyFn_004c5b70 ucopy;
    static UfillFn_004c5b70 ufill;
};

// FUNCTION: 0x4c5b70 ?_Destroy@?$vector@UTdfField@@V?$allocator@UTdfField@@@std@@@std@@IAEXPAUTdfField@@0@Z
DestroyFn_004c5b70 Access_004c5b70::destroy = &Access_004c5b70::_Destroy;

// FUNCTION: 0x4c5bc0 ?_Ucopy@?$vector@UTdfField@@V?$allocator@UTdfField@@@std@@@std@@IAEPAUTdfField@@PBU3@0PAU3@@Z
UcopyFn_004c5b70 Access_004c5b70::ucopy = &Access_004c5b70::_Ucopy;

// FUNCTION: 0x4c5c20 ?_Ufill@?$vector@UTdfField@@V?$allocator@UTdfField@@@std@@@std@@IAEXPAUTdfField@@IABU3@@Z
UfillFn_004c5b70 Access_004c5b70::ufill = &Access_004c5b70::_Ufill;

// lower_bound over the sorted vector held by the global at 0x51fdb8, keyed by
// the string handle in the first field of each 8-byte element (0x4c5740
// repeats this loop followed by a final key check).
#pragma auto_inline(off)
// FUNCTION: 0x4c5c60
TdfField* Class_004c5c60::FindLowerBound(const char* key)
{
    TdfField* first = v.begin();
    TdfField* last = v.end();

    while (first != last) {
        TdfField* mid = first + (last - first) / 2;
        bool less = strcmp(mid->key.ptr, key) < 0;
        if (less)
            first = mid + 1;
        else
            last = mid;
    }
    return first;
}
#pragma auto_inline(on)

// std::fill<Elem*, Elem>(first, last, x) from MSVC 5's <xutility>, used by
// std::vector<TdfField>::insert; like its neighbour copy_backward
// (0x4c5d10) it ends in `ret N`, so it is written as a __stdcall function.
// FUNCTION: 0x4c5cd0
void __stdcall AssignPairRange(TdfField* first, TdfField* last, const TdfField& x)
{
    for (; first != last; ++first)
        *first = x;
}

// std::copy_backward<Elem*, Elem*>(first, last, dest), used by
// std::vector<TdfField>::insert (0x4c59d0): it assigns [first, last) backwards
// into the range ending at dest and returns the start of the copies.
// __stdcall: the original file's default convention (neighbours end in ret N).
// FUNCTION: 0x4c5d10
TdfField* __stdcall AssignPairRangeBack(TdfField* first, TdfField* last, TdfField* dest)
{
    while (first != last)
        *--dest = *--last;
    return dest;
}

// std::_Construct(TdfField*, const TdfField&): placement-new copy of the pair
// of string handles, called without ecx from the copy loops of vector insert
// (0x4c59d0).
// FUNCTION: 0x4c5d60
void __stdcall ConstructPair(TdfField* p, const TdfField& value)
{
    new ((void*)p) TdfField(value);
}
