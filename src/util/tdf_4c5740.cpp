// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Binary search (lower_bound-style) over the sorted vector held by the global
// at 0x51fdb8, then a final key check. Each 8-byte element is {key, value};
// the value field is at +4 (the vector member sits at offset 1 of the packed
// singleton, so _First/_Last land at +5/+9). A char** intermediate is what
// produces the lea/mov pair for the second field.
#include <string.h>

#pragma pack(push, 1)
struct Elem_004c5740 {
    char* key;                          // +0
    char* value;                        // +4
};

struct TranslationTable {
    char unknown_0[5];
    Elem_004c5740* first;               // +5
    Elem_004c5740* last;                // +9
};
#pragma pack(pop)

extern TranslationTable* g_translations;

// FUNCTION: 0x4c5740
char* __stdcall Translate(char* key)
{
    if (key == 0)
        return 0;
    TranslationTable* c = g_translations;
    if (c == 0)
        return key;

    Elem_004c5740* first = c->first;
    Elem_004c5740* last = c->last;
    Elem_004c5740* end = c->last;

    while (first != last) {
        Elem_004c5740* mid = first + (last - first) / 2;
        bool less = strcmp(mid->key, key) < 0;
        if (less)
            first = mid + 1;
        else
            last = mid;
    }

    char** found;
    if (first != end) {
        bool before = strcmp(key, first->key) < 0;
        found = before ? 0 : &first->value;
    } else {
        found = 0;
    }
    if (found != 0)
        return *found;
    return key;
}
