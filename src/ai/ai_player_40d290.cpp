// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash and space-bunny-free, finished by GPT-6, matched by Claude Opus 5.5. Names are provisional.
// std::vector<unsigned char>::insert(iterator, size_type, const T&), with
// _Ucopy, _Ufill, fill and copy_backward all inlined. 0x409160 calls it from
// the inlined resize() of the vector at +0x9d (next to its erase, 0x40d470).
// The headers below and ta_types.h last set the symbol ids the bytes follow.
#include <windows.h>
#include <shlobj.h>
#include <imagehlp.h>
#include <d3d.h>
#include <memory.h>
#include <math.h>
#include "ta_types.h"

// The member pointer's type is spelled inline (no typedef): it shifts symbol ids.
// FUNCTION: 0x40d290 ?insert@?$vector@EV?$allocator@E@std@@@std@@QAEXPAEIABE@Z
void (std::vector<unsigned char>::*g_insert_0040d290)(unsigned char*, unsigned int,
                                                      const unsigned char&) = &std::vector<unsigned char>::insert;
