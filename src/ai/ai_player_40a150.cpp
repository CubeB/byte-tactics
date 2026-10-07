// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Initialises two 12-byte sub-structures of the object. Each Sub is
// { short v0, v1, v2, v3; int v4; }; the object has one at +0xf1 (s0) and a
// second at +0xfd (s1). v0/v1 become a size from the base v4 plus a random
// amount (+8), and v2/v3 become a random offset centred on that size
// (rand(size) - size/2).
struct Sub {
    short v0;                          // +0x00
    short v1;                          // +0x02
    short v2;                          // +0x04
    short v3;                          // +0x06
    int   v4;                          // +0x08
};

#pragma pack(push, 1)
struct Class_0040a150 {
    char unknown_0[0xf1];
    Sub  s0;                           // +0xf1
    Sub  s1;                           // +0xfd
    void InitPlacementGrid();
};
#pragma pack(pop)

int __stdcall RandomInt(int range);

// FUNCTION: 0x40a150
void Class_0040a150::InitPlacementGrid()
{
    s0.v4 = 3;
    s0.v0 = RandomInt(10) + s0.v4 + 8;
    s0.v1 = RandomInt(3) + s0.v4 + 8;
    s0.v2 = RandomInt(s0.v0) - s0.v0 / 2;
    s0.v3 = RandomInt(s0.v1) - s0.v1 / 2;
    s1.v4 = 6;
    s1.v0 = RandomInt(0x14) + s1.v4 + 8;
    s1.v1 = RandomInt(3) + s1.v4 + 8;
    s1.v2 = RandomInt(s1.v0) - s1.v0 / 2;
    s1.v3 = RandomInt(s1.v1) - s1.v1 / 2;
}
