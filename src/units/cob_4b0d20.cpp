// Decompiled by Sonnet. Names are provisional.

struct Entry_004b0d20 {
    unsigned int flags;
    char unknown_4[0x20 - 4];
    int handle;
    char unknown_24[0xa4 - 0x24];
};

class CobScript {
public:
    char unknown_0[0x1c];
    Entry_004b0d20 entries[8];
    int guard_53c;

    void RemoveCallback(int handle);
};

// FUNCTION: 0x4b0d20
void CobScript::RemoveCallback(int handle)
{
    if (guard_53c != 0) {
        for (int i = 0; i < 8; i++) {
            if ((entries[i].flags & 0xff000000) != 0 && entries[i].handle == handle) {
                entries[i].handle = 0;
            }
        }
    }
}
