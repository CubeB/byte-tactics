// Decompiled by Sonnet. Names are provisional.
// Constructor of a memory-mapped-file wrapper: initialises the handle/state
// fields, then opens the file if a name was given (see FUN_004e1590).

class Class_004e1590 {
public:
    void FUN_004e1590(const char* fileName);
};

class Class_004e1560 {
public:
    void* hFile;      // +0x0
    void* hMapping;   // +0x4
    void* view;       // +0x8
    int size;         // +0xc
    int state;        // +0x10

    Class_004e1560(const char* fileName);
};

// FUNCTION: 0x4e1560
Class_004e1560::Class_004e1560(const char* fileName)
{
    hFile = (void*)-1;
    hMapping = 0;
    view = 0;
    size = 0;
    state = 0;
    if (fileName != 0)
        ((Class_004e1590*)this)->FUN_004e1590(fileName);
}
