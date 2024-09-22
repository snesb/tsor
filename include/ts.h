#pragma once
#include <map>

namespace ts {
    #pragma pack(push, 1)
    /**
     * Packed struct representing MPEG-TS packet header
     */
    struct Packet {
        unsigned int    sync:   8;
        bool            tei:    1;
        bool            pusi:   1;
        bool            pri:    1;
        unsigned int    pid:    13;
        unsigned int    tsc:    2;
        unsigned int    afc:    2;
        unsigned int    cont:   4;
        unsigned char   data[184];
    };
    #pragma pack(pop)

    /**
     * Map standard table names to PID value (ETSI EN 300 468)
     */
    enum PID {
        PAT  = 0x0000,              // Program Association Table
        CAT  = 0x0001,              // Conditional Access Table
        TSDT = 0x0002,              // Transport Stream Description Table
        IPMP = 0x0003,              // Intellectual Property Management and Protection
        NIT  = 0x0010,              // DVB Network Information Table
        SDT  = 0x0011,              // DVB Service Description Table
        BAT  = 0x0011,              // DVB Bouquet Association Table
        EIT  = 0x0012,              // DVB Event Information Table
        RST  = 0x0013,              // DVB Running Status Table
        TDT  = 0x0014,              // DVB Time and Date Table
        TOT  = 0x0014,              // DVB Time Offset Table
        NSYN = 0x0015,              // DVB Network Synchronisation
        RNT  = 0x0016,              // DVB Resolution Authority Record (RAR) Notification Table
        LLIS = 0x001C,              // DVB Link-local Inband Signalling
        MEAS = 0x001D,              // DVB Measurement
        DIT  = 0x001E,              // DVB Discontinuity Information Table
        SIT  = 0x001F,              // DVB Selection Information Table
        FILL = 0x1FFF               // Null Packet
    };

    /**
     * Map standard PID values to abbreviated table name
     */
    extern std::map<unsigned int,const char*> PIDMap;
}
