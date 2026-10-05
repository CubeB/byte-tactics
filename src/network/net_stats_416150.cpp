// Decompiled by Opus. Names are provisional.
// Network statistics: about once a second (more than 30 ticks), turns the
// sent and received byte counters (see 0x415f40) into per-second rates, and
// returns the latest rates. Needs a header (headers.py: <stdlib.h>) for the
// first rate's store to be scheduled after the second counter's update.
#include <stdlib.h>

extern int DAT_00511bc8;
extern int DAT_00511c50;
extern int g_byteRatesLastSent;
extern int g_byteRatesLastReceived;
extern unsigned int g_byteRatesTick;
extern unsigned int g_bytesSentPerSecond;
extern unsigned int g_bytesReceivedPerSecond;

unsigned int __cdecl GetTicks();

// FUNCTION: 0x416150
void __stdcall GetByteRates(unsigned int* sent, unsigned int* received)
{
    unsigned int now = GetTicks();
    unsigned int elapsed = now - g_byteRatesTick;
    if (elapsed > 30) {
        g_byteRatesTick = now;
        g_bytesSentPerSecond = (DAT_00511bc8 * 30 - g_byteRatesLastSent * 30) / elapsed;
        g_bytesReceivedPerSecond = (DAT_00511c50 * 30 - g_byteRatesLastReceived * 30) / elapsed;
        g_byteRatesLastSent = DAT_00511bc8;
        g_byteRatesLastReceived = DAT_00511c50;
    }
    *sent = g_bytesSentPerSecond;
    *received = g_bytesReceivedPerSecond;
}
