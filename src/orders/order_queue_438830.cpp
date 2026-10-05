// Decompiled by Haiku. Names are provisional.

extern int DAT_00512344;

struct Class_00438830;

struct Class_00438830
{
public:
    int FUN_00438830();
};

// FUNCTION: 0x438830
int Class_00438830::FUN_00438830()
{
    unsigned int result = 0;
    result = *(unsigned char*)this;
    int* base = (int*)&DAT_00512344;
    int fives = result * 5;
    return *base + fives * 4 + fives;
}
