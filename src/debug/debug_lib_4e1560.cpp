// Decompiled by Sonnet. Names are provisional.
// Constructor of a memory-mapped-file wrapper: initialises the handle/state
// fields, then opens the file if a name was given (see OpenMappedFile).

class Class_004e1590 {
public:
    void OpenMappedFile(const char* fileName);
};

class MappedFile {
public:
    void* hFile;      // +0x0
    void* hMapping;   // +0x4
    void* view;       // +0x8
    int size;         // +0xc
    int state;        // +0x10

    MappedFile(const char* fileName);
};

// FUNCTION: 0x4e1560
MappedFile::MappedFile(const char* fileName)
{
    hFile = (void*)-1;
    hMapping = 0;
    view = 0;
    size = 0;
    state = 0;
    if (fileName != 0)
        ((Class_004e1590*)this)->OpenMappedFile(fileName);
}
