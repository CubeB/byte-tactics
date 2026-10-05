// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash and space-bunny-free, finished by GPT-6, matched by Claude Opus 5.5. Names are provisional.
// MATCH (Claude Opus 5.5, #5668). std::vector<unsigned char>::insert(iterator,
// size_type, const T&) from MSVC 5's <vector>, with _Ucopy, _Ufill, fill and
// copy_backward all inlined. 0x409160 calls it from the inlined resize() of
// the vector at +0x9d (next to its erase, 0x40d470). Taking the member's
// address makes the compiler emit the template instantiation out of line.
//
// The source never changed; the last three hunks (the `_End` add, the `_Ufill`
// count and the `_Last` sum) are decided by symbol ids (#5544, #5565, #5635,
// #5662):
//  * The insert's own front-end id must be even and in 65540..65654, so the
//    vector<unsigned char> instantiation straddles 65536, and the ids given at
//    the end of the file (the body's locals, _N/_S/this, the inlined helpers)
//    must have _N even.
//  * The instantiation is numbered where the first game type holding a
//    std::vector<unsigned char> is defined. That is Owner, the AI translation
//    unit's own class (vec_8d, vec_9d), so in the original it sat in that
//    unit's own header, after the common ones. include/ta_types.h has the
//    unit's 40 own types (every view in files 0x407350 to 0x40d5b0) last,
//    written by `tools/gametypes.py --reorder include/ta_types.h --last
//    0x407350-0x40d5b0`, which puts the insert at 65604 behind the headers
//    below.
//  * The headers in front (<memory.h> can be <malloc.h> or <direct.h>, or
//    <process.h> <io.h> for <memory.h> <math.h>) and the member pointer's type
//    written in its declaration (an InsertFn typedef shifts the end ids by
//    one, giving 98.1%) put _N at 66554. The same from a second directory.
// Earlier notes (/Gi, the tail spellings, the extern scans, the explicit
// specialisation that compiles byte-identical) are in this file's history.
#include <windows.h>
#include <shlobj.h>
#include <imagehlp.h>
#include <d3d.h>
#include <memory.h>
#include <math.h>
#include "ta_types.h"

// FUNCTION: 0x40d290 ?insert@?$vector@EV?$allocator@E@std@@@std@@QAEXPAEIABE@Z
void (std::vector<unsigned char>::*g_insert_0040d290)(unsigned char*, unsigned int,
                                                      const unsigned char&) = &std::vector<unsigned char>::insert;
