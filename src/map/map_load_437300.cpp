// Decompiled by Opus. Names are provisional.
// Counts the type-1 entries (see GetStartPosition).

struct Entry_00437300 {
    int type;                          // +0x0
    int id;                            // +0x4
    short x;                           // +0x8
    short z;                           // +0xa
};

class Mission {
public:
    char unknown_0[0xdb4];
    Entry_00437300* entries;           // +0xdb4
    int entry_count;                   // +0xdb8

    int CountStartPositions();
};

// FUNCTION: 0x437300
int Mission::CountStartPositions()
{
    int n = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].type == 1) {
            n++;
        }
    }
    return n;
}
