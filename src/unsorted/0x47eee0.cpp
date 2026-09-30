// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, continued by GPT-6.1-sol, continued by Space Bunny Free. Names are provisional.
// Space Bunny Free pass (#1958): the file below still holds the 86.2% best (219
// bytes); nothing in this pass beat it, all other shapes scored with --sym:
//  * count pointer declared BEFORE the list local (`int* count =
//    &DAT_0051e68c->count; List* list = DAT_0051e68c;`): 84.3%, 213 bytes. The
//    lea is then hoisted above the null test and the guard folds to
//    `cmp [edi],ebx`, so the original's `mov eax,[ebp+0x99]` load is lost. Same
//    84.3% and 217 bytes with the guard spelled through the base.
//  * the count pointer declared INSIDE the guard, after a base-form guard
//    (`if (list->count > 0) { int* count = &DAT_0051e68c->count; do {...} while
//    (*count > 0); }`): 76.7%, 217 bytes, but this is the only shape that gets
//    the original's ORDER right (null test, `cmp [ebp+0x99],edi`, then
//    `lea edi,[eax+0x99]`); the zero register moves to edi and the eax detour
//    stays. `List* const list` and an extra `entries` local do not change it.
//  * no `list` local at all (every access through DAT_0051e68c): 24.7%, 237
//    bytes; the count pointer lands in ebx and the zero in edi, the mirror of
//    the original's roles, and the list temp dies early.
// What this pass established (not in the file above): MSVC 5 folds an address
// temp with a REGISTER base into memrefs in every local-derived spelling
// (`&list->count`, `(int*)((char*)list+0x99)`, a `List*` alias, the entries
// base, the pointer before the test), and never folds the GLOBAL-derived one.
// So `&list->count` can never produce the wanted `lea edi,[ebp+0x99]`, and
// `&DAT_0051e68c->count` always costs the `mov eax,[DAT]; mov ebp,eax` detour
// that puts the lea on eax instead of ebp. The two are exclusive in every
// spelling tried, which is why 86.2% stands.
// Retry #1748: GPT-6.1-sol confirmed 86.2% (219/224) after four checks; no MATCH. The saved source still differs in the count-pointer register plan and final zero store.
// Claude Sonnet 5.5 pass (#746): nothing beat 86.2% (219 bytes). Compiler state is
// ruled out: the declaration-count sweep (0 to 400 in steps of 8) is 86.2% for every N
// and all 128 header sets of headers.py give 86.2% at best. What the register plan of
// the original is (from the zero register's life): list in ebp, the count pointer
// in edi (`lea edi, [ebp+0x99]`, kept for the whole loop), i in esi, and ebx is BOTH
// the zero constant (null checks, the `> 0` guard, the final stores) and, inside
// the loop, the entry pointer `last` (`lea ebx, [esi+ecx]`), which is why the
// original re-zeroes it (`xor ebx, ebx`) after the loop and stores a literal 0
// (`mov [ebx+0xc], 0`) where every version of ours reuses a zero register.
// Scored (all --sym, none counted as runs): the clear loop as inline member
// functions RemoveLast/Clear (224 bytes, 53.8% and 59.9%), the count pointer as
// &list->count (53.8%), as a second pointer for the inner loop (69.2%, 224 bytes,
// the pointer folds into [list+0x99]), `count` reassigned inside the loop (44 to
// 58%), no `list` local at all (24.7%), a CountPtr(list) inline helper (53.8%),
// the count as a member of an embedded tail struct (53.8%), and `count` formed
// before the `if (list)` (84.3%, 213 bytes: lea edi before the null check, DAT
// loaded before the pushes, but no eax detour after it). The guard in the original
// reads `[ebp+0x99]` directly while the loop body reads [edi], and the lea before
// the inner loop is a loop-invariant hoist of `&list->count` merged into edi; no
// spelling kept that pointer alive without also keeping the eax copy.
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
// deepseek-v4.1-flash retry (#1451): confirmed 86.2% (219 bytes) is the ceiling
// for every shape tried this pass, all scored with --sym (0 counted check runs
// beyond the confirming one). New shapes scored, all <= 86.2%: count derived
// from a local list as `&list->count` folds the pointer (53.8%), as
// `(int*)((char*)list + 0x99)` still folds (53.8%), a `for` loop with the
// pointer in the init clause folds (53.8%), a `List* const list` folds (53.8%),
// a pointer-to-pointer route `(**pp)` reproduces the exact 86.2% eax detour,
// `&DAT->count` with a separate `entries` local (69.2%), and `while(list->count)`
// with the pointer as the decrement (69.2%). The root difference is unchanged:
// the original materialises the global into ebp directly (`mov ebp,[DAT]`) and
// derives edi from ebp, with the extra `lea edi,[ebp+0x99]` rematerialised at
// the shift-loop preheader; every spelling that derives the count pointer from
// the global puts the global in eax and copies to ebp (+1 byte, -6 bytes from
// the missing lea), and every spelling that derives it from the list folds the
// pointer away. Best remains 86.2%.

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

void __cdecl operator delete(void* p);
void __cdecl FUN_004d85a0(int* data);

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
