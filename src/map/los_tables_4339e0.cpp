// Decompiled by Haiku. Names are provisional.

class Class_004339e0 {
public:
    void GetLosLineStep(short index, unsigned short* out1, unsigned short* out2);
};

// FUNCTION: 0x4339e0
void Class_004339e0::GetLosLineStep(short index, unsigned short* out1, unsigned short* out2)
{
    int idx = index;
    unsigned char* base = (unsigned char*)*(void**)((char*)this + 4);
    unsigned char* ptr = base + idx * 4;
    unsigned short val1 = *(unsigned short*)ptr;
    *out1 = val1;
    unsigned short val2 = *(unsigned short*)(ptr + 2);
    *out2 = val2;
}
