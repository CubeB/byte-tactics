// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Loads table number `table` (0-based) of gamedata\los.tdf: builds the
// section name "TABLE%d" (table + 1), rewinds the TDF reader, finds the
// section and, if it is there, resizes the table to numlines * 4 lines and
// has each line read itself (LoadLosLine) from the four quarter blocks
// (line i, numlines + i, 2 * numlines + i, 3 * numlines + i).
// Header set matters: fewer headers change the table index's register choice.
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

class TdfFile;

// A line of a table, read from the TDF file.
struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

class LosLine {
public:
    void LoadLosLine(TdfFile* file, short line, int col);
};

// One table: a vector of lines (see 0x4335f0.cpp).
class LosTable : public std::vector<Elem_00434360> {
public:
    // Inline copy of GetLosLine.
    Elem_00434360* GetLine(short i) { return &(*this)[i]; }
    // Inline copy of FUN_004335f0.
    void SetNumLines(short n) { resize(n * 4); }
};

class TdfRecord {
public:
    int GetFieldInt(const char* name, int def);
};

class TdfFile {
public:
    void* root;                        // +0x0
    TdfRecord* current;                // +0x4

    int SelectRecord(char* name);
    void ResetCurrentRecord();
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    int LoadFile(char* path);
    void StripComments(char* text);
    int SelectRecordAt(int index);
    void LoadBuffer(char* buffer, int size, int flags, char* name);
};

class LosTables {
public:
    std::vector<LosTable> tables; // +0x0

    // Inline copy of GetLosTable: table number n, counted from 1.
    LosTable* GetTable(short n)
    {
        // Decrements the parameter itself: keeps table 16 bits wide.
        n--;
        return &tables[n];
    }
    void LoadLosTable(TdfFile* file, short table);
};

// FUNCTION: 0x433380
void LosTables::LoadLosTable(TdfFile* file, short table)
{
    char name[32];
    sprintf(name, "TABLE%d", table + 1);
    ((TdfFile*)file)->ResetCurrentRecord();
    if (file->SelectRecord(name)) {
        LosTable* t = GetTable(table + 1);
        short numlines = (short)file->current->GetFieldInt("numlines", 0);
        t->SetNumLines(numlines);
        // Short locals before the loop: match the original induction variables.
        short n2 = numlines * 2;
        short n3 = numlines * 3;
        for (short i = 0; i < numlines; i++) {
            ((LosLine*)t->GetLine(i))->LoadLosLine(file, i, 0);
            ((LosLine*)t->GetLine(numlines + i))->LoadLosLine(file, i, 1);
            ((LosLine*)t->GetLine(n2 + i))->LoadLosLine(file, i, 2);
            ((LosLine*)t->GetLine(n3 + i))->LoadLosLine(file, i, 3);
        }
    }
}
