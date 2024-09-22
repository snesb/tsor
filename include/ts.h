#pragma once
#include <map>

/**
 * MPEG Transport Stream with DVB Extensions (ETSI EN 300 468)
 */
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
     * Map standard table names to PID value
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
    static std::map<unsigned int,const char*> PIDMap = {
        {PID::PAT,  "PAT"},         // Program Association Table
        {PID::CAT,  "CAT"},         // Conditional Access Table
        {PID::TSDT, "TSDT"},        // Transport Stream Description Table
        {PID::IPMP, "IPMP"},        // Intellectual Property Management and Protection
        {PID::NIT,  "NIT"},         // DVB Network Information Table
        {PID::SDT,  "SDT/BAT"},     // DVB Service Description Table or DVB Bouquet Association Table
        {PID::EIT,  "EIT"},         // DVB Event Information Table
        {PID::RST,  "RST"},         // DVB Running Status Table
        {PID::TDT,  "TDT/TOT"},     // DVB Time and Date Table or DVB Time Offset Table
        {PID::NSYN, "NSYN"},        // DVB Network Synchronisation
        {PID::RNT,  "RNT"},         // DVB Resolution Authority Record (RAR) Notification Table
        {PID::LLIS, "LLIS"},        // DVB Link-local Inband Signalling
        {PID::MEAS, "MEAS"},        // DVB Measurement
        {PID::DIT,  "DIT"},         // DVB Discontinuity Information Table
        {PID::SIT,  "SIT"},         // DVB Selection Information Table
        {PID::FILL, "FILL"}         // Null Packet
    };
}
