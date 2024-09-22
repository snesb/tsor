#pragma once
#include <map>

#pragma pack(push, 1)
/**
 * Packed struct representing MPEG-TS packet header
 */
typedef struct Packet {
    unsigned int    sync:   8;
    bool            tei:    1;
    bool            pusi:   1;
    bool            pri:    1;
    unsigned int    pid:    13;
    unsigned int    tsc:    2;
    unsigned int    afc:    2;
    unsigned int    cont:   4;
    unsigned char   data[184];
} Packet;
#pragma pack(pop)

/**
 * Map numeric PID value to abbreviated table name
 */
extern std::map<unsigned int,const char*> M2TS_PID;

Packet packet_test();
