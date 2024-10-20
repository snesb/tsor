#pragma once
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include "pid.h"
#define TS_BUFFER_LENGTH 188

namespace ts
{
    /**
     * Struct representing MPEG-TS packet header
     */
    struct Packet
    {
        uint8_t     sync;       // Synchronisation Byte (0x47, "G")
        bool        tei;        // Transport Error Indicator Flag
        bool        pusi;       // Payload Unit Start Indicator Flag
        bool        pri;        // Transport Priority Flag
        uint16_t    pid;        // Packet Identifier
        uint        tsc;        // Transport Scrambling Control
        uint        afc;        // Adaptation Field Control
        uint        cont;       // Continuity Counter
        uint8_t     data[184];  // Payload
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
        std::map<uint16_t,unsigned long> pid_count;     // Number of packets per PID
        std::vector<uint16_t> pid_filter;               // List of PIDs to process

        /**
         * Process new packet
         */
        bool push(char* bytes, ts::Packet& p);

        /**
         * Returns true if PID is in filter list (or if filter list is empty)
         */
        bool is_filtered(uint16_t pid);

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
