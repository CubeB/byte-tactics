// UnitSync: the shared unit list every player is checked against, the 0x68-byte
// object behind the sync pointer at g_game+0x2a30 (constructor 0x46d040). The
// one declaration of the class for the files that call its methods; the
// unit_sync files that define them keep their own views, since they need the
// real std::map, std::vector and std::list members (unit_sync.cpp models them
// by hand to keep _Tree's insert, the vector's _Destroy and the list's erase
// out of line). The container members stay opaque here: a header that pulls in
// nothing cannot name them, and every file that reads them keeps its own view.
#ifndef UNIT_SYNC_H
#define UNIT_SYNC_H

struct Unit_0046e330;
struct UnitSyncEntry;
struct UnitSyncPacket;
struct Target_0046d530;
struct Source_0046d630;

class UnitSync {
public:
    // +0x00 the std::map<unsigned int, UnitSyncEntry> (map in unit_sync.cpp and
    // 46dad0, rects in 46ca60, 46d1a0 and unit_sync_player), then the
    // std::vector of player sync records (players or elems), then the
    // std::list<int> of changed keys (ids or queue), then the three-word
    // sequencer (unit_sync.cpp's Sub_0046d040, whose words the other views name
    // seqSent, seqCur and seqMax), then the two queued-packet vectors (seqSentQueue
    // and seqHeldQueue).
    char unknown_0[0x2c];
    int seqSent;                       // +0x2c
    int seqCur;                        // +0x30
    int seqMax;                        // +0x34
    char unknown_38[0x20];             // +0x38
    int direct;                        // +0x58
    int pendingPlayerCount;            // +0x5c, the sync sub-object's first word
    int checksumProgress;              // +0x60, the sync sub-object's last word
    int disabled;                      // +0x64

    UnitSync(int param);
    ~UnitSync();
    void ResetEntries();
    void SendSyncPacket(unsigned int* param_1, UnitSyncPacket* param_2, int unused);
    void ReceiveSyncPacket(void* param_1, int param_2);
    void SendSyncMessage(unsigned char arg, int a, int b, int unused);
    void SendSyncMessageTo(Target_0046d530* target, unsigned char arg, int a, int b, int unused);
    void SendEntryTo(Target_0046d530* target, unsigned char arg, Source_0046d630* src, int unused);
    void HandleSyncPacket(UnitSyncPacket* packet, unsigned char player);
    void NotifyEntryChanged(unsigned int param_1);
    void CheckUnitAvailable(unsigned int key, int y);
    void ProcessSync();
    char* GetSyncStatusText();
    int AllPlayersSynced();
    int IsPlayerSynced(int id);
    void ApplyToUnitTypes();
    int PopChangedEntry(UnitSyncEntry* out);
    int GetUnitEntry(Unit_0046e330* unit, UnitSyncEntry* out);
    int ToggleUnitAllowed(Unit_0046e330* unit);
    int DisallowUnit(Unit_0046e330* unit);
    int AllowUnit(Unit_0046e330* unit);
    void SetUnitLimit(Unit_0046e330* unit, int value);
};

#endif
