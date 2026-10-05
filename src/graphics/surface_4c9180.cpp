// Decompiled by Haiku. Names are provisional.

extern int g_emptyStringRefs;
extern void* g_emptyString;

class Class_004c9180
{
public:
    void* vtable;
    Class_004c9180();
};

// FUNCTION: 0x4c9180
Class_004c9180::Class_004c9180()
{
    g_emptyStringRefs++;
    vtable = &g_emptyString;
}
