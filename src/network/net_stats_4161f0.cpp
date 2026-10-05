// Decompiled by Opus. Names are provisional.

struct Pair_00419560 {
    int a;
    int b;
};

class PacketManager {
public:
    void FUN_00461610();
};

extern Pair_00419560 DAT_00511a60[44];
extern Pair_00419560 DAT_00511c60[44];
extern PacketManager g_packetManager;

// The sums are never used (their consumer was presumably compiled out).
// As in FUN_00419560, the second field of the second table is read through a
// walking pointer.
// FUNCTION: 0x4161f0
void FUN_004161f0()
{
    int sum3 = 0, sum4 = 0, sum1 = 0, sum2 = 0;
    Pair_00419560* p = DAT_00511c60;
    for (int i = 0; i < 44; i++) {
        sum1 += DAT_00511a60[i].a;
        sum2 += DAT_00511a60[i].b;
        sum3 += DAT_00511c60[i].a;
        sum4 += p->b;
        p++;
    }
    g_packetManager.FUN_00461610();
}
