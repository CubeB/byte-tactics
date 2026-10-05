// Decompiled by Sonnet. Names are provisional.

class Class_00462370 {
public:
    int count;          // +0
    char unknown_4[4];
    int writeIdx;       // +8
    int buf[0x400];     // +0xc

    int FUN_00462370(int value);
};

// FUNCTION: 0x462370
int Class_00462370::FUN_00462370(int value)
{
    if (count < 0x400) {
        writeIdx = writeIdx + 1;
        if (writeIdx >= 0x400) {
            writeIdx = 0;
        }
        buf[writeIdx] = value;
        count = count + 1;
        return 1;
    }
    return 0;
}
