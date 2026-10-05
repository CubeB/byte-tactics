// Decompiled by Opus, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. re-verified by GPT-6, matched by Claude Opus 5.5. Names are provisional.
// Deletes every entry of the global vector at DAT_00511fb4 (see 0x422460),
// then the vector itself. 0x424c00 inlines this whole function (its copy
// calls the vector's _Destroy out of line, 0x4251e0).
//
// The vector pointer is a file-scope `static` in this TU (0x4222e0 fills it,
// 0x422460 searches it, 0x424c00 inlines this function). With an `extern`
// declaration every spelling stayed at 65.9%: MSVC must assume the inlined
// ~vector's stores through the vector could change the global, so it keeps
// the destructor's `this` in esi and the zero in ebx. Declared `static`, the
// plain `delete` gives the original's `lea esi,[eax+4]` / `lea ebx,[eax+8]`
// for &_First and &_Last, the immediate zeros and `test esi,esi` in the loop.
#include <vector>

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

typedef std::vector<Class_004c2ea0*> FeatureList;

static FeatureList* DAT_00511fb4;

// FUNCTION: 0x4223e0
void FreeFeatureFileList()
{
    for (Class_004c2ea0** p = DAT_00511fb4->begin(); p < DAT_00511fb4->end(); p++)
        delete *p;
    delete DAT_00511fb4;
    DAT_00511fb4 = 0;
}
