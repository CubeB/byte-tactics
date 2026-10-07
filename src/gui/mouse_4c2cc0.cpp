// Decompiled by Opus. Names are provisional.
// Shuts down the object GetDisplay returns: when flag bit 10 is set and
// the worker at +0x1ca is running, asks it to stop (+0x1d6) and waits up to
// 20 x 100 ms for it to acknowledge; then releases the three objects at
// +0x1be..+0x1c6 and frees the event buffer at +0x18a (see 0x4c2d60).

#pragma pack(push, 2)
struct Input_004c2cc0 {
    char unknown_0[0xf0];
    unsigned short bit0_9 : 10;        // +0xf0
    unsigned short active : 1;         // +0xf0, bit 10
    unsigned short bit11_15 : 5;
    char unknown_f2[0x18a - 0xf2];
    void* entries;                     // +0x18a
    char unknown_18e[0x1be - 0x18e];
    void* obj_1be;                     // +0x1be
    void* obj_1c2;                     // +0x1c2
    void* obj_1c6;                     // +0x1c6
    int running;                       // +0x1ca
    char unknown_1ce[0x1d6 - 0x1ce];
    int stopping;                      // +0x1d6
};
#pragma pack(pop)

Input_004c2cc0* GetDisplay(void);
void __stdcall SleepMilliseconds(unsigned int param_1);
void __stdcall FreeSurface(void* param_1);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4c2cc0
void ShutdownMouse(void)
{
    Input_004c2cc0* o = GetDisplay();
    if (o->entries) {
        // Separate nested ifs, not `&&`: it changes how the bit is tested.
        if (o->active) {
            if (o->running) {
                o->stopping = 1;
                int i = 0;
                // while (1) keeps the loop test at the top.
                while (1) {
                    if (++i > 20)
                        break;
                    SleepMilliseconds(100);
                    if (!o->stopping) {
                        o->running = 0;
                        break;
                    }
                }
            }
        }
        FreeSurface(o->obj_1c6);
        FreeSurface(o->obj_1c2);
        FreeSurface(o->obj_1be);
        FUN_004d85a0(o->entries);
        o->entries = 0;
    }
}
