// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Partial, 72.9% (1914 vs 1935 bytes). Campaign screen click handler.
// Remaining diff is BLOCK PLACEMENT, not shape. Verified against our own
// object file (build/obj/unsorted/0x477ab0.obj): MSVC 5 emits
// `mov eax,[0x51e668]; xor ebx,ebx; cmp eax,ebx; je 0x606` at offset 0x66,
// then the Missions/Start gadget calls inline, then `cmp [0x51e668],ebx`
// and `je 0x606`, exactly like the original. The divergence is the target:
// the original's Campaign/Start pair sits at 0x477b4b (offset 0x9b) and its
// BigButton block right after at 0x477b6d (offset 0xbd), so all four
// gadget-test branches are 2-byte shorts (74/75), while our build sinks the
// Campaign/Start pair and the whole BigButton body to offsets 0x606/0x620,
// making those same branches 6-byte near forms (0f 84 / 0f 85) and costing
// 16 bytes at those sites. The zero register in ebx is NOT a diff: our build
// already keeps `xor ebx,ebx` function-wide (see SHARED 0x4624a0: it is a
// compiler constant-in-register choice, not a source variable).
// Tried and rejected: `if (FUN(menu,"Missions") || FUN(menu,"Start")) goto`
// for the inner pair (byte-identical to the two separate ifs: 1914 bytes,
// 72.9%), for the outer pair with an explicit `goto PrevMenu` fallthrough
// (62.3%; the extra jump makes the layout worse), and both at once (62.3%).
// Next idea: the sink is caused by the `goto BigButton` graph (the original
// source very likely used a single if/else-if chain with no labels); a
// nesting that keeps BigButton as the fallthrough of the last gadget test
// should bring it up to 0xbd.
// Stopped (timebox) with the file unchanged at 72.9%. New analysis from the
// disassembly: the original's last Start test is `test eax,eax; je 0x477cc7`
// (branch on FALSE to PrevMenu, BigButton is the fallthrough of the TRUE
// path), and test4 is `jne 0x477cc7` (Campaign pair is its FALSE fallthrough).
// Our source writes `if (Start) goto BigButton;` directly before the
// `BigButton:` label (both edges merge, so the compiler sees a degenerate
// branch and sinks Campaign/BigButton to 0x606/0x620 and inverts test4 to
// `je 0x606` with PrevMenu as the fallthrough). Ideas NOT yet scored (copies
// in build/scratch/0x477ab0/v1..v3.cpp, stopped before any check run):
// v1: spell the last test inverted, `if (!FUN_0049fd60(menu,"Start")) goto
// PrevMenu;` so BigButton is the natural fallthrough of the TRUE edge;
// v2: v1 plus a flat `if (DAT_0051e668 == 0) goto CampaignPair;` for test1;
// v3: untouched control. Secondary diffs that remain even in matched regions:
// our build CSEs the campaign holder across FUN_004d85a0 (`mov eax,[esi+4]`)
// where the original reloads `mov ecx,[g_game]; mov edx,[ecx+0x531];
// mov eax,[edx+4]`, and the Difficulty block uses eax as the g_game base
// where the original uses ecx; the esi/edi holder/menuSub assignment is
// swapped in the Missions rebuild block. Those look like register colouring
// falling out of the block placement, not separate shape errors.
// Retry by deepseek-v4.1-flash (10 min timebox): scored v1..vI scratch copies,
// all <= 72.9%. Tried: inverted last Start test as `if (!FUN(menu,"Start"))
// goto PrevMenu;` (v1 nested, vD flat), flat `if (DAT_0051e668 == 0) goto
// CampaignPair;` test1, `||` for the outer pair, a do/while(0) break wrapper,
// and an explicit else. None moved CampaignPair/BigButton up to the original
// 0x9b/0xbd: the compiler still keeps the inner redundant DAT test as
// `je CampaignPair` (PrevMenu fallthrough) and sinks CampaignPair+BigButton to
// the end. Inverting the last Start test only relocates the whole PrevMenu /
// Difficulty / side-handler group and drops the score to 62.3%. The sibling
// matched handlers 0x4775a0 and 0x478cb0 write the same dispatch as an
// if/else-if chain with the action body inlined once per clause; duplicating
// the 0x15a-byte BigButton body 4 times did not look like it would tail-merge
// and was not tried. Best remains 72.9%.

#pragma pack(push, 1)
struct Entry_00477ab0 {
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
    char unknown_c6[0x15b - 0xc6];
};

struct Holder_00477ab0 {
    char unknown_0[4];
    Entry_00477ab0* entries;           // +0x04
};

struct Menu_00477ab0 {
    char unknown_0[0x18];
    Holder_00477ab0* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};
#pragma pack(pop)

class Class_00435110 {
public:
    void FUN_00435110(char* name);
};

class Class_00435760 {
public:
    int FUN_00435760(int** out);
};

class Class_00435c00 {
public:
    int FUN_00435c00(int index);
};

