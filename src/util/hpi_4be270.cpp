// Decompiled by space-bunny-free. Names are provisional.
// Takes a record out of the state's list: closes the record's file, frees its
// buffer and the record itself, then shifts the tail of the list down over the
// hole and shrinks the count. A record that is not in the list is left alone.

#include <stdio.h>

struct Record_004be270;

struct State_004be270 {
    char unknown_0[0x618];
    Record_004be270** records;      // +0x618
    int numRecords;                 // +0x61c
};

struct Record_004be270 {
    FILE* file;                     // +0x0
    int unknown_4;                  // +0x4
    void* data;                     // +0x8
};

State_004be270* GetDisplay(void);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4be270
void __stdcall FUN_004be270(Record_004be270* pRecord)
{
    State_004be270* state = GetDisplay();
    int i;
    for (i = 0; i < state->numRecords; i++) {
        if (state->records[i] == pRecord) {
            Record_004be270* p = state->records[i];
            if (p != 0) {
                if (p->file != 0)
                    fclose(p->file);
                FUN_004d85a0(p->data);
                FUN_004d85a0(p);
            }
            state->records[i] = 0;
            for (int j = i; j < state->numRecords - 1; j++) {
                state->records[j] = state->records[j + 1];
            }
            state->numRecords--;
            return;
        }
    }
}
