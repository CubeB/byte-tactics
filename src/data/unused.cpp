// Data the original holds that no code reads: constants a compiled function
// left behind (its object keeps every constant its source names, even one
// the optimiser folded away), variables nothing uses, and tables of menu
// options whose menus are gone. Each keeps its place and value; none has a
// name the code gives it.

// The constants 2.0f and 2.0 among other objects' constants: between
// 0x407d40's and 0x408cb0's, 0x4158d0's and 0x414770's, 0x43d290's and
// 0x43f0e0's, and among 0x49a890's own.
// GLOBAL: 0x4fc978
extern const float DAT_004fc978 = 2.0f;
// GLOBAL: 0x4fcc48
extern const double DAT_004fcc48 = 2.0;
// GLOBAL: 0x4fd2e4
extern const float DAT_004fd2e4 = 2.0f;
// GLOBAL: 0x4fda70
extern const double DAT_004fda70 = 2.0;

// Before the "Code segment checksum error" message.
// GLOBAL: 0x502f98
int DAT_00502f98 = 16;

// Before 0x477ab0's flag at 0x507b6c.
// GLOBAL: 0x507b64
int DAT_00507b64 = 0;
// GLOBAL: 0x507b68
int DAT_00507b68 = 1;

// Menu options: a label with the setting's value in it, and the value, each
// list ending with a null label.
struct OptionChoice {
    const char* label;
    int value;
};

// Speeds.
// GLOBAL: 0x5066f8
OptionChoice DAT_005066f8[4] = {
    {"%d  Slow", -16},
    {"%d  Normal", 16},
    {"%d  Fast", 64},
    {0, -1},
};

// Rates of fire.
// GLOBAL: 0x506718
OptionChoice DAT_00506718[4] = {
    {"%d  Sluggish", -5},
    {"%d  Normal", 5},
    {"%d  Rapid Fire", 20},
    {0, -1},
};

// How much the units say.
// GLOBAL: 0x506738
OptionChoice DAT_00506738[7] = {
    {"No Chat", 0},
    {"Only important info", 2},
    {"Medium chat", 4},
    {"Detailed chat", 6},
    {"High chat level", 8},
    {"All info chatted", 10},
    {0, -2},
};

// The CD music modes, by their names in the game's settings; 0x45d130 and
// 0x45d280 use the first one's string, DAT_005067bc, themselves.
extern char DAT_005067bc[8];

// GLOBAL: 0x506770
const char* DAT_00506770[6] = {DAT_005067bc, "NORMTRAK", "RANDTRAK", "REPTTRAK", "SPECTRAK", 0};

// The wave formats a sound device may support: its WAVEOUTCAPS.dwFormats bit
// (WAVE_FORMAT_1M08 and the rest), a description, and the format itself,
// ending with an empty entry. Among the sound driver's data (0x4ceb60 to
// 0x4d01b0), which reads none of it.
#pragma pack(push, 1)
struct WaveFormatName {
    unsigned int flag;
    const char* name;
    unsigned char bits;
    unsigned char channels;
    unsigned int rate;
};
#pragma pack(pop)

// GLOBAL: 0x50b6e0
WaveFormatName DAT_0050b6e0[13] = {
    {0x1, "11.025 kHz, mono, 8-bit", 8, 1, 11025},
    {0x4, "11.025 kHz, mono, 16-bit", 16, 1, 11025},
    {0x2, "11.025 kHz, stereo, 8-bit", 8, 2, 11025},
    {0x8, "11.025 kHz, stereo, 16-bit", 16, 2, 11025},
    {0x10, "22.05 kHz, mono, 8-bit", 8, 1, 22050},
    {0x40, "22.05 kHz, mono, 16-bit", 16, 1, 22050},
    {0x20, "22.05 kHz, stereo, 8-bit", 8, 2, 22050},
    {0x80, "22.05 kHz, stereo, 16-bit", 16, 2, 22050},
    {0x100, "44.1 kHz, mono, 8-bit", 8, 1, 44100},
    {0x400, "44.1 kHz, mono, 16-bit", 16, 1, 44100},
    {0x200, "44.1 kHz, stereo, 8-bit", 8, 2, 44100},
    {0x800, "44.1 kHz, stereo, 16-bit", 16, 2, 44100},
    {0, 0, 0, 0, 0},
};
