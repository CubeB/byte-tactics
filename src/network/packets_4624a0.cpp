// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, mimo-v2.6-pro and opus. Names are provisional.
//
// Sends one player's queued packets: every packet whose frame matches the
// queue head's is appended to the outgoing buffer (g_packetManager, whose
// buffer and size fields are DAT_0051e2f4 and DAT_0051e2f8), the others go
// back on the queue, and the buffer is handed to g_sendCondenser with the frame
// and the player's DPID.
//
// What matched it (opus, #4310), rebuilt from the disassembly instead of the
// 81.7% file's address-escape and do/while(0) devices:
//  * A packet's data is `owner->data[offset]`: +0x4 is the offset and +0xc
//    the owning buffer (PacketBuffer, data at +0x14). Written twice as an
//    expression, it gives the original's base+offset for the log call and
//    the fresh reload after FUN_004b6340, with the zero kept in ebx.
//  * `if (now >= nextSend || force != 0) { ...; while ((n = queue.count) != 0)
//    {...} } return 1;` gives the original's three epilogues.
//  * The send block reads DAT_0051e2f8, dpid and DAT_0051e2f4 into locals
//    before the log call, in that order: equal priorities, so the one written
//    first takes edi (`nbytes` before `id`; 97.5% the other way round).
unsigned int __cdecl FUN_004b6340();
void __cdecl PacketTrace(const char* fmt, ...);

extern char* g_game;
extern int* DAT_0051e2f4;
extern unsigned int DAT_0051e2f8;

class PacketManager {
public:
    int AppendToSendBuffer(unsigned char* data, unsigned int len);
};

class Class_004626e0 {
public:
    void SendPacketTo(void* session, int from, int value, void* data, int size);
};

extern PacketManager g_packetManager;
extern Class_004626e0 g_sendCondenser;

struct Buffer_004624a0 {
    char unknown_0[0x14];
    unsigned char data[0x42a];       // +0x14
};

struct Packet_004624a0 {
    int frame;                       // +0x0
    int offset;                      // +0x4
    int size;                        // +0x8
    Buffer_004624a0* owner;          // +0xc
    int queued;                      // +0x10
    unsigned int time;               // +0x14
};

// The ring buffer of 0x462370 (push) and 0x4623b0 (pop), inlined here.
class Queue_004624a0 {
public:
    int count;                            // +0x0
    int readIdx;                          // +0x4
    int writeIdx;                         // +0x8
    Packet_004624a0* buf[0x400];          // +0xc

    Packet_004624a0* Peek()
    {
        if (count > 0)
            return buf[readIdx];
        return 0;
    }

    Packet_004624a0* Pop()
    {
        if (count > 0) {
            count--;
            Packet_004624a0* value = buf[readIdx];
            readIdx++;
            if (readIdx < 0x400)
                return value;
            readIdx = 0;
            return value;
        }
        return 0;
    }

    int Push(Packet_004624a0* value)
    {
        if (count < 0x400) {
            writeIdx = writeIdx + 1;
            if (writeIdx >= 0x400)
                writeIdx = 0;
            buf[writeIdx] = value;
            count = count + 1;
            return 1;
        }
        return 0;
    }
};

class Class_004624a0 {
public:
    int field_00;                     // +0x0
    unsigned int ticks;               // +0x4
    void** field_08;                  // +0x8
    char unknown_c[4];
    int frame;                        // +0x10
    int dpid;                         // +0x14
    char unknown_18[8];
    unsigned int queuedBytes;         // +0x20
    unsigned int nextSend;            // +0x24
    char unknown_28[0x10];
    Queue_004624a0 queue;             // +0x38

    int SendQueued(int force);
};

// FUNCTION: 0x4624a0
int Class_004624a0::SendQueued(int force)
{
    unsigned int now = FUN_004b6340();
    PacketTrace("player: %ld, ticks betw sends=%lu, nextsend=%lu, gametimereal=%lu\n",
                 dpid, ticks, nextSend, now);
    if (now >= nextSend || force != 0) {
        nextSend = now + ticks;
        int n;
        while ((n = queue.count) != 0) {
            int headFrame = queue.Peek()->frame;
            int sent = 0;
            PacketTrace("assigning packets to frame number: %ld\n", frame);
            for (int i = 0; i < n; i++) {
                Packet_004624a0* entry = queue.Peek();
                queue.Pop();
                if (entry->frame == headFrame) {
                    PacketTrace("extracted packet (len=%ld, type=%d, data=\"%s\")\n",
                                 entry->size, entry->owner->data[entry->offset],
                                 &entry->owner->data[entry->offset + 1]);
                    entry->queued = frame;
                    entry->time = FUN_004b6340();
                    if (g_packetManager.AppendToSendBuffer(&entry->owner->data[entry->offset], entry->size) == 0)
                        return 0;
                    sent++;
                } else {
                    queue.Push(entry);
                }
            }
            queuedBytes = 0;
            if (sent > 0) {
                PacketTrace("sending %ld packets in frame: %ld\n", sent, frame);
                *DAT_0051e2f4 = dpid != 0 ? -1 : frame;
                unsigned int nbytes = DAT_0051e2f8;
                int id = dpid;
                int* data = DAT_0051e2f4;
                PacketTrace("bytes to send to (DPID)(%ld): %ld\n", id, nbytes);
                g_sendCondenser.SendPacketTo(g_game + 0x14, headFrame, id, data, nbytes);
                DAT_0051e2f8 = DAT_0051e2f4 != 0 ? 4 : 0;
                frame--;
                if (frame >= -1)
                    frame = -2;
                if (queue.count == 0)
                    return 1;
            }
        }
    }
    return 1;
}
