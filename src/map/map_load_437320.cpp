// Decompiled by Opus. Names are provisional.

struct Vec3_00437320 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

struct Entry_00437320 {
    int type;                          // +0x0
    int id;                            // +0x4
    short x;                           // +0x8
    short z;                           // +0xa
};

class Class_00437320 {
public:
    char unknown_0[0xdb4];
    Entry_00437320* entries;           // +0xdb4
    int entry_count;                   // +0xdb8

    int GetStartPosition(Vec3_00437320* out, int id);
};

// Finds the type-1 entry with the given id and returns its position (16.16
// fixed point, y = 0) in `out`. Returns 0 if there is none.
// FUNCTION: 0x437320
int Class_00437320::GetStartPosition(Vec3_00437320* out, int id)
{
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].type == 1 && entries[i].id == id) {
            out->x = entries[i].x << 16;
            out->z = entries[i].z << 16;
            out->y = 0;
            return 1;
        }
    }
    return 0;
}
