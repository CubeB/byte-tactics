// Decompiled by deepseek-v4.1-flash. Names are provisional.
// 90.7% match. Still differs in four spots:
//  - the inner-loop increment: original keeps the table index in edx and
//    stores the slot pointer before the index; ours uses esi and stores the
//    index first (same instructions, different register/order).
//  - case 1 (READY): original does `or byte [rec+0x13c],1` directly; ours
//    loads into al, stores +0x138, then ors and stores.
//  - cases 2/6 (LOGO/RES): original encodes the player access as
//    `[ecx+eax]` (ecx=g_game, eax=off); ours swaps base/index to `[eax+ecx]`.
//  - case 3 (SIDE): original materialises the condition in a bool local
//    (mov esi,1 / xor esi,esi / test / sete), ours computes the 0/1 directly.
// The jump-table entry address also shows as <addr> in the diff.
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#pragma pack(push, 1)
struct Head_004455b0 {                 // 0x13e-byte copy of an entry
    unsigned char state;               // +0x00
    char unknown_1;                    // +0x01
    char name[0x13];                   // +0x02
    short field_15;                    // +0x15
    char unknown_17[2];                // +0x17
    short field_19;                    // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x29 - 0x1f];
    unsigned char field_29;            // +0x29
    char unknown_2a[0xb6 - 0x2a];
    char text[0x13e - 0xb6];           // +0xb6
};

struct Entry_004455b0 {                // 0x15b-byte array element
    unsigned char state;               // +0x00
    char unknown_1;                    // +0x01
    char name[0x13];                   // +0x02
    short field_15;                    // +0x15
    char unknown_17[2];                // +0x17
    short field_19;                    // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x29 - 0x1f];
    unsigned char field_29;            // +0x29
    char unknown_2a[0xb6 - 0x2a];
    char text[0xcc - 0xb6];            // +0xb6
    char ready[0x138 - 0xcc];          // +0xcc
    short field_138;                   // +0x138
    unsigned char field_13a;           // +0x13a
    unsigned char unknown_13b;         // +0x13b
    unsigned char field_13c;           // +0x13c
    char unknown_13d[0x15b - 0x13d];   // +0x13d
};

struct Holder_004455b0 {
    int unknown_0;
    char* gadgets;                     // +0x04
};

struct Menu_004455b0 {
    char unknown_0[0x18];
    Holder_004455b0* holder;           // +0x18
};

struct Player_004455b0 {
    int active;                        // +0x00
    char unknown_4[0x73 - 4];
    unsigned char type;                // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_004455b0 {
    char unknown_0[0x519];
    Menu_004455b0 menu;                // +0x519
    char unknown_1[0x1b63 - 0x519 - sizeof(Menu_004455b0)];
    Player_004455b0 players[10];       // +0x1b63
    char unknown_2[0x2a42 - 0x1b63 - 10 * 0x14b];
    unsigned char myPlayer;            // +0x2a42
};
#pragma pack(pop)

extern Game_004455b0* g_game;
extern char* DAT_005054b0[];
extern int DAT_00512760;
extern short DAT_00512764;
extern int DAT_0051276c;
extern int DAT_00512994;

int __stdcall FUN_0049fdf0(void* gadgets, char* name, int type);
int __stdcall FUN_004a5d50(Menu_004455b0* menu, int index);
void __stdcall FUN_004a1450(Menu_004455b0* menu, char* name, int value);
int __stdcall FUN_004a1080(Menu_004455b0* menu, char* name, int value);

static void CloneFix_004455b0(Entry_004455b0* rec)
{
    Head_004455b0 tmp = *(Head_004455b0*)rec;
    int index = FUN_0049fdf0(g_game->menu.holder->gadgets, rec->name, 0xe);
    FUN_004a5d50(&g_game->menu, index);
    rec->field_15 += 2;
    rec->state = 5;
    strcpy(rec->text, tmp.text);
    rec->flags |= 0x10;
}

// FUNCTION: 0x4455b0
void __cdecl FUN_004455b0(void)
{
    char* base = g_game->menu.holder->gadgets;
    int p = 0;
    int off = 0x1b63;
    int t;
    char** slot;

    *(short*)(base + 0xb6) = DAT_00512764;
    do {
        for (t = 0, slot = DAT_005054b0; *slot != 0; t++, slot++) {
            int index = FUN_0049fdf0(base, *slot, 0xe);
            Entry_004455b0* rec = (Entry_004455b0*)(base + 0x15b * index);
            Entry_004455b0* dst;
            short count;

            DAT_00512760 = rec->field_15;
            if (t == 0)
                DAT_0051276c = rec->field_19;
            count = ++*(short*)(base + 0xb6);
            dst = (Entry_004455b0*)(base + 0x15b * count);
            *dst = *rec;
            dst->name[strlen(dst->name) - 1] = (char)('0' + p);
            dst->field_15 += p * 20;
            dst->unknown_1 = 0;
            dst->field_29 = 1;
            if (dst->state != 5) {
                switch (t) {
                case 0:
                    if (p == g_game->myPlayer) {
                        if (dst->state == 1)
                            CloneFix_004455b0(dst);
                        dst->flags = 1;
                    } else {
                        dst->flags |= 0x8000;
                    }
                    break;
                case 1:
                    if (p != g_game->myPlayer) {
                        dst->field_138 = 0;
                        dst->field_13c |= 1;
                    }
                    break;
                case 2:
                    {
                        Player_004455b0* pl = (Player_004455b0*)((char*)g_game + off);
                        if (pl->active == 0 || (pl->type != 1 && pl->type != 2))
                            dst->field_29 = 0;
                    }
                    break;
                case 3:
                    {
                        Player_004455b0* pl = (Player_004455b0*)((char*)g_game + off);
                        FUN_004a1450(&g_game->menu, dst->name,
                                     !(pl->active != 0 && (pl->type == 1 || pl->type == 2)));
                    }
                    dst->field_29 = 0;
                    break;
                case 6:
                    if (p != g_game->myPlayer && dst->state == 1)
                        CloneFix_004455b0(dst);
                    if (*(int*)((char*)g_game + off) != 0 &&
                        ((Player_004455b0*)((char*)g_game + off))->type == 2)
                        dst->field_29 = 0;
                    break;
                case 7:
                    if (p == g_game->myPlayer)
                        dst->field_29 = 0;
                    break;
                case 9:
                    dst->field_29 = 0;
                    FUN_004a1080(&g_game->menu, dst->name, 10);
                    break;
                default:
                    if (dst->state == 1)
                        CloneFix_004455b0(dst);
                    break;
                }
            }
        }
        p++;
        off += 0x14b;
    } while (off < 0x2851);

    {
        char name[52];
        int index;
        sprintf(name, "PLAYER%d", g_game->myPlayer);
        index = FUN_0049fdf0(base, name, 0xe);
        if (index != -1) {
            Entry_004455b0* rec = (Entry_004455b0*)(base + 0x15b * index);
            if (rec->state == 1)
                CloneFix_004455b0(rec);
        }
        sprintf(name, "READY%d", g_game->myPlayer);
        index = FUN_0049fdf0(base, name, 1);
        if (index != -1) {
            Entry_004455b0* rec = (Entry_004455b0*)(base + 0x15b * index);
            rec->field_13a = (unsigned char)tolower(name[0]);
            strcpy(base + 0xcc, name);
        }
    }
    DAT_00512994 = 1;
}
