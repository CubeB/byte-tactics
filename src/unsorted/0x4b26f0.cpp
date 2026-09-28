// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// This is the game code that does `return g_map[key];` for a global
// std::map<int,int> living at 0x51fbc0 (its static _Nil sentinel is
// DAT_0051fbbc). MSVC 5 /Ob2 inlines map::operator[], map::insert and
// _Tree::insert here, leaving _Tree::_Insert (0x4b3020) out of line; the
// resulting bytes are identical to the original's 336.
//
// check.py reports the three callees that the inlined template code leaves
// out of line (0x4b2840 = _Tree::begin, 0x4b34f0 = iterator::_Dec,
// 0x4b3000 = pair<iterator,bool>'s constructor) as "BAD" references only
// because data/symbols.csv filed them under the provisional placeholder names
// Class_004b2840::FUN_004b2840 / Class_004b34f0::FUN_004b34f0 /
// Class_004b3000::FUN_004b3000. This file's symbols are the real template
// members, so the reference names differ even though every byte matches.
// Rewriting the body by hand with those placeholder classes fixes the
// references but drops the byte match to 38%, so the real <map> source is kept.
#include <map>

typedef std::map<int, int> Map_004b26f0;

extern Map_004b26f0 DAT_0051fbc0;

// FUNCTION: 0x4b26f0
int __stdcall FUN_004b26f0(int key)
{
    return DAT_0051fbc0[key];
}
