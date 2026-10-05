// Decompiled by Claude Opus 5.5. Names are provisional.
// Network statistics text: about once a second (more than 30 ticks), turns
// the packet and byte counters (see 0x415f40 and 0x416150) into per-second
// rates and a compression percentage, then prints the latest values.
// Match notes: the "previous" counters are all updated after the rates are
// computed (the scheduler moves the stores up again), which is what keeps the
// current counters in callee-saved registers; and it needs a header set
// (headers.py: <windows.h> <stdio.h> <math.h>).
#include <windows.h>
#include <stdio.h>
#include <math.h>
extern int DAT_00511bc0;
extern int DAT_00511bc4;
extern int DAT_00511bc8;
extern int DAT_00511c34;
extern int DAT_00511c48;
extern int DAT_00511c50;
extern int g_compressedBytesSent;
extern unsigned int g_netStatsTick;
extern int g_lastMessageBytesReceived;
extern int g_lastMessageBytesSent;
extern int g_lastCompressedBytesSent;
extern int g_lastPacketBytesSent;
extern int g_lastPacketBytesReceived;
extern int g_lastPacketsSent;
extern int g_lastPacketsReceived;
extern unsigned int g_messageBytesReceivedRate;
extern unsigned int g_messageBytesSentRate;
extern int g_compressionPercent;
extern unsigned int g_packetBytesSentRate;
extern unsigned int g_packetBytesReceivedRate;
extern unsigned int g_packetsSentRate;
extern unsigned int g_packetsReceivedRate;

unsigned int __cdecl FUN_004b6340();

// FUNCTION: 0x415fa0
void __stdcall FormatNetStats(char* text)
{
    unsigned int now = FUN_004b6340();
    unsigned int elapsed = now - g_netStatsTick;
    if (elapsed > 30) {
        g_netStatsTick = now;
        g_messageBytesReceivedRate = (DAT_00511bc0 * 30 - g_lastMessageBytesReceived * 30) / elapsed;
        int packets = DAT_00511bc4 - g_lastMessageBytesSent;
        g_messageBytesSentRate = packets * 30 / elapsed;
        g_lastMessageBytesReceived = DAT_00511bc0;
        g_lastMessageBytesSent = DAT_00511bc4;
        if (packets > 0)
            g_compressionPercent = (g_lastCompressedBytesSent + packets - g_compressedBytesSent) * 100 / packets;
        else
            g_compressionPercent = 0;
        g_lastCompressedBytesSent = g_compressedBytesSent;
        g_packetBytesSentRate = (DAT_00511bc8 * 30 - g_lastPacketBytesSent * 30) / elapsed;
        g_packetBytesReceivedRate = (DAT_00511c50 * 30 - g_lastPacketBytesReceived * 30) / elapsed;
        g_packetsSentRate = (DAT_00511c34 * 30 - g_lastPacketsSent * 30) / elapsed;
        g_packetsReceivedRate = (DAT_00511c48 * 30 - g_lastPacketsReceived * 30) / elapsed;
        g_lastPacketBytesSent = DAT_00511bc8;
        g_lastPacketBytesReceived = DAT_00511c50;
        g_lastPacketsSent = DAT_00511c34;
        g_lastPacketsReceived = DAT_00511c48;
    }
    sprintf(text, "pS=%4d pR=%4d (S=%d/%4d, R=%d/%4d) C=%3d%%\n", g_messageBytesSentRate, g_messageBytesReceivedRate,
            g_packetsSentRate, g_packetBytesSentRate, g_packetsReceivedRate, g_packetBytesReceivedRate, g_compressionPercent);
}
