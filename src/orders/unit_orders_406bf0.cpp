// Decompiled by Opus. Names are provisional.
// Registers a table with RegisterOrderTypes under a numeric id; one of several
// small functions doing the same for different tables.

void __stdcall RegisterOrderTypes(void* table, int id);

extern char DAT_004fc6e8[];

// FUNCTION: 0x406bf0
void RegisterGroundOrders()
{
    RegisterOrderTypes(DAT_004fc6e8, 0x16);
}
