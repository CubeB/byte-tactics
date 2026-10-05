// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

extern int DAT_0051ff10;
extern int DAT_0051ff20[];
extern int DAT_0050b544;
extern int DAT_0050b540;

extern int __stdcall FUN_004b64d0(int handle);
extern int __stdcall FUN_004b63f0(int delay, int id, void (__stdcall* callback)(void*));
extern void __stdcall FUN_004ce5e0(void* unused);

class Class_004cdb40 {
public:
    void FUN_004cdb40();
};

class Class_004d00d0 {
public:
    int FUN_004d00d0(int volume, int temporary);
};

class Class_004ce690 {
public:
    char unknown_0[0x20];
    int field_20;                      // +0x20
    char unknown_24[0x1fc - 0x24];
    int field_1fc;                     // +0x1fc
    char unknown_200[0x208 - 0x200];
    int field_208;                     // +0x208
    char unknown_20c[0x278 - 0x20c];
    int field_278;                     // +0x278
    char unknown_27c[0x284 - 0x27c];
    int field_284;                     // +0x284

    void FUN_004ce690(int mode);
};

// FUNCTION: 0x4ce690
void Class_004ce690::FUN_004ce690(int mode)
{
    int old = field_278;
    if (old == mode)
        return;
    if (old >= 0)
        DAT_0051ff20[old] = field_208;
    field_278 = mode;
    if (field_1fc == 4 || mode == 2 || mode == 3) {
        DAT_0051ff10 = field_20;
        if (old == 4) {
            if (DAT_0050b544 >= 0) {
                FUN_004b64d0(DAT_0050b544);
                DAT_0050b544 = -1;
            }
            if (DAT_0050b540 >= 0) {
                FUN_004b64d0(DAT_0050b540);
                DAT_0050b540 = -1;
            }
            ((Class_004d00d0*)this)->FUN_004d00d0(field_20, 0);
            ((Class_004cdb40*)this)->FUN_004cdb40();
        } else {
            if (DAT_0050b544 >= 0) {
                FUN_004b64d0(DAT_0050b544);
                DAT_0050b544 = -1;
                FUN_004b64d0(DAT_0050b540);
                DAT_0050b540 = -1;
                ((Class_004cdb40*)this)->FUN_004cdb40();
            } else {
                field_284 = field_20 / -18;
                DAT_0050b544 = FUN_004b63f0(2, 0, FUN_004ce5e0);
            }
        }
    }
}
