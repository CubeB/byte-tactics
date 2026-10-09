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
    int SendAllQueued(int param_1);
    void* QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
    int QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4);
    void SetDefaultSendPacing(int rate);
    void HandleIntegrityNop(int, int, int);
};

#endif
