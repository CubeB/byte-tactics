// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1. Names are provisional.
// Pass 3 (deepseek-v4.1): 64.7%, ours 4716 bytes against 4772.
// Confirmed: the original's single `xor esi,esi` at 0x42c0f0 initialises the
// soundcategory loop counter (`int sound = 0;`), which stays in esi across the
// whole function; the original then serves every zero default argument from
// that live zero register (`push esi`, 85 sites) and the `cmp ..., esi` tests
// at 0x42cbde/0x42cc5f, instead of materialising its own zero constant.
// Declaring `int sound = 0;` right after the objectname block (where the
// original's xor sits) and removing the `int def = 0;` temp raised the score
// from 62.6% to 64.7% and shrank our body from 4804 to 4716 bytes.
// Remaining first divergence: MSVC still materialises its own zero register in
// ebx in our build (`xor ebx,ebx` at the start of the getter block, then
// `push ebx` at the default sites), where the original pushes esi; the
// FUN_004c4800 group rematerialises `xor eax,eax; push eax` in the original
// while ours pushes ebx.
// Tried this pass, all still 64.7%: passing the `sound` variable itself as the
// default argument at the FUN_004c46c0/FUN_004c4760 sites; declaring
// `int number = 0;` early; moving `unsigned int value` down to its first
// assignment. Inlining the FUN_004c46c0 float conversions (dropping the named
// `scratch` local) grew the frame by 4 so every esp+N shifted, 62.3%.
// Still different: the Class_00438760 temp is read as `mov al,[eax]` in ours
// where the original loads `mov al,[esp+0x2b]` from its byte-packed slot.

// Still 62.6% (ours 4804 bytes against 4772) after deepseek-v4.1's second pass.
// Confirmed fixed this pass (both match the original now):
//   - the Class_00438760 conversion temp must be an unnamed temporary in the
//     assignment expression (`unitdef[0x230] = Class_00438760(buf).value;`): as a
//     named local it gets a 4-aligned slot at [esp+0x20], as a temporary it lands
//     byte-packed at [esp+0x23] with its byte at [esp+0x2b], exactly the original.
//   - the three extent stores at unitdef+0x176/+0x17a/+0x17e go through one
//     `int* p = (int*)(unitdef + 0x176);` with p[0]/p[1]/p[2], matching the
//     original's lea esi,[ebp+0x176] / mov edi,esi / mov [edi],eax / [edi+4] /
//     [edi+8]; it also let the four /2 divisions keep w,h in registers.
//   - `int w`/`int h` locals for the width/height movsx reduce that hunk further
//     (still +12 bytes though).
// Still different: the 0 default argument (original: xor esi,esi once at 0x42c0f0
// plus 83 `push esi`, and `xor eax,eax; push eax` at the FUN_004c4800 sites; ours
// has `push 0` at each; every initialized-local shape MSVC5 folds) so all esp
// offsets drift by one pushed dword in the FUN_004c46c0 float region (ours filds
// at [esp+0x24] where the original filds at [esp+0x1c]) and the tail jumps land
// 20 bytes late. YardMap loop: original keeps the outer y counter in [esp+0x1c]
// and x in edi, ours keeps y in edi.
// The dominant difference is the shared zero default argument. The whole
// original function contains ZERO `push 0` and 83 `push esi`: it sets
// `xor esi,esi` once at 0x42c0f0 after the objectname strcpy test and pushes
// esi for every default (the FUN_004c46c0 group, FUN_004c4760's two defaults,
// FUN_004c58a0's char* default). The FUN_004c4800 group instead rematerialises
// its default in eax (`xor eax,eax; push eax`, 7 times) alongside
// `mov eax,0x10000` for the 0x10000 defaults.
// Tried (all compile, all byte-identical at 62.2%):
//   - one `int def = 0;` at the top of the function used at all 81 zero-default
//     sites; MSVC5 constant-folds it and emits `push 0` at all 90 sites.
//   - the same local declared at point of use (after the objectname test, where
//     the original's xor esi,esi sits) instead of at the top.
//   - a second `unsigned int value = 0;` for the 7 FUN_004c4800 defaults
//     (original rematerialises those in eax): also folded to `push 0`.
// The original's 58a0 name/description defaults are literal `push 0` (the
// xor esi,esi comes after them), so those two sites are right either way.
// A single initialised local is NOT the shape that produces the hoisted esi
// zero; the next attempt should look for the non-constant source of that zero
// (or for the instruction that makes esi live across the funnels, e.g. our
// extra `mov [ebp+0x166],esi` / `mov [ebp+0x176],edi` stores near 0x42d11x,
// which the original does not have: it keeps those values in registers, stores
// them through edi at 0x42d0fb-0x42d107 and stores 0x166 earlier at 0x42d0a5).
// Other confirmed differences:
//  1. The Class_00438760 conversion temp does not sit in its own slot: the
//     original builds it at [esp+0x23] with its char member at [esp+0x2b]
//     (this+8), overlapping the head of the `char buf[100]` slot (which starts
//     at [esp+0x24]). Ours places it at [esp+0x20] with the byte at [esp+0x28].
//  2. The bail-out jumps differ (0x42d154 vs 0x42d167) only because our body is
//     20 bytes longer; fix the instruction differences first, the offsets then
//     follow.
// Remaining: local-byte placement, flag-operation scheduling, yard-map control
// flow and branch differences, plus the tail /3 sequence at 0x42d10a-0x42d11f.
#include <windows.h>
#include <stdlib.h>
#include <string.h>
class Class_004c4630 {
  public:
    char* FUN_004c4630(char* key);
};
class Class_00488e70 {
  public:
    void FUN_00488e70(char* category);
};
class Class_004402e0 {
  public:
    char data[32];
    Class_004402e0();
    ~Class_004402e0();
};
class Class_00440320 {
  public:
    void FUN_00440340(void* parser);
};
extern char* g_game;
short __stdcall FUN_00422e40(char* name);
void* __stdcall FUN_00440420(char* name);
char* __stdcall FUN_0049e5b0(char* name);
void* __cdecl FUN_004d83b0(char* name, int size);

