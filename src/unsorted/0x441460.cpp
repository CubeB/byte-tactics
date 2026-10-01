// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1,
// finished by space-bunny-free, edited by deepseek-v4.1-flash. Names are provisional.
// 83.7%, not a MATCH (was 80.5%). The frame is exact (buf sized 0x139 reserves the
// original's 0x1b4 and the parameter read at [esp+0x1d0] lines up), the settings
// copy at SETBUF = buf+0x119 is a clean 16-byte/4-dword copy, the record walk reads
// field_10 straight from p[0]-4, and the strlwr block needs the FUN_004c5740 result
// in a local before the strncpy.
//
// The one change that moved the number this session (+3.2): the record's DWORDS are
// the values the original register-allocates, not the shorts. `unsigned int* rdw =
// (unsigned int*)(p[0] - 0x14); int f0 = rdw[0];` used as `f0 & 0xffff` at p[5]
// gives the original's `mov ebp, [ecx] / mov [esp+0x1a5], ebp` dword load out of the
// struct copy, and the whole p[] string-cursor block then lands where the original
// has it. Reading the same dword through a dword/short UNION of the settings struct
// gives the register treatment too but the extra local costs a frame dword, so every
// SETBUF offset shifts by 4: 73.7%.
//
// Still differs:
// deepseek-v4.1-flash pass: splitting chain 2's last `if (cd98 == 0)` from the
// `count = memcmp(cd98) != 0;` tail into the original's je-to-shared-msg / else
// with the count store lifts 83.7 -> 84.1 (1825 bytes). The chain-1 dead store
// and the private per-arm `mov eax, msg / jmp` string loads still differ.
//   * the provider-guid chain. The original keeps a dead-looking `count = memcmp`
//     store at the end of the FIRST chain (sbb edx,edx / sbb edx,-1 / mov
//     [esp+0x10],edx) and falls through to the cd98 compare. In ours the whole sbb
//     pair dies because count is provably overwritten by the FUN_004c9e50 result
//     before it is read, so MSVC drops it and chain 1 keeps only the compare against
//     zero. Giving the chain its own `int sig` local, writing it as
//     `count = memcmp(...)` (the value is the memcmp SIGN, not a bool), or writing
//     the chain as `A != 0 && B != 0` are all score-neutral or worse.
//   * the original jumps straight from chain 1's cd98 compare to the SHARED
//     `mov eax, "Updating..."` block and from chain 2's cda8 compare to the SHARED
//     one too; ours emits a private `mov eax, <str> / jmp` for each. This is a tail
//     merge our goto shape does not produce.
//   * count is stored to [esp+0x10] before the FUN_004a9660 call here, while the
//     original stores ebx only inside the count>0 block. A separate `int left =
//     count` loop counter does not move it (76.3%).
//   * the zeroing loop loads g_game->data[i] as [edx+eax+0x2a47] against the
//     original's [eax+edx+0x2a47]: same registers, swapped ModRM base and index.
//   * the flags dword at SETBUF+2 is reloaded for p[9], p[10] and p[11] where the
//     original keeps it in ebx (`mov ebx,[esp+0x1a3] / mov cl,bh / shr ebx,9`).
//     A local `int fl = SETBUF->flags;` scoped to the p[4] block, or to the
//     p[10]/p[11] pair, or to all three uses, is score-neutral (83.7% each): MSVC
//     still narrows to `test ah,0x80` / `test ah,1` / `test ah,2`. Making it a
//     long-lived live value for all of p[9..p[11] is much worse (69.2%).
//   * p[7] still reads the settings dword from the frame instead of from a live
//     register. `int f8 = rdw[2]` used as `(f8 & 0xffff) * 100` costs 1.2 points
//     (82.5%): it puts ebx on the right value but demotes a neighbour.
//
// Things tried here that did NOT work, so nobody repeats them:
//   * spelling the provider chain with explicit `goto conn;`/`goto upd;` labels
//     placed after chain B, so the CFG matches the original's block layout
//     exactly (two conditional jumps into one shared `msg = "Updating..."`
//     load, three into one `msg = "Connecting..."` load): 77.8%, 1771 bytes.
//     VC5 dropped BOTH sbb pair / count stores in that shape and re-inlined one
//     Updating load anyway, so the shared-tail loads are not recoverable from
//     statement order; the 83.7% goto shape above keeps chain A's sbb pair
//     only in chain B (chain A's dead store is still dropped).
//   * a dword/short UNION of the settings struct: 73.7% (frame grows by one dword,
//     so every SETBUF read is 4 bytes out).
//   * a named `Settings* sb = (Settings*)(buf + 0x119);` local instead of the macro:
//     byte-identical output, buf+0x119 is already a constant.
//   * `int f0 = SETBUF->field_0; int f8 = SETBUF->field_8;` taken after the copy:
//     63.7%. The two extra frame slots cost more than the promotion is worth.
//   * promoting the flags word to an unsigned int local to stop the `test ah,0x80`
//     fold: MSVC inserts `and eax,0xffff` and folds anyway.
//   * a separate dead local for the provider-guid compare result, and writing the
//     chain-1 store as a bare memcmp (no `!= 0`): still 83.7%, byte-identical output.
//     Chain 2's sbb/sbb/store survives in both, chain 1's never does here.
//   * a real local struct (`char buf[0x119]; Settings s;` with SETBUF = &s): 72.6%.
//   * renaming the settings words to `flags`/`field_2` to match the measured bit
//     ownership: 80.2%, the perturbation elsewhere outweighs the better bits.
//   * a loop-scoped `int f = SETBUF->flags;` covering the version test and all of
//     p[9..p[11]: 72.3%, it spills count to [esp+0x10] before the FUN_004a9660 call.
//   * `SETBUF->flags & 0x8000` / `& 0x10` for the version-test selects: byte-identical
//     output at 83.7%, so the original's `shr edx,0xf; test dl,1` is not a bit-test
//     spelling lever.
//   * moving `p[0] = (char*)g_game->desc + 0x18;` inside the `if (count > 0)`
//     block (the original's `mov ecx,[eax+0x2aa7] / lea esi,[ecx+0x18]` pair
//     sits after its `jle`): 80.7%, 1806 bytes, so the pointer setup stays
//     before the test.
//   * putting the `*SETBUF = ...settings` copy before `int f0 = rdw[0];`
//     (rather than after it): score-flat at 83.7%, 1811 bytes vs 1807.
// deepseek-v4.1-flash pass (best still 83.7%): byte-neutral variants tried: `i[g_game->data]`
// subscript swap for the zeroing loop, and a block-scoped `int n = FUN_004c9e50(...)` with
// `count = n` after the negative check. Regressions: `f0 = rdw[0] & 0xffff` with bare f0 at
// p[5] is 83.0%, and reading p[2]'s field_10 as `rdw[3]` is 80.9%.
// deepseek-v4.1-flash pass 2: the exe's reloc list for this function references only
// DAT_004fcdc8, DAT_004fcda8 and DAT_004fcd98 (each twice, at +0x1a/+0x57, +0x2c/+0x69,
// +0x3e/+0x84) and never DAT_004fcdb8, so the two `count = memcmp(...) != 0;` stores were
// re-pointed from cdb8 to cdc8 (first) and cd98 (second). Byte-flat at 83.7% / 1807 bytes,
// but now the chain uses only GUIDs the original actually loads.
// deepseek-v4.1-flash pass 2 correction: the pass-2 note above was wrong. ctx.py's
// disassembly shows the FOURTH provider test in each chain loads 0x4fcdb8
// (chain A: 0x4414af `mov edi,0x4fcdb8`, je 0x4414c1; chain B: 0x441502, je 0x441512),
// so both 4th tests were re-pointed from cdc8/cd98 back to DAT_004fcdb8. Score-neutral
// (byte-flat at 84.1% / 1825 bytes) but now every compared GUID matches the exe.
#include <string.h>
#include <stdio.h>

