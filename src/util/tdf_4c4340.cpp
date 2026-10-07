// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds a reference-counted string handle (see 0x4c91b0 for the constructor
// from a C string, 0x4c91a0 for the copy constructor, 0x4c9390 for the
// destructor) out of the text between start and end, with leading and trailing
// whitespace trimmed. The handle is constructed in place at out, which is
// returned.
extern char DAT_005119b8[];

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 {
public:
    char* ptr;

    Class_004c91b0(const char* text);
};

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

class Class_004c4340 {
public:
    Class_004c91a0* MakeTrimmedString(Class_004c91a0* out, char* start, char* end);
};

// FUNCTION: 0x4c4340
Class_004c91a0* Class_004c4340::MakeTrimmedString(Class_004c91a0* out, char* start, char* end)
{
    char* p = start;
    while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n'))
        p++;
    start = p;
    if (start >= end) {
        Class_004c91b0 tmp(DAT_005119b8);
        out->Class_004c91a0::Class_004c91a0(*(Class_004c91a0*)&tmp);
        ((Class_004c9390*)&tmp)->ReleaseRef();
        return out;
    }
    char* last = end - 1;
    while (*last == ' ' || *last == '\t' || *last == '\r' || *last == '\n')
        last--;
    char saved = last[1];
    last[1] = 0;
    Class_004c91b0 tmp(start);
    last[1] = saved;
    out->Class_004c91a0::Class_004c91a0(*(Class_004c91a0*)&tmp);
    ((Class_004c9390*)&tmp)->ReleaseRef();
    return out;
}
