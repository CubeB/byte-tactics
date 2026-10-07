// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// lower_bound over the sorted vector held by the global at 0x51fdb8, keyed by
// the string handle in the first field of each 8-byte element (0x4c5740
// repeats this loop followed by a final key check). The vector member sits at offset 1 of the packed singleton object,
// so its _First/_Last land at +5/+9.
#include <string.h>

#pragma pack(push, 1)
struct Elem_004c5c60 {
    const char* key;                   // +0x0
    char unknown_4[4];
};

struct Class_004c5c60 {
    char unknown_0[5];
    Elem_004c5c60* first;              // +0x5
    Elem_004c5c60* last;               // +0x9

    Elem_004c5c60* FindLowerBound(const char* key);
};
#pragma pack(pop)

// FUNCTION: 0x4c5c60
Elem_004c5c60* Class_004c5c60::FindLowerBound(const char* key)
{
    Elem_004c5c60* first = this->first;
    Elem_004c5c60* last = this->last;

    while (first != last) {
        Elem_004c5c60* mid = first + (last - first) / 2;
        bool less = strcmp(mid->key, key) < 0;
        if (less)
            first = mid + 1;
        else
            last = mid;
    }
    return first;
}
