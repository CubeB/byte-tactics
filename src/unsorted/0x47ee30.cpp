// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5. Names are provisional.
// Claude Sonnet 5.5 pass (#746): nothing beat 91.2% and 169 bytes. Compiler state is
// ruled out (declaration-count sweep 0 to 400 in steps of 8 and all 128 header sets
// of headers.py: 91.2% everywhere). About 40 more shapes were scored:
//  * every access spelled `list->count` in a guarded do/while (`if (list->count > 0)
//    do { ... } while (--list->count > 0);`), the same with `while`, or every access
//    spelled `entries[9].field_0` (the count is entries[9].field_0, 9 * 0x11 = 0x99):
//    167 bytes, 67.7%: the compiler folds the count into the list base (list in edi,
//    a zero in ebp, `cmp [edi+0x99], ebp`), so there is no separate count pointer;
//  * a separate `int* count = &DAT_0051e68c->count` (from the global, not from the
//    list) with the do/while `--*count > 0` back edge: 161 bytes, 83.2%. The back
//    edge then forwards (`mov eax, ecx`) as in the original, but the prologue loads
//    the global into eax first and copies it (`lea edi,[eax+0x99]; mov ebp,eax;
//    cmp [edi],0`), and the redundant `lea edi,[ebp+0x99]` is still missing. Which of
//    the guard, `i = ... - 1`, the shift loop's condition and the decrement go through
//    the count pointer or through `list->count` makes no difference (all 8
//    combinations 83.2%, one 80.9%), nor does the guard through a local, through
//    `(char*)list + 0x99` or through `entries[9].field_0`, nor placing the pointer
//    inside the if;
//  * the shift loop or the whole removal (free + shift) as a `static inline` helper
//    taking the list (the hoped-for source of the second `lea edi,[ebp+0x99]`):
//    83.2%, unchanged;
//  * this file's while loop with the back edge as `if (--*count <= 0) break;` (83.7%)
//    or as a `for (;;)` with the guard as an early break (91.2%, same bytes).
// So the two wanted things (a `list` in ebp with `count` derived from it, and a
// forwarded back edge) still exclude each other in every spelling tried.
// Best: 91.2%. The prologue and the tail now match. Two codegen differences remain:
//  - the loop back-edge reloads the count through the base ([ebp+0x99]) where the
//    original keeps the decremented value in eax (`mov eax, ecx`);
//  - the original rematerialises `lea edi, [ebp+0x99]` after the free call, ours
//    keeps edi live instead.
// A natural alternative with the same prologue and back-edge but a one-byte
// prologue difference (DAT is loaded into eax and copied to ebp) is a guarded
// do/while: `int* count = &DAT->count; Entry* entries = DAT->entries;
// if (entries[9].field_0 > 0) { do { ... } while (*count > 0); }`.
//
// Retry by deepseek-v4.1-flash: about 45 shapes were scored in scratch. They
// split into two families and neither reaches 100%:
//  - guard/condition written through the base (`*(int*)((char*)entries+0x99)`,
//    `((List*)entries)->count`, `entries[9].field_0`) keeps the prologue exact
//    (ebp = DAT, edi = &count) but the back edge reloads [ebp+0x99] instead of
//    forwarding the decrement into eax;
//  - condition written as `*count` (the same lvalue as `(*count)--`) makes the
//    back edge forward (`mov eax, ecx`) but the compiler then computes the
//    count pointer first, from a scratch load of the global:
//    `mov eax,[DAT]; ...; mov ebp,eax; lea edi,[eax+0x99]`, and the guard is
//    folded to `cmp [edi], 0`.
// Deriving count from `list`/`entries` (`&list->count`, `(char*)entries+0x99`)
// always folds the pointer away, so count must come from the global to stay a
// separate edi. The original needs both the ebp-based prologue and the
// forwarded back edge at once, plus the redundant `lea edi,[ebp+0x99]` before
// the shift loop, which none of the scored shapes produced.

void FUN_004d85a0(int* param_1);

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

struct Table_0047ee30 {                // 0x18 bytes
    int field_0;                       // +0x0
    int unknown_4;                     // +0x4
    int unknown_8;                     // +0x8
    int field_c;                       // +0xc
    int unknown_10;                    // +0x10
    int unknown_14;                    // +0x14
};

extern List_0047f8c0* DAT_0051e68c;
extern Table_0047ee30 DAT_005086e0[24];

// The count is reached through its own pointer and the guard reads it through the
// base: that keeps the list in ebp and the count pointer in edi, as in the
// original, instead of folding the count into the list base.
// FUNCTION: 0x47ee30
void FUN_0047ee30()
{
    int* count = &DAT_0051e68c->count;
    Entry_0047f8c0* entries = DAT_0051e68c->entries;
    while (*(int*)((char*)entries + 0x99) > 0) {
        int i = *count - 1;
        if (entries[i].data != 0) {
            FUN_004d85a0(entries[i].data);
            entries[i].data = 0;
        }
        for (int j = i; j < *count; j++)
            entries[j] = entries[j + 1];
        (*count)--;
    }
    ((List_0047f8c0*)entries)->field_9d = 0;
    for (int k = 0; k < 24; k++)
        DAT_005086e0[k].field_c = 0;
}
