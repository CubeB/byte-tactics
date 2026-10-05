// Decompiled by Opus. Names are provisional.
// Registers a table with RegisterOrderTypes under a numeric id; one of several
// small functions doing the same for different tables.

void __stdcall RegisterOrderTypes(void* table, int id);

extern char DAT_004fc490[];

// FUNCTION: 0x403180
void RegisterUnitOrders()
{
    RegisterOrderTypes(DAT_004fc490, 0x17);
}
