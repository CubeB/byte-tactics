// Decompiled by space-bunny-free. Names are provisional.

// A display mode (width, height, refresh rate), and the list of the 8-bit
// modes the driver reports (see FUN_004b5330, the EnumDisplayModes callback
// that fills one in). FUN_0045e4c0 sorts the list by (width, height) with a
// selection sort, then drops every mode smaller than 640x480.
struct Mode_0045e4c0 {
    int width;                       // +0x0
    int height;                      // +0x4
    int refreshRate;                 // +0x8
};

struct ModeList_0045e4c0 {
    int count;                       // +0x0
    Mode_0045e4c0* modes;            // +0x4
};

// FUNCTION: 0x45e4c0
void __stdcall FUN_0045e4c0(ModeList_0045e4c0* list)
{
    int i;
    int j;
    for (i = 0; i < list->count; i++) {
        for (j = list->count - 1; j > i; j--) {
            if (list->modes[j].width < list->modes[i].width
                || (list->modes[j].width == list->modes[i].width
                    && list->modes[j].height < list->modes[i].height)) {
                Mode_0045e4c0 tmp = list->modes[i];
                list->modes[i] = list->modes[j];
                list->modes[j] = tmp;
            }
        }
    }
    for (i = 0; i < list->count; i++) {
        if (list->modes[i].width < 640 || list->modes[i].height < 480) {
            if (i < list->count - 1) {
                for (j = i; j < list->count - 1; j++)
                    list->modes[j] = list->modes[j + 1];
            }
            i--;
            list->count--;
        }
    }
}
