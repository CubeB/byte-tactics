// Decompiled by Haiku. Names are provisional.

class Class_004623b0 {
public:
    int count;
    int index;
    int unused;
    int buffer[0x400];

    int FUN_004623b0();
};

// FUNCTION: 0x4623b0
int Class_004623b0::FUN_004623b0()
{
    if (count > 0) {
        count--;
        int value = buffer[index];
        index++;
        if (index < 0x400) {
            return value;
        }
        index = 0;
        return value;
    }
    return 0;
}
