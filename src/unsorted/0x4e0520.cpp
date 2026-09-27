// Decompiled by Opus. Names are provisional.

// Registry key helper: the constructor opens (or creates) a key under
// HKCU\Software\Cavedog Entertainment, the destructor is empty.
// Note: 0x4e2cb0 is this class's (empty, out-of-line) destructor; it is
// called with ecx = the local key object at the end of its scope.
class Class_004e2be0 {
public:
    int key;                           // +0x00
    unsigned char readOnly;            // +0x04
    Class_004e2be0(int readOnly, char* app, char* section);
    ~Class_004e2be0();
};

class Class_004e2fe0 {
public:
    void FUN_004e2fe0(char* name, unsigned char* value, unsigned char def);
};

extern char* DAT_0050d72c;

class Class_004e0520 {
public:
    char unknown_0[0x79];
    unsigned char workingSet;          // +0x79
    void FUN_004e0520(int readOnly);
};

// FUNCTION: 0x4e0520
void Class_004e0520::FUN_004e0520(int readOnly)
{
    Class_004e2be0 key(readOnly, DAT_0050d72c, "CavedogLibrary");
    ((Class_004e2fe0*)&key)->FUN_004e2fe0("WorkingSet", &workingSet, 0);
}
