// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Loads table number `table` (0-based) of gamedata\los.tdf: builds the
// section name "TABLE%d" (table + 1), rewinds the TDF reader, finds the
// section and, if it is there, resizes the table to numlines * 4 lines and
// has each line read itself (LoadLosLine) from the four quarter blocks
// (line i, numlines + i, 2 * numlines + i, 3 * numlines + i).
//
// The shape comes from the out-of-line helpers later in the same original
// file, which /Ob2 inlined here: GetTable is GetLosTable (a 1-based table
// number that it decrements in place), GetLine is GetLosLine and
// SetNumLines is FUN_004335f0 (the inlined vector::resize). What they pin
// down:
//   * `n--` on GetTable's own short parameter keeps `table` 16 bits wide in
//     di with a separate `movsx` at each use; indexing tables[table]
//     directly lets MSVC share one sign-extended copy and puts `this` in edi.
//   * Going through the helpers spends the inline budget the way the
//     original did, so the vector<Elem_00434360> size/insert/erase and the
//     fill value's constructor and destructor stay out of line with the
//     real <vector> header.
//   * n2 and n3 as short locals before the loop give the original's
//     induction variables (2n spilled, n + i and 3n + i rebuilt from 2n + i);
//     writing numlines * 2 + i in the calls makes separate counters instead.
// The headers set the compiler state: with only <stdio.h> and <vector> the
// same source gives 81% (`movsx edi, di` instead of `movsx eax, di` for the
// table index). <windows.h> alone also matches, but at the edge: three more
// declarations flip it back. With <ddraw.h> as well, the file sits in the
// middle of the matching range.
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

class Class_004c3410;

// A line of a table, read from the TDF file.
struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

class Class_004336f0 {
public:
    void LoadLosLine(Class_004c3410* file, short line, int col);
};

// One table: a vector of lines (see 0x4335f0.cpp).
class Class_004335f0 : public std::vector<Elem_00434360> {
public:
    // Inline copy of GetLosLine.
    Elem_00434360* GetLine(short i) { return &(*this)[i]; }
    // Inline copy of FUN_004335f0.
    void SetNumLines(short n) { resize(n * 4); }
};

class Class_004c46c0 {
public:
    int GetFieldInt(const char* name, int def);
};

class Class_004c3410 {
public:
    void* root;                        // +0x0
    Class_004c46c0* current;           // +0x4

    int SelectRecord(char* name);
};

class Class_004c3e10 {
public:
    void ResetCurrentRecord();
};

class Class_00433380 {
public:
    std::vector<Class_004335f0> tables; // +0x0

    // Inline copy of GetLosTable: table number n, counted from 1.
    Class_004335f0* GetTable(short n)
    {
        n--;
        return &tables[n];
    }
    void LoadLosTable(Class_004c3410* file, short table);
};

// FUNCTION: 0x433380
void Class_00433380::LoadLosTable(Class_004c3410* file, short table)
{
    char name[32];
    sprintf(name, "TABLE%d", table + 1);
    ((Class_004c3e10*)file)->ResetCurrentRecord();
    if (file->SelectRecord(name)) {
        Class_004335f0* t = GetTable(table + 1);
        short numlines = (short)file->current->GetFieldInt("numlines", 0);
        t->SetNumLines(numlines);
        short n2 = numlines * 2;
        short n3 = numlines * 3;
        for (short i = 0; i < numlines; i++) {
            ((Class_004336f0*)t->GetLine(i))->LoadLosLine(file, i, 0);
            ((Class_004336f0*)t->GetLine(numlines + i))->LoadLosLine(file, i, 1);
            ((Class_004336f0*)t->GetLine(n2 + i))->LoadLosLine(file, i, 2);
            ((Class_004336f0*)t->GetLine(n3 + i))->LoadLosLine(file, i, 3);
        }
    }
}
