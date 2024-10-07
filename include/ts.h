#pragma once
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include "pid.h"

namespace ts
{
    /**
     * Struct representing MPEG-TS packet header
     */
    struct Packet
    {
        uint8_t sync;       // Synchronisation Byte (0x47, "G")
        bool    tei;        // Transport Error Indicator Flag
        bool    pusi;       // Payload Unit Start Indicator Flag
        bool    pri;        // Transport Priority Flag
        uint    pid;        // Packet Identifier
        uint    tsc;        // Transport Scrambling Control
        uint    afc;        // Adaptation Field Control
        uint    cont;       // Continuity Counter
        uint8_t data[184];  // Payload data
    };

    /**
     * MPEG Transport Stream with DVB Extensions (ETSI EN 300 468)
     */
    class Mux
    {
    public:
        Mux();
        ~Mux();

        uint8_t tsid;                               // Transport Stream ID
        std::map<uint,unsigned long> pid_count;     // Number of packets per PID
        std::vector<uint> pid_filter;               // List of PIDs to process

        /**
         * Process new packet
         */
        bool push(char* bytes, ts::Packet& p);

    private:
        /**
         * Unpack 188 byte buffer into Packet struct
         */
        Packet unpack_bytes(char* buffer);
    };

    /**
     * Print packet information
     */
    std::string packet_info(Packet& p);
} // namespace ts