extern char* g_game;                   // 0x511de8
extern int* DAT_0051e65c;              // 0x51e65c
extern int* DAT_0051e660;              // 0x51e660
extern int DAT_0051e668;               // 0x51e668
extern int DAT_00507b6c;               // 0x507b6c

int __stdcall FUN_0049fd60(Menu_00477ab0* menu, char* name);
char __stdcall FUN_0041d6a0(int param_1);
void FUN_0041d4c0();
void FUN_0041da30();
void FUN_00430f00();
void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_00491c80(int value);
Entry_00477ab0* __stdcall FUN_0049ff90(Entry_00477ab0* entries, char* name);
int __stdcall FUN_0049fdf0(Entry_00477ab0* entries, char* name, int type);
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a2be0(void* menu, int index);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall FUN_004a1110(void* menu, char* name, int flag);
int __stdcall FUN_00476a60(int** out, int side);
void __stdcall FUN_004a32a0(void* menu, char* name, int* data, int count, int flag);
void __stdcall FUN_004a0570(void* menu, char* name, int flag);
void __cdecl FUN_004d85a0(void* ptr);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004abd90(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall FUN_004ab0a0(void* menu);

// FUNCTION: 0x477ab0
void __stdcall FUN_00477ab0(Menu_00477ab0* menu)
{
    Entry_00477ab0* entries = menu->holder->entries;
    char* playerInfo = g_game + 0x14b * *(unsigned char*)(g_game + 0x2a42);
    int index;

    if (menu->current == -1) {
        FUN_004d85a0(DAT_0051e65c);
        FUN_004d85a0(DAT_0051e660);
        DAT_0051e65c = 0;
        DAT_0051e660 = 0;
        return;
    }

    if (DAT_0051e668 != 0) {
        if (FUN_0049fd60(menu, "Missions"))
            goto BigButton;
        if (FUN_0049fd60(menu, "Start"))
            goto BigButton;
        if (DAT_0051e668 != 0)
            goto PrevMenu;
    }
    if (FUN_0049fd60(menu, "Campaign"))
        goto BigButton;
    if (FUN_0049fd60(menu, "Start"))
        goto BigButton;

BigButton:
    index = 0;
    FUN_0047f1a0("bigButton", 0);
    if (!FUN_0041d6a0(0)) {
        FUN_004abd90(g_game + 0x519,
                     FUN_004c5740("Please insert the Campaign CD (Disc 2) and try again"),
                     200, 1, 1);
        FUN_004ab0a0(g_game + 0x519);
        return;
    }
    FUN_0041d4c0();
    FUN_0041da30();
    {
        char* name;
        if (DAT_00507b6c == 0) {
            Entry_00477ab0* e = FUN_0049ff90(entries, "Campaign");
            name = FUN_004b6af0(e->text, e->selected);
        } else if (*(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) == 0) {
            name = "Arm Campaign";
        } else {
            name = "Core Campaign";
        }
        ((Class_00435110*)*(void**)(g_game + 0x391e9))->FUN_00435110(name);
    }
    if (DAT_0051e668 != 0) {
        Entry_00477ab0* e = FUN_0049ff90(entries, "Missions");
        index = e->selected;
    }
    if (((Class_00435c00*)*(void**)(g_game + 0x391e9))->FUN_00435c00(index) != 0) {
        FUN_00491c80(0x14);
        *(unsigned char*)(*(int*)(g_game + 0x1b8a) + 0x96) = 0;
        *(unsigned char*)(*(int*)(g_game + 0x1cd5) + 0x96) = 1;
        FUN_00430f00();
        if (DAT_0051e668 != 0) {
            *(unsigned char*)(g_game + 0x2bc0) = 0x10;
            return;
        }
        *(unsigned char*)(g_game + 0x2bc0) = 0x0f;
        return;
    }
    goto End;

PrevMenu:
    if (FUN_0049fd60(menu, "PrevMenu")) {
        FUN_0047f1a0("Previous", 0);
        *(unsigned char*)(g_game + 0x2bc0) = 3;
        FUN_00491c80(0x14);
        return;
    }
    if (FUN_0049fd60(menu, "Difficulty")) {
        FUN_0047f1a0("SmlButton", 0);
        int diff = *(int*)(g_game + 0x37eee);
        if (diff == 0) {
            *(int*)(g_game + 0x37eee) = 1;
            FUN_004ab0a0(menu);
            return;
        }
        if (diff == 1) {
            *(int*)(g_game + 0x37eee) = 2;
            FUN_004ab0a0(menu);
            return;
        }
        if (diff == 2) {
            *(int*)(g_game + 0x37eee) = 0;
            FUN_004ab0a0(menu);
            return;
        }
        goto End;
    }
    if (FUN_0049fd60(menu, "Side0") || FUN_0049fd60(menu, "Arm"))
        goto ArmSide;
    if (!FUN_0049fd60(menu, "Side1") && !FUN_0049fd60(menu, "Core"))
        goto End;

CoreSide:
    FUN_004a1110(g_game + 0x519, "Core", 1);
    FUN_004a1110(g_game + 0x519, "Side1", 1);
    FUN_0047f1a0("SideSelect2", 0);
    index = FUN_0049fdf0(entries, "Side1", 1);
    *(int*)((char*)entries + index * 0x15b + 0x1f) = 0x1f;
    *(int*)(g_game + 0x37ef2) = 1;
    *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) = 1;
    *(unsigned char*)(*(int*)(playerInfo + 0x1cd5) + 0x95) = 0;
    if (DAT_00507b6c == 0) {
        char* pi = g_game + 0x14b * *(unsigned char*)(g_game + 0x2a42);
        int side = *(unsigned char*)(*(int*)(pi + 0x1b8a) + 0x95);
        Holder_00477ab0* campaignHolder = *(Holder_00477ab0**)(g_game + 0x531);
        if (DAT_0051e65c) {
            FUN_004d85a0(DAT_0051e65c);
            DAT_0051e65c = 0;
        }
        FUN_0047f1a0("smlbutton", 0);
        int count = FUN_00476a60(&DAT_0051e65c, side);
        FUN_004a32a0(g_game + 0x519, "Campaign", DAT_0051e65c, count, 0);
        FUN_004a2be0(g_game + 0x519, FUN_0049fdf0(campaignHolder->entries, "Campaign", 2));
        FUN_0049fa90(g_game + 0x519);
    }
    if (DAT_0051e668 != 0) {
        FUN_0049ff90(entries, "Campaign");
        {
            Holder_00477ab0* holder = *(Holder_00477ab0**)(g_game + 0x531);
            void* menuSub = g_game + 0x519;
            if (DAT_0051e660) {
                FUN_004d85a0(DAT_0051e660);
                DAT_0051e660 = 0;
            }
            Entry_00477ab0* e = FUN_0049ff90(holder->entries, "Campaign");
            char* text = FUN_004b6af0(e->text, e->selected);
            ((Class_00435110*)*(void**)(g_game + 0x391e9))->FUN_00435110(text);
            index = ((Class_00435760*)*(void**)(g_game + 0x391e9))->FUN_00435760(&DAT_0051e660);
            FUN_004a32a0(menuSub, "Missions", DAT_0051e660, index, 0);
            FUN_004a2be0(g_game + 0x519, FUN_0049fdf0(holder->entries, "Missions", 2));
            FUN_0049fa90(g_game + 0x519);
        }
        FUN_004ab0a0(menu);
        return;
    }
    goto End;

ArmSide:
    FUN_004a1110(g_game + 0x519, "Arm", 1);
    FUN_004a1110(g_game + 0x519, "Side0", 1);
    FUN_0047f1a0("SideSelect", 0);
    index = FUN_0049fdf0(entries, "Side0", 1);
    *(int*)((char*)entries + index * 0x15b + 0x1f) = 0x1f;
    *(int*)(g_game + 0x37ef2) = 0;
    *(unsigned char*)(*(int*)(playerInfo + 0x1b8a) + 0x95) = 0;
    *(unsigned char*)(*(int*)(playerInfo + 0x1cd5) + 0x95) = 1;
    {
        char* pi = g_game + 0x14b * *(unsigned char*)(g_game + 0x2a42);
        int side = *(unsigned char*)(*(int*)(pi + 0x1b8a) + 0x95);
        Holder_00477ab0* campaignHolder = *(Holder_00477ab0**)(g_game + 0x531);
        if (DAT_0051e65c) {
            FUN_004d85a0(DAT_0051e65c);
            DAT_0051e65c = 0;
        }
        FUN_0047f1a0("smlbutton", 0);
        int count = FUN_00476a60(&DAT_0051e65c, side);
        FUN_004a32a0(g_game + 0x519, "Campaign", DAT_0051e65c, count, 0);
        FUN_004a2be0(g_game + 0x519, FUN_0049fdf0(campaignHolder->entries, "Campaign", 2));
        FUN_0049fa90(g_game + 0x519);
    }
    FUN_004a0570(menu, "Campaign", DAT_00507b6c == 0);
    if (DAT_0051e668 != 0) {
        FUN_0049ff90(entries, "Campaign");
        {
            Holder_00477ab0* holder = *(Holder_00477ab0**)(g_game + 0x531);
            void* menuSub = g_game + 0x519;
            if (DAT_0051e660) {
                FUN_004d85a0(DAT_0051e660);
                DAT_0051e660 = 0;
            }
            Entry_00477ab0* e = FUN_0049ff90(holder->entries, "Campaign");
            char* text = FUN_004b6af0(e->text, e->selected);
            ((Class_00435110*)*(void**)(g_game + 0x391e9))->FUN_00435110(text);
            index = ((Class_00435760*)*(void**)(g_game + 0x391e9))->FUN_00435760(&DAT_0051e660);
            FUN_004a32a0(menuSub, "Missions", DAT_0051e660, index, 0);
            FUN_004a2be0(g_game + 0x519, FUN_0049fdf0(holder->entries, "Missions", 2));
            FUN_0049fa90(g_game + 0x519);
        }
    }

End:
    FUN_004ab0a0(menu);
}
