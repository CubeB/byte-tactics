// Decompiled by Haiku. Names are provisional.

// The timer's elapsed-time getter (0x4e1e30), a method on the same object.
class Timer {
public:
    double GetElapsedSeconds();
};

struct Class_004e2150 {
public:
    double field_0;
    char unknown_8[0x40];
    unsigned char field_48;
    
    void StopTimer();
};

// FUNCTION: 0x4e2150
void Class_004e2150::StopTimer()
{
    field_0 = ((Timer*)this)->GetElapsedSeconds();
    field_48 = 1;
}
