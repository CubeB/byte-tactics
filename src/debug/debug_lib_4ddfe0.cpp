// Decompiled by Opus. Names are provisional.
// Sibling of 0x4ddfa0: the number of FPO records in the image (the size of
// debug directory type 3 divided by sizeof(FPO_DATA)), or 0 if it has none.

struct DebugDir_004ddfa0 {
    unsigned int characteristics;      // +0x00
    unsigned int timeDateStamp;        // +0x04
    unsigned short majorVersion;       // +0x08
    unsigned short minorVersion;       // +0x0a
    unsigned int type;                 // +0x0c
    unsigned int sizeOfData;           // +0x10
    unsigned int addressOfRawData;     // +0x14
    unsigned int pointerToRawData;     // +0x18
};

struct Fpo_004de020 {
    unsigned int offStart;             // +0x0
    unsigned int procSize;             // +0x4
    unsigned int locals;               // +0x8
    unsigned short params;             // +0xc
    unsigned short flags;              // +0xe
};

class Class_004ddfe0 {
public:
    char unknown_0[0x8];
    char* base;                        // +0x08, the mapped file
    char unknown_c[0x24 - 0xc];
    DebugDir_004ddfa0* debugDirs;      // +0x24
    int numDebugDirs;                  // +0x28

    unsigned int GetFpoRecordCount();
};

// FUNCTION: 0x4ddfe0
unsigned int Class_004ddfe0::GetFpoRecordCount()
{
    if (debugDirs == 0)
        return 0;
    for (int i = 0; i < numDebugDirs; i++) {
        if (debugDirs[i].type == 3)
            return debugDirs[i].sizeOfData / sizeof(Fpo_004de020);
    }
    return 0;
}
