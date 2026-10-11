// Decompiled by Haiku. Names are provisional.
// Stays in its own file: unit_orders.cpp inlines it into the repair patrol's vector insert.

// FUNCTION: 0x406c70
void __stdcall CopyDwordIfNonNull(int* dest, int* src)
{
    if (dest != 0) {
        *dest = *src;
    }
}
