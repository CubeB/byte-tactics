// Decompiled by Opus. Names are provisional.
// Frees three buffers (clearing each pointer and its size) and a fourth
// buffer at +0xc14. Written out in full: an inline Free() helper keeps the
// two clearing stores together instead of letting them sink past the next
// load.

void FUN_004d85a0(int* param_1);

struct Buffer_00437280 {
    int* data;
    int size;
};

class Class_00437280 {
public:
    char unknown_0[0xc14];
    int* field_c14;                    // +0xc14
    char unknown_c18[0xdac - 0xc18];
    Buffer_00437280 buffer0;           // +0xdac
    Buffer_00437280 buffer1;           // +0xdb4
    Buffer_00437280 buffer2;           // +0xdbc

    void FUN_00437280();
};

// FUNCTION: 0x437280
void Class_00437280::FUN_00437280()
{
    if (buffer0.data)
        FUN_004d85a0(buffer0.data);
    buffer0.data = 0;
    buffer0.size = 0;
    if (buffer1.data)
        FUN_004d85a0(buffer1.data);
    buffer1.data = 0;
    buffer1.size = 0;
    if (buffer2.data)
        FUN_004d85a0(buffer2.data);
    buffer2.data = 0;
    buffer2.size = 0;
    if (field_c14) {
        FUN_004d85a0(field_c14);
        field_c14 = 0;
    }
}