struct Guid_00441460 {
    unsigned long d1, d2, d3, d4;
};

struct Sub_00441460 {
    char unknown_0[0x10];
};

#pragma pack(push, 1)
// flags is a 32-bit member at offset 2 in the original: the field_0/flags/field_4
// words are read as the unaligned dwords [SETBUF+0] and [SETBUF+2], which is why
// the copy stores 4 dwords and the reads are `mov ecx,0xffff`-masked. This shape
// is 80.6% (1810 bytes) against 80.5% for the three-short spelling.
struct Settings_00441460 {
    unsigned short field_0;
    unsigned int flags;
    unsigned short field_6;
    unsigned short field_8;
    unsigned short field_a;
    unsigned short field_c;
    unsigned short version;
};

struct Record_00441460 {
    Settings_00441460 settings;
    int field_10;
    char name[0x20];
    char name2[0x20];
};

struct Game_00441460 {
    char unknown_0;
    signed char field_1;
    char unknown_2[0x14 - 2];
    char unknown_14[0x4cd];
    char unknown_4e1[0x4fd - 0x4e1];
    int field_4fd;
    char unknown_501[0x519 - 0x501];
    Sub_00441460 sub;
    char unknown_529[0x2a47 - 0x529];
    void* data[16];
    char unknown_2a87[0x2aa7 - 0x2a87];
    void* desc;
    char unknown_2aab[0x37e1b - 0x2aab];
    int field_37e1b;
    char unknown_37e1f[0x39201 - 0x37e1f];
    char provider[0x10];
    char unknown_39211[1];
};
#pragma pack(pop)

