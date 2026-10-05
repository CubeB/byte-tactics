// Decompiled by Haiku. Names are provisional.
extern void* GetPerformanceWindow();

struct Class_004df280 {
public:
    void SetPerformanceWindowVisible(int param_1);
};

// FUNCTION: 0x4dfd00
void ShowPerformanceStatus()
{
    ((Class_004df280*)GetPerformanceWindow())->SetPerformanceWindowVisible(1);
}
