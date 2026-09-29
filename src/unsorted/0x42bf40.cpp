// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL (10 min timebox, 4772 byte function). Transcribed through 0x42c417
// (the `bmcode` read): prologue, UNITINFO open, unitname/name/description,
// defaultmissiontype, the four *badTargetCategory string pointers, objectname
// (with the strcpy fallback from unitname), buildcostenergy/metal, the eight
// FUN_004c4800 int fields, turnrate/waterline/transportsize/transportcapacity,
// energymake/energyuse/metalmake/extractsmetal, makesmetal, the three
// generators + storages, buildtime/workertime/healtime/maxdamage, the four
// distance shorts and bmcode. NOT reproduced: the two flag dwords at
// def+0x241/0x245, standingmoveorder onwards, category/soundcategory, corpse,
// movementclass, the yardmap switch, weapon names and the closing frame math.
//
// Two known differences from the original (the big ones):
//   1. Register allocation. The original keeps `unitdef` in ebp and also saves
//      ebp in the prologue (push ebx; push ebp; push esi; push edi), so the
//      incoming args are at [esp+0x52c]/[esp+0x530]. Ours keeps unitdef in esi
//      and saves only ebx/esi/edi, so every [ebp+K] is [esi+K] and the args
//      sit 4 bytes lower. This is why the transcribed body still does not
//      align: the instruction shapes are right but the memory operands differ.
//      A fixed ebp/spill of unitdef would likely jump the score sharply.
//   2. Frame size. The original frame is 0x518 because local_4a0 (32 bytes,
//      movementclass), local_480 (128, weapon names) and a 1024-byte YardMap
//      buffer are all live. We do not reach those uses yet, so MSVC dropped
//      them and the frame collapsed to 0x78, shifting every stack offset by
//      0x4a0. To reproduce the original frame without those uses, `buf` below
//      is deliberately oversized (1284 instead of 100) so the frame is exactly
//      0x518; replace it with the real buffers once the tail is written.
//
// Established facts:
//   signature: __stdcall FUN(char* fbi_file, char* unitdef)   (ret 0x8, 2 args)
//   local Class_004c2ea0 parser at esp+0 (0xc bytes: field_0, current at +4, field_8)
//   parser.current is a TDF node; it is the `this` for the getters:
//     Class_004c48c0::FUN_004c48c0(dst, key, size, def)  string
//     Class_004c46c0::FUN_004c46c0(key, def) -> int
//     Class_004c4800::FUN_004c4800(out, key, def) -> int* (caller reads *ret)
//     Class_004c4760::FUN_004c4760(key, def, def2) -> float  (3 args, ret 0xc;
//       both defs are pushed as 0 in this function)
//   free helpers: FUN_004c58a0(parser, dst, key, size, def) __stdcall
//                 FUN_00488c50(char*) __stdcall -> void*
//   Class_00438760 ctor at 0x438760 converts a string into a 1-byte enum
//     (caller reads byte 0 of the constructed object).
//   unitdef offsets: +0x20 unitname[0x20], +0x40 description[0x40],
//     +0x80 objectname, +0x186 float buildcostenergy, +0x18a float buildcostmetal,
//     +0x192 maxvelocity, +0x19a brakerate, +0x19e acceleration, +0x1a2 bankscale,
//     +0x1a6 pitchscale, +0x1aa damagemodifier, +0x1ae/+0x1b2 moverate1/2,
//     +0x1ba turnrate short, +0x22a transportsize, +0x22b transportcapacity,
//     +0x22c waterline, +0x1c2..+0x1e6 resource floats, +0x1ea buildtime,
//     +0x1fa maxdamage, +0x1fe/+0x200 workertime/healtime, +0x202..+0x20c
//     distance shorts, +0x22d makesmetal, +0x22f bmcode,
//     +0x230 defaultmissiontype char, +0x231/+0x235/+0x239/+0x23d badTargetCategory,
//     flag dwords at +0x241 and +0x245.

class Class_004c2ea0 {
public:
    int field_0;
    void* current;                      // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, int size, char* def);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(char* key, int def);
};

class Class_004c4800 {
public:
    int* FUN_004c4800(int* out, char* key, int def);
};

class Class_004c4760 {
public:
    float FUN_004c4760(char* key, int def, int def2);
};

class Class_00438760 {
public:
    char value;                         // +0x0
    Class_00438760(char* text);
};

extern char DAT_005119b8[];
extern char DAT_00503ea0[];

void __stdcall FUN_004c58a0(void* parser, char* dst, char* key, int size, char* def);
void* __stdcall FUN_00488c50(char* text);
char* strcpy(char* dst, const char* src);

