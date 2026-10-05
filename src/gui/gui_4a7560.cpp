// Decompiled by Opus. Names are provisional.
// Finds the next entry of type 3 after `index` in a 1-based list of 0x15b-byte
// entries (entry 0 holds the count at +0xb6), wrapping round to the start.
// The parameter itself is the loop counter (the original loads it first and
// keeps a copy of its old value for the wrapped search).

#pragma pack(push, 1)
struct Entry_004a7560 {
    char type;                         // +0x0
    char unknown_1[0xb6 - 0x1];
    short count;                       // +0xb6
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x4a7560
int __stdcall FindNextTextInput(Entry_004a7560* list, int index)
{
    int old = index;
    for (index++; index < list[0].count + 1; index++) {
        if (list[index].type == 3)
            break;
    }
    if (index == list[0].count + 1) {
        for (index = 1; index < old; index++) {
            if (list[index].type == 3)
                break;
        }
    }
    return index;
}
