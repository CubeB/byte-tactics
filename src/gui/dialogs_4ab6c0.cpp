// Decompiled by Opus. Names are provisional.
// Stores the length of a text in a control (or empties the text when it is
// too long or the flag is set), then refreshes the control. MSVC duplicates
// the shared call into both branches itself.
#include <string.h>

struct Control_004ab6c0 {
    char unknown_0[0x74];
    int textLength;                    // +0x74
};

void __stdcall DrawTextInput(Control_004ab6c0* control, void* param_2);

// FUNCTION: 0x4ab6c0
void __stdcall FUN_004ab6c0(Control_004ab6c0* control, void* param_2, char* text, int maxLength, int clear)
{
    int length = strlen(text);
    if (clear == 0 && length <= maxLength) {
        control->textLength = length;
    } else {
        *text = 0;
        control->textLength = 0;
    }
    DrawTextInput(control, param_2);
}
