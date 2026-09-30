// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The out-of-line destructor of Class_004b3630 (see 0x432590.cpp): frees the
// slot table at +0x0 and everything hanging off its slots.

struct Item_004b3630_14 {           // 0x14 bytes
    int field_0;                    // +0x00
    void* field_4;                  // +0x04
    char unknown_8[8];              // +0x08
    void* field_10;                 // +0x10
};

struct Item_004b3630_10 {           // 0x10 bytes
    void* field_0;                  // +0x00
    int field_4;                    // +0x04
    void* field_8;                  // +0x08
    char unknown_c[4];              // +0x0c
};

struct Slot_004b3630 {              // 0x18 bytes
    void* field_0;                  // +0x00
    int count_4;                    // +0x04
    int count_8;                    // +0x08
    char unknown_c[4];              // +0x0c
    Item_004b3630_10* field_10;     // +0x10
    Item_004b3630_14* field_14;     // +0x14
};

struct Table_004b3630 {
    int count;                      // +0x00
    Slot_004b3630* slots;           // +0x04
};

void __cdecl FUN_004d85a0(void* p);

class Class_004b3630 {
public:
    Table_004b3630* table;          // +0x00
    void FUN_004b3630();
};

// FUNCTION: 0x4b3630
void Class_004b3630::FUN_004b3630()
{
    if (table != 0) {
        for (int i = 0; i < table->count; i++) {
            Slot_004b3630* slot = &table->slots[i];
            FUN_004d85a0(slot->field_0);
            if (slot->field_10 != 0) {
                for (int j = 0; j < slot->count_4; j++) {
                    FUN_004d85a0(slot->field_10[j].field_0);
                    if (slot->field_10[j].field_4 == 3) {
                        FUN_004d85a0(slot->field_10[j].field_8);
                    }
                }
                FUN_004d85a0(slot->field_10);
            }
            if (slot->field_14 != 0) {
                for (int k = 0; k < slot->count_8; k++) {
                    if (slot->field_14[k].field_0 != 0) {
                        FUN_004d85a0(slot->field_14[k].field_4);
                    }
                    if (slot->field_14[k].field_10 != 0) {
                        FUN_004d85a0(slot->field_14[k].field_10);
                    }
                }
                FUN_004d85a0(slot->field_14);
            }
        }
        if (table->slots != 0) {
            FUN_004d85a0(table->slots);
        }
        FUN_004d85a0(table);
    }
}
