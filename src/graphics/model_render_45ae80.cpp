// Decompiled by Haiku. Names are provisional.

struct Object3do {
    char unknown_0[0x2c];
    int unknown_2c;  // +0x2c
    int unknown_30;  // +0x30
};

// FUNCTION: 0x45ae80
int __stdcall CountObjects(Object3do* edi)
{
    int esi = 1;
    int eax;

    eax = edi->unknown_30;
    if (eax != 0) {
        esi = CountObjects((Object3do*)eax);
        esi++;
    }

    eax = edi->unknown_2c;
    if (eax != 0) {
        eax = CountObjects((Object3do*)eax);
        esi = esi + eax;
    }

    return esi;
}
