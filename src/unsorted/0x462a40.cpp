// Decompiled by Haiku. Names are provisional.

struct Entry
{
public:
    char unknown_0[0xc];
    void* owner;
    char unknown_10[0xc];
    Entry* next;
};

struct Class_00462a40
{
public:
    char unknown_0[0x8];
    Entry* field_8;
    char unknown_c[0x4];
    Entry* field_10;

    void FUN_00462a40();
};

// FUNCTION: 0x462a40
void Class_00462a40::FUN_00462a40()
{
    if (field_8 != 0) {
        Entry* eax = field_10;
        while (eax != 0 && eax->owner == this) {
            eax = eax->next;
        }
    }
}
