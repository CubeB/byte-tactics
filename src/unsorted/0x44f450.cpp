// Decompiled by Sonnet. Names are provisional.

class Class_0040e9c0 {
public:
    void FUN_0040e9c0(void* param);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14207];
    Class_0040e9c0* field_14207;        // +0x14207
};
#pragma pack(pop)

class Class_0044f450 {
public:
    void* vtable;

    void FUN_0044f450();
};

extern Game* g_game;
extern void* DAT_004fd458[];
extern void* DAT_004fd428[];

// FUNCTION: 0x44f450
void Class_0044f450::FUN_0044f450()
{
    vtable = DAT_004fd458;
    g_game->field_14207->FUN_0040e9c0(this);
    vtable = DAT_004fd428;
}
