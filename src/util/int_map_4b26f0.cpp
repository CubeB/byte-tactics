// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free. Names are provisional.
//
// This is the game code that does `return g_map[key];` for a global
// std::map<int,int> living at 0x51fbc0 (its static _Nil sentinel is
// DAT_0051fbbc).
#include <map>

// Keep the real <map>: hand-written tree classes break the match.
typedef std::map<int, int> Map_004b26f0;

extern Map_004b26f0 DAT_0051fbc0;

// FUNCTION: 0x4b26f0
int __stdcall GetCobChecksum(int key)
{
    return DAT_0051fbc0[key];
}
