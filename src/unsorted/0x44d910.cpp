// Decompiled by Haiku. Names are provisional.

extern void* DAT_004fd2f8;
extern void __cdecl operator delete(void*);

class Class_0044d910 {
public:
    virtual ~Class_0044d910() {}

    Class_0044d910* FUN_0044d910(unsigned char should_delete);
};

// FUNCTION: 0x44d910
Class_0044d910* Class_0044d910::FUN_0044d910(unsigned char should_delete)
{
    Class_0044d910* esi = this;
    *(void**)esi = &DAT_004fd2f8;
    if (should_delete & 1) {
        operator delete(esi);
    }
    return esi;
}
