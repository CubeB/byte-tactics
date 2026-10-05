// Decompiled by Haiku. Names are provisional.
extern void* FUN_004e1410();

struct Class_004e05f0 {
public:
    void SetMemoryStatusWindowVisible(int param_1);
};

// FUNCTION: 0x4e1400
void ShowMemoryStatus()
{
    ((Class_004e05f0*)FUN_004e1410())->SetMemoryStatusWindowVisible(1);
}
