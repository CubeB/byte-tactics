// Decompiled by GPT-6 Astra. Names are provisional.
#include <stdio.h>
#include <string.h>
#include <vector>
class Mission { public: const char* FUN_004356c0(int); };
#pragma pack(push, 1)
struct Player { char name[0x48]; unsigned char control; char pad49[331-0x49]; };
struct UnitDef { char name[32]; char description[585-32]; };
struct Base { signed char value, metal, energy; };
struct AI {
    char pad0[0x65]; std::vector<Base> base; char pad75[0xad-0x75]; std::vector<unsigned char> result;
    char padbd[16]; std::vector<int> limits;
};
struct Game {
    char pad0[0x1b8e]; Player players[10];
    char padPlayers[0x1438f-0x1b8e-3310]; int typeCount; char pad14393[8]; UnitDef* types;
    char pad1439f[0x37eee-0x1439f]; int difficulty;
    char pad37ef2[0x38a47-0x37ef2]; unsigned int tick;
    char pad38a4b[0x391e9-0x38a4b]; Mission* net;
};
#pragma pack(pop)
extern Game* g_game;
extern AI* g_playerAI[];
// FUNCTION: 0x40c250
void __stdcall DumpPlayerAI(int player, FILE* file)
{
    AI* ai=g_playerAI[player];
    char buffer[256];
    unsigned int hours=g_game->tick/108000;
    int remaining=g_game->tick-hours*108000;
    int minutes=remaining/1800;
    int seconds=(remaining-minutes*1800)/30;
    sprintf(buffer,"%02d:%02d:%02d",hours,minutes,seconds);
    fprintf(file,"gametime: '%s'\r\n",buffer);
    fprintf(file,"player:   '%s' num: %d\r\n",g_game->players[player].name,player);
    switch(g_game->players[player].control) {
    case 1: strcpy(buffer,"HUMAN"); break;
    case 2: strcpy(buffer,"AI"); break;
    default: strcpy(buffer,"INVALID"); break;
    }
    fprintf(file,"controller: %s\r\n",buffer);
    fprintf(file,"terrain:    '%s'\r\n",g_game->net->FUN_004356c0(1));
    fprintf(file,"profile:    '%s'\r\n",g_game->net->FUN_004356c0(7));
    const char* difficulties[]={"EASY","MEDIUM","HARD"};
    fprintf(file,"difficulty: '%s'\r\n",difficulties[g_game->difficulty]);
    fprintf(file,"================================================\r\n");
    fprintf(file,"<limit> - <base:baseML:baseEL> : <end result - before economy-based tweaks> - <unit name>\r\n");
    for (unsigned short i=1;i<g_game->typeCount;++i) {
        UnitDef* type=&g_game->types[i];
        if (ai->limits[i]<0) fprintf(file,"n/a ");
        else fprintf(file,"%4d",ai->limits[i]);
        sprintf(buffer," - %3d : %3d : %3d = %3d - '%s\t\t:%s'\r\n",ai->base[i].value,ai->base[i].metal,ai->base[i].energy,ai->result[i],type->description,type->name);
        // Original passes the formatted unit text as a format string, so percent signs are interpreted again.
        fprintf(file,buffer);
    }
}
