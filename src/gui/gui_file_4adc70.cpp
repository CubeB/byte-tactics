// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Reads a GUI control's TDF entry: status, text, quickkey, grayedout and
// stages, into the control object. Argument 1 is the control, argument 2 is
// the TDF source (its +4 is the key/value table).
#include <windows.h>
#include <stdlib.h>
#include <string.h>

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

struct Source_004adc70 {
    char unknown_0[4];
    Class_004c48c0* tdf;               // +0x4
};

struct Obj_004adc70 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    unsigned char stages;              // +0x136
    char unknown_137;                  // +0x137
    short status;                      // +0x138
    unsigned char quickkey;            // +0x13a
    char unknown_13b;                  // +0x13b
    unsigned short grayedout;          // +0x13c
};

extern char DAT_005119b8[];

char* __stdcall FUN_004c5740(char* text);

// FUNCTION: 0x4adc70
void __stdcall ReadButtonFields(Obj_004adc70* obj, Source_004adc70* src)
{
    obj->status = (short)((Class_004c46c0*)src->tdf)->FUN_004c46c0("status", 0);
    memset(obj->text, 0, 0x80);
    src->tdf->FUN_004c48c0(obj->text, "text", 0x80, DAT_005119b8);
    strncpy(obj->text, FUN_004c5740(obj->text), 0x80);
    char local[20];
    src->tdf->FUN_004c48c0(local, "quickkey", 0x13, DAT_005119b8);
    if (IsCharAlphaA(local[0]))
        obj->quickkey = (unsigned char)local[0];
    else
        obj->quickkey = (unsigned char)atoi(local);
    int gray = ((Class_004c46c0*)src->tdf)->FUN_004c46c0("grayedout", 0);
    obj->grayedout = (gray ^ obj->grayedout) & 1 ^ obj->grayedout;
    obj->stages = (unsigned char)((Class_004c46c0*)src->tdf)->FUN_004c46c0("stages", 0);
}
