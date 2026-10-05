// Decompiled by space-bunny-free. Names are provisional.
// IsKeyDown: maps a key code to a Windows virtual key code and reports
// whether it is held down. GetAsyncKeyState is a short, so the helper keeps a
// short result: that is what makes the compiler test the low word with
// `neg ax` and only mask the low byte. The cases are listed in the order the
// original emits their blocks; the jump table sorts the case values itself.
#include <windows.h>

static short key_state(int vk)
{
    return GetAsyncKeyState(vk) & 0xfffe;
}

// FUNCTION: 0x4c1b80
int __stdcall IsKeyDown(int key)
{
    switch (key)
    {
    case 0xfb:  return key_state(0x12) != 0;
    case 0xf9:  return key_state(0x10) != 0;
    case 0xfa:  return key_state(0x11) != 0;
    case 0xf5:  return key_state(0x26) != 0;
    case 0xf7:  return key_state(0x28) != 0;
    case 0xf4:  return key_state(0x25) != 0;
    case 0xf6:  return key_state(0x27) != 0;
    case 0x20:  return key_state(0x20) != 0;
    }
    return 0;
}
