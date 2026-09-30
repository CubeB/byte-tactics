// Decompiled by Opus. Names are provisional.
// Frees a global buffer, clears three globals and deletes the object at
// g_game+0x391e9.

void __cdecl FUN_004d85a0(void* p);

class Class_00434f70 {
public:
    ~Class_00434f70();
};

extern void* DAT_005122d4;
extern int DAT_005122d8;
extern int DAT_005122dc;
extern int DAT_005122e0;
extern char* g_game;

// FUNCTION: 0x434b90
void FUN_00434b90()
{
    if (DAT_005122d4) {
        FUN_004d85a0(DAT_005122d4);
        DAT_005122d4 = 0;
    }
    DAT_005122d8 = 0;
    DAT_005122dc = 0;
    DAT_005122e0 = 0;
    delete *(Class_00434f70**)(g_game + 0x391e9);
    *(Class_00434f70**)(g_game + 0x391e9) = 0;
}
