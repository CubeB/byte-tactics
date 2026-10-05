// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Saving counterpart of Class_0044d470's constructor: writes a 24-byte header
// whose last five dwords are the stored values. The first dword is left
// uninitialised, exactly as in 0x44d090 and 0x44d9a0.

class Class_004b4ba0 {
public:
    int OpenNamedBox(char* name);
};

class Class_004b4c10 {
public:
    void SeekBox(int pos);
};

class Class_004b4cf0 {
public:
    int WriteBox(void* src, int len);
};

struct Data_0044d470 {
    int a;
    int b;
    int c;
    int d;
    int e;
};

struct Header_0044d470 {
    int magic;                         // +0x0
    Data_0044d470 data;                // +0x4
};

class Class_0044d470 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4
    Data_0044d470 data;                // +0x8

    int FUN_0044d500(int unused, Class_004b4ba0* file, char* name);
};

// FUNCTION: 0x44d500
int Class_0044d470::FUN_0044d500(int unused, Class_004b4ba0* file, char* name)
{
    Header_0044d470 hdr;
    hdr.data.a = data.a;
    hdr.data.b = data.b;
    hdr.data.c = data.c;
    hdr.data.d = data.d;
    hdr.data.e = data.e;
    file->OpenNamedBox(name);
    ((Class_004b4c10*)file)->SeekBox(0);
    ((Class_004b4cf0*)file)->WriteBox(&hdr, 24);
    return 1;
}
