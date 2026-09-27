// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Receives one network packet into the shared buffer (g_game+0x2a38),
// growing the buffer ("PACKET DATA AGAIN") while the receive call returns
// 0x8877001e (buffer too small), through DAT_0051e300 when DAT_00506dbc is
// set, else through DirectPlay (FUN_004c9840), and counts it in the
// network statistics.
//
// PARTIAL (99.0%). The only difference: before FUN_00415f40(size, 0, 0)
// the original loads size after the two `push 0` (`mov ecx, [esp+8]`);
// ours loads it first (`mov ecx, [esp]; push 0; push 0`).
//
// Notes from Claude Opus 5.5 (#295): flat at 99.0% for 0 to 400 unused
// externs, up to 8000 unused prototypes, every headers.py set and
// <windows.h> with <string>, <vector> + <map>, <iostream> or <dplay.h>, so
// it is the source shape. MSVC hoists a load of an address-taken local
// above constant pushes in every form tried; in the whole exe a load after
// constant pushes right after a call is rare and, where the source is
// known (0x41f0a0's `entries`), it is the reload of a spilled register
// variable. Tried and ruled out: unsigned/unsigned long/array/struct size,
// int& receive parameters, inline helpers for either or both stats calls
// (by value, by pointer, with default-style arguments), a copy local,
// `*psize` through a pointer, a comma return, `result` as the zeros,
// break instead of return, and each receive loop as an inline helper.

#pragma pack(push, 1)
struct Game_004534e0 {
    char unknown_0[0x2a34];
    int field_2a34;                    // +0x2a34
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x2a44 - 0x2a3c];
    unsigned char flags_2a44;          // +0x2a44
};
#pragma pack(pop)

extern Game_004534e0* g_game;
extern int DAT_00506dbc;

class Class_00462f30 {
public:
    int FUN_00462f30(void* net, unsigned char* data, int* size);
};

extern Class_00462f30 DAT_0051e300;

int __stdcall FUN_004c9840(void* net, void* data, int* size);
void __stdcall FUN_00415ef0(unsigned char kind, int amount, int player);
void __stdcall FUN_00415f40(int size, int overhead, int sent);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);

// FUNCTION: 0x4534e0
int FUN_004534e0(void)
{
    int size;

    if (!(g_game->flags_2a44 & 1))
        return 0;

    size = g_game->field_2a34;
    if (DAT_00506dbc != 0) {
        while (1) {
            int result = DAT_0051e300.FUN_00462f30((char*)g_game + 0x14, g_game->buffer, &size);
            if (result == 0) {
                FUN_00415ef0(*g_game->buffer, size, 0);
                return 1;
            }
            if (result == 0x887700be)
                return 0;
            if (result != 0x8877001e)
                return 0;
            g_game->field_2a34 = size;
            g_game->buffer = (unsigned char*)FUN_004d84a0(g_game->buffer, "PACKET DATA AGAIN", size);
        }
    } else {
        while (1) {
            int result = FUN_004c9840((char*)g_game + 0x14, g_game->buffer, &size);
            if (result == 0) {
                FUN_00415ef0(*g_game->buffer, size, 0);
                FUN_00415f40(size, 0, 0);
                return 1;
            }
            if (result == 0x887700be)
                return 0;
            if (result != 0x8877001e)
                return 0;
            g_game->field_2a34 = size;
            g_game->buffer = (unsigned char*)FUN_004d84a0(g_game->buffer, "PACKET DATA AGAIN", size);
        }
    }
}
