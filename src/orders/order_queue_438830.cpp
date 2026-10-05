// Decompiled by Haiku. Names are provisional.

extern int DAT_00512344;

struct OrderType;

struct OrderType
{
public:
    int FUN_00438830();
};

// FUNCTION: 0x438830
int OrderType::FUN_00438830()
{
    unsigned int result = 0;
    result = *(unsigned char*)this;
    int* base = (int*)&DAT_00512344;
    int fives = result * 5;
    return *base + fives * 4 + fives;
}
