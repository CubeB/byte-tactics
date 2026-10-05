// Decompiled by space-bunny-free. Names are provisional.
// Releases the per-unit-type data tables: the array at g_game+0x14377, the
// 0x249-byte entries of g_game+0x1439b (with their script and file members),
// and the object at g_game+0x1437b. The loop starts at index 1 because entry 0
// is not a real unit type.
#pragma pack(push, 1)

struct UnitType_0042db90 {
    char unknown_0[0x14e];
    void* field_14e;                   // +0x14e
    int field_152;                     // +0x152
    void* field_156;                   // +0x156
    char unknown_15a[0x18e - 0x15a];
    void* field_18e;                   // +0x18e
    char unknown_192[0x249 - 0x192];
};

class Class_004581c0 {
public:
    char unknown_0[0x10];
    void* ptr;                         // +0x10

    void FUN_004581c0();
};

struct Game {
    char unknown_0[0x14377];
    void** field_14377;                // +0x14377
    Class_004581c0* field_1437b;       // +0x1437b
    char unknown_1437f[0x1438f - 0x1437f];
    int field_1438f;                   // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0042db90* field_1439b;    // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d8780(void* param_1);
void __cdecl FUN_004d85a0(int* param_1);
void __stdcall FUN_004b2540(void* param_1);

// FUNCTION: 0x42db90
void FreeUnitTypes()
{
    FUN_004d8780(g_game->field_1439b);
    FUN_004d8780(g_game->field_14377);

    for (unsigned short i = 1; i < g_game->field_1438f; i++) {
        UnitType_0042db90* type = &g_game->field_1439b[i];
        void* p = g_game->field_14377[i];
        if (p != 0) {
            FUN_004d85a0((int*)p);
            g_game->field_14377[i] = 0;
        }
        if (type->field_14e != 0) {
            FUN_004d85a0((int*)type->field_14e);
            type->field_14e = 0;
        }
        if (type->field_18e != 0) {
            FUN_004b2540(type->field_18e);
            type->field_18e = 0;
        }
        if (type->field_156 != 0) {
            FUN_004d85a0((int*)type->field_156);
            type->field_152 = 0;
            type->field_156 = 0;
        }
    }

    Class_004581c0* obj = g_game->field_1437b;
    if (obj != 0) {
        obj->FUN_004581c0();
        delete obj;
    }
    g_game->field_1437b = 0;

    FUN_004d85a0((int*)g_game->field_14377);
    FUN_004d85a0((int*)g_game->field_1439b);

    g_game->field_14377 = 0;
    g_game->field_1439b = 0;
}
