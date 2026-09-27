// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x41da10
void __stdcall FUN_0041da10(int param1, int param2, int param3)
{
    unsigned char al;
    if (param3 == 0) {
        al = 0;
    } else {
        al = 0xff;
    }
    al &= 0xb;
    al += 0x4c;
    *(unsigned char*)(param2 + param1) = al;
}
