// Decompiled by Opus. Names are provisional.
// Refreshes every entry in use of the table at 0x512358 (the loop that
// 0x440a70 inlines as its UpdateAll helper).

struct Point_00440a40 {
    short x;
    short y;
};

struct Class_00440320 {
    int* field_0;                      // +0x0
    char unknown_4[0x20 - 0x4];
};

class Class_00440830 {
public:
    void FUN_00440830(Point_00440a40 a, Point_00440a40 b);
};

struct Class_00440290 {
    Class_00440320 entries[32];

    static Class_00440290 DAT_00512358;
};

// FUNCTION: 0x440a40
void __stdcall FUN_00440a40(Point_00440a40 a, Point_00440a40 b)
{
    for (int i = 0; i < 32; i++) {
        if (Class_00440290::DAT_00512358.entries[i].field_0 != 0) {
            ((Class_00440830*)&Class_00440290::DAT_00512358.entries[i])->FUN_00440830(a, b);
        }
    }
}
