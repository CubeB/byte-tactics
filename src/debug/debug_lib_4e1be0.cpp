// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct EventEntry {
    int id;                            // +0x0
    int unk4;                          // +0x4
    int unk8;                          // +0x8
    int unkc;                          // +0xc
};

extern EventEntry* g_pmcEventCatalog;
extern int g_pmcEventCount;
extern EventEntry g_pentiumProEvents[];
extern EventEntry g_pentiumEvents[];
extern EventEntry g_pmcEvent0;         // "Event0"
extern EventEntry g_pmcEvent1;         // "Event1"

void __cdecl SyncPerformanceSettings(int arg);
unsigned char HasPerfCounters(void);
int GetCpuFamily(void);

// FUNCTION: 0x4e1be0
void InitPerformanceEvents(void)
{
    if (g_pmcEventCatalog == 0) {
        g_pmcEventCatalog = g_pentiumProEvents;
        g_pmcEventCount = 0x11;
        SyncPerformanceSettings(1);
        if (HasPerfCounters() != 0) {
            if (GetCpuFamily() < 6) {
                g_pmcEventCatalog = g_pentiumEvents;
                g_pmcEventCount = 8;
            }
            EventEntry* table = g_pmcEventCatalog;
            int count = g_pmcEventCount;
            // The pointer locals (declared in this order) make MSVC hoist the
            // two Event ids into the loop preheader in the original order.
            EventEntry* e1 = &g_pmcEvent1;
            EventEntry* e0 = &g_pmcEvent0;
            for (int i = 0; i < count; i++) {
                if (e0->id == table[i].id)
                    *e0 = table[i];
                if (e1->id == table[i].id)
                    *e1 = table[i];
            }
            if (g_pmcEvent0.unk4 == 0)
                g_pmcEvent0 = table[0];
            if (g_pmcEvent1.unk4 == 0)
                g_pmcEvent1 = table[0];
        }
    }
}
