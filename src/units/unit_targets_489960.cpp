// Decompiled by Haiku. Names are provisional.

class Unit {
public:
    int CanReclaim(void *param_1);
};

// FUNCTION: 0x489960
int Unit::CanReclaim(void *param_1)
{
    void *ptr = *(void **)((char *)this + 0x92);
    unsigned int val1 = *(unsigned int *)((char *)ptr + 0x245);

    if ((val1 & 0x400) != 0) {
        unsigned int val2 = *(unsigned int *)((char *)param_1 + 0x110);

        if ((val2 & 3) != 2) {
            void *ptr2 = *(void **)((char *)param_1 + 0x92);
            unsigned int val3 = *(unsigned int *)((char *)ptr2 + 0x245);

            if ((val3 & 0x1000) == 0) {
                return 1;
            }
        }
    }

    return 0;
}
