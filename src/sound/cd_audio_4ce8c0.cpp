// Decompiled by Opus. Names are provisional.

class Class_004ce8c0 {
public:
    char unknown_0[0x200];
    int count;          // +0x200
    char unknown_204[4];
    int current;        // +0x208
    int mode;           // +0x20c
    int SelectTrack(int index);
};

class Sound {
public:
    void PlayCdTrack(int index, int flag);
};

// FUNCTION: 0x4ce8c0
int Class_004ce8c0::SelectTrack(int index)
{
    if (count == 0)
        return 0;
    if (index > count)
        index = index % count;
    if (mode == 1) {
        ((Sound*)this)->PlayCdTrack(index, 1);
        return current;
    }
    current = index;
    return index;
}
