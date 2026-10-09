// The mission order type: one order type, its index into the sorted
// mission-order table at g_missionOrderTableBegin, one byte wide and passed by
// value. The constructor from a name is at 0x438760; the default constructor
// and operator== are inline for the views that build or compare two.
#ifndef MISSION_TYPE_H
#define MISSION_TYPE_H

class MissionType {
public:
    unsigned char index;
    MissionType() : index(0) {}
    MissionType(const char* name);
    int operator==(const MissionType& other) const { return index == other.index; }
};

#endif
