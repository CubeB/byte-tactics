// Decompiled by space-bunny-free. Names are provisional.
// Checks every 0x5c-byte entry of the array at +0x14. An entry passes when its
// owner is still a live player of a type that needs no bookkeeping (type 2, or
// type 3 whose team data->field_94 is 2) and, otherwise, when its cached count
// is not zero, matches the size of its id vector, and its two counters agree.
// Returns 1 when nothing needs checking (field_64 set, field_58 clear, no
// entries) or when every entry passes, 0 on the first entry that does not.
// The entry's second half is walked through a second pointer that steps with
// the entry's stride, which is what the original's code does.

struct Data_0046e000 {
    char unknown_0[0x94];
    unsigned char field_94;            // +0x94
};

#pragma pack(push, 1)
struct Player_0046e000 {
    int field_0;                       // +0x0
    char unknown_4[0x27 - 0x4];
    Data_0046e000* data;               // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
};
#pragma pack(pop)

struct Ids_0046e000 {
    int* begin;                        // +0x0
    int* end;                          // +0x4
    int* capacity;                     // +0x8
};

struct Sub_0046e000 {                 // an entry from +0x8 on, stepped by 0x5c
    Ids_0046e000 ids;                 // +0x0
    char unknown_c[0x1c - 0xc];
    int count;                        // +0x1c
    int field_20;                     // +0x20
    int field_24;                     // +0x24
    char unknown_28[0x5c - 0x28];
};

struct Entry_0046e000 {                // 0x5c bytes
    int id;                            // +0x0
    char unknown_4[0x5c - 0x4];
};

class Class_0046e000 {
public:
    char unknown_0[0x14];
    Entry_0046e000* begin;             // +0x14
    Entry_0046e000* end;               // +0x18
    char unknown_1c[0x58 - 0x1c];
    int field_58;                      // +0x58
    char unknown_5c[0x64 - 0x5c];
    int field_64;                      // +0x64

    int AllPlayersSynced();
};

Player_0046e000* __stdcall FindPlayerByDpid(int id);

// FUNCTION: 0x46e000
int Class_0046e000::AllPlayersSynced()
{
    if (field_64 != 0)
        return 1;
    if (field_58 == 0)
        return 1;
    Entry_0046e000* p = begin;
    if (p == end)
        return 1;
    Sub_0046e000* s = (Sub_0046e000*)((char*)p + 8);
    for (; p != end; p++, s++) {
        Player_0046e000* pl = FindPlayerByDpid(p->id);
        if (pl != 0) {
            // pl->field_0 is tested again in the second test: the original
            // reloads it rather than reusing the first test's result.
            if (pl->field_0 != 0 && pl->type == 3 && pl->data->field_94 == 2)
                continue;
            if (pl->field_0 != 0 && pl->type == 2)
                continue;
            if (s->count == 0)
                return 0;
            // The count comes from the byte distance between the list's ends,
            // which keeps it a plain arithmetic shift.
            if ((s->ids.begin == 0 ? 0 : ((char*)s->ids.end - (char*)s->ids.begin) >> 2)
                != s->count)
                return 0;
            if (s->field_20 != s->field_24)
                return 0;
        }
    }
    return 1;
}
