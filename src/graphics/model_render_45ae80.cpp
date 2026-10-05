// Decompiled by Haiku. Names are provisional.

struct Class_0045ae80 {
    char unknown_0[0x2c];
    int unknown_2c;  // +0x2c
    int unknown_30;  // +0x30
};

// FUNCTION: 0x45ae80
int __stdcall CountObjects(Class_0045ae80* edi)
{
    int esi = 1;
    int eax;

    eax = edi->unknown_30;
    if (eax != 0) {
        esi = CountObjects((Class_0045ae80*)eax);
        esi++;
    }

    eax = edi->unknown_2c;
    if (eax != 0) {
        eax = CountObjects((Class_0045ae80*)eax);
        esi = esi + eax;
    }

    return esi;
}