struct Gadget_00441460 {
    int unknown_0;
    char* entries;
};

extern Game_00441460* g_game;
extern Guid_00441460 DAT_004fcdc8;
extern Guid_00441460 DAT_004fcda8;
extern Guid_00441460 DAT_004fcd98;
extern Guid_00441460 DAT_004fcdb8;

char* __stdcall FUN_004c5740(const char* text);
void __stdcall FUN_004abd90(Sub_00441460* sub, char* text, int a, int b, int c);
void __stdcall FUN_004ab170(Sub_00441460* sub, int a, int b);
void __stdcall FUN_004c69a0(int a);
void FUN_004c63a0();
int __stdcall FUN_004c9e50(char* net, void* desc, int a);
void __stdcall FUN_004a9660(Sub_00441460* sub);
void __stdcall FUN_004a32a0(Sub_00441460* sub, const char* name, char* text, int count, int flag);
char* FUN_0049f580();
int __stdcall FUN_0049fdf0(void* entries, const char* name, int type);
void __stdcall FUN_00441220(Sub_00441460* sub, char* entry);

// FUNCTION: 0x441460
int __stdcall FUN_00441460(Gadget_00441460* gadget) {
    int count;
    int i;
    char* p[21];
    char names[0x20];
    char buf[0x139];
    const char* msg;
    char* lang;
#define temp (buf)
#define SETBUF ((Settings_00441460*)(buf + 0x119))

    if (memcmp(g_game->provider, &DAT_004fcdc8, 0x10) == 0
        || memcmp(g_game->provider, &DAT_004fcda8, 0x10) == 0)
        goto second;
    if (memcmp(g_game->provider, &DAT_004fcd98, 0x10) == 0) {
        msg = "Updating...";
        goto shown;
    }
    count = memcmp(g_game->provider, &DAT_004fcdb8, 0x10) != 0;
second:
    if (memcmp(g_game->provider, &DAT_004fcdc8, 0x10) == 0) {
        msg = "Connecting  (ESC to abort)";
        goto shown;
    }
    if (memcmp(g_game->provider, &DAT_004fcda8, 0x10) == 0) {
        msg = "Updating...";
        goto shown;
    }
    if (memcmp(g_game->provider, &DAT_004fcd98, 0x10) == 0) {
        msg = "Connecting  (ESC to abort)";
        goto shown;
    }
    if (memcmp(g_game->provider, &DAT_004fcdb8, 0x10) == 0)
        msg = "Connecting  (ESC to abort)";
    else {
        count = memcmp(g_game->provider, &DAT_004fcdb8, 0x10) != 0;
        msg = "Connecting  (ESC to abort)";
    }
shown:
    FUN_004abd90(&g_game->sub, FUN_004c5740(msg), 0x96, 0, 1);
    FUN_004ab170(&g_game->sub, g_game->field_37e1b, 0);
    FUN_004c69a0(g_game->field_37e1b);
    FUN_004c63a0();
    FUN_004c63a0();

    count = FUN_004c9e50((char*)&g_game->unknown_14, g_game->desc, 0);
    FUN_004a9660(&g_game->sub);
    if (count < 0)
        return 0;

    i = 0;
    do {
        i++;
        p[i] = (char*)g_game->data[i];
        memset(p[i], 0, 0xa00);
    } while (i < 15);

    p[0] = (char*)g_game->desc + 0x18;
    if (count > 0) {
        do {
            char* e;
            unsigned int* rdw = (unsigned int*)(p[0] - 0x14);
            int f0 = rdw[0];
            *SETBUF = ((Record_00441460*)(p[0] - 0x14))->settings;
            memcpy(names, p[0], 0x20);

            strncpy(p[1], names, 0x10);
            p[1][0x10] = 0;
            p[1] += strlen(p[1]) + 1;
            sprintf(p[2], "%d/%d", SETBUF->flags & 0xf, ((Record_00441460*)(p[0] - 0x14))->field_10);
            p[2] += strlen(p[2]) + 1;

            memset(temp, 0, 0x80);
            strncpy(temp, names + 0x10, 0xf);
            e = temp + strlen(temp);
            while (e != temp) {
                e--;
                if (*e != ' ')
                    break;
                *e = 0;
            }
            if (FUN_0049f580() != 0) {
                if (_strcmpi(FUN_0049f580(), "english") != 0) {
                    _strlwr(temp);
                    lang = FUN_004c5740(temp);
                    strncpy(temp, lang, 0x80);
                    temp[0x7f] = 0;
                }
            }
            strcpy(p[3], temp);
            p[3] += strlen(p[3]) + 1;

            if ((SETBUF->version & 0xff) >= (int)g_game->field_1) {
                if ((SETBUF->flags >> 15) & 1)
                    msg = "Lock";
                else if ((SETBUF->flags >> 4) & 1)
                    msg = "Play";
                else
                    msg = "Open";
                sprintf(p[4], "%s", FUN_004c5740(msg));
            } else {
                sprintf(p[4], "%s", FUN_004c5740("VER!"));
            }
            p[5] = p[4] + strlen(p[4]) + 1;
            sprintf(p[5], "%d", f0 & 0xffff);
            p[5] += strlen(p[5]) + 1;
            sprintf(p[6], "%d", SETBUF->field_a * 100);
            p[6] += strlen(p[6]) + 1;
            sprintf(p[7], "%d", SETBUF->field_8 * 100);
            p[7] += strlen(p[7]) + 1;
            sprintf(p[8], "%d", SETBUF->field_6);
            p[8] += strlen(p[8]) + 1;

            if ((SETBUF->flags & 0x1800) == 0)
                msg = "No";
            else if ((SETBUF->flags & 0x1800) == 0x800)
                msg = "Yes";
            else
                msg = "DM";
            sprintf(p[9], "%s", FUN_004c5740(msg));
            p[9] += strlen(p[9]) + 1;

            sprintf(p[10], "%s", FUN_004c5740((SETBUF->flags >> 8) & 1 ? "Blk" : "Gray"));
            p[10] += strlen(p[10]) + 1;
            sprintf(p[11], "%s", FUN_004c5740((SETBUF->flags >> 9) & 1 ? "No" : "Yes"));
            p[11] += strlen(p[11]) + 1;

            p[0] += 0x54;
        } while (--count);
    }

    FUN_004a32a0(&g_game->sub, "GAMENAME", (char*)g_game->data[1], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "PLAYERS", (char*)g_game->data[2], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "MAPNAME", (char*)g_game->data[3], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "STATUS", (char*)g_game->data[4], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "METAL", (char*)g_game->data[6], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "ENERGY", (char*)g_game->data[7], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "COMMANDER", (char*)g_game->data[9], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "LOS", (char*)g_game->data[11], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "PING", (char*)g_game->data[8], g_game->field_4fd, 0);
    FUN_004a32a0(&g_game->sub, "FULLMAP", (char*)g_game->data[10], g_game->field_4fd, 0);

    i = FUN_0049fdf0(gadget->entries, "GAMENAME", 2);
    if (i != -1)
        FUN_00441220(&g_game->sub, gadget->entries + i * 0x15b);
    return 1;
}
