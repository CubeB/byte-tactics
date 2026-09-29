// Decompiled by Space Bunny Free. Names are provisional.
// This file of the original was built with /Gz, so the function is __stdcall;
// found by the orchestrator's calling-convention sweep of every partial.
// Sonnet 5.5 retry (#679), still 97.9%: also flat under every single and
// every pair of statement moves (4000 sampled pairs), function-scope or
// in-block declaration of gadget/entries/choice1/choice2 in all 24 orders,
// typed 0x15b-byte rows (`rows + i`, `->text`), `(char*)` and `&p[0xb6]`
// spellings, `unsigned char*` pointers, and a local for
// g_game + 0x519, which drops to 64.5%.
// Opens the YESORNO.GUI dialog asking "Close Windows CD Player?", relabels
// the CHOICE1 / CHOICE2 gadgets, puts the localised "Yes" and "No" on the two
// gadgets the lookups found, and installs FUN_00426190 as the gadget handler.
// FUN_004a76b0 is then told about "CHOICE1".
//
// Not a MATCH yet: 97.9% (411 of 420 bytes), same length as the original and
// every reference resolves. What still differs is only the list scheduler's
// order inside the two inlined strcpy bodies:
//
//   original                         ours
//   call FUN_004c5740                call FUN_004c5740
//   mov edi, eax                     mov ecx, [esp + 0x10]   <-
//   xor eax, eax                     mov edi, eax
//   mov ecx, [esp + 0x10]   <-       xor eax, eax
//   lea edx, [ecx + 0xb6]            lea edx, [ecx + 0xb6]
//
// and in the second copy, where the original pushes the next call's string
// argument ("Close Windows CD Player?") before the copy loop while ours pushes
// it after `sub edi, ecx`. So the reload of the two stack-held choice
// pointers and one argument push are each scheduled one slot too early here.
// The score is flat at 97.9% for about forty source shapes (typed and untyped
// entry pointers, the two choice pointers in an array or a struct, the name
// copies through a typed form struct, the offsets as decimal, each strcpy and
// each call split into its own static inline helper, the text and the title in
// locals, an early return instead of a guarded block, and the pointers
// computed by a helper), for all 128 header sets and for the 768 sets of
// tools/headers.py --cpp, and for a sweep of 0 to 160 unused extern
// declarations. This is the "reload of an address-taken local drifting around a
// call's argument pushes" scheduler tie-break (see 0x45b6570 in the guide),
// which source rewrites have not reached.
//
// A third pass added four more, none of which moved it: the two copies in the
// opposite order (96.4%), the two destination pointers held in locals
// `choice1 + 0xb6` and `choice2 + 0xb6` (97.9%, identical bytes), the two
// looked-up strings held in locals before the copies (81.9%), and both together
// (81.9%). The destination-pointer form is worth keeping as a null result: it
// is the shape the guide recommends for deciding which load MSVC hoists, and
// it changes nothing here.
//
// The residue is the same scheduler tie-break as 0x4b6570 (96.7%) and 0x4a35a0
// (96.9%): the reload of a stack-held pointer is scheduled one slot early
// relative to the `mov edi, eax` and `xor eax, eax` of the inlined strcpy, and
// one argument push lands after the copy loop instead of before it. Three
// functions, three different code shapes, the same place the scheduler decides
// to put a load. Treat it as compiler state.
#include <string.h>

extern char* g_game;

struct Gadget_00426200 {
    int unknown_0;
    char* entries;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

Gadget_00426200* __stdcall FUN_004aa8f0(char* sub, const char* name, int flags);
void __stdcall FUN_0049fb10(char* sub, int value);
int __stdcall FUN_0049fdf0(char* entries, const char* name, int type);
void __stdcall FUN_004a0bf0(char* sub, const char* name, const char* text, int param_4);
void __stdcall FUN_004a76b0(char* sub, const char* name);
void __stdcall FUN_004a81e0(char* sub, int value);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_00426190(void* gadget);

// FUNCTION: 0x426200
void __stdcall FUN_00426200()
{
    Gadget_00426200* gadget = FUN_004aa8f0(g_game + 0x519, "YESORNO.GUI", 0x100);
    if (gadget != 0) {
        FUN_0049fb10(g_game + 0x519, 1);
        char* entries = gadget->entries;
        char* choice1 = entries + 0x15b * FUN_0049fdf0(entries, "CHOICE1", 1);
        char* choice2 = entries + 0x15b * FUN_0049fdf0(entries, "CHOICE2", 1);
        strcpy(entries + 0xcc, "CHOICE1");
        strcpy(entries + 0xdc, "CHOICE2");
        strcpy(choice1 + 0xb6, FUN_004c5740("Yes"));
        strcpy(choice2 + 0xb6, FUN_004c5740("No"));
        FUN_004a0bf0(g_game + 0x519, "TITLE", FUN_004c5740("Close Windows CD Player?"), 0);
        FUN_004a76b0(g_game + 0x519, "CHOICE1");
        gadget->handler = FUN_00426190;
        FUN_004a81e0(g_game + 0x519, 0x40);
    }
}
