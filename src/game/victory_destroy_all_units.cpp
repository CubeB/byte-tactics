// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1df2];
    short field_1df2;                  // +0x1df2
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall PlaySoundByName(char* str, int flag);

class HapiBank {
public:
    void OpenAccount(const char* name);
    void SetIntegerItem(const char* name, int value);
    int GetIntegerItem(char* name, int def);
};

extern char DAT_00508f60[]; // "VictoryCondition_DestroyAllUnits"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class VictoryDestroyAllUnits {
public:
    int satisfied;                     // +0x4
    int celebrated;                    // +0x8

    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 0 (IsSatisfied) of the "destroy all units" victory condition (vtable
// 0x4fd948, state saved by 0x48eb80): satisfied while the game field at
// +0x1df2 is zero, announcing "Victory Condition" once.
// FUNCTION: 0x48eb40
int VictoryDestroyAllUnits::IsSatisfied()
{
    if (g_game->field_1df2 == 0) {
        if (celebrated == 0) {
            PlaySoundByName("Victory Condition", 0);
            celebrated = 1;
        }
        return 1;
    }
    return 0;
}

// Same shape as 0x48eac0: writes the "destroy all units" victory condition's
// state to a section.
// FUNCTION: 0x48eb80
void VictoryDestroyAllUnits::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_00508f60);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// Same shape as 0x48faf0: reads the "destroy all units" victory condition's
// state from a section.
// FUNCTION: 0x48ebc0
void VictoryDestroyAllUnits::LoadState(HapiBank* obj)
{
    obj->OpenAccount("VictoryCondition_DestroyAllUnits");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
