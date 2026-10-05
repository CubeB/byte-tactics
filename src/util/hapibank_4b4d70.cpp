// Decompiled by GPT-5.6-Terra. Names are provisional.
#include <stdio.h>

struct BankItem {
    char* name;
    int type;
    int value;
    int value_high;
};

struct SafeDepositBox {
    int named;
    int value;
    int size;
    int unknown_c;
    int unknown_10;
};

struct BankAccount {
    char* name;
    int item_count;
    int box_count;
    char unknown_c[4];
    BankItem* items;
    SafeDepositBox* boxes;
};

struct AccountList {
    int account_count;
    BankAccount* accounts;
};

class Class_004b4d70 {
public:
    AccountList* bank;
    void WriteAuditFile(char* filename);
};

// FUNCTION: 0x4b4d70
void Class_004b4d70::WriteAuditFile(char* filename)
{
    FILE* file = fopen(filename, "w");
    int i;

    if (file == 0) {
        return;
    }

    fprintf(file, "HapiBank Audit File\n\n");
    fprintf(file, "Number of accounts: %i\n\n", bank->account_count);

    for (i = 0; i < bank->account_count; i++) {
        BankAccount* account = (BankAccount*)((char*)bank->accounts + i * 0x18);
        int j;

        fprintf(file, "Account:  \"%s\"\n{\n", account->name);
        for (j = 0; j < account->item_count; j++) {
            BankItem* item = (BankItem*)((char*)account->items + j * 0x10);

            switch (item->type) {
            case 1:
                fprintf(file, "   Item \"%s\" Integer:  %i (0x%08x)\n", item->name, item->value, item->value);
                break;
            case 2:
                fprintf(file, "   Item \"%s\" Double:  %f\n", item->name, item->value, item->value_high);
                break;
            case 3:
                fprintf(file, "   Item \"%s\" String:  \"%s\"\n", item->name, item->value);
                break;
            default:
                fprintf(file, "   Item \"%s\" Uninitialized or Unknown\n", item->name);
                break;
            }
        }

        fprintf(file, "\n");
        for (j = 0; j < account->box_count; j++) {
            SafeDepositBox* box = (SafeDepositBox*)((char*)account->boxes + j * 0x14);

            if (box->named != 0) {
                fprintf(file, "   Safe-Deposit Box \"%s\" contains %i bytes\n", box->value, box->size);
            } else {
                fprintf(file, "   Safe-Deposit Box #%i contains %i bytes\n", box->value, box->size);
            }
        }
        fprintf(file, "}\n\n");
    }

    fprintf(file, "----------EOF----------\n");
    fclose(file);
}
