// Decompiled by Haiku. Names are provisional.

extern void* DAT_0051e690;
extern void* DAT_0051e694;
extern char* g_game;

class Class_004cf150 {
public:
    void FUN_004cf150();
};

extern void FUN_0049f640();

// FUNCTION: 0x47f750
void FUN_0047f750() {
    if (DAT_0051e690 == 0) {
        void* game_obj = *(void**)g_game;
        Class_004cf150* obj = (Class_004cf150*)((char*)game_obj + 0x10);
        obj->FUN_004cf150();
    }
    if (DAT_0051e694 != 0) {
        FUN_0049f640();
    }
}
