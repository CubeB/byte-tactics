// Decompiled by Opus. Names are provisional.
// Sets the position (+0xc) of the current chunk of the current slot, clamped
// to 0..size (+0x8); the seek counterpart of 0x4b4c50.

struct SafeDepositBox {             // 0x14 bytes
    char unknown_0[8];
    int size;                       // +0x08
    int pos;                        // +0x0c
    char* data;                     // +0x10
};

struct BankAccount {                // 0x18 bytes
    char unknown_0[0xc];
    int current;                    // +0x0c
    char unknown_10[4];
    SafeDepositBox* chunks;         // +0x14
};

struct AccountList {
    char unknown_0[4];
    BankAccount* slots;             // +0x04
    int index;                      // +0x08
};

class Class_004b4c10 {
public:
    AccountList* table;             // +0x00

    void SeekBox(int pos);
};

// FUNCTION: 0x4b4c10
void Class_004b4c10::SeekBox(int pos)
{
    BankAccount* s = &table->slots[table->index];
    SafeDepositBox* c = &s->chunks[s->current];
    if (pos < 0) {
        pos = 0;
    }
    if (pos > c->size) {
        pos = c->size;
    }
    c->pos = pos;
}
