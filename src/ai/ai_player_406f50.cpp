// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Target_00406f50 {
    char unknown_0[0xbb];
    unsigned short bit0_4 : 5;         // +0xbb
    unsigned short flag5 : 1;          // 0x20
    unsigned short flag6 : 1;          // 0x40
    unsigned short bit7_15 : 9;
};

struct Obj_00406f50 {
    char unknown_0[0x52];
    Target_00406f50* target;           // +0x52
};
#pragma pack(pop)

class SquadManager {
public:
    void FUN_00406f50(Obj_00406f50* obj, int a, int b);
};

// FUNCTION: 0x406f50
void SquadManager::FUN_00406f50(Obj_00406f50* obj, int a, int b)
{
    Target_00406f50* t = obj->target;
    if (t) {
        if (a > b * 2) {
            t->flag6 = 1;
            return;
        }
        t->flag5 = 1;
    }
}