class Class_004c2ea0 {
  public:
    int field_0;
    void* current; // +0x4
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
    char value; // +0x0
    Class_00438760(char* text);
};

extern char DAT_005119b8[];
extern char DAT_00503ea0[];

void __stdcall FUN_004c58a0(void* parser, char* dst, char* key, int size, char* def);
void* __stdcall FUN_00488c50(char* text);

// FUNCTION: 0x42bf40
void __stdcall FUN_0042bf40(char* fbi_file, char* unitdef) {
    Class_004c2ea0 parser;
    int scratch;
    char buf[100];
    char weapon[128];
    char yard[1024];

    if (((Class_004c2f60*)&parser)->FUN_004c2f60(fbi_file)) {
        if (!((Class_004c3410*)&parser)->FUN_004c3410("UNITINFO")) {
            ((Class_004c3240*)&parser)->FUN_004c3240();
            goto FINISH;
        }
        {
            ((Class_004c48c0*)parser.current)
                ->FUN_004c48c0(unitdef + 0x20, "unitname", 0x20, DAT_005119b8);
            FUN_004c58a0(&parser, unitdef, "name", 0x20, 0);
            FUN_004c58a0(&parser, unitdef + 0x40, "description", 0x40, 0);
            ((Class_004c48c0*)parser.current)
                ->FUN_004c48c0(buf, "defaultmissiontype", 100, DAT_005119b8);
            unitdef[0x230] = Class_00438760(buf).value;
            ((Class_004c48c0*)parser.current)
                ->FUN_004c48c0(buf, "wpri_badTargetCategory", 100, DAT_00503ea0);
            *(void**)(unitdef + 0x231) = FUN_00488c50(buf);
            ((Class_004c48c0*)parser.current)
                ->FUN_004c48c0(buf, "wsec_badTargetCategory", 100, DAT_00503ea0);
            *(void**)(unitdef + 0x235) = FUN_00488c50(buf);
            ((Class_004c48c0*)parser.current)
                ->FUN_004c48c0(buf, "wspe_badTargetCategory", 100, DAT_00503ea0);
            *(void**)(unitdef + 0x239) = FUN_00488c50(buf);
            ((Class_004c48c0*)parser.current)
                ->FUN_004c48c0(buf, "noChaseCategory", 100, DAT_00503ea0);
            *(void**)(unitdef + 0x23d) = FUN_00488c50(buf);
            if (((Class_004c48c0*)parser.current)
                    ->FUN_004c48c0(unitdef + 0x80, "objectname", 0x20, DAT_005119b8) == 0) {
                strcpy(unitdef + 0x80, unitdef + 0x20);
            }
            int sound = 0;
            scratch = ((Class_004c46c0*)parser.current)->FUN_004c46c0("buildcostenergy", 0);
            *(float*)(unitdef + 0x186) = (float)scratch;
            scratch = ((Class_004c46c0*)parser.current)->FUN_004c46c0("buildcostmetal", 0);
            *(float*)(unitdef + 0x18a) = (float)scratch;
            *(int*)(unitdef + 0x192) =
                *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "maxvelocity", 0);
            *(int*)(unitdef + 0x19a) =
                *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "brakerate", 0);
            *(int*)(unitdef + 0x19e) =
                *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "acceleration", 0);
            *(int*)(unitdef + 0x1a2) =
                *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "bankscale", 0x10000);
            *(int*)(unitdef + 0x1a6) =
                *((Class_004c4800*)parser.current)->FUN_004c4800(&scratch, "pitchscale", 0);
            *(int*)(unitdef + 0x1aa) = *((Class_004c4800*)parser.current)
                                            ->FUN_004c4800(&scratch, "damagemodifier", 0x10000);
            *(int*)(unitdef + 0x1ae) =
                *((Class_004c4800*)parser.current)
                     ->FUN_004c4800(&scratch, "moverate1", *(int*)(unitdef + 0x192) * 2);
            *(int*)(unitdef + 0x1b2) =
                *((Class_004c4800*)parser.current)
                     ->FUN_004c4800(&scratch, "moverate2", *(int*)(unitdef + 0x192) * 2);
            *(short*)(unitdef + 0x1ba) =
                (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("turnrate", 0);
            unitdef[0x22c] = (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("waterline", 0);
            unitdef[0x22a] =
                (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("transportsize", 0);
            unitdef[0x22b] =
                (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("transportcapacity", 0);
            *(float*)(unitdef + 0x1c2) =
                ((Class_004c4760*)parser.current)->FUN_004c4760("energymake", 0, 0);
            *(float*)(unitdef + 0x1c6) =
                ((Class_004c4760*)parser.current)->FUN_004c4760("energyuse", 0, 0);
            *(float*)(unitdef + 0x1ca) =
                ((Class_004c4760*)parser.current)->FUN_004c4760("metalmake", 0, 0);
            *(float*)(unitdef + 0x1ce) =
                ((Class_004c4760*)parser.current)->FUN_004c4760("extractsmetal", 0, 0);
            unitdef[0x22d] = (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("makesmetal", 0);
            *(float*)(unitdef + 0x1d2) =
                ((Class_004c4760*)parser.current)->FUN_004c4760("windgenerator", 0, 0);
            *(float*)(unitdef + 0x1d6) =
                ((Class_004c4760*)parser.current)->FUN_004c4760("tidalgenerator", 0, 0);
            *(float*)(unitdef + 0x1e2) =
                ((Class_004c4760*)parser.current)->FUN_004c4760("energystorage", 0, 0);
            *(float*)(unitdef + 0x1e6) =
                ((Class_004c4760*)parser.current)->FUN_004c4760("metalstorage", 0, 0);
            *(int*)(unitdef + 0x1ea) =
                ((Class_004c46c0*)parser.current)->FUN_004c46c0("buildtime", 0);
            *(short*)(unitdef + 0x1fe) =
                (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("workertime", 0);
            *(short*)(unitdef + 0x200) =
                (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("healtime", 0);
            *(int*)(unitdef + 0x1fa) =
                ((Class_004c46c0*)parser.current)->FUN_004c46c0("maxdamage", 0);
            *(short*)(unitdef + 0x202) =
                (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("sightdistance", 0);
            *(short*)(unitdef + 0x204) =
                (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("radardistance", 0);
            *(short*)(unitdef + 0x206) =
                (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("sonardistance", 0);
            *(short*)(unitdef + 0x20a) =
                (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("radardistancejam", 0);
            *(short*)(unitdef + 0x20c) =
                (short)((Class_004c46c0*)parser.current)->FUN_004c46c0("sonardistancejam", 0);
            unitdef[0x22f] = (char)((Class_004c46c0*)parser.current)->FUN_004c46c0("bmcode", 0);
            unsigned int value2;
            int number;
            unsigned int value;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("standingmoveorder", 2);
            *(unsigned int*)(unitdef + 0x241) =
                (value ^ *(unsigned int*)(unitdef + 0x241)) & 3 ^ *(unsigned int*)(unitdef + 0x241);
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("standingfireorder", 2);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 3) << 2 | *(unsigned int*)(unitdef + 0x241) & 0xfffffff3;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("init_cloaked", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 4 | *(unsigned int*)(unitdef + 0x241) & 0xffffffef;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("downloadable", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 5 | *(unsigned int*)(unitdef + 0x241) & 0xffffffdf;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("builder", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 6 | *(unsigned int*)(unitdef + 0x241) & 0xffffffbf;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("stealth", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 8 | *(unsigned int*)(unitdef + 0x241) & 0xfffffeff;
            scratch = ((Class_004c46c0*)parser.current)->FUN_004c46c0("cloakcost", 0);
            *(float*)(unitdef + 0x1da) = (float)scratch;
            int cloakDefault = (int)*(float*)(unitdef + 0x1da);
            scratch =
                ((Class_004c46c0*)parser.current)->FUN_004c46c0("cloakcostmoving", cloakDefault);
            *(float*)(unitdef + 0x1de) = (float)scratch;
            number = ((Class_004c46c0*)parser.current)->FUN_004c46c0("mincloakdistance", 0);
            *(short*)(unitdef + 0x208) = (short)number;
            number = ((Class_004c46c0*)parser.current)->FUN_004c46c0("buildangle", 0);
            *(short*)(unitdef + 0x210) = (short)number;
            number = ((Class_004c46c0*)parser.current)->FUN_004c46c0("builddistance", 0);
            *(short*)(unitdef + 0x212) = (short)number;
            number = ((Class_004c46c0*)parser.current)->FUN_004c46c0("sortbias", 0);
            *(short*)(unitdef + 0x21a) = (short)number;
            number = ((Class_004c46c0*)parser.current)->FUN_004c46c0("cruisealt", 0);
            *(short*)(unitdef + 0x21c) = (short)number;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("zbuffer", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 7 | *(unsigned int*)(unitdef + 0x241) & 0xffffff7f;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("isairbase", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 9 | *(unsigned int*)(unitdef + 0x241) & 0xfffffdff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("istargetingupgrade", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 10 | *(unsigned int*)(unitdef + 0x241) & 0xfffffbff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("teleporter", 0);
            *(unsigned int*)(unitdef + 0x241) =
                *(unsigned int*)(unitdef + 0x241) & 0xffffdfff | (value & 1) << 0xd;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("hidedamage", 0);
            *(unsigned int*)(unitdef + 0x241) =
                *(unsigned int*)(unitdef + 0x241) & 0xffffbfff | (value & 1) << 0xe;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("shootme", 0);
            *(unsigned int*)(unitdef + 0x241) =
                *(unsigned int*)(unitdef + 0x241) & 0xffff7fff | (value & 1) << 0xf;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("armoredstate", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x11 | *(unsigned int*)(unitdef + 0x241) & 0xfffdffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("activatewhenbuilt", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x12 | *(unsigned int*)(unitdef + 0x241) & 0xfffbffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canfly", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0xb | *(unsigned int*)(unitdef + 0x241) & 0xfffff7ff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canhover", 0);
            *(unsigned int*)(unitdef + 0x241) =
                *(unsigned int*)(unitdef + 0x241) & 0xffffefff | (value & 1) << 0xc;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("upright", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x14 | *(unsigned int*)(unitdef + 0x241) & 0xffefffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("floater", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x13 | *(unsigned int*)(unitdef + 0x241) & 0xfff7ffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("amphibious", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x15 | *(unsigned int*)(unitdef + 0x241) & 0xffdfffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("isfeature", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x18 | *(unsigned int*)(unitdef + 0x241) & 0xfeffffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("noshadow", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x19 | *(unsigned int*)(unitdef + 0x241) & 0xfdffffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("immunetoparalyzer", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x1a | *(unsigned int*)(unitdef + 0x241) & 0xfbffffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("hoverattack", 0);
            *(unsigned int*)(unitdef + 0x241) =
                (value & 1) << 0x1b | *(unsigned int*)(unitdef + 0x241) & 0xf7ffffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("antiweapons", 0);
            *(unsigned int*)(unitdef + 0x241) =
                *(unsigned int*)(unitdef + 0x241) & 0xdfffffff | (value & 1) << 0x1d;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("digger", 0);
            *(unsigned int*)(unitdef + 0x241) =
                *(unsigned int*)(unitdef + 0x241) & 0xbfffffff | (value & 1) << 0x1e;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("onoffable", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 2 | *(unsigned int*)(unitdef + 0x245) & 0xfffffffb;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("mobilestandorders", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value ^ *(unsigned int*)(unitdef + 0x245)) & 1 ^ *(unsigned int*)(unitdef + 0x245);
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("firestandorders", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 1 | *(unsigned int*)(unitdef + 0x245) & 0xfffffffd;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canstop", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 3 | *(unsigned int*)(unitdef + 0x245) & 0xfffffff7;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canattack", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 4 | *(unsigned int*)(unitdef + 0x245) & 0xffffffef;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canguard", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 5 | *(unsigned int*)(unitdef + 0x245) & 0xffffffdf;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canpatrol", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 6 | *(unsigned int*)(unitdef + 0x245) & 0xffffffbf;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canmove", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 7 | *(unsigned int*)(unitdef + 0x245) & 0xffffff7f;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canload", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 8 | *(unsigned int*)(unitdef + 0x245) & 0xfffffeff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canreclamate", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 10 | *(unsigned int*)(unitdef + 0x245) & 0xfffffbff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("canresurrect", 0);
            *(unsigned int*)(unitdef + 0x245) = (*(unsigned int*)(unitdef + 0x245) & 0x400) >> 1 |
                                                (value & 1) << 0xb |
                                                *(unsigned int*)(unitdef + 0x245) & 0xfffff5ff;
            value2 = ((Class_004c46c0*)parser.current)->FUN_004c46c0("cancapture", 0);
            value = *(unsigned int*)(unitdef + 0x245);
            value2 = (value2 & 1) << 0xc;
            *(unsigned int*)(unitdef + 0x245) = value & 0xffffefff | value2;
            *(unsigned int*)(unitdef + 0x245) = value & 0xffffcfff | value2 |
                                                (unsigned int)(0.0 < *(float*)(unitdef + 0x1da))
                                                    << 0xd;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("candgun", 0);
            *(unsigned int*)(unitdef + 0x245) =
                *(unsigned int*)(unitdef + 0x245) & 0xffffbfff | (value & 1) << 0xe;
            number = ((Class_004c46c0*)parser.current)->FUN_004c46c0("maneuverleashlength", 0);
            *(short*)(unitdef + 0x214) = (short)number;
            number = ((Class_004c46c0*)parser.current)->FUN_004c46c0("attackrunlength", 0);
            *(short*)(unitdef + 0x216) = (short)number;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("kamikaze", 0);
            *(unsigned int*)(unitdef + 0x241) =
                *(unsigned int*)(unitdef + 0x241) & 0xefffffff | (value & 1) << 0x1c;
            number = ((Class_004c46c0*)parser.current)->FUN_004c46c0("kamikazedistance", 0);
            *(short*)(unitdef + 0x218) = (short)number;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("norestrict", 0);
            *(unsigned int*)(unitdef + 0x245) =
                *(unsigned int*)(unitdef + 0x245) & 0xffff7fff | (value & 1) << 0xf;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("showplayername", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 0x11 | *(unsigned int*)(unitdef + 0x245) & 0xfffdffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("commander", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 0x12 | *(unsigned int*)(unitdef + 0x245) & 0xfffbffff;
            value = ((Class_004c46c0*)parser.current)->FUN_004c46c0("cantbetransported", 0);
            *(unsigned int*)(unitdef + 0x245) =
                (value & 1) << 0x13 | *(unsigned int*)(unitdef + 0x245) & 0xfff7ffff;

            char* countdown =
                ((Class_004c4630*)parser.current)->FUN_004c4630("selfdestructcountdown");
            unsigned int* flags = (unsigned int*)(unitdef + 0x245);
            if (countdown == (char*)0)
                *flags = (*flags & 0xffdfffff) | 0x500000;
            else
                *flags = (*flags & 0xff8fffff) | ((unsigned int)atoi(countdown) & 7) << 20;
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(buf, "category", 100, DAT_005119b8);
            ((Class_00488e70*)unitdef)->FUN_00488e70(buf);
            if (((Class_004c48c0*)parser.current)
                    ->FUN_004c48c0(buf, "soundcategory", 100, DAT_005119b8) != 0) {
                while (sound < *(int*)(g_game + 0x37e17)) {
                    if (_strcmpi(*(char**)(g_game + 0x37e13) + sound * 0x160, buf) == 0)
                        goto SOUND_FOUND;
                    sound++;
                }
                sound = atoi(buf);
            }
        SOUND_FOUND:
            *(short*)(unitdef + 0x20e) = (short)sound;
            *(short*)(unitdef + 0x1bc) = -1;
            if (((Class_004c48c0*)parser.current)->FUN_004c48c0(buf, "corpse", 100, DAT_005119b8))
                *(short*)(unitdef + 0x1bc) = FUN_00422e40(buf);
            *(void**)(unitdef + 0x1b6) = 0;
            if (((Class_004c48c0*)parser.current)
                    ->FUN_004c48c0(buf, "movementclass", 100, DAT_005119b8))
                *(void**)(unitdef + 0x1b6) = FUN_00440420(buf);
            Class_004402e0 movement;
            char* move = *(char**)(unitdef + 0x1b6);
            if (move == 0) {
                ((Class_00440320*)&movement)->FUN_00440340(&parser);
                move = (char*)&movement;
            }
            *(short*)(unitdef + 0x14a) = *(short*)(move + 4);
            *(short*)(unitdef + 0x14c) = *(short*)(move + 6);
            *(short*)(unitdef + 0x1be) = *(short*)(move + 8);
            *(short*)(unitdef + 0x1c0) = *(short*)(move + 10);
            unitdef[0x228] = move[12];
            unitdef[0x229] = move[14];
            *(int*)(unitdef + 0x196) = (int)(((__int64)*(int*)(unitdef + 0x192) << 16) /
                                             (((unsigned char)unitdef[0x228] + 1) * 0x10000));
            char* defaultWeapon = g_game + 0x2cf3;
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(weapon, "weapon1", 128, DAT_005119b8);
            char* weapon1 = FUN_0049e5b0(weapon);
            *(char**)(unitdef + 0x1ee) = weapon1 ? weapon1 : defaultWeapon;
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(weapon, "weapon2", 128, DAT_005119b8);
            char* weapon2 = FUN_0049e5b0(weapon);
            *(char**)(unitdef + 0x1f2) = weapon2 ? weapon2 : defaultWeapon;
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(weapon, "weapon3", 128, DAT_005119b8);
            char* weapon3 = FUN_0049e5b0(weapon);
            *(char**)(unitdef + 0x1f6) = weapon3 ? weapon3 : defaultWeapon;
            ((Class_004c48c0*)parser.current)->FUN_004c48c0(weapon, "explodeas", 128, DAT_005119b8);
            char* explodeas = FUN_0049e5b0(weapon);
            *(char**)(unitdef + 0x220) = explodeas ? explodeas : defaultWeapon;
            ((Class_004c48c0*)parser.current)
                ->FUN_004c48c0(weapon, "selfdestructas", 128, DAT_005119b8);
            char* selfdestructas = FUN_0049e5b0(weapon);
            *(char**)(unitdef + 0x224) = selfdestructas ? selfdestructas : defaultWeapon;
            if (*(char**)(unitdef + 0x1ee) == defaultWeapon &&
                *(char**)(unitdef + 0x1f2) == defaultWeapon &&
                *(char**)(unitdef + 0x1f6) == defaultWeapon)
                *(unsigned int*)(unitdef + 0x241) &= ~0x10000;
            else
                *(unsigned int*)(unitdef + 0x241) |= 0x10000;
            *(void**)(unitdef + 0x14e) = 0;
            if (unitdef[0x22f] == 0) {
                ((Class_004c48c0*)parser.current)
                    ->FUN_004c48c0(yard, "YardMap", 1024, DAT_005119b8);
                *(char**)(unitdef + 0x14e) = (char*)FUN_004d83b0(
                    "BUILDING YARD", *(short*)(unitdef + 0x14a) * *(short*)(unitdef + 0x14c));
                int cell = 0;
                char* cursor = yard;
                for (int y = 0; y < *(short*)(unitdef + 0x14c); y++) {
                    for (int x = 0; x < *(short*)(unitdef + 0x14a);) {
                        switch (*cursor) {
                        case '.':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x0;
                            break;
                        case 'C':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x35;
                            break;
                        case 'G':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x8f;
                            break;
                        case 'O':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x2b;
                            break;
                        case 'Y':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x31;
                            break;
                        case 'c':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x2d;
                            break;
                        case 'f':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x6f;
                            break;
                        case 'o':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x2f;
                            break;
                        case 'w':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x37;
                            break;
                        case 'y':
                            (*(char**)(unitdef + 0x14e))[cell] = 0x29;
                            break;
                        default:
                            cursor++;
                            continue;
                        }
                        x++;
                        cell++;
                        if (cursor[1] != 0)
                            cursor++;
                    }
                }
            }
            int w = *(short*)(unitdef + 0x14a);
            int h = *(short*)(unitdef + 0x14c);
            *(int*)(unitdef + 0x15e) = (w * -0x100000) / 2;
            *(int*)(unitdef + 0x166) = (h * -0x100000) / 2;
            *(int*)(unitdef + 0x16a) = (w << 20) / 2;
            *(int*)(unitdef + 0x172) = (h << 20) / 2;
            // tail
            int* p = (int*)(unitdef + 0x176);
            p[0] = *(int*)(unitdef + 0x16a) - *(int*)(unitdef + 0x15e);
            p[1] = *(int*)(unitdef + 0x16e) - *(int*)(unitdef + 0x162);
            p[2] = *(int*)(unitdef + 0x172) - *(int*)(unitdef + 0x166);
            *(int*)(unitdef + 0x182) = (*(int*)(unitdef + 0x17e) + *(int*)(unitdef + 0x176)) / 3;
            ((Class_004c3240*)&parser)->FUN_004c3240();
            if ((*(unsigned int*)(unitdef + 0x245) & 0x2000) && *(short*)(unitdef + 0x208) == 0)
                *(short*)(unitdef + 0x208) = 80;

        }
    }
FINISH:;
}
