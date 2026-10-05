// What a unit says: one entry per kind of report ("select", "underattack",
// "count5"), with the sound.tdf entry it plays and the text it shows. The
// code indexes it by kind from 1 and entry 0 is empty, so its references
// point into it: 0x5086dc (+0x4, 0x47fad0), 0x5086e0 (+0x8, 0x47ee30 and
// 0x47fd70), 0x5086e8 (+0x10, 0x47f780, 0x47f7e0, 0x47f850), 0x5086ec (+0x14)
// and 0x5086fc (entry 1's sound, 0x42f580).

struct UnitMessage {
    int kind;                       // +0x00 its own index
    int priority;                   // +0x04 a higher one replaces a report on screen
    int delay;                      // +0x08 seconds before it may play again
    const char* sound;              // +0x0c its entry in sound.tdf
    const char* text;               // +0x10 shown on screen, or null
    unsigned int nextFrame;         // +0x14 the frame it may play again (set at run time)
};

// GLOBAL: 0x5086d8
UnitMessage g_unitMessages[24] = {
    {0, 0, 0, 0, 0, 0},
    {1, 10, 0, "select", 0, 0},
    {2, 9, 20, "underattack", "Under Attack", 0},
    {3, 4, 2, "activate", 0, 0},
    {4, 4, 2, "deactivate", 0, 0},
    {5, 5, 1, "ok", 0, 0},
    {6, 3, 4, "arrived", "Arrived", 0},
    {7, 8, 1, "cant", "Cannot Comply", 0},
    {8, 3, 3, "unitcomplete", "Nanolathe Complete", 0},
    {9, 4, 2, "build", 0, 0},
    {10, 3, 1, "repair", 0, 0},
    {11, 2, 1, "working", 0, 0},
    {12, 7, 1, "load", 0, 0},
    {13, 7, 1, "unload", 0, 0},
    {14, 7, 1, "cloak", "Cloaked", 0},
    {15, 7, 1, "uncloak", "Visible", 0},
    {16, 4, 1, "capture", 0, 0},
    {17, 10, 0, "count5", "five", 0},
    {18, 10, 0, "count4", "four", 0},
    {19, 10, 0, "count3", "three", 0},
    {20, 10, 0, "count2", "two", 0},
    {21, 10, 0, "count1", "one", 0},
    {22, 10, 0, "count0", "zero", 0},
    {23, 10, 0, "canceldestruct", "Self destruct terminated", 0},
};
