// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Best 86.2%. Only the loop prologue still differs (the tail and the
// back-edge already match):
//  - ours loads DAT into eax and copies it to ebp; the original loads it
//    straight into ebp (`mov ebp,[DAT]`), so the whole guard is one byte
//    further along;
//  - because of that the count pointer is based on eax (`lea edi,[eax+0x99]`)
//    where the original bases it on ebp (`lea edi,[ebp+0x99]`), and the guard
//    load is `mov eax,[eax+0x99]` instead of `mov eax,[ebp+0x99]`;
//  - the original re-materialises `lea edi,[ebp+0x99]` just before the inner
//    shift loop (0x47ef33); this version keeps edi live.
// The eax detour is what taking the count pointer through the global
// (&DAT_0051e68c->count) produces; taking it through the local list instead
// makes MSVC fold the pointer away entirely (53.8%, list lands in esi and the
// zero in ebp). Everything that had been tried by the end of this attempt:
// local list + &list->count (fold), references, char*/int casts, uninitialised
// locals, do/while, and deriving the list from a global-derived count pointer
// (75.5%). Also note: FUN_004ceee0 is declared here as a method of
// Class_004d0130, but ctx.py names it Class_004ceee0::FUN_004ceee0; if the
// loop is ever fixed the symbol check will want a separate Class_004ceee0 and
// a cast on `sound`.

#pragma pack(push, 1)
struct Entry_0047f8c0 {                // 0x11 bytes
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int* data;                         // +0xc
    char field_10;                     // +0x10
};

struct List_0047f8c0 {
    Entry_0047f8c0 entries[9];         // +0x0
    int count;                         // +0x99
    int field_9d;                      // +0x9d
};
#pragma pack(pop)

class Class_004d0130 {
public:
    void FUN_004d0130();
    void FUN_004ceee0();
};

struct Game_0047eee0 {
    char unknown_0[0x10];
    Class_004d0130* sound;             // +0x10
};

extern List_0047f8c0* DAT_0051e68c;
extern Game_0047eee0* g_game;

void operator delete(void* p);
void FUN_004d85a0(int* data);

// FUNCTION: 0x47eee0
void FUN_0047eee0()
{
    List_0047f8c0* list = DAT_0051e68c;
    if (list) {
        int* count = &DAT_0051e68c->count;
        while (*count > 0) {
            int i = *count - 1;
            Entry_0047f8c0* last = &list->entries[i];
            if (last->data) {
                FUN_004d85a0(last->data);
                last->data = 0;
            }
            for (int j = i; j < *count; j++)
                list->entries[j] = list->entries[j + 1];
            (*count)--;
        }
        list->field_9d = 0;
        operator delete(list);
        DAT_0051e68c = 0;
    }
    g_game->sound->FUN_004d0130();
    Class_004d0130* sound = g_game->sound;
    if (sound) {
        sound->FUN_004ceee0();
        operator delete(sound);
    }
    g_game->sound = 0;
}