// FUNCTION: 0x42bf40
void __stdcall FUN_0042bf40(char* fbi_file, char* unitdef)
{
    Class_004c2ea0 parser;
    int scratch;
    char temp;
    char pad[3];
    char buf[1284];                     // oversized to pin the frame at 0x518

    if (((Class_004c2f60*)&parser)->FUN_004c2f60(fbi_file)) {
        if (((Class_004c3410*)&parser)->FUN_004c3410("UNITINFO")) {
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(unitdef + 0x20, "unitname", 0x20, DAT_005119b8);
            FUN_004c58a0(&parser, unitdef, "name", 0x20, 0);
            FUN_004c58a0(&parser, unitdef + 0x40, "description", 0x40, 0);
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(buf, "defaultmissiontype", 100, DAT_005119b8);
            Class_00438760 conv(buf);
            unitdef[0x230] = conv.value;
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(buf, "wpri_badTargetCategory", 100, DAT_00503ea0);
            *(void**)(unitdef + 0x231) = FUN_00488c50(buf);
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(buf, "wsec_badTargetCategory", 100, DAT_00503ea0);
            *(void**)(unitdef + 0x235) = FUN_00488c50(buf);
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(buf, "wspe_badTargetCategory", 100, DAT_00503ea0);
            *(void**)(unitdef + 0x239) = FUN_00488c50(buf);
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(buf, "noChaseCategory", 100, DAT_00503ea0);
            *(void**)(unitdef + 0x23d) = FUN_00488c50(buf);
            if (((Class_004c48c0*)parser.current)->FUN_004c48c0(unitdef + 0x80, "objectname", 0x20, DAT_005119b8) == 0) {
                strcpy(unitdef + 0x80, unitdef + 0x20);
            }
            scratch = ((Class_004c46c0*)parser.current)->FUN_004c46c0("buildcostenergy", 0);
            *(float*)(unitdef + 0x186) = (float)scratch;
            scratch = ((Class_004c46c0*)parser.current)->FUN_004c46c0("buildcostmetal", 0);
            *(float*)(unitdef + 0x18a) = (float)scratch;
            *(int*)(unitdef + 0x192) = *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "maxvelocity", 0);
            *(int*)(unitdef + 0x19a) = *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "brakerate", 0);
            *(int*)(unitdef + 0x19e) = *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "acceleration", 0);
            *(int*)(unitdef + 0x1a2) = *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "bankscale", 0x10000);
            *(int*)(unitdef + 0x1a6) = *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "pitchscale", 0);
            *(int*)(unitdef + 0x1aa) = *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "damagemodifier", 0x10000);
            *(int*)(unitdef + 0x1ae) = *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "moverate1", *(int*)(unitdef + 0x192) * 2);
            *(int*)(unitdef + 0x1b2) = *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "moverate2", *(int*)(unitdef + 0x192) * 2);
            *(short*)(unitdef + 0x1ba) = (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("turnrate", 0);
            unitdef[0x22c] = (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("waterline", 0);
            unitdef[0x22a] = (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("transportsize", 0);
            unitdef[0x22b] = (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("transportcapacity", 0);
            *(float*)(unitdef + 0x1c2) = ((Class_004c4760*)parser.current)->FUN_004c4760("energymake", 0, 0);
            *(float*)(unitdef + 0x1c6) = ((Class_004c4760*)parser.current)->FUN_004c4760("energyuse", 0, 0);
            *(float*)(unitdef + 0x1ca) = ((Class_004c4760*)parser.current)->FUN_004c4760("metalmake", 0, 0);
            *(float*)(unitdef + 0x1ce) = ((Class_004c4760*)parser.current)->FUN_004c4760("extractsmetal", 0, 0);
            unitdef[0x22d] = (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("makesmetal", 0);
            *(float*)(unitdef + 0x1d2) = ((Class_004c4760*)parser.current)->FUN_004c4760("windgenerator", 0, 0);
            *(float*)(unitdef + 0x1d6) = ((Class_004c4760*)parser.current)->FUN_004c4760("tidalgenerator", 0, 0);
            *(float*)(unitdef + 0x1e2) = ((Class_004c4760*)parser.current)->FUN_004c4760("energystorage", 0, 0);
            *(float*)(unitdef + 0x1e6) = ((Class_004c4760*)parser.current)->FUN_004c4760("metalstorage", 0, 0);
            *(int*)(unitdef + 0x1ea) = ((Class_004c46c0*)parser.current)->FUN_004c46c0("buildtime", 0);
            *(short*)(unitdef + 0x1fe) = (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("workertime", 0);
            *(short*)(unitdef + 0x200) = (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("healtime", 0);
            *(int*)(unitdef + 0x1fa) = ((Class_004c46c0*)parser.current)->FUN_004c46c0("maxdamage", 0);
            *(short*)(unitdef + 0x202) = (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("sightdistance", 0);
            *(short*)(unitdef + 0x204) = (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("radardistance", 0);
            *(short*)(unitdef + 0x206) = (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("sonardistance", 0);
            *(short*)(unitdef + 0x20a) = (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("radardistancejam", 0);
            *(short*)(unitdef + 0x20c) = (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("sonardistancejam", 0);
            unitdef[0x22f] = (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("bmcode", 0);
            ((Class_004c3240*)&parser)->FUN_004c3240();
        } else {
            ((Class_004c3240*)&parser)->FUN_004c3240();
        }
    }
    ((Class_004c2ea0*)&parser)->~Class_004c2ea0();
}
