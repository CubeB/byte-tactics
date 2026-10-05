// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Vec3 {
    int x, y, z;
};

struct Entry {
    Vec3 pos;                          // +0x0
    float val;                         // +0xc
};

struct Feature {
    char unknown_0[0xec];
    float weightA;                     // +0xec
    float weightB;                     // +0xf0
    char unknown_f4[0xfe - 0xf4];
    unsigned short flags;              // +0xfe
};

struct Game {
    char unknown_0[0x1426f];
    Feature* features;                 // +0x1426f
};
#pragma pack(pop)

union Fixed {
    int value;
    struct { unsigned short fraction; short whole; };
};

extern Game* g_game;

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

void* __stdcall GetMapCellAtPosition(Vec3* pos);
unsigned short __stdcall GetCellFeature(void* cell);
int __stdcall FUN_004b6c30(int range);

// FUNCTION: 0x47ea40
int __stdcall FUN_0047ea40(Vec3* center, Fixed radius, Vec3** out1, float* val1, Vec3** out2, float* val2)
{
    int countB = 0;
    int countA = 0;
    int max = radius.whole * radius.whole / 256;
    Entry* a = (Entry*)operator new(max * sizeof(Entry));
    Entry* b = (Entry*)operator new(max * sizeof(Entry));
    int half = radius.value / 2;
    Vec3 pos;
    for (pos.z = center->z - half; pos.z <= center->z + half; pos.z += 0x300000) {
        for (pos.x = center->x - half; pos.x <= center->x + half; pos.x += 0x300000) {
            void* cell = GetMapCellAtPosition(&pos);
            if (!cell)
                continue;
            unsigned short index = GetCellFeature(cell);
            if (index >= 0xfffb)
                continue;
            unsigned short fl = g_game->features[index].flags;
            if (!(fl & 0x80))
                continue;
            if (!(fl & 0x100))
                continue;
            if (g_game->features[index].weightA != 0.0f) {
                b[countB].pos = pos;
                b[countB].val = g_game->features[index].weightA;
                countB++;
            }
            if (g_game->features[index].weightB != 0.0f) {
                a[countA].pos = pos;
                a[countA].val = g_game->features[index].weightB;
                countA++;
            }
        }
    }
    int found = 0;
    if (countB != 0) {
        int best = 0;
        float bestVal = 0.0f;
        for (int k = 0; k < 3; k++) {
            int i = FUN_004b6c30(countB);
            if (bestVal < b[i].val) {
                bestVal = b[i].val;
                best = i;
            }
        }
        **out1 = b[best].pos;
        *val1 = b[best].val;
        found = 1;
    } else {
        *out1 = 0;
        *val1 = 0;
    }
    if (countA != 0) {
        float bestVal = 0.0f;
        int best = 0;
        for (int k = 0; k < 3; k++) {
            int i = FUN_004b6c30(countA);
            if (bestVal < a[i].val) {
                bestVal = a[i].val;
                best = i;
            }
        }
        **out2 = a[best].pos;
        *val2 = a[best].val;
        found = 1;
    } else {
        *out2 = 0;
        *val2 = 0;
    }
    operator delete(b);
    operator delete(a);
    return found;
}
