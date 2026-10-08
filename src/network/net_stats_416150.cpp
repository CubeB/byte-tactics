// Decompiled by Opus. Names are provisional.
// Network statistics: about once a second (more than 30 ticks), turns the
// sent and received byte counters (see 0x415f40) into per-second rates, and
// returns the latest rates.

// Must stay out of net_stats.cpp (no <ddraw.h> here); <stdlib.h> must stay too.
#include <stdlib.h>

extern int g_packetBytesSent;
extern int g_packetBytesReceived;
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
        g_bytesSentPerSecond = (g_packetBytesSent * 30 - g_byteRatesLastSent * 30) / elapsed;
        g_bytesReceivedPerSecond = (g_packetBytesReceived * 30 - g_byteRatesLastReceived * 30) / elapsed;
        g_byteRatesLastSent = g_packetBytesSent;
        g_byteRatesLastReceived = g_packetBytesReceived;
    }
    *sent = g_bytesSentPerSecond;
    *received = g_bytesReceivedPerSecond;
}
