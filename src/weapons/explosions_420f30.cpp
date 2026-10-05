// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and GPT-6.1-sol. Names are provisional.
// MATCH: use a local int pointer for the gravity subtraction so MSVC reloads vel.y after the position updates.
#include <stdio.h>
#pragma pack(push, 1)

struct Obj_00420f30 {
    unsigned char state;                // +0x00
};

struct Ref_00420f30 {
    unsigned short index;               // +0x0
    unsigned short value;               // +0x2
    unsigned char kind;                 // +0x4
    char unknown_5[3];
    void* src;                          // +0x8
};

struct Vec3_00420f30 {
    int x;
    int y;
    int z;
};

struct Debris_00420f30 {
    Obj_00420f30* obj;                  // +0x00
    Ref_00420f30 ref1;                  // +0x04
    Ref_00420f30 ref2;                  // +0x10
    Vec3_00420f30 pos;                  // +0x1c
    Vec3_00420f30 size;                 // +0x28
    Vec3_00420f30 vel;                  // +0x34
    Vec3_00420f30 spin;                 // +0x40
    short angle_x;                      // +0x4c
    short angle_y;                      // +0x4e
    short angle_z;                      // +0x50
    unsigned short flag : 1;            // +0x52
};

struct Net_00420f30 {
    char unknown_0[0xd44];
    int f_d44;                          // +0xd44
    int f_d48;                          // +0xd48
};

struct Game {
    char unknown_0[0x14263];
    int gravity;                        // +0x14263
    char unknown_14267[0x1427f - 0x14267];
    unsigned char seaLevel;             // +0x1427f
    char unknown_14280[0x147eb - 0x14280];
    void* src1;                         // +0x147eb
    void* src2;                         // +0x147ef
    char unknown_147f3[0x147f7 - 0x147f3];
    void* src3;                         // +0x147f7
    char unknown_147fb[0x1491b - 0x147fb];
    int count;                          // +0x1491b
    Debris_00420f30 debris[300];        // +0x1491f
    char unknown_1ab8f[0x391e9 - 0x1ab8f];
    Net_00420f30* net;                  // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;
extern Obj_00420f30* DAT_00511df0[100];

int __stdcall UpdateExplodedPiece(Obj_00420f30* obj);
int __stdcall GetCellMeanHeight(Vec3_00420f30* pos);
int __stdcall StepGafSequence(Ref_00420f30* ref);
void __stdcall AddExplosionEffect(Vec3_00420f30* pos, void* src, int index, int flag);

// FUNCTION: 0x420f30
void UpdateExplosions()
{
    for (int i = 0; i < 100; i++) {
        if (DAT_00511df0[i] != 0 && UpdateExplodedPiece(DAT_00511df0[i]) == 0)
            DAT_00511df0[i] = 0;
    }

    int* pCount = &g_game->count;
    Debris_00420f30* d = (Debris_00420f30*)(pCount + 1);
    Debris_00420f30* base = d;
    int n;
    for (n = 0; n < *pCount; n++, d++) {
        if (d->obj != 0) {
            Vec3_00420f30 old = d->pos;
            d->pos.x += d->size.x + d->vel.x;
            d->pos.y += d->size.y + d->vel.y;
            d->pos.z += d->size.z + d->vel.z;
            int* velocityY = &d->vel.y;
            *velocityY = *velocityY - g_game->gravity;
            d->angle_x += d->spin.x;
            d->angle_y += d->spin.y;
            d->angle_z += d->spin.z;
            int r = GetCellMeanHeight(&d->pos);
            if (d->pos.y > (int)((unsigned)g_game->seaLevel << 16) || r >= (int)g_game->seaLevel) {
                if (*(short*)((char*)&d->pos.y + 2) <= r) {
                    d->pos = old;
                    d->vel.y = -(d->vel.y / 2);
                    if (*(short*)((char*)&d->vel.y + 2) <= 0) {
                        if (d->flag)
                            AddExplosionEffect(&d->pos, g_game->src3, 0, 0);
                        d->obj->state = 0xff;
                        d->obj = 0;
                    }
                }
            } else {
                if (d->flag) {
                    Net_00420f30* net = g_game->net;
                    if (net->f_d48 == 0) {
                        void* src = net->f_d44 != 0 ? g_game->src2 : g_game->src1;
                        AddExplosionEffect(&d->pos, src, -1, 1);
                    }
                }
                d->obj->state = 0xff;
                d->obj = 0;
            }
        }
        if (d->ref1.src != 0)
            StepGafSequence(&d->ref1);
        if (d->ref2.src != 0)
            StepGafSequence(&d->ref2);
    }

    int removed = 1;
    while (removed) {
        removed = 0;
        int count = *pCount;
        Debris_00420f30* e = base;
        int k;
        for (k = 0; k < count; k++, e++) {
            if (e->obj == 0 && e->ref1.src == 0 && e->ref2.src == 0) {
                int j;
                for (j = k; j < *pCount - 1; j++, e++)
                    *e = e[1];
                (*pCount)--;
                removed = 1;
                break;
            }
        }
    }
}
