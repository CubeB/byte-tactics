// Decompiled by Opus. Names are provisional.

// One FPO_DATA record from an image's debug directory (IMAGE_DEBUG_TYPE_FPO).
struct Fpo_004de020 {
    unsigned int offStart;             // +0x0
    unsigned int procSize;             // +0x4
    unsigned int locals;               // +0x8
    unsigned short params;             // +0xc
    unsigned short flags;              // +0xe
};

class Class_004ddfa0 {
public:
    Fpo_004de020* GetFpoRecords();
};

class Class_004ddfe0 {
public:
    int GetFpoRecordCount();
};

class Class_004de020 {
public:
    char unknown_0[0x18];
    unsigned int imageBase;            // +0x18
    Fpo_004de020* FindFpoRecord(unsigned int address);
};

// Binary search of the FPO records for the one covering an address.
// FUNCTION: 0x4de020
Fpo_004de020* Class_004de020::FindFpoRecord(unsigned int address)
{
    Fpo_004de020* first = ((Class_004ddfa0*)this)->GetFpoRecords();
    if (first == 0)
        return 0;
    int n = ((Class_004ddfe0*)this)->GetFpoRecordCount();
    if (n == 0)
        return 0;
    Fpo_004de020* last = first + n;
    Fpo_004de020* mid = first + n / 2;
    unsigned int rva = address - imageBase;
    while (first + 1 != last) {
        if (rva < mid->offStart)
            last = mid;
        else
            first = mid;
        mid = first + (last - first) / 2;
    }
    if (rva >= first->offStart && rva < first->offStart + first->procSize)
        return first;
    return 0;
}
