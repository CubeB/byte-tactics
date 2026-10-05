// Decompiled by Opus. Names are provisional.
// Sets entry i's end time to now plus its duration.

#pragma pack(push, 1)
struct Entry_004a4620 {
    char unknown_0[0xc2];
    unsigned int duration;             // +0xc2
    unsigned int end;                  // +0xc6
    char unknown_ca[0x15b - 0xca];
};
#pragma pack(pop)

struct Table_004a4620 {
    char unknown_0[4];
    Entry_004a4620* entries;           // +0x4
};

struct Class_004a4620 {
    char unknown_0[0x18];
    Table_004a4620* table;             // +0x18
};

unsigned int __cdecl GetTicks();

// FUNCTION: 0x4a4620
void __stdcall FUN_004a4620(Class_004a4620* obj, int i)
{
    Entry_004a4620* e = &obj->table->entries[i];
    e->end = GetTicks() + e->duration;
}
