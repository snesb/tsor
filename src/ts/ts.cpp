#include <algorithm>
#include <cstring>
#include <iomanip>
#include <stdexcept>
#include <string>

#include "ts.h"

namespace ts
{
    Mux::Mux() {}
    Mux::~Mux() {}

    Packet Mux::unpack_bytes(char* buffer)
    {
        // Ideally ts::Packet would be a packed struct with bit widths but 
        // that's not portable so we'll have to put up with masks and bit-shifting
        Packet packet
        {
            static_cast<uint8_t>(buffer[0]),                                // Synchronisation Byte (0x47, "G")
            static_cast<bool>((buffer[1] & 0x80) >> 7),                     // Transport Error Indicator Flag
            static_cast<bool>((buffer[1] & 0x40) >> 6),                     // Payload Unit Start Indicator Flag
            static_cast<bool>((buffer[1] & 0x20) >> 5),                     // Transport Priority Flag
            static_cast<uint16_t>(((buffer[1] << 8) | buffer[2]) & 0x1FFF), // Packet Identifier
            static_cast<uint>((buffer[3] & 0xC0) >> 6),                     // Transport Scrambling Control
            static_cast<uint>((buffer[3] & 0x30) >> 4),                     // Adaptation Field Control
            static_cast<uint>((buffer[3] & 0x0F)),                          // Continuity Counter
            ""                                                              // Payload
        };

        // Copy payload into struct memory
        std::memcpy(packet.data, &buffer[4], 184);

        return packet;
    }

    bool Mux::push(char* buffer, ts::Packet& p)
    {
        // Check sync byte is present
        if (buffer[0] != 0x47)
            throw std::runtime_error("Packet missing sync byte (0x47)");

        // Unpack packet bytes
        p = unpack_bytes(buffer);
        pid_count[p.pid]++;

        // Skip null or corrupt packets
        if (p.pid == PID::FILL || p.tei) return false;

        // Skip packets not matching PID filter
        return is_filtered(p.pid);
    }

    bool Mux::is_filtered(uint16_t pid)
    {
        return pid_filter.empty() || (!pid_filter.empty() && std::find(pid_filter.begin(), pid_filter.end(), pid) != pid_filter.end());
    }

    std::string packet_info(Packet& p)
    {
        std::stringstream s;

        // Continuity Counter
        s << std::uppercase << std::hex << p.cont << " ";

        // Packet Identifier
        s << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << p.pid << " ";
        if (PIDMap.find(p.pid) != PIDMap.end())
            s << std::setfill(' ') << std::setw(8) << std::left << PIDMap[p.pid];
        else
            s << std::setfill(' ') << std::setw(8) << " ";

        // Packet Flags
        s << (p.tei ? "TEI " : "");
        s << (p.pusi ? "PUSI " : "" );
        s << (p.pri ? "PRI" : "");

        // Transport Scrambling Control
        if (p.tsc == 2 || p.tsc == 3) s << "TSC ";

        // Adaptation Field Control
        if (p.afc == 2 || p.afc == 3) s << "AFC ";

        return s.str();
    }
} // namespace ts
