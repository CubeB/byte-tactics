// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash; further tried by GPT-6.1-sol, edited by deepseek-v4.1, further tried by Space Bunny Free, finished by claude-sonnet-5-5. Names are provisional.
// Reloads one TDF file into the global map (g_translations):
// frees the old map when the section name changed, builds a new one and inserts
// every (name, value) pair of the file's sections.
#include <string.h>
#include <memory.h>
#include <stdio.h>

extern char g_language[256];
extern char DAT_005119b8[];

void __cdecl FUN_004d83a0(int);
void __cdecl operator delete(void* p);

class Class_004c9390 {
public:
    char* ptr;

    void ReleaseRef();
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
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

class Class_004c9180 : public Class_004c91a0 {
public:
    Class_004c9180();
    ~Class_004c9180() { ((Class_004c9390*)this)->ReleaseRef(); }
};

static inline bool operator==(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return strcmp(a.ptr, b.ptr) == 0;
}

static inline bool Ne(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return !(a == b);
}

// One entry of the global map: a key and a value, both string handles.
struct TdfField {
    Class_004c91a0 key;                  // +0x0
    Class_004c91a0 value;                // +0x4

    ~TdfField();
};

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

// The vector of entries (same class as 0x4c59d0's vector).
class Class_004c5ba0 {
public:
    char count;                          // +0x0
    char pad[3];
    TdfField* first;                     // +0x4
    TdfField* last;                      // +0x8
    TdfField* end;                       // +0xc

    TdfField* FUN_004c59d0(TdfField* pos, const TdfField& val);
};

static inline void DestroyVec(Class_004c5ba0* w)
{
    TdfField* e = w->last;
    TdfField* p = w->first;
    while (p != e) {
        p->~TdfField();
        p++;
    }
    ::operator delete(w->first);
    w->first = 0;
    w->last = 0;
    w->end = 0;
}

// The map itself, keyed by the string the handle points at.
#pragma pack(push, 1)
class Class_004c5c60 {
public:
    char unknown_0[5];
    TdfField* first;                    // +0x5
    TdfField* last;                     // +0x9

    TdfField* FindLowerBound(const char* key);
};

class TranslationTable {
public:
    char unknown_0;                      // +0x0
    Class_004c5ba0 v;                      // +0x1 (_First at +0x5)

    TranslationTable(char count);
};
#pragma pack(pop)

extern TranslationTable* g_translations;

TranslationTable::TranslationTable(char count)
{
    v.first = 0;
    v.count = count;
    v.last = 0;
    v.end = 0;
}

// Inserts a new entry for the key at e, with an empty value, and returns its
// value handle. The temporaries (the empty handle and the entry) live until
// the end of the return statement.
static inline Class_004c93f0* InsertNew(TranslationTable* s, TdfField* e, const Class_004c91a0& key)
{
    return (Class_004c93f0*)&((Class_004c5ba0*)(1 + (char*)s))->FUN_004c59d0(e, MakeElem(key, Class_004c9180()))->value;
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

static inline void LoadMap(char flag, char* section)
{
    g_translations = new TranslationTable(flag);
    FUN_004d83a0((int)g_translations);
    strcpy(g_language, section);
}

// FUNCTION: 0x4c54f0
void __stdcall LoadTranslations(char* filename, char* section)
{
    // Deliberately uninitialised: the exe copies this stack byte into the new map.
    char flag;
    TranslationTable* s;

    if (_strcmpi(section, g_language) == 0)
        return;
    // Global read once for the vector and once into old: separate registers.
    if (g_translations) {
        TranslationTable* old = g_translations;
        DestroyVec(&g_translations->v);
        ::operator delete(old);
    }
    LoadMap(flag, section);
    {
        TdfFile f;
        char value[256];
        char name[256];
        if (((TdfFile*)&f)->LoadFile(filename)) {
            int index;
            index = 0;
            while (((TdfFile*)&f)->SelectRecordAt(index)) {
                f.current->CopyRecordName(name, 0xff);
                ((TdfRecord*)f.current)->GetFieldString(value, g_language, 0xff, DAT_005119b8);
                if (strlen(value) != 0) {
                    Class_004c91b0 key(name);
                    TdfField* e;
                    s = g_translations;
                    e = ((Class_004c5c60*)g_translations)->FindLowerBound(key.ptr);
                    Class_004c93f0* r;
                    // Nested inline == and Ne helpers give the exe's bool sequence.
                    if (e == ((Class_004c5c60*)s)->last || Ne(e->key, key)) {
                        r = InsertNew(s, e, key);
                    } else {
                        r = (Class_004c93f0*)&e->value;
                    }
                    r->AssignText(value);
                }
                ((TdfFile*)&f)->ResetCurrentRecord();
                index++;
            }
            ((TdfFile*)&f)->Unload();
        }
    }
}