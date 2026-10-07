// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6,
// edited by deepseek-v4.1, finished by space-bunny-free, finished by
// deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by
// deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny
// Free. Names are provisional.
// The 'O' arm reuses one variable for both of its jobs: frame 0x28 holds
// (flags>>18)&3, is the first %d and is read back for the combine, so nothing
// uninitialised is combined.

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Vec3_00487bf0 {
    int x, y, z;
};

struct Unit {
    int field_0;
    char unknown_4[0x110 - 4];
    unsigned int flags;
};

struct Table_00487bf0 {
    char unknown_0[4];
    int* ids;
};
#pragma pack(pop)

class Class_00438760 {
public:
    unsigned char index;
    char pad[3]; // makes each Outs_t member a dword slot like the original
    Class_00438760(const char* name);
    Class_00438760() {}
};

void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* owner,
                            int id, Vec3_00487bf0* pos, int param_6, int param_7);
void __stdcall GetOrderType(Class_00438760* out, int mode, Unit* unit,
                            int target, Vec3_00487bf0* pos);
int __stdcall FindMissionUnit(char* name, Table_00487bf0* table, int value);
unsigned short __stdcall FindUnitTypeId(char* name);
void __stdcall AttachUnitToPiece(Unit* unit, int target, int a, int b);

struct Outs_t {
    Class_00438760 g, a, m, u, p;
};

