// Decompiled by Sonnet. Names are provisional.

class Base {
public:
    virtual void unused0(int);
    virtual void Func(int param);
};

#pragma pack(push, 1)
class SquadManager {
public:
    char unknown_0[0x11];
    Base* ptrs[10];

    void DeleteTimers();
};
#pragma pack(pop)

// FUNCTION: 0x408f10
void SquadManager::DeleteTimers()
{
    for (int i = 0; i < 10; i++) {
        if (ptrs[i] != 0) {
            ptrs[i]->Func(1);
        }
    }
}
