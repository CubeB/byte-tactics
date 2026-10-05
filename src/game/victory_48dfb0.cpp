// Decompiled by Opus. Names are provisional.

class Class_0048dfb0 {
public:
    void* bufsA[16];             // +0
    int countA;                  // +0x40
    void* bufsB[16];             // +0x44
    int countB;                  // +0x84

    void FUN_0048dfb0();
};

// FUNCTION: 0x48dfb0
void Class_0048dfb0::FUN_0048dfb0()
{
    int i;
    for (i = 0; i < countA; i++) {
        operator delete(bufsA[i]);
    }
    for (i = 0; i < countB; i++) {
        operator delete(bufsB[i]);
    }
}