// FUNCTION: 0x487bf0
void __stdcall RunInitialMission(Unit* unit, char* text, Table_00487bf0* table)
{
    int count;
    // Locals stay grouped in small structs: loose scalars get a different frame order.
    struct { float f1, f2; int n; Vec3_00487bf0 pos; int move; } L;
    Outs_t out;
    char buf[256];
    int processed = 0, selected = 0;
    struct { float wf; float pf; int fire; } M2;

    while (*text != 0) {
        if (isspace(*text)) {
            do
                text += 1;
            while (isspace(*text));
        }
        L.n = strcspn(text, ",");
        strncpy(buf, text, L.n);
        text += L.n;
        buf[L.n] = 0;
        if (*text == ',')
            text = text + 1;

        switch (buf[0]) {
        case 'O':
        case 'o': {
            // One of the two reads of unit->flags goes through this pointer: stops the load CSE.
            unsigned int* flags = &unit->flags;
            L.move = (unit->flags >> 0x12) & 3;
            M2.fire = (int)((*flags >> 0x14) & 3);
            sscanf(buf + 1, " %d %d", &L.move, &M2.fire);
            unit->flags = (0xffc3ffff & unit->flags)
                          | ((((3 & M2.fire) << 2) | (3 & L.move)) << 0x12);
            break;
        }
        case 'M':
        case 'm': {
            sscanf(buf + 1, " %f %f", &L.f1, &L.f2);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(L.f2 * 65536.0);
            GetOrderType(&out.m, 2, unit, 0, &L.pos);
            AddOrder(out.m, 1, unit, 0, &L.pos, 0, 0);
            processed = 1;
            break;
        }
        case 'U':
        case 'u': {
            sscanf(buf + 1, " %f %f", &L.f1, &L.f2);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(65536.0 * L.f2);
            GetOrderType(&out.u, 5, unit, 0, &L.pos);
            AddOrder(out.u, 1, unit, 0, &L.pos, 0, 0);
            processed = 1;
            break;
        }
        case 'G':
        case 'g': {
            sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
            int target = FindMissionUnit(buf, table, 0);
            if (target != 0) {
                GetOrderType(&out.g, 7, unit, target, 0);
                AddOrder(out.g, 1, unit, target, 0, 0, 0);
                processed = 1;
            }
            break;
        }
        case 'P':
        case 'p': {
            M2.pf = 0.0f;
            sscanf(buf + 1, " %f %f %f", &L.f1, &L.f2, &M2.pf);
            L.pos.x = (int)(L.f1 * 65536.0);
            L.pos.y = 0;
            L.pos.z = (int)(L.f2 * 65536.0);
            GetOrderType(&out.p, 9, unit, 0, &L.pos);
            // The (int)(f * 30.0f) goes straight into AddOrder: no float local.
            AddOrder(out.p, 1, unit, 0, &L.pos, (int)(M2.pf * 30.0f), 0);
            processed = 1;
            selected = 1;
            break;
        }
        case 'A':
        case 'a': {
            if (sscanf(buf + 1, " %f %f", &L.f1, &L.f2) == 2) {
                L.pos.x = (int)(L.f1 * 65536.0);
                L.pos.y = 0;
                L.pos.z = (int)(L.f2 * 65536.0);
                GetOrderType(&out.a, 3, unit, 0, &L.pos);
                AddOrder(out.a, 1, unit, 0, &L.pos, 0, 0);
                selected = 1;
                processed = 1;
            } else {
                sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
                unsigned short id = FindUnitTypeId(buf);
                if (id != 0) {
                    // String kinds are implicit conversions in the argument, never a named temporary.
                    AddOrder("ATTACKUTYPE", 1, unit, 0, 0, id, 0);
                    processed = 1;
                }
            }
            break;
        }
        case 'B':
        case 'b': {
            L.n = 1;
            if (buf[1] == 'w' || buf[1] == 'W') {
                sscanf(buf + 2, " %d", &L.n);
                AddOrder("BUILDWEAPON", 1, unit, 0, 0, 0, L.n);
            } else {
                sscanf(buf + 1, " %[a-zA-Z0-9_.] %d %f %f", buf, &L.n, &L.f1, &L.f2);
                L.pos.x = (int)(L.f1 * 65536.0);
                L.pos.y = 0;
                L.pos.z = (int)(65536.0 * L.f2);
                unsigned short id = FindUnitTypeId(buf);
                if (id != 0) {
                    if (unit->field_0 != 0)
                        AddOrder("MOBILEBUILD", 1, unit, 0,
                                     &L.pos, id, L.n);
                    else
                        AddOrder("BUILDINGBUILD", 1, unit, 0,
                                     0, id, L.n);
                    processed = 1;
                }
            }
            break;
        }
        case 'W':
        case 'w':
            if (buf[1] == 'a' || buf[1] == 'A') {
                // count and target are separate locals: leaves target in eax.
                count = sscanf(buf + 2, " %[a-zA-Z0-9.]", buf);
                int target = 0;
                if (count == 1)
                    target = FindMissionUnit(buf, table, 0);
                if (target == 0)
                    target = (int)unit;
                AddOrder("WAITFORATTACK", 1, unit, target,
                             0, 0, 0);
                processed = 1;
            } else {
                M2.wf = 0.0f;
                L.move = 0;
                sscanf(buf + 1, " %f %d", &M2.wf, &L.move);
                AddOrder("WAIT", 1, unit, 0, 0, (int)(M2.wf * 30.0f), L.move);
                processed = 1;
            }
            break;
        case 'D':
        case 'd':
            AddOrder("SELFDESTRUCTFG", 1, unit, 0, 0, 1, 0);
            // After the AddOrder call, not before it.
            processed = 1;
            selected = 1;
            break;
        case 'S':
        case 's':
            AddOrder("MAKESELECTABLE", 1, unit, 0, 0, 0, 0);
            processed = 1;
            selected = 1;
            break;
        case 'I':
        case 'i': {
            sscanf(buf + 1, " %[a-zA-Z0-9_.]", buf);
            int target = FindMissionUnit(buf, table, 0);
            if (target != 0)
                AttachUnitToPiece(unit, target, -1, 0);
            break;
        }
        }
    }
    if (processed) {
        unit->flags &= ~0x20u;
        if (selected == 0)
            AddOrder("MAKESELECTABLE", 1, unit, 0, 0, 0, 0);
    }
}