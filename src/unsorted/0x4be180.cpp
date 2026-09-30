// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Walks the record list and probes each record whose file is not open yet:
// opens the record's filename in "rb" and, if it can be opened, closes it
// again (the record only checks that the file exists). If the open fails the
// record is freed and removed from the list, shifting the tail down.
// The cleanup is a shared helper (also out of line at 0x4be270) that /Ob2
// inlines here; passing it state->records[i] rather than the local keeps the
// redundant file test the original has.

#include <stdio.h>

struct Record_004be180 {
    FILE* file;             // +0x0
    int unknown_4;          // +0x4
    void* data;             // +0x8
    int unknown_c;          // +0xc
    int unknown_10;         // +0x10
    char name[1];           // +0x14
};

struct State_004be180 {
    char unknown_0[0x618];
    Record_004be180** records;   // +0x618
    int numRecords;              // +0x61c
};

State_004be180* FUN_004b6220(void);
void __cdecl FUN_004d85a0(void* p);

static void FreeRecord_004be180(Record_004be180* p)
{
    if (p != 0) {
        if (p->file != 0)
            fclose(p->file);
        FUN_004d85a0(p->data);
        FUN_004d85a0(p);
    }
}

// FUNCTION: 0x4be180
void FUN_004be180(void)
{
    State_004be180* state = FUN_004b6220();
    int i;
    for (i = 0; i < state->numRecords; i++) {
        if (state->records[i]->file == 0) {
            state->records[i]->file = fopen(state->records[i]->name, "rb");
            Record_004be180* rec = state->records[i];
            if (rec->file == 0) {
                FreeRecord_004be180(state->records[i]);
                state->records[i] = 0;
                for (int j = i; j < state->numRecords - 1; j++)
                    state->records[j] = state->records[j + 1];
                state->numRecords--;
                i--;
            } else {
                fclose(state->records[i]->file);
                state->records[i]->file = 0;
            }
        }
    }
}
