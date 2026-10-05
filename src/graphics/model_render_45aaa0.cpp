// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Frees the entries of an object state block (one 0x36-byte entry per piece),
// clears its two ids from the global id-registry buffer, then frees the block.

class CMemoryCache {
public:
    void ReleaseHandle(int id);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1437b];
    CMemoryCache* obj;         // +0x1437b
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d85a0(void* p);

#pragma pack(push, 2)
struct Entry_0045aaa0 {
    void* ptr;                 // +0x0
    char unknown_4[0x32];
};

struct ObjectState_0045aaa0 {
    int count;                 // +0x0
    char unknown_4[0xc];
    int field_10;              // +0x10
    int field_14;              // +0x14
    char unknown_18[0x2c];
    Entry_0045aaa0 entries[1]; // +0x44
};
#pragma pack(pop)

// FUNCTION: 0x45aaa0
void __stdcall FreeObjectState(ObjectState_0045aaa0* state)
{
    for (int i = 0; i < state->count; i++) {
        FUN_004d85a0(state->entries[i].ptr);
    }
    if (g_game->obj != 0) {
        g_game->obj->ReleaseHandle((int)&state->field_10);
        g_game->obj->ReleaseHandle((int)&state->field_14);
    }
    FUN_004d85a0(state);
}
