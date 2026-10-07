// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Returns 0 when MCI reports the type of track 1 on the CD as "audio",
// otherwise 1.
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

class Class_004ce460 {
public:
    int IsFirstTrackData();
};

// FUNCTION: 0x4ce460
int Class_004ce460::IsFirstTrackData()
{
    int type;
    char buf[32];
    mciSendStringA("set cdaudio time format tmsf", 0, 0, 0);
    if (mciSendStringA("status cdaudio type track 1", buf, 0x20, 0) != 0)
        return 1;
    if (strcmp(buf, "audio") == 0) {
        return 0;
    } else {
        // Keep the type == 0 test even though both branches return 1.
        type = strcmp(buf, "other");
        if (type == 0) {
            return 1;
        }
        return 1;
    }
}
