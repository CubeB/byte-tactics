// Decompiled by space-bunny-free. Names are provisional.
#include <windows.h>

void* operator new(unsigned int size);
void FUN_00461170(const char* fmt, ...);

class Class_00462470 {
public:
    int field_0;
    char unknown_4[0x18];
    int field_1c;
    char unknown_20[4];
    int field_24;
    char unknown_28[0x1044 - 0x28];

    void FUN_00461db0(int a1, int a2, int a3, int a4);
};

struct Channel_4619e0 {
    unsigned long pacing;        // +0
    char unknown_4[0x1044 - 4];
};

extern int DAT_00506dbc;

class Class_004619e0 {
public:
    int field_0;
    unsigned long m_defaultSendPacingMs;   // +4
    char unknown_8[4];
    Channel_4619e0 channels[11];           // +0xc

    void ResetAll(int a, int b)
    {
        ((Class_00462470*)this)->FUN_00461db0(0, m_defaultSendPacingMs, a, b);
        for (int i = 1; i < 11; i++) {
            ((Class_00462470*)&channels[i])->FUN_00461db0(-1, m_defaultSendPacingMs, 2, 100);
        }
    }

    void FUN_004619e0(int rate)
    {
        if (rate < 0) {
            DAT_00506dbc = 0;
            return;
        }
        if (rate == 0) {
            m_defaultSendPacingMs = 200;
        } else {
            if (rate < 2) {
                rate = 2;
            } else if (rate > 30) {
                rate = 30;
            }
            m_defaultSendPacingMs = 1000 / rate;
        }
        FUN_00461170("setting m_defaultSendPacingMs to: %lums\n", m_defaultSendPacingMs);
        for (int i = 0; i < 11; i++) {
            channels[i].pacing = (m_defaultSendPacingMs * 30 + 999) / 1000;
        }
    }
};

struct Buffer_461020 {
    char unknown_0[0xc];
    int field_c;
};

extern Class_004619e0 DAT_00513000;     // the send pacing object
extern int DAT_00512c94;
extern Buffer_461020* DAT_0051e314;
extern void* DAT_0051e318;
extern void* DAT_0051e31c;
extern int DAT_0051e528;
extern int DAT_0051e52c;

// FUNCTION: 0x461020
int __stdcall FUN_00461020(int param_1, int param_2)
{
    if (DAT_00512c94 != 0) {
        DAT_00513000.FUN_004619e0(DAT_00512c94);
    }
    if (DAT_00506dbc != 0) {
        if (DAT_0051e318 == 0) {
            if (DAT_0051e31c != 0) {
                DAT_0051e318 = DAT_0051e31c;
                DAT_0051e31c = 0;
            } else {
                DAT_0051e318 = operator new(0x42a);
                if (DAT_0051e318 != 0) {
                    DAT_0051e528 = 0x42a;
                } else {
                    return 0;
                }
            }
        }
        DAT_0051e52c = 0;
        if (DAT_0051e314 != 0) {
            DAT_0051e314->field_c = 0;
            DAT_0051e314 = 0;
        }
        DAT_00513000.ResetAll(param_1, param_2);
        SetThreadPriority(GetCurrentThread(), -2);
    }
    return 1;
}
