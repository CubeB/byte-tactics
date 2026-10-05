// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Takes a free slot from the 100-entry pool at DAT_00511df0 and copies the
// argument header plus the per-unit record into the freshly allocated block.

#pragma pack(push, 1)

struct Descriptor_00421620 {
    char unknown_0[4];
    int field_4;                // +0x04
};

struct Rec_00421620 {
    Descriptor_00421620* desc;  // +0x00
    char unknown_4[0x12];       // +0x04
    int field_16;               // +0x16
    int field_1a;               // +0x1a
    int field_1e;               // +0x1e
    void* field_22;             // +0x22
    char unknown_26[0x28 - 0x26];
    unsigned short field_28;    // +0x28
    char unknown_2a[0x36 - 0x2a];
};

struct Unit {
    char unknown_0[0x6a];
    int field_6a;               // +0x6a
    int field_6e;               // +0x6e
    int field_72;               // +0x72
    char unknown_76[0x9e - 0x76];
    void* field_9e;             // +0x9e
};

struct Header_00421620 {
    Unit* obj;                  // +0x00
    int index;                  // +0x04
    char unknown_8[0x20];       // +0x08
    unsigned int bits_28 : 2;   // +0x28
    unsigned int flag_28 : 1;   // +0x28, bit 2
    unsigned int rest_28 : 29;  // +0x28
    Rec_00421620* field_2c;     // +0x2c
};

struct Block_00421620 {
    Header_00421620 header;     // +0x00
    Rec_00421620 rec;           // +0x30
};

#pragma pack(pop)

class CMemoryCache {
public:
    int AllocHandle(int* slot, int size);
};

extern CMemoryCache DAT_00511f80;
extern int* DAT_00511df0[];

void __stdcall BreakPieceIntoDebris(Header_00421620* param_1);

static int FindFreeSlot_00421620()
{
    for (int i = 0; i < 100; i++) {
        if (DAT_00511df0[i] == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x421620
void __stdcall StartExplodePiece(Header_00421620* param_1)
{
    Unit* obj = param_1->obj;
    Rec_00421620* rec = (Rec_00421620*)((char*)obj->field_9e + 0x22 + param_1->index * 0x36);
    rec->field_28 &= 0xfffe;
    if (param_1->flag_28) {
        BreakPieceIntoDebris(param_1);
        return;
    }
    int index = FindFreeSlot_00421620();
    if (index < 0) {
        return;
    }
    int num = rec->desc->field_4;
    if (DAT_00511f80.AllocHandle((int*)&DAT_00511df0[index], num * 12 + 0x66) == 0) {
        return;
    }
    Block_00421620* block = (Block_00421620*)DAT_00511df0[index];
    block->header = *param_1;
    Rec_00421620* dst = (Rec_00421620*)((char*)block + 0x30);
    block->header.field_2c = dst;
    *dst = *rec;
    block->header.field_2c->field_22 = (char*)block->header.field_2c + 0x36;
    block->header.field_2c->field_16 += obj->field_6a;
    block->header.field_2c->field_1a += obj->field_6e;
    block->header.field_2c->field_1e += obj->field_72;
}
