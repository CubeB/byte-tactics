// Decompiled by Opus. Names are provisional.
// Calls FUN_004cf570 on the same object with the global flag at 0x51ff48 set
// for the duration of the call.

class Class_004cf570 {
public:
    void FUN_004cf570(int a, int b, int c);
};

class Class_004cf540 {
public:
    void FUN_004cf540(int a, int b);
};

extern int DAT_0051ff48;

// FUNCTION: 0x4cf540
void Class_004cf540::FUN_004cf540(int a, int b)
{
    DAT_0051ff48 = 1;
    ((Class_004cf570*)this)->FUN_004cf570(a, b, 0);
    DAT_0051ff48 = 0;
}
