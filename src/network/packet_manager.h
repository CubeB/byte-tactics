// PacketManager: the network packet manager behind the global g_packetManager
// (0x513000): the eleven per-player packet channels, the outgoing send buffer
// and the packet receiver, with the packet pools, rings and per-player frame
// queues they are built from. The one declaration of the class for the files
// that call its methods, carrying the method signatures the views agree on.
// The data members stay in packets.cpp's own view, since the channel and
// receiver arrays behind them have no shared header yet, and so do the methods
// only packets.cpp calls; the types behind the pointers stay private to it.
#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

class PacketChannel;

class PacketManager {
public:
    void NopRet_D();
    int SendAllQueued(int force);
    void* QueuePacket(int frame, int dpid, void* data, unsigned int len);
    int QueueOnChannel(int frame, PacketChannel* channel, int data, int len);
    void SetDefaultSendPacing(int rate);
    void HandleIntegrityNop(int, int, int);
};

#endif
