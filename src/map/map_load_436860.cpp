// Decompiled by Claude Opus 5.5. Names are provisional.
// Picks the mission file's "Schema <n>" block for a game type. Type 1 (a
// campaign mission) takes the first schema whose type is the current
// difficulty (then the others); types 2 and 3 (multiplayer) try the network
// schemas and keep the one whose count of StartPos specials fits the number
// of players best. The chosen block's name goes to `schema` and its section
// becomes the parser's current section. Returns 1 when one was found.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Slot_00436860 {
    int active;                        // +0x0
    char unknown_4[0x14];
};

struct Game {
    char unknown_0[0x29a0];
    Slot_00436860* slots;              // +0x29a0
    char unknown_29a4[0x2a3c - 0x29a4];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2c28 - 0x2a3e];
    int playerIds[10];                 // +0x2c28
    char unknown_2c50[0x37eee - 0x2c50];
    int difficulty;                    // +0x37eee
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void __stdcall FatalError(char* text);

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, size_t size, char* def);
};

class Class_004c44c0 {
public:
    TdfRecord* GetSubRecord(int index);
};

class Class_004c3410 {
public:
    int SelectRecord(char* name);
};

class Class_004c3e10 {
public:
    void ResetCurrentRecord();
};

class Class_004c2ea0 {
public:
    int field_0;
    TdfRecord* current;                 // +0x4
    int field_8;
};

class Class_00435c00 {
public:
    int FUN_00436860(int type, Class_004c2ea0* parser, char* schema);
};

// FUNCTION: 0x436860
int Class_00435c00::FUN_00436860(int type, Class_004c2ea0* parser, char* schema)
{
    int order[4];
    char name[0x10];
    char what[0x10];
    char kind[0x20];
    char* names[7];
    memset(order, -1, sizeof(order));
    names[0] = "Easy";
    names[1] = "Medium";
    names[2] = "Hard";
    names[3] = "Network 1";
    names[4] = "Network 2";
    names[5] = "Network 3";
    names[6] = "Network 4";
    int players = g_game->numPlayers;
    switch (type) {
    case 1:
        switch (g_game->difficulty) {
        case 0:
            order[0] = 0;
            order[1] = 1;
            order[2] = 2;
            break;
        case 1:
            order[0] = 1;
            order[1] = 0;
            order[2] = 2;
            break;
        case 2:
            order[0] = 2;
            order[1] = 1;
            order[2] = 0;
            break;
        default:
            return 0;
        }
        break;
    case 3: {
        for (int i = 0; i < 10; i++) {
            if (g_game->playerIds[i] != -1 && g_game->playerIds[i] != 0)
                players = i + 1;
        }
        order[0] = 3;
        order[1] = 4;
        order[2] = 5;
        order[3] = 6;
        break;
    }
    case 2: {
        for (int i = 0; i < 10; i++) {
            if (g_game->slots[i].active != 0)
                players = i + 1;
        }
        order[0] = 3;
        order[1] = 4;
        order[2] = 5;
        order[3] = 6;
        break;
    }
    case 0:
        return 0;
    default:
        return 0;
    }

    int bestCount = 0;
    int found = 0;
    TdfRecord* best = 0;
    for (int k = 0; k < 4; k++) {
        int d = order[k];
        if (d == -1)
            break;
        for (int n = 0; ; n++) {
            ((Class_004c3e10*)parser)->ResetCurrentRecord();
            if (!((Class_004c3410*)parser)->SelectRecord("GlobalHeader"))
                FatalError("Very bad news!  No MSG!");
            sprintf(name, "Schema %i", n);
            if (!((Class_004c3410*)parser)->SelectRecord(name))
                break;
            if (!parser->current->GetFieldString(kind, "type", 0x20, DAT_005119b8))
                continue;
            if (_strcmpi(kind, names[d]) != 0)
                continue;
            if (type != 3 && type != 2 || schema == 0) {
                if (schema)
                    strcpy(schema, name);
                return 1;
            }
            TdfRecord* section = parser->current;
            int count = 0;
            if (((Class_004c3410*)parser)->SelectRecord("specials")) {
                Class_004c44c0* specials = (Class_004c44c0*)parser->current;
                TdfRecord* s;
                for (int i = 0; (s = specials->GetSubRecord(i)) != 0; i++) {
                    if (s->GetFieldString(what, "specialwhat", 0x10, DAT_005119b8)) {
                        static int len = strlen("StartPos");
                        if (_strnicmp(what, "StartPos", len) == 0)
                            count++;
                    }
                }
            }
            if (count != 0 && (count == players || players == 0 || (count > bestCount && bestCount != players))) {
                best = section;
                bestCount = count;
                found = 1;
                strcpy(schema, name);
            }
        }
    }
    if (best)
        parser->current = best;
    return found;
}
