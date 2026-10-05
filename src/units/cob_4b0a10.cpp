// Decompiled by Opus. Names are provisional.
// FUN_004b08c0 claims one of the 8 channel slots for an id and returns its
// index (or -1); this one then stores a value in the new slot.
struct Channel_004b0a10 {
    int used;                          // +0x0
    int id;                            // +0x4
    int unknown_8;                     // +0x8
    char unknown_c[0x1c - 0xc];
    int unknown_1c;                    // +0x1c
    int value;                         // +0x20
    char unknown_24[0xa4 - 0x24];
};

class Class_004b0a10 {
public:
    char unknown_0[0x1c];
    Channel_004b0a10 channels[8];      // +0x1c
    int activeCount;                   // +0x53c

    int FUN_004b0a10(int id, int value, int update);
};

class Class_004b08c0 {
public:
    int FUN_004b08c0(int id);
};

class Class_004b0da0 {
public:
    void FUN_004b0da0(int channel, int param_2);
};

class Class_004b1c00 {
public:
    void FUN_004b1c00(int param_1);
};

// FUNCTION: 0x4b0a10
int Class_004b0a10::FUN_004b0a10(int id, int value, int update)
{
    int i = ((Class_004b08c0*)this)->FUN_004b08c0(id);
    if (i < 0)
        return 0;
    channels[i].value = value;
    if (update) {
        if (activeCount) {
            for (int j = 0; j < 8; j++)
                ((Class_004b0da0*)this)->FUN_004b0da0(j, 0);
        }
        ((Class_004b1c00*)this)->FUN_004b1c00(0);
    }
    return 1;
}
