// Decompiled by mimo-v2.6-pro. Names are provisional.
// v3: call-pattern probe. No copy redirect. Sites 1-3 insert(end(), 1, p),
// site 4 push_back(p). Not the deliverable file.
#include <vector>

struct Point_0044eec0 {
    short x;
    short y;
};

typedef std::vector<Point_0044eec0> Vec_0044da00;

class Class_0044da00 {
public:
    char unknown_0[8];
    int x1;                 // +0x8
    int x2;                 // +0xc
    int y1;                 // +0x10
    int y2;                 // +0x14

    void FUN_0044da00(Vec_0044da00* list);
};

static inline Point_0044eec0 MakePoint_0044da00(int x, int y)
{
    Point_0044eec0 p;
    p.x = (short)x;
    p.y = (short)y;
    return p;
}

// FUNCTION: 0x44da00
void Class_0044da00::FUN_0044da00(Vec_0044da00* list)
{
    list->clear();
    for (int i = x1; i <= x2; i++) {
        list->insert(list->end(), 1, MakePoint_0044da00(i, y1));
        list->insert(list->end(), 1, MakePoint_0044da00(i, y2));
    }
    for (int j = y1 + 1; j <= y2 - 1; j++) {
        list->insert(list->end(), 1, MakePoint_0044da00(x1, j));
        list->push_back(MakePoint_0044da00(x2, j));
    }
}
