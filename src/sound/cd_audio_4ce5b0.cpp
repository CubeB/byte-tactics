// Decompiled by Haiku. Names are provisional.

extern int DAT_0050b540;
extern void* DAT_0051ff14;

extern void __stdcall RemoveTimer(int);

class Class_004cdb40 {
public:
    void FUN_004cdb40();
};

// FUNCTION: 0x4ce5b0
void __stdcall FUN_004ce5b0(void*)
{
    int temp = DAT_0050b540;
    RemoveTimer(temp);
    DAT_0050b540 = 0xffffffff;
    ((Class_004cdb40*)DAT_0051ff14)->FUN_004cdb40();
}
