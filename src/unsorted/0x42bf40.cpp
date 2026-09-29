// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL (10 min timebox, 4772 byte function). Only the prologue and the first
// ~90 instructions are transcribed (through the four *badTargetCategory reads,
// ending near 0x42c0b0). Everything after that (the rest of the TDF field
// reads, the two flag dwords at def+0x241/def+0x245, category/soundcategory/
// corpse/movementclass, the yardmap switch, weapon names and the frame math)
// is NOT reproduced. The tail here is a placeholder so the file compiles.
//
// Established facts:
//   signature: __stdcall FUN(char* fbi_file, char* unitdef)   (ret 0x8, 2 args)
//   local Class_004c2ea0 parser at esp+0 (0xc bytes: field_0, current at +4, field_8)
//   parser.current is a TDF node; it is the `this` for the getters:
//     Class_004c48c0::FUN_004c48c0(dst, key, size, def)  string
//     Class_004c46c0::FUN_004c46c0(key, def) -> int
//     Class_004c4800::FUN_004c4800(out?, key, def) -> int* (caller reads *ret)
//     Class_004c4760::FUN_004c4760(key, def) -> float
//   free helpers: FUN_004c58a0(parser, dst, key, size, def) __stdcall
//                 FUN_00488c50(char*) __stdcall -> void*
//   Class_00438760 ctor at 0x438760 converts a string into a 1-byte enum
//     (caller reads byte 0 of the constructed object).
//   unitdef offsets: +0x20 unitname[0x20], +0x40 description[0x40],
//     +0x80 objectname, +0x186 float buildcostenergy, +0x18a float buildcostmetal,
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
    float FUN_004c4760(char* key, float def);
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

// FUNCTION: 0x42bf40
void __stdcall FUN_0042bf40(char* fbi_file, char* unitdef)
{
    Class_004c2ea0 parser;
    int scratch;
    char temp;
    char buf[100];

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
            ((Class_004c3240*)&parser)->FUN_004c3240();
        } else {
            ((Class_004c3240*)&parser)->FUN_004c3240();
        }
    }
    ((Class_004c2ea0*)&parser)->~Class_004c2ea0();
}
