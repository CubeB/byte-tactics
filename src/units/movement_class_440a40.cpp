// Decompiled by Opus. Names are provisional.
// Refreshes every entry in use of the table at 0x512358 (the loop that
// 0x440a70 inlines as its UpdateAll helper).

struct Point_00440a40 {
    short x;
    short y;
};

struct MovementClass {
    int* field_0;                      // +0x0
    char unknown_4[0x20 - 0x4];
    void RefreshPassMap(Point_00440a40 a, Point_00440a40 b);
};

struct MovementClassTable {
    MovementClass entries[32];

    static MovementClassTable g_movementClasses;
};

// FUNCTION: 0x440a40
void __stdcall RefreshAllPassMaps(Point_00440a40 a, Point_00440a40 b)
{
    for (int i = 0; i < 32; i++) {
        if (MovementClassTable::g_movementClasses.entries[i].field_0 != 0) {
            ((MovementClass*)&MovementClassTable::g_movementClasses.entries[i])->RefreshPassMap(a, b);
        }
    }
}
