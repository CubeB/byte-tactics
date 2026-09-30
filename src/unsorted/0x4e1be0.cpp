// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct EventEntry {
    int id;                            // +0x0
    int unk4;                          // +0x4
    int unk8;                          // +0x8
    int unkc;                          // +0xc
};

extern EventEntry* DAT_00529df8;
extern int DAT_00529dcc;
extern EventEntry DAT_0050da00[];
extern EventEntry DAT_0050d980[];
extern EventEntry DAT_00529e00;        // "Event0"
extern EventEntry DAT_00529e10;        // "Event1"

void __cdecl FUN_004e1b10(int arg);
unsigned char FUN_004e1680(void);
int FUN_004e39a0(void);

// FUNCTION: 0x4e1be0
void FUN_004e1be0(void)
{
    if (DAT_00529df8 == 0) {
        DAT_00529df8 = DAT_0050da00;
        DAT_00529dcc = 0x11;
        FUN_004e1b10(1);
        if (FUN_004e1680() != 0) {
            if (FUN_004e39a0() < 6) {
                DAT_00529df8 = DAT_0050d980;
                DAT_00529dcc = 8;
            }
            EventEntry* table = DAT_00529df8;
            int count = DAT_00529dcc;
            // The pointer locals (declared in this order) make MSVC hoist the
            // two Event ids into the loop preheader in the original order.
            EventEntry* e1 = &DAT_00529e10;
            EventEntry* e0 = &DAT_00529e00;
            for (int i = 0; i < count; i++) {
                if (e0->id == table[i].id)
                    *e0 = table[i];
                if (e1->id == table[i].id)
                    *e1 = table[i];
            }
            if (DAT_00529e00.unk4 == 0)
                DAT_00529e00 = table[0];
            if (DAT_00529e10.unk4 == 0)
                DAT_00529e10 = table[0];
        }
    }
}
