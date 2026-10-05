// Decompiled by Opus. Names are provisional.

class MovementClass {
public:
    char unknown_0[0x10];
    unsigned int width;             // +0x10
    unsigned int height;            // +0x14
    int* cells;                     // +0x18

    void ResizePassMap(unsigned int w, unsigned int h);
};

// FUNCTION: 0x440470
void MovementClass::ResizePassMap(unsigned int w, unsigned int h)
{
    width = w;
    height = h;
    unsigned int n = ((h + 15) >> 4) * w;
    delete cells;
    if (n != 0) {
        cells = new int[n];
    } else {
        cells = 0;
    }
}
